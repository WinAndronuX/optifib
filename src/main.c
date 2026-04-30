#include <stdio.h>
#include <stdbool.h>
#include <optifib/utils.h>
#include <optifib/menu.h>


bool programFinished = false;

void menu() {
    int opc = -1;

START:
    clearConsole();
    printf("%s", menuHeadStart);
    menuPrint(menuStart);
    opc = menuInputOpt(0, 5);

    switch (opc) {
    case 0: {
        programFinished = true;
        printf("Saliendo...\n");
        return;
    }
    case 1:
        goto MAP;
        break;
    case 2:
        goto SIM;
        break;
    }

    goto START;

MAP:
    while (1) {
        clearConsole();
        printf("%s", menuHeadMap);

        menuPrint(menuMap);
        opc = menuInputOpt(0, 5);

        switch (opc) {
            case 0:
                goto START;
                break;
            case 1:
                // TODO: Llamada a funcion
                break;
            case 2:
                // TODO: Llamada a funcion
                break;
            case 3:
                // TODO: Llamada a funcion
                break;
            case 4:
                // TODO: Llamada a funcion
                break;
            case 5:
                // TODO: Llamada a funcion
                break;
        }
    }

SIM:
    while (1) {
        clearConsole();
        printf("%s", menuHeadSim);

        menuPrint(menuSim);
        opc = menuInputOpt(0, 5);

        switch (opc) {
            case 0:
                goto START;
                break;
            case 1:
                // TODO: Llamada a funcion
                break;
            case 2:
                // TODO: Llamada a funcion
                break;
            case 3:
                // TODO: Llamada a funcion
                break;
            case 4:
                // TODO: Llamada a funcion
                break;
            case 5:
                // TODO: Llamada a funcion
                break;
        }
    }

}

int main() {
    clearConsole();

    menu();

    return 0;
}
