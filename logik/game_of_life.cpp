#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> initField(int fieldHeight, int fieldWidth) {
    vector<vector<int>> fieldMatrix(fieldHeight, vector<int>(fieldWidth, 0));
    return fieldMatrix;
}

int main() {
    int height = 3;
    int width = 4;
    
    vector<vector<int>> fieldMatrix = initField(height, width);
    
    
    
    return 0;
}