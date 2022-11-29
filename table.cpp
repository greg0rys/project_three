#include "table.h"
/*
 * filename: table.cpp
 * the implementation file for table.h includes all operations
 * a table must have to successfully store websites.
 */



// class default constructor
table::table()
{
    hashTable = new node*[INIT_SIZE];
    currentCapacity = INIT_SIZE;
    size = 0;

    // the table is an array of node pointers so set each spot in the table
    // to nullptr first round.
    for(auto i = 0; i < INIT_SIZE; i++)
        hashTable[i] = nullptr;
}



// class copy constructor
table::table(const table &aTable): hashTable(nullptr), currentCapacity(0),
size(0)
{
    *this = aTable;
}



// class destructor
table::~table()
{
    destroy();
}




/*To destroy a hash table of linked lists we must delete both the linked
 * list itself, and then the pointer to the overall array.
 *
 *  destroy the pointers and the array
 * */
void table::destroy()
{
    // every hashTable[i] there is a head to
    // a linked list, and the destroy list function requires a head to a
    // linked list to delete the linked list
    for(auto i = 0; i < currentCapacity; i++)
    {
        destroyList(hashTable[i]);
    }

    // now that all the lists are gone delete the overall pointer to the
    // hashtable
    if(hashTable)
        delete []hashTable;
    hashTable = nullptr;
}




/*
 * use recursion to delete all the nodes in the list
 *
 */
void table::destroyList(node *& head)
{
    if(!head)
        return;

    destroyList(head->next);
    delete head; // deleting the node also deletes the data using the nodes
    // destructor no need to also delete head->data it causes mem leak.
    head = nullptr;
    size--;
}




/*
 * When a table is being copied using the copy constructor or the = operator
 * we need to be aware that we have to copy the linked lists in the hash table
 * along with the lists pointers. so copying a hash table with chains is a
 * two part operation
 * @param sourceList the list we wish to copy
 * @param destinationList the list we are going to copy the value into.
 */
void table::copyList(node * sourceList, node * destinationList)
{
    // if the sourceList is null return
    if(!sourceList)
        return;

    // else recursively copy the next item in the list
    // we don't have to alter destination->next because we are copying the node,
    // so we will get a copy of its next pointer at each iter.
    //ie if there is a ->next on the node we are copying, the new node will
    // also have a copy of the ->next pointer.
    destinationList = new node(*sourceList->data);
    copyList(sourceList->next, destinationList->next);
}



// overloaded = operator to copy a table
const table& table::operator=(const table &aTable)
{
    // protect against self assignment.
    if(this == &aTable)
        return *this;

    // destroy this objects current hashtable and chains.
    if(hashTable)
        destroy();

    size = aTable.size;
    currentCapacity = aTable.currentCapacity;
    // recreate this objects has table to the size of the one we are copying.
    hashTable = new node*[currentCapacity];

    for(auto i = 0; i < currentCapacity; i++)
    {
        // set each chains head to nullptr first.
        hashTable[i] = nullptr;
        // then copy the values in.
        copyList(aTable.hashTable[i], hashTable[i]);
    }

    return *this;
}




// overload ostream operator display all data in the table
ostream& operator<<(ostream &out,  const table &aTable)
{
    if(aTable.isEmptyTable())
    {
        out << "The table has nothing to display currently!" << endl;
        return  out;
    }

    out << "Displaying the table.\t "
        <<  "[ " << aTable.getTotalChains() << "/"
        <<   aTable.getCapacity() << " in use ]" << endl;

    for(auto i = 0; i < aTable.getCapacity(); i++)
    {
        if(!aTable.isEmptyChain(i))
        {
            out << "Chain " << i << "\tLength: "
                << aTable.countChain(aTable.hashTable[i]) << endl;
            aTable.displayList(out, aTable.hashTable[i]);
            out << endl;
        }

    }

    return out;
}



int table::init(table & aTable)
{
    return loadFromFile("websites.txt", aTable);
}



int table::loadFromFile(const char *filename, table &aTable)
{

    fstream file(filename);
    int totalLoaded = 0;
    static int CALL_COUNT = 0;
    const int MAX_CHAR = 300;
    char topicName[MAX_CHAR];
    char siteURL[MAX_CHAR];
    char siteSummary[MAX_CHAR];
    char siteReview[MAX_CHAR];
    int siteRating;
    website siteData;

    if(CALL_COUNT  < 1)
    {
        cout << (!file ? "Error! " : " Success: ")
             << (!file ? "failed to open " : "loading from ")
             << filename << endl;
        CALL_COUNT++;
    }

    if(!file)
        return 0;

    file.get(topicName, MAX_CHAR, ';');

    while(!file.eof())
    {
        file.get(); // throw out the delim from the first file.get
        file.get(siteURL, MAX_CHAR, ';');
        file.get();
        file.get(siteSummary, MAX_CHAR, ';');
        file.get();
        file.get(siteReview, MAX_CHAR, ';');
        file.get();
        file >> siteRating;
        file.ignore(MAX_CHAR, '\n');

        siteData.setTopic(topicName);
        siteData.setURL(siteURL);
        siteData.writeSummary(siteSummary);
        siteData.writeReview(siteReview);
        siteData.setRating(siteRating);
        aTable.insert(siteData);
        totalLoaded++;

        file.get(topicName, MAX_CHAR, ';');
    }
    file.close();

    return totalLoaded;
}



