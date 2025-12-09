#include "ATCSystem.h"
#include "Utilities.h"
#include "Exceptions.h"
#include "Colors.h"
#include <iostream>
#include <cstring>
#include <cstdio>
#include <limits>

using namespace std;

void ATCSystem::initializeAirspace() {
    try {
        graph.addNode("JFK", 2, 2, true);
        graph.addNode("LHR", 17, 3, true);
        graph.addNode("DXB", 10, 17, true);
        graph.addNode("ISB", 15, 10, true);

        graph.addNode("WP1", 5, 5, false);
        graph.addNode("WP2", 10, 5, false);
        graph.addNode("WP3", 15, 5, false);
        graph.addNode("WP4", 5, 10, false);
        graph.addNode("WP5", 10, 10, false);
        graph.addNode("WP6", 5, 15, false);
        graph.addNode("WP7", 10, 15, false);
        graph.addNode("WP8", 15, 15, false);

        graph.addEdge(0, 4, 50);
        graph.addEdge(4, 0, 50);
        graph.addEdge(4, 5, 40);
        graph.addEdge(5, 4, 40);
        graph.addEdge(4, 7, 45);
        graph.addEdge(7, 4, 45);
        graph.addEdge(5, 6, 35);
        graph.addEdge(6, 5, 35);
        graph.addEdge(5, 8, 40);
        graph.addEdge(8, 5, 40);
        graph.addEdge(6, 1, 30);
        graph.addEdge(1, 6, 30);
        graph.addEdge(6, 3, 45); 
        graph.addEdge(3, 6, 45);
        graph.addEdge(7, 8, 40);
        graph.addEdge(8, 7, 40);
        graph.addEdge(7, 9, 35); 
        graph.addEdge(9, 7, 35);
        graph.addEdge(8, 10, 40);
        graph.addEdge(10, 8, 40);
        graph.addEdge(8, 3, 50);
        graph.addEdge(3, 8, 50);
        graph.addEdge(9, 10, 35);
        graph.addEdge(10, 9, 35);
        graph.addEdge(10, 2, 30);
        graph.addEdge(2, 10, 30);
        graph.addEdge(10, 11, 35);
        graph.addEdge(11, 10, 35);
        graph.addEdge(11, 3, 40);
        graph.addEdge(3, 11, 40);

        radar.initialize(graph);
    }
    catch (const ATCException& e) {
        printError(e.what());
        throw;
    }
}

void ATCSystem::displayRadar() {
    try {
        Aircraft flights[MAX_FLIGHTS];
        int count;
        registry.getAllFlights(flights, count);
        radar.initialize(graph);
        radar.placePlanes(flights, count);
        radar.display();
    }
    catch (const ATCException& e) {
        printError(e.what());
    }
}

void ATCSystem::checkCollision(int x, int y, const char* excludeFlightID) {
    Aircraft existingFlights[MAX_FLIGHTS];
    int count;
    registry.getAllFlights(existingFlights, count);

    for (int i = 0; i < count; i++) {
        if (excludeFlightID != nullptr && strcmp(existingFlights[i].flightID, excludeFlightID) == 0) {
            continue;
        }
        if (existingFlights[i].gridX == x && existingFlights[i].gridY == y) {
            throw CollisionException(existingFlights[i].flightID);
        }
    }
}

ATCSystem::ATCSystem() {
    initializeAirspace();
}

