#include "mainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QString>
#include <sstream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), sheet(new SpreadSheet(100, 26)) {
    setupUI();
}

MainWindow::~MainWindow() {
    delete sheet;
}

void MainWindow::setupUI() {
    setWindowTitle("SpreadSheet — AyED");
    resize(900, 600);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // barra de formula arriba
    QHBoxLayout* barLayout = new QHBoxLayout();
    QLabel* fxLabel = new QLabel("fx:", this);
    formulaBar = new QLineEdit(this);
    formulaBar->setPlaceholderText("Ingresa un valor o formula (ej: =A1+B1)");
    barLayout->addWidget(fxLabel);
    barLayout->addWidget(formulaBar);
    mainLayout->addLayout(barLayout);

    // tabla principal
    table = new QTableWidget(50, 26, this);

    // headers de columna A-Z
    QStringList colHeaders;
    for (int i = 0; i < 26; i++)
        colHeaders << QString(QChar('A' + i));
    table->setHorizontalHeaderLabels(colHeaders);
    table->horizontalHeader()->setDefaultSectionSize(80);
    table->verticalHeader()->setDefaultSectionSize(24);

    mainLayout->addWidget(table);

    // botones abajo
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnDelRow = new QPushButton("Eliminar Fila", this);
    QPushButton* btnDelCol = new QPushButton("Eliminar Columna", this);
    QPushButton* btnSum    = new QPushButton("Suma Rango", this);
    QPushButton* btnAvg = new QPushButton("Promedio Rango", this);
    QPushButton* btnMax = new QPushButton("Máximo Rango",   this);
    QPushButton* btnMin = new QPushButton("Mínimo Rango",   this);
    btnLayout->addWidget(btnDelRow);
    btnLayout->addWidget(btnDelCol);
    btnLayout->addWidget(btnSum);
    mainLayout->addLayout(btnLayout);
    btnLayout->addWidget(btnAvg);
    btnLayout->addWidget(btnMax);
    btnLayout->addWidget(btnMin);

    // status bar
    statusLabel = new QLabel("Listo", this);
    mainLayout->addWidget(statusLabel);

    // conexiones
    connect(table, &QTableWidget::cellChanged, this, &MainWindow::onCellChanged);
    connect(btnDelRow, &QPushButton::clicked, this, &MainWindow::onDeleteRow);
    connect(btnDelCol, &QPushButton::clicked, this, &MainWindow::onDeleteCol);
    connect(btnSum,    &QPushButton::clicked, this, &MainWindow::onSumRange);
    connect(btnAvg, &QPushButton::clicked, this, &MainWindow::onAvgRange);
    connect(btnMax, &QPushButton::clicked, this, &MainWindow::onMaxRange);
    connect(btnMin, &QPushButton::clicked, this, &MainWindow::onMinRange);
}

void MainWindow::onCellChanged(int row, int col) {
    QTableWidgetItem* item = table->item(row, col);
    if (!item) return;

    std::string val = item->text().toStdString();

    if (val.empty()) {
        sheet->deleteCell(row, col);
        statusLabel->setText(QString("Celda [%1,%2] eliminada").arg(row).arg(col));
        return;
    }

    // si es formula la evalua
    if (val[0] == '=') {
        val = evalFormula(val);
        // bloquea señal para no entrar en loop
        table->blockSignals(true);
        item->setText(QString::fromStdString(val));
        table->blockSignals(false);
    }

    sheet->insertCell(row, col, val);
    statusLabel->setText(QString("Celda [%1,%2] = %3")
        .arg(row).arg(col).arg(QString::fromStdString(val)));
}

void MainWindow::onDeleteRow() {
    int row = table->currentRow();
    if (row < 0) return;

    sheet->deleteRow(row);

    // limpia la fila en la tabla visual
    table->blockSignals(true);
    for (int c = 0; c < 26; c++) {
        QTableWidgetItem* item = table->item(row, c);
        if (item) item->setText("");
    }
    table->blockSignals(false);

    statusLabel->setText(QString("Fila %1 eliminada").arg(row));
}

void MainWindow::onDeleteCol() {
    int col = table->currentColumn();
    if (col < 0) return;

    sheet->deleteCol(col);

    table->blockSignals(true);
    for (int r = 0; r < 50; r++) {
        QTableWidgetItem* item = table->item(r, col);
        if (item) item->setText("");
    }
    table->blockSignals(false);

    statusLabel->setText(QString("Columna %1 eliminada").arg(col));
}

