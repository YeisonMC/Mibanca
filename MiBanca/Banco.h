#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <Windows.h>

#include "Usuario.h"
#include "ListaUsuarios.h"
#include "InicioSesion.h"
#include "RegistroUsuario.h"
#include "GestorTarjetas.h"
#include "GestorCuentas.h"
#include "Utilidades.h"
#include "Cola.h"

using namespace std;
using namespace System;

void setColor(int color); 

template<typename T>
class Banco {
private:
    ListaUsuarios<T>* usuarios;
    GestorTarjetas<T>* gestorTarjetas;
    GestorCuentas<T>* gestorCuentas;
    InicioSesion<T>* sesion;
    T* usuarioActual;
    bool sesionActiva;

    struct Servicio {
        string nombre;
        double monto;
    };

public:
    Banco() {
        usuarios = new ListaUsuarios<T>();
        gestorTarjetas = new GestorTarjetas<T>();
        gestorCuentas = new GestorCuentas<T>();
        sesion = new InicioSesion<T>();
        usuarioActual = nullptr;
        sesionActiva = false;
    }

    ~Banco() {
        if (usuarios) {
            usuarios->guardar();
            gestorTarjetas->guardarTarjetas(*usuarios);
            gestorCuentas->guardarCuentas(*usuarios);
            delete usuarios;
        }
        if (gestorTarjetas) delete gestorTarjetas;
        if (gestorCuentas) delete gestorCuentas;
        if (sesion) delete sesion;
    }

    // ============================
    // MÉTODO PRINCIPAL
    // ============================
    void ejecutar() {
        inicializarSistema();
        menuInicio();
        cerrarSistema();
    }

private:
    // ============================
    // ETAPA DE INICIO
    // ============================
    void inicializarSistema() {
        srand((unsigned)time(nullptr));
        usuarios->cargar();
        gestorTarjetas->cargarTarjetas(*usuarios);
        gestorCuentas->cargarCuentas(*usuarios);
        Console::Clear();
        tituloConsola("SISTEMA BANCARIO DIGITAL");
    }

    bool iniciarSesion() {
        return sesion->iniciar(*usuarios);
    }

    // ======================================================
    // MENU INICIAL (ANTES DE SESION)
    // ======================================================
    void menuInicio() {
        bool continuar = true;
        while (continuar) {
            Console::Clear();
            tituloConsola("BANCO DIGITAL - BIENVENIDO");
            cout << "1. Iniciar sesion\n";
            cout << "2. Registrar nuevo usuario\n";
            cout << "3. Salir\n";
            cout << string(60, '=') << endl;

            int opcion;
            cout << "Seleccione una opcion: ";
            if (!(cin >> opcion)) {
                limpiarBuffer();
                mensajeError("Opción invalida.");
                pausar();
                continue;
            }
            limpiarBuffer();

            switch (opcion) {
            case 1:
                if (iniciarSesion()) {
                    usuarioActual = sesion->getUsuarioActual();
                    sesionActiva = true;
                    menuPrincipal(); 
                }
                break;

            case 2: {
                auto nuevoUsuario = RegistroUsuario<T>::registrarNuevo(*usuarios);
                if (nuevoUsuario) {
                    usuarioActual = nuevoUsuario;
                    sesionActiva = true;
                    menuPrincipal(); // entra directo al menú tras registrarse
                }
                break;
            }

            case 3:
                continuar = false;
                break;

            default:
                mensajeError("Opcion no valida.");
                pausar();
                break;
            }
        }
    }

    // ============================
    // MENÚ PRINCIPAL
    // ============================
    void menuPrincipal() {
        bool continuar = true;
        while (continuar && sesionActiva) {
            mostrarEncabezado();
            mostrarOpcionesMenu();

            int opcion;
            cout << "\nSeleccione una opcion: ";
            if (!(cin >> opcion)) {
                limpiarBuffer();
                mensajeError("Opcion invalida.");
                pausar();
                continue;
            }
            limpiarBuffer();

            continuar = procesarOpcion(opcion);
        }
    }

    void mostrarEncabezado() {
        Console::Clear();
        if (usuarioActual != nullptr) {
            tituloConsola("MI PERFIL");
            cout << "Usuario: " << usuarioActual->getNombre() << " "
                << usuarioActual->getApellido() << endl;
            cout << "DNI: " << usuarioActual->getDni() << endl;
            cout << "Edad: " << usuarioActual->getEdad() << endl;
            cout << "Tarjetas registradas: " << usuarioActual->getTarjetas().size() << endl;
            cout << string(60, '=') << endl;
        }
    }

    void mostrarOpcionesMenu() {
        cout << "1. Retirar dinero\n";
        cout << "2. Transferir fondos\n";
        cout << "3. Ver historial de operaciones\n";
        cout << "4. Pago de servicios\n";
        cout << "5. Solicitar prestamo\n";
        cout << "6. Cerrar sesion\n";
        cout << string(60, '=') << endl;
    }

    // ============================
    // PROCESAR OPCIONES DEL MENÚ
    // ============================
    bool procesarOpcion(int opcion) {
        switch (opcion) {
        case 1:
            operacionRetirar();
            usuarios->guardar();
            return true;
        case 2:
            operacionTransferir();
            usuarios->guardar();
            return true;
        case 3:
            operacionVerHistorial();
            return true;
        case 4:
            operacionPagoServicios();
            usuarios->guardar();
            return true;
        case 5:
            operacionSolicitarPrestamo();
            return true;
        case 6:
            return false;
        default:
            mensajeError("Opcion no valida.");
            pausar();
            return true;
        }
    }

