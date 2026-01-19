#include "Library.h"
#include <fstream>
#include <sstream>
#include <algorithm>
/* load the Books from the file (the DB) */
void Library::loadFromFile(const string& filename)
{
    books.clear();
    ifstream file(filename);
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr, title, author, statusStr;
        getline(ss, idStr, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, statusStr, '|');

        int id = stoi(idStr);
        bool borrowed = (statusStr == "1");
        books.emplace_back(id, title, author, borrowed);
    }
}

/* get books const */
const vector<Book>& Library::getBooks() const { return books; }

/* get books Mutable */
vector<Book>& Library::getBooksMutable() { return books; }

/* Remove books from the library */
void Library::removeBook(int bookId)
{
    books.erase(remove_if(books.begin(), books.end(),
                               [bookId](const Book& b) { return b.getId() == bookId; }), books.end());
}

/* Add books from the library */
void Library::addBook(const Book &book)
{
    books.push_back(book);
}

/* Save the books into the file (the DB) */
void Library::saveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    for (const auto& book : books)
    {
        file << book.getId() << "|"
             << book.getTitle() << "|"
             << book.getAuthor() << "|"
             << (book.getIsBorrowed() ? "1" : "0") << "\n";
    }
}
