#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "spreadSheet.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onCellChanged(int row, int col);
    void onDeleteRow();
    void onDeleteCol();
    void onSumRange();

private:
    SpreadSheet* sheet;
    QTableWidget* table;
    QLineEdit* formulaBar;
    QLabel* statusLabel;

    void setupUI();
    void syncToTable();
    std::string evalFormula(const std::string& formula);
};
