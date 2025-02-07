//My code
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

vector<int> convertToAscii(string &line);
vector<string> convertToCode (string &line, HuffmanTree& h, vector<int> ascii);
void insertData (string &line, string &fileName, int &numChars, vector<int> &ascii, vector<string> &codes);
void compress (string &line, HuffmanTree& h, vector<int> &ascii, vector<string> &codes);
int numUniqueChars (string &line);
int getFrequency (char &c, string &line);

int main(int argLength, const char* args[]) {

	HuffmanTree h;

	//Exits if command line is invalid
	if (argLength > 3 || argLength < 2) {
		cout << "Invalid args." << endl;
		return 0;
	}

	string inputLine;
	string line;

	int index = 1;

	if (argLength == 3) 
		index++;

	ifstream sourceFile(args[index]);

	if (!sourceFile) { 
		cout << "File not found." << endl;
		return 0;
	}

	while (getline(sourceFile, inputLine)) 
		line += inputLine + '\n';

	map<char, bool> duplicate;

	for (int i = 0; i < line.length(); i++)
		duplicate[line[i]] = false;

	for (int i = 0; i < line.length(); i++) {
		if (!duplicate[line[i]]) {
			duplicate[line[i]] = true;
			h.insert(line[i], getFrequency(line[i], line));
		}
	}

	h.build();

	if (argLength == 3) {
		string args1 = args[1];

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

	int found = fileName.find(".zip");

	if (found != -1) {
		cout << "Cannot zip another zip file. Program terminated." << endl;
		return 0;
	}
	int chars = numUniqueChars(line);

	vector<int> ascii = convertToAscii(line);

	vector<string> codes = convertToCode(line, h, ascii);

	insertData(line, fileName, chars, ascii, codes);

	compress(line, h, ascii, codes);

	remove(args[index]);

	sourceFile.close();
}


vector<int> convertToAscii (string &line) {

	vector<int> vector;
	map<char, bool> duplicate;

	for (int i = 0; i < line.length(); i++)
		duplicate[line[i]] = false;


	for (int i = 0; i < line.length(); i++) {
		if (!duplicate[line[i]]) {
			duplicate[line[i]] = true;
			char c = line[i];
			int a = c;
			vector.push_back(a);


		}
	}

	sort(vector.begin(), vector.end());

	return vector;
}
vector<string> convertToCode (string &line, HuffmanTree& h, vector<int> ascii) {

	vector<string> vector;
	map<char, bool> duplicate;

	for (int i = 0; i < line.length(); i++)
		duplicate[line[i]] = false;

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

void insertData (string &line,string &fileName, int &numChars, vector<int> &ascii, vector<string> &codes) {

	ofstream zipFile(fileName + ".zip");

	zipFile << numChars << endl;

	for (int i = 0; i < numChars; i++)
		zipFile << ascii[i] << " " << codes[i] << endl;

	map<char, string> map;

	for (int i = 0; i < ascii.size() && i < codes.size(); i++)
		map[char(ascii[i])] = codes[i];

	for (int i = 0; i < line.length(); i++)
		zipFile << map[line[i]];

	zipFile << endl;
}

void compress (string &line, HuffmanTree& h, vector<int> &ascii, vector<string> &codes) {

	int bitsize = 0;

	for (int i = 0; i < ascii.size() && i < codes.size(); i++) { 
		bitsize += h.GetFrequency(char(ascii[i])) * codes[i].length();
	}

	double ratio;

	try {
		ratio = (1 - ( (double) bitsize/(line.length() * 8)));

	}

	catch (DivisionByZero e) {
		cout << "Division by zero error." << endl;
	}

	printf("File compressed to %d bits (%.2f%% less).\n", bitsize, ratio*100);

}

int numUniqueChars (string &line) {

	map<char, int> map;

	for (int i = 0; i < line.length(); i++)
		map[line[i]] = 0;

	int count = 0;

	for (int i = 0; i < line.length(); i++) {
		if (map[line[i]] < 1) {
			map[line[i]]++;
			count++;
		}
	}
	return count;
}

int getFrequency (char &c, string &line) {

	int freq = 0;

	for (int i = 0; i < line.length(); i++) {
		if (line[i] == c)
			freq++;
	}
	return freq;
}
