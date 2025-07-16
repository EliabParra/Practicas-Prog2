#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <unordered_map>
using namespace std;

// clase persona
class Person {
public:
    char nombre[50];
    char apellido[50];
    int ci;
    int edad;
    float iq;

    Person() : ci(0), edad(0), iq(0.0f) {
        nombre[0] = apellido[0] = '\0';
    }

    void input() {
        string temp;
        cout << "Nombre: ";
        cin.ignore();
        getline(cin, temp);
        strcpy(nombre, temp.c_str());

        cout << "Apellido: ";
        getline(cin, temp);
        strcpy(apellido, temp.c_str());

        cout << "Cedula de Identidad: ";
        cin >> ci;

        cout << "Edad: ";
        cin >> edad;

        cout << "IQ: ";
        cin >> iq;
    }

    void show() const {
        cout << "Nombre: " << nombre << endl;
        cout << "Apellido: " << apellido << endl;
        cout << "Cedula: " << ci << endl;
        cout << "Edad: " << edad << " años" << endl;
        cout << "IQ: " << iq << endl;
    }
};

// clase gestora de personas
class PersonManager {
private:
    const string ARCHIVO = "personas.dat";
    unordered_map<int, streampos> indiceCedula;

public:
    PersonManager() {
        buildIndex();
    }

    void buildIndex() {
        indiceCedula.clear();
        ifstream archivo(ARCHIVO, ios::binary);
        if (!archivo.is_open()) return;
        Person p;
        streampos pos = archivo.tellg();
        while (archivo.read((char*)&p, sizeof(Person))) {
            indiceCedula[p.ci] = pos;
            pos = archivo.tellg();
        }
        archivo.close();
    }

    void addPerson() {
        Person p;
        cout << "\n=== AGREGAR PERSONA ===" << endl;
        p.input();

        if (indiceCedula.find(p.ci) != indiceCedula.end()) {
            cout << "Error: Ya existe una persona con esa cedula." << endl;
            return;
        }

        ofstream archivoSalida(ARCHIVO, ios::binary | ios::app);
        if (archivoSalida.is_open()) {
            archivoSalida.write((char*)&p, sizeof(Person));
            archivoSalida.close();
            cout << "Persona agregada exitosamente." << endl;
            buildIndex();
        } else {
            cout << "Error al abrir el archivo." << endl;
        }
    }

    void searchByCI() {
        int ci;
        cout << "Ingrese la cedula a buscar: ";
        cin >> ci;

        auto it = indiceCedula.find(ci);
        if (it == indiceCedula.end()) {
            cout << "No se encontro ninguna persona con cedula " << ci << endl;
            return;
        }

        ifstream archivo(ARCHIVO, ios::binary);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo." << endl;
            return;
        }

