#include "HashTable.h"
#include "Exceptions.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

HashNode::HashNode(const Aircraft& a) : aircraft(a), next(nullptr) {}

int HashTable::hashFunction(const char* key) {
    if (key == nullptr) {
        throw InvalidInputException("Null key provided to hash function");
    }
    int sum = 0;
    for (int i = 0; key[i]; i++) {
        sum += (int)key[i];
    }
    return sum % HASH_TABLE_SIZE;
}

HashTable::HashTable() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode* current = table[i];
        while (current) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

bool HashTable::exists(const char* flightID) {
    return search(flightID) != nullptr;
}

void HashTable::insert(const Aircraft& aircraft) {
    try {
        if (strlen(aircraft.flightID) == 0) {
            throw InvalidFlightDataException("Cannot insert aircraft with empty flight ID");
        }

        if (exists(aircraft.flightID)) {
            throw DuplicateFlightException(aircraft.flightID);
        }

        int index = hashFunction(aircraft.flightID);
        HashNode* newNode = new(nothrow) HashNode(aircraft);

        if (newNode == nullptr) {
            throw MemoryAllocationException();
        }

        newNode->next = table[index];
        table[index] = newNode;
    }
    catch (const bad_alloc&) {
        throw MemoryAllocationException();
    }
}

Aircraft* HashTable::search(const char* flightID) {
    if (flightID == nullptr || strlen(flightID) == 0) {
        return nullptr;
    }

    int index = hashFunction(flightID);
    HashNode* current = table[index];

    while (current) {
        if (strcmp(current->aircraft.flightID, flightID) == 0) {
            return &(current->aircraft);
        }
        current = current->next;
    }
    return nullptr;
}

bool HashTable::remove(const char* flightID) {
    if (flightID == nullptr || strlen(flightID) == 0) {
        return false;
    }

    int index = hashFunction(flightID);
    HashNode* current = table[index];
    HashNode* prev = nullptr;

    while (current) {
        if (strcmp(current->aircraft.flightID, flightID) == 0) {
            if (prev) prev->next = current->next;
            else table[index] = current->next;
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

void HashTable::getAllFlights(Aircraft* flights, int& count) {
    if (flights == nullptr) {
        throw InvalidInputException("Null array provided to getAllFlights");
    }

    count = 0;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode* current = table[i];
        while (current) {
            if (current->aircraft.inAir && count < MAX_FLIGHTS) {
                flights[count++] = current->aircraft;
            }
            current = current->next;
        }
    }
}

void HashTable::saveToFile(const char* filename) {
    if (filename == nullptr || strlen(filename) == 0) {
        throw FileException("Invalid filename");
    }

    ofstream file(filename);
    if (!file.is_open()) {
        throw FileException(filename);
    }

    try {
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            HashNode* current = table[i];
            while (current) {
                Aircraft& a = current->aircraft;
                file << a.flightID << "|" << a.model << "|" << a.fuelLevel << "|"
                    << a.origin << "|" << a.destination << "|" << a.priority << "|"
                    << a.gridX << "|" << a.gridY << "|" << a.graphNode << "|"
                    << a.inAir << "|" << a.timestamp << endl;

                if (file.fail()) {
                    throw FileException(filename);
                }
                current = current->next;
            }
        }
        file.close();
    }
    catch (...) {
        file.close();
        throw;
    }
}

void HashTable::loadFromFile(const char* filename) {
    if (filename == nullptr || strlen(filename) == 0) {
        throw FileException("Invalid filename");
    }

    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    try {
        char line[512];
        while (file.getline(line, 512)) {
            Aircraft a;
            char tempLine[512];
            strcpy_s(tempLine, 512, line);

            char* context = nullptr;
            char* token = strtok_s(tempLine, "|", &context);

            if (token) strcpy_s(a.flightID, 20, token);
            token = strtok_s(nullptr, "|", &context);
            if (token) strcpy_s(a.model, 30, token);
            token = strtok_s(nullptr, "|", &context);
            if (token) a.fuelLevel = atoi(token);
            token = strtok_s(nullptr, "|", &context);
            if (token) strcpy_s(a.origin, 20, token);
            token = strtok_s(nullptr, "|", &context);
            if (token) strcpy_s(a.destination, 20, token);
            token = strtok_s(nullptr, "|", &context);
            if (token) a.priority = atoi(token);
            token = strtok_s(nullptr, "|", &context);
            if (token) a.gridX = atoi(token);
            token = strtok_s(nullptr, "|", &context);
            if (token) a.gridY = atoi(token);
            token = strtok_s(nullptr, "|", &context);
            if (token) a.graphNode = atoi(token);
            token = strtok_s(nullptr, "|", &context);
            if (token) a.inAir = atoi(token);
            token = strtok_s(nullptr, "|", &context);
            if (token) a.timestamp = atoi(token);

            if (strlen(a.flightID) > 0 && !exists(a.flightID)) {
                int index = hashFunction(a.flightID);
                HashNode* newNode = new(nothrow) HashNode(a);
                if (newNode) {
                    newNode->next = table[index];
                    table[index] = newNode;
                }
            }
        }
        file.close();
    }
    catch (...) {
        file.close();
        throw;
    }
}
