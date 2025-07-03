// main_logik.h
#pragma once
#include <vector>

class main_logik {
private:
    int height;
    int width;
    std::vector<std::vector<int>> fieldMatrix;

public:
    main_logik(int h, int w);
    void printField() const;
};
