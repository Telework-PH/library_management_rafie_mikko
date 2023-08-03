#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <algorithm>

#include "../includes/Helper.h"
#include "../includes/Book.h"

using namespace std;

string bookDataPath = "data/Books.csv";
vector<string> headerBook = {"ID", "Title", "Author", "Publisher", "Genre", "Status"};
vector<Book> books;

void refreshBook() {
    books.clear();
    ifstream bookData(bookDataPath);
    int index = 1;
    while (bookData.peek() != EOF) {
        string id;
        string title;
        string author;
        string publisher;
        string genre;
        string status;
        getline(bookData, id, token);
        if (id == headerBook[0]) {
            bookData.ignore(INT_MAX, '\n');
            continue;
        }
        getline(bookData, title, token);
        getline(bookData, author, token);
        getline(bookData, publisher, token);
        getline(bookData, genre, token);
        getline(bookData, status, '\n');
        books.push_back({stoi(id), title, author, publisher, genre, status});
    }
    bookData.close();
    sortBook();
}

void menuBook() {
    clear();
    cout << cMargin << "BOOK MANAGEMENT //" << endl << endl;
    printBook();
    cout << endl << endl
         << "\t0 Main menu" << endl
         << "\t1 Add" << endl
         << "\t2 Update" << endl
         << "\t3 Remove" << endl
         << "\t4 Search" << endl
         << "\t5 Refresh" << endl;
    int navigate = getInput("\t> ", "\tx Not in choices.", 0, 5, true);
    cout << endl << endl;
    if (navigate == 0) {
        return;
    } else if (navigate == 1) {
        addBook();
    } else if (navigate == 2) {
        if (books.size() > 0)
            updateBook();
        else
            cout << "\tx No records to update." << endl;
    } else if (navigate == 3) {
        if (books.size() > 0)
            removeBook();
        else
            cout << "\tx No records to remove." << endl;
    } else if (navigate == 4) {
        searchBook("");
    } else if (navigate == 5) {
//            refresh();
    }
    menuBook();
}

void addBook() {
    cout << "\tAdd Book" << endl << endl;
    int id = sloppyUUID();
    if (idExistBook(id)) {
        addBook();
        return;
    }
    string title = getInput("\tTitle : ", "\tx Input should not include semicolon (;).", to_string(token));
    string author = getInput("\tAuthor : ", "\tx Input should not include semicolon (;).", to_string(token));
    string publisher = getInput("\tPublisher : ", "\tx Input should not include semicolon (;).", to_string(token));
    string genre = getInput("\tGenre : ", "\tx Input should not include semicolon (;).", to_string(token));
    cout << endl
         << "\tWould you like to commit this action?" << endl
         << "\t1 Yes" << endl
         << "\t2 No" << endl;
    int action = getInput("\t> ", "\tx Not in choices.", 1, 2, true);
    if (action == 1) {
        books.push_back({id, title, author, publisher, genre, "Available"});
        sortBook();
        saveBook();
    }
}

// TODO: add choices if they want to update all or specified info
void updateBook() {
    cout << "\tUpdate Book" << endl << endl;
    int id = getInput("\tID : ",
                      "\tx The input you entered may contain invalid characters or is too large.",
                      1, INT_MAX, true);
    if (!idExistBook(id)) {
        cout << "\tx Action failed. The book with ID " << id << " was not found." << endl << endl;
        pause();
        return;
    }
    if (getBook(id).Status != "Available") {
        cout << endl << "\tx Action failed. The book is currently not available." << endl << endl;
        pause();
        return;
    }
    cout << endl << "\tEnter new info" << endl;
    string title = getInput("\tTitle : ", "\tx Input should not include semicolon (;).", to_string(token));
    string author = getInput("\tAuthor : ", "\tx Input should not include semicolon (;).", to_string(token));
    string publisher = getInput("\tPublisher : ", "\tx Input should not include semicolon (;).", to_string(token));
    string genre = getInput("\tGenre : ", "\tx Input should not include semicolon (;).", to_string(token));
    cout << endl
         << "\tWould you like to commit this action?" << endl
         << "\t1 Yes" << endl
         << "\t2 No" << endl;
    int action = getInput("\t> ", "\tx Not in choices.", 1, 2, true);
    if (action == 1) {
        for (Book &book : books) {
            if (book.ID == id) {
                book.Title = title;
                book.Author = author;
                book.Publisher = publisher;
                book.Genre = genre;
                break;
            }
        }
        sortBook();
        saveBook();
    }
}

