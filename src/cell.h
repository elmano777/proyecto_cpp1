#pragma once

#include <string>

using namespace std;

struct Cell {
  int row;
  int col;
  string value;
  Cell *nextInRow;
  Cell *nextInCol;

  Cell(int r, int c, const string &v)
      : row(r), col(c), value(v), nextInRow(nullptr), nextInCol(nullptr) {}
};
