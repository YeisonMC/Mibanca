#pragma once
#include "Nodo.h"

template<typename T>
class Cola {
private:
    Nodo<T>* frente;
    Nodo<T>* final;
    int tamano;

public:
    Cola() : frente(nullptr), final(nullptr), tamano(0) {}

    ~Cola() {
        limpiar();
    }

    bool vacia() { return frente == nullptr; }
    int size() { return tamano; }

    void encolar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (final)
            final->siguiente = nuevo;
        else
            frente = nuevo;
        final = nuevo;
        tamano++;
    }

    bool desencolar() {
        if (!frente) return false;
        Nodo<T>* temp = frente;
        frente = frente->siguiente;
        if (!frente) final = nullptr;
        delete temp;
        tamano--;
        return true;
    }

    T& primero() {
        return frente->dato; // No llamar si vacía
    }

    T* obtenerPos(int idx) {
        if (idx < 0 || idx >= tamano) return nullptr;
        Nodo<T>* cur = frente;
        for (int i = 0; i < idx; i++) cur = cur->siguiente;
        return &(cur->dato);
    }

    bool eliminarPos(int idx) {
        if (idx < 0 || idx >= tamano) return false;
        if (idx == 0) return desencolar();

        Nodo<T>* prev = frente;
        for (int i = 0; i < idx - 1; i++) prev = prev->siguiente;

        Nodo<T>* temp = prev->siguiente;
        prev->siguiente = temp->siguiente;
        if (temp == final) final = prev;

        delete temp;
        tamano--;
        return true;
    }

    void limpiar() {
        while (!vacia()) desencolar();
    }
};
