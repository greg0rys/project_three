#include "table.h"

// section default constructor
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


// section copy constructor
table::table(const table &aTable): hashTable(nullptr), currentCapacity(0),
size(0)
{
    *this = aTable;
}


// section destructor
table::~table()
{
    destroy();
}


// section destroy()
/*To destroy a hash table of linked lists we must delete both the linked
 * list itself, and then the pointer to the overall array.
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


// section destroyList()
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


// section copyList()
/*
 * When a table is being copied using the copy constructor or the = operator
 * we need to be aware that we have to copy the linked lists in the hash table
 * along with the lists pointers. so copying a hash table with chains is a
 * two part operation
 */
void table::copyList(node * sourceList, node * destinationList)
{
    // if the sourceList is null return
    if(!sourceList)
        return;

    // else recursivley copy the next item in the list
    destinationList = new node(*sourceList->data);
    copyList(sourceList->next, destinationList->next);
}


// section operator=
const table& table::operator=(const table &aTable)
{
    if(this == &aTable)
        return *this;
    if(hashTable)
        destroy();

    size = aTable.size;
    currentCapacity = aTable.currentCapacity;
    hashTable = new node*[currentCapacity];

    for(auto i = 0; i < currentCapacity; i++)
    {
        hashTable[i] = nullptr;
        copyList(aTable.hashTable[i], hashTable[i]);
    }

    return *this;
}


// section operator<<
ostream& operator<<(ostream &out,  const table &aTable)
{
    if(aTable.isEmptyTable())
    {
        out << "The table has nothing to display currently!" << endl;
        return  out;
    }

    out << "Displaying the table.\t There are currently " <<
    aTable.getTotalChains()
        << " chains present" << endl;
    for(auto i = 0; i < aTable.getCapacity(); i++)
    {
        out << "Chain " << i << "\tLength: " << aTable.countChain(aTable
        .hashTable[i]) << endl;
        aTable.displayList(out, aTable.hashTable[i]);
        out << endl;
    }

    return out;


}


// section displayList()
void table::displayList(ostream &out, node * listHead) const
{
    if(!listHead)
        return;
    out << *listHead->data << endl;
    displayList(out, listHead->next);
}

// mix this with retrieve so we can output the number found in driver.
bool table::printByTopic(const char * topicName)
{
   // the index of the topic name once hashed.
    if(isEmptyTable())
        return false;
    // get the index of the topic;
    int index = hashIndex(topicName);

    // if the chain is empty return false, there isn't anything to print
    if(isEmptyChain(index))
        return false;

    // loop through the length of the chain.
    for(auto i = 0; i < countChain(hashTable[index]); i++)
    {
        cout << *hashTable[index]->data << endl;
    }
    return true;
}




// section growTable()
// this cannot just grow it must rehash as well.
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

// section hashIndex()
int table::hashIndex(const char * topicName) const
{
    int hashedIndex = 0;

    // use the whole key get each its char values into hashedIndex
    for(; *topicName != '\0'; topicName++)
        hashedIndex += *topicName;

    return (hashedIndex % currentCapacity);
}


// section getcapacity()
int table::getCapacity() const
{
    return currentCapacity;
}

// section getSize()
int table::getSize() const
{
    return size;
}

// section getTotalChains
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


bool table::insert(const website &aSite)
{
    // a website must have a topic to be iserted.
    if(strcmp(aSite.getTopic(), "") == 0)
        return false;

    int index;
    node * newNode = nullptr;
    if(size == currentCapacity)
        growTable();

    index = hashIndex(aSite.getTopic());
    newNode = new node(aSite);
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
    size++;
    return true;
}

//section retrieve()
bool table::retrieve(const char * topicName, website matches[], int &
totalMatches)
{
    int searchLocation = hashIndex(topicName);
    if(isEmptyChain(searchLocation))
        return false;

    totalMatches = countChain(hashTable[searchLocation]);
    getSitesByTopic(hashTable[searchLocation], matches);

    return true;
}


// section edit()
bool table::edit( website &aSite)
{
    if(isEmptyTable())
        return false;

    if(getSiteAt(hashTable[hashIndex(aSite.getTopic())],aSite)) {
        return true;
    }

    return false;
}



// section remove()
// remove all websites that have a one star rating
int table::remove()
{
    return searchAllChains(1);
}


int table::searchAllChains( int targetRating)
{
    int totalRemoved = 0;

    for(auto i = 0; i < currentCapacity; i++)
        totalRemoved += removeFromChain(hashTable[i], hashTable[i],
                                        targetRating);

    return totalRemoved;

}


// section removeFromChain()
int table::removeFromChain(node *& head, node * prev,  int targetRating)
{
    static int totalRemoved = 0;
    node * curr = nullptr;
    if(!head)
        return 0;
    if(head->data->isOneStar())
    {
        curr = head;
        if(prev)
        {
            prev->next = curr->next;
            delete curr;
            head = prev;
            totalRemoved++;
            size--;

        }
        else
        {
            head = curr->next;
            delete curr;
            size--;
            totalRemoved++;
        }
    }

    return totalRemoved + removeFromChain(head->next, head, targetRating);
}



// section getSiteAt()
bool table::getSiteAt(table::node *list, website &theSite)
{

    if(!list)
        return false;
    if(strcmp(theSite.getURL(),list->data->getURL()) == 0)
    {
        theSite.writeSummary(list->data->getSummary());
         *list->data = theSite;
        return true;
    }

    return getSiteAt(list->next, theSite);

}


// section isEmpty()
bool table::isEmptyChain(const int location) const
{
    return hashTable[location] == nullptr;
}


// section isEmptyTable()
bool table::isEmptyTable() const
{

    return size < 0;
}

// section getSitesByTopic()
void table::getSitesByTopic(table::node *sourceList, website *matches)
{
    static int storage_idx = 0;
    if(!sourceList)
        return;
    matches[storage_idx] = *sourceList->data;
    storage_idx++;
    getSitesByTopic(sourceList->next, matches);
}


// section countChain()
int table::countChain(table::node *sourceList) const
{
    if(!sourceList)
        return 0;
    return 1 + countChain(sourceList->next);
}


void table::chainLength(int *& chainLengths) const
{
    int counter = 0;
    if(chainLengths)
        delete []chainLengths;

    chainLengths = new int[getTotalChains()];
   for(auto i = 0; i < currentCapacity; i++)
   {
       if(!isEmptyChain(i))
       {
           chainLengths[counter] = countChain(hashTable[i]);
           counter++;
       }
   }
}

