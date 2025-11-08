#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Cuenta.h"
#include "ListaUsuarios.h"

using namespace std;

template<typename T>
class GestorCuentas {
private:
    string archivoCuentas;

public:
    GestorCuentas(const string& nombreArchivo = "cuentas.bin")
        : archivoCuentas(nombreArchivo) {
    }

    // ===================================================
    // GUARDAR TODAS LAS CUENTAS DE TODAS LAS TARJETAS
    // ===================================================
    void guardarCuentas(const ListaUsuarios<T>& listaUsuarios) {
        ofstream ofs(archivoCuentas, ios::binary | ios::trunc);
        if (!ofs) {
            cerr << "[ERROR] No se pudo abrir " << archivoCuentas << " para guardar." << endl;
            return;
        }

        Nodo<T>* actual = listaUsuarios.obtenerInicioNodo();
        vector<Cuenta*> cuentasGuardadas;

        while (actual != nullptr) {
            const vector<Tarjeta*>& tarjetas = actual->dato.getTarjetas();
            for (auto t : tarjetas) {
                Cuenta* c = t->getCuentaAsociada();
                if (c == nullptr) continue;

                // Evitar guardar cuentas repetidas
                if (find(cuentasGuardadas.begin(), cuentasGuardadas.end(), c) != cuentasGuardadas.end())
                    continue;

                cuentasGuardadas.push_back(c);
                guardarCuenta(ofs, *c);
            }
            actual = actual->siguiente;
        }

        ofs.close();
        cout << "[INFO] Cuentas guardadas correctamente en '" << archivoCuentas << "'.\n";
    }

    // ===================================================
    // CARGAR CUENTAS Y VINCULARLAS CON TARJETAS
    // ===================================================
    void cargarCuentas(ListaUsuarios<T>& listaUsuarios) {
        ifstream ifs(archivoCuentas, ios::binary);
        if (!ifs) {
            cout << "[INFO] No existe el archivo '" << archivoCuentas << "', se creará al guardar.\n";
            return;
        }

        vector<Cuenta*> cuentasCargadas;

        while (true) {
            Cuenta* cuenta = leerCuenta(ifs);
            if (!cuenta) break;
            cuentasCargadas.push_back(cuenta);
        }

        // Vincular las cuentas con sus tarjetas
        Nodo<T>* actual = listaUsuarios.obtenerInicioNodo();
        while (actual != nullptr) {
            vector<Tarjeta*>& tarjetas = const_cast<vector<Tarjeta*>&>(actual->dato.getTarjetas());
            for (auto t : tarjetas) {
                for (auto c : cuentasCargadas) {
                    /*if (c->getIdCuenta() == t->getCuentaAsociada()->getIdCuenta()) {
                        t->setCuentaAsociada(c);
                    }*/
                  /*  if (t && c) {
                        Cuenta* cuentaAsociada = t->getCuentaAsociada();
                        if (cuentaAsociada && cuentaAsociada->getIdCuenta() == c->getIdCuenta()) {
                            t->setCuentaAsociada(c);
                        }
                    }*/
                }
            }
            actual = actual->siguiente;
        }

        ifs.close();
        cout << "[INFO] Cuentas cargadas correctamente desde '" << archivoCuentas << "'.\n";
    }

private:
    // ===================================================
    // FUNCIONES DE SERIALIZACIÓN
    // ===================================================
    void guardarCuenta(ofstream& ofs, const Cuenta& c) {
        string id = c.getIdCuenta();
        double saldo = c.getSaldo();

        guardarString(ofs, id);
        ofs.write((char*)&saldo, sizeof(double));

        // Detectar tipo de cuenta dinamicamente
        const CuentaAhorro* ahorro = dynamic_cast<const CuentaAhorro*>(&c);
        const CuentaCredito* credito = dynamic_cast<const CuentaCredito*>(&c);

        if (ahorro) {
            int tipo = 1;
            ofs.write((char*)&tipo, sizeof(int));
            double tasa = ahorro->getTasaInteres();
            ofs.write((char*)&tasa, sizeof(double));
        }
        else if (credito) {
            int tipo = 2;
            ofs.write((char*)&tipo, sizeof(int));
            double limite = credito->getLimiteCredito();
            ofs.write((char*)&limite, sizeof(double));
            double deuda = credito->getDeudaActual();
            ofs.write((char*)&deuda, sizeof(double));
        }
        else {
            int tipo = 0;
            ofs.write((char*)&tipo, sizeof(int));
        }
    }

    Cuenta* leerCuenta(ifstream& ifs) {
        string id = leerString(ifs);
        if (id.empty()) return nullptr;

        double saldo = 0.0;
        if (!ifs.read((char*)&saldo, sizeof(double))) return nullptr;

        int tipo = 0;
        if (!ifs.read((char*)&tipo, sizeof(int))) return nullptr;

        if (tipo == 1) {
            double tasa;
            ifs.read((char*)&tasa, sizeof(double));
            return new CuentaAhorro(id, saldo, tasa);
        }
        else if (tipo == 2) {
            double limite, deuda;
            ifs.read((char*)&limite, sizeof(double));
            ifs.read((char*)&deuda, sizeof(double));
            return new CuentaCredito(id, saldo, limite, deuda);
        }
        else {
            return new CuentaAhorro(id, saldo, 0.0); // tipo desconocido -> ahorro basico
        }
    }

    // ===================================================
    // FUNCIONES DE APOYO
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
};
