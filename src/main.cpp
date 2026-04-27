#include <iostream>
#include "spreadSheet.h"

int main() {
    SpreadSheet sheet(100, 26);  // 100 filas, 26 columnas (A-Z)

    sheet.insertCell(0, 0, "10");
    sheet.insertCell(0, 1, "20");
    sheet.insertCell(1, 0, "30");
    sheet.insertCell(1, 1, "texto");
    sheet.insertCell(2, 2, "50");

    sheet.printSheet();

    std::cout << "Celda [0,0]: " << sheet.getCell(0, 0) << "\n";
    std::cout << "Celda [1,1]: " << sheet.getCell(1, 1) << "\n";
    std::cout << "Celda vacia [5,5]: '" << sheet.getCell(5, 5) << "'\n";

    std::cout << "Suma fila 0: " << sheet.sumRow(0) << "\n";
    std::cout << "Suma col 0: " << sheet.sumCol(0) << "\n";
    std::cout << "Suma rango [0,0]-[2,2]: " << sheet.sumRange(0,0,2,2) << "\n";
    std::cout << "Max rango: " << sheet.maxRange(0,0,2,2) << "\n";
    std::cout << "Min rango: " << sheet.minRange(0,0,2,2) << "\n";

    sheet.deleteCell(0, 0);
    std::cout << "\nDespues de eliminar [0,0]:\n";
    sheet.printSheet();

    return 0;
}
