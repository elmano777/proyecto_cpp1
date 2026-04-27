#include "spreadSheet.h"
#include <iostream>
#include <stdexcept>
#include <climits>

SpreadSheet::SpreadSheet(int rows, int cols)
    : maxRows(rows), maxCols(cols) {
    rowHeads = new Cell*[rows]();
    colHeads = new Cell*[cols]();
}

SpreadSheet::~SpreadSheet() {
    for (int i = 0; i < maxRows; i++) {
        Cell* cur = rowHeads[i];
        while (cur) {
            Cell* tmp = cur;
            cur = cur->nextInRow;
            delete tmp;
        }
    }
    delete[] rowHeads;
    delete[] colHeads;
}

void SpreadSheet::insertCell(int row, int col, const std::string& value) {
    Cell* cur = rowHeads[row];
    while (cur) {
        if (cur->col == col) {
            cur->value = value;
            return;
        }
        cur = cur->nextInRow;
    }

    Cell* node = new Cell(row, col, value);

    if (!rowHeads[row] || rowHeads[row]->col > col) {
        node->nextInRow = rowHeads[row];
        rowHeads[row] = node;
    } else {
        Cell* prev = rowHeads[row];
        while (prev->nextInRow && prev->nextInRow->col < col)
            prev = prev->nextInRow;
        node->nextInRow = prev->nextInRow;
        prev->nextInRow = node;
    }

    if (!colHeads[col] || colHeads[col]->row > row) {
        node->nextInCol = colHeads[col];
        colHeads[col] = node;
    } else {
        Cell* prev = colHeads[col];
        while (prev->nextInCol && prev->nextInCol->row < row)
            prev = prev->nextInCol;
        node->nextInCol = prev->nextInCol;
        prev->nextInCol = node;
    }
}

std::string SpreadSheet::getCell(int row, int col) {
    Cell* cur = rowHeads[row];
    while (cur) {
        if (cur->col == col) return cur->value;
        cur = cur->nextInRow;
    }
    return "";
}

void SpreadSheet::deleteCell(int row, int col) {
    Cell* target = nullptr;
    if (!rowHeads[row]) return;

    if (rowHeads[row]->col == col) {
        target = rowHeads[row];
        rowHeads[row] = target->nextInRow;
    } else {
        Cell* prev = rowHeads[row];
        while (prev->nextInRow && prev->nextInRow->col != col)
            prev = prev->nextInRow;
        if (!prev->nextInRow) return;
        target = prev->nextInRow;
        prev->nextInRow = target->nextInRow;
    }

    if (colHeads[col] == target) {
        colHeads[col] = target->nextInCol;
    } else {
        Cell* prev = colHeads[col];
        while (prev->nextInCol && prev->nextInCol != target)
            prev = prev->nextInCol;
        if (prev->nextInCol)
            prev->nextInCol = target->nextInCol;
    }

    delete target;
}

void SpreadSheet::deleteRow(int row) {
    Cell* cur = rowHeads[row];
    while (cur) {
        Cell* next = cur->nextInRow;
        deleteCell(row, cur->col);
        cur = next;
    }
}

void SpreadSheet::deleteCol(int col) {
    Cell* cur = colHeads[col];
    while (cur) {
        Cell* next = cur->nextInCol;
        deleteCell(cur->row, col);
        cur = next;
    }
}

double SpreadSheet::sumRow(int row) {
    double total = 0;
    Cell* cur = rowHeads[row];
    while (cur) {
        try { total += std::stod(cur->value); } catch (...) {}
        cur = cur->nextInRow;
    }
    return total;
}

double SpreadSheet::sumCol(int col) {
    double total = 0;
    Cell* cur = colHeads[col];
    while (cur) {
        try { total += std::stod(cur->value); } catch (...) {}
        cur = cur->nextInCol;
    }
    return total;
}

double SpreadSheet::sumRange(int r1, int c1, int r2, int c2) {
    double total = 0;
    for (int r = r1; r <= r2; r++) {
        Cell* cur = rowHeads[r];
        while (cur) {
            if (cur->col >= c1 && cur->col <= c2)
                try { total += std::stod(cur->value); } catch (...) {}
            cur = cur->nextInRow;
        }
    }
    return total;
}

double SpreadSheet::avgRange(int r1, int c1, int r2, int c2) {
    double total = 0;
    int count = 0;
    for (int r = r1; r <= r2; r++) {
        Cell* cur = rowHeads[r];
        while (cur) {
            if (cur->col >= c1 && cur->col <= c2) {
                try { total += std::stod(cur->value); count++; } catch (...) {}
            }
            cur = cur->nextInRow;
        }
    }
    return count > 0 ? total / count : 0;
}

double SpreadSheet::maxRange(int r1, int c1, int r2, int c2) {
    double maxVal = -1e18;
    bool found = false;
    for (int r = r1; r <= r2; r++) {
        Cell* cur = rowHeads[r];
        while (cur) {
            if (cur->col >= c1 && cur->col <= c2) {
                try {
                    double v = std::stod(cur->value);
                    if (v > maxVal) { maxVal = v; found = true; }
                } catch (...) {}
            }
            cur = cur->nextInRow;
        }
    }
    return found ? maxVal : 0;
}

double SpreadSheet::minRange(int r1, int c1, int r2, int c2) {
    double minVal = 1e18;
    bool found = false;
    for (int r = r1; r <= r2; r++) {
        Cell* cur = rowHeads[r];
        while (cur) {
            if (cur->col >= c1 && cur->col <= c2) {
                try {
                    double v = std::stod(cur->value);
                    if (v < minVal) { minVal = v; found = true; }
                } catch (...) {}
            }
            cur = cur->nextInRow;
        }
    }
    return found ? minVal : 0;
}

void SpreadSheet::printSheet() {
    std::cout << "\n=== SpreadSheet ===\n";
    for (int r = 0; r < maxRows; r++) {
        Cell* cur = rowHeads[r];
        while (cur) {
            std::cout << "[" << r << "," << cur->col << "] = " << cur->value << "\n";
            cur = cur->nextInRow;
        }
    }
    std::cout << "===================\n";
}
