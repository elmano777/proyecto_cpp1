# Proyecto Nro. 1: Hoja de Cálculo Simple con Matrices Dispersas

**Curso:** Algoritmos y Estructuras de Datos  
**Institución:** Universidad de Ingeniería y Tecnología (UTEC)  
**Integrantes:**
*   [Nombre del Integrante 1]
*   [Nombre del Integrante 2]
*   [Nombre del Integrante 3]

## 1. Descripción del Proyecto
Este proyecto consiste en el diseño e implementación de una aplicación de **hoja de cálculo funcional** utilizando **matrices dispersas** (*sparse matrices*). El objetivo central es manejar eficientemente grandes volúmenes de celdas donde la mayoría se encuentran vacías, optimizando tanto el uso de memoria como el tiempo de ejecución de las operaciones básicas y de agregación.

## 2. Justificación de la Estructura de Datos
Para este proyecto, se ha implementado una **lista enlazada cruzada** (*cross-linked list*) de forma manual, cumpliendo con la restricción de no utilizar contenedores asociativos de la STL como `std::map` para la estructura principal.

### Eficiencia de Memoria
A diferencia de una matriz densa tradicional (bidimensional), nuestra estructura **solo crea nodos para las celdas que contienen información**. En una hoja de cálculo con miles de celdas potenciales, esto evita el desperdicio masivo de memoria al no almacenar valores nulos o vacíos.

### Eficiencia de Operaciones
El uso de punteros `nextInRow` y `nextInCol`, junto con **nodos cabecera** para cada fila y columna activa, permite que las operaciones de inserción y eliminación se realicen con un costo de **$O(k)$**, donde $k$ representa el número de elementos con datos en la fila o columna respectiva. Esto es significativamente más eficiente que recorrer una matriz densa de tamaño $n \times m$.

## 3. Instrucciones de Compilación y Ejecución
La aplicación ha sido desarrollada en **C++17** utilizando la biblioteca gráfica **[Especificar aquí: Qt / SFML / wxWidgets]**.

### Requisitos previos:
*   Compilador compatible con C++17 (GCC, Clang o MSVC).
*   Librería [Nombre de la librería GUI] instalada.

### Pasos para compilar:
```bash
# Ejemplo si usas Makefile
make all

# Ejemplo si usas CMake
mkdir build && cd build
cmake ..
make
```

### Pasos para ejecutar:
```bash
./hoja_calculo_ejecutable
```

## 4. Análisis de Complejidad
Basándonos en la implementación de listas enlazadas cruzadas, la complejidad asintótica de las operaciones clave es la siguiente:

| Operación | Complejidad Temporal (Peor Caso) | Justificación |
| :--- | :--- | :--- |
| **Insertar Celda** | $O(k)$ | Búsqueda y actualización de punteros en fila/columna activa. |
| **Consultar Celda** | $O(k)$ | Recorrido lineal sobre los nodos con contenido en la fila. |
| **Eliminar Celda** | $O(k)$ | Localización del nodo y reajuste de enlaces vecinos. |
| **Suma/Promedio Rango** | $O(k \cdot r)$ | Recorrido de los elementos con valor dentro del área rectangular definida. |
| **Eliminar Fila/Columna** | $O(k)$ | Eliminación secuencial de todos los nodos enlazados a la cabecera. |

## 5. Operaciones Implementadas
*   **Gestión de Celdas:** Inserción, consulta, modificación y eliminación individual.
*   **Gestión Estructural:** Eliminación de filas, columnas y rangos rectangulares (ej. A1:C4).
*   **Funciones de Agregación:** Suma, promedio, máximo y mínimo en rangos de celdas.
*   **Soporte de Fórmulas:** Procesamiento de expresiones aritméticas básicas (ej. `=A1+B1`).
*   **Casos Borde:** Manejo de celdas inexistentes, rangos vacíos y celdas con texto en operaciones numéricas.

---
*Nota: Este proyecto cumple con las reglas de integridad académica de la institución, asegurando una implementación original basada en punteros y nodos propios.*
