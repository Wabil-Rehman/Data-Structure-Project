#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <cstring>

class ATCException : public std::exception {
protected:
    char message[256];
public:
    ATCException(const char* msg) {
        strcpy_s(message, 256, msg);
    }
    const char* what() const noexcept override {
        return message;
    }
};

class FlightNotFoundException : public ATCException {
public:
    FlightNotFoundException(const char* flightID) : ATCException("Flight not found: ") {
        strcat_s(message, 256, flightID);
    }
};

class DuplicateFlightException : public ATCException {
public:
    DuplicateFlightException(const char* flightID) : ATCException("Flight already exists: ") {
        strcat_s(message, 256, flightID);
    }
};

class InvalidFlightDataException : public ATCException {
public:
    InvalidFlightDataException(const char* msg) : ATCException(msg) {
    }
};

class InvalidNodeException : public ATCException {
public:
    InvalidNodeException(int nodeID) : ATCException("Invalid node ID specified") {
    }
};

class NodeFullException : public ATCException {
public:
    NodeFullException() : ATCException("Maximum nodes reached in graph") {
    }
};

class NoRouteException : public ATCException {
public:
    NoRouteException() : ATCException("No route found to any airport") {
    }
};

class CollisionException : public ATCException {
public:
    CollisionException(const char* existingFlightID) : ATCException("Collision alert! Position occupied by: ") {
        strcat_s(message, 256, existingFlightID);
    }
};

class HeapFullException : public ATCException {
public:
    HeapFullException() : ATCException("Landing queue is full") {
    }
};

class HeapEmptyException : public ATCException {
public:
    HeapEmptyException() : ATCException("Landing queue is empty") {
    }
};

class HashTableException : public ATCException {
public:
    HashTableException(const char* msg) : ATCException(msg) {
    }
};

class FileException : public ATCException {
public:
    FileException(const char* filename) : ATCException("File operation failed: ") {
        strcat_s(message, 256, filename);
    }
};

class InvalidInputException : public ATCException {
public:
    InvalidInputException(const char* msg) : ATCException(msg) {
    }
};

class OutOfRangeException : public ATCException {
public:
    OutOfRangeException(const char* field) : ATCException("Value out of range for: ") {
        strcat_s(message, 256, field);
    }
};

class MemoryAllocationException : public ATCException {
public:
    MemoryAllocationException() : ATCException("Memory allocation failed") {
    }
};

#endif
