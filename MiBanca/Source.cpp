#include <iostream>
#include <conio.h>
#include "Cuenta.h"
#include "Tarjeta.h"
#include "Usuario.h"
using namespace std;

int main() {
    // Crear cuentas
    CuentaAhorro* cuentaAhorro1 = new CuentaAhorro("AH001", 1500.0, 0.03);
    CuentaCredito* cuentaCredito1 = new CuentaCredito("CR001", 500.0, 2000.0, 300.0);

    // Crear tarjetas asociadas
    Tarjeta* tarjetaDebito = new Tarjeta("1234-5678-9012-3456", TipoTarjeta::DEBITO, 123, "Juan Perez", cuentaAhorro1);
    Tarjeta* tarjetaCredito = new Tarjeta("9876-5432-1098-7654", TipoTarjeta::CREDITO, 456, "Juan Perez", cuentaCredito1);

    // Crear usuario
    Usuario* usuario1 = new Usuario("CL-001", "Juan", "Perez", 12345678, 30, "clave123");

    // Asociar tarjetas al usuario
    usuario1->agregarTarjeta(tarjetaDebito);
    usuario1->agregarTarjeta(tarjetaCredito);

    // Mostrar informacion general del usuario
    usuario1->mostrarInfo();

    // Mostrar tarjetas del usuario
    usuario1->mostrarTarjetas();

    _getch();
    return 0;
}