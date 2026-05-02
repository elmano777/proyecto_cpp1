#pragma once
#include "spreadSheet.h"
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void onCellChanged(int row, int col);
  void onDeleteRow();
  void onDeleteCol();
  void onSumRange();
  void onAvgRange();
  void onMaxRange();
  void onMinRange();

private:
  SpreadSheet *sheet;
  QTableWidget *table;
  QLineEdit *formulaBar;
  QLabel *statusLabel;

  void setupUI();
  void syncToTable();
  std::string evalFormula(const std::string &formula);
};
