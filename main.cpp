/*
File: main.cpp
Author: Michael Bisulca
Description:
Appalachian Trail Data Processing Program
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <cctype>

using namespace std;

const int MAX_NAME_LENGTH = 15;
const int MAX_NUM_STATES = 14;

struct ATState {
    char name[MAX_NAME_LENGTH];
    double miles;
    int shelters;
};

void checkFile(ifstream &input, const string &filename);
void readfile(ifstream &input, ATState state[], int &linecount);
void changeFile(ATState state[], int linecount);
void totalMiles(ATState state[], int linecount, double &totalmiles, int &totalshelters);

int main() {
    ifstream input;
    string filename;
    ATState state[MAX_NUM_STATES];
    int linecount, totalshelters = 0;
    double totalmiles = 0;

    cout << "Welcome to the Appalachian Trail (AT) Information System\n";
    cout << setw(25) << "Enter AT filename: ";
    cin >> filename;

    input.open(filename);
    checkFile(input, filename);
    readfile(input, state, linecount);
    changeFile(state, linecount);
    totalMiles(state, linecount, totalmiles, totalshelters);

    cout << setw(11) << "State" << setw(18) << "Miles" << setw(10) << "Shelters\n";
    cout << setw(39) << "---------------------------------\n";

    for (int i = 0; i < linecount; i++) {
        cout << "      " << left << setw(17) << state[i].name
             << right << setw(5) << fixed << setprecision(1) << state[i].miles
             << setw(10) << state[i].shelters << endl;
    }

    cout << setw(39) << "---------------------------------\n";
    cout << setw(12) << "TOTALS" << setw(17) << totalmiles << setw(9) << totalshelters << endl;

    return 0;
}

void checkFile(ifstream &input, const string &filename) {
    if (input.fail()) {
        cout << "Input file " << filename << " does not exist.\n";
        exit(1);
    }

    string line;
    if (!getline(input, line)) {
        cout << "Input file " << filename << " is empty.\n";
        exit(1);
    }

    input.close();
    input.open(filename);
}

void readfile(ifstream &input, ATState state[], int &linecount) {
    linecount = 0;
    char line[100];

    while (linecount < MAX_NUM_STATES && input.getline(line, 100)) {
        strncpy(state[linecount].name, line, MAX_NAME_LENGTH - 1);
        state[linecount].name[MAX_NAME_LENGTH - 1] = '\0';

        input >> state[linecount].miles;
        input >> state[linecount].shelters;
        input.ignore(100, '\n');

        linecount++;
    }
}

void changeFile(ATState state[], int linecount) {
    for (int k = 0; k < linecount; k++) {
        bool startOfWord = true;

        for (int i = 0; state[k].name[i] != '\0'; i++) {
            if (isalpha(state[k].name[i])) {
                if (startOfWord) {
                    state[k].name[i] = toupper(state[k].name[i]);
                    startOfWord = false;
                } else {
                    state[k].name[i] = tolower(state[k].name[i]);
                }
            } else if (state[k].name[i] == ' ') {
                startOfWord = true;
            }
        }
    }
}

void totalMiles(ATState state[], int linecount, double &totalmiles, int &totalshelters) {
    totalmiles = 0.0;
    totalshelters = 0;

    for (int m = 0; m < linecount; m++) {
        totalmiles += state[m].miles;
        totalshelters += state[m].shelters;
    }

    // Sort alphabetically
    for (int k = 0; k < linecount; k++) {
        int minIndex = k;

        for (int l = k + 1; l < linecount; l++) {
            if (strcmp(state[l].name, state[minIndex].name) < 0) {
                minIndex = l;
            }
        }

        if (minIndex != k) {
            ATState temp = state[k];
            state[k] = state[minIndex];
            state[minIndex] = temp;
        }
    }
}