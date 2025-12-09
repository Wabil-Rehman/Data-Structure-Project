#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

void clearScreen();
void printLine(char ch = '=', int len = 60);
void printCentered(const std::string& text, int width = 60);
void printHeader(const std::string& title);
int getCurrentTimestamp();
void printError(const char* message);
void printSuccess(const char* message);
void printWarning(const char* message);
void printInfo(const char* message);
void waitForEnter();
bool getValidInteger(int& value, int min, int maxVal);
bool getValidString(char* buffer, int maxLen);

#endif
