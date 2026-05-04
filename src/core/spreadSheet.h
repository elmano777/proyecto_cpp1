/*
 * Define la clase SpreadSheet, que representa una hoja de cálculo
 * implementada como una matriz dispersa.
 *
 * Responsabilidades principales:
 * - Gestionar inserción, modificación y eliminación de celdas
 * - Mantener la estructura enlazada por filas y columnas
 * - Proveer operaciones como suma, promedio, mínimo y máximo
 * - Validar rangos y coordenadas
 */

#pragma once

#include "cell.h"
#include <optional>
#include <string>
#include <vector>

using namespace std;

struct CellInfo {
  int row;
  int col;
  string value;
};

class SpreadSheet {
private:
  Cell **rowHeads;
  Cell **colHeads;
  int maxRows;
  int maxCols;

  bool isValidCoordinate(int row, int col) const;
  bool normalizeRange(int &r1, int &c1, int &r2, int &c2) const;
  Cell *findCell(int row, int col) const;
  static string trim(const string &text);
  static bool parseNumber(const string &text, double &out);

public:
  SpreadSheet(int rows, int cols);
  ~SpreadSheet();

  SpreadSheet(const SpreadSheet &) = delete;
  SpreadSheet &operator=(const SpreadSheet &) = delete;

  int rows() const;
  int cols() const;

  bool insertCell(int row, int col, const string &value);
  bool modifyCell(int row, int col, const string &value);
  string getCell(int row, int col) const;
  bool cellExists(int row, int col) const;

  bool deleteCell(int row, int col);
  bool deleteRow(int row);
  bool deleteCol(int col);
  bool deleteRange(int r1, int c1, int r2, int c2);

  double sumRow(int row) const;
  double sumCol(int col) const;
  double sumRange(int r1, int c1, int r2, int c2) const;

  optional<double> averageRow(int row) const;
  optional<double> averageCol(int col) const;
  optional<double> averageRange(int r1, int c1, int r2, int c2) const;
  optional<double> maxRange(int r1, int c1, int r2, int c2) const;
  optional<double> minRange(int r1, int c1, int r2, int c2) const;

  int numericCountRange(int r1, int c1, int r2, int c2) const;
  vector<CellInfo> getOccupiedCells() const;
};
