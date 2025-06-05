#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

struct Persona {
    char nombre[50];
    char apellido[50];
    int ci;
    int edad;
    float iq;
};

const string ARCHIVO = "personas.dat";

void mostrarPersona(Persona p) {
    cout << "Nombre: " << p.nombre << endl;
    cout << "Apellido: " << p.apellido << endl;
    cout << "Cedula: " << p.ci << endl;
    cout << "Edad: " << p.edad << " años" << endl;
    cout << "IQ: " << p.iq << endl;
}

void agregarPersona() {
    Persona p;
    string temp;
    
    cout << "\n=== AGREGAR PERSONA ===" << endl;
    cout << "Nombre: ";
    cin.ignore();
    getline(cin, temp);
    strcpy(p.nombre, temp.c_str());
    
    cout << "Apellido: ";
    getline(cin, temp);
    strcpy(p.apellido, temp.c_str());
    
    cout << "Cedula de Identidad: ";
    cin >> p.ci;
    
    cout << "Edad: ";
    cin >> p.edad;
    
    // Verificar si ya existe la cedula
    ifstream archivo(ARCHIVO, ios::binary);
    if (archivo.is_open()) {
        Persona temp_p;
        while (archivo.read((char*)&temp_p, sizeof(Persona))) {
            if (temp_p.ci == p.ci) {
                cout << "Error: Ya existe una persona con esa cedula." << endl;
                archivo.close();
                return;
            }
        }
        archivo.close();
    }
    
    // Agregar al archivo
    ofstream archivoSalida(ARCHIVO, ios::binary | ios::app);
    if (archivoSalida.is_open()) {
        archivoSalida.write((char*)&p, sizeof(Persona));
        archivoSalida.close();
        cout << "Persona agregada exitosamente." << endl;
    } else {
        cout << "Error al abrir el archivo." << endl;
    }
}

void buscarPorCedula() {
    int ci;
    cout << "Ingrese la cedula a buscar: ";
    cin >> ci;
    
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }
    
    Persona p;
    bool encontrada = false;
    
    while (archivo.read((char*)&p, sizeof(Persona))) {
        if (p.ci == ci) {
            cout << "\n=== PERSONA ENCONTRADA ===" << endl;
            mostrarPersona(p);
            encontrada = true;
            break;
        }
    }
    
    archivo.close();
    
    if (!encontrada) {
        cout << "No se encontro ninguna persona con cedula " << ci << endl;
    }
}

void listarPersonas() {
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }
    
    Persona p;
    int contador = 0;
    
    cout << "\n=== LISTA DE PERSONAS ===" << endl;
    cout << "----------------------------------------" << endl;
    
    while (archivo.read((char*)&p, sizeof(Persona))) {
        contador++;
        cout << "Persona #" << contador << ":" << endl;
        mostrarPersona(p);
        cout << "----------------------------------------" << endl;
    }
    
    archivo.close();
    
    if (contador == 0) {
        cout << "No hay personas registradas." << endl;
    } else {
        cout << "Total de personas: " << contador << endl;
    }
}

