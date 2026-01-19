#pragma once

#include <vector>
#include <string>
#include "Book.h"

using namespace std;

class Library
{
    public:
        void loadFromFile(const string& filename);
        void saveToFile(const string& filename) const;
        const vector<Book>& getBooks() const;     /* get books const */
        vector<Book>& getBooksMutable();          /* get books Mutable */
        void addBook(const Book& book);
        void removeBook(int bookId);



    private:
        vector<Book> books;
};



