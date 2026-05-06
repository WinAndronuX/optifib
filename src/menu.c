#include <stdio.h>
#include <optifib/utils.h>
#include <optifib/menu.h>

const char* menuHeadStart = {
    "+--------------------------------------------+\n"
    "|                  OPTIFIB                   |\n"
    "|                                            |\n"
    "|               Menu Principal               |\n"
    "|                                            |\n"
    "| v0.0.1                                     |\n"
    "+--------------------------------------------+\n"
};

const char* menuStart[] = {
    "1 ) Gestion de Mapas",
    "2 ) Motor de Simulacion",
    "",
    "0 ) Salir",
    NULL
};


const char* menuHeadMap = {
    "+--------------------------------------------+\n"
    "|                  OPTIFIB                   |\n"
    "|                                            |\n"
    "|              Gestion de Mapas              |\n"
    "|                                            |\n"
    "| v0.0.1                                     |\n"
    "+--------------------------------------------+\n"
};

const char* menuMap[] = {
    "1 ) Visualizar topologia civil (nodos y tramos)",
    "2 ) Crear nuevo trazado urbano",
    "3 ) Cargar mapa base desde CSV",
    "4 ) Conectar nodos (trazar ducteria/posteria)",
    "5 ) Guardar mapa base",
    "",
    "0 ) Volver",
    NULL
};


const char* menuHeadSim = {
    "+--------------------------------------------+\n"
    "|                  OPTIFIB                   |\n"
    "|                                            |\n"
    "|             Motor de Simulacion            |\n"
    "|                                            |\n"
    "| v0.0.1                                     |\n"
    "+--------------------------------------------+\n"
};

const char* menuSim[] = {
    "1 ) Equipar Nodo: Instalar Fusion (0.1 dB)",
    "2 ) Equipar Nodo: Instalar Conector (0.5 dB)",
    "3 ) Equipar Nodo: Instalar Splitter (ej. 1:8 o 1:16)",
    "4 ) Definir Potencia OLT y Sensibilidad ONU",
    "5 ) EJECUTAR SIMULACION",
    "",
    "0 ) Volver",
    NULL
};

void menuPrint(const char* menu[]) {
    printf("\nOpciones:\n");

    int i;
    for (i = 0; menu[i] != NULL; i++)
        printf("\t%s\n", menu[i]);
}

int menuInputOpt(int a, int b) {
    int x = a - 1;

    while (1) {

        fseek(stdin, 0, SEEK_END);

        printf("%c ", PROMPT);
        scanf("%d", &x);

        if (isInRange(x, a, b)) break;

        printf("Opcion invalida.\n");
    }

    return x;
}

void pausa() {
    printf("\nOprima [Enter] para continuar...");
    while ((getchar()) != '\n');
    getchar();
}