void ATCSystem::showAirspaceVisualizer() {
    clearScreen();
    printHeader("AIRSPACE VISUALIZER - RADAR VIEW");

    try {
        displayRadar();

        cout << "\n";
        printLine('-');
        setColor(14); 
        cout << "  NEXT TO LAND:" << endl;
        setColor(15);
        printLine('-');

        if (!landingQueue.isEmpty()) {
            Aircraft next = landingQueue.peekMin();
            const char* status;
            int statusColor;
            switch (next.priority) {
            case 1: status = "CRITICAL"; statusColor = 12; break;
            case 2: status = "HIGH"; statusColor = 14; break;
            case 3: status = "MEDIUM"; statusColor = 10; break;
            default: status = "LOW"; statusColor = 7; break;
            }
            cout << "  Flight: ";
            setColor(11);
            cout << next.flightID;
            setColor(15);
            cout << " | Priority: ";
            setColor(statusColor);
            cout << status;
            setColor(15);
            cout << " | Fuel: ";
            if (next.fuelLevel < 20) setColor(12);
            else if (next.fuelLevel < 50) setColor(14);
            else setColor(10);
            cout << next.fuelLevel << "%";
            setColor(15);
            cout << endl;
        }
        else {
            setColor(8); 
            cout << "  No flights in landing queue." << endl;
            setColor(15);
        }
    }
    catch (const ATCException& e) {
        printError(e.what());
    }

    waitForEnter();
}

void ATCSystem::addFlight() {
    clearScreen();
    printHeader("ADD NEW FLIGHT");

    try {
        Aircraft newFlight;
        char tempStr[50];

        setColor(11);
        cout << "  Enter Flight ID (e.g., PK-786): ";
        setColor(10);
        if (!getValidString(tempStr, 20)) {
            throw InvalidInputException("Invalid flight ID format");
        }
        setColor(15);
        newFlight.setFlightID(tempStr);

        if (registry.exists(newFlight.flightID)) {
            throw DuplicateFlightException(newFlight.flightID);
        }

        setColor(11);
        cout << "  Enter Aircraft Model: ";
        setColor(10);
        if (!getValidString(tempStr, 30)) {
            throw InvalidInputException("Invalid model format");
        }
        setColor(15);
        newFlight.setModel(tempStr);

        setColor(11);
        cout << "  Enter Fuel Level (0-100): ";
        setColor(10);
        int fuel;
        if (!getValidInteger(fuel, 0, 100)) {
            throw OutOfRangeException("Fuel level (must be 0-100)");
        }
        setColor(15);
        newFlight.fuelLevel = fuel;

        setColor(11);
        cout << "  Enter Origin Airport: ";
        setColor(10);
        if (!getValidString(tempStr, 20)) {
            throw InvalidInputException("Invalid origin format");
        }
        setColor(15);
        newFlight.setOrigin(tempStr);

        setColor(11);
        cout << "  Enter Destination Airport: ";
        setColor(10);
        if (!getValidString(tempStr, 20)) {
            throw InvalidInputException("Invalid destination format");
        }
        setColor(15);
        newFlight.setDestination(tempStr);

        setColor(14);
        cout << "\n  Flight Type:" << endl;
        setColor(12); cout << "  1. "; setColor(15); cout << "CRITICAL (Medical/Emergency)" << endl;
        setColor(14); cout << "  2. "; setColor(15); cout << "HIGH (Low Fuel)" << endl;
        setColor(10); cout << "  3. "; setColor(15); cout << "MEDIUM (Commercial)" << endl;
        setColor(7);  cout << "  4. "; setColor(15); cout << "LOW (Cargo/Private)" << endl;
        setColor(11);
        cout << "  Select Priority (1-4): ";
        setColor(10);

        int priority;
        if (!getValidInteger(priority, 1, 4)) {
            printWarning("Invalid priority, defaulting to MEDIUM (3)");
            priority = 3;
        }
        setColor(15);
        newFlight.priority = priority;

        if (newFlight.fuelLevel < 10 && newFlight.priority > 2) {
            newFlight.priority = 2;
            printWarning("Low fuel detected! Priority upgraded to HIGH.");
        }

        graph.displayNodes();
        setColor(11);
        cout << "\n  Enter Starting Node ID: ";
        setColor(10);

        int nodeID;
        if (!getValidInteger(nodeID, 0, graph.getNodeCount() - 1)) {
            throw InvalidNodeException(nodeID);
        }
        setColor(15);
        newFlight.graphNode = nodeID;

        newFlight.gridX = graph.getNodeX(newFlight.graphNode);
        newFlight.gridY = graph.getNodeY(newFlight.graphNode);

        checkCollision(newFlight.gridX, newFlight.gridY);

        newFlight.inAir = true;
        newFlight.timestamp = getCurrentTimestamp();

        registry.insert(newFlight);
        landingQueue.insert(newFlight);

        setColor(10); 
        cout << "\n  ========================================" << endl;
        cout << "  FLIGHT " << newFlight.flightID << " ADDED SUCCESSFULLY!" << endl;
        cout << "  Position: (" << newFlight.gridX << ", " << newFlight.gridY << ")" << endl;
        cout << "  ========================================" << endl;
        setColor(15);
    }
    catch (const ATCException& e) {
        printError(e.what());
    }
    catch (const exception& e) {
        printError(e.what());
    }

    waitForEnter();
}

