#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Constants.h"
#include "Aircraft.h"

struct HashNode {
    Aircraft aircraft;
    HashNode* next;
    HashNode(const Aircraft& a);
};

class HashTable {
private:
    HashNode* table[HASH_TABLE_SIZE];
    int hashFunction(const char* key);

public:
    HashTable();
    ~HashTable();
    void insert(const Aircraft& aircraft);
    Aircraft* search(const char* flightID);
    bool remove(const char* flightID);
    void getAllFlights(Aircraft* flights, int& count);
    void saveToFile(const char* filename);
    void loadFromFile(const char* filename);
    bool exists(const char* flightID);
};

#endif
