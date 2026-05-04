/*
 * Implementa la lógica de la clase SpreadSheet.
 *
 * Contiene:
 * - Manejo de memoria (creación y destrucción de celdas)
 * - Inserción y eliminación en la estructura enlazada
 * - Búsqueda eficiente de celdas
 * - Operaciones sobre datos (sumas, promedios, etc.)
 * - Funciones auxiliares como parseo de números y limpieza de texto
 *
 * La estructura utilizada es una matriz dispersa enlazada por filas
 * y columnas, lo que permite ahorrar memoria al almacenar únicamente
 * las celdas que contienen valores.
 */

#include "spreadSheet.h"

#include <algorithm>
#include <cctype>
#include <limits>
#include <stdexcept>

using namespace std;

SpreadSheet::SpreadSheet(int rows, int cols)
    : rowHeads(nullptr), colHeads(nullptr), maxRows(rows), maxCols(cols) {
  if (rows <= 0 || cols <= 0) {
    throw invalid_argument(
        "La hoja debe tener al menos una fila y una columna.");
  }

  rowHeads = new Cell *[maxRows]();
  colHeads = new Cell *[maxCols]();
}

SpreadSheet::~SpreadSheet() {
  for (int r = 0; r < maxRows; ++r) {
    Cell *current = rowHeads[r];
    while (current != nullptr) {
      Cell *toDelete = current;
      current = current->nextInRow;
      delete toDelete;
    }
  }

  delete[] rowHeads;
  delete[] colHeads;
}

int SpreadSheet::rows() const { return maxRows; }

int SpreadSheet::cols() const { return maxCols; }

bool SpreadSheet::isValidCoordinate(int row, int col) const {
  return row >= 0 && row < maxRows && col >= 0 && col < maxCols;
}

bool SpreadSheet::normalizeRange(int &r1, int &c1, int &r2, int &c2) const {
  if (r1 > r2)
    swap(r1, r2);
  if (c1 > c2)
    swap(c1, c2);

  if (r2 < 0 || r1 >= maxRows || c2 < 0 || c1 >= maxCols) {
    return false;
  }

  r1 = max(r1, 0);
  c1 = max(c1, 0);
  r2 = min(r2, maxRows - 1);
  c2 = min(c2, maxCols - 1);
  return true;
}

Cell *SpreadSheet::findCell(int row, int col) const {
  if (!isValidCoordinate(row, col)) {
    return nullptr;
  }

  Cell *current = rowHeads[row];
  while (current != nullptr && current->col < col) {
    current = current->nextInRow;
  }

  if (current != nullptr && current->col == col) {
    return current;
  }
  return nullptr;
}

string SpreadSheet::trim(const string &text) {
  int left = 0;
  int right = static_cast<int>(text.size()) - 1;

  while (left <= right && isspace(static_cast<unsigned char>(text[left]))) {
    ++left;
  }
  while (right >= left && isspace(static_cast<unsigned char>(text[right]))) {
    --right;
  }

  return text.substr(left, right - left + 1);
}

bool SpreadSheet::parseNumber(const string &text, double &out) {
  string cleaned = trim(text);
  if (cleaned.empty()) {
    return false;
  }

  try {
    size_t used = 0;
    out = stod(cleaned, &used);
    return used == cleaned.size();
  } catch (...) {
    return false;
  }
}

bool SpreadSheet::insertCell(int row, int col, const string &value) {
  if (!isValidCoordinate(row, col)) {
    return false;
  }

  if (value.empty()) {
    return deleteCell(row, col);
  }

  Cell *existing = findCell(row, col);
  if (existing != nullptr) {
    existing->value = value;
    return true;
  }

  Cell *node = new Cell(row, col, value);

  if (rowHeads[row] == nullptr || rowHeads[row]->col > col) {
    node->nextInRow = rowHeads[row];
    rowHeads[row] = node;
  } else {
    Cell *previous = rowHeads[row];
    while (previous->nextInRow != nullptr && previous->nextInRow->col < col) {
      previous = previous->nextInRow;
    }
    node->nextInRow = previous->nextInRow;
    previous->nextInRow = node;
  }

  if (colHeads[col] == nullptr || colHeads[col]->row > row) {
    node->nextInCol = colHeads[col];
    colHeads[col] = node;
  } else {
    Cell *previous = colHeads[col];
    while (previous->nextInCol != nullptr && previous->nextInCol->row < row) {
      previous = previous->nextInCol;
    }
    node->nextInCol = previous->nextInCol;
    previous->nextInCol = node;
  }

  return true;
}

