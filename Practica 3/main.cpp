#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// clase base: publicacion
class Publication {
protected:
    string title;
    string author;
    int publicationYear;
public:
    Publication(string t, string a, int year) {
        title = t;
        author = a;
        publicationYear = year;
    }

    virtual ~Publication() {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getYear() const { return publicationYear; }

    // muestra la informacion de la publicacion
    virtual void displayInfo() const {
        cout << "Titulo: " << title << endl;
        cout << "Autor: " << author << endl;
        cout << "Anio de publicacion: " << publicationYear << endl;
    }

    // devuelve el tipo de publicacion
    virtual string getType() const {
        return "Publicacion";
    }
};

// clase derivada: libro
class Book : public Publication {
private:
    int pages;
public:
    Book(string t, string a, int year, int p) : Publication(t, a, year) {
        pages = p;
    }

    // muestra la informacion del libro
    void displayInfo() const override {
        Publication::displayInfo();
        cout << "Paginas: " << pages << endl;
    }

    // devuelve el tipo de publicacion
    string getType() const override {
        return "Libro";
    }
};

// clase derivada: revista
class Magazine : public Publication {
private:
    int edition;
public:
    Magazine(string t, string a, int year, int e) : Publication(t, a, year) {
        edition = e;
    }

    // muestra la informacion de la revista
    void displayInfo() const override {
        Publication::displayInfo();
        cout << "Edicion: " << edition << endl;
    }

    // devuelve el tipo de publicacion
    string getType() const override {
        return "Revista";
    }
};

// clase derivada: periodico
class Newspaper : public Publication {
private:
    string date;
    string city;
public:
    Newspaper(string t, string a, int year, string d, string c) : Publication(t, a, year) {
        date = d;
        city = c;
    }

    // muestra la informacion del periodico
    void displayInfo() const override {
        Publication::displayInfo();
        cout << "Fecha: " << date << endl;
        cout << "Ciudad: " << city << endl;
    }

    // devuelve el tipo de publicacion
    string getType() const override {
        return "Periodico";
    }
};

// clase catalogo
class Catalog {
private:
    Publication** publications;
    int capacity;
    int size;

    // redimensiona el arreglo si es necesario
    void resize() {
        int newCapacity = capacity * 2;
        Publication** newArray = new Publication*[newCapacity];
        for (int i = 0; i < size; i++) {
            newArray[i] = publications[i];
        }
        delete[] publications;
        publications = newArray;
        capacity = newCapacity;
    }

public:
    Catalog() : capacity(2), size(0) {
        publications = new Publication*[capacity];
    }

    ~Catalog() {
        for (int i = 0; i < size; i++) {
            delete publications[i];
        }
        delete[] publications;
    }

    // agrega una publicacion al catalogo
    void addPublication(Publication* pub) {
        if (size >= capacity) {
            resize();
        }
        publications[size] = pub;
        size++;
    }

    // muestra todas las publicaciones
    void showPublications() const {
        if (size == 0) {
            cout << "No hay publicaciones en el catalogo." << endl;
            return;
        }
        cout << "\n=== CATALOGO COMPLETO ===" << endl;
        for (int i = 0; i < size; i++) {
            cout << "\n[" << (i + 1) << "] ";
            publications[i]->displayInfo();
            cout << string(40, '-') << endl;
        }
    }

