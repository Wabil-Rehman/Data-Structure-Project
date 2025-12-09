#include "Utilities.h"
#include "Colors.h"
#include "Exceptions.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cstring>

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLine(char ch, int len) {
    setColor(3);
    for (int i = 0; i < len; i++) cout << ch;
    cout << endl;
    setColor(15); 
}

void printCentered(const string& text, int width) {
    int padding = (width - (int)text.length()) / 2;
    if (padding < 0) padding = 0;
    for (int i = 0; i < padding; i++) cout << " ";
    cout << text << endl;
}

void printHeader(const string& title) {
    setColor(11); 
    printLine('*');
    setColor(13); 
    printCentered("SKYNET AIR TRAFFIC CONTROL SIMULATOR");
    setColor(11);
    printLine('*');
    cout << endl;
    setColor(3);
    printLine('-');
    setColor(14);
    printCentered(title);
    setColor(3); 
    printLine('-');
    setColor(15);
    cout << endl;
}

int getCurrentTimestamp() {
    return (int)time(nullptr);
}

void printError(const char* message) {
    setColor(12);
    cout << "\n  [ERROR] " << message << endl;
    setColor(15); 
}

void printSuccess(const char* message) {
    setColor(10); 
    cout << "\n  [SUCCESS] " << message << endl;
    setColor(15); 
}

void printWarning(const char* message) {
    setColor(14); 
    cout << "\n  [WARNING] " << message << endl;
    setColor(15); 
}

void printInfo(const char* message) {
    setColor(11); 
    cout << "\n  [INFO] " << message << endl;
    setColor(15);
}

void waitForEnter() {
    setColor(8); 
    cout << "\n  Press Enter to continue...";
    setColor(15);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

bool getValidInteger(int& value, int min, int maxVal) {
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    if (value < min || value > maxVal) {
        return false;
    }
    return true;
}

bool getValidString(char* buffer, int maxLen) {
    cin >> buffer;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    if ((int)strlen(buffer) >= maxLen) {
        return false;
    }
    return true;
}
