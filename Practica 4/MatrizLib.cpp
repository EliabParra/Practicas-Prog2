#include "MatrizLib.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>

// Constructor por defecto
Matriz::Matriz() : datos(nullptr), filas(0), columnas(0) {}

// Constructor parametrizado
Matriz::Matriz(int f, int c) : filas(f), columnas(c) {
    if (f <= 0 || c <= 0) {
        std::cout << "Las dimensiones deben ser positivas";
        return;
    }
    asignarMemoria();
    
    // Inicializar todos los elementos en 0
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            datos[i][j] = 0.0;
        }
    }
}

// Constructor de copia
Matriz::Matriz(const Matriz& otra) : filas(otra.filas), columnas(otra.columnas) {
    if (otra.datos == nullptr) {
        datos = nullptr;
        return;
    }
    asignarMemoria();
    copiarDatos(otra);
}

// Destructor
Matriz::~Matriz() {
    liberarMemoria();
}

// Operador de asignación
Matriz& Matriz::operator=(const Matriz& otra) {
    if (this != &otra) {
        liberarMemoria();
        filas = otra.filas;
        columnas = otra.columnas;
        
        if (otra.datos != nullptr) {
            asignarMemoria();
            copiarDatos(otra);
        } else {
            datos = nullptr;
        }
    }
    return *this;
}

// Método auxiliar para asignar memoria
void Matriz::asignarMemoria() {
    datos = new double*[filas];
    for (int i = 0; i < filas; i++) {
        datos[i] = new double[columnas];
    }
}

// Método auxiliar para liberar memoria
void Matriz::liberarMemoria() {
    if (datos != nullptr) {
        for (int i = 0; i < filas; i++) {
            delete[] datos[i];
        }
        delete[] datos;
        datos = nullptr;
    }
}

// Método auxiliar para copiar datos
void Matriz::copiarDatos(const Matriz& otra) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            datos[i][j] = otra.datos[i][j];
        }
    }
}

// Llenar matriz con datos del usuario
void Matriz::llenarMatriz() {
    if (datos == nullptr) {
        std::cout << "Matriz no inicializada";
        return;
    }
    
    std::cout << "Ingrese los elementos de la matriz " << filas << "x" << columnas << ":\n";
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            bool entradaValida = false;
            while (!entradaValida) {
                std::cout << "Elemento [" << i+1 << "][" << j+1 << "]: ";
                if (std::cin >> datos[i][j]) {
                    entradaValida = true;
                } else {
                    std::cout << "Entrada inválida. Por favor ingrese un número.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                }
            }
        }
    }
}

// Mostrar matriz en formato tabular
void Matriz::mostrarMatriz() const {
    if (datos == nullptr) {
        std::cout << "Matriz vacía\n";
        return;
    }
    
    std::cout << std::fixed << std::setprecision(2);
    for (int i = 0; i < filas; i++) {
        std::cout << "| ";
        for (int j = 0; j < columnas; j++) {
            std::cout << std::setw(8) << datos[i][j] << " ";
        }
        std::cout << "|\n";
    }
}

// Calcular transpuesta
Matriz Matriz::transpuesta() const {
    if (datos == nullptr) {
        std::cout << "No se puede calcular transpuesta de matriz vacía";
        return;
    }
    
    Matriz resultado(columnas, filas);
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            resultado.datos[j][i] = datos[i][j];
        }
    }
    
    return resultado;
}

// Calcular determinante
double Matriz::determinante() const {
    if (datos == nullptr) {
        std::cout << "No se puede calcular determinante de matriz vacía";
        return;
    }
    
    if (filas != columnas) {
        std::cout << "El determinante solo se puede calcular para matrices cuadradas";
        return;
    }
    
    return calcularDeterminante(datos, filas);
}

// Método auxiliar para calcular determinante recursivamente
double Matriz::calcularDeterminante(double** matriz, int n) const {
    if (n == 1) {
        return matriz[0][0];
    }
    
    if (n == 2) {
        return matriz[0][0] * matriz[1][1] - matriz[0][1] * matriz[1][0];
    }
    
    double det = 0.0;
    double** submatriz = new double*[n-1];
    for (int i = 0; i < n-1; i++) {
        submatriz[i] = new double[n-1];
    }
    
    for (int x = 0; x < n; x++) {
        crearSubmatriz(matriz, submatriz, 0, x, n);
        det += (x % 2 == 0 ? 1 : -1) * matriz[0][x] * calcularDeterminante(submatriz, n-1);
    }
    
    // Liberar memoria de submatriz
    for (int i = 0; i < n-1; i++) {
        delete[] submatriz[i];
    }
    delete[] submatriz;
    
    return det;
}

