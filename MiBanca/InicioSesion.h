#pragma once
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include "Utilidades.h"
#include "ListaUsuarios.h"
#include "Usuario.h"

using namespace std;
using namespace System;

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

template<typename T>
class InicioSesion {
private:
    string nombreUsuario;
    string contrasenia;
    T* usuarioActual;

public:
    InicioSesion() : usuarioActual(nullptr) {}

    ~InicioSesion() {}

    // ========== FUNCIÓN PRINCIPAL ==========
    bool iniciar(ListaUsuarios<T>& usuarios) {
        /* galeria.Fondo_1();
         _sleep(1000);
         Console::Clear();
         galeria.Fondo_2();*/

        Console::Clear();
        tituloConsola("SISTEMA BANCARIO - INICIO DE SESION");

        Console::SetCursorPosition(30, 12);
        cout << "Nombre de usuario: ";
        getline(cin, nombreUsuario);

        Console::SetCursorPosition(30, 14);
        cout << "Contrasenia: ";
        contrasenia = leerPasswordOculto();

        animacionVerificando();

        return procesarLogin(usuarios);
    }

    // ========== GETTERS ==========
    T* getUsuarioActual() { return usuarioActual; }
    string getNombreActual() { return usuarioActual ? usuarioActual->getNombre() : ""; }

    void cerrarSesion() { usuarioActual = nullptr; }

private:
    // ========== PROCESO DE LOGIN ==========
    bool procesarLogin(ListaUsuarios<T>& usuarios) {
        // Buscar usuario por nombre
        T* usuario = usuarios.buscarPorNombre(nombreUsuario);

        if (usuario == nullptr) {
            mensajeError("Usuario no encontrado.");
            if (confirmarAccion("Desea registrarse ahora?")) {
                // (Hook para registrar nuevo usuario)
                mensajeInfo("Redirigiendo a registro (pendiente de implementar)...");
                pausar();
            }
            return false;
        }

        // Verificar contraseña
        if (usuario->getPassword() != contrasenia) {
            mensajeError("Contrasenia incorrecta.");
            pausar();
            return false;
        }

        // Login exitoso
        usuarioActual = usuario;
        mostrarBienvenida(usuario);
        return true;
    }

    // ========== MENSAJE DE BIENVENIDA ==========
    void mostrarBienvenida(T* usuario) {
        Console::Clear();
        tituloConsola("BIENVENIDO/A");
        setColor(10); // verde
        cout << "\nUsuario: " << usuario->getNombre() << " " << usuario->getApellido();
        cout << "\nDNI: " << usuario->getDni();
        cout << "\nEdad: " << usuario->getEdad();
        cout << "\nTarjetas registradas: " << usuario->getTarjetas().size() << endl;
        setColor(15);
        pausar("\nPresione cualquier tecla para continuar...");
    }
};
