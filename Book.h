#ifndef BOOK_H
#define BOOK_H

#include <string>

using namespace std;

class Book
{
    public:
        Book(int id = 0, const string& title = "", const string& author = "", bool isBorrowed = false)
            : id(id), title(title), author(author), borrowed(isBorrowed) {}

        int getId() const { return id; }
        string getTitle() const { return title; }
        string getAuthor() const { return author; }
        bool getIsBorrowed() const { return borrowed; }
        void setTitle(const string& t)  { title = t; }
        void setAuthor(const string& a) { author = a; }
        void setBorrowed(bool b)             { borrowed = b; }

    private:
        int id;
        string title;
        string author;
        bool borrowed;
};

#endif