// Crear submatriz para cofactores
void Matriz::crearSubmatriz(double** matriz, double** submatriz, int fila, int col, int n) const {
    int subfila = 0, subcol = 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != fila && j != col) {
                submatriz[subfila][subcol++] = matriz[i][j];
                if (subcol == n-1) {
                    subcol = 0;
                    subfila++;
                }
            }
        }
    }
}

// Suma de matrices
Matriz Matriz::suma(const Matriz& otra) const {
    if (datos == nullptr || otra.datos == nullptr) {
        std::cout << "No se pueden sumar matrices vacías";
        return;
    }
    
    if (filas != otra.filas || columnas != otra.columnas) {
        std::cout << "Las matrices deben tener las mismas dimensiones para la suma";
        return;
    }
    
    Matriz resultado(filas, columnas);
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            resultado.datos[i][j] = datos[i][j] + otra.datos[i][j];
        }
    }
    
    return resultado;
}

// Multiplicación de matrices
Matriz Matriz::multiplicacion(const Matriz& otra) const {
    if (datos == nullptr || otra.datos == nullptr) {
        std::cout << "No se pueden multiplicar matrices vacías";
        return;
    }
    
    if (columnas != otra.filas) {
        std::cout << "El número de columnas de la primera matriz debe ser igual al número de filas de la segunda";
        return;
    }
    
    Matriz resultado(filas, otra.columnas);
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < otra.columnas; j++) {
            resultado.datos[i][j] = 0.0;
            for (int k = 0; k < columnas; k++) {
                resultado.datos[i][j] += datos[i][k] * otra.datos[k][j];
            }
        }
    }
    
    return resultado;
}

// Comparar si dos matrices son iguales
bool Matriz::esIgual(const Matriz& otra) const {
    if (filas != otra.filas || columnas != otra.columnas) {
        return false;
    }
    
    if (datos == nullptr && otra.datos == nullptr) {
        return true;
    }
    
    if (datos == nullptr || otra.datos == nullptr) {
        return false;
    }
    
    const double EPSILON = 1e-9;
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (std::abs(datos[i][j] - otra.datos[i][j]) > EPSILON) {
                return false;
            }
        }
    }
    
    return true;
}

// Añadir una fila a la matriz
void Matriz::añadirFila() {
    if (datos == nullptr) {
        std::cout << "No se puede añadir fila a matriz vacía";
        return;
    }
    
    // Crear nueva matriz con una fila más
    double** nuevos_datos = new double*[filas + 1];
    
    // Copiar datos existentes
    for (int i = 0; i < filas; i++) {
        nuevos_datos[i] = new double[columnas];
        for (int j = 0; j < columnas; j++) {
            nuevos_datos[i][j] = datos[i][j];
        }
    }
    
    // Crear nueva fila e inicializar en 0
    nuevos_datos[filas] = new double[columnas];
    for (int j = 0; j < columnas; j++) {
        nuevos_datos[filas][j] = 0.0;
    }
    
    // Liberar memoria anterior
    liberarMemoria();
    
    // Actualizar atributos
    datos = nuevos_datos;
    filas++;
}

// Añadir una columna a la matriz
void Matriz::añadirColumna() {
    if (datos == nullptr) {
        std::cout << "No se puede añadir columna a matriz vacía";
        return;
    }
    
    // Crear nueva matriz con una columna más
    double** nuevos_datos = new double*[filas];
    
    for (int i = 0; i < filas; i++) {
        nuevos_datos[i] = new double[columnas + 1];
        
        // Copiar datos existentes
        for (int j = 0; j < columnas; j++) {
            nuevos_datos[i][j] = datos[i][j];
        }
        
        // Inicializar nueva columna en 0
        nuevos_datos[i][columnas] = 0.0;
    }
    
    // Liberar memoria anterior
    liberarMemoria();
    
    // Actualizar atributos
    datos = nuevos_datos;
    columnas++;
}

// Getter para elemento específico
double Matriz::getElemento(int i, int j) const {
    if (i < 0 || i >= filas || j < 0 || j >= columnas) {
        std::cout << "Índices fuera de rango";
        return;
    }
    return datos[i][j];
}

// Setter para elemento específico
void Matriz::setElemento(int i, int j, double valor) {
    if (i < 0 || i >= filas || j < 0 || j >= columnas) {
        std::cout << "Índices fuera de rango";
        return;
    }
    datos[i][j] = valor;
}

// Implementación del namespace MatrizLib
namespace MatrizLib {
    Matriz crearMatriz(int filas, int columnas) {
        if (filas <= 0 || columnas <= 0) {
            std::cout << "Las dimensiones deben ser positivas";
            return;
        }
        
        return Matriz(filas, columnas);
    }
}