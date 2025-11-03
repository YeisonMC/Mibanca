#pragma once
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

string fechaTextoActual() {
	time_t now = time(0);
	tm* timeptr = localtime(&now);

	vector<string> dia_semana;
	dia_semana.push_back("Domingo");
	dia_semana.push_back("Lunes");
	dia_semana.push_back("Martes");
	dia_semana.push_back("Miercoles");
	dia_semana.push_back("Jueves");
	dia_semana.push_back("Viernes");
	dia_semana.push_back("Sabado");

	vector<string> mes;
	mes.push_back("Enero");
	mes.push_back("Febrero");
	mes.push_back("Marzo");
	mes.push_back("Abril");
	mes.push_back("Mayo");
	mes.push_back("Junio");
	mes.push_back("Julio");
	mes.push_back("Agosto");
	mes.push_back("Septiembre");
	mes.push_back("Octubre");
	mes.push_back("Noviembre");
	mes.push_back("Diciembre");

	int year = 1900 + timeptr->tm_year;

	ostringstream os;
	os << dia_semana[timeptr->tm_wday] << ", "
		<< timeptr->tm_mday << " de "
		<< mes[timeptr->tm_mon] << " del "
		<< year;
	return os.str();
}

string horaTextoActual() {
	time_t now = time(0);
	tm* timeptr = localtime(&now);

	ostringstream os;
	os << setfill('0')
		<< setw(2) << timeptr->tm_hour << ":"
		<< setw(2) << timeptr->tm_min << ":"
		<< setw(2) << timeptr->tm_sec;
	return os.str();
}

template<typename T1, typename T2, typename T3, typename T4>
class Movimiento {
private:
	T1 tipo;
	T2 fecha;
	T3 hora;
	T4 monto;

public:
	// Constructores
	Movimiento() {
		tipo = "";
		fecha = "";
		hora = "";
		monto = 0.0;
	}

	Movimiento(T1 t, T4 m) {
		tipo = t;
		fecha = fechaTextoActual();
		hora = horaTextoActual();
		monto = m;
	}

	Movimiento(T1 t, T2 f, T3 h, T4 m) {
		tipo = t;
		fecha = f;
		hora = h;
		monto = m;
	}

	// Metodos 
	string aLinea() {
		ostringstream os;
		os << tipo << '|' << fecha << '|' << hora << '|'
			<< formatoDinero(monto);
		return os.str();
	}

	void imprimir() {
		cout << fecha << " - " << hora
			<< " | " << tipo
			<< " | S/ " << formatoDinero(monto) << "\n";
	}

	// Getters
	T1 getTipo() { return tipo; }
	T2 getFecha() { return fecha; }
	T3 getHora() { return hora; }
	T4 getMonto() { return monto; }
};

// FUNCIONES 
Movimiento<string, string, string, double> parsearMovimiento(string linea) {
	string t, f, h, m;
	stringstream ss(linea);
	getline(ss, t, '|');
	getline(ss, f, '|');
	getline(ss, h, '|');
	getline(ss, m, '|');

	double mon = 0.0;
	if (!m.empty()) mon = stod(m);

	return Movimiento<string, string, string, double>(t, f, h, mon);
}