    // busca publicaciones por titulo
    void searchByTitle(const string& titleSearch) const {
        string search = titleSearch;
        transform(search.begin(), search.end(), search.begin(), ::tolower);
        bool found = false;
        cout << "\n=== RESULTADOS DE BUSQUEDA ===" << endl;
        for (int i = 0; i < size; i++) {
            string pubTitle = publications[i]->getTitle();
            string pubTitleLower = pubTitle;
            transform(pubTitleLower.begin(), pubTitleLower.end(), pubTitleLower.begin(), ::tolower);
            if (pubTitleLower.find(search) != string::npos) {
                cout << "\n[" << (i + 1) << "] ";
                publications[i]->displayInfo();
                cout << string(40, '-') << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No se encontraron publicaciones con ese titulo." << endl;
        }
    }

    // elimina una publicacion por indice
    bool deletePublication(int index) {
        if (index < 1 || index > size) {
            return false;
        }
        int pos = index - 1;
        delete publications[pos];
        for (int i = pos; i < size - 1; i++) {
            publications[i] = publications[i + 1];
        }
        size--;
        return true;
    }

    // muestra estadisticas del catalogo
    void showStats() const {
        if (size == 0) {
            cout << "No hay publicaciones para mostrar estadisticas." << endl;
            return;
        }
        int books = 0, magazines = 0, newspapers = 0;
        int oldest = 3000, newest = 0;
        for (int i = 0; i < size; i++) {
            string type = publications[i]->getType();
            if (type == "Libro") books++;
            else if (type == "Revista") magazines++;
            else if (type == "Periodico") newspapers++;
            int year = publications[i]->getYear();
            if (year < oldest) oldest = year;
            if (year > newest) newest = year;
        }
        cout << "\n=== ESTADISTICAS DEL CATALOGO ===" << endl;
        cout << "Total de publicaciones: " << size << endl;
        cout << "Libros: " << books << endl;
        cout << "Revistas: " << magazines << endl;
        cout << "Periodicos: " << newspapers << endl;
        cout << "Anio mas antiguo: " << oldest << endl;
        cout << "Anio mas reciente: " << newest << endl;
    }

    int getSize() const { return size; }
};

// validacion de anio
bool validateYear(int year) {
    return year >= 1500 && year <= 2025;
}
// validacion de paginas
bool validatePages(int pages) {
    return pages > 0;
}
// validacion de edicion
bool validateEdition(int edition) {
    return edition > 0;
}
// validacion de texto
bool validateText(const string& text) {
    return !text.empty() && text.find_first_not_of(" \t\n") != string::npos;
}

// muestra el menu principal
void showMainMenu() {
    cout << "\n" << string(50, '=') << endl;
    cout << "    SISTEMA DE GESTION DE BIBLIOTECA" << endl;
    cout << string(50, '=') << endl;
    cout << "1. Agregar Publicacion" << endl;
    cout << "2. Mostrar Publicaciones" << endl;
    cout << "3. Buscar por Titulo" << endl;
    cout << "4. Eliminar Publicacion" << endl;
    cout << "5. Estadisticas" << endl;
    cout << "6. Salir" << endl;
    cout << string(50, '=') << endl;
    cout << "Seleccione una opcion: ";
}

// muestra el submenu para agregar
void showAddSubmenu() {
    cout << "\n=== AGREGAR PUBLICACION ===" << endl;
    cout << "1. Libro" << endl;
    cout << "2. Revista" << endl;
    cout << "3. Periodico" << endl;
    cout << "4. Volver al menu principal" << endl;
    cout << "Seleccione tipo: ";
}

// crea un libro
Publication* createBook() {
    string title, author;
    int year, pages;
    cout << "\n=== AGREGAR LIBRO ===" << endl;
    do {
        cout << "Titulo: ";
        getline(cin, title);
        if (!validateText(title)) cout << "El titulo no puede estar vacio." << endl;
    } while (!validateText(title));
    do {
        cout << "Autor: ";
        getline(cin, author);
        if (!validateText(author)) cout << "El autor no puede estar vacio." << endl;
    } while (!validateText(author));
    do {
        cout << "Anio de publicacion (1500-2025): ";
        cin >> year;
        if (!validateYear(year)) cout << "Anio invalido. Debe estar entre 1500 y 2025." << endl;
    } while (!validateYear(year));
    do {
        cout << "Numero de paginas: ";
        cin >> pages;
        if (!validatePages(pages)) cout << "El numero de paginas debe ser mayor que 0." << endl;
    } while (!validatePages(pages));
    cin.ignore();
    return new Book(title, author, year, pages);
}

// crea una revista
Publication* createMagazine() {
    string title, author;
    int year, edition;
    cout << "\n=== AGREGAR REVISTA ===" << endl;
    do {
        cout << "Titulo: ";
        getline(cin, title);
        if (!validateText(title)) cout << "El titulo no puede estar vacio." << endl;
    } while (!validateText(title));
    do {
        cout << "Autor: ";
        getline(cin, author);
        if (!validateText(author)) cout << "El autor no puede estar vacio." << endl;
    } while (!validateText(author));
    do {
        cout << "Anio de publicacion (1500-2025): ";
        cin >> year;
        if (!validateYear(year)) cout << "Anio invalido. Debe estar entre 1500 y 2025." << endl;
    } while (!validateYear(year));
    do {
        cout << "Numero de edicion: ";
        cin >> edition;
        if (!validateEdition(edition)) cout << "El numero de edicion debe ser mayor que 0." << endl;
    } while (!validateEdition(edition));
    cin.ignore();
    return new Magazine(title, author, year, edition);
}

// crea un periodico
Publication* createNewspaper() {
    string title, author, date, city;
    int year;
    cout << "\n=== AGREGAR PERIODICO ===" << endl;
    do {
        cout << "Titulo: ";
        getline(cin, title);
        if (!validateText(title)) cout << "El titulo no puede estar vacio." << endl;
    } while (!validateText(title));
    do {
        cout << "Autor: ";
        getline(cin, author);
        if (!validateText(author)) cout << "El autor no puede estar vacio." << endl;
    } while (!validateText(author));
    do {
        cout << "Anio de publicacion (1500-2025): ";
        cin >> year;
        if (!validateYear(year)) cout << "Anio invalido. Debe estar entre 1500 y 2025." << endl;
    } while (!validateYear(year));
    cin.ignore();
    do {
        cout << "Fecha de publicacion: ";
        getline(cin, date);
        if (!validateText(date)) cout << "La fecha no puede estar vacia." << endl;
    } while (!validateText(date));
    do {
        cout << "Ciudad: ";
        getline(cin, city);
        if (!validateText(city)) cout << "La ciudad no puede estar vacia." << endl;
    } while (!validateText(city));
    return new Newspaper(title, author, year, date, city);
}

// funcion principal
int main() {
    Catalog catalog;
    int option;
    cout << "Bienvenido al Sistema de Gestion de Biblioteca!" << endl;
    do {
        showMainMenu();
        cin >> option;
        cin.ignore();
        switch (option) {
            case 1: {
                int type;
                do {
                    showAddSubmenu();
                    cin >> type;
                    cin.ignore();
                    Publication* newPub = nullptr;
                    switch (type) {
                        case 1: newPub = createBook(); break;
                        case 2: newPub = createMagazine(); break;
                        case 3: newPub = createNewspaper(); break;
                        case 4: break;
                        default: cout << "Opcion invalida." << endl; continue;
                    }
                    if (newPub != nullptr) {
                        catalog.addPublication(newPub);
                        cout << "\nPublicacion agregada exitosamente!" << endl;
                        break;
                    }
                } while (type != 4);
                break;
            }
            case 2:
                catalog.showPublications();
                break;
            case 3: {
                string title;
                cout << "Ingrese el titulo a buscar: ";
                getline(cin, title);
                catalog.searchByTitle(title);
                break;
            }
            case 4: {
                if (catalog.getSize() == 0) {
                    cout << "No hay publicaciones para eliminar." << endl;
                    break;
                }
                catalog.showPublications();
                cout << "\nIngrese el numero de la publicacion a eliminar: ";
                int idx;
                cin >> idx;
                char confirm;
                cout << "Esta seguro de eliminar esta publicacion? (s/n): ";
                cin >> confirm;
                if (confirm == 's' || confirm == 'S') {
                    if (catalog.deletePublication(idx)) {
                        cout << "Publicacion eliminada exitosamente." << endl;
                    } else {
                        cout << "Indice invalido." << endl;
                    }
                } else {
                    cout << "Eliminacion cancelada." << endl;
                }
                break;
            }
            case 5:
                catalog.showStats();
                break;
            case 6:
                cout << "Esta seguro de salir? (s/n): ";
                char confirm;
                cin >> confirm;
                if (confirm == 's' || confirm == 'S') {
                    cout << "Cerrando el programa..." << endl;
                    cout << "Gracias por usar el Sistema de Gestion de Biblioteca!" << endl;
                } else {
                    option = 0;
                }
                break;
            default:
                cout << "Opcion invalida. Intente nuevamente." << endl;
        }
    } while (option != 6);

    return 0;
}