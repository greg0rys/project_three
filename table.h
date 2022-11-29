#include <iostream>
#include <cstring>
#include <cctype>
#include <fstream>
#include "website.h"
using namespace std;

/*
 * filename: table.h
 * the header file for table.cpp
 */

class table {
	public:
		table(); // default constructor
		table(const table &); // copy constructor
		~table(); // class destructor
        const table& operator=(const table &aTable);
        friend ostream& operator<< (ostream &, const table &);
        int init(table &);
        int loadFromFile(const char *, table &);
		bool insert(const website &); //
		bool retrieve(const char *, website *&, int &); // this will call
        bool edit(website &);
        int remove();
        bool printByTopic(const char *);
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
        void getSitesByTopic(node * sourceList, website *& matches);
        int countChain(node * sourceList) const;
        bool getSiteAt(node * list, website &theSite); // rename this to
        // edit site at
        int removeFromChain(node *&);

};
