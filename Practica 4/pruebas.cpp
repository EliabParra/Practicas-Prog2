#include "MatrizLib.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

int main() {
    std::cout << "--- Creando Matriz 1 (2x2) ---\n";
    Matriz mat1 = MatrizLib::crearMatriz(3, 3);
    mat1.llenarMatriz();
    std::cout << "\nMatriz 1:\n";
    mat1.mostrarMatriz();
    
    std::cout << "\n--- Creando Matriz 2 (2x2) ---\n";
    Matriz mat2 = MatrizLib::crearMatriz(2, 2);
    mat2.llenarMatriz();
    std::cout << "\nMatriz 2:\n";
    mat2.mostrarMatriz();
    
    std::cout << "\n--- Suma de matrices ---\n";
    Matriz suma = mat1.suma(mat2);
    std::cout << "Resultado de la suma:\n";
    suma.mostrarMatriz();
    
    std::cout << "\n--- Multiplicación de matrices ---\n";
    Matriz producto = mat1.multiplicacion(mat2);
    std::cout << "Resultado de la multiplicación:\n";
    producto.mostrarMatriz();
    
    std::cout << "\n--- Determinante de Matriz 1 ---\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "El determinante es: " << mat1.determinante() << "\n";
    
    std::cout << "\n--- Transpuesta de Matriz 1 ---\n";
    std::cout << "Matriz 1 original:\n";
    mat1.mostrarMatriz();
    Matriz transpuesta = mat1.transpuesta();
    std::cout << "Transpuesta de Matriz 1:\n";
    transpuesta.mostrarMatriz();
    
    std::cout << "\n--- Comparación de matrices ---\n";
    if (mat1.esIgual(mat2)) {
        std::cout << "La Matriz 1 y la Matriz 2 son iguales.\n";
    } else {
        std::cout << "La Matriz 1 y la Matriz 2 son diferentes.\n";
    }
    
    std::cout << "\n--- Añadir fila a Matriz 1 ---\n";
    mat1.addFila();
    std::cout << "Matriz 1 después de añadir una fila:\n";
    mat1.mostrarMatriz();
    
    std::cout << "\n--- Añadir columna a Matriz 1 ---\n";
    mat1.addColumna();
    std::cout << "Matriz 1 después de añadir una columna:\n";
    mat1.mostrarMatriz();
    
    return 0;
}