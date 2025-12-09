#ifndef MINHEAP_H
#define MINHEAP_H

#include "Constants.h"
#include "Aircraft.h"

class MinHeap {
private:
    Aircraft heap[MAX_FLIGHTS];
    int size;

    int parent(int i);
    int leftChild(int i);
    int rightChild(int i);
    void swap(Aircraft& a, Aircraft& b);
    void heapifyUp(int i);
    void heapifyDown(int i);

public:
    MinHeap();
    bool isEmpty() const;
    int getSize() const;
    void insert(const Aircraft& aircraft);
    Aircraft extractMin();
    Aircraft peekMin() const;
    bool decreaseKey(const char* flightID, int newPriority);
    void displayQueue() const;
    bool removeByFlightID(const char* flightID);
};

#endif