std::string MainWindow::evalFormula(const std::string& formula) {
    // parser simple: =A1+B1, =A1*2, etc.
    // por ahora devuelve la formula sin el = para mostrar algo
    // esto lo expandimos despues con el parser completo
    std::string expr = formula.substr(1);

    // busca operador
    char op = 0;
    int opIdx = -1;
    for (int i = 1; i < (int)expr.size(); i++) {
        if (expr[i] == '+' || expr[i] == '-' ||
            expr[i] == '*' || expr[i] == '/') {
            op = expr[i];
            opIdx = i;
            break;
        }
    }

    if (opIdx == -1) return expr;

    std::string left  = expr.substr(0, opIdx);
    std::string right = expr.substr(opIdx + 1);

    auto resolveOperand = [&](const std::string& s) -> double {
        if (s.empty()) return 0;
        if (std::isupper(s[0])) {
            int col = s[0] - 'A';
            int row = std::stoi(s.substr(1)) - 1;
            std::string val = sheet->getCell(row, col);
            if (val.empty()) return 0;
            try { return std::stod(val); } catch (...) { return 0; }
        }
        try { return std::stod(s); } catch (...) { return 0; }
    };

    double l = resolveOperand(left);
    double r = resolveOperand(right);
    double result = 0;

    if (op == '+') result = l + r;
    if (op == '-') result = l - r;
    if (op == '*') result = l * r;
    if (op == '/' && r != 0) result = l / r;

    std::ostringstream oss;
    oss << result;
    return oss.str();
}

// helper interno para no repetir el parsing del rango
static bool parseRange(const QString& input, int& r1, int& c1, int& r2, int& c2) {
    QStringList parts = input.toUpper().split(":");
    if (parts.size() != 2) return false;

    auto parseRef = [](const QString& ref, int& row, int& col) {
        if (ref.isEmpty()) return false;
        col = ref[0].toLatin1() - 'A';
        row = ref.mid(1).toInt() - 1;
        return col >= 0 && col < 26 && row >= 0;
    };

    return parseRef(parts[0], r1, c1) && parseRef(parts[1], r2, c2);
}

void MainWindow::onAvgRange() {
    bool ok;
    QString input = QInputDialog::getText(this, "Promedio de Rango",
        "Ingresa rango (ej: A1:C3):", QLineEdit::Normal, "", &ok);
    if (!ok || input.isEmpty()) return;

    int r1, c1, r2, c2;
    if (!parseRange(input, r1, c1, r2, c2)) {
        statusLabel->setText("Rango invalido");
        return;
    }

    double result = sheet->avgRange(r1, c1, r2, c2);
    QMessageBox::information(this, "Resultado",
        QString("PROMEDIO(%1) = %2").arg(input).arg(result));
    statusLabel->setText(QString("PROMEDIO(%1) = %2").arg(input).arg(result));
}

void MainWindow::onMaxRange() {
    bool ok;
    QString input = QInputDialog::getText(this, "Máximo de Rango",
        "Ingresa rango (ej: A1:C3):", QLineEdit::Normal, "", &ok);
    if (!ok || input.isEmpty()) return;

    int r1, c1, r2, c2;
    if (!parseRange(input, r1, c1, r2, c2)) {
        statusLabel->setText("Rango invalido");
        return;
    }

    double result = sheet->maxRange(r1, c1, r2, c2);
    QMessageBox::information(this, "Resultado",
        QString("MÁXIMO(%1) = %2").arg(input).arg(result));
    statusLabel->setText(QString("MÁXIMO(%1) = %2").arg(input).arg(result));
}

void MainWindow::onMinRange() {
    bool ok;
    QString input = QInputDialog::getText(this, "Mínimo de Rango",
        "Ingresa rango (ej: A1:C3):", QLineEdit::Normal, "", &ok);
    if (!ok || input.isEmpty()) return;

    int r1, c1, r2, c2;
    if (!parseRange(input, r1, c1, r2, c2)) {
        statusLabel->setText("Rango invalido");
        return;
    }

    double result = sheet->minRange(r1, c1, r2, c2);
    QMessageBox::information(this, "Resultado",
        QString("MÍNIMO(%1) = %2").arg(input).arg(result));
    statusLabel->setText(QString("MÍNIMO(%1) = %2").arg(input).arg(result));
}

void MainWindow::onSumRange() {
    bool ok;
    QString input = QInputDialog::getText(this, "Suma de Rango",
        "Ingresa rango (ej: A1:C3):", QLineEdit::Normal, "", &ok);
    if (!ok || input.isEmpty()) return;

    int r1, c1, r2, c2;
    if (!parseRange(input, r1, c1, r2, c2)) {
        statusLabel->setText("Rango invalido");
        return;
    }

    double result = sheet->sumRange(r1, c1, r2, c2);
    QMessageBox::information(this, "Resultado",
        QString("SUMA(%1) = %2").arg(input).arg(result));
    statusLabel->setText(QString("SUMA(%1) = %2").arg(input).arg(result));
}
