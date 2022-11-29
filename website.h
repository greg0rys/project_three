#include <iostream>
#include <cctype>
#include <cstring>

using namespace std;

/*
 * filename: website.h
 * the header file for website.cpp
 */

class website {
public:
    // constructors
    website();
    website(const website &);
    ~website();

    // operator overlaods
    website& operator=(const website &);
    friend ostream& operator<<(ostream &, website &);

    // setters
    void setTopic(const char *);
    void setURL(const char *);
    void writeSummary(const char *);
    void writeReview(const char *);
    void setRating(const int);

    // getters
    int getRating() const;
    char * getTopic() const;
    char * getURL() const;
    char * getSummary() const;
    char * getReview() const;

private:
    char * topic;
    char * URL;
    char * summary;
    char * review;
    int    rating;

    void destroySelf(); // deallocate all pointers in use.
};