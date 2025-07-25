#ifndef MATRIZ_HPP
#define MATRIZ_HPP

#include <iostream>
#include <iomanip>

class Matriz {
private:
    double** datos;
    int filas;
    int columnas;
    
    void asignarMemoria();
    
    void liberarMemoria();
    
    void copiarDatos(const Matriz& otra);

public:
    Matriz();
    Matriz(int f, int c);
    Matriz(const Matriz& otra);
    
    ~Matriz();
    
    Matriz& operator=(const Matriz& otra);
    
    void llenarMatriz();
    void mostrarMatriz() const;
    Matriz transpuesta() const;
    double determinante() const;
    Matriz suma(const Matriz& otra) const;
    Matriz multiplicacion(const Matriz& otra) const;
    bool esIgual(const Matriz& otra) const;
    
    void addFila();
    void addColumna();
    
    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }
    double getElemento(int i, int j) const;
    void setElemento(int i, int j, double valor);
    
private:
    double calcularDeterminante(double** matriz, int n) const;
    
    void crearSubmatriz(double** matriz, double** submatriz, int fila, int col, int n) const;
};

namespace MatrizLib {
    Matriz crearMatriz(int filas, int columnas);
}

#endif // MATRIZ_HPP