bool SpreadSheet::modifyCell(int row, int col, const string &value) {
  Cell *cell = findCell(row, col);
  if (cell == nullptr) {
    return false;
  }

  if (value.empty()) {
    return deleteCell(row, col);
  }

  cell->value = value;
  return true;
}

string SpreadSheet::getCell(int row, int col) const {
  Cell *cell = findCell(row, col);
  return cell == nullptr ? "" : cell->value;
}

bool SpreadSheet::cellExists(int row, int col) const {
  return findCell(row, col) != nullptr;
}

bool SpreadSheet::deleteCell(int row, int col) {
  if (!isValidCoordinate(row, col)) {
    return false;
  }

  Cell *target = nullptr;

  if (rowHeads[row] != nullptr && rowHeads[row]->col == col) {
    target = rowHeads[row];
    rowHeads[row] = target->nextInRow;
  } else {
    Cell *previous = rowHeads[row];
    while (previous != nullptr && previous->nextInRow != nullptr &&
           previous->nextInRow->col < col) {
      previous = previous->nextInRow;
    }

    if (previous == nullptr || previous->nextInRow == nullptr ||
        previous->nextInRow->col != col) {
      return false;
    }

    target = previous->nextInRow;
    previous->nextInRow = target->nextInRow;
  }

  if (colHeads[col] == target) {
    colHeads[col] = target->nextInCol;
  } else {
    Cell *previous = colHeads[col];
    while (previous != nullptr && previous->nextInCol != nullptr &&
           previous->nextInCol != target) {
      previous = previous->nextInCol;
    }

    if (previous != nullptr && previous->nextInCol == target) {
      previous->nextInCol = target->nextInCol;
    }
  }

  delete target;
  return true;
}

bool SpreadSheet::deleteRow(int row) {
  if (row < 0 || row >= maxRows) {
    return false;
  }

  bool deletedSomething = false;
  Cell *current = rowHeads[row];
  while (current != nullptr) {
    Cell *next = current->nextInRow;
    deleteCell(row, current->col);
    deletedSomething = true;
    current = next;
  }
  return deletedSomething;
}

bool SpreadSheet::deleteCol(int col) {
  if (col < 0 || col >= maxCols) {
    return false;
  }

  bool deletedSomething = false;
  Cell *current = colHeads[col];
  while (current != nullptr) {
    Cell *next = current->nextInCol;
    deleteCell(current->row, col);
    deletedSomething = true;
    current = next;
  }
  return deletedSomething;
}

bool SpreadSheet::deleteRange(int r1, int c1, int r2, int c2) {
  if (!normalizeRange(r1, c1, r2, c2)) {
    return false;
  }

  bool deletedSomething = false;
  for (int r = r1; r <= r2; ++r) {
    Cell *current = rowHeads[r];
    while (current != nullptr) {
      Cell *next = current->nextInRow;
      if (current->col >= c1 && current->col <= c2) {
        deleteCell(r, current->col);
        deletedSomething = true;
      }
      current = next;
    }
  }
  return deletedSomething;
}

double SpreadSheet::sumRow(int row) const {
  if (row < 0 || row >= maxRows) {
    return 0.0;
  }

  double total = 0.0;
  Cell *current = rowHeads[row];
  while (current != nullptr) {
    double value = 0.0;
    if (parseNumber(current->value, value)) {
      total += value;
    }
    current = current->nextInRow;
  }
  return total;
}

double SpreadSheet::sumCol(int col) const {
  if (col < 0 || col >= maxCols) {
    return 0.0;
  }

  double total = 0.0;
  Cell *current = colHeads[col];
  while (current != nullptr) {
    double value = 0.0;
    if (parseNumber(current->value, value)) {
      total += value;
    }
    current = current->nextInCol;
  }
  return total;
}

