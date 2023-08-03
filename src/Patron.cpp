#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <algorithm>

#include "../includes/Helper.h"
#include "../includes/Patron.h"
#include "../includes/CheckOut.h"

using namespace std;

string patronDataPath = "data/Patrons.csv";
vector<string> headerPatron = {"ID", "Name", "Email", "Phone", "Address", "Status"};
vector<Patron> patrons;

void refreshPatron() {
    patrons.clear();
    ifstream patronData(patronDataPath);
    int index = 1;
    while (patronData.peek() != EOF) {
        string id;
        string name;
        string email;
        string phone;
        string address;
        string status;
        getline(patronData, id, token);
        if (id == headerPatron[0]) {
            patronData.ignore(INT_MAX, '\n');
            continue;
        }
        getline(patronData, name, token);
        getline(patronData, email, token);
        getline(patronData, phone, token);
        getline(patronData, address, token);
        getline(patronData, status, '\n');
        patrons.push_back({stoi(id), name, email, phone, address, status});
    }
    patronData.close();
    sortPatron();
}

void menuPatron() {
    clear();
    cout << cMargin << "PATRON MANAGEMENT //" << endl << endl;
    printPatron();
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
        addPatron();
    } else if (navigate == 2) {
        if (patrons.size() > 0)
            updatePatron();
        else
            cout << "\tx No records to update." << endl;
    } else if (navigate == 3) {
        if (patrons.size() > 0)
            removePatron();
        else
            cout << "\tx No records to remove." << endl;
    } else if (navigate == 4) {
        searchPatron("");
    } else if (navigate == 5) {
//            refresh();
    }
    menuPatron();
}

void addPatron() {
    cout << "\tAdd Patron" << endl << endl;
    int id = sloppyUUID();
    if (idExistPatron(id)) {
        addPatron();
        return;
    }
    string name = getInput("\tName : ", "\tx Input should not include semicolon (;).", to_string(token));
    string email = getEmail("\tEmail : ", "\tx Invalid email format.");
    string phone = getPhone("\tPhone : ", "\tx Invalid phone number format. Should be a Philippine number starting with 09, consisting of 11 digits number.");
    string address = getInput("\tAddress : ", "\tx Input should not include semicolon (;).", to_string(token));
    cout << endl
         << "\tWould you like to commit this action?" << endl
         << "\t1 Yes" << endl
         << "\t2 No" << endl;
    int action = getInput("\t> ", "\tx Not in choices.", 1, 2, true);
    if (action == 1) {
        patrons.push_back({id, name, email, phone, address, "Active"});
        sortPatron();
        savePatron();
    }
}

// TODO: add choices if they want to update all or specified info
void updatePatron() {
    cout << "\tUpdate Patron" << endl << endl;
    int id = getInput("\tID : ",
                      "\tx The input you entered may contain invalid characters or is too large.",
                      1, INT_MAX, true);
    if (!idExistPatron(id)) {
        cout << "\tx Action failed. The patron with ID " << id << " was not found." << endl << endl;
        pause();
        return;
    }
    if (getPatron(id).Status == "Deactivated") {
        cout << endl << "\tx Action failed. This patron account is deactivated." << endl << endl;
        pause();
        return;
    }
    cout << endl << "\tEnter new info" << endl;
    string name = getInput("\tName : ", "\tx Input should not include semicolon (;).", to_string(token));
    string email = getEmail("\tEmail : ", "\tx Invalid email format.");
    string phone = getPhone("\tPhone : ", "\tx Invalid phone number format. Should be a Philippine number starting with 09, consisting of 11 digits number.");
    string address = getInput("\tAddress : ", "\tx Input should not include semicolon (;).", to_string(token));
    cout << endl
         << "\tWould you like to commit this action?" << endl
         << "\t1 Yes" << endl
         << "\t2 No" << endl;
    int action = getInput("\t> ", "\tx Not in choices.", 1, 2, true);
    if (action == 1) {
        for (Patron &patron : patrons) {
            if (patron.ID == id) {
                patron.Name = name;
                patron.Email = email;
                patron.Phone = phone;
                patron.Address = address;
                break;
            }
        }
        sortPatron();
        savePatron();
    }
}

