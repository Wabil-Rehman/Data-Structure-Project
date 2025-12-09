#include "Aircraft.h"
#include "Exceptions.h"

Aircraft::Aircraft() {
    strcpy_s(flightID, 20, "");
    strcpy_s(model, 30, "");
    fuelLevel = 100;
    strcpy_s(origin, 20, "");
    strcpy_s(destination, 20, "");
    priority = 4;
    gridX = gridY = 0;
    graphNode = 0;
    inAir = false;
    timestamp = 0;
}

void Aircraft::setFlightID(const char* id) {
    if (id == nullptr || strlen(id) == 0) {
        throw InvalidFlightDataException("Flight ID cannot be empty");
    }
    if (strlen(id) >= 20) {
        throw InvalidFlightDataException("Flight ID too long (max 19 characters)");
    }
    strcpy_s(flightID, 20, id);
}

void Aircraft::setModel(const char* m) {
    if (m == nullptr || strlen(m) == 0) {
        throw InvalidFlightDataException("Model cannot be empty");
    }
    if (strlen(m) >= 30) {
        throw InvalidFlightDataException("Model name too long (max 29 characters)");
    }
    strcpy_s(model, 30, m);
}

void Aircraft::setOrigin(const char* o) {
    if (o == nullptr || strlen(o) == 0) {
        throw InvalidFlightDataException("Origin cannot be empty");
    }
    if (strlen(o) >= 20) {
        throw InvalidFlightDataException("Origin name too long (max 19 characters)");
    }
    strcpy_s(origin, 20, o);
}

void Aircraft::setDestination(const char* d) {
    if (d == nullptr || strlen(d) == 0) {
        throw InvalidFlightDataException("Destination cannot be empty");
    }
    if (strlen(d) >= 20) {
        throw InvalidFlightDataException("Destination name too long (max 19 characters)");
    }
    strcpy_s(destination, 20, d);
}
