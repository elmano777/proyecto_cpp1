#pragma once
#include "cell.h"
#include <string>

using namespace std;

class SpreadSheet {
private:
  Cell **rowHeads;
  Cell **colHeads;
  int maxRows, maxCols;

public:
  SpreadSheet(int rows, int cols);
  ~SpreadSheet();

  void insertCell(int row, int col, const std::string &value);
  string getCell(int row, int col);
  void deleteCell(int row, int col);
  void deleteRow(int row);
  void deleteCol(int col);

  double sumRow(int row);
  double sumCol(int col);
  double sumRange(int r1, int c1, int r2, int c2);
  double avgRange(int r1, int c1, int r2, int c2);
  double maxRange(int r1, int c1, int r2, int c2);
  double minRange(int r1, int c1, int r2, int c2);

  void printSheet();
};
