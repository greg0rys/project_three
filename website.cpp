#include "website.h"

// section default constructor
website::website():topic(nullptr),URL(nullptr),summary(nullptr),review
(nullptr), rating(0){}

//section custom constructor
website::website(char * siteTopic, char * siteURL, char * siteSummary,
                 char * siteReview, int rating)
{
    this->topic = new char[strlen(siteTopic) + 1];
    strcpy(this->topic, siteTopic);

    this->URL = new char[strlen(siteURL) + 1];
    strcpy(this->URL, siteURL);

    this->summary = new char[strlen(siteSummary) + 1];
    strcpy(this->summary, siteSummary);

    this->review = new char[strlen(siteReview) + 1];
    strcpy(this->review, siteReview);

    this->rating = rating;
}

// section copy constructor
website::website(const website &aSite):topic(nullptr), URL(nullptr), summary
(nullptr),review(nullptr), rating(0)
{
    *this = aSite;
}



// section destructor
website::~website()
{
    destroySelf();
}

//section destroySelf()
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


// section setTopic()
void website::setTopic(const char * topicName)
{
    if(this->topic)
        delete []topic;

    this->topic = new char[strlen(topicName) + 1];
    strcpy(this->topic, topicName);
}


// section setURL()
void website::setURL(const char * urlInfo)
{
    if(this->URL)
        delete []URL;

    this->URL = new char[strlen(urlInfo) + 1];
    strcpy(this->URL, urlInfo);
}


// section writeSummary()
void website::writeSummary(const char * summaryInfo)
{
    if(this->summary)
        delete []summary;
    this->summary = new char[strlen(summaryInfo) + 1];
    strcpy(this->summary, summaryInfo);
}


// section writeReview()
void website::writeReview(const char * reviewInfo)
{
    if(this->review)
        delete []review;
    this->review = new char[strlen(reviewInfo) + 1];
    strcpy(this->review, reviewInfo);
}


// section setRating()
void website::setRating(const int newRating)
{
    this->rating = newRating;
}


// section getRating()
int website::getRating() const
{
    return this->rating;
}


// section getTopic()
char* website::getTopic() const
{
    return this->topic;
}

// section getURL()
char* website::getURL() const
{
    return this->URL;
}

// section getSummary()
char* website::getSummary() const
{
    return this->summary;
}

//section getReview()
char* website::getReview() const
{
    return this->review;
}


// section isOneStar()
/*
 * This will allow for a faster check when removing one star websites, instead
 * of having to get the sites rating through the get method and then
 * comparing it
 * we can simply just return a boolean check each time we need to delete one
 * star websites.
 */
bool website::isOneStar() const
{
    return (this->rating == 1);
}



// section operator=
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

// section operator<<
// this overloaded operator is operating under the assumption that none of
// the fields in the website will ever be null pointer
// if there was a case they could be nullptr we must check before printing it.
ostream& operator<<(ostream & out, website &aSite)
{
    out <<  aSite.URL << "\tRating: " << aSite.rating << endl;
    out << "Summary:\n\t" << aSite.summary << endl;
    out << "Review:\n\t" <<aSite.review << endl;

    return out;
}
