#pragma once
#include <iostream>

template<typename T>
class Nodo {
public:
	T dato;
	Nodo<T>* siguiente;

	// Constructor
	Nodo(T v, Nodo<T>* sig = nullptr) {
		dato = v;
		siguiente = sig;
	}
};
