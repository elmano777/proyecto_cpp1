#pragma once

#include "spreadSheet.h"

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QTableWidget>
#include <string>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    void onCellChanged(int row, int col);
    void onCurrentCellChanged(int currentRow, int currentCol, int previousRow, int previousCol);
    void onApplyFormulaBar();
    void onConsultCell();
    void onDeleteCell();
    void onDeleteRow();
    void onDeleteCol();
    void onDeleteRange();
    void onSumRow();
    void onSumCol();
    void onSumRange();
    void onAverageRange();
    void onMaxRange();
    void onMinRange();

private:
    static constexpr int TABLE_ROWS = 50;
    static constexpr int TABLE_COLS = 26;

    SpreadSheet* sheet;
    QTableWidget* table;
    QLineEdit* formulaBar;
    QLabel* statusLabel;

    void setupUI();
    void syncToTable();
    void refreshCell(int row, int col);
    void showMessage(const QString& title, const QString& text);

    QString cellName(int row, int col) const;
    bool parseCellRef(const QString& text, int& row, int& col) const;
    bool parseRange(const QString& text, int& r1, int& c1, int& r2, int& c2) const;
    QString askRange(const QString& title) const;

    bool evalFormula(const std::string& formula, std::string& result) const;
    bool resolveOperand(const std::string& text, double& value) const;
    static std::string trim(const std::string& text);
};
