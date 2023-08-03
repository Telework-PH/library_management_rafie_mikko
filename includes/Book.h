#ifndef _Books
#define _Books

#include <vector>

struct Book {
    int ID;
    string Title;
    string Author;
    string Publisher;
    string Genre;
    string Status;
};

void refreshBook();
void menuBook();
void addBook();
void updateBook();
void removeBook();
void searchBook(string key);
void saveBook();
void sortBook();
void printBook();
void printBook(vector<Book> data);
vector<Book> underStatusBook(string filter);
Book getBook(int id);
bool idExistBook(int id);

#endif
