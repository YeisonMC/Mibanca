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
