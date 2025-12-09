#include "Graph.h"
#include "Exceptions.h"
#include "Colors.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

EdgeNode::EdgeNode(int d, int w) : dest(d), weight(w), next(nullptr) {}

Graph::Graph() : nodeCount(0) {
    for (int i = 0; i < MAX_NODES; i++) {
        adjList[i] = nullptr;
        nodeNames[i][0] = '\0';
        nodeX[i] = nodeY[i] = 0;
        isAirport[i] = false;
    }
}

Graph::~Graph() {
    for (int i = 0; i < MAX_NODES; i++) {
        EdgeNode* current = adjList[i];
        while (current) {
            EdgeNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

bool Graph::isValidNode(int nodeID) const {
    return nodeID >= 0 && nodeID < nodeCount;
}

int Graph::addNode(const char* name, int x, int y, bool airport) {
    if (nodeCount >= MAX_NODES) {
        throw NodeFullException();
    }
    if (name == nullptr || strlen(name) == 0) {
        throw InvalidInputException("Node name cannot be empty");
    }
    if (strlen(name) >= 20) {
        throw InvalidInputException("Node name too long");
    }
    if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE) {
        throw OutOfRangeException("Node coordinates");
    }

    strcpy_s(nodeNames[nodeCount], 20, name);
    nodeX[nodeCount] = x;
    nodeY[nodeCount] = y;
    isAirport[nodeCount] = airport;
    return nodeCount++;
}

void Graph::addEdge(int src, int dest, int weight) {
    if (!isValidNode(src) || !isValidNode(dest)) {
        throw InvalidNodeException(src);
    }
    if (weight < 0) {
        throw OutOfRangeException("Edge weight");
    }

    EdgeNode* newNode = new(nothrow) EdgeNode(dest, weight);
    if (newNode == nullptr) {
        throw MemoryAllocationException();
    }
    newNode->next = adjList[src];
    adjList[src] = newNode;
}

int Graph::getNodeCount() const { return nodeCount; }

const char* Graph::getNodeName(int i) const {
    if (!isValidNode(i)) {
        throw InvalidNodeException(i);
    }
    return nodeNames[i];
}

int Graph::getNodeX(int i) const {
    if (!isValidNode(i)) {
        throw InvalidNodeException(i);
    }
    return nodeX[i];
}

int Graph::getNodeY(int i) const {
    if (!isValidNode(i)) {
        throw InvalidNodeException(i);
    }
    return nodeY[i];
}

bool Graph::getIsAirport(int i) const {
    if (!isValidNode(i)) {
        throw InvalidNodeException(i);
    }
    return isAirport[i];
}

int Graph::findNode(const char* name) const {
    if (name == nullptr || strlen(name) == 0) {
        return -1;
    }
    for (int i = 0; i < nodeCount; i++) {
        if (strcmp(nodeNames[i], name) == 0) return i;
    }
    return -1;
}

void Graph::dijkstra(int src, int dist[], int parent[]) const {
    if (!isValidNode(src)) {
        throw InvalidNodeException(src);
    }
    if (dist == nullptr || parent == nullptr) {
        throw InvalidInputException("Null arrays provided to dijkstra");
    }

    bool visited[MAX_NODES] = { false };

    for (int i = 0; i < nodeCount; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[src] = 0;

    for (int i = 0; i < nodeCount; i++) {
        int minDist = INF, u = -1;
        for (int j = 0; j < nodeCount; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) break;
        visited[u] = true;

        EdgeNode* edge = adjList[u];
        while (edge) {
            int v = edge->dest;
            int w = edge->weight;
            if (!visited[v] && dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
            }
            edge = edge->next;
        }
    }
}

int Graph::findNearestAirport(int src, int path[], int& pathLen) const {
    if (!isValidNode(src)) {
        throw InvalidNodeException(src);
    }
    if (path == nullptr) {
        throw InvalidInputException("Null path array");
    }

    int dist[MAX_NODES], parent[MAX_NODES];
    dijkstra(src, dist, parent);

    int nearestAirport = -1;
    int minDist = INF;

    for (int i = 0; i < nodeCount; i++) {
        if (isAirport[i] && i != src && dist[i] < minDist) {
            minDist = dist[i];
            nearestAirport = i;
        }
    }

    pathLen = 0;
    if (nearestAirport != -1) {
        int temp[MAX_NODES];
        int tempLen = 0;
        int curr = nearestAirport;
        while (curr != -1 && tempLen < MAX_NODES) {
            temp[tempLen++] = curr;
            curr = parent[curr];
        }
        for (int i = tempLen - 1; i >= 0; i--) {
            path[pathLen++] = temp[i];
        }
    }

    return nearestAirport;
}

void Graph::displayNodes() const {
    setColor(14);
    cout << "\n  Available Nodes:" << endl;
    setColor(11);
    cout << "  +------+----------+----------+----------+" << endl;
    cout << "  | ID   | Name     | Type     | Position |" << endl;
    cout << "  +------+----------+----------+----------+" << endl;
    setColor(15);

    for (int i = 0; i < nodeCount; i++) {
        setColor(3); 
        cout << "  | ";
        setColor(15); 
        cout << setw(4) << i;
        setColor(3);
        cout << " | ";
        setColor(11); 
        cout << left << setw(8) << nodeNames[i];
        setColor(3);
        cout << " | ";

        if (isAirport[i]) {
            setColor(10); 
            cout << left << setw(8) << "AIRPORT";
        }
        else {
            setColor(14); 
            cout << left << setw(8) << "WAYPOINT";
        }

        setColor(3);
        cout << " | ";
        setColor(13); 
        cout << "(" << setw(2) << nodeX[i] << "," << setw(2) << nodeY[i] << ")";
        setColor(3);
        cout << "  |" << endl;
    }

    setColor(11); 
    cout << "  +------+----------+----------+----------+" << endl;
    setColor(15);
}