// section displayList()
void table::displayList(ostream &out, node * listHead) const
{
    if(!listHead)
        return;

    out << *listHead->data << endl;
    displayList(out, listHead->next);
}



/*
 * Print a given chain based on a topic
 * @param topicName a char array with the topics name in it
 * @return null, prints the list at the given chain head.
 */
bool table::printByTopic(const char * topicName)
{
//    node * curr = nullptr;
    int index = hashIndex(topicName);

    if(isEmptyChain(index))
        return false;


    for(auto * curr=hashTable[index]; curr; curr = curr->next)
    {
        cout << *curr->data << endl;
    }
    return true;
}



/*
 * If our table reaches capacity grow by 3 to keep size prime
 * With growing a hash table, you cannot just simply grow it and then copy
 * the values in, because we changed the table size, we must rehash each
 * item in our old table to ensure that they get put into valid indexs. as
 * their old index was based of the previous size of the table. so (300%101)
 * != (300%303)
 */
void table::growTable()
{
    currentCapacity *= GROWTH_FACTOR;
    int newLocation;
    auto ** tempTable = new node*[currentCapacity];
    // before adding it to the temp table, we must first rehash since we are
    // growing. that way items are hashed correctly.
    for(auto i = 0; i < currentCapacity; i++)
    {
        newLocation = hashIndex(hashTable[i]->data->getTopic());
        tempTable[newLocation] = hashTable[i];
    }
    delete []hashTable;
    hashTable = tempTable;
}



/*
 * hash each items key to calculate its index in our hash table.
 * @param topicName - the key for each website, we are hashing based on
 * topicName as a key.
 * @return a legal index in our table for the item to be placed
 */
int table::hashIndex(const char * topicName) const
{
    int hashedIndex = 0;

    // use the whole key get each its char values into hashedIndex
    for(; *topicName != '\0'; topicName++)
        hashedIndex += *topicName;

    return (hashedIndex % currentCapacity);
}



/*
 * Get the tables current capacity.
 * @return an int representing the capacity of table.
 */
int table::getCapacity() const
{
    return currentCapacity;
}


/*
 * For this hash table size is = to the total number of items across all
 * chains.
 * @return the number of items in all the chains currently.
 */
int table::getSize() const
{
    return size;
}



/*
 * Get the total number of chains actually in use in our table currently.
 * @return int the total number of chains in use currently.
 */
int table::getTotalChains() const
{
    int totalChains = 0;
    if(isEmptyTable())
        return 0;

    for(auto i = 0; i < currentCapacity; i++)
    {
        if(countChain(hashTable[i]) > 0)
        {
            totalChains++;
        }
    }

    return totalChains;
}

/*
 * Insert a website to the proper chain by hashing its topic, and then
 * placing it at the head of the chain for that topic
 * @param aSite, the website we wish to add to the table.
 */
bool table::insert(const website &aSite)
{
    // a website must have a topic to be inserted.
    if(strcmp(aSite.getTopic(), "") == 0)
        return false;

    int index;
    node * newNode;
    if(size == currentCapacity)
        growTable();

    index = hashIndex(aSite.getTopic());
    newNode = new node(aSite);
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
    size++;
    return true;
}



/*
 * Get an array of websites based on a topic
 * @param topicName the topic name we wish to retrieve
 * @param website *& a pointer ref to the array that will hold the matches
 * @param int &totalMatches the total number of websites for a given topic
 * @return false if the table is empty or the chain we wish to retrieve is
 * empty
 * true if else.
 */
bool table::retrieve(const char * topicName, website *& matches , int &
totalMatches)
{
    if(isEmptyTable())
        return false;

    int searchLocation = hashIndex(topicName);

    if(isEmptyChain(searchLocation))
        return false;

    totalMatches = countChain(hashTable[searchLocation]);
    getSitesByTopic(hashTable[searchLocation], matches);

    return true;
}



/*
 * Edit a given websites review and rating
 * @param aSite, a reference to the size we wish to edit
 * @return false if the table is empty, true if we were able to edit the
 * website, or false if the website doesn't exist.
 */