void ATCSystem::declareEmergency() {
    clearScreen();
    printHeader("DECLARE EMERGENCY");

    try {
        setColor(11);
        cout << "  Enter Flight ID: ";
        setColor(10);
        char flightID[20];
        if (!getValidString(flightID, 20)) {
            throw InvalidInputException("Invalid flight ID");
        }
        setColor(15);

        Aircraft* flight = registry.search(flightID);
        if (!flight) {
            throw FlightNotFoundException(flightID);
        }

        cout << "\n  Current Priority: ";
        setColor(14);
        cout << flight->priority << endl;
        setColor(15);

        setColor(14);
        cout << "\n  Emergency Type:" << endl;
        setColor(12); cout << "  1. "; setColor(15); cout << "Medical Emergency" << endl;
        setColor(12); cout << "  2. "; setColor(15); cout << "Engine Failure" << endl;
        setColor(12); cout << "  3. "; setColor(15); cout << "Hijack Alert" << endl;
        setColor(14); cout << "  4. "; setColor(15); cout << "Low Fuel" << endl;
        setColor(11);
        cout << "  Select Emergency (1-4): ";
        setColor(10);

        int emergency;
        if (!getValidInteger(emergency, 1, 4)) {
            throw InvalidInputException("Invalid emergency type");
        }
        setColor(15);

        int newPriority = (emergency <= 3) ? 1 : 2;

        if (landingQueue.decreaseKey(flightID, newPriority)) {
            flight->priority = newPriority;
            setColor(12);
            cout << "\n  ========================================" << endl;
            cout << "  [EMERGENCY DECLARED]" << endl;
            cout << "  Flight " << flightID << " upgraded to Priority " << newPriority << endl;
            cout << "  ========================================" << endl;
            setColor(15);
        }
        else {
            printWarning("Priority not changed (already at or below requested level)");
        }
    }
    catch (const ATCException& e) {
        printError(e.what());
    }

    waitForEnter();
}

void ATCSystem::landFlight() {
    clearScreen();
    printHeader("LAND FLIGHT");

    try {
        if (landingQueue.isEmpty()) {
            throw HeapEmptyException();
        }

        setColor(14);
        cout << "  Current Landing Queue:" << endl;
        setColor(15);
        landingQueue.displayQueue();

        setColor(11);
        cout << "\n  Process landing for top priority flight? (y/n): ";
        setColor(10);
        char choice;
        cin >> choice;
        setColor(15);

        if (choice == 'y' || choice == 'Y') {
            Aircraft landed = landingQueue.extractMin();

            Aircraft* inRegistry = registry.search(landed.flightID);
            if (inRegistry) {
                inRegistry->inAir = false;
            }

            landed.timestamp = getCurrentTimestamp();
            flightLog.insert(landed, "LANDED");

            setColor(10);
            cout << "\n  ========================================" << endl;
            cout << "         FLIGHT LANDED SUCCESSFULLY!" << endl;
            cout << "  ========================================" << endl;
            setColor(15);
            cout << "  Flight ID: ";
            setColor(11);
            cout << landed.flightID << endl;
            setColor(15);
            cout << "  Origin: ";
            setColor(14);
            cout << landed.origin << endl;
            setColor(15);
            cout << "  Destination: ";
            setColor(14);
            cout << landed.destination << endl;
            setColor(10);
            cout << "  ========================================" << endl;
            setColor(15);
        }
    }
    catch (const ATCException& e) {
        printError(e.what());
    }

    waitForEnter();
}

