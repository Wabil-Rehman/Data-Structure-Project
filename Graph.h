#ifndef GRAPH_H
#define GRAPH_H

#include "Constants.h"

struct EdgeNode {
    int dest;
    int weight;
    EdgeNode* next;

    EdgeNode(int d, int w);
};

class Graph {
private:
    EdgeNode* adjList[MAX_NODES];
    char nodeNames[MAX_NODES][20];
    int nodeX[MAX_NODES];
    int nodeY[MAX_NODES];
    bool isAirport[MAX_NODES];
    int nodeCount;

public:
    Graph();
    ~Graph();
    int addNode(const char* name, int x, int y, bool airport);
    void addEdge(int src, int dest, int weight);
    int getNodeCount() const;
    const char* getNodeName(int i) const;
    int getNodeX(int i) const;
    int getNodeY(int i) const;
    bool getIsAirport(int i) const;
    int findNode(const char* name) const;
    void dijkstra(int src, int dist[], int parent[]) const;
    int findNearestAirport(int src, int path[], int& pathLen) const;
    void displayNodes() const;
    bool isValidNode(int nodeID) const;
};

#endif
