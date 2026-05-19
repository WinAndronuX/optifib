/*
 * Implementación del algoritmo de Dijkstra con Reporte de Viabilidad y Rastreo Inverso
 */

#include <stdio.h>
#include <float.h>
#include <optifib/graph.h>
#include <optifib/priority_queue.h>
#include <optifib/dijkstra.h>
#include <stdlib.h>

#include <optifib/persistence.h>

double current_olt_power_dbm = 2.0;      // Potencia estándar inicial
double current_onu_sensitivity_dbm = -28.0; // Umbral estándar inicial

void printPath(int current_node_id, Graph* graph) {
    if (current_node_id == -1) return;

    Node* n = NULL;
    for (int i = 0; i < graph->V; i++) {
        if (graph->arr[i].nodes->id == current_node_id) {
            n = graph->arr[i].nodes;
            break;
        }
    }

    if (n != NULL) {
        printPath(n->previous_node_id, graph);
        
        if (n->previous_node_id != -1) {
            printf(" -> ");
        }
        printf("%s [%d]", n->description, n->id);
    }
}

void emitirReporteViabilidad(double perdida_total) {
    double potencia_final = current_olt_power_dbm - perdida_total;

    printf("\n┌──────────────────────────────────────────────┐\n");
    printf("│        REPORTE DE VIABILIDAD ÓPTICA          │\n");
    printf("├──────────────────────────────────────────────┤\n");
    printf("│ Potencia de Salida (OLT):    %8.2f dBm    │\n", current_olt_power_dbm);
    printf("│ Pérdida Total Calculada:     %8.2f dB     │\n", perdida_total);
    printf("│ Potencia en el Destino:      %8.2f dBm    │\n", potencia_final);
    printf("│ Umbral de Sensibilidad (ONU):%8.2f dBm    │\n", current_onu_sensitivity_dbm);
    printf("├──────────────────────────────────────────────┤\n");

    if (potencia_final >= current_onu_sensitivity_dbm) {
        printf("│ ESTADO: [ VIABLE / OPERACIONAL ]             │\n");
        printf("│ Nota: La señal cumple con los estándares.    │\n");
    } else {
        printf("│ ESTADO: [ !!! SEÑAL CRÍTICA !!! ]            │\n");
        printf("│ ALERTA: La potencia es menor al umbral.      │\n");
        printf("│ Acción: Requiere rediseño o amplificación    │\n");
    }
    printf("└──────────────────────────────────────────────┘\n");
}


void dijkstra(Graph* graph, int src_id) {
    if (graph == NULL || graph->V == 0) return;

    Node* originCheck = nodeFind(graph, src_id);
    if (originCheck == NULL || (originCheck->type != NODE_OLT && originCheck->civil_type != NODE_OLT)) {
        printf("\n|ERROR| El nodo %d no es una OLT valida. La simulacion requiere una fuente de luz.\n", src_id);
        return;
    }

    int max_id = -1;
    for (int i = 0; i < graph->V; i++) {
        if (graph->arr[i].nodes->id > max_id) {
            max_id = graph->arr[i].nodes->id;
        }
    }

    Node** idToNode = (Node**) calloc(max_id + 1, sizeof(Node*));
    for (int i = 0; i < graph->V; i++) {
        Node* n = graph->arr[i].nodes;
        idToNode[n->id] = n;
        n->min_accumulated_loss = DBL_MAX;
        n->visited = 0;
        n->previous_node_id = -1;
    }

    Node* srcNode = idToNode[src_id];
    srcNode->min_accumulated_loss = 0;

    MinHeap* minHeap = createMinHeap(graph->V, max_id);
    for (int i = 0; i < graph->V; i++) {
        minHeap->array[i] = graph->arr[i].nodes;
        minHeap->pos[graph->arr[i].nodes->id] = i;
    }
    minHeap->size = graph->V;

    decreaseKey(minHeap, src_id, 0.0);

    while (minHeap->size != 0) {
        Node* uNode = extractMin(minHeap);
        uNode->visited = 1;

        if (uNode->min_accumulated_loss == DBL_MAX) break;

        Edge* edge = uNode->adj_list;
        while (edge != NULL) {
            Node* vNode = idToNode[edge->target_id];
            if (vNode != NULL && !vNode->visited) {
                double weight = edge->link_loss_db + vNode->intrinsic_loss_db;
                if (uNode->min_accumulated_loss + weight < vNode->min_accumulated_loss) {
                    vNode->min_accumulated_loss = uNode->min_accumulated_loss + weight;
                    vNode->previous_node_id = uNode->id;
                    decreaseKey(minHeap, vNode->id, vNode->min_accumulated_loss);
                }
            }
            edge = edge->next;
        }
    }

    printf("\n┌──────┬───────────────────────────┬────────────────┬────────────┐\n");
    printf("│ %-4s │ %-25s │ %-14s │ %-10s │\n", "ID", "DESCRIPCIÓN ", "PÉRDIDA (dB)", "PREDECESOR");
    printf("├──────┼───────────────────────────┼────────────────┼────────────┤\n");
    for (int i = 0; i < graph->V; i++) {
        Node* n = graph->arr[i].nodes;
        printf("│ %-4d │ %-25.25s │ ", n->id, n->description);
        if (n->min_accumulated_loss == DBL_MAX) printf("%-14s │ ", "INF");
        else printf("%-14.2f │ ", n->min_accumulated_loss);
        
        if (n->previous_node_id == -1) printf("%-10s │\n", "N/A");
        else printf("%-10d │\n", n->previous_node_id);
    }
    printf("└──────┴───────────────────────────┴────────────────┴────────────┘\n");

    int dest_id;
    printf("\nIngrese el ID del nodo destino para analizar la ruta y viabilidad: ");
    if (scanf("%d", &dest_id) == 1) {
        Node* destNode = nodeFind(graph, dest_id);
        if (destNode == NULL) {
            printf("Error: El nodo destino %d no existe.\n", dest_id);
        } else if (destNode->min_accumulated_loss == DBL_MAX) {
            printf("Alerta: El nodo destino %d es inaccesible desde la OLT.\n", dest_id);
        } else {
            // Ejecutar el Rastreo Inverso impreso en orden correcto
            printf("\n>>> RECONSTRUCCION DE RUTA OPTIMA:\n");
            printPath(dest_id, graph);
            printf("\n");

            emitirReporteViabilidad(destNode->min_accumulated_loss);

            printf("\nDesea exportar este reporte a un archivo? (1 = Si, 0 = No): ");
            int opt;
            scanf("%d", &opt);
            if (opt == 1) {
                exportReport(graph, src_id, dest_id, destNode->min_accumulated_loss, 
                             current_olt_power_dbm, current_olt_power_dbm - destNode->min_accumulated_loss, 
                             current_onu_sensitivity_dbm);
            }
        }
    }

    free(idToNode);
    freeMinHeap(minHeap);
}