void ATCSystem::searchFlight() {
    clearScreen();
    printHeader("SEARCH FLIGHT");

    try {
        setColor(11);
        cout << "  Enter Flight ID: ";
        setColor(10);
        char flightID[20];
        if (!getValidString(flightID, 20)) {
            throw InvalidInputException("Invalid flight ID");
        }
        setColor(15);

        Aircraft* flight = registry.search(flightID);

        if (!flight) {
            throw FlightNotFoundException(flightID);
        }

        const char* status;
        int statusColor;
        switch (flight->priority) {
        case 1: status = "CRITICAL"; statusColor = 12; break;
        case 2: status = "HIGH"; statusColor = 14; break;
        case 3: status = "MEDIUM"; statusColor = 10; break;
        default: status = "LOW"; statusColor = 7; break;
        }

        setColor(11);
        cout << "\n  ========================================" << endl;
        cout << "           FLIGHT INFORMATION" << endl;
        cout << "  ========================================" << endl;
        setColor(15);
        cout << "  Flight ID:    "; setColor(14); cout << flight->flightID << endl;
        setColor(15); cout << "  Model:        "; setColor(11); cout << flight->model << endl;
        setColor(15); cout << "  Fuel Level:   ";
        if (flight->fuelLevel < 20) setColor(12);
        else if (flight->fuelLevel < 50) setColor(14);
        else setColor(10);
        cout << flight->fuelLevel << "%" << endl;
        setColor(15); cout << "  Origin:       "; setColor(10); cout << flight->origin << endl;
        setColor(15); cout << "  Destination:  "; setColor(10); cout << flight->destination << endl;
        setColor(15); cout << "  Priority:     "; setColor(statusColor); cout << status << " (" << flight->priority << ")" << endl;
        setColor(15); cout << "  Position:     "; setColor(13); cout << "(" << flight->gridX << ", " << flight->gridY << ")" << endl;
        setColor(15); cout << "  Status:       ";
        if (flight->inAir) { setColor(10); cout << "IN AIR"; }
        else { setColor(14); cout << "LANDED"; }
        cout << endl;
        setColor(11);
        cout << "  ========================================" << endl;
        setColor(15);
    }
    catch (const ATCException& e) {
        printError(e.what());
    }

    waitForEnter();
}

void ATCSystem::printFlightLog() {
    clearScreen();
    printHeader("FLIGHT LOG HISTORY");

    try {
        flightLog.printLog();
    }
    catch (const ATCException& e) {
        printError(e.what());
    }

    waitForEnter();
}

void ATCSystem::findSafeRoute() {
    clearScreen();
    printHeader("FIND SAFE ROUTE - DIJKSTRA'S ALGORITHM");

    try {
        setColor(11);
        cout << "  Enter Flight ID: ";
        setColor(10);
        char flightID[20];
        if (!getValidString(flightID, 20)) {
            throw InvalidInputException("Invalid flight ID");
        }
        setColor(15);

        Aircraft* flight = registry.search(flightID);
        if (!flight) {
            throw FlightNotFoundException(flightID);
        }

        int path[MAX_NODES];
        int pathLen;
        int nearestAirport = graph.findNearestAirport(flight->graphNode, path, pathLen);

        if (nearestAirport == -1) {
            throw NoRouteException();
        }

        setColor(10);
        cout << "\n  ========================================" << endl;
        cout << "           SAFE ROUTE CALCULATED" << endl;
        cout << "  ========================================" << endl;
        setColor(15);
        cout << "  Current Location: ";
        setColor(14);
        cout << graph.getNodeName(flight->graphNode) << endl;
        setColor(15);
        cout << "  Nearest Airport:  ";
        setColor(10);
        cout << graph.getNodeName(nearestAirport) << endl;
        setColor(15);
        cout << "\n  Route: ";
        setColor(11);

        for (int i = 0; i < pathLen; i++) {
            if (graph.getIsAirport(path[i]))
                setColor(10); 
            else
                setColor(14);
            cout << graph.getNodeName(path[i]);
            setColor(15);
            if (i < pathLen - 1) {
                setColor(13); 
                cout << " -> ";
            }
        }
        cout << endl;
        setColor(10);
        cout << "  ========================================" << endl;
        setColor(15);
    }
    catch (const ATCException& e) {
        printError(e.what());
    }

    waitForEnter();
}

