#include "mainWindow.h"

#include <QBrush>
#include <QColor>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalBlocker>
#include <QStringList>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>
#include <cctype>
#include <sstream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), sheet(new SpreadSheet(TABLE_ROWS, TABLE_COLS)),
      table(nullptr), formulaBar(nullptr), statusLabel(nullptr) {
  setupUI();
}

MainWindow::~MainWindow() { delete sheet; }

void MainWindow::setupUI() {
  setWindowTitle("Hoja de Cálculo Simple - Matriz Dispersa");
  resize(1100, 650);

  QWidget *central = new QWidget(this);
  setCentralWidget(central);

  QVBoxLayout *mainLayout = new QVBoxLayout(central);

  QHBoxLayout *formulaLayout = new QHBoxLayout();
  QLabel *fxLabel = new QLabel("fx:", this);
  formulaBar = new QLineEdit(this);
  formulaBar->setPlaceholderText(
      "Selecciona una celda y escribe valor o fórmula. Ej: =A1+B1, =A1*2");
  QPushButton *applyFormulaButton = new QPushButton("Aplicar", this);
  formulaLayout->addWidget(fxLabel);
  formulaLayout->addWidget(formulaBar);
  formulaLayout->addWidget(applyFormulaButton);
  mainLayout->addLayout(formulaLayout);

  table = new QTableWidget(TABLE_ROWS, TABLE_COLS, this);

  QStringList colHeaders;
  for (int c = 0; c < TABLE_COLS; ++c) {
    colHeaders << QString(QChar('A' + c));
  }
  table->setHorizontalHeaderLabels(colHeaders);
  table->horizontalHeader()->setDefaultSectionSize(85);
  table->verticalHeader()->setDefaultSectionSize(25);
  table->setAlternatingRowColors(true);
  mainLayout->addWidget(table);

  QGridLayout *buttonLayout = new QGridLayout();

  QPushButton *consultButton = new QPushButton("Consultar celda", this);
  QPushButton *insertCellButton = new QPushButton("Insertar celda", this);
  QPushButton *modifyCellButton = new QPushButton("Modificar celda", this);
  QPushButton *deleteCellButton = new QPushButton("Eliminar celda", this);
  QPushButton *deleteRowButton = new QPushButton("Eliminar fila", this);
  QPushButton *deleteColButton = new QPushButton("Eliminar columna", this);
  QPushButton *deleteRangeButton = new QPushButton("Eliminar rango", this);

  QPushButton *sumRowButton = new QPushButton("SUMA fila", this);
  QPushButton *sumColButton = new QPushButton("SUMA columna", this);
  QPushButton *sumRangeButton = new QPushButton("SUMA rango", this);
  QPushButton *avgRangeButton = new QPushButton("PROMEDIO rango", this);
  QPushButton *maxRangeButton = new QPushButton("MÁX rango", this);
  QPushButton *minRangeButton = new QPushButton("MÍN rango", this);

  buttonLayout->addWidget(consultButton, 0, 0);
  buttonLayout->addWidget(insertCellButton, 0, 1);
  buttonLayout->addWidget(modifyCellButton, 0, 2);
  buttonLayout->addWidget(deleteCellButton, 0, 3);
  buttonLayout->addWidget(deleteRowButton, 2, 0);
  buttonLayout->addWidget(deleteColButton, 2, 1);
  buttonLayout->addWidget(deleteRangeButton, 2, 2);

  buttonLayout->addWidget(sumRowButton, 1, 0);
  buttonLayout->addWidget(sumColButton, 1, 1);
  buttonLayout->addWidget(sumRangeButton, 1, 2);
  buttonLayout->addWidget(avgRangeButton, 1, 3);
  buttonLayout->addWidget(maxRangeButton, 1, 4);
  buttonLayout->addWidget(minRangeButton, 1, 5);

  mainLayout->addLayout(buttonLayout);

  statusLabel = new QLabel(
      "Listo. Las celdas ocupadas se resaltan en verde claro.", this);
  mainLayout->addWidget(statusLabel);

  connect(table, &QTableWidget::cellChanged, this, &MainWindow::onCellChanged);
  connect(table, &QTableWidget::currentCellChanged, this,
          &MainWindow::onCurrentCellChanged);
  connect(applyFormulaButton, &QPushButton::clicked, this,
          &MainWindow::onApplyFormulaBar);
  connect(formulaBar, &QLineEdit::returnPressed, this,
          &MainWindow::onApplyFormulaBar);

  connect(consultButton, &QPushButton::clicked, this,
          &MainWindow::onConsultCell);
  connect(insertCellButton, &QPushButton::clicked, this,
          &MainWindow::onInsertCell);
  connect(modifyCellButton, &QPushButton::clicked, this,
          &MainWindow::onModifyCell);
  connect(deleteCellButton, &QPushButton::clicked, this,
          &MainWindow::onDeleteCell);
  connect(deleteRowButton, &QPushButton::clicked, this,
          &MainWindow::onDeleteRow);
  connect(deleteColButton, &QPushButton::clicked, this,
          &MainWindow::onDeleteCol);
  connect(deleteRangeButton, &QPushButton::clicked, this,
          &MainWindow::onDeleteRange);
  connect(sumRowButton, &QPushButton::clicked, this, &MainWindow::onSumRow);
  connect(sumColButton, &QPushButton::clicked, this, &MainWindow::onSumCol);
  connect(sumRangeButton, &QPushButton::clicked, this, &MainWindow::onSumRange);
  connect(avgRangeButton, &QPushButton::clicked, this, &MainWindow::onAvgRange);
  connect(maxRangeButton, &QPushButton::clicked, this, &MainWindow::onMaxRange);
  connect(minRangeButton, &QPushButton::clicked, this, &MainWindow::onMinRange);
}

