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
        double saldoInicial = 0.0;

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

        pausar("Presione una tecla para continuar...");
        return usuarios.buscarPorNombre(nombre);
    }
};
