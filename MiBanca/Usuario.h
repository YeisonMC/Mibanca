#pragma once
#include <iostream>
#include <vector>
#include "Tarjeta.h"

using namespace std;

class Usuario {
private:
    string id;
    string nombre, apellido;
    int dni;
    int edad;
    string password;
    vector<Tarjeta*> tarjetas;

public:
    Usuario(string id, string nombre, string apellido, int dni, int edad, string password)
        : id(id), nombre(nombre), dni(dni), password(password) {
    }

    void agregarTarjeta(Tarjeta* t) {
        if (t != nullptr) {
            tarjetas.push_back(t);
            cout << "Tarjeta agregada correctamente al usuario " << nombre << " " << apellido << ".\n";
        }
        else {
            cout << "Error: Tarjeta invalida.\n";
        }
    }

    void mostrarTarjetas() const {
        if (tarjetas.empty()) {
            cout << "El usuario no tiene tarjetas registradas.\n";
            return;
        }

        cout << "\n=== Tarjetas de " << nombre << " " << apellido << " ===\n";
        for (auto t : tarjetas) {
            t->mostrarInfo();
        }
    }

    void mostrarInfo() const {
        cout << "==============================\n";
        cout << "ID Usuario: " << id << endl;
        cout << "Nombre: " << nombre << " " << apellido << endl;
        cout << "DNI: " << dni << endl;
        cout << "Edad: " << edad << endl;
        cout << "Tarjetas registradas: " << tarjetas.size() << endl;
        cout << "==============================\n";
    }
};