#include "MinHeap.h"
#include "Exceptions.h"
#include "Colors.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

int MinHeap::parent(int i) { return (i - 1) / 2; }
int MinHeap::leftChild(int i) { return 2 * i + 1; }
int MinHeap::rightChild(int i) { return 2 * i + 2; }

void MinHeap::swap(Aircraft& a, Aircraft& b) {
    Aircraft temp = a;
    a = b;
    b = temp;
}

void MinHeap::heapifyUp(int i) {
    while (i > 0 && heap[parent(i)].priority > heap[i].priority) {
        swap(heap[parent(i)], heap[i]);
        i = parent(i);
    }
}

void MinHeap::heapifyDown(int i) {
    int smallest = i;
    int left = leftChild(i);
    int right = rightChild(i);

    if (left < size && heap[left].priority < heap[smallest].priority)
        smallest = left;
    if (right < size && heap[right].priority < heap[smallest].priority)
        smallest = right;

    if (smallest != i) {
        swap(heap[i], heap[smallest]);
        heapifyDown(smallest);
    }
}

MinHeap::MinHeap() : size(0) {}

bool MinHeap::isEmpty() const { return size == 0; }
int MinHeap::getSize() const { return size; }

void MinHeap::insert(const Aircraft& aircraft) {
    if (size >= MAX_FLIGHTS) {
        throw HeapFullException();
    }

    if (strlen(aircraft.flightID) == 0) {
        throw InvalidFlightDataException("Cannot insert aircraft with empty flight ID");
    }

    heap[size] = aircraft;
    heapifyUp(size);
    size++;
}

Aircraft MinHeap::extractMin() {
    if (isEmpty()) {
        throw HeapEmptyException();
    }

    Aircraft min = heap[0];
    heap[0] = heap[size - 1];
    size--;

    if (size > 0) {
        heapifyDown(0);
    }

    return min;
}

Aircraft MinHeap::peekMin() const {
    if (isEmpty()) {
        throw HeapEmptyException();
    }
    return heap[0];
}

bool MinHeap::decreaseKey(const char* flightID, int newPriority) {
    if (flightID == nullptr || strlen(flightID) == 0) {
        throw InvalidInputException("Invalid flight ID for decreaseKey");
    }

    if (newPriority < 1 || newPriority > 4) {
        throw OutOfRangeException("Priority (must be 1-4)");
    }

    for (int i = 0; i < size; i++) {
        if (strcmp(heap[i].flightID, flightID) == 0) {
            if (newPriority < heap[i].priority) {
                heap[i].priority = newPriority;
                heapifyUp(i);
                return true;
            }
            return false;
        }
    }
    return false;
}

void MinHeap::displayQueue() const {
    if (isEmpty()) {
        setColor(14); 
        cout << "  No flights in landing queue." << endl;
        setColor(15);
        return;
    }

    setColor(11);
    cout << "  +------+------------+----------+------+-----------+" << endl;
    cout << "  | Rank | Flight ID  | Priority | Fuel | Status    |" << endl;
    cout << "  +------+------------+----------+------+-----------+" << endl;
    setColor(15);

    Aircraft temp[MAX_FLIGHTS];
    for (int i = 0; i < size; i++) temp[i] = heap[i];

    for (int i = 0; i < size && i < 10; i++) {
        int minIdx = i;
        for (int j = i + 1; j < size; j++) {
            if (temp[j].priority < temp[minIdx].priority)
                minIdx = j;
        }
        Aircraft t = temp[i];
        temp[i] = temp[minIdx];
        temp[minIdx] = t;

        const char* status;
        int statusColor;
        switch (temp[i].priority) {
        case 1: status = "CRITICAL"; statusColor = 12; break;  
        case 2: status = "HIGH"; statusColor = 14; break;    
        case 3: status = "MEDIUM"; statusColor = 10; break;   
        default: status = "LOW"; statusColor = 7; break;  
        }

        setColor(3);
        cout << "  | ";
        setColor(15);
        cout << setw(4) << i + 1;
        setColor(3);
        cout << " | ";
        setColor(11);
        cout << left << setw(10) << temp[i].flightID;
        setColor(3);
        cout << " | ";
        setColor(statusColor);
        cout << setw(8) << temp[i].priority;
        setColor(3);
        cout << " | ";

        // Fuel color based on level
        if (temp[i].fuelLevel < 20) setColor(12);     
        else if (temp[i].fuelLevel < 50) setColor(14); 
        else setColor(10);                             
        cout << setw(3) << temp[i].fuelLevel << "%";

        setColor(3);
        cout << " | ";
        setColor(statusColor);
        cout << left << setw(9) << status;
        setColor(3);
        cout << " |" << endl;
    }

    setColor(11);
    cout << "  +------+------------+----------+------+-----------+" << endl;
    setColor(15);
}

bool MinHeap::removeByFlightID(const char* flightID) {
    if (flightID == nullptr || strlen(flightID) == 0) {
        return false;
    }

    for (int i = 0; i < size; i++) {
        if (strcmp(heap[i].flightID, flightID) == 0) {
            heap[i] = heap[size - 1];
            size--;
            if (size > 0 && i < size) {
                heapifyDown(i);
                heapifyUp(i);
            }
            return true;
        }
    }
    return false;
}
