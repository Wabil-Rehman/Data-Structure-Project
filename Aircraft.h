#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <cstring>

struct Aircraft {
    char flightID[20];
    char model[30];
    int fuelLevel;
    char origin[20];
    char destination[20];
    int priority;
    int gridX, gridY;
    int graphNode;
    bool inAir;
    int timestamp;

    Aircraft();
    void setFlightID(const char* id);
    void setModel(const char* m);
    void setOrigin(const char* o);
    void setDestination(const char* d);
};

#endif
