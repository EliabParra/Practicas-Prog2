#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

const char* FILENAME = "productos.dat";

struct Producto {
    int codigo;
    char nombre[31];
    float precio;
    int stock;
    char categoria[21];
    bool activo;
};

void agregarProducto();
void mostrarProductos();
void mostrarPorCategoria();
void buscarPorCodigo();
void modificarProducto();
void eliminarProducto();
int buscarPosPorCodigo(int codigo);
void limpiarBuffer();

int main() {
    int opcion;
    do {
        cout << "\n--- MENU INVENTARIO ---\n";
        cout << "1. Agregar producto\n";
        cout << "2. Mostrar productos activos\n";
        cout << "3. Mostrar productos por categoria\n";
        cout << "4. Buscar producto por codigo\n";
        cout << "5. Modificar producto\n";
        cout << "6. Eliminar producto (borrado logico)\n";
        cout << "7. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        limpiarBuffer();
        switch(opcion) {
            case 1: agregarProducto(); break;
            case 2: mostrarProductos(); break;
            case 3: mostrarPorCategoria(); break;
            case 4: buscarPorCodigo(); break;
            case 5: modificarProducto(); break;
            case 6: eliminarProducto(); break;
            case 7: cout << "Saliendo...\n"; break;
            default: cout << "Opcion invalida.\n";
        }
    } while(opcion != 7);
    return 0;
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

int buscarPosPorCodigo(int codigo) {
    ifstream file(FILENAME, ios::binary);
    if (!file) return -1;
    Producto p;
    int pos = 0;
    while (file.read((char*)&p, sizeof(Producto))) {
        if (p.codigo == codigo) {
            file.close();
            return pos;
        }
        pos++;
    }
    file.close();
    return -1;
}

void agregarProducto() {
    Producto p;
    cout << "Codigo: ";
    cin >> p.codigo;
    limpiarBuffer();
    if (buscarPosPorCodigo(p.codigo) != -1) {
        cout << "Ya existe un producto con ese codigo.\n";
        system("pause");
        system("cls");
        return;
    }
    cout << "Nombre: ";
    cin.getline(p.nombre, 31);
    cout << "Precio: ";
    cin >> p.precio;
    cout << "Stock: ";
    cin >> p.stock;
    limpiarBuffer();
    cout << "Categoria: ";
    cin.getline(p.categoria, 21);
    p.activo = true;

    ofstream file(FILENAME, ios::binary | ios::app);
    if (!file) {
        cout << "No se pudo abrir el archivo.\n";
        system("pause");
        system("cls");
        return;
    }
    file.write((char*)&p, sizeof(Producto));
    file.close();
    cout << "Producto agregado.\n";
    system("pause");
    system("cls");
}

void mostrarProductos() {
    ifstream file(FILENAME, ios::binary);
    if (!file) {
        cout << "No hay productos.\n";
        system("pause");
        system("cls");
        return;
    }
    Producto p;
    cout << "\n--- Productos activos ---\n";
    while (file.read((char*)&p, sizeof(Producto))) {
        if (p.activo) {
            cout << "Codigo: " << p.codigo << ", Nombre: " << p.nombre
                << ", Precio: " << p.precio << ", Stock: " << p.stock
                << ", Categoria: " << p.categoria << endl;
        }
    }
    file.close();
    system("pause");
    system("cls");
}

void mostrarPorCategoria() {
    char cat[21];
    cout << "Ingrese categoria: ";
    cin.getline(cat, 21);
    ifstream file(FILENAME, ios::binary);
    if (!file) {
        cout << "No hay productos.\n";
        system("pause");
        system("cls");
        return;
    }
    Producto p;
    cout << "\n--- Productos en categoria '" << cat << "' ---\n";
    bool encontrado = false;
    while (file.read((char*)&p, sizeof(Producto))) {
        if (p.activo && strcmp(p.categoria, cat) == 0) {
            cout << "Codigo: " << p.codigo << ", Nombre: " << p.nombre
                << ", Precio: " << p.precio << ", Stock: " << p.stock << endl;
            encontrado = true;
        }
    }
    if (!encontrado) cout << "No hay productos en esa categoria.\n";
    file.close();
    system("pause");
    system("cls");
}

void buscarPorCodigo() {
    int codigo;
    cout << "Ingrese codigo: ";
    cin >> codigo;
    limpiarBuffer();

    int pos = buscarPosPorCodigo(codigo);
    if (pos == -1) {
        cout << "No existe ese producto.\n";
        system("pause");
        system("cls");
        return;
    }

    ifstream file(FILENAME, ios::binary);
    if (!file) {
        cout << "No hay productos.\n";
        system("pause");
        system("cls");
        return;
    }

    Producto p;
    file.seekg(pos * sizeof(Producto), ios::beg);
    file.read((char*)&p, sizeof(Producto));
    file.close();

    if (p.activo) {
        cout << "Codigo: " << p.codigo << ", Nombre: " << p.nombre
            << ", Precio: " << p.precio << ", Stock: " << p.stock
            << ", Categoria: " << p.categoria << endl;
    } else {
        cout << "El producto esta eliminado.\n";
    }
    system("pause");
    system("cls");
}

void modificarProducto() {
    int codigo;
    cout << "Ingrese codigo a modificar: ";
    cin >> codigo;
    limpiarBuffer();

    int pos = buscarPosPorCodigo(codigo);
    if (pos == -1) {
        cout << "Producto no encontrado.\n";
        return;
    }

    fstream file(FILENAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "No hay productos.\n";
        return;
    }

    Producto p;
    file.seekg(pos * sizeof(Producto), ios::beg);
    file.read((char*)&p, sizeof(Producto));

    if (!p.activo) {
        cout << "El producto esta eliminado.\n";
        file.close();
        return;
    }

    cout << "Codigo: " << p.codigo << ", Nombre: " << p.nombre
        << ", Precio: " << p.precio << ", Stock: " << p.stock
        << ", Categoria: " << p.categoria << endl;

    string input;

    cout << "Ingrese nuevos datos (deje en blanco para no modificar):\n";
    cout << "Nuevo nombre (actual: " << p.nombre << "): ";
    getline(cin, input);
    if (!input.empty()) {
        strncpy(p.nombre, input.c_str(), 30);
        p.nombre[30] = '\0';
        limpiarBuffer();
    }
    cout << "Nuevo precio: ";
    getline(cin, input);
    if (!input.empty()) p.precio = stof(input);
    cout << "Nuevo stock: ";
    getline(cin, input);
    if (!input.empty()) p.stock = stoi(input);
    cout << "Nueva categoria: ";
    getline(cin, input);
    if (!input.empty()) {
        strncpy(p.categoria, input.c_str(), 20);
        p.categoria[20] = '\0';
        limpiarBuffer();
    }

    file.seekp(pos * sizeof(Producto), ios::beg);
    file.write((char*)&p, sizeof(Producto));
    file.close();

    cout << "Producto modificado.\n";
    system("pause");
    system("cls");
}

void eliminarProducto() {
    int codigo;
    cout << "Ingrese codigo a eliminar: ";
    cin >> codigo;
    limpiarBuffer();

    int pos = buscarPosPorCodigo(codigo);
    if (pos == -1) {
        cout << "Producto no encontrado.\n";
        system("pause");
        system("cls");
        return;
    }

    fstream file(FILENAME, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "No hay productos.\n";
        system("pause");
        system("cls");
        return;
    }

    Producto p;
    file.seekg(pos * sizeof(Producto), ios::beg);
    file.read((char*)&p, sizeof(Producto));

    if (!p.activo) {
        cout << "El producto ya esta eliminado.\n";
        file.close();
        system("pause");
        system("cls");
        return;
    }

    p.activo = false;
    file.seekp(pos * sizeof(Producto), ios::beg);
    file.write((char*)&p, sizeof(Producto));
    file.close();

    cout << "Producto eliminado.\n";
    system("pause");
    system("cls");
}