#pragma once
#include <iostream>

template<typename T>
class Nodo {
public:
	T dato;
	Nodo<T>* siguiente;

	// Constructor
	Nodo(T valor, Nodo<T>* sig = nullptr) {
		dato = valor;
		siguiente = sig;
	}
};
