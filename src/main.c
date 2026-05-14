#include <stdio.h>
#include <stdbool.h>
#include <optifib/utils.h>
#include <optifib/menu.h>
#include <optifib/graph.h>
#include <optifib/dijkstra.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define GRAPH_MAX 5

bool programFinished = false;


void menu() {
    int opc = -1;

    GraphList* graphs = initGrapList();

START:
    clearConsole();
    printf("%s", menuHeadStart);
    menuPrint(menuStart);

    opc = menuInputOpt(0, 2);

    switch (opc) {
    case 0: {
        programFinished = true;
        printf("Saliendo y liberando memoria...\n");

        graphs->destroy(graphs);
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
        opc = menuInputOpt(0, 6);

        switch (opc) {
            case 0:
                goto START;
                break;
            case 1:
                graphs->print(graphs);
                if (graphs->size > 0) {
                    printf("\nSelecciona el indice del mapa para visualizar detalles (o -1 para cancelar): ");
                    int idx = getInt();
                    Graph* g = graphs->get(graphs, idx);
                    if (g) graphPrint(g);
                    else if (idx != -1) printf("Indice invalido.\n");
                }
                pausa();
                break;
            case 2: {
                char name[50];
                printf("\nIngresa el nombre para el nuevo mapa: ");
                scanf(" %49[^\n]", name);
                Graph* newG = graphCreate(name);
                graphs->add(graphs, newG);
                printf("Mapa '%s' creado con exito.\n", name);
                pausa();
                break;
            }
            case 3:
                // TODO: Llamada a funcion de carga
                break;
            case 4: {
                graphs->print(graphs);
                if (graphs->size == 0) {
                    pausa();
                    break;
                }
                printf("\nSelecciona el indice del mapa: ");
                int idx = getInt();
                Graph* network = graphs->get(graphs, idx);
                if (!network) {
                    printf("Indice invalido.\n");
                    pausa();
                    break;
                }

                int src, dest;
                double dist;

                printf("\nIngresa el ID del poste ORIGEN: ");
                src = getInt();

                printf("\nIngresa el ID del poste DESTINO: ");
                dest = getInt();

                Node* source_node = nodeFind(network, src);
                Node* dest_node = nodeFind(network, dest);

                if (src == dest) {
                    printf("\n|ERROR| No puedes conectar un poste consigo mismo.\n");
                    pausa();
                    break;
                }
                if (source_node == NULL || dest_node == NULL)  {
                    printf("\n|ERROR| No se puede tender fibra. ");
                    if (source_node == NULL) printf("\nEl poste origen [%d] no existe. \n", src);
                    if (dest_node == NULL) printf("\nEl poste destino [%d] no existe.\n", dest);
                    pausa();
                    break;
                }

                printf("Ingresa la distancia (km): ");
                dist = getDouble();


                printf("\n\nTipo de despliegue de Fibra:\n");
                printf("\t0. Aereo (0.35 dB/km)\n");
                printf("\t1. Subterraneo (0.40 dB/km)\n");
                int tipo_fibra = menuInputOpt(0, 1);

                double coeficiente = (tipo_fibra == FIBER_AERIAL) ? 0.35 : 0.40;
                double linkLoss = dist * coeficiente;


                edgeAdd(source_node, dest, (FiberDeployment)tipo_fibra, dist, linkLoss);
                edgeAdd(dest_node, src, (FiberDeployment)tipo_fibra, dist, linkLoss);
                printf("\nConexion establecida con exito desde [%d] hacia [%d]\n", src, dest);
                pausa();
                break;
            }
            case 5:
                // TODO: Llamada a funcion de guardado
                break;
            case 6: {
                graphs->print(graphs);
                if (graphs->size == 0) {
                    pausa();
                    break;
                }
                printf("\nSelecciona el indice del mapa: ");
                int idx = getInt();
                Graph* network = graphs->get(graphs, idx);
                if (!network) {
                    printf("Indice invalido.\n");
                    pausa();
                    break;
                }

                int nodoId;
                char desc[50];
                double intrinsic_loss = 0.0;
                NodeType tipo;

                printf("\nIngresa el ID del nuevo poste/nodo:");
                nodoId = getInt();
                printf("Ingresa una breve descripcion\n > ");
                scanf(" %49[^\n]", desc);

                printf("\nSelecciona el tipo de equipo:\n\n");
                printf("\t0. Poste (0.0 dB)\n");
                printf("\t1. Pozo / Manhole (0.0 dB)\n");
                printf("\t2. Caja de Empalme (0.1 dB)\n");
                printf("\t3. Conexión / Conector (0.75 dB)\n");
                printf("\t4. Hub de Distribución (0.5 dB)\n");
                printf("\t5. Nodo OLT (origen, 0.0 dB)\n");
                printf("\t6. Splitter 1:2 (3.5 dB)\n");
                printf("\t7. Splitter 1:8 (10.5 dB)\n");
                printf("\t8. Splitter 1:16 (13.8 dB)\n");

                int tipo_input = menuInputOpt(0, 8);
                tipo = (NodeType)tipo_input;

                switch(tipo) {
                    case NODE_SPLICE_ENCLOSURE: intrinsic_loss = 0.1; break;
                    case NODE_CONNECTION:       intrinsic_loss = 0.75; break;
                    case NODE_DISTRIBUTION_HUB: intrinsic_loss = 0.5; break;
                    case NODE_SPLITTER_1_2:     intrinsic_loss = 3.5; break;
                    case NODE_SPLITTER_1_8:     intrinsic_loss = 10.5; break;
                    case NODE_SPLITTER_1_16:    intrinsic_loss = 13.8; break;
                    default:                    intrinsic_loss = 0.0; break; // POSTE, MANHOLE, OLT
                }

                nodeAdd(network, nodoId, desc, tipo, intrinsic_loss);
                printf("\nPoste [%d] agregado con exito al mapa '%s'\n", nodoId, network->name);
                pausa();
                break;
            }
        }
    }

SIM:
    graphs->print(graphs);
    if (graphs->size == 0) {
        printf("\nNo hay mapas cargados para simular.\n");
        pausa();
        goto START;
    }
    printf("\nSelecciona el indice del mapa para la simulacion: ");
    int simIdx = getInt();
    Graph* simNetwork = graphs->get(graphs, simIdx);
    if (!simNetwork) {
        printf("Indice invalido.\n");
        pausa();
        goto START;
    }

    while (1) {
        clearConsole();
        printf("%s", menuHeadSim);
        printf("\nMAPA ACTUAL: %s\n", simNetwork->name);

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
            case 5: {

                int originId;
                printf("\n--- Simulador de Enrutamiento Optimo ---\n");
                if (simNetwork->V == 0) {
                    printf("|Error| La red esta vacia. Agregue nodos primero.\n");
                } else {
                    printf("Ingrese el ID del Nodo OLT (Origen de la señal): ");
                    originId = getInt();

                    dijkstra(simNetwork, originId);
                }

                pausa();
                break;
            }
        }
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    clearConsole();

    menu();

    return 0;
}
