#pragma once
#include <iostream>
#include <vector>
#include "Tarjeta.h"
#include <string> 

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
        : id(id), nombre(nombre), apellido(apellido), dni(dni), edad(edad), password(password) {}

    Usuario() : id(""), nombre(""), apellido(""), dni(0), edad(0), password("") {}


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

    // === Getters ===
    string getId() const { return id; }
    string getNombre() const { return nombre; }
    string getApellido() const { return apellido; }
    int getDni() const { return dni; }
    int getEdad() const { return edad; }
    string getPassword() const { return password; }
    const vector<Tarjeta*>& getTarjetas() const { return tarjetas; }

    // === Setters ===
    void setId(const string& id) { this->id = id; }
    void setNombre(const string& nombre) { this->nombre = nombre; }
    void setApellido(const string& apellido) { this->apellido = apellido; }
    void setDni(int dni) { this->dni = dni; }
    void setEdad(int edad) { this->edad = edad; }
    void setPassword(const string& password) { this->password = password; }
};