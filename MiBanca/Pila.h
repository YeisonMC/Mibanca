#pragma once
#include "Nodo.h"

template<typename T>
class Pila {
private:
    Nodo<T>* tope;
    int tamano;

public:
    Pila() : tope(nullptr), tamano(0) {}

    ~Pila() {
        limpiar();
    }

    bool vacia() { return tope == nullptr; }
    int size() { return tamano; }

    void push(const T& valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor, tope);
        tope = nuevo;
        tamano++;
    /*    Nodo<T>* nuevo = new Nodo<T>(valor);
        nuevo->siguiente = tope;
        tope = nuevo; 
        tamano++;*/
    }

    bool pop() {
        if (!tope) return false;
        Nodo<T>* temp = tope;
        tope = tope->siguiente;
        delete temp;
        tamano--;
        return true;
    }

    T& top() {
        return tope->dato; // ¡no llamar si está vacía!
    }

    void limpiar() {
        while (!vacia()) pop();
    }
};
