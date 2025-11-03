#pragma once
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace System;

// Animación de verificacion
void animacionVerificando() {
    string texto = "Verificando ";
    vector<string> anim = { ".  ", ".. ", "..." };
    Console::CursorVisible = false;

    for (int i = 0; i < 15; i++) {
        Console::SetCursorPosition(35, 38); // mover cursor en cada paso
        cout << texto << anim[i % 3] << flush;
        _sleep(100);
    }

    Console::SetCursorPosition(45, 40); // limpiar la animación
    cout << string(20, ' ') << flush;
    Console::CursorVisible = true;
}


void animacionDeEspera() {
    string texto = "Espere unos segundos ";
    vector<string> anim = { ".  ", ".. ", "..." };
    Console::CursorVisible = false;

    for (int i = 0; i < 15; i++) {
        Console::SetCursorPosition(35, 38);
        cout << texto << anim[i % 3] << flush;
        _sleep(100);
    }
    Console::SetCursorPosition(45, 40);
    cout << string(20, ' ') << flush;
    Console::CursorVisible = true;
}

// Leer contraseña oculta con asteriscos
string leerPasswordOculto(string pass = "") {
    const int MAX_LEN = 8; // VERIFICAR DADO A QUE ES LA LONGITUD DE LA CONTRASEÑA
    char ch = _getch();

    if (ch == '\r') { // Enter
        cout << endl;
        return pass;
    }
    else if (ch == '\b') { // Backspace
        if (!pass.empty()) {
            pass.pop_back();
            cout << "\b \b";
        }
        return leerPasswordOculto(pass);
    }
    else if (pass.size() < MAX_LEN) { // solo si no excede 8
        pass += ch;
        cout << '*';
        return leerPasswordOculto(pass);
    }
    else { // ya llegó al máximo, ignora tecla
        return leerPasswordOculto(pass);
    }
}


// Validar formato de contraseña

int contarDigitos(string password, int posicion) {
    if (posicion >= password.length()) {
        return 0;
    }

    char c = password[posicion];
    int esDigito = (c >= '0' && c <= '9') ? 1 : 0;

    return esDigito + contarDigitos(password, posicion + 1);
}

int contarMinusculas(string password, int posicion) {
    if (posicion >= password.length()) {
        return 0;
    }

    char c = password[posicion];
    int esMinuscula = (c >= 'a' && c <= 'z') ? 1 : 0;

    return esMinuscula + contarMinusculas(password, posicion + 1);
}

int contarMayusculas(string password, int posicion) {
    if (posicion >= password.length()) {
        return 0;
    }

    char c = password[posicion];
    int esMayuscula = (c >= 'A' && c <= 'Z') ? 1 : 0;

    return esMayuscula + contarMayusculas(password, posicion + 1);
}

int contarEspeciales(string password, int posicion) {
    if (posicion >= password.length()) {
        return 0;
    }

    char c = password[posicion];
    int esEspecial = 0;

    // Si NO es letra ni dígito, entonces es especial
    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))) {
        esEspecial = 1;
    }

    return esEspecial + contarEspeciales(password, posicion + 1);
}

bool validarPassword(string password) {
    int digitos = contarDigitos(password, 0);
    int minusculas = contarMinusculas(password, 0);
    int mayusculas = contarMayusculas(password, 0);
    int especiales = contarEspeciales(password, 0);

    // Verificar que cumpla todos los requisitos
    if (digitos >= 2 && minusculas >= 2 && mayusculas >= 1 && especiales >= 1) {
        return true;
    }

    return false;
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

// Pausar con mensaje
void pausar(const string& mensaje = "Presione cualquier tecla para salir...") {
    cout << mensaje;
    _getch();
}

// Uso de Lambdas 
auto formatoDinero = [](double monto) -> string {
    ostringstream os;
    os << fixed << setprecision(2) << monto;
    return os.str();
    };

void setColor(int color);

auto mensajeExito = [](const string& msg) {
    setColor(2); cout << "\n[EXITO] ";
    setColor(15); cout << msg << "\n";
    };

auto mensajeError = [](const string& msg) {
    setColor(4); cout << "\n[ERROR] ";
    setColor(15); cout << msg << "\n";
    };

// aun falta usar
auto mensajeInfo = [](const string& msg) {
    setColor(14); cout << "\n[INFO] ";
    setColor(15); cout << msg << "\n";
    };

// NOTA: se puede hacer tambien para eliminar cuenta
auto confirmarAccion = [](const string& mensaje) -> bool {
    char respuesta;
    cout << mensaje << " (S/N): ";
    cin >> respuesta;
    limpiarBuffer();
    return (respuesta == 'S' || respuesta == 's');
    };


// ------------------- PODRIA PASARSE A USER.H -------------------
auto generarCuenta = []() -> string {
    string cuenta;
    for (int i = 0; i < 9; i++) {
        cuenta += to_string(rand() % 10);
    }
    return cuenta;
    };

auto validarNumeroCuenta = [](const string& cuenta) -> bool {
    if (cuenta.size() != 9) return false;
    for (char c : cuenta) {
        if (!isdigit(c)) return false;
    }
    return true;
    };

//PEDIENTE
auto convertirDolaresASoles = [](double dolares, double tipoCambio = 3.85) -> double {
    return dolares * tipoCambio;
    };

auto formatearCuenta = [](const string& cuenta) -> string {
    string salida;
    for (int i = 0; i < cuenta.size(); i++) {
        salida += cuenta[i];
        if ((i + 1) % 3 == 0 && i != cuenta.size() - 1) salida += '-';
    }
    return salida;
    };

auto esMontoValido = [](double monto) -> bool {
    return (monto > 0 && monto <= 100000.0);
    };

auto tituloConsola = [](const string& titulo) {
    int ancho = 60;
    int espacio = (ancho - titulo.size()) / 2;
    cout << string(espacio, ' ') << titulo << "\n";
    cout << string(ancho, '=') << "\n";
    };