void buscarPorEdad() {
    int edad;
    cout << "Ingrese la edad a buscar: ";
    cin >> edad;
    
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }
    
    Persona p;
    int contador = 0;
    
    cout << "\n=== PERSONAS CON EDAD " << edad << " ===" << endl;
    cout << "----------------------------------------" << endl;
    
    while (archivo.read((char*)&p, sizeof(Persona))) {
        if (p.edad == edad) {
            contador++;
            cout << "Persona #" << contador << ":" << endl;
            mostrarPersona(p);
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

void eliminarPersona() {
    int ci;
    cout << "Ingrese la cedula de la persona a eliminar: ";
    cin >> ci;
    
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }
    
    vector<Persona> personas;
    Persona p;
    bool encontrada = false;
    
    // Leer todas las personas excepto la que se va a eliminar
    while (archivo.read((char*)&p, sizeof(Persona))) {
        if (p.ci == ci) {
            encontrada = true;
            cout << "Persona encontrada y eliminada:" << endl;
            mostrarPersona(p);
        } else {
            personas.push_back(p);
        }
    }
    archivo.close();
    
    if (!encontrada) {
        cout << "No se encontro ninguna persona con cedula " << ci << endl;
        return;
    }
    
    // Reescribir el archivo sin la persona eliminada
    ofstream archivoSalida(ARCHIVO, ios::binary);
    if (archivoSalida.is_open()) {
        for (int i = 0; i < personas.size(); i++) {
            archivoSalida.write((char*)&personas[i], sizeof(Persona));
        }
        archivoSalida.close();
        cout << "Persona eliminada exitosamente." << endl;
    } else {
        cout << "Error al reescribir el archivo." << endl;
    }
}

void editarPersona() {
    int ci;
    cout << "Ingrese la cedula de la persona a editar: ";
    cin >> ci;
    
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }
    
    vector<Persona> personas;
    Persona p;
    bool encontrada = false;
    
    // Buscar la persona y cargar todas en memoria
    while (archivo.read((char*)&p, sizeof(Persona))) {
        if (p.ci == ci) {
            encontrada = true;
            cout << "\nPersona actual:" << endl;
            mostrarPersona(p);
            
            // Editar los datos
            string temp;
            cout << "\n=== EDITAR DATOS ===" << endl;
            cout << "Nuevo nombre (actual: " << p.nombre << "): ";
            cin.ignore();
            getline(cin, temp);
            if (temp.length() > 0) {
                strcpy(p.nombre, temp.c_str());
            }
            
            cout << "Nuevo apellido (actual: " << p.apellido << "): ";
            getline(cin, temp);
            if (temp.length() > 0) {
                strcpy(p.apellido, temp.c_str());
            }
            
            cout << "Nueva edad (actual: " << p.edad << ", 0 para no cambiar): ";
            int nuevaEdad;
            cin >> nuevaEdad;
            if (nuevaEdad > 0) {
                p.edad = nuevaEdad;
            }
        }
        personas.push_back(p);
    }
    archivo.close();
    
    if (!encontrada) {
        cout << "No se encontro ninguna persona con cedula " << ci << endl;
        return;
    }
    
    // Reescribir el archivo con los datos actualizados
    ofstream archivoSalida(ARCHIVO, ios::binary);
    if (archivoSalida.is_open()) {
        for (int i = 0; i < personas.size(); i++) {
            archivoSalida.write((char*)&personas[i], sizeof(Persona));
        }
        archivoSalida.close();
        cout << "Datos actualizados exitosamente." << endl;
    } else {
        cout << "Error al actualizar el archivo." << endl;
    }
}

void personasMayorEdad() {
    ifstream archivo(ARCHIVO, ios::binary);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }
    
    vector<Persona> personas;
    Persona p;
    
    // Cargar todas las personas
    while (archivo.read((char*)&p, sizeof(Persona))) {
        personas.push_back(p);
    }
    archivo.close();
    
    if (personas.size() == 0) {
        cout << "No hay personas registradas." << endl;
        return;
    }
    
    // Encontrar la edad maxima
    int edadMaxima = personas[0].edad;
    for (int i = 1; i < personas.size(); i++) {
        if (personas[i].edad > edadMaxima) {
            edadMaxima = personas[i].edad;
        }
    }
    
    // Mostrar todas las personas con la edad maxima
    cout << "\n=== PERSONA(S) CON MAYOR EDAD (" << edadMaxima << " años) ===" << endl;
    cout << "----------------------------------------" << endl;
    
    int contador = 0;
    for (int i = 0; i < personas.size(); i++) {
        if (personas[i].edad == edadMaxima) {
            contador++;
            cout << "Persona #" << contador << ":" << endl;
            mostrarPersona(personas[i]);
            cout << "----------------------------------------" << endl;
        }
    }
}

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
    
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch (opcion) {
            case 1:
                agregarPersona();
                break;
            case 2:
                buscarPorCedula();
                break;
            case 3:
                listarPersonas();
                break;
            case 4:
                buscarPorEdad();
                break;
            case 5:
                eliminarPersona();
                break;
            case 6:
                editarPersona();
                break;
            case 7:
                personasMayorEdad();
                break;
            case 8:
                cout << "Gracias por usar el sistema!" << endl;
                break;
            default:
                cout << "Opcion invalida. Seleccione del 1 al 8." << endl;
        }
        
        if (opcion != 8) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while (opcion != 8);
    
    return 0;
}