#include <stdio.h>
#include <optifib/utils.h>
#include <optifib/menu.h>

const char* menuHeadStart = {
    "┌────────────────────────────────────────────┐\n"
    "│                  OPTIFIB                   │\n"
    "│                                            │\n"
    "│               Menú Principal               │\n"
    "│                                            │\n"
    "│ v0.0.1                                     │\n"
    "└────────────────────────────────────────────┘\n"
};

const char* menuStart[] = {
    "1 ) Gestión de Mapas",
    "2 ) Motor de Simulación",
    "",
    "0 ) Salir",
    NULL
};


const char* menuHeadMap = {
    "┌────────────────────────────────────────────┐\n"
    "│                  OPTIFIB                   │\n"
    "│                                            │\n"
    "│              Gestión de Mapas              │\n"
    "│                                            │\n"
    "│ v0.0.1                                     │\n"
    "└────────────────────────────────────────────┘\n"
};

const char* menuMap[] = {
    "1 ) Visualizar listado de mapas",
    "2 ) Crear nuevo mapa (trazado urbano)",
    "3 ) Cargar mapa base desde CSV",
    "4 ) Conectar nodos (trazar ductería/postería)",
    "5 ) Guardar mapa base",
    "6 ) Agregar nodo a un mapa",
    "",
    "0 ) Volver",
    NULL
};


const char* menuHeadSim = {
    "┌────────────────────────────────────────────┐\n"
    "│                  OPTIFIB                   │\n"
    "│                                            │\n"
    "│             Motor de Simulación            │\n"
    "│                                            │\n"
    "│ v0.0.1                                     │\n"
    "└────────────────────────────────────────────┘\n"
};

const char* menuSim[] = {
    "1 ) Equipar Nodo: Instalar Fusión (0.1 dB)",
    "2 ) Equipar Nodo: Instalar Conector (0.5 dB)",
    "3 ) Equipar Nodo: Instalar Splitter (ej. 1:8 o 1:16)",
    "4 ) Definir Potencia OLT y Sensibilidad ONU",
    "5 ) EJECUTAR SIMULACIÓN",
    "",
    "0 ) Volver",
    NULL
};

void menuPrint(const char* menu[]) {
    printf("\n Opciones:\n");

    int i;
    for (i = 0; menu[i] != NULL; i++) {
        if (menu[i][0] != '\0') {
            printf("\t %s\n", menu[i]);
        } else {
            printf("\n");
        }
    }
}

int menuInputOpt(int a, int b) {
    int x = a - 1;

    while (1) {

        fseek(stdin, 0, SEEK_END);

        printf("\n %c ", PROMPT);
        scanf("%d", &x);

        if (isInRange(x, a, b)) break;

        printf(" Opción inválida.\n");
    }

    return x;
}

void pausa() {
    printf("\n Oprima [Enter] para continuar...");
    while ((getchar()) != '\n');
    getchar();
}
