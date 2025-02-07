// Program to compress a text file using Huffman encoding
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>

using namespace std;

#include "huffman.h"
#include "exceptions.h"

// Function declarations
vector<int> convertToAscii(string &line);
vector<string> convertToCode(string &line, HuffmanTree &h, vector<int> ascii);
void insertData(string &line, string &fileName, int &numChars, vector<int> &ascii, vector<string> &codes);
void compress(string &line, HuffmanTree &h, vector<int> &ascii, vector<string> &codes);
int numUniqueChars(string &line);
int getFrequency(char &c, string &line);

int main(int argLength, const char *args[]) {
    HuffmanTree h;

    // Exit if the command line arguments are invalid
    if (argLength > 3 || argLength < 2) {
        cout << "Invalid args." << endl;
        return 0;
    }

    string inputLine;
    string line;
    int index = 1;

    // Skip extra command-line argument if provided
    if (argLength == 3)
        index++;

    ifstream sourceFile(args[index]);

    // Check if the input file can be opened
    if (!sourceFile) {
        cout << "File not found." << endl;
        return 0;
    }

    // Read the entire file content into the string `line`
    while (getline(sourceFile, inputLine))
        line += inputLine + '\n';

    // Map to track duplicate characters
    map<char, bool> duplicate;

    // Initialize the map with all characters in the input line
    for (int i = 0; i < line.length(); i++)
        duplicate[line[i]] = false;

    // Insert unique characters and their frequencies into the Huffman Tree
    for (int i = 0; i < line.length(); i++) {
        if (!duplicate[line[i]]) {
            duplicate[line[i]] = true;
            h.insert(line[i], getFrequency(line[i], line));
        }
    }

    // Build the Huffman Tree based on character frequencies
    h.build();

    if (argLength == 3) {
        string args1 = args[1];

        // Print the Huffman table if the "--t" argument is provided
        if (args1 == "--t")
            h.PrintTable();
        else {
            cout << "Invalid command." << endl;
            return 0;
        }
    }

    string fileName = args[index];

    if (fileName == "--help") {
        cout << "--t       Display Huffman table." << endl;
        cout << "--help    Get help with commands." << endl;
    }

    // Prevent compression of files that are already compressed
    int found = fileName.find(".zip");
    if (found != -1) {
        cout << "Cannot zip another zip file. Program terminated." << endl;
        return 0;
    }

    int chars = numUniqueChars(line);
    vector<int> ascii = convertToAscii(line);
    vector<string> codes = convertToCode(line, h, ascii);

    // Insert encoded data into a new zip file
    insertData(line, fileName, chars, ascii, codes);

    // Compress the file and display the compression statistics
    compress(line, h, ascii, codes);

    // Remove the original file after compression
    remove(args[index]);

    sourceFile.close();
}

vector<int> convertToAscii(string &line) {
    vector<int> vector;
    map<char, bool> duplicate;

    // Initialize the map with unique characters from the line
    for (int i = 0; i < line.length(); i++)
        duplicate[line[i]] = false;

    // Convert unique characters to their ASCII values
    for (int i = 0; i < line.length(); i++) {
        if (!duplicate[line[i]]) {
            duplicate[line[i]] = true;
            char c = line[i];
            int a = c;
            vector.push_back(a);
        }
    }

    // Sort the ASCII values in ascending order
    sort(vector.begin(), vector.end());

    return vector;
}

vector<string> convertToCode(string &line, HuffmanTree &h, vector<int> ascii) {
    vector<string> vector;
    map<char, bool> duplicate;

    // Initialize the map with unique characters from the line
    for (int i = 0; i < line.length(); i++)
        duplicate[line[i]] = false;

    // Map each unique character to its corresponding Huffman code
    for (int i = 0; i < line.length(); i++) {
        if (!duplicate[line[i]]) {
            duplicate[line[i]] = true;
            for (int j = 0; j < ascii.size(); j++) {
                if (line[i] == char(ascii[j])) {
                    string temp = h.GetCode(line[i]);
                    vector.push_back(temp);
                    break;
                }
            }
        }
    }

    return vector;
}

void insertData(string &line, string &fileName, int &numChars, vector<int> &ascii, vector<string> &codes) {
    ofstream zipFile(fileName + ".zip");

    // Write the number of unique characters
    zipFile << numChars << endl;

    // Write the ASCII values and their corresponding Huffman codes
    for (int i = 0; i < numChars; i++)
        zipFile << ascii[i] << " " << codes[i] << endl;

    map<char, string> map;

    // Create a map for fast lookup of Huffman codes
    for (int i = 0; i < ascii.size() && i < codes.size(); i++)
        map[char(ascii[i])] = codes[i];

    // Write the compressed data to the file
    for (int i = 0; i < line.length(); i++)
        zipFile << map[line[i]];

    zipFile << endl;
}

void compress(string &line, HuffmanTree &h, vector<int> &ascii, vector<string> &codes) {
    int bitsize = 0;

    // Calculate the total bit size of the compressed data
    for (int i = 0; i < ascii.size() && i < codes.size(); i++) {
        bitsize += h.GetFrequency(char(ascii[i])) * codes[i].length();
    }

    double ratio;

    try {
        // Compute the compression ratio
        ratio = (1 - ((double)bitsize / (line.length() * 8)));
    } catch (DivisionByZero e) {
        cout << "Division by zero error." << endl;
    }

    printf("File compressed to %d bits (%.2f%% less).\n", bitsize, ratio * 100);
}

int numUniqueChars(string &line) {
    map<char, int> map;

    // Initialize the map with all characters from the line
    for (int i = 0; i < line.length(); i++)
        map[line[i]] = 0;

    int count = 0;

    // Count the number of unique characters
    for (int i = 0; i < line.length(); i++) {
        if (map[line[i]] < 1) {
            map[line[i]]++;
            count++;
        }
    }

    return count;
}

int getFrequency(char &c, string &line) {
    int freq = 0;

    // Count the frequency of the character in the line
    for (int i = 0; i < line.length(); i++) {
        if (line[i] == c)
            freq++;
    }

    return freq;
}
