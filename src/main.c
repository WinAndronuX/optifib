#include <stdio.h>
#include <stdbool.h>
#include <optifib/utils.h>
#include <optifib/menu.h>
#include <optifib/graph.h>
#include <optifib/dijkstra.h>
#include <optifib/persistence.h>

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
            case 3: {
                char name[50];
                printf("\nIngresa el prefijo del mapa a cargar : ");
                scanf(" %49s", name);
                Graph* loadedG = loadGraph(name);
                if (loadedG) {
                    graphs->add(graphs, loadedG);
                    printf("Mapa '%s' cargado con exito desde CSV.\n", name);
                } else {
                    printf("|ERROR| No se pudo encontrar o cargar el mapa '%s'.\n", name);
                }
                pausa();
                break;
            }
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


                printf("\n  Tipo de despliegue de Fibra:\n");
                printf("\t 0. Aéreo (0.35 dB/km)\n");
                printf("\t 1. Subterráneo (0.40 dB/km)\n");
                int tipo_fibra = menuInputOpt(0, 1);

                double coeficiente = (tipo_fibra == FIBER_AERIAL) ? 0.35 : 0.40;


                edgeAdd(source_node, dest, (FiberDeployment)tipo_fibra, dist, coeficiente);
                edgeAdd(dest_node, src, (FiberDeployment)tipo_fibra, dist, coeficiente);
                printf("\nConexion establecida con exito desde [%d] hacia [%d]\n", src, dest);
                pausa();
                break;
            }
            case 5: {
                graphs->print(graphs);
                if (graphs->size == 0) {
                    pausa();
                    break;
                }
                printf("\nSelecciona el indice del mapa para GUARDAR: ");
                int idx = getInt();
                Graph* gToSave = graphs->get(graphs, idx);
                if (gToSave) {
                    if (saveGraph(gToSave) == MAP_STATUS_OP_OK) {
                        printf("Mapa '%s' guardado exitosamente.\n", gToSave->name);
                    } else {
                        printf("|ERROR| No se pudo guardar el mapa.\n");
                    }
                } else {
                    printf("Indice invalido.\n");
                }
                pausa();
                break;
            }
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

                printf("\n  Selecciona el tipo de equipo:\n");
                printf("\t 0. Poste (0.0 dB)\n");
                printf("\t 1. Pozo / Manhole (0.0 dB)\n");
                printf("\t 2. Caja de Empalme (0.1 dB)\n");
                printf("\t 3. Conexión / Conector (0.75 dB)\n");
                printf("\t 4. Hub de Distribución (0.5 dB)\n");
                printf("\t 5. Nodo OLT (origen, 0.0 dB)\n");
                printf("\t 6. Splitter 1:2 (3.5 dB)\n");
                printf("\t 7. Splitter 1:8 (10.5 dB)\n");
                printf("\t 8. Splitter 1:16 (13.8 dB)\n");

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

                nodeAdd(network, nodoId, desc, tipo, tipo, intrinsic_loss);
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
            case 1: { // Fusión
                printf("\nIngrese el ID del nodo para instalar Fusión: ");
                int id = getInt();
                Node* n = nodeFind(simNetwork, id);
                if (n) {
                    n->type = NODE_SPLICE_ENCLOSURE;
                    n->intrinsic_loss_db = 0.1;
                    printf("Nodo [%d] equipado con Fusión (0.1 dB).\n", id);
                } else printf("|ERROR| Nodo no encontrado.\n");
                pausa();
                break;
            }
            case 2: { // Conector
                printf("\nIngrese el ID del nodo para instalar Conector: ");
                int id = getInt();
                Node* n = nodeFind(simNetwork, id);
                if (n) {
                    n->type = NODE_CONNECTION;
                    n->intrinsic_loss_db = 0.75;
                    printf("Nodo [%d] equipado con Conector (0.75 dB).\n", id);
                } else printf("|ERROR| Nodo no encontrado.\n");
                pausa();
                break;
            }
            case 3: { // Splitter
                printf("\nIngrese el ID del nodo para instalar Splitter: ");
                int id = getInt();
                Node* n = nodeFind(simNetwork, id);
                if (n) {
                    printf("\n  Selecciona tipo de Splitter:\n");
                    printf("\t 1. Splitter 1:2 (3.5 dB)\n");
                    printf("\t 2. Splitter 1:8 (10.5 dB)\n");
                    printf("\t 3. Splitter 1:16 (13.8 dB)\n");
                    int sopt = menuInputOpt(1, 3);
                    if (sopt == 1) { n->type = NODE_SPLITTER_1_2; n->intrinsic_loss_db = 3.5; }
                    else if (sopt == 2) { n->type = NODE_SPLITTER_1_8; n->intrinsic_loss_db = 10.5; }
                    else { n->type = NODE_SPLITTER_1_16; n->intrinsic_loss_db = 13.8; }
                    printf("Nodo [%d] equipado con Splitter.\n", id);
                } else printf("|ERROR| Nodo no encontrado.\n");
                pausa();
                break;
            }
            case 4: {
                printf("\n--- Configuracion de Potencia y Sensibilidad ---");
                printf("\nPotencia actual OLT: %.2f dBm", current_olt_power_dbm);
                printf("\nIngrese nueva potencia OLT (dBm): ");
                current_olt_power_dbm = getDouble();

                printf("\nSensibilidad actual ONU: %.2f dBm", current_onu_sensitivity_dbm);
                printf("\nIngrese nueva sensibilidad ONU (ej. -28.0): ");
                current_onu_sensitivity_dbm = getDouble();

                printf("\nParametros actualizados correctamente.\n");
                pausa();
                break;
            }
            case 5: {

                int originId;
                printf("\n  ┌──────────────────────────────────────────┐\n");
                printf("  │    Simulador de Enrutamiento Óptimo      │\n");
                printf("  └──────────────────────────────────────────┘\n");
                if (simNetwork->V == 0) {
                    printf("|Error| La red esta vacia. Agregue nodos primero.\n");
                } else if (nodeCountType(simNetwork, NODE_OLT) == 0) {
                    printf("|Error| No se ha detectado ninguna OLT en el mapa.\n");
                    printf("Debe agregar al menos un nodo tipo OLT o equipar uno existente.\n");
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