void removePatron() {
    cout << "\tRemove Patron" << endl << endl;
    int id = getInput("\tID : ",
                      "\tx The input you entered may contain invalid characters or is too large.",
                      1, INT_MAX, true);
    if (!idExistPatron(id)) {
        cout << "\tx Action failed. The patron with ID " << id << " was not found." << endl << endl;
        pause();
        return;
    }
    if (getPatron(id).Status == "Deactivated") {
        cout << endl << "\tx Action failed. This patron account is already deactivated." << endl << endl;
        pause();
        return;
    }
    if (hasActiveTransaction(id)) {
        cout << endl << "\tx Action failed. This patron has an ongoing transaction." << endl << endl;
        pause();
        return;
    }
    cout << endl
         << "\tWould you like to commit this action?" << endl
         << "\t1 Yes" << endl
         << "\t2 No" << endl;
    int action = getInput("\t> ", "\tx Not in choices.", 1, 2, true);
    if (action == 1) {
        for (Patron &patron : patrons) {
            if (patron.ID == id) {
                patron.Status = "Deactivated";
                break;
            }
        }
        sortPatron();
        savePatron();
    }
}

void searchPatron(string key = "") {
    clear();
    cout << cMargin << "Search Patron" << endl << endl;
    key = toLow(key);
    if (key == "") {
        printPatron();
    } else {
        vector<Patron> list;
        for (const Patron &patron : patrons) {
            if (to_string(patron.ID).find(key) != string::npos ||
                toLow(patron.Name).find(key) != string::npos ||
                toLow(patron.Email).find(key) != string::npos ||
                toLow(patron.Phone).find(key) != string::npos ||
                toLow(patron.Address).find(key) != string::npos ||
                toLow(patron.Status).find(key) != string::npos)
                list.push_back(patron);
        }
        cout << cMargin << "Search results for '" << key << "'" << endl << endl;
        printPatron(list);
    }
    cout << endl << endl << "\tEnter ;back to go back" << endl;
    key = getInput("\tSearch key : ", "\tx Unknown Error.", "");
    if (key == ";back")
        return;
    else
        searchPatron(key);
}

void savePatron() {
    ofstream patronData(patronDataPath);
    for (const string &header : headerPatron)
        patronData << header << (&header - headerPatron.data() != headerPatron.size() - 1 ? token : '\n');
    for (auto patron : patrons)
        patronData << patron.ID << token
                   << patron.Name << token
                   << patron.Email << token
                   << patron.Phone << token
                   << patron.Address << token
                   << patron.Status << (&patron - patrons.data() != patrons.size() - 1 ? "\n" : "");
    patronData.close();
}

void sortPatron() {
    sort(patrons.begin(), patrons.end(), [](const Patron p1, const Patron p2) { return p1.Name < p2.Name; } );
}

void printPatron() {
    refreshPatron();
    printPatron(patrons);
}

void printPatron(vector<Patron> data) {
    ifstream patronData(patronDataPath);
    if (!patronData.is_open()) {
        cout << cMargin << "Patron database file is missing." << endl << endl
             << cMargin << "A new one will be generated after adding an entry." << endl;
        return;
    }
    if (data.size() == 0) {
        cout << cMargin << "No records found." << endl;
        return;
    }
    int index = 1;
    cout << left << setw(5) << " "
         << setw(10)<< headerPatron[0] << " "
         << setw(50)<< headerPatron[1] << " "
         << setw(30)<< headerPatron[2] << " "
         << setw(15)<< headerPatron[3] << " "
         << setw(30)<< headerPatron[4] << " "
         << setw(15)<< headerPatron[5] << endl;
    cout << cMargin << string(155, '-') << endl;
    for (const auto &patron : data)
        if (patron.Status != "Deactivated")
            cout << left << setw(5) << index++
                 << setw(10) << patron.ID << " "
                 << setw(50) << (patron.Name.length() > 50 ? patron.Name.substr(0, 47) + "..." : patron.Name) << " "
                 << setw(30) << (patron.Email.length() > 30 ? patron.Email.substr(0, 27) + "..." : patron.Email) << " "
                 << setw(15) << patron.Phone << " "
                 << setw(30) << (patron.Address.length() > 30 ? patron.Address.substr(0, 27) + "..." : patron.Address) << " "
                 << setw(15) << patron.Status
                 << endl << (&patron - data.data() != data.size() - 1 ? "\n" : "");
}

vector<Patron> underStatusPatron(string filter) {
    vector<Patron> ret;
    for (const auto &patron : patrons)
        if (patron.Status == filter) ret.push_back(patron);
    sort(ret.begin(), ret.end(), [](const Patron p1, const Patron p2) { return p1.Name < p2.Name; } );
    return ret;
}

Patron getPatron(int id) {
    refreshPatron();
    for (auto patron : patrons) {
        if (patron.ID == id)
            return patron;
    }
    Patron patron;
    return patron;
}

bool idExistPatron(int id) {
    refreshPatron();
    for (auto patron : patrons)
        if (patron.ID == id)
            return true;
    return false;
}