void MainWindow::onCellChanged(int row, int col) {
  QTableWidgetItem *item = table->item(row, col);
  if (item == nullptr) {
    return;
  }

  string value = item->text().toStdString();
  value = trim(value);

  if (value.empty()) {
    sheet->deleteCell(row, col);
    refreshCell(row, col);
    statusLabel->setText(QString("%1 eliminada").arg(cellName(row, col)));
    return;
  }

  if (value[0] == '=') {
    string evaluated;
    if (!evalFormula(value, evaluated)) {
      evaluated = "ERR";
    }
    value = evaluated;
  }

  if (sheet->cellExists(row, col)) {
    sheet->modifyCell(row, col, value);
    statusLabel->setText(
        QString("%1 modificada = %2")
            .arg(cellName(row, col), QString::fromStdString(value)));
  } else {
    sheet->insertCell(row, col, value);
    statusLabel->setText(
        QString("%1 insertada = %2")
            .arg(cellName(row, col), QString::fromStdString(value)));
  }

  refreshCell(row, col);
}

void MainWindow::onCurrentCellChanged(int currentRow, int currentCol, int,
                                      int) {
  if (currentRow < 0 || currentCol < 0) {
    return;
  }

  QString value =
      QString::fromStdString(sheet->getCell(currentRow, currentCol));
  formulaBar->setText(value);
  statusLabel->setText(QString("Seleccionada %1. Valor: %2")
                           .arg(cellName(currentRow, currentCol),
                                value.isEmpty() ? "<vacía>" : value));
}

void MainWindow::onApplyFormulaBar() {
  int row = table->currentRow();
  int col = table->currentColumn();
  if (row < 0 || col < 0) {
    showMessage("Sin celda seleccionada",
                "Selecciona una celda antes de aplicar un valor o fórmula.");
    return;
  }

  QSignalBlocker blocker(table);
  QTableWidgetItem *item = table->item(row, col);
  if (item == nullptr) {
    item = new QTableWidgetItem();
    table->setItem(row, col, item);
  }
  item->setText(formulaBar->text());

  blocker.unblock();
  onCellChanged(row, col);
}

void MainWindow::onConsultCell() {
  bool ok = false;
  QString input = QInputDialog::getText(
      this, "Consultar celda", "Referencia (ej: B3):", QLineEdit::Normal, "A1",
      &ok);
  if (!ok)
    return;

  int row = 0;
  int col = 0;
  if (!parseCellRef(input, row, col)) {
    showMessage("Referencia inválida", "Usa una referencia como A1, B3 o C10.");
    return;
  }

  QString value = QString::fromStdString(sheet->getCell(row, col));
  if (value.isEmpty()) {
    value = "<vacía>";
  }
  showMessage("Consulta", QString("%1 = %2").arg(cellName(row, col), value));
}

