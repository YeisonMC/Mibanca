#pragma once
#include <iostream>
#include <iomanip>
#include "Utilidades.h"
#include "ListaUsuarios.h"
#include "Cuenta.h"
#include "Tarjeta.h"

using namespace std;
using namespace System;

template<typename T>
class RegistroUsuario {
public:
    static T* registrarNuevo(ListaUsuarios<T>& usuarios) {
        Console::Clear();
        tituloConsola("REGISTRO DE NUEVO USUARIO");

        string nombre, apellido, password, passwordConfirm;
        int dni, edad;

        cout << "Ingrese su nombre: ";
        cin >> nombre;
        limpiarBuffer();

        if (usuarios.buscarPorNombre(nombre) != nullptr) {
            mensajeError("El usuario '" + nombre + "' ya existe.");
            pausar();
            return nullptr;
        }

        cout << "Ingrese su apellido: ";
        cin >> apellido;
        limpiarBuffer();

        cout << "Ingrese su DNI: ";
        while (!(cin >> dni) || dni <= 0) {
            mensajeError("DNI invalido.");
            limpiarBuffer();
        }

        cout << "Ingrese su edad: ";
        while (!(cin >> edad) || edad < 18) {
            mensajeError("Debe ser mayor de edad para registrarse.");
            limpiarBuffer();
        }

        // Crear contraseña segura
        bool passwordValida = false;
        while (!passwordValida) {
            cout << "\n--- Crear Contrasenia ---\n";
            cout << "Requisitos: minimo 6 caracteres, 2 numeros, 1 especial y 1 mayuscula.\n";

            cout << "Contrasenia: ";
            password = leerPasswordOculto();

            if (!validarPassword(password)) {
                mensajeError("Contrasenia no cumple los requisitos.");
                pausar();
                continue;
            }

            cout << "Confirmar Contrasenia: ";
            passwordConfirm = leerPasswordOculto();

            if (password != passwordConfirm) {
                mensajeError("Contrasenias no coinciden.");
                pausar();
            }
            else passwordValida = true;
        }

        // Generar ID automático
        string nuevoId = "U00" + to_string(usuarios.size() + 1);

        // Crear usuario
        T nuevoUsuario(nuevoId, nombre, apellido, dni, edad, password);
        usuarios.agregarFinal(nuevoUsuario);
        usuarios.guardar();


        mensajeExito("Usuario registrado exitosamente.");
        cout << "ID: " << nuevoId << " | Nombre: " << nombre << " " << apellido << endl;

        // === ESCENARIO 1: Creacion automatica de cuenta y tarjeta ===
        char deseaSaldo;
        cout << "\nDesea ingresar un saldo inicial para su cuenta? (S/N): ";
        cin >> deseaSaldo;
        limpiarBuffer();
        double saldoInicial = 0.0;

        if (deseaSaldo == 'S' || deseaSaldo == 's') {
            cout << "Ingrese el monto inicial (S/): ";
            cin >> saldoInicial;
            limpiarBuffer();

            if (!esMontoValido(saldoInicial)) {
                mensajeError("Monto invalido. Se establecera saldo inicial en S/ 0.00.");
                saldoInicial = 0.0;
            }
        }
        else {
            cout << "Saldo inicial establecido en S/ 0.00.\n";
        }

        /*_sleep(500);*/
        cout << "\nCreando su tarjeta y cuenta bancaria..." << endl;
        /*_sleep(800);*/
        animacionDeEspera();

        Console::Clear();
        // 1. Crear cuenta ahorro basica
        string idCuenta = "C" + to_string(rand() % 90000 + 10000);
        CuentaAhorro* cuenta = new CuentaAhorro(idCuenta, saldoInicial, 0.02);

        // 2. Crear tarjeta asociada (tipo débito)
        string numTarjeta;
        for (int i = 0; i < 12; i++) numTarjeta += to_string(rand() % 10);
        int cvc = rand() % 900 + 100;

        Tarjeta* tarjeta = new Tarjeta(
            numTarjeta,
            TipoTarjeta::DEBITO,
            cvc,
            nombre + " " + apellido,
            cuenta
        );

        // 3. Agregar la tarjeta al usuario
        T* usuario = usuarios.buscarPorNombre(nombre);
        if (usuario) {
            usuario->agregarTarjeta(tarjeta);
        /*    mensajeInfo("Se creo automaticamente una cuenta y tarjeta de débito asociadas.");*/
            cout << "\n[Tarjeta creada exitosamente]\n";
            cout << "Numero: " << numTarjeta << endl;
            cout << "Tipo: Debito" << endl;
            cout << "CVC: " << cvc << endl;
            cout << "Titular: " << nombre << " " << apellido << endl;
            cout << "Cuenta asociada: " << idCuenta << endl;
            cout << "Saldo inicial: S/ " << fixed << setprecision(2) << saldoInicial << endl;
        }

        // 4. Guardar cambios
        usuarios.guardar();
        GestorCuentas<T> gestorCuentas;
        GestorTarjetas<T> gestorTarjetas;

        gestorCuentas.guardarCuentas(usuarios);
        gestorTarjetas.guardarTarjetas(usuarios);

        pausar("Presione una tecla para continuar...");
        return usuarios.buscarPorNombre(nombre);
    }
};