    // ============================
    // OPERACIONES
    // ============================

    // --- Retiro simple (versión base) ---
    void operacionRetirar() {
        Console::Clear();
        tituloConsola("RETIRO DE DINERO");

        double monto;
        cout << "Ingrese el monto a retirar: S/ ";
        if (!(cin >> monto) || monto <= 0) {
            mensajeError("Monto invalido.");
            limpiarBuffer();
            pausar();
            return;
        }

        // TODO: identificar cuenta asociada principal
        if (usuarios->retirarDeUsuario(usuarioActual->getId(), monto)) {
            cout << "Nuevo saldo total: S/ " << formatoDinero(usuarioActual->obtenerSaldoTotal()) << endl;
        }
        pausar();
    }

    // --- Transferencia entre usuarios ---
    void operacionTransferir() {
        Console::Clear();
        tituloConsola("TRANSFERENCIA ENTRE USUARIOS");

        string idDestino;
        double monto;

        cout << "Ingrese ID del usuario destinatario: ";
        cin >> idDestino;

        T* destinatario = usuarios->buscarPorId(idDestino);
        if (!destinatario) {
            mensajeError("Usuario destino no encontrado.");
            pausar();
            return;
        }

        cout << "Ingrese monto a transferir: S/ ";
        if (!(cin >> monto) || monto <= 0) {
            mensajeError("Monto invalido.");
            limpiarBuffer();
            pausar();
            return;
        }

        usuarios->transferirEntreUsuarios(usuarioActual->getId(), idDestino, monto);
        pausar();
    }

    // --- Historial ---
    void operacionVerHistorial() {
        Console::Clear();
        tituloConsola("HISTORIAL DE OPERACIONES");
        usuarioActual->mostrarHistorialDesdeTxt();
        pausar();
    }

    // --- Pago de servicios ---
    void operacionPagoServicios() {
        Console::Clear();
        tituloConsola("PAGO DE SERVICIOS");

        vector<string> catalogo = {
            "Luz", "Agua", "Internet", "Telefono", "Gas",
            "Netflix", "Spotify", "BBVA_Seguros", "HBO_Max", "Amazon_Prime"
        };

        int cantidad;
        cout << "Cuantos pagos pendientes desea generar? (1-5): ";
        if (!(cin >> cantidad) || cantidad < 1 || cantidad > 5) {
            mensajeError("Cantidad invalida.");
            limpiarBuffer();
            pausar();
            return;
        }
        limpiarBuffer();

        Cola<Servicio> cola;
        generarPagosPendientes(cola, catalogo, cantidad);
        procesarPagos(cola);
    }

    void generarPagosPendientes(Cola<Servicio>& cola, vector<string>& catalogo, int cantidad) {
        for (int i = 0; i < cantidad; i++) {
            Servicio s;
            s.nombre = catalogo[rand() % catalogo.size()];
            s.monto = (10 + rand() % 300) * 1.5;
            cola.encolar(s);
        }
    }

    void procesarPagos(Cola<Servicio>& cola) {
        while (!cola.vacia()) {
            Console::Clear();
            mostrarPagosPendientes(cola);

            int opcion;
            if (!(cin >> opcion)) {
                limpiarBuffer();
                continue;
            }
            limpiarBuffer();

            if (opcion == 0) break;

            Servicio* elegido = cola.obtenerPos(opcion - 1);
            if (!elegido) {
                mensajeError("Opcion invalida.");
                pausar();
                continue;
            }

            realizarPago(cola, elegido, opcion - 1);
        }
    }

    void mostrarPagosPendientes(Cola<Servicio>& cola) {
        cout << "\nPagos pendientes:\n";
        for (int i = 0; i < cola.size(); i++) {
            Servicio* s = cola.obtenerPos(i);
            if (s) {
                cout << (i + 1) << ") " << setw(20) << left << s->nombre
                    << " S/ " << formatoDinero(s->monto) << endl;
            }
        }
        cout << "\nSeleccione un pago (1-" << cola.size() << ") o 0 para salir: ";
    }

    void realizarPago(Cola<Servicio>& cola, Servicio* servicio, int indice) {
        cout << "\nProcesando pago: " << servicio->nombre
            << " por S/ " << formatoDinero(servicio->monto) << endl;
        cout << "En proceso... (falta vincular con saldo del usuario)\n";
        cola.eliminarPos(indice);
        pausar("\nPresione una tecla para continuar...");
    }

    // --- Solicitar préstamo ---
    void operacionSolicitarPrestamo() {
        Console::Clear();
        tituloConsola("SOLICITAR PRESTAMO");
        cout << "En proceso... (modulo pendiente de implementacion)\n";
        pausar();
    }

    // --- Cerrar sesión ---
    void cerrarSistema() {
        Console::Clear();
        tituloConsola("CERRANDO SESION");
        usuarios->guardar();
        gestorCuentas->guardarCuentas(*usuarios); 
        gestorTarjetas->guardarTarjetas(*usuarios); 
     /*   sesion->cerrarSesion();
        sesionActiva = false;*/
        if (sesionActiva && usuarioActual != nullptr) {
            sesion->cerrarSesion();
            usuarioActual = nullptr;
        }

        sesionActiva = false;
        _sleep(800);
        mensajeInfo("Gracias por usar el Banco Digital.");
    }
};
