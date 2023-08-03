#ifndef _Helper
#define _Helper

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <regex>

using namespace std;

static char token = ';';
static string cMargin = "     ";

static int TerminalWidth = 180;

static void clear() {
    #if __MAC__
        system("clear");
    #else
        system("cls");
    #endif
        cout << endl;
}

static void pause() {
    cout << "Press ENTER to continue...";
    cin.get();
}

static void cPrint(string message, int terminalWidth = TerminalWidth) {
    cout << right << setw((terminalWidth + message.length()) / 2) << setfill(' ') << message << endl;
}

static string toLow(string text) {
    for (char &c : text) c = tolower(c);
    return text;
}

static double getInput(string message, string errMessage, double min = 0, double max = 0, bool returnInt = false) {
    string input;
    while (true) {
        cout << message;
        getline(cin, input);
        try {
            bool isInteger = all_of(input.begin() + (input[0] == '-' or input[0] == '+' ? 1 : 0),
                                    input.end(),
                                    [](char c) { return isdigit(c); });
            bool isDouble = all_of(input.begin() + (input[0] == '-' or input[0] == '+' ? 1 : 0),
                                   input.end(),
                                   [](char c) { return isdigit(c) or c == '.'; });
            int decimalCount = count(input.begin(), input.end(), '.');
            if ((returnInt and (!isInteger or decimalCount > 0)) or
                (!returnInt and (!isDouble or decimalCount > 1)))
                throw runtime_error(errMessage);
            if (returnInt)
                stoi(input);
            stod(input);
        } catch (exception &e) {
            cout << errMessage << endl;
            continue;
        }
        if ((returnInt and min == 0 and max == 0) or
            (returnInt and stoi(input) >= min and stoi(input) <= max))
            return stoi(input);
        else if ((!returnInt and min == 0 and max == 0) or
                 (!returnInt and stod(input) >= min and stod(input) <= max))
            return stod(input);
        else cout << errMessage << endl;
    }
    return returnInt ? stoi(input) : stod(input);
}

static string getInput(string message, string errMessage, string exclude) {
    cout << message;
    string input;
    getline(cin, input);
    if (exclude == "") {
        return input;
    } else if (input.find(exclude) != string::npos) {
        cout << errMessage << endl;
        return getInput(message, errMessage, exclude);
    }
    return input;
}

static string getPhone(string message, string errMessage) {
    cout << message;
    string input;
    getline(cin, input);
    if (input.length() != 11 or
        input[0] != '0' or
        input[1] != '9' or
        !all_of(input.begin(), input.end(), [](char c){ return isdigit(c); })) {
        cout << errMessage << endl;
        return getPhone(message, errMessage);
    }
    return input;
}

static string getEmail(string message, string errMessage) {
    const regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    cout << message;
    string input;
    getline(cin, input);
    if (regex_match(input, emailRegex))
        return input;
    else {
        cout << errMessage << endl;
        return getEmail(message, errMessage);
    }
    return "";
}

static int sloppyUUID() {
    return time(0);
}

static string formatUNIX(time_t timestamp) {
    struct tm *timeinfo = localtime(&timestamp);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}

#endif
