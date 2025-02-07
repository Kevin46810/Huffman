// Huffman Tree Implementation
#include <iostream>
#include <map>
#include <fstream>
#include <queue>
#include "huffman.h"

using namespace std;

// Constructor for HuffmanTree, initializes number of characters and built status
HuffmanTree::HuffmanTree() : numChars(0), built(false) {}

void HuffmanTree::insert(char ch, int weight) {
    // Create a new node with given character and weight
    HNode newNode = {ch, weight, 0, -1}; 
    built = false;  // Mark tree as unbuilt after insertion
    nodes.push_back(newNode);  // Add node to the vector
    numChars++;  // Update the count of characters
}

bool HuffmanTree::inTree(char ch) {
    // Ensure the tree is built before checking for character
    if (!built)
        return false;

    // Search for the character in the nodes vector
    for (int i = 0; i < nodes.size(); i++) {
        if (ch == nodes[i].ch) 
            return true;
    }
    return false;
}

int HuffmanTree::GetFrequency(char ch) {
    // Find the character and retrieve its frequency using private function
    for (int i = 0; i < numNodes(); i++) {
        if (ch == nodes[i].ch) 
            return GetFrequency(i);
    }
    return 0;
}

int HuffmanTree::GetFrequency(int i) {
    // Return the weight (frequency) of the node at index i
    return nodes[i].weight;
}

int HuffmanTree::lookUp(char ch) {
    if (!built)  // Cannot run if the tree is not built
        return -1;

    // Return the index of the given character
    for (int i = 0; i < numNodes(); i++) {
        if (nodes[i].ch == ch) {
            return i;
        }
    }
    return -1;
}

string HuffmanTree::GetCode(char ch) {
    if (!built)  // Cannot run if the tree is not built
        return "";

    // Get the Huffman code for the character using the index
    return GetCode(lookUp(ch));
}

string HuffmanTree::GetCode(int i) {
    // Base case: if the node has no parent, return an empty string
    if (nodes[i].parent == 0) 
        return "";

    // Append '0' for left child and '1' for right child
    if (nodes[i].childType == 0)
        return GetCode(nodes[i].parent) + "0";
    else 
        return GetCode(nodes[i].parent) + "1";
}

void HuffmanTree::PrintTable() {
    if (!built) {  // Cannot run if the tree is not built
        cout << "Tree not built." << endl;
        return;
    }

    // Print table header
    cout << "index  char   freq  parent   childType" << endl;
    cout << "--------------------------------------" << endl;

    int TCount = 1;

    // Print all nodes with their properties
    for (int i = 0; i < numNodes(); i++) {
        // Format index for output
        if (i < 10)
            cout << "   " << i;
        else if (i < 100)
            cout << "  " << i;
        else
            cout << " " << i;

        cout << "   ";

        // Handle special characters for printing
        if (i >= numNodes() / 2 + 1) {
            cout << "  T" << TCount; 
            TCount++;
        } else if (nodes[i].ch == '\n')
            cout << "  nl";
        else if (nodes[i].ch == ' ')
            cout << "  sp";
        else
            cout << "   " << nodes[i].ch;

        // Format and print weight
        if (nodes[i].weight <= 9)
            cout << "       " << nodes[i].weight;
        else if (nodes[i].weight > 99)
            cout << "     " << nodes[i].weight;
        else
            cout << "      " << nodes[i].weight;

        // Format and print parent index
        if (nodes[i].parent <= 9 && nodes[i].parent > -1)
            cout << "       " << nodes[i].parent;
        else if (nodes[i].parent > 99)
            cout << "     " << nodes[i].parent;
        else
            cout << "      " << nodes[i].parent;

        // Print child type if available
        if (nodes[i].childType != -1)
            cout << "         " << nodes[i].childType;
        else
            cout << "       N/A";

        cout << endl;
    }
}

int HuffmanTree::numNodes() {
    // Return the number of nodes in the tree
    return nodes.size();
}

void HuffmanTree::build() {
    int freq;

    if (built)
        return;

    map<char, bool> map;

    for (int i = 0; i < numNodes(); i++)
        map[nodes[i].ch] = false;

    // Remove duplicate characters from the vector
    uniqueCharsOnly(nodes);

    int c = numNodes();

    HNode TNode;
    int minIndex1 = -1;
    int minIndex2 = -1;

    while (numNodes() < 2 * c - 1) {
        int min1 = getGreatestWeight(nodes) + 1; 
        int min2 = getGreatestWeight(nodes) + 1;

        // Find first minimum weight node
        for (int i = 0; i < numNodes(); i++) {
            if (nodes[i].weight < min1 && i != minIndex2 && nodes[i].parent == 0 && nodes[i].childType == -1) {
                min1 = nodes[i].weight;
                minIndex1 = i;
            }
        }

        // Find second minimum weight node
        for (int i = 0; i < numNodes(); i++) {
            if (nodes[i].weight < min2 && i != minIndex1 && nodes[i].parent == 0 && nodes[i].childType == -1) {
                min2 = nodes[i].weight;
                minIndex2 = i;
            }
        }

        // Establish parent node weight
        int parentWeight = min1 + min2;

        // Assign child types
        nodes[minIndex1].childType = 0;
        nodes[minIndex2].childType = 1;

        // Create merged node
        nodes.push_back({'T', parentWeight, 0, -1});
        nodes[minIndex1].parent = numNodes() - 1;
        nodes[minIndex2].parent = numNodes() - 1;
    }

    built = true;  // Tree is now built
}

void HuffmanTree::uniqueCharsOnly(vector<HNode> &v) {
    map<char, bool> duplicate;
    vector<HNode> newVector;

    // Initialize map to track duplicates
    for (int i = 0; i < v.size(); i++) 
        duplicate[v[i].ch] = false;

    // Filter out duplicate characters
    for (int i = 0; i < v.size(); i++) {
        if (!duplicate[v[i].ch]) {
            duplicate[v[i].ch] = true;
            newVector.push_back(v[i]);
        }
    }

    v = newVector;  // Replace the input vector with the filtered vector
}

int HuffmanTree::getGreatestWeight(vector<HNode> &v) {
    int greatestWeight = 0;

    // Find the node with the greatest weight
    for (int i = 0; i < numNodes(); i++) {
        if (greatestWeight < nodes[i].weight) 
            greatestWeight = nodes[i].weight;
    }

    return greatestWeight;
}
