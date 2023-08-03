#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <conio.h>

#include "includes/Helper.h"
#include "includes/Book.h"
#include "includes/Patron.h"
#include "includes/CheckOut.h"

using namespace std;

void art() {
    cPrint(".-~~~~~~~~~-._       _.-~~~~~~~~~-.");
    cPrint("__.'              ~.   .~              `.__");
    cPrint(".'//                  \\./                  \\\\`.");
    cPrint(".'//                     |                     \\\\`.");
    cPrint(".'// .-~\"\"\"\"\"\"\"~~~~-._     |     _,-~~~~\"\"\"\"\"\"\"~-. \\\\`.");
    cPrint(".'//.-\"   -------       `-.  |  .-'       -------   \"-.\\\\`.");
    cPrint(".'//______.============-..   \\ | /   ..-============.______\\\\`.");
    cPrint(".'______________________________\\|/______________________________`.");
}

void checkData() {
    bool isDataComplete = true;
    extern string checkOutDataPath;
    extern string bookDataPath;
    extern string patronDataPath;

    ifstream data(checkOutDataPath);
    if (!data.is_open()){
        cPrint("Checkout database file is missing.");
        isDataComplete = false;
    }
    data.close();
    
    data.open(bookDataPath);
    if (!data.is_open()) {
        cPrint("Book database file is missing.");
        isDataComplete = false;
    }
    data.close();
    
    data.open(patronDataPath);
    if (!data.is_open()) {
        cPrint("Patron database file is missing.");
        isDataComplete = false;
    }
    data.close();
    
    if (!isDataComplete)
        cout << endl << endl;
}

void mainMenu() {
    clear();
    cout << endl << endl << endl << endl << endl;
    art();
    cout << endl << endl;
    cPrint("Library Management");
    cout << endl << endl;
    checkData();
    cPrint("0 Exit          ");
    cPrint("1 Checkout      ");
    cPrint("2 Manage Books  ");
    cPrint("3 Manage Patrons");
    cPrint("4 Refresh       ");
    int navigate = getInput(string(82, ' ') + "> ", string(82, ' ') + "x Not in choices.", 0, 4, true);
    if (navigate == 0) {
        exit(0);
    } else if (navigate == 1) {
        refreshCheckOut();
        menuCheckOut();
    } else if (navigate == 2) {
        refreshBook();
        menuBook();
    } else if (navigate == 3) {
        refreshPatron();
        menuPatron();
    }
    mainMenu();
}

void login() {
    string const username="admin";
    string const password="admin";
    string getuser;
    string getpass;
    bool isCorrect = (username == getuser and password == getpass);
    bool firstBoot = true;
    while(!isCorrect) {
        getpass = "";
        char ch;
        clear();
        cout << endl << endl;
        cPrint("Log-in credentials");
        cout << endl << endl;
        if (!isCorrect and !firstBoot) {
            cPrint("Incorrect username or password");
            cout << endl << endl;
        }
        cPrint("Username:" );
        getuser = getInput(string(80, ' ') + "> ", "", "");
        cout << endl;
        cPrint("Password:" );
        cout << string(80, ' ') << "> ";
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!getpass.empty()) {
                    getpass.pop_back();
                    cout << "\b \b";
                }
            } else {
                getpass += ch;
                cout << '*';
            }
        }
        isCorrect = (username == getuser and password == getpass);
        firstBoot = false;
    }
    mainMenu();
}

int main() {
    this_thread::sleep_for(chrono::milliseconds(100));
    login();
    return 0;
}
