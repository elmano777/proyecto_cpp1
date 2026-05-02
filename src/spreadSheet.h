#pragma once

#include "cell.h"
#include <optional>
#include <string>
#include <vector>

struct CellInfo {
  int row;
  int col;
  std::string value;
};

class SpreadSheet {
private:
  Cell** rowHeads;
  Cell** colHeads;
  int maxRows;
  int maxCols;

  bool isValidCoordinate(int row, int col) const;
  bool normalizeRange(int& r1, int& c1, int& r2, int& c2) const;
  Cell* findCell(int row, int col) const;
  static std::string trim(const std::string& text);
  static bool parseNumber(const std::string& text, double& out);

public:
  SpreadSheet(int rows, int cols);
  ~SpreadSheet();

  SpreadSheet(const SpreadSheet&) = delete;
  SpreadSheet& operator=(const SpreadSheet&) = delete;

  int rows() const;
  int cols() const;

  bool insertCell(int row, int col, const std::string& value);
  bool modifyCell(int row, int col, const std::string& value);
  std::string getCell(int row, int col) const;
  bool cellExists(int row, int col) const;

  bool deleteCell(int row, int col);
  bool deleteRow(int row);
  bool deleteCol(int col);
  bool deleteRange(int r1, int c1, int r2, int c2);

  double sumRow(int row) const;
  double sumCol(int col) const;
  double sumRange(int r1, int c1, int r2, int c2) const;

  std::optional<double> averageRow(int row) const;
  std::optional<double> averageCol(int col) const;
  std::optional<double> averageRange(int r1, int c1, int r2, int c2) const;
  std::optional<double> maxRange(int r1, int c1, int r2, int c2) const;
  std::optional<double> minRange(int r1, int c1, int r2, int c2) const;

  int numericCountRange(int r1, int c1, int r2, int c2) const;
  std::vector<CellInfo> getOccupiedCells() const;
  void printSheet() const;
};
