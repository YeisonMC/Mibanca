#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Nodo.h"
#include "Usuario.h"
#include "Cuenta.h"
#include "Tarjeta.h"

using namespace std;

template<class T>
class ListaUsuarios {
private:
    Nodo<T>* inicio;
    vector<Cuenta*> listaDeCuentas;
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
    Nodo<T>* obtenerInicioNodo() const { return inicio; }

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

    Cuenta* buscarCuentaPorId(const string& idCuenta) {
        for (auto cuenta : listaDeCuentas) {
            if (cuenta->getIdCuenta() == idCuenta) {
                return cuenta;
            }
        }
        return nullptr; 
    }

    // ===================================================
    // GUARDAR Y CARGAR USUARIOS (solo datos básicos)
    // ===================================================
    void guardar(const string& archivo = "usuarios.bin") {
        ofstream ofs(archivo, ios::binary | ios::trunc);
        if (!ofs) {
            cerr << "Error al abrir el archivo para guardar usuarios." << endl;
            return;
        }

        Nodo<T>* actual = inicio;
        while (actual != nullptr) {
            escribirUsuario(ofs, actual->dato);
            actual = actual->siguiente;
        }

        ofs.close();
        //cout << "[INFO] Usuarios guardados correctamente en '" << archivo << "'.\n";
    }

    void cargar(const string& archivo = "usuarios.bin") {
        ifstream ifs(archivo, ios::binary);
        if (!ifs) {
            cout << "[INFO] No existe el archivo '" << archivo << "', se creará al guardar.\n";
            return;
        }

        while (true) {
            T u;
            if (!leerUsuario(ifs, u)) break;
            agregarFinal(u);
        }

        ifs.close();
        cout << "[INFO] Usuarios cargados desde '" << archivo << "'.\n";
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

    // ===================================================
// OPERACIONES BANCARIAS GLOBALES
// ===================================================
    bool retirarDeUsuario(const string& idUsuario, double monto) {
        if (monto <= 0) return false;
        T* u = buscarPorId(idUsuario);
        if (!u) {
            mensajeError("Usuario no encontrado.");
            return false;
        }

        if (u->retirarDinero(monto)) {
            mensajeExito("Retiro realizado correctamente.");
            return true;
        }

        mensajeError("No se pudo realizar el retiro (saldo insuficiente o sin cuenta).");
        return false;
    }

    bool depositarAUsuario(const string& idUsuario, double monto) {
        if (monto <= 0) return false;
        T* u = buscarPorId(idUsuario);
        if (!u) {
            mensajeError("Usuario no encontrado.");
            return false;
        }

        if (u->depositarDinero(monto)) {
            mensajeExito("Deposito realizado correctamente.");
            return true;
        }

        mensajeError("No se pudo realizar el deposito (sin cuenta asociada).");
        return false;
    }

    bool transferirEntreUsuarios(const string& idOrigen, const string& idDestino, double monto) {
        if (monto <= 0 || idOrigen == idDestino) {
            mensajeError("Operacion invalida.");
            return false;
        }

        T* origen = buscarPorId(idOrigen);
        T* destino = buscarPorId(idDestino);

        if (!origen || !destino) {
            mensajeError("Uno de los usuarios no existe.");
            return false;
        }

        if (origen->transferirDinero(destino, monto)) {
            mensajeExito("Transferencia realizada correctamente.");
            return true;
        }

        mensajeError("No se pudo completar la transferencia.");
        return false;
    }

    // ====== LIMPIAR ======
    void limpiar() {
        //while (!vacia()) eliminarInicio();
        if (vacia()) return;

        Nodo<T>* actual = inicio;
        while (actual != nullptr) {
            Nodo<T>* temp = actual;
            actual = actual->siguiente;
            delete temp; 
        }
        inicio = nullptr;
        tamano = 0;
    }

private:
    // ====== FUNCIONES PRIVADAS DE ARCHIVO ======
    void escribirUsuario(ofstream& ofs, T& u) {
        guardarString(ofs, u.getId());
        guardarString(ofs, u.getNombre());
        guardarString(ofs, u.getApellido());
        int dni = u.getDni();
        ofs.write((char*)&dni, sizeof(int));
        int edad = u.getEdad();
        ofs.write((char*)&edad, sizeof(int));
        guardarString(ofs, u.getPassword());
    }


    bool leerUsuario(ifstream& ifs, T& u) {
        string id = leerString(ifs);
        if (id.empty()) return false;  

        string nombre = leerString(ifs);
        string apellido = leerString(ifs);

        int dni = 0;
        if (!ifs.read((char*)&dni, sizeof(int))) return false;

        int edad = 0;
        if (!ifs.read((char*)&edad, sizeof(int))) return false;

        string password = leerString(ifs);

        // Crear el usuario con los datos basicos
        u = T(id, nombre, apellido, dni, edad, password);
        return true;
    }

    // ===================================================
    // FUNCIONES DE SOPORTE PARA STRINGS BINARIOS
    // ===================================================
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
