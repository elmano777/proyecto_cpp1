/*
 * Define la estructura básica de una celda dentro de la hoja de cálculo.
 *
 * Cada celda almacena:
 * - Su posición (fila y columna)
 * - Su valor
 * - Punteros a la siguiente celda en su fila y en su columna
 *
 * Esto permite implementar una matriz dispersa (sparse matrix),
 * donde solo se almacenan las celdas que contienen datos.
 */

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
