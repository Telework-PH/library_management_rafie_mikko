#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>

#include "../includes/Helper.h"
#include "../includes/CheckOut.h"
#include "../includes/Book.h"
#include "../includes/Patron.h"

using namespace std;

string checkOutDataPath = "data/CheckOuts.csv";
vector<string> headerCheckOut = {"ID", "PatronID", "BookID", "Timestamp", "Duration", "Status"};
vector<CheckOut> checkouts;
extern vector<Book> books;
extern vector<Patron> patrons;

void refreshCheckOut() {
    checkouts.clear();
    ifstream checkOutData(checkOutDataPath);
    int index = 1;
    while (checkOutData.peek() != EOF) {
        string id;
        string patronID;
        string bookID;
        string dateTime;
        string duration;
        string status;
        getline(checkOutData, id, token);
        if (id == headerCheckOut[0]) {
            checkOutData.ignore(INT_MAX, '\n');
            continue;
        }
        getline(checkOutData, patronID, token);
        getline(checkOutData, bookID, token);
        getline(checkOutData, dateTime, token);
        getline(checkOutData, duration, token);
        getline(checkOutData, status, '\n');
        checkouts.push_back({stoi(id), stoi(patronID), stoi(bookID), stoi(dateTime), stoi(duration), status});
    }
    checkOutData.close();
    sortCheckOut();
}

void menuCheckOut() {
    clear();
    cout << cMargin << "CHECKOUT //" << endl << endl;
    printCheckOut();
    cout << endl << endl
         << "\t0 Main menu" << endl
         << "\t1 Borrow" << endl
         << "\t2 Return" << endl
         << "\t3 Search" << endl
         << "\t4 Refresh" << endl;
    int navigate = getInput("\t> ", "\tx Not in choices.", 0, 4, true);
    if (navigate == 0)
        return;
    else if (navigate == 1) {
        bool hasPatron = underStatusPatron("Active").size() > 0;
        bool hasBook = underStatusBook("Available").size() > 0;
        if (!hasPatron) {
            cout << endl << "\tx No active patrons found." << endl << endl;
            pause();
        }
        if (!hasBook) {
            cout << endl << "\tx No available books found." << endl << endl;
            pause();
        }
        if (hasPatron and hasBook)
            borrowCheckOut();
    } else if (navigate == 2) {
        returnCheckOut();
    } else if (navigate == 3) {
        searchCheckOut("");
    }
    menuCheckOut();
}

void borrowCheckOut() {
    clear();
    cout << cMargin << "BORROW BOOK //" << endl << endl;
    cout << cMargin << "Book List" << endl << endl;
    printBook(underStatusBook("Available"));
    cout << endl << endl;
    int bookID = getAvailableBook();
    cout << endl << endl
         << cMargin << "Patron list" << endl << endl;
    printPatron(underStatusPatron("Active"));
    cout << endl << endl;
    int patronID = getActivePatron();
    cout << endl;
    int duration = getInput("\tDuration (Day) : ",
                            "\tx Duration should be a numeric range between 1 and 31.",
                            1, 31, true);
    checkouts.push_back({sloppyUUID(), patronID, bookID, (int)time(0), duration, "Ongoing"});
    sortCheckOut();
    saveCheckOut();
    refreshBook();
    for (Book &book : books) {
        if (book.ID == bookID) {
            book.Status = "Borrowed";
            break;
        }
    }
    sortBook();
    saveBook();
}

int getAvailableBook() {
    int id = getInput("\tID : ",
                      "\tx The input you entered may contain invalid characters or is too large.",
                      1, INT_MAX, true);
    if (!idExistBook(id)) {
        cout << "\tx Action failed. The book with ID " << id << " was not found." << endl;
        return getAvailableBook();
    }
    if (getBook(id).Status != "Available") {
        cout << "\tx Action failed. The book is currently not available." << endl;
        return getAvailableBook();
    }
    return id;
}

int getActivePatron() {
    int id = getInput("\tID : ",
                      "\tx The input you entered may contain invalid characters or is too large.",
                      1, INT_MAX, true);
    if (!idExistPatron(id)) {
        cout << "\tx Action failed. The book with ID " << id << " was not found." << endl;
        return getActivePatron();
    }
    if (getPatron(id).Status == "Deactivated") {
        cout << endl << "\tx Action failed. This patron account is already deactivated." << endl;
        return getActivePatron();
    }
    return id;
}

void returnCheckOut() {
    cout << "\tReturn Book" << endl << endl;
    int id = getInput("\tID : ",
                      "\tx The input you entered may contain invalid characters or is too large.",
                      1, INT_MAX, true);
    if (!idExistCheckOut(id)) {
        cout << "\tx Action failed. The checkout with ID " << id << " was not found." << endl << endl;
        pause();
        return;
    }
    if (getCheckOut(id).Status == "Settled") {
        cout << endl << "\tx Action failed. This checkout is already settled." << endl << endl;
        pause();
        return;
    }
    cout << endl
         << "\tWould you like to commit this action?" << endl
         << "\t1 Yes" << endl
         << "\t2 No" << endl;
    int action = getInput("\t> ", "\tx Not in choices.", 1, 2, true);
    if (action == 1) {
        for (CheckOut &checkOut: checkouts) {
            if (checkOut.ID == id) {
                checkOut.Status = "Settled";
                break;
            }
        }
        sortCheckOut();
        saveCheckOut();
        refreshBook();
        for (Book &book : books) {
            if (book.ID == getCheckOut(id).BookID) {
                book.Status = "Available";
                break;
            }
        }
        sortBook();
        saveBook();
    }
}

