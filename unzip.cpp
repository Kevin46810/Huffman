// Program to read a zip-encoded file and "inflate" it back to the original text
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <cctype>
#include <cstdio>

using namespace std;

// Function declarations
bool hasZipExtension(string &fileName);
string getNewFileName(string &fileName);
string getOriginalText(ifstream &sourceFile, map<string, int> &map);

int main(int argCount, const char* args[]) {

    // Check if the correct number of arguments is provided
    if (argCount != 2) {
        cout << "Invalid input. Enter a file name." << endl;
        return 0;
    }

    // Open the source file for reading
    ifstream sourceFile(args[1]);

    // Check if the file exists
    if (!sourceFile) {
        cout << "File not found." << endl;
        return 0;
    }

    string fileName = args[1];

    // Validate that the file has a ".zip" extension
    if (!hasZipExtension(fileName)) {
        cout << "File must have zip extension." << endl;
        return 0;
    }

    // Create the output file with the new file name (without ".zip" extension)
    ofstream newFile(getNewFileName(fileName));
    map<string, int> map;  // Map to store Huffman-like codes and corresponding ASCII values

    // Decode the original text from the source file
    string output = getOriginalText(sourceFile, map);
    sourceFile.close();

    // Write the inflated text to the new file
    newFile << output; 
    cout << "File inflated successfully!" << endl;
}

// Function to check if the file has a ".zip" extension
bool hasZipExtension(string &fileName) {
    return (fileName.size() > 4 && fileName.substr(fileName.size() - 4) == ".zip");
}

// Function to generate the new file name by removing the ".zip" extension
string getNewFileName(string &fileName) {
    return fileName.substr(0, fileName.size() - 4);
}

// Function to decode the original text from the source file
string getOriginalText(ifstream &sourceFile, map<string, int> &map) {
    string inputLine;
    string line;
    string code;
    string asciiString;
    int ascii;
    string gluedCode;
    string tempCode;
    string ogString;
    int count = 0;

    // Process each line of the source file
    while (getline(sourceFile, inputLine)) {
        line = inputLine;

        // Skip the first line
        if (count >= 1) {
            int found = line.find(' ');

            // If no space is found, it's a sequence of encoded characters
            if (found == -1) {
                gluedCode = line;

                // Decode the sequence based on the map
                for (int j = 0; j < gluedCode.length(); j++) {
                    tempCode += gluedCode[j];

                    // If the code is found in the map, append the corresponding character
                    if (map.find(tempCode) != map.end()) {
                        char c(map[tempCode]);
                        ogString += c;
                        tempCode = "";  // Reset temporary code for next sequence
                    }
                }
            } 
	    else {
                // Parse the line for ASCII value and its corresponding code
                for (int j = 0; line[j] != ' '; j++) {
                    asciiString += line[j];
                }

                try {
                    // Convert the extracted string to an integer
                    ascii = stoi(asciiString);
                } 
		catch (invalid_argument e) {
                    cout << "Invalid ASCII value: " << asciiString << "." << endl;
                }

                // Extract the code after the ASCII value
                for (int j = asciiString.length() + 1; j < line.length(); j++) {
                    code += line[j];
                }

                // Store the code and its corresponding ASCII value in the map
                asciiString = "";
                map[code] = ascii;
                code = "";
            }
        }
        count++;
    }

    // Return the fully decoded original text
    return ogString;
}
