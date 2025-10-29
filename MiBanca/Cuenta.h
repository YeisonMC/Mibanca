#pragma once
#include <iostream>

using namespace std;

class Cuenta {
protected:
	string idCuenta;
	double saldo;

public:
	// Constructor
	Cuenta(string idCuenta, double saldo = 0.0) : idCuenta(idCuenta), saldo(saldo) {}

	// Metodos
	virtual void mostrarDetalles() = 0;
	virtual void aplicarInteres() = 0;
	virtual bool retirar(double monto) = 0;

	// Getters
	double getSaldo() const { return saldo; }
	string getIdCuenta() const { return idCuenta; }

	void depositar(double monto) {
		if (monto > 0) {
			saldo += monto;
			cout << "Deposito exitoso de S/. " << monto << " a la cuenta " << idCuenta << endl;
		}
		else {
			cout << "Error: El monto a depositar debe ser positivo." << endl;
		}
	}
};

class CuentaAhorro : public Cuenta {
private:
	double tasaInteres;

public:
	CuentaAhorro(string idCuenta, double saldo, double tasaInteres) : Cuenta(idCuenta, saldo), tasaInteres(tasaInteres) {}

	bool retirar(double monto) override {
		if (monto <= 0 || monto > saldo) return false;
		saldo -= monto; return true;
	}

	void aplicarInteres() override {
		saldo += saldo * tasaInteres;
	}

	void mostrarDetalles() override {
		cout << "Tipo: Cuenta de Ahorro" << endl;
		cout << "ID Cuenta: " << idCuenta << endl;
		cout << "Saldo: S/. " << saldo << endl;
		cout << "Tasa de Interes: " << tasaInteres * 100 << "%" << endl;
	}
};

class CuentaCredito : public Cuenta {
private:
	double limiteCredito;
	double deudaActual;

public:
	CuentaCredito(string idCuenta, double saldo, double limiteCredito, double deudaActual) : Cuenta(idCuenta, saldo), limiteCredito(limiteCredito), deudaActual(deudaActual) {}

	bool retirar(double monto) override {
		if (monto <= 0) return false;

		double disponible = saldo + (limiteCredito - deudaActual);
		if (monto > disponible) return false;

		if (monto <= saldo) {
			saldo -= monto;
		}
		else {
			double restante = monto - saldo;
			saldo = 0;
			deudaActual += restante;
		}
		return true;
	}

	void aplicarInteres() override {
		double interes = deudaActual * 0.02; // 2% mensual, por ejemplo
		deudaActual += interes;
	}

	void mostrarDetalles() override {
		cout << "Tipo: Cuenta de Credito" << endl;
		cout << "ID Cuenta: " << idCuenta << endl;
		cout << "Saldo disponible: S/. " << saldo << endl;
		cout << "Limite de Credito: S/. " << limiteCredito << endl;
		cout << "Deuda actual: S/. " << deudaActual << endl;
	}
};