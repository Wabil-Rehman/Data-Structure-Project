#ifndef ATCSYSTEM_H
#define ATCSYSTEM_H

#include "Constants.h"
#include "Aircraft.h"
#include "Graph.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "AVLTree.h"
#include "Radar.h"

class ATCSystem {
private:
    Graph graph;
    HashTable registry;
    MinHeap landingQueue;
    AVLTree flightLog;
    Radar radar;

    void initializeAirspace();
    void displayRadar();
    void checkCollision(int x, int y, const char* excludeFlightID = nullptr);

public:
    ATCSystem();
    void showAirspaceVisualizer();
    void addFlight();
    void declareEmergency();
    void landFlight();
    void searchFlight();
    void printFlightLog();
    void findSafeRoute();
    void moveFlight();
    void saveState();
    void loadState();
    void showLandingQueue();
    void run();
};

#endif