bool table::edit( website &aSite)
{
    if(isEmptyTable())
        return false;

    if(getSiteAt(hashTable[hashIndex(aSite.getTopic())],aSite)) {
        return true;
    }

    return false;
}



/*
 * Remove all websites from the table that have a one star rating.
 * @return an int representing the number of items we deleted.
 */
int table::remove()
{
    int totalRemoved = 0;

    for(auto i = 0; i < currentCapacity; i++)
    {
        if(!isEmptyChain(i))
        {
            totalRemoved += removeFromChain(hashTable[i]);
        }
    }

    return totalRemoved;
}



/*
 * Recurse a given chain and remove every node from it that has a 1 star rating
 * @return an int representing the total number of nodes removed.
 */
int table::removeFromChain(table::node *&head)
{
    int totalRemoved = 0;
    node * temp;

    if(!head)
        return 0;

    if(head->data->getRating() == 1)
    {

        temp = head->next;
        delete head;
        head = temp;
        totalRemoved++;
        size--;

    }
    else
    {
        return totalRemoved + removeFromChain(head->next);

    }

    return totalRemoved;
}



/*
 * Edit a website at a given location by recursing the list to find it.
 * @param list the head to the list of the node we want to edit.
 * @param theSite is a reference to the updated info of this website, all it
 * has left to do is copy the original summary.
 * once we copy the summary from the site we will use the overloaded =
 * operator to set the current value of the website = to the updated value.
 * @return true if we were able to edit false if else.
 */
bool table::getSiteAt(table::node *list, website &theSite)
{

    // terminating step
    if(!list)
        return false;

    // base case - the site we want to edit.
    if(strcmp(theSite.getURL(),list->data->getURL()) == 0)
    {
        // theSite has all the new data for this website all thats left
        // is to copy the websites original summary as the user cannot edit it.
         theSite.writeSummary(list->data->getSummary());
         // make the current website = the the updated version.
         *list->data = theSite; // make use of websites overloaded = op
        return true;
    }

    return getSiteAt(list->next, theSite);

}



/*
 * Check a given index number to see if its chain is empty or not
 * @param location the location of the chain we wish to check
 * @return true if the chain @ location is empty; false if else.
 */
bool table::isEmptyChain(const int location) const
{
    return hashTable[location] == nullptr;
}



/*
 * Check if the table has no value stored in any chain
 * is size < 0 that means we have no items; as the size of this table = the
 * number of items in each chain.
 * @return true if there are <= 0 items false if > 0 items.
 */
bool table::isEmptyTable() const
{

    return size <= 0;
}



/*
 * Get an array of websites based on their topic
 * @param sourceList the list we wish to get an array of
 * @param matches a pointer ref to the array that will store the matches
 * found in the sourceList.
 */
void table::getSitesByTopic(table::node *sourceList, website *& matches)
{

    // we can simply delete the array if it isn't null pointer as the array
    // doesn't hold any pointers itself, the websites inside are not pointers
    // so it will not cause any segfaults this way. if this array had
    // pointers in it we would need to first delete each pointer THEN delete
    // the pointer to the actual array.
    if(matches)
        delete []matches;
    // make our matches array as big as the chain we are getting.
    matches = new website[countChain(sourceList)];
    int i = 0; // index for the matches array

    // store each website at the given link in the array.
     while(sourceList)
     {
         matches[i] = *sourceList->data;
         i++;
         sourceList = sourceList->next;
     }


}


/*
 * Get the length of any given chain recursively.
 * @param sourceList the list we wish to count
 * @return the total number of items in the given list.
 */
int table::countChain(table::node *sourceList) const
{
    if(!sourceList)
        return 0;

    return 1 + countChain(sourceList->next);
}


/*
 * Get the lengths of all in use chains in the table; This does not count
 * empty chains.
 * @param chainLengths a pointer ref to the array we are going to store each
 * index number of a given chain in.
 */
void table::chainLength(int *& chainLengths) const
{
    int counter = 0; // use counter to keep the array of chain lengths
    // start at 0 => totalChains
    //if we used I then the chainLengths array would have random indexes that
    // would be hard to loop through ie it could first item = 59
    if(chainLengths)
        delete []chainLengths;

    chainLengths = new int[getTotalChains()]; // make our lengths array as
    // big as the number of inuse chains in the table.

   for(auto i = 0; i < currentCapacity; i++)
   {
       // if the chain is empty, don't count it.
       if(!isEmptyChain(i))
       {
           // set chainLengths[i] = to the length of the current chain.
           chainLengths[counter] = countChain(hashTable[i]);
           counter++;
       }
   }
}




