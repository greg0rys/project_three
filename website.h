#include <iostream>
#include <cctype>
#include <cstring>

using namespace std;

class website {
public:
    website();
    website(char *,char *, char *, char *, int); //topic,url,summ,review,rating
    website(const website &);
    // constructor for
    // searching
    ~website();
    website& operator=(const website &);
    friend ostream& operator<<(ostream &, website &);
    void setTopic(const char *);
    void setURL(const char *);
    void writeSummary(const char *);
    void writeReview(const char *);
    void setRating(const int);
    int getRating() const;
    char * getTopic() const;
    char * getURL() const;
    char * getSummary() const;
    char * getReview() const;
    bool isOneStar() const;

private:
    char * topic;
    char * URL;
    char * summary;
    char * review;
    int    rating;

    void destroySelf();
};