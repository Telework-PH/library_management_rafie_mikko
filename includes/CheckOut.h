#ifndef _CheckOuts
#define _CheckOuts

#include <vector>

struct CheckOut {
    int ID;
    int PatronID;
    int BookID;
    int DateTime;
    int Duration;
    string Status;
};

void refreshCheckOut();
void menuCheckOut();
void borrowCheckOut();
int getAvailableBook();
int getActivePatron();
void returnCheckOut();
void searchCheckOut(string key);
void saveCheckOut();
void sortCheckOut();
void printCheckOut();
void printCheckOut(vector<CheckOut> data);
bool hasActiveTransaction(int patronID);
CheckOut getCheckOut(int id);
bool idExistCheckOut(int id);

#endif