#pragma once
enum class TipoTarjeta { DEBITO, CREDITO };

#include <string>
#include "Cuenta.h"
using namespace std;

class Tarjeta {
private:
    string numero;
    TipoTarjeta tipo;
    int cvc;
    string titular;
    Cuenta* cuentaAsociada;

public:
    // Constructor
    Tarjeta(string num, TipoTarjeta t, int cvc, string titular, Cuenta* cuenta)
        : numero(num), tipo(t), cvc(cvc), titular(titular), cuentaAsociada(cuenta) {
    }

    // Metodos principales
    bool pagar(double monto) {
        if (!cuentaAsociada) {
            cout << "No hay cuenta asociada a esta tarjeta.\n";
            return false;
        }

        if (tipo == TipoTarjeta::DEBITO) {
            // Las tarjetas de debito retiran directamente del saldo
            return cuentaAsociada->retirar(monto);
        }
        else if (tipo == TipoTarjeta::CREDITO) {
            // Para tarjetas de credito, deberiamos tener una CuentaCredito asociada
            // que maneje la deuda e intereses. Aqui simplemente intentamos retirar.
            return cuentaAsociada->retirar(monto);
        }
        return false;
    }

    void mostrarInfo() const {
        cout << "------------------------------" << endl;
        cout << "Titular: " << titular << endl;
        cout << "Numero Tarjeta: " << numero << endl;
        cout << "Tipo: " << (tipo == TipoTarjeta::DEBITO ? "Debito" : "Credito") << endl;
        cout << "CVC: " << cvc << endl;
        if (cuentaAsociada)
            cout << "Cuenta Asociada: " << cuentaAsociada->getIdCuenta() << endl;
        else
            cout << "Cuenta Asociada: Ninguna" << endl;
        cout << "------------------------------" << endl;
    }

    void recargar(double monto) {
        if (!cuentaAsociada) {
            cout << "No hay cuenta asociada para recargar.\n";
            return;
        }
        if (tipo == TipoTarjeta::CREDITO) {
            cout << "No se puede recargar una tarjeta de credito.\n";
            return;
        }

        cuentaAsociada->depositar(monto);
        cout << "Recarga exitosa de S/. " << monto << " a la cuenta "
            << cuentaAsociada->getIdCuenta() << endl;
    }

    // Getters
    string getNumero() const { return numero; }
    TipoTarjeta getTipo() const { return tipo; }
    int getCvc() const { return cvc; }
    string getTitular() const { return titular; }
    Cuenta* getCuentaAsociada() const { return cuentaAsociada; }
};