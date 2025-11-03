#pragma once
#include <iostream>
#include <vector>
#include <string> 
#include <fstream>
#include <iomanip>
#include "Pila.h"
#include "Tarjeta.h"
#include "Movimiento.h"
#include "Utilidades.h"

using namespace std;

typedef Movimiento<string, string, string, double> P_Movimiento;
class Usuario {
private:
    string id;
    string nombre, apellido;
    int dni;
    int edad;
    string password;
    vector<Tarjeta*> tarjetas;
    Pila<P_Movimiento> historial;

    string rutaHistorial() const {
        return "movimientos/" + nombre + "_mov_" + id + ".txt";
    }


public:
    Usuario(string id, string nombre, string apellido, int dni, int edad, string password)
        : id(id), nombre(nombre), apellido(apellido), dni(dni), edad(edad), password(password) {}

    Usuario() : id(""), nombre(""), apellido(""), dni(0), edad(0), password("") {}

   // ===================================================
   // OPERACIONES BANCARIAS
   // ===================================================
    bool retirarDinero(double monto) {
        Tarjeta* t = obtenerTarjetaPrincipal();
        if (!t || !t->getCuentaAsociada()) {
            mensajeError("El usuario no tiene una cuenta asociada para retirar.");
            return false;
        }

        if (t->getCuentaAsociada()->retirar(monto)) {
            registrarMovimiento("RETIRO", monto);
            return true;
        }
        return false;
    }

    bool depositarDinero(double monto) {
        Tarjeta* t = obtenerTarjetaPrincipal();
        if (!t || !t->getCuentaAsociada()) {
            mensajeError("El usuario no tiene una cuenta asociada para depositar.");
            return false;
        }

        t->getCuentaAsociada()->depositar(monto);
        registrarMovimiento("DEPOSITO", monto);
        return true;
    }

    bool transferirDinero(Usuario* destino, double monto) {
        Tarjeta* tOrigen = obtenerTarjetaPrincipal();
        Tarjeta* tDestino = destino ? destino->obtenerTarjetaPrincipal() : nullptr;

        if (!tOrigen || !tOrigen->getCuentaAsociada()) {
            mensajeError("El usuario origen no tiene cuenta asociada.");
            return false;
        }
        if (!tDestino || !tDestino->getCuentaAsociada()) {
            mensajeError("El usuario destino no tiene cuenta asociada.");
            return false;
        }

        if (!tOrigen->getCuentaAsociada()->retirar(monto)) {
            mensajeError("Saldo insuficiente para realizar la transferencia.");
            return false;
        }

        tDestino->getCuentaAsociada()->depositar(monto);
        registrarMovimiento("TRANSFERENCIA A " + destino->getId(), monto);
        destino->registrarMovimiento("TRANSFERENCIA DE " + id, monto);
        return true;
    }

    // ===================================================
    // TARJETAS
    // ===================================================
    Tarjeta* obtenerTarjetaPrincipal() {
        if (tarjetas.empty()) return nullptr;
        return tarjetas[0];
    }

    Tarjeta* buscarTarjetaPorNumero(const string& num) {
        for (auto t : tarjetas)
            if (t->getNumero() == num)
                return t;
        return nullptr;
    }

    double obtenerSaldoTotal() const {
        double total = 0.0;
        for (auto t : tarjetas)
            if (t->getCuentaAsociada())
                total += t->getCuentaAsociada()->getSaldo();
        return total;
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

    // ===================================================
    // MOSTRAR INFO
    // ===================================================
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

    // ===================================================
    // HISTORIAL DE MOVIMIENTOS
    // ===================================================
    void registrarMovimiento(string tipo, double monto) {
        P_Movimiento mov(tipo, monto);
        historial.push(mov);

        ofstream ofs(rutaHistorial(), ios::app);
        if (ofs) {
            //string linea = mov.aLinea();
            ofs << mov.aLinea() << "\n";
        }

        cout << "[Movimiento registrado] " << tipo << " - S/ " << formatoDinero(monto);
        cout << endl;
    }

    void mostrarHistorialDesdeTxt() {
        ifstream ifs(rutaHistorial());
        if (!ifs) {
            cout << "Sin movimientos registrados." << endl;
            return;
        }

        Pila<P_Movimiento> pilaTmp;
        string linea;
        while (getline(ifs, linea)) {
            if (linea.empty()) continue;
            pilaTmp.push(parsearMovimiento(linea));
        }

        cout << "\n--- HISTORIAL DE MOVIMIENTOS DE " << nombre << " " << apellido << " (ID: " << id << ") ---\n";
        if (pilaTmp.vacia()) {
            cout << "Sin movimientos." << endl;
        }
        else {
            while (!pilaTmp.vacia()) {
                pilaTmp.top().imprimir();
                pilaTmp.pop();
            }
        }
        cout << "----------------------------------------------\n";
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