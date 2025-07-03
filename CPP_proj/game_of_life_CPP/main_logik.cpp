// main_logik.cpp
#include "main_logik.h"
#include <iostream>

using namespace std;

main_logik::main_logik(int h, int w) : height(h), width(w) {
    fieldMatrix = vector<vector<int>>(height, vector<int>(width, 0));
}

void main_logik::printField() const {
    for (const auto& row : fieldMatrix) {
        for (int cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

