#pragma once

#include <string>

struct Cell {
    int row;
    int col;
    std::string value;
    Cell* nextInRow;
    Cell* nextInCol;

    Cell(int r, int c, const std::string& v)
        : row(r), col(c), value(v), nextInRow(nullptr), nextInCol(nullptr) {}
};
