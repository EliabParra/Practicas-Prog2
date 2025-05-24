#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <locale>
#include <windows.h>
#include <algorithm>

using namespace std;

void setColor(string color);
string toLower(string str);

int main() {
    setlocale(LC_ALL, "spanish");

    ifstream archivo("ejemplo.txt");
    int totalCoincidencias = 0;
    if (archivo.is_open()) {
        string linea;
        cout << "Contenido del archivo:\n";
        while (getline(archivo, linea)) {
            string lineaLower = toLower(linea);
            string palabra = "a";
            size_t pos = 0, lastPos = 0;
            while ((pos = lineaLower.find(palabra, lastPos)) != string::npos) {
                // Imprimir texto antes de la coincidencia
                cout << linea.substr(lastPos, pos - lastPos);
                // Imprimir la coincidencia en verde
                setColor("verde");
                cout << linea.substr(pos, palabra.length());
                setColor("blanco");
                lastPos = pos + palabra.length();
                totalCoincidencias++;
            }
            // Imprimir el resto de la línea
            cout << linea.substr(lastPos) << endl;
        }
        archivo.close();
        cout << "\nTotal de coincidencias encontradas: ";
        setColor("verde");
        cout << totalCoincidencias << endl;
        setColor("blanco");
    } else {
        cout << "No se pudo abrir el archivo para lectura." << endl;
    }
}

void setColor(string color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (color == "verde") {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    } else {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

string toLower(string str) {
    for (char& c : str) c = tolower(c);
    return str;
}