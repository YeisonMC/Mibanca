#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Nodo.h"
#include "Usuario.h"

using namespace std;

template<class T>
class ListaUsuarios {
private:
    Nodo<T>* inicio;
    int tamano;

public:
    // ====== CONSTRUCTOR Y DESTRUCTOR ======
    ListaUsuarios() : inicio(nullptr), tamano(0) {}

    ~ListaUsuarios() {
        limpiar();
    }

    // ====== METODOS BÁSICOS ======
    bool vacia() const { return inicio == nullptr; }
    int size() const { return tamano; }

    // === Agregar ===
    void agregarInicio(T nuevo) {
        Nodo<T>* nodo = new Nodo<T>(nuevo);
        nodo->siguiente = inicio;
        inicio = nodo;
        tamano++;
    }

    void agregarFinal(T nuevo) {
        if (vacia()) { agregarInicio(nuevo); return; }
        Nodo<T>* actual = inicio;
        while (actual->siguiente != nullptr)
            actual = actual->siguiente;
        actual->siguiente = new Nodo<T>(nuevo);
        tamano++;
    }

    void agregarPos(T nuevo, int pos) {
        if (pos <= 0) { agregarInicio(nuevo); return; }
        if (pos >= tamano) { agregarFinal(nuevo); return; }

        Nodo<T>* actual = inicio;
        for (int i = 0; i < pos - 1; i++)
            actual = actual->siguiente;

        Nodo<T>* nuevoNodo = new Nodo<T>(nuevo);
        nuevoNodo->siguiente = actual->siguiente;
        actual->siguiente = nuevoNodo;
        tamano++;
    }

    // === Obtener ===
    T* obtenerInicio() {
        if (vacia()) return nullptr;
        return &(inicio->dato);
    }

    T* obtenerFinal() {
        if (vacia()) return nullptr;
        Nodo<T>* actual = inicio;
        while (actual->siguiente != nullptr)
            actual = actual->siguiente;
        return &(actual->dato);
    }

    T* obtenerPos(int pos) {
        if (pos < 0 || pos >= tamano) return nullptr;
        Nodo<T>* actual = inicio;
        for (int i = 0; i < pos; i++)
            actual = actual->siguiente;
        return &(actual->dato);
    }

    // === Eliminar ===
    void eliminarInicio() {
        if (vacia()) return;
        Nodo<T>* temp = inicio;
        inicio = inicio->siguiente;
        delete temp;
        tamano--;
    }

    void eliminarPos(int pos) {
        if (pos < 0 || pos >= tamano) return;
        if (pos == 0) { eliminarInicio(); return; }

        Nodo<T>* actual = inicio;
        for (int i = 0; i < pos - 1; i++)
            actual = actual->siguiente;

        Nodo<T>* temp = actual->siguiente;
        actual->siguiente = temp->siguiente;
        delete temp;
        tamano--;
    }

    // ====== METODOS DE BUSQUEDA ======
    T* buscarPorId(const string& idBuscado) {
        Nodo<T>* actual = inicio;
        while (actual != nullptr) {
            if (actual->dato.getId() == idBuscado)
                return &(actual->dato);
            actual = actual->siguiente;
        }
        return nullptr;
    }

    T* buscarPorDni(int dniBuscado) {
        Nodo<T>* actual = inicio;
        while (actual != nullptr) {
            if (actual->dato.getDni() == dniBuscado)
                return &(actual->dato);
            actual = actual->siguiente;
        }
        return nullptr;
    }

    T* buscarPorNombre(const string& nombreBuscado) {
        Nodo<T>* actual = inicio;
        while (actual != nullptr) {
            if (actual->dato.getNombre() == nombreBuscado)
                return &(actual->dato);
            actual = actual->siguiente;
        }
        return nullptr;
    }

   /* T* buscarPorCuenta(const string& cuentaBuscada) {
        Nodo<T>* actual = inicio;
        while (actual != nullptr) {
            if (actual->dato.getNumeroDeCuenta() == cuentaBuscada)
                return &(actual->dato);
            actual = actual->siguiente;
        }
        return nullptr;
    }*/

    // ====== GUARDAR Y CARGAR EN ARCHIVO ======
    void guardar(const string& archivo = "usuarios.bin") {
        ofstream ofs(archivo, ios::binary | ios::trunc);
        if (!ofs) {
            cerr << "Error al abrir el archivo para guardar." << endl;
            return;
        }

        Nodo<T>* actual = inicio;
        while (actual != nullptr) {
            escribir(ofs, actual->dato);
            actual = actual->siguiente;
        }

        ofs.close();
        cout << "Usuarios guardados correctamente en '" << archivo << "'.\n";
    }

    void cargar(const string& archivo = "usuarios.bin") {
        ifstream ifs(archivo, ios::binary);
        if (!ifs) return;

        while (true) {
            T u;
            if (!leer(ifs, u)) break;
            agregarFinal(u);
        }

        ifs.close();
    }

    // ====== LISTAR USUARIOS ======
    void listarUsuarios() const {
        if (vacia()) {
            cout << "\nNo hay usuarios registrados en el sistema.\n";
            return;
        }

        cout << "\n===== LISTADO DE USUARIOS =====\n";
        Nodo<T>* actual = inicio;
        int indice = 1;

        while (actual != nullptr) {
            cout << "Usuario #" << indice++ << endl;
            actual->dato.mostrarInfo();
            actual = actual->siguiente;
        }

        cout << "===============================\n";
    }

    // ====== LIMPIAR ======
    void limpiar() {
        while (!vacia()) eliminarInicio();
    }

private:
    // ====== FUNCIONES PRIVADAS DE ARCHIVO ======
    void escribir(ofstream& ofs, T& u) {
        guardarString(ofs, u.getId());
        guardarString(ofs, u.getNombre());
        guardarString(ofs, u.getApellido());
        int dni = u.getDni();
        ofs.write((char*)&dni, sizeof(int));
        int edad = u.getEdad();
        ofs.write((char*)&edad, sizeof(int));
        guardarString(ofs, u.getPassword());
        // No se guardan tarjetas (ya que son punteros)
    }

    bool leer(ifstream& ifs, T& u) {
        string id = leerString(ifs);
        if (id.empty()) return false;

        string nombre = leerString(ifs);
        string apellido = leerString(ifs);
        int dni = 0;
        if (!ifs.read((char*)&dni, sizeof(int))) return false;
        int edad = 0;
        if (!ifs.read((char*)&edad, sizeof(int))) return false;
        string password = leerString(ifs);

        // Creamos un nuevo usuario con los datos leídos
        u = T(id, nombre, apellido, dni, edad, password);
        return true;
    }

    void guardarString(ofstream& ofs, const string& s) {
        int len = (int)s.size();
        ofs.write((char*)&len, sizeof(int));
        ofs.write(s.c_str(), len);
    }

    string leerString(ifstream& ifs) {
        int len = 0;
        if (!ifs.read((char*)&len, sizeof(int))) return string();
        if (len < 0) return string();
        char* buffer = new char[len + 1];
        if (!ifs.read(buffer, len)) {
            delete[] buffer;
            return string();
        }
        buffer[len] = '\0';
        string s(buffer);
        delete[] buffer;
        return s;
    }
};
