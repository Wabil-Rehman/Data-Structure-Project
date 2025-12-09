#ifndef RADAR_H
#define RADAR_H

#include "Constants.h"
#include "Aircraft.h"
#include "Graph.h"

class Radar {
private:
    char grid[GRID_SIZE][GRID_SIZE];

public:
    Radar();
    void initialize(Graph& graph);
    void placePlanes(Aircraft* flights, int count);
    bool isOccupied(int x, int y) const;
    void display() const;
    void clear();
};

#endif