//todo
void searchCheckOut(string key = "") {
    clear();
    cout << cMargin << "Search CheckOut" << endl << endl;
    key = toLow(key);
    if (key == "") {
        printCheckOut();
    } else {
        vector<CheckOut> list;
        for (const CheckOut &checkOut : checkouts) {
            if (to_string(checkOut.ID).find(key) != string::npos ||
                toLow(to_string(checkOut.PatronID)).find(key) != string::npos ||
                toLow(to_string(checkOut.BookID)).find(key) != string::npos ||
                toLow(formatUNIX(checkOut.DateTime)).find(key) != string::npos ||
                toLow(to_string(checkOut.Duration)).find(key) != string::npos ||
                toLow(checkOut.Status).find(key) != string::npos)
                list.push_back(checkOut);
        }
        cout << cMargin << "Search results for '" << key << "'" << endl << endl;
        printCheckOut(list);
    }
    cout << endl << endl << "\tEnter ;back to go back" << endl;
    key = getInput("\tSearch key : ", "\tx Unknown Error.", "");
    if (key == ";back")
        return;
    else
        searchCheckOut(key);
}

void saveCheckOut() {
    ofstream checkOutData(checkOutDataPath);
    for (const string &header : headerCheckOut)
        checkOutData << header << (&header - headerCheckOut.data() != headerCheckOut.size() - 1 ? token : '\n');
    for (const auto &checkout : checkouts)
        checkOutData << checkout.ID << token
                     << checkout.PatronID << token
                     << checkout.BookID << token
                     << checkout.DateTime << token
                     << checkout.Duration << token
                     << checkout.Status << (&checkout - checkouts.data() != checkouts.size() - 1 ? "\n" : "");
    checkOutData.close();
}

void sortCheckOut() {
    sort(checkouts.begin(), checkouts.end(), [](const CheckOut c1, const CheckOut c2) { return c1.DateTime > c2.DateTime; } );
    sort(checkouts.begin(), checkouts.end(), [](const CheckOut c1, const CheckOut c2) { return c1.Status < c2.Status; } );
}

void printCheckOut() {
    refreshCheckOut();
    printCheckOut(checkouts);
}

void printCheckOut(vector<CheckOut> data) {
    ifstream checkOutData(checkOutDataPath);
    if (!checkOutData.is_open()) {
        cout << cMargin << "CheckOut database file is missing." << endl << endl
             << cMargin << "A new one will be generated after adding an entry." << endl;
        return;
    }
    if (data.size() == 0) {
        cout << cMargin << "No records found." << endl;
        return;
    }
    int index = 1;
    for (const auto &checkOut : data) {
        Patron patron = getPatron(checkOut.PatronID);
        Book book = getBook(checkOut.BookID);
        cout << left << setw(5) << index++ << "ID : "
             << setw(40) << checkOut.ID << endl
             << "     " << "Patron ID : "
             << setw(40-12) << patron.ID
             << " " << "Book ID : "
             << setw(40-10) << book.ID
             << " " << "Date Time : "
             << setw(30-12) << formatUNIX(checkOut.DateTime) << endl
             << "     " << "Name : "
             << setw(40-7) << (patron.Name.length() > 40-7 ? patron.Name.substr(0, 37-7) + "..." : patron.Name)
             << " " << "Title : "
             << setw(40-8) << (book.Title.length() > 40-8 ? book.Title.substr(0, 37-8) + "..." : book.Title)
             << " " << "Duration (Days) : "
             << setw(30) << checkOut.Duration << endl
             << "     " << "Email : "
             << setw(40-8) << (patron.Email.length() > 40-8 ? patron.Email.substr(0, 37-8) + "..." : patron.Email)
             << " " << "Author : "
             << setw(40-9) << (book.Author.length() > 40-9 ? book.Author.substr(0, 37-9) + "..." : book.Author)
             << " " << "Status : "
             << setw(30) << checkOut.Status
             << endl << (&checkOut - data.data() != data.size() - 1 ? "\n" : "");
    }
}

bool hasActiveTransaction(int patronID) {
    refreshCheckOut();
    for (const CheckOut &checkOut : checkouts)
        if (checkOut.PatronID == patronID and checkOut.Status == "Ongoing")
           return true;
    return false;
}

CheckOut getCheckOut(int id) {
    refreshCheckOut();
    for (auto checkout : checkouts)
        if (checkout.ID == id)
            return checkout;
    return CheckOut();
}

bool idExistCheckOut(int id) {
    refreshCheckOut();
    for (auto checkout : checkouts)
        if (checkout.ID == id)
            return true;
    return false;
}
