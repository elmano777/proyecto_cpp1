/*
 * Punto de entrada de la aplicación.
 *
 * Responsabilidades:
 * - Inicializar la aplicación Qt (QApplication)
 * - Crear la ventana principal (MainWindow)
 * - Mostrar la interfaz gráfica
 * - Ejecutar el bucle principal de eventos
 *
 * Este archivo conecta la lógica del programa con la interfaz gráfica.
 */

#include "ui/mainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow window;
  window.show();
  return app.exec();
}