void MainWindow::onInsertCell() {
  bool ok = false;
  QString cellRef = QInputDialog::getText(
      this, "Insertar celda", "Referencia (ej: B3):", QLineEdit::Normal, "A1",
      &ok);
  if (!ok || cellRef.isEmpty())
    return;

  int row = 0;
  int col = 0;
  if (!parseCellRef(cellRef, row, col)) {
    showMessage("Referencia inválida",
                "Usa una referencia como A1, B3 o Z100.");
    return;
  }

  QString value = QInputDialog::getText(
      this, "Insertar celda", QString("Valor para %1:").arg(cellName(row, col)),
      QLineEdit::Normal, "", &ok);
  if (!ok)
    return;

  sheet->insertCell(row, col, value.toStdString());
  refreshCell(row, col);
  statusLabel->setText(QString("%1 = %2").arg(cellName(row, col), value));
}

void MainWindow::onModifyCell() {
  bool ok = false;
  QString cellRef = QInputDialog::getText(
      this, "Modificar celda", "Referencia (ej: B3):", QLineEdit::Normal, "A1",
      &ok);
  if (!ok || cellRef.isEmpty())
    return;

  int row = 0;
  int col = 0;
  if (!parseCellRef(cellRef, row, col)) {
    showMessage("Referencia inválida",
                "Usa una referencia como A1, B3 o Z100.");
    return;
  }

  if (!sheet->cellExists(row, col)) {
    showMessage("Celda vacía",
                QString("%1 no tiene valor. Usa 'Insertar celda'.")
                    .arg(cellName(row, col)));
    return;
  }

  QString currentValue = QString::fromStdString(sheet->getCell(row, col));
  QString newValue = QInputDialog::getText(
      this, "Modificar celda",
      QString("Nuevo valor para %1:").arg(cellName(row, col)),
      QLineEdit::Normal, currentValue, &ok);
  if (!ok)
    return;

  sheet->modifyCell(row, col, newValue.toStdString());
  refreshCell(row, col);
  statusLabel->setText(QString("%1 = %2").arg(cellName(row, col), newValue));
}

void MainWindow::onDeleteCell() {
  int row = table->currentRow();
  int col = table->currentColumn();
  if (row < 0 || col < 0) {
    showMessage("Sin celda seleccionada",
                "Selecciona una celda para eliminarla.");
    return;
  }

  bool deleted = sheet->deleteCell(row, col);
  refreshCell(row, col);
  statusLabel->setText(
      deleted ? QString("%1 eliminada").arg(cellName(row, col))
              : QString("%1 ya estaba vacía").arg(cellName(row, col)));
}

void MainWindow::onDeleteRow() {
  int row = table->currentRow();
  if (row < 0) {
    showMessage("Sin fila seleccionada",
                "Selecciona cualquier celda de la fila que quieres eliminar.");
    return;
  }

  bool deleted = sheet->deleteRow(row);
  syncToTable();
  statusLabel->setText(
      deleted ? QString("Fila %1 eliminada").arg(row + 1)
              : QString("La fila %1 ya estaba vacía").arg(row + 1));
}

void MainWindow::onDeleteCol() {
  int col = table->currentColumn();
  if (col < 0) {
    showMessage(
        "Sin columna seleccionada",
        "Selecciona cualquier celda de la columna que quieres eliminar.");
    return;
  }

  bool deleted = sheet->deleteCol(col);
  syncToTable();
  statusLabel->setText(
      deleted ? QString("Columna %1 eliminada").arg(QChar('A' + col))
              : QString("La columna %1 ya estaba vacía").arg(QChar('A' + col)));
}

void MainWindow::onDeleteRange() {
  QString input = askRange("Eliminar rango");
  if (input.isEmpty())
    return;

  int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
  if (!parseRange(input, r1, c1, r2, c2)) {
    showMessage("Rango inválido", "Usa un rango como A1:C4.");
    return;
  }

  bool deleted = sheet->deleteRange(r1, c1, r2, c2);
  syncToTable();
  statusLabel->setText(
      deleted
          ? QString("Rango %1 eliminado").arg(input.toUpper())
          : QString("Rango %1 no tenía celdas ocupadas").arg(input.toUpper()));
}

void MainWindow::onSumRow() {
  int row = table->currentRow();
  if (row < 0) {
    showMessage("Sin fila seleccionada",
                "Selecciona una fila para calcular la suma.");
    return;
  }

  double result = sheet->sumRow(row);
  showMessage("SUMA fila",
              QString("SUMA(fila %1) = %2").arg(row + 1).arg(result));
}

void MainWindow::onSumCol() {
  int col = table->currentColumn();
  if (col < 0) {
    showMessage("Sin columna seleccionada",
                "Selecciona una columna para calcular la suma.");
    return;
  }

  double result = sheet->sumCol(col);
  showMessage(
      "SUMA columna",
      QString("SUMA(columna %1) = %2").arg(QChar('A' + col)).arg(result));
}

