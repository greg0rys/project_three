#include "website.h"

/*
 * filename: website.cpp
 * the implementation file for website.h
 */

// class default constructor
website::website():topic(nullptr),URL(nullptr),summary(nullptr),review
(nullptr), rating(0){}



// class copy constructor
website::website(const website &aSite):topic(nullptr), URL(nullptr), summary
(nullptr),review(nullptr), rating(0)
{
    *this = aSite;
}



// class destructor
website::~website()
{
    destroySelf();
}

/*
 * destroy all pointers used by object at class destruction
 */
void website::destroySelf()
{
    if(topic)
        delete []topic;
    if(URL)
        delete []URL;
    if(summary)
        delete []summary;
    if(review)
        delete []review;

    // set all pointers to null pointer.
    topic = URL = summary = review = nullptr;
    rating = 0;
}


/*
 * Set the topic for a given website
 * @param topicName the new topic for this website.
 */
void website::setTopic(const char * topicName)
{
    if(this->topic)
        delete []topic;

    this->topic = new char[strlen(topicName) + 1];
    strcpy(this->topic, topicName);
}


/*
 * set the url of a given website
 * @param urlInfo the url we wish to set the website too.
 */
void website::setURL(const char * urlInfo)
{
    if(this->URL)
        delete []URL;

    this->URL = new char[strlen(urlInfo) + 1];
    strcpy(this->URL, urlInfo);
}


/*
 * Set the summary for a given website
 * @param summaryInfo the summary for the website.
 */
void website::writeSummary(const char * summaryInfo)
{
    if(this->summary)
        delete []summary;
    this->summary = new char[strlen(summaryInfo) + 1];
    strcpy(this->summary, summaryInfo);
}


/*
 * Set the review for a given website
 * @param reviewInfo the review for the website.
 */
void website::writeReview(const char * reviewInfo)
{
    if(this->review)
        delete []review;
    this->review = new char[strlen(reviewInfo) + 1];
    strcpy(this->review, reviewInfo);
}


/*
 * Set the rating for a given website
 * @param newRating the rating we are setting this website too.
 */
void website::setRating(const int newRating)
{
    this->rating = newRating;
}


/*
 * Get the websites rating
 * @return the rating of this website.
 */
int website::getRating() const
{
    return this->rating;
}


/*
 * Get the topic of website
 * @return a char * to this websites topic
 */
char* website::getTopic() const
{
    return this->topic;
}

/*
 * Get the URL of a website
 * @return a char pointer with the websites URL
 */
char* website::getURL() const
{
    return this->URL;
}

/*
 * Get the websites summary
 * @return a char pointer with the websites summary
 */
char* website::getSummary() const
{
    return this->summary;
}

/*
 * Get the review for the website
 * @return a char pointer with the websites review in it.
 */
char* website::getReview() const
{
    return this->review;
}



// overloaded operator=
website& website::operator=(const website &aSite)
{
    if(this == &aSite)
        return *this;

    destroySelf();

    if(!aSite.topic)
    {
        topic = nullptr;
    }
    else
    {
        topic = new char[strlen(aSite.topic) + 1];
        strcpy(topic, aSite.topic);
    }
    if(!aSite.URL)
    {
        URL = nullptr;
    }
    else
    {
        URL = new char[strlen(aSite.URL) + 1];
        strcpy(URL,aSite.URL);
    }
    if(!aSite.summary)
    {
        summary = nullptr;
    }
    else
    {
        summary = new char[strlen(aSite.summary) + 1];
        strcpy(summary, aSite.summary);
    }
    if(!aSite.review)
    {
        review = nullptr;
    }
    else
    {
        review = new char[strlen(aSite.review) + 1];
        strcpy(review, aSite.review);
    }

    rating = aSite.rating;

    return *this;

}

//  operator<<
// this overloaded operator is operating under the assumption that none of
// the fields in the website will ever be null pointer
// if there was a case they could be nullptr we must check before printing it.
ostream& operator<<(ostream & out, website &aSite)
{
    out <<  aSite.URL << "\tRating: " << aSite.rating
        << "\tTopic: " << aSite.getTopic() <<endl;
    out << "\tSummary:\n\t" << aSite.summary << endl;
    out << "\tReview:\n\t" <<aSite.review << endl;

    return out;
}
