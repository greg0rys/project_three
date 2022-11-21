#include "driver.h"
// section menu
void menu()
{
    table theTable;
    int option = 0;

    do {
        cout <<  " ****** MENU: ****** " << endl;
        cout << "1.\t Add a Website to the table." << endl
             << "2.\t Get all Websites based on a topic" << endl
             << "3.\t Modify the Rating and Review for of a Website" << endl
             << "4.\t Remove all Websites with a 1-Star rating" << endl
             << "5.\t Display Websites by Topic" << endl
             << "6.\t Display [ all ] Websites in the table" << endl
             << "7.\t Display the length of each chain in the table" << endl
             << "8.\t Quit the program" << endl;
        cout << "Please enter a menu choice [EX 1. to add]: ";
        option = getInteger();
        menuOperations(theTable, option);

    } while(option != 8);
}


void menuOperations(table &aTable, const int operationNo)
{
    website aWebsite;
    char * topicName = nullptr;
    char * siteURL = nullptr;
    website * searchResults = nullptr;
    int totalRemoved;
    int totalCounted;
    int * chainLengths = nullptr;;

    int numResults = 0;
    switch(operationNo)
    {
        case 1:
            generateWebsite(aWebsite);
            if(aTable.insert(aWebsite))
                cout << "Success added: " << aWebsite.getURL() << endl;
            cout << aTable.getSize() << endl;
            break;

        case 2:
            cout << "Please enter the name of the topic to display: ";
            getInput(topicName);
            if(aTable.retrieve(topicName, searchResults, numResults))
            {
                cout << "****** FOUND " << numResults << " ****** " << endl
                     << "\t For Topic: " << topicName << endl;
                displaySearchResults(searchResults, numResults);

            }
            if(topicName)
                delete []topicName;
            topicName = nullptr;
            break;

        case 3:
            generateSearchSite(aWebsite);
            if(aTable.edit(aWebsite))
            {
                cout << "Success " << aWebsite.getURL() << " was updated "
                     << endl << "\tNew Rating: " << aWebsite.getRating()
                     << endl << "\tNew Review: \n\t" << aWebsite.getReview()
                     << endl;
            }
            else
            {
                cout << "No matching Website found!" << endl;
            }
            break;

        case 4:
            cout << "Removing all Websites with a rating of 1 " << endl;
          totalRemoved =  aTable.remove();
          cout << (totalRemoved == 0 ? "There were no Websites rated 1. "
                                       "\tRemoved: " :
          "Removed: ") << (totalRemoved == 0 ? 0 : totalRemoved) << endl;
          break;


        case 5:
            cout << "Which topic would you like to see the websites for? ";
            getInput(topicName);
            if(!aTable.printByTopic(topicName))
                cout << "No Websites found for topic name: " << topicName <<
                endl;
            break;

        case 6:
            cout << aTable << endl;
            break;
        case 7:
            totalCounted = aTable.getTotalChains();
            aTable.chainLength(chainLengths);
            cout << "There are " << totalCounted
                 << " Chains in the table " << endl;
            for(auto i = 1; i <= totalCounted; i++)
            {
                cout << "Chain #" << i << endl
                     << "\t Length: " << chainLengths[i - 1]
                     << endl;
            }

            // we only have to delete the pointer to the array
            // because this array doesn't hold pointers it only holds
            // regular ints so we don't have to free the items in the array.
            if(chainLengths)
                delete []chainLengths;
            break;

        case 8:
            cout << "Thanks for using me! Goodbye!" << endl;
            break;
        default:
            cout << "That is not a valid menu choice try again " << endl;



    }
}


void displaySearchResults( website * results,const int numResults)
{
    for(auto i = 0; i < numResults; i++)
    {
        cout << results[i] << endl;
    }
}


void generateWebsite(website & aSite)
{
    typedef char* charC;
    charC topicName = nullptr;
    charC siteURL = nullptr;
    charC siteSummary = nullptr;
    charC siteReview = nullptr;
    int siteRating;

    cout << "Please enter the topic of the website [EX Programming]: ";
    getInput(topicName);
    cout << "Please enter the URL of the website [ LIKE https://www.google"
         << ".com ]: ";
    getInput(siteURL);
    cout << "Please enter a summary of this websites content: [ Like Linked "
         <<   "Lists ]: ";
    getInput(siteSummary);
    cout << "Please enter a review for this website [ EX How helpful you find"
         <<  " this websites content ]: ";
    getInput(siteReview);
    cout << "How would you rate this website on a scale of 5 [ EX 1 not very"
         <<   " helpful 5 very helpful ]: ";
    siteRating = getInteger();

    // set the website up.
    aSite.setTopic(topicName);
    aSite.setURL(siteURL);
    aSite.writeSummary(siteSummary);
    aSite.writeReview(siteReview);
    aSite.setRating(siteRating);

    if(topicName)
        delete []topicName;
    if(siteURL)
        delete []siteURL;
    if(siteSummary)
        delete [] siteSummary;
    if(siteReview)
        delete []siteReview;
    topicName = siteURL = siteSummary = siteReview = nullptr;
}


void generateSearchSite(website &aSite)
{
    typedef char* charC;
    charC topicName = nullptr;
    charC siteReview = nullptr;
    charC siteURL = nullptr;
    int newRating;

    cout << "Please enter the topic of the website to search for: ";
    getInput(topicName);
    cout << "Please enter the URL of the website to search for: ";
    getInput(siteURL);
    cout << "Please enter your new review for the website: ";
    getInput(siteReview);
    cout << "Please enter your new rating for the website on a scale of 5: ";
    newRating = getInteger();

    aSite.setTopic(topicName);
    aSite.setURL(siteURL);
    aSite.writeReview(siteReview);
    aSite.setRating(newRating);

    if(topicName)
        delete []topicName;
    if(siteReview)
        delete []siteReview;
    if(siteURL)
        delete []siteURL;
    topicName = siteReview = siteURL = nullptr;


}







/*
 * Get character input dynamically from the user.
 */
void getInput(char *& chars) {
    char *input = nullptr; // store the input from input stream
    char *temp = nullptr; // store a temp copy of the input each time input
    // grows
    int wordSize = 0; // the size of the word we got from input

    while (cin.good() && cin.peek() != EOF) {
        char letter = cin.get(); // grabs the current character in the input

        if (letter == '\n') {
            break;
        }

        if (!input) {
            input = new char[2];
            input[0] = letter;
            input[1] = '\0';
            wordSize = 1;
        } else {
            wordSize = strlen(input);
            // +2 so we can get the letter plus have enough space to append
            // cstrings null node terminator to the end.
            temp = new char[wordSize + 2]{0};
            strcpy(temp, input);
            delete[]input;
            input = temp;
            input[wordSize] = letter;
            input[wordSize + 1] = '\0';
        }

    }


    chars = new char[strlen(input) + 1];
    // copy the users input into the passed in pointer.
    strcpy(chars, input);
    // get rid of our dynamic input pointer.
    delete[]input;
}


/*
 * Get validated integer input from the user. ensures that the user did in
 * fact enter a valid input, or it will keep prompting until satisfied.
 */
int getInteger()
{
    int numberIn;
    cin >> numberIn;

    while(cin.fail())
    {
        cin.clear();
        cin.ignore(101, '\n');
        cerr << "Please enter a whole number as input "
             << "(EX 100 or 379) " << endl;
        cin >> numberIn;
    }

    cin.ignore(101, '\n');
    return numberIn;
}





int main()
{
    cout << "Starting the program " << endl;
    menu();
}