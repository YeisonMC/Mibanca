#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Tarjeta.h"
#include "ListaUsuarios.h"

using namespace std;

template<typename T>
class GestorTarjetas {
private:
    string archivoTarjetas;

public:
    GestorTarjetas(const string& nombreArchivo = "tarjetas.bin")
        : archivoTarjetas(nombreArchivo) {
    }

    // ===================================================
    // GUARDAR TODAS LAS TARJETAS DE TODOS LOS USUARIOS
    // ===================================================
    void guardarTarjetas(const ListaUsuarios<T>& listaUsuarios) {
        ofstream ofs(archivoTarjetas, ios::binary | ios::trunc);
        if (!ofs) {
            cerr << "[ERROR] No se pudo abrir " << archivoTarjetas << " para guardar." << endl;
            return;
        }

        // Recorremos todos los usuarios
        int cantidadUsuarios = listaUsuarios.size();
        ofs.write((char*)&cantidadUsuarios, sizeof(int));

        Nodo<T>* actual = listaUsuarios.obtenerInicioNodo();
        while (actual != nullptr) {
            const T& usuario = actual->dato;
            const vector<Tarjeta*>& tarjetas = usuario.getTarjetas();

            guardarString(ofs, usuario.getId());

            // Cantidad de tarjetas del usuario
            int n = tarjetas.size();
            ofs.write((char*)&n, sizeof(int));

            // Guardamos cada tarjeta
            for (auto t : tarjetas) {
                guardarString(ofs, t->getNumero());
                int tipo = (int)t->getTipo();
                ofs.write((char*)&tipo, sizeof(int));
                int cvc = t->getCvc();
                ofs.write((char*)&cvc, sizeof(int));
                guardarString(ofs, t->getTitular());

                string idCuenta = (t->getCuentaAsociada() ? t->getCuentaAsociada()->getIdCuenta() : "SIN_CUENTA");
                guardarString(ofs, idCuenta);
            }

            actual = actual->siguiente;
        }

        ofs.close();
        cout << "[INFO] Tarjetas guardadas correctamente en '" << archivoTarjetas << "'.\n";
    }

    // ===================================================
    // CARGAR TARJETAS Y VINCULARLAS CON USUARIOS
    // ===================================================
    void cargarTarjetas(ListaUsuarios<T>& listaUsuarios) {
        ifstream ifs(archivoTarjetas, ios::binary);
        if (!ifs) {
            cout << "[INFO] No existe el archivo '" << archivoTarjetas << "', se creará al guardar.\n";
            return;
        }

        int cantidadUsuarios = 0;
        if (!ifs.read((char*)&cantidadUsuarios, sizeof(int))) return;

        for (int i = 0; i < cantidadUsuarios; i++) {
            string idUsuario = leerString(ifs);
            int nTarjetas = 0;
            ifs.read((char*)&nTarjetas, sizeof(int));

            T* usuario = listaUsuarios.buscarPorId(idUsuario);
            if (!usuario) {
                // Si el usuario no existe, saltamos sus tarjetas
                for (int j = 0; j < nTarjetas; j++) {
                    ignorarTarjeta(ifs);
                }
                continue;
            }

            for (int j = 0; j < nTarjetas; j++) {
                string numero = leerString(ifs);
                int tipoInt = 0; ifs.read((char*)&tipoInt, sizeof(int));
                int cvc = 0; ifs.read((char*)&cvc, sizeof(int));
                string titular = leerString(ifs);
                string idCuenta = leerString(ifs);

                // Por ahora, la cuenta asociada será nullptr (a futuro se vincula con GestorCuentas)
                Tarjeta* nuevaTarjeta = new Tarjeta(numero, (TipoTarjeta)tipoInt, cvc, titular, nullptr);
                usuario->agregarTarjeta(nuevaTarjeta);
            }
        }

        ifs.close();
        cout << "[INFO] Tarjetas cargadas correctamente desde '" << archivoTarjetas << "'.\n";
    }

private:
    // ===================================================
    // FUNCIONES AUXILIARES BINARIAS
    // ===================================================
    void guardarString(ofstream& ofs, const string& s) {
        int len = (int)s.size();
        ofs.write((char*)&len, sizeof(int));
        ofs.write(s.c_str(), len);
    }

    string leerString(ifstream& ifs) {
        int len = 0;
        if (!ifs.read((char*)&len, sizeof(int))) return string();
        if (len <= 0) return string();
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

    void ignorarTarjeta(ifstream& ifs) {
        // Salta los campos de una tarjeta en el archivo
        leerString(ifs); // numero
        int dummyInt;
        ifs.read((char*)&dummyInt, sizeof(int)); // tipo
        ifs.read((char*)&dummyInt, sizeof(int)); // cvc
        leerString(ifs); // titular
        leerString(ifs); // idCuenta
    }
};