void ATCSystem::moveFlight() {
    clearScreen();
    printHeader("MOVE FLIGHT");

    try {
        setColor(11);
        cout << "  Enter Flight ID: ";
        setColor(10);
        char flightID[20];
        if (!getValidString(flightID, 20)) {
            throw InvalidInputException("Invalid flight ID");
        }
        setColor(15);

        Aircraft* flight = registry.search(flightID);
        if (!flight) {
            throw FlightNotFoundException(flightID);
        }
        if (!flight->inAir) {
            throw InvalidFlightDataException("Flight is not in air");
        }

        graph.displayNodes();
        cout << "\n  Current Position: Node ";
        setColor(14);
        cout << flight->graphNode << " (" << graph.getNodeName(flight->graphNode) << ")" << endl;
        setColor(11);
        cout << "  Enter Destination Node ID: ";
        setColor(10);

        int destNode;
        if (!getValidInteger(destNode, 0, graph.getNodeCount() - 1)) {
            throw InvalidNodeException(destNode);
        }
        setColor(15);

        int newX = graph.getNodeX(destNode);
        int newY = graph.getNodeY(destNode);

        checkCollision(newX, newY, flightID);

        flight->graphNode = destNode;
        flight->gridX = newX;
        flight->gridY = newY;
        flight->fuelLevel -= 5;
        if (flight->fuelLevel < 0) flight->fuelLevel = 0;

        if (flight->fuelLevel < 10 && flight->priority > 2) {
            flight->priority = 2;
            landingQueue.decreaseKey(flightID, 2);
            printWarning("LOW FUEL ALERT! Priority upgraded to HIGH!");
        }

        setColor(10); 
        cout << "\n  ========================================" << endl;
        cout << "           FLIGHT MOVED SUCCESSFULLY" << endl;
        cout << "  ========================================" << endl;
        setColor(15);
        cout << "  New Position: ";
        setColor(14);
        cout << graph.getNodeName(destNode) << " (" << newX << ", " << newY << ")" << endl;
        setColor(15);
        cout << "  Remaining Fuel: ";
        if (flight->fuelLevel < 20) setColor(12);
        else if (flight->fuelLevel < 50) setColor(14);
        else setColor(10);
        cout << flight->fuelLevel << "%" << endl;
        setColor(10);
        cout << "  ========================================" << endl;
        setColor(15);
    }
    catch (const ATCException& e) {
        printError(e.what());
    }

    waitForEnter();
}

void ATCSystem::saveState() {
    try {
        registry.saveToFile("flights.txt");
        flightLog.saveToFile("flightlog.txt");
        printSuccess("Data saved to files!");
    }
    catch (const ATCException& e) {
        printError(e.what());
    }
}

void ATCSystem::loadState() {
    try {
        registry.loadFromFile("flights.txt");
        flightLog.loadFromFile("flightlog.txt");
        printSuccess("Data loaded from files!");
    }
    catch (const ATCException& e) {
        printError(e.what());
    }
}

void ATCSystem::showLandingQueue() {
    clearScreen();
    printHeader("LANDING QUEUE STATUS");

    try {
        landingQueue.displayQueue();
    }
    catch (const ATCException& e) {
        printError(e.what());
    }

    waitForEnter();
}