void removeBook() {
    cout << "\tRemove Book" << endl << endl;
    int id = getInput("\tID : ",
                      "\tx The input you entered may contain invalid characters or is too large.",
                      1, INT_MAX, true);
    if (!idExistBook(id)) {
        cout << "\tx Action failed. The book with ID " << id << " was not found." << endl << endl;
        pause();
        return;
    }
    if (getBook(id).Status != "Available") {
        cout << endl << "\tx Action failed. The book is currently not available." << endl << endl;
        pause();
        return;
    }
    cout << endl
         << "\tWould you like to commit this action?" << endl
         << "\t1 Yes" << endl
         << "\t2 No" << endl;
    int action = getInput("\t> ", "\tx Not in choices.", 1, 2, true);
    if (action == 1) {
        for (Book &book : books) {
            if (book.ID == id) {
                book.Status = "Removed";
                break;
            }
        }
        sortBook();
        saveBook();
    }
}

void searchBook(string key = "") {
    clear();
    cout << cMargin << "Search Book" << endl << endl;
    key = toLow(key);
    if (key == "") {
        printBook();
    } else {
        vector<Book> list;
        for (const Book &book : books) {
            if (toLow(to_string(book.ID)).find(key) != string::npos ||
                toLow(book.Title).find(key) != string::npos ||
                toLow(book.Author).find(key) != string::npos ||
                toLow(book.Publisher).find(key) != string::npos ||
                toLow(book.Genre).find(key) != string::npos ||
                toLow(book.Status).find(key) != string::npos)
                list.push_back(book);
        }
        cout << cMargin << "Search results for '" << key << "'" << endl << endl;
        printBook(list);
    }
    cout << endl << endl << "\tEnter ;back to go back" << endl;
    key = getInput("\tSearch key : ", "\tx Unknown Error.", "");
    if (toLow(key) == ";back")
        return;
    else
        searchBook(key);
}

void saveBook() {
    ofstream bookData(bookDataPath);
    for (const string &header : headerBook)
        bookData << header << (&header - headerBook.data() != headerBook.size() - 1 ? token : '\n');
    for (const auto &book : books)
        bookData << book.ID << token
                 << book.Title << token
                 << book.Author << token
                 << book.Publisher << token
                 << book.Genre << token
                 << book.Status << (&book - books.data() != books.size() - 1 ? "\n" : "");
    bookData.close();
}

void sortBook() {
    sort(books.begin(), books.end(), [](const Book b1, const Book b2) { return b1.Title < b2.Title; } );
}

void printBook() {
    refreshBook();
    printBook(books);
}

void printBook(vector<Book> data) {
    ifstream bookData(bookDataPath);
    if (!bookData.is_open()) {
        cout << cMargin << "Book database file is missing." << endl << endl
             << cMargin << "A new one will be generated after adding an entry." << endl;
        return;
    }
    if (data.size() == 0) {
        cout << cMargin << "No records found." << endl;
        return;
    }
    int index = 1;
    cout << left << setw(5) << " "
         << setw(10)<< headerBook[0] << " "
         << setw(50)<< headerBook[1] << " "
         << setw(30)<< headerBook[2] << " "
         << setw(20)<< headerBook[3] << " "
         << setw(20)<< headerBook[4] << " "
         << setw(20)<< headerBook[5] << endl;
    cout << cMargin << string(155, '-') << endl;
    for (const auto &book : data)
        if (book.Status != "Removed")
            cout << left << setw(5) << index++
                 << setw(10) << book.ID << " "
                 << setw(50) << (book.Title.length() > 50 ? book.Title.substr(0, 47) + "..." : book.Title) << " "
                 << setw(30) << (book.Author.length() > 30 ? book.Author.substr(0, 27) + "..." : book.Author) << " "
                 << setw(20) << (book.Publisher.length() > 20 ? book.Publisher.substr(0, 17) + "..." : book.Publisher) << " "
                 << setw(20) << (book.Genre.length() > 20 ? book.Genre.substr(0, 17) + "..." : book.Genre) << " "
                 << setw(20) << book.Status
                 << endl << (&book - data.data() != data.size() - 1 ? "\n" : "");
}

vector<Book> underStatusBook(string filter) {
    vector<Book> ret;
    for (auto book : books)
        if (book.Status == filter) ret.push_back(book);
    sort(ret.begin(), ret.end(), [](const Book b1, const Book b2) { return b1.Title < b2.Title; } );
    return ret;
}

Book getBook(int id) {
    refreshBook();
    for (auto book : books) {
        if (book.ID == id)
            return book;
    }
    return Book();
}

bool idExistBook(int id) {
    refreshBook();
    for (auto book : books)
        if (book.ID == id)
            return true;
    return false;
}
