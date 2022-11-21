#include <iostream>
#include <cstring>
#include <cctype>
#include <fstream>
#include "website.h"
using namespace std;
/*
 * 1.	Constructor
2.	Destructor (deallocate the hash table)
3.	Insert a new website by topic (add website’s information into the hash table)
4.	Retrieve (retrieve all websites based on the topic keyword supplied)
a.	retrieve will need an array of websites and the number of matchings as arguments for the ADT to fill. It returns a success/fail flag for whether or not it found a match. Retrieve’s arguments might be something like:
bool retrieve(char * topic_keyword, website all_matches[], int & num_found);
b.	Retrieve should not correspond with the user (i.e., it should not prompt, echo, input, or output data).


5.	Edit (modify the review and rating for a particular topic and website match)
6.	Remove (remove all websites with a 1 star rating)
7.	Display (only displaying matches, based on the topic keyword)
8.	Display all
9.	Monitor the performance of hashing function by displaying each chain length
*/

class table {
	public:
		table(); // default constructor
		table(const table &); // copy constructor
		~table(); // class destructor
        const table& operator=(const table &aTable);
        friend ostream& operator<< (ostream &, const table &);

		bool insert(const website &); // this will call the private function
        // to hash
		bool retrieve(const char *, website[], int &); // this will call
        // private method
        // to search the lists
        bool edit(website &);
        int remove();
        bool printByTopic(const char *);
        // chains length
        bool isEmptyChain(const int location) const;
        bool isEmptyTable() const;
        int getTotalChains() const;
        void chainLength(int *&) const;
        int getCapacity() const;
        int getSize() const;


	private:
        struct node
        {
            website * data{};
            node * next;

            node():data(nullptr), next(nullptr){}

            node(const website &aWebsite)
            {
                data = new website(aWebsite);
                next = nullptr;
            }

            ~node()
            {
                if(data)
                    delete data;
                data = nullptr;
                next = nullptr;
            }

        };

        node ** hashTable;
        const static int INIT_SIZE = 101;
        const static int GROWTH_FACTOR = 3;
        int currentCapacity;
        int size;

        int hashIndex(const char *) const;
        void destroy();
        void destroyList(node *&);
        void copyList(node * sourceList, node * destinationList);
        void displayList(ostream &out, node * listHead) const;
        void growTable();
        void getSitesByTopic(node * sourceList, website matches[]);
        int countChain(node * sourceList) const;
        bool getSiteAt(node * list, website &theSite);
        int searchAllChains( int targetRating);

        int removeFromChain(node *&, node *, int);

};
