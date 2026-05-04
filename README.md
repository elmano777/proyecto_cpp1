# Proyecto Nro. 1: Hoja de Cálculo Simple con Matrices Dispersas

**Curso:** Algoritmos y Estructuras de Datos  
**Institución:** Universidad de Ingeniería y Tecnología (UTEC)  
**Integrantes:**
*   Rolando David Diaz Jara
*   Miguel Angel Ramos Bonilla

## 1. Descripción del Proyecto
Este proyecto consiste en el diseño e implementación de una aplicación de **hoja de cálculo funcional** utilizando **matrices dispersas** (*sparse matrices*). El objetivo central es manejar eficientemente grandes volúmenes de celdas donde la mayoría se encuentran vacías, optimizando tanto el uso de memoria como el tiempo de ejecución de las operaciones básicas y de agregación.

## 2. Instrucciones de Compilación y Ejecución

La aplicación ha sido desarrollada en **C++17** con interfaz gráfica en **Qt6**.

### Requisitos previos

- Compilador compatible con C++17 (GCC o Clang)
- Qt6 instalado
- `bear` (opcional, solo para IntelliSense en el IDE)

---

### Linux (Fedora/Ubuntu)

**Instalar Qt6:**
```bash
# Fedora
sudo dnf install qt6-qtbase-devel qt6-qtwidgets-devel

# Ubuntu/Debian
sudo apt install qt6-base-dev
```

**Compilar y ejecutar:**
```bash
make run
```

---

### Windows

**Instalar Qt6:**
1. Descarga el instalador desde https://www.qt.io/download-open-source
2. Instala Qt 6.x con el componente **MinGW** incluido
3. Agrega al PATH: `C:\Qt\6.x.x\mingw_64\bin` y `C:\Qt\Tools\mingw_xx\bin`

**Compilar y ejecutar:**
```bash
mingw32-make -f Makefile
spreadsheet.exe
```

O desde Qt Creator: File → Open → selecciona el `CMakeLists.txt`.

---

### macOS

**Instalar Qt6:**
```bash
brew install qt6
```

**Compilar y ejecutar:**
```bash
export PATH="/opt/homebrew/opt/qt/bin:$PATH"
make run
```

---

### Nota sobre el Makefile

El `Makefile` usa `pkg-config` para detectar Qt6 automáticamente en Linux y macOS.
En Windows se recomienda usar **Qt Creator** para abrir el proyecto directamente.

## 3. Operaciones Implementadas
*   **Gestión de Celdas:** Inserción, consulta, modificación y eliminación individual.
*   **Gestión Estructural:** Eliminación de filas, columnas y rangos rectangulares (ej. A1:C4).
*   **Funciones de Agregación:** Suma, promedio, máximo y mínimo en rangos de celdas.
*   **Soporte de Fórmulas:** Procesamiento de expresiones aritméticas básicas (ej. `=A1+B1`).
*   **Casos Borde:** Manejo de celdas inexistentes, rangos vacíos y celdas con texto en operaciones numéricas.
