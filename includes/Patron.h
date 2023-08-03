#ifndef _Patrons
#define _Patrons

#include <vector>

struct Patron {
    int ID;
    string Name;
    string Email;
    string Phone;
    string Address;
    string Status;
};

void refreshPatron();
void menuPatron();
void addPatron();
void updatePatron();
void removePatron();
void searchPatron(string key);
void savePatron();
void sortPatron();
void printPatron();
void printPatron(vector<Patron> data);
vector<Patron> underStatusPatron(string filter) ;
Patron getPatron(int id);
bool idExistPatron(int id);

#endif
