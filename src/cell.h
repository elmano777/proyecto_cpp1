#pragma once
#include <string>

using namespace std;

struct Cell {
  int row, col;
  string value;
  Cell *nextInRow;
  Cell *nextInCol;

  Cell(int r, int c, const std::string &v)
      : row(r), col(c), value(v), nextInRow(nullptr), nextInCol(nullptr) {}
};