void ATCSystem::run() {
    int choice;

    do {
        clearScreen();
        printHeader("MAIN MENU");

        setColor(3);
        cout << "  +==========================================+" << endl;
        setColor(13);
        cout << "  |        AIRSPACE MANAGEMENT               |" << endl;
        setColor(3);
        cout << "  +==========================================+" << endl;
        setColor(15);
        cout << "  |  "; setColor(14); cout << "1."; setColor(15); cout << " View Radar (Airspace Visualizer)     |" << endl;
        cout << "  |  "; setColor(14); cout << "2."; setColor(15); cout << " View Landing Queue                   |" << endl;
        setColor(3);
        cout << "  +==========================================+" << endl;
        setColor(13);
        cout << "  |        FLIGHT CONTROL                    |" << endl;
        setColor(3);
        cout << "  +==========================================+" << endl;
        setColor(15);
        cout << "  |  "; setColor(14); cout << "3."; setColor(15); cout << " Add New Flight                       |" << endl;
        cout << "  |  "; setColor(14); cout << "4."; setColor(15); cout << " Move Flight                          |" << endl;
        cout << "  |  "; setColor(14); cout << "5."; setColor(15); cout << " Declare Emergency                    |" << endl;
        cout << "  |  "; setColor(14); cout << "6."; setColor(15); cout << " Land Flight                          |" << endl;
        setColor(3);
        cout << "  +==========================================+" << endl;
        setColor(13);
        cout << "  |        SEARCH & REPORTING                |" << endl;
        setColor(3);
        cout << "  +==========================================+" << endl;
        setColor(15);
        cout << "  |  "; setColor(14); cout << "7."; setColor(15); cout << " Search Flight                        |" << endl;
        cout << "  |  "; setColor(14); cout << "8."; setColor(15); cout << " Print Flight Log                     |" << endl;
        cout << "  |  "; setColor(14); cout << "9."; setColor(15); cout << " Find Safe Route (Dijkstra)           |" << endl;
        setColor(3);
        cout << "  +==========================================+" << endl;
        setColor(13);
        cout << "  |        SYSTEM                            |" << endl;
        setColor(3);
        cout << "  +==========================================+" << endl;
        setColor(15);
        cout << "  | "; setColor(14); cout << "10."; setColor(15); cout << " Save Data                            |" << endl;
        cout << "  | "; setColor(14); cout << "11."; setColor(15); cout << " Load Data                            |" << endl;
        cout << "  |  "; setColor(12); cout << "0."; setColor(15); cout << " Exit                                 |" << endl;
        setColor(3);
        cout << "  +==========================================+" << endl;
        setColor(15);

        setColor(11);
        cout << "\n  Enter your choice: ";
        setColor(10);

        if (!getValidInteger(choice, 0, 11)) {
            printError("Invalid choice! Please enter a number between 0-11.");
            waitForEnter();
            continue;
        }
        setColor(15);

        try {
            switch (choice) {
            case 1: showAirspaceVisualizer();
                break;
            case 2: showLandingQueue();
                break;
            case 3: addFlight();
                break;
            case 4: moveFlight();
                break;
            case 5: declareEmergency();
                break;
            case 6: landFlight();
                break;
            case 7: searchFlight();
                break;
            case 8: printFlightLog();
                break;
            case 9: findSafeRoute();
                break;
            case 10: saveState();
                waitForEnter();
                break;
            case 11: loadState();
                waitForEnter();
                break;
            case 0:
                setColor(14);
                cout << "\n  Saving data before exit..." << endl;
                setColor(15);
                saveState();
                setColor(10);
                cout << "  Goodbye!" << endl;
                setColor(15);
                break;
            }
        }
        catch (const ATCException& e) {
            printError(e.what());
            waitForEnter();
        }
        catch (const exception& e) {
            printError(e.what());
            waitForEnter();
        }

    } while (choice != 0);
}
