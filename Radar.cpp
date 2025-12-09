#include "Radar.h"
#include "Exceptions.h"
#include "Colors.h"
#include <iostream>
#include <iomanip>

using namespace std;

Radar::Radar() {
    clear();
}

void Radar::clear() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = '.';
        }
    }
}

void Radar::initialize(Graph& graph) {
    clear();

    try {
        for (int i = 0; i < graph.getNodeCount(); i++) {
            int x = graph.getNodeX(i);
            int y = graph.getNodeY(i);
            if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
                grid[y][x] = graph.getIsAirport(i) ? 'A' : 'W';
            }
        }
    }
    catch (const ATCException& e) {
        cerr << "Warning: " << e.what() << endl;
    }
}

void Radar::placePlanes(Aircraft* flights, int count) {
    if (flights == nullptr && count > 0) {
        throw InvalidInputException("Null flights array");
    }

    for (int i = 0; i < count; i++) {
        int x = flights[i].gridX;
        int y = flights[i].gridY;
        if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
            grid[y][x] = 'P';
        }
    }
}

bool Radar::isOccupied(int x, int y) const {
    if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE) {
        return true;
    }
    return grid[y][x] == 'P';
}

void Radar::display() const {
    // Column headers
    setColor(14); 
    cout << "\n     ";
    for (int i = 0; i < GRID_SIZE; i++) cout << setw(2) << i;
    cout << endl;

    // Top border
    setColor(3);
    cout << "    +";
    for (int i = 0; i < GRID_SIZE * 2 + 1; i++) cout << "-";
    cout << "+" << endl;

    // Grid rows
    for (int i = 0; i < GRID_SIZE; i++) {
        setColor(14);
        cout << setw(2) << i << "  ";
        setColor(3); 
        cout << "|";

        for (int j = 0; j < GRID_SIZE; j++) {
            char c = grid[i][j];
            cout << " ";

            switch (c) {
            case 'P':
                setColor(14); // Yellow - Plane
                break;
            case 'A':
                setColor(10); // Green - Airport
                break;
            case 'W':
                setColor(11); // Cyan - Waypoint
                break;
            default:
                setColor(8); // Dark Gray - Empty
                break;
            }
            cout << c;
            setColor(3); // Reset for grid
        }

        setColor(3); 
        cout << " |" << endl;
    }

    // Bottom border
    cout << "    +";
    for (int i = 0; i < GRID_SIZE * 2 + 1; i++) cout << "-";
    cout << "+" << endl;

    cout << "\n  ";
    setColor(15); 
    cout << "Legend: ";
    setColor(14); 
    cout << "P";
    setColor(15);
    cout << "=Plane  ";
    setColor(10);
    cout << "A";
    setColor(15);
    cout << "=Airport  ";
    setColor(11); 
    cout << "W";
    setColor(15);
    cout << "=Waypoint  ";
    setColor(8); 
    cout << ".";
    setColor(15);
    cout << "=Empty" << endl;
}
