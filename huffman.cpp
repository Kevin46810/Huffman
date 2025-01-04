#include <iostream>
#include <map>
#include <fstream>
#include <queue>
#include "huffman.h"

using namespace std;

HuffmanTree:: HuffmanTree()
        : numChars(0), built(false) {}

void HuffmanTree:: insert(char ch, int weight) {
    HNode newNode = {ch, weight, 0, -1}; 
    built = false;
    nodes.push_back(newNode);
    numChars++;

}

bool HuffmanTree:: inTree(char ch) {

        //Tree must be built to run
        if (!built)
                return false;

        //Searches for character.  Returns false if not found
        for (int i = 0; i < nodes.size(); i++) {
                if (ch == nodes[i].ch) 
                        return true;

}
        return false;

}

int HuffmanTree:: GetFrequency(char ch) {

        //Finds character in its index and calls the private function
        for (int i = 0; i < numNodes(); i++) {
                if (ch == nodes[i].ch) 
                        return GetFrequency(i);

}
        return 0;
}

int HuffmanTree:: GetFrequency(int i) {

        return nodes[i].weight;
}

int HuffmanTree:: lookUp(char ch) {

        if (!built)
                return -1;

        //Returns the index of the chosen character
        for (int i = 0; i < numNodes(); i++) {
                if (nodes[i].ch == ch) {
                        return i;
}
}
        return -1;
}

string HuffmanTree:: GetCode(char ch) {

        if (!built)
                return "";

        return GetCode(lookUp(ch));
}


string HuffmanTree:: GetCode(int i) {

        if (nodes[i].parent == 0) 
                return "";

        if (nodes[i].childType == 0)
                return GetCode(nodes[i].parent) + "0";

        else 
                return GetCode(nodes[i].parent) + "1";

}

void HuffmanTree:: PrintTable() {

        //Tree must be built to run
        if (!built) {
                cout << "Tree not built." << endl;
                return;
}

        cout << "index  char   freq  parent   childType" << endl;
        cout << "--------------------------------------" << endl;

        int TCount = 1;

        for (int i = 0; i < numNodes(); i++) {

                if (i < 10)
                        cout << "   " << i;

                else if (i < 100)
                        cout << "  " << i;

                else
                        cout << " " << i;

                cout << "   ";

                if (i >= numNodes()/2 + 1) {
                        cout << "  T" << TCount; 
                        TCount++;
}
                else if (nodes[i].ch == '\n')
                        cout << "  nl";

                else if (nodes[i].ch == ' ')
                        cout << "  sp";

                else
                        cout << "   " << nodes[i].ch;

                if (nodes[i].weight <= 9)
                        cout << "       " << nodes[i].weight;

                else if (nodes[i].weight > 99)
                        cout << "     " << nodes[i].weight;

                else
                        cout << "      " << nodes[i].weight;


                if (nodes[i].parent <= 9 && nodes[i].parent > -1)
                        cout << "       " << nodes[i].parent;

                else if (nodes[i].parent > 99)
                        cout << "     " << nodes[i].parent;

                else
                        cout << "      " << nodes[i].parent;

                if (nodes[i].childType != -1)
                        cout << "         " << nodes[i].childType;
                else
                        cout << "       N/A";

                cout << endl; 

        //All if else statements are for formatting
}
}

int HuffmanTree:: numNodes() {

        return nodes.size();
}

void HuffmanTree:: build() {

        int freq;

        if (built)
                return;

        map<char, bool> map;

        for (int i = 0; i < numNodes(); i++)
                map[nodes[i].ch] = false;

        uniqueCharsOnly(nodes); //Gets rid of duplicate characters originally from the string
        int c = numNodes(); //For number of iterations in the while loop

        HNode TNode;

        int minIndex1 = -1;
        int minIndex2 = -1;

        while (numNodes() < 2*c-1) {

                //Required for finding all the minimums
                int min1 = getGreatestWeight(nodes) + 1; 
                int min2 = getGreatestWeight(nodes) + 1;

                //Find first min
                for (int i = 0; i < numNodes(); i++) {
                        if (nodes[i].weight < min1 && i != minIndex2 && nodes[i].parent == 0 && nodes[i].childType == -1) {
                                min1 = nodes[i].weight;
                                minIndex1 = i;
}
}
                //Find second min
                for (int i = 0; i < numNodes(); i++) {
                        if (nodes[i].weight < min2 && i != minIndex1 && nodes[i].parent == 0 && nodes[i].childType == -1) {
                                min2 = nodes[i].weight;
                                minIndex2 = i;
}
}
                //Establish parent weight
                int parentWeight = min1 + min2;

                //Assign child types
                nodes[minIndex1].childType = 0;
                nodes[minIndex2].childType = 1;

                //Create merged node

                //Only activates on the final iteration

                nodes.push_back({'T', parentWeight, 0, -1});

                nodes[minIndex1].parent = numNodes() - 1;
                nodes[minIndex2].parent = numNodes() - 1;

}
        built = true; //Tree now built
}

void HuffmanTree::uniqueCharsOnly (vector<HNode> &v) {

        map<char, bool> duplicate;
        vector<HNode> newVector;

        //Initialize map
        for (int i = 0; i < v.size(); i++) 
                duplicate[v[i].ch] = false;

        //Weed out all duplicates
        for (int i = 0; i < v.size(); i++) {
                if (!duplicate[v[i].ch]) {
                        duplicate[v[i].ch] = true;
                        newVector.push_back(v[i]);
}
}
        v = newVector; //Make the vector passed in the created vector
}

int HuffmanTree::getGreatestWeight (vector<HNode> &v) {

        int greatestWeight = 0;

        for (int i = 0; i < numNodes(); i++) {
                if (greatestWeight < nodes[i].weight) 
                        greatestWeight = nodes[i].weight;
}

        return greatestWeight;

}