        archivo.seekg(it->second);
        Person p;
        archivo.read((char*)&p, sizeof(Person));
        cout << "\n=== PERSONA ENCONTRADA ===" << endl;
        p.show();
        archivo.close();
    }

    void listPersons() {
        ifstream archivo(ARCHIVO, ios::binary);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo." << endl;
            return;
        }

        Person p;
        int contador = 0;

        cout << "\n=== LISTA DE PERSONAS ===" << endl;
        cout << "----------------------------------------" << endl;

        while (archivo.read((char*)&p, sizeof(Person))) {
            contador++;
            cout << "Persona #" << contador << ":" << endl;
            p.show();
            cout << "----------------------------------------" << endl;
        }

        archivo.close();

        if (contador == 0) {
            cout << "No hay personas registradas." << endl;
        } else {
            cout << "Total de personas: " << contador << endl;
        }
    }

    void searchByAge() {
        int edad;
        cout << "Ingrese la edad a buscar: ";
        cin >> edad;

        ifstream archivo(ARCHIVO, ios::binary);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo." << endl;
            return;
        }

        Person p;
        int contador = 0;

        cout << "\n=== PERSONAS CON EDAD " << edad << " ===" << endl;
        cout << "----------------------------------------" << endl;

        while (archivo.read((char*)&p, sizeof(Person))) {
            if (p.edad == edad) {
                contador++;
                cout << "Persona #" << contador << ":" << endl;
                p.show();
                cout << "----------------------------------------" << endl;
            }
        }

        archivo.close();

        if (contador == 0) {
            cout << "No se encontraron personas con edad " << edad << endl;
        } else {
            cout << "Total encontradas: " << contador << endl;
        }
    }

    void deletePerson() {
        int ci;
        cout << "Ingrese la cedula de la persona a eliminar: ";
        cin >> ci;

        ifstream archivo(ARCHIVO, ios::binary);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo." << endl;
            return;
        }

        vector<Person> personas;
        Person p;
        bool encontrada = false;

        while (archivo.read((char*)&p, sizeof(Person))) {
            if (p.ci == ci) {
                encontrada = true;
                cout << "Persona encontrada y eliminada:" << endl;
                p.show();
            } else {
                personas.push_back(p);
            }
        }
        archivo.close();

        if (!encontrada) {
            cout << "No se encontro ninguna persona con cedula " << ci << endl;
            return;
        }

        ofstream archivoSalida(ARCHIVO, ios::binary);
        if (archivoSalida.is_open()) {
            for (int i = 0; i < personas.size(); i++) {
                archivoSalida.write((char*)&personas[i], sizeof(Person));
            }
            archivoSalida.close();
            cout << "Persona eliminada exitosamente." << endl;
            buildIndex();
        } else {
            cout << "Error al reescribir el archivo." << endl;
        }
    }

    void editPerson() {
        int ci;
        cout << "Ingrese la cedula de la persona a editar: ";
        cin >> ci;

        ifstream archivo(ARCHIVO, ios::binary);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo." << endl;
            return;
        }

        vector<Person> personas;
        Person p;
        bool encontrada = false;

        while (archivo.read((char*)&p, sizeof(Person))) {
            if (p.ci == ci) {
                encontrada = true;
                cout << "\nPersona actual:" << endl;
                p.show();

                string temp;
                cout << "\n=== EDITAR DATOS ===" << endl;
                cout << "Nuevo nombre (actual: " << p.nombre << "): ";
                cin.ignore();
                getline(cin, temp);
                if (temp.length() > 0) strcpy(p.nombre, temp.c_str());

                cout << "Nuevo apellido (actual: " << p.apellido << "): ";
                getline(cin, temp);
                if (temp.length() > 0) strcpy(p.apellido, temp.c_str());

                cout << "Nueva edad (actual: " << p.edad << ", 0 para no cambiar): ";
                int nuevaEdad;
                cin >> nuevaEdad;
                if (nuevaEdad > 0) p.edad = nuevaEdad;

                cout << "Nuevo IQ (actual: " << p.iq << ", 0 para no cambiar): ";
                float nuevoIq;
                cin >> nuevoIq;
                if (nuevoIq > 0) p.iq = nuevoIq;
            }
            personas.push_back(p);
        }
        archivo.close();

        if (!encontrada) {
            cout << "No se encontro ninguna persona con cedula " << ci << endl;
            return;
        }

        ofstream archivoSalida(ARCHIVO, ios::binary);
        if (archivoSalida.is_open()) {
            for (int i = 0; i < personas.size(); i++) {
                archivoSalida.write((char*)&personas[i], sizeof(Person));
            }
            archivoSalida.close();
            cout << "Datos actualizados exitosamente." << endl;
            buildIndex();
        } else {
            cout << "Error al actualizar el archivo." << endl;
        }
    }

    void showOldest() {
        ifstream archivo(ARCHIVO, ios::binary);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo." << endl;
            return;
        }

        vector<Person> personas;
        Person p;

        while (archivo.read((char*)&p, sizeof(Person))) {
            personas.push_back(p);
        }
        archivo.close();

        if (personas.size() == 0) {
            cout << "No hay personas registradas." << endl;
            return;
        }

        int edadMaxima = personas[0].edad;
        for (int i = 1; i < personas.size(); i++) {
            if (personas[i].edad > edadMaxima) {
                edadMaxima = personas[i].edad;
            }
        }

        cout << "\n=== PERSONA(S) CON MAYOR EDAD (" << edadMaxima << " años) ===" << endl;
        cout << "----------------------------------------" << endl;

        int contador = 0;
        for (int i = 0; i < personas.size(); i++) {
            if (personas[i].edad == edadMaxima) {
                contador++;
                cout << "Persona #" << contador << ":" << endl;
                personas[i].show();
                cout << "----------------------------------------" << endl;
            }
        }
    }
};

void mostrarMenu() {
    cout << "\n===============================================" << endl;
    cout << "        SISTEMA DE GESTION DE PERSONAS" << endl;
    cout << "===============================================" << endl;
    cout << "1. Agregar persona" << endl;
    cout << "2. Buscar persona por cedula" << endl;
    cout << "3. Listar todas las personas" << endl;
    cout << "4. Buscar personas por edad" << endl;
    cout << "5. Eliminar persona" << endl;
    cout << "6. Editar datos de persona" << endl;
    cout << "7. Mostrar persona(s) con mayor edad" << endl;
    cout << "8. Salir" << endl;
    cout << "===============================================" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    int opcion;
    PersonManager manager;

    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1: manager.addPerson(); break;
            case 2: manager.searchByCI(); break;
            case 3: manager.listPersons(); break;
            case 4: manager.searchByAge(); break;
            case 5: manager.deletePerson(); break;
            case 6: manager.editPerson(); break;
            case 7: manager.showOldest(); break;
            case 8: cout << "Gracias por usar el sistema!" << endl; break;
            default: cout << "Opcion invalida. Seleccione del 1 al 8." << endl;
        }

        if (opcion != 8) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }

    } while (opcion != 8);

    return 0;
}