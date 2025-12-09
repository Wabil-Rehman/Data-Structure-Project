#include "AVLTree.h"
#include "Exceptions.h"
#include "Colors.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

AVLNode::AVLNode(const Aircraft& a, const char* s) : aircraft(a), height(1), left(nullptr), right(nullptr) {
    if (s != nullptr && strlen(s) < 20) {
        strcpy_s(status, 20, s);
    }
    else {
        strcpy_s(status, 20, "UNKNOWN");
    }
}

int AVLTree::height(AVLNode* node) {
    return node ? node->height : 0;
}

int AVLTree::balanceFactor(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

void AVLTree::updateHeight(AVLNode* node) {
    if (node) {
        int hl = height(node->left);
        int hr = height(node->right);
        node->height = 1 + (hl > hr ? hl : hr);
    }
}

AVLNode* AVLTree::rotateRight(AVLNode* y) {
    if (y == nullptr || y->left == nullptr) return y;

    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    updateHeight(y);
    updateHeight(x);
    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    if (x == nullptr || x->right == nullptr) return x;

    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    return y;
}

AVLNode* AVLTree::balance(AVLNode* node) {
    if (node == nullptr) return nullptr;

    updateHeight(node);
    int bf = balanceFactor(node);

    if (bf > 1 && balanceFactor(node->left) >= 0)
        return rotateRight(node);

    if (bf < -1 && balanceFactor(node->right) <= 0)
        return rotateLeft(node);

    if (bf > 1 && balanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (bf < -1 && balanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

AVLNode* AVLTree::insert(AVLNode* node, const Aircraft& aircraft, const char* status) {
    if (!node) {
        AVLNode* newNode = new(nothrow) AVLNode(aircraft, status);
        if (newNode == nullptr) {
            throw MemoryAllocationException();
        }
        return newNode;
    }

    if (aircraft.timestamp < node->aircraft.timestamp)
        node->left = insert(node->left, aircraft, status);
    else
        node->right = insert(node->right, aircraft, status);

    return balance(node);
}

void AVLTree::inOrder(AVLNode* node, int& count) const {
    if (!node) return;
    inOrder(node->left, count);

    time_t t = node->aircraft.timestamp;
    char timeStr[30];
    struct tm timeinfo;
    localtime_s(&timeinfo, &t);
    strftime(timeStr, 30, "%H:%M:%S", &timeinfo);

    setColor(3); // Dark Cyan
    cout << "  | ";
    setColor(15); // White
    cout << setw(4) << ++count;
    setColor(3);
    cout << " | ";
    setColor(11); // Cyan
    cout << left << setw(10) << node->aircraft.flightID;
    setColor(3);
    cout << " | ";
    setColor(10); // Green
    cout << left << setw(8) << node->aircraft.origin;
    setColor(3);
    cout << " | ";
    setColor(14); // Yellow
    cout << left << setw(8) << node->aircraft.destination;
    setColor(3);
    cout << " | ";
    setColor(13); // Magenta
    cout << timeStr;
    setColor(3);
    cout << " |" << endl;

    // Status line
    setColor(3);
    cout << "  |      | Status: ";
    if (strcmp(node->status, "LANDED") == 0) setColor(10);     // Green
    else if (strcmp(node->status, "DEPARTED") == 0) setColor(11); // Cyan
    else setColor(14); // Yellow
    cout << node->status;
    setColor(3);
    cout << "                              |" << endl;
    setColor(15);

    inOrder(node->right, count);
}

void AVLTree::saveInOrder(AVLNode* node, ofstream& file) const {
    if (!node) return;
    saveInOrder(node->left, file);
    Aircraft a = node->aircraft;
    file << a.flightID << "|" << a.origin << "|" << a.destination << "|"
        << a.timestamp << "|" << node->status << endl;
    saveInOrder(node->right, file);
}

void AVLTree::destroyTree(AVLNode* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    destroyTree(root);
}

bool AVLTree::isEmpty() const {
    return root == nullptr;
}

void AVLTree::insert(const Aircraft& aircraft, const char* status) {
    if (strlen(aircraft.flightID) == 0) {
        throw InvalidFlightDataException("Cannot log aircraft with empty flight ID");
    }
    if (status == nullptr || strlen(status) == 0) {
        throw InvalidInputException("Status cannot be empty");
    }
    root = insert(root, aircraft, status);
}

void AVLTree::printLog() const {
    if (!root) {
        setColor(14); // Yellow
        cout << "  No flight logs available." << endl;
        setColor(15);
        return;
    }

    setColor(11); // Cyan
    cout << "  +------+------------+----------+----------+-----------+" << endl;
    cout << "  | #    | Flight ID  | Origin   | Dest     | Time      |" << endl;
    cout << "  +------+------------+----------+----------+-----------+" << endl;
    setColor(15);

    int count = 0;
    inOrder(root, count);

    setColor(11); // Cyan
    cout << "  +------+------------+----------+----------+-----------+" << endl;
    setColor(15);
}

void AVLTree::saveToFile(const char* filename) const {
    if (filename == nullptr || strlen(filename) == 0) {
        throw FileException("Invalid filename");
    }

    ofstream file(filename);
    if (!file.is_open()) {
        throw FileException(filename);
    }

    try {
        saveInOrder(root, file);
        file.close();
    }
    catch (...) {
        file.close();
        throw;
    }
}

void AVLTree::loadFromFile(const char* filename) {
    if (filename == nullptr || strlen(filename) == 0) {
        throw FileException("Invalid filename");
    }

    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    try {
        char line[512];
        while (file.getline(line, 512)) {
            Aircraft a;
            char status[20];
            char tempLine[512];
            strcpy_s(tempLine, 512, line);

            char* context = nullptr;
            char* token = strtok_s(tempLine, "|", &context);

            if (token) strcpy_s(a.flightID, 20, token);
            token = strtok_s(nullptr, "|", &context);
            if (token) strcpy_s(a.origin, 20, token);
            token = strtok_s(nullptr, "|", &context);
            if (token) strcpy_s(a.destination, 20, token);
            token = strtok_s(nullptr, "|", &context);
            if (token) a.timestamp = atoi(token);
            token = strtok_s(nullptr, "|", &context);
            if (token) strcpy_s(status, 20, token);

            if (strlen(a.flightID) > 0) {
                root = insert(root, a, status);
            }
        }
        file.close();
    }
    catch (...) {
        file.close();
        throw;
    }
}
