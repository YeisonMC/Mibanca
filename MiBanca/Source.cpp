//#include <iostream>
//#include <conio.h>
//#include "Cuenta.h"
//#include "Tarjeta.h"
//#include "Usuario.h"
//#include "ListaUsuarios.h"
//using namespace std;
//
//int main() {
//    CuentaAhorro* cuentaAhorro1 = new CuentaAhorro("CTA001", 5000.0, 0.03);
//    CuentaCredito* cuentaCredito1 = new CuentaCredito("CTA003", 2000.0, 5000.0, 0.0);  
//
//     // Crear algunos usuarios
//    Usuario u1 ("U001", "Juan", "Perez", 12345678, 30, "clave123");
//
//    // Crear algunas tarjetas para cada usuario
//    Tarjeta* tarjeta1 = new Tarjeta("1234-5678-9012-3456", TipoTarjeta::DEBITO, 123, "Juan Perez", cuentaAhorro1);
//    Tarjeta* tarjeta2 = new Tarjeta("9876-5432-1098-7654", TipoTarjeta::CREDITO, 456, "Juan Perez", cuentaCredito1);
//
//    // Agregar tarjetas a los usuarios
//    u1.agregarTarjeta(tarjeta1);
//    u1.agregarTarjeta(tarjeta2);
//
//    cout << "==== DETALLES DE TARJETAS " << endl;
//    cout << endl;
//    tarjeta1->mostrarInfo();
//    tarjeta2->mostrarInfo();
//
//    // Crear la lista de usuarios y agregar los usuarios
//    ListaUsuarios<Usuario>* listaUsuarios = new  ListaUsuarios<Usuario>;
//    listaUsuarios->agregarFinal(u1);
//
//    // Guardar los usuarios en un archivo binario
//    listaUsuarios->guardar("usuarios.bin");
//
//    // Limpiar la lista para cargar de nuevo
//    listaUsuarios->limpiar();
//
//    // Cargar los usuarios desde el archivo binario
//    listaUsuarios->cargar("usuarios.bin");
//
//    // Listar los usuarios cargados
//    listaUsuarios->listarUsuarios();
//
//    _getch();
//    return 0;
//}

#include <iostream>
#include "Banco.h"
#include "Usuario.h"

using namespace std;

int main() {
    Console::SetWindowSize(90, 50);
    Console::CursorVisible = false;

    Banco<Usuario> banco;
    banco.ejecutar();

    return 0;
}