void MainWindow::onSumRange() {
  QString input = askRange("SUMA rango");
  if (input.isEmpty())
    return;

  int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
  if (!parseRange(input, r1, c1, r2, c2)) {
    showMessage("Rango inválido", "Usa un rango como A1:C4.");
    return;
  }

  double result = sheet->sumRange(r1, c1, r2, c2);
  showMessage("SUMA rango",
              QString("SUMA(%1) = %2").arg(input.toUpper()).arg(result));
}

void MainWindow::onAvgRange() {
  QString input = askRange("PROMEDIO rango");
  if (input.isEmpty())
    return;

  int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
  if (!parseRange(input, r1, c1, r2, c2)) {
    showMessage("Rango inválido", "Usa un rango como A1:C4.");
    return;
  }

  auto result = sheet->averageRange(r1, c1, r2, c2);
  if (!result.has_value()) {
    showMessage(
        "PROMEDIO rango",
        QString("PROMEDIO(%1): no hay celdas numéricas.").arg(input.toUpper()));
    return;
  }
  showMessage(
      "PROMEDIO rango",
      QString("PROMEDIO(%1) = %2").arg(input.toUpper()).arg(result.value()));
}

void MainWindow::onMaxRange() {
  QString input = askRange("MÁX rango");
  if (input.isEmpty())
    return;

  int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
  if (!parseRange(input, r1, c1, r2, c2)) {
    showMessage("Rango inválido", "Usa un rango como A1:C4.");
    return;
  }

  auto result = sheet->maxRange(r1, c1, r2, c2);
  if (!result.has_value()) {
    showMessage(
        "MÁX rango",
        QString("MÁX(%1): no hay celdas numéricas.").arg(input.toUpper()));
    return;
  }
  showMessage("MÁX rango",
              QString("MÁX(%1) = %2").arg(input.toUpper()).arg(result.value()));
}

void MainWindow::onMinRange() {
  QString input = askRange("MÍN rango");
  if (input.isEmpty())
    return;

  int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
  if (!parseRange(input, r1, c1, r2, c2)) {
    showMessage("Rango inválido", "Usa un rango como A1:C4.");
    return;
  }

  auto result = sheet->minRange(r1, c1, r2, c2);
  if (!result.has_value()) {
    showMessage(
        "MÍN rango",
        QString("MÍN(%1): no hay celdas numéricas.").arg(input.toUpper()));
    return;
  }
  showMessage("MÍN rango",
              QString("MÍN(%1) = %2").arg(input.toUpper()).arg(result.value()));
}

void MainWindow::syncToTable() {
  QSignalBlocker blocker(table);
  table->clearContents();

  for (const CellInfo &cell : sheet->getOccupiedCells()) {
    QTableWidgetItem *item =
        new QTableWidgetItem(QString::fromStdString(cell.value));
    table->setItem(cell.row, cell.col, item);
  }
}

void MainWindow::refreshCell(int row, int col) {
  if (row < 0 || row >= TABLE_ROWS || col < 0 || col >= TABLE_COLS) {
    return;
  }

  QSignalBlocker blocker(table);
  QString value = QString::fromStdString(sheet->getCell(row, col));
  QTableWidgetItem *item = table->item(row, col);

  if (value.isEmpty()) {
    if (item != nullptr) {
      item->setText("");
      item->setBackground(QBrush());
    }
    return;
  }

  if (item == nullptr) {
    item = new QTableWidgetItem();
    table->setItem(row, col, item);
  }
  item->setText(value);
}

void MainWindow::showMessage(const QString &title, const QString &text) {
  QMessageBox::information(this, title, text);
  statusLabel->setText(text);
}

QString MainWindow::cellName(int row, int col) const {
  return QString("%1%2").arg(QChar('A' + col)).arg(row + 1);
}

bool MainWindow::parseCellRef(const QString &text, int &row, int &col) const {
  QString ref = text.trimmed().toUpper();
  if (ref.isEmpty()) {
    return false;
  }

  int i = 0;
  int colNumber = 0;
  while (i < ref.size() && ref[i].isLetter()) {
    colNumber = colNumber * 26 + (ref[i].unicode() - 'A' + 1);
    ++i;
  }

  if (i == 0 || i >= ref.size()) {
    return false;
  }

  bool ok = false;
  int rowNumber = ref.mid(i).toInt(&ok);
  if (!ok) {
    return false;
  }

  row = rowNumber - 1;
  col = colNumber - 1;
  return row >= 0 && row < TABLE_ROWS && col >= 0 && col < TABLE_COLS;
}

bool MainWindow::parseRange(const QString &text, int &r1, int &c1, int &r2,
                            int &c2) const {
  QString cleaned = text.trimmed().toUpper();
  QStringList parts = cleaned.split(':');
  if (parts.size() == 1) {
    if (!parseCellRef(parts[0], r1, c1)) {
      return false;
    }
    r2 = r1;
    c2 = c1;
    return true;
  }

  if (parts.size() != 2) {
    return false;
  }

  if (!parseCellRef(parts[0], r1, c1) || !parseCellRef(parts[1], r2, c2)) {
    return false;
  }

  if (r1 > r2)
    std::swap(r1, r2);
  if (c1 > c2)
    std::swap(c1, c2);
  return true;
}

QString MainWindow::askRange(const QString &title) const {
  bool ok = false;
  QString input = QInputDialog::getText(const_cast<MainWindow *>(this), title,
                                        "Rango (ej: A1:C4):", QLineEdit::Normal,
                                        "A1:C4", &ok);
  if (!ok) {
    return "";
  }
  return input.trimmed();
}

bool MainWindow::evalFormula(const string &formula, string &result) const {
  string expr = trim(formula);
  if (expr.empty() || expr[0] != '=') {
    result = expr;
    return true;
  }

  expr = trim(expr.substr(1));
  if (expr.empty()) {
    return false;
  }

  int opIndex = -1;
  char op = '\0';
  for (int i = 1; i < static_cast<int>(expr.size()); ++i) {
    char ch = expr[i];
    if ((ch == '+' || ch == '-' || ch == '*' || ch == '/') &&
        expr[i - 1] != 'e' && expr[i - 1] != 'E') {
      opIndex = i;
      op = ch;
      break;
    }
  }

  if (opIndex == -1) {
    double value = 0.0;
    if (!resolveOperand(expr, value)) {
      return false;
    }
    ostringstream oss;
    oss << value;
    result = oss.str();
    return true;
  }

  string left = trim(expr.substr(0, opIndex));
  string right = trim(expr.substr(opIndex + 1));

  double a = 0.0;
  double b = 0.0;
  if (!resolveOperand(left, a) || !resolveOperand(right, b)) {
    return false;
  }

  double answer = 0.0;
  switch (op) {
  case '+':
    answer = a + b;
    break;
  case '-':
    answer = a - b;
    break;
  case '*':
    answer = a * b;
    break;
  case '/':
    if (b == 0.0) {
      result = "ERR_DIV0";
      return true;
    }
    answer = a / b;
    break;
  default:
    return false;
  }

  ostringstream oss;
  oss << answer;
  result = oss.str();
  return true;
}

bool MainWindow::resolveOperand(const string &text, double &value) const {
  string cleaned = trim(text);
  if (cleaned.empty()) {
    return false;
  }

  QString q = QString::fromStdString(cleaned).toUpper();
  int row = 0;
  int col = 0;
  if (parseCellRef(q, row, col)) {
    string cellValue = sheet->getCell(row, col);
    if (cellValue.empty()) {
      value = 0.0;
      return true;
    }
    try {
      size_t used = 0;
      value = std::stod(trim(cellValue), &used);
      return used == trim(cellValue).size();
    } catch (...) {
      return false;
    }
  }

  try {
    size_t used = 0;
    value = std::stod(cleaned, &used);
    return used == cleaned.size();
  } catch (...) {
    return false;
  }
}

string MainWindow::trim(const string &text) {
  int left = 0;
  int right = static_cast<int>(text.size()) - 1;

  while (left <= right &&
         std::isspace(static_cast<unsigned char>(text[left]))) {
    ++left;
  }
  while (right >= left &&
         std::isspace(static_cast<unsigned char>(text[right]))) {
    --right;
  }

  return text.substr(left, right - left + 1);
}

static bool parseRange(const QString &input, int &r1, int &c1, int &r2,
                       int &c2) {
  QStringList parts = input.toUpper().split(":");
  if (parts.size() != 2)
    return false;

  auto parseRef = [](const QString &ref, int &row, int &col) {
    if (ref.isEmpty())
      return false;
    col = ref[0].toLatin1() - 'A';
    row = ref.mid(1).toInt() - 1;
    return col >= 0 && col < 26 && row >= 0;
  };

  return parseRef(parts[0], r1, c1) && parseRef(parts[1], r2, c2);
}
