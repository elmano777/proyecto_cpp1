/*
 * Define la clase MainWindow, que representa la interfaz gráfica
 * principal de la hoja de cálculo.
 *
 * Responsabilidades:
 * - Gestionar los elementos visuales (tabla, botones, barra de fórmula)
 * - Manejar eventos del usuario (clicks, edición de celdas, etc.)
 * - Conectar la interfaz con la lógica (SpreadSheet)
 * - Interpretar entradas del usuario (referencias de celdas, rangos, fórmulas)
 */

#pragma once

#include "../core/spreadSheet.h"

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QTableWidget>
#include <string>

using namespace std;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

private:
  void onCellChanged(int row, int col);
  void onCurrentCellChanged(int currentRow, int currentCol, int previousRow,
                            int previousCol);
  void onApplyFormulaBar();
  void onConsultCell();
  void onInsertCell();
  void onModifyCell();
  void onDeleteCell();
  void onDeleteRow();
  void onDeleteCol();
  void onDeleteRange();
  void onSumRow();
  void onSumCol();
  void onSumRange();
  void onAvgRange();
  void onMaxRange();
  void onMinRange();
  void onAvgRow();
  void onAvgCol();

private:
  static constexpr int TABLE_ROWS = 50;
  static constexpr int TABLE_COLS = 26;

  SpreadSheet *sheet;
  QTableWidget *table;
  QLineEdit *formulaBar;
  QLabel *statusLabel;

  void setupUI();
  void syncToTable();
  void refreshCell(int row, int col);
  void showMessage(const QString &title, const QString &text);

  QString cellName(int row, int col) const;
  bool parseCellRef(const QString &text, int &row, int &col) const;
  bool parseRange(const QString &text, int &r1, int &c1, int &r2,
                  int &c2) const;
  QString askRange(const QString &title) const;

  bool evalFormula(const string &formula, string &result) const;
  bool resolveOperand(const string &text, double &value) const;
  static string trim(const string &text);
};