double SpreadSheet::sumRange(int r1, int c1, int r2, int c2) const {
  if (!normalizeRange(r1, c1, r2, c2)) {
    return 0.0;
  }

  double total = 0.0;
  for (int r = r1; r <= r2; ++r) {
    Cell *current = rowHeads[r];
    while (current != nullptr) {
      if (current->col >= c1 && current->col <= c2) {
        double value = 0.0;
        if (parseNumber(current->value, value)) {
          total += value;
        }
      }
      current = current->nextInRow;
    }
  }
  return total;
}

optional<double> SpreadSheet::averageRow(int row) const {
  if (row < 0 || row >= maxRows) {
    return nullopt;
  }

  double total = 0.0;
  int count = 0;
  Cell *current = rowHeads[row];
  while (current != nullptr) {
    double value = 0.0;
    if (parseNumber(current->value, value)) {
      total += value;
      ++count;
    }
    current = current->nextInRow;
  }

  if (count == 0)
    return nullopt;
  return total / count;
}

optional<double> SpreadSheet::averageCol(int col) const {
  if (col < 0 || col >= maxCols) {
    return nullopt;
  }

  double total = 0.0;
  int count = 0;
  Cell *current = colHeads[col];
  while (current != nullptr) {
    double value = 0.0;
    if (parseNumber(current->value, value)) {
      total += value;
      ++count;
    }
    current = current->nextInCol;
  }

  if (count == 0)
    return nullopt;
  return total / count;
}

optional<double> SpreadSheet::averageRange(int r1, int c1, int r2,
                                           int c2) const {
  if (!normalizeRange(r1, c1, r2, c2)) {
    return nullopt;
  }

  double total = 0.0;
  int count = 0;
  for (int r = r1; r <= r2; ++r) {
    Cell *current = rowHeads[r];
    while (current != nullptr) {
      if (current->col >= c1 && current->col <= c2) {
        double value = 0.0;
        if (parseNumber(current->value, value)) {
          total += value;
          ++count;
        }
      }
      current = current->nextInRow;
    }
  }

  if (count == 0)
    return nullopt;
  return total / count;
}

optional<double> SpreadSheet::maxRange(int r1, int c1, int r2, int c2) const {
  if (!normalizeRange(r1, c1, r2, c2)) {
    return nullopt;
  }

  bool found = false;
  double answer = numeric_limits<double>::lowest();
  for (int r = r1; r <= r2; ++r) {
    Cell *current = rowHeads[r];
    while (current != nullptr) {
      if (current->col >= c1 && current->col <= c2) {
        double value = 0.0;
        if (parseNumber(current->value, value)) {
          answer = max(answer, value);
          found = true;
        }
      }
      current = current->nextInRow;
    }
  }

  if (!found)
    return nullopt;
  return answer;
}

optional<double> SpreadSheet::minRange(int r1, int c1, int r2, int c2) const {
  if (!normalizeRange(r1, c1, r2, c2)) {
    return nullopt;
  }

  bool found = false;
  double answer = numeric_limits<double>::max();
  for (int r = r1; r <= r2; ++r) {
    Cell *current = rowHeads[r];
    while (current != nullptr) {
      if (current->col >= c1 && current->col <= c2) {
        double value = 0.0;
        if (parseNumber(current->value, value)) {
          answer = min(answer, value);
          found = true;
        }
      }
      current = current->nextInRow;
    }
  }

  if (!found)
    return nullopt;
  return answer;
}

int SpreadSheet::numericCountRange(int r1, int c1, int r2, int c2) const {
  if (!normalizeRange(r1, c1, r2, c2)) {
    return 0;
  }

  int count = 0;
  for (int r = r1; r <= r2; ++r) {
    Cell *current = rowHeads[r];
    while (current != nullptr) {
      if (current->col >= c1 && current->col <= c2) {
        double value = 0.0;
        if (parseNumber(current->value, value)) {
          ++count;
        }
      }
      current = current->nextInRow;
    }
  }
  return count;
}

vector<CellInfo> SpreadSheet::getOccupiedCells() const {
  vector<CellInfo> cells;
  for (int r = 0; r < maxRows; ++r) {
    Cell *current = rowHeads[r];
    while (current != nullptr) {
      cells.push_back({current->row, current->col, current->value});
      current = current->nextInRow;
    }
  }
  return cells;
}
