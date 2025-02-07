//My code
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <cctype>
#include <cstdio>

using namespace std;

bool hasZipExtension(string &fileName);
string getNewFileName(string &fileName);
string getOriginalText (ifstream &sourceFile, map<string, int> &map);

int main(int argCount, const char* args[]) {

	if (argCount != 2) {
		cout << "Invalid input.  Enter a file name." << endl;
		return 0;
	}
	ifstream sourceFile(args[1]);

	if (!sourceFile) {
		cout << "File not found." << endl;
		return 0;
	}
	string fileName = args[1];

	if (!hasZipExtension(fileName)) {
		cout << "File must have zip extension." << endl;
		return 0;
	}

	ofstream newFile(getNewFileName(fileName));
	map<string, int> map;

	string output = getOriginalText(sourceFile, map);
	sourceFile.close();

	newFile << output; 
	cout << "File inflated successfully!" << endl;
}

bool hasZipExtension (string &fileName) {

	return (fileName.size() > 4 && fileName.substr(fileName.size() - 4) == ".zip");
}

string getNewFileName (string &fileName) {

	return fileName.substr(0, fileName.size() - 4);
}

string getOriginalText (ifstream &sourceFile, map<string, int> &map) {

	string inputLine;
	string line;
	string code;
	string asciiString;
	int ascii;
	string gluedCode;
	string tempCode;
	string ogString;

	int count = 0;

	while (getline(sourceFile, inputLine)) {

		line = inputLine;

		if (count >= 1) {

			int found = line.find(' ');

			if (found == -1) {
				gluedCode = line;
				for (int j = 0; j < gluedCode.length(); j++) {
					tempCode += gluedCode[j];

					if (map.find(tempCode) != map.end()) {
						char c(map[tempCode]);
						ogString += c;
						tempCode = "";
					}
				}
			}
			else {
				for (int j = 0; line[j] != ' '; j++)
					asciiString += line[j];

				try {
					ascii = stoi(asciiString);
				}
				catch (invalid_argument e) {
					cout << "Invalid ASCII value: " << asciiString << "." << endl;
				}
				for (int j = asciiString.length() + 1; j < line.length(); j++)
					code += line[j];

				asciiString = "";
				map[code] = ascii;
				code = "";
			}
		}
		count++;
	}
	return ogString;
}
