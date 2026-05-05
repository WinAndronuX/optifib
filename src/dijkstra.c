/*
 * Implementación del algoritmo de Dijkstra
 */

#include <stdio.h>
#include <float.h>
#include <optifib/graph.h>
#include <optifib/priority_queue.h>
#include <optifib/dijkstra.h>
#include <stdlib.h>

void dijkstra(Graph* graph, int src_id) {
    if (graph == NULL || graph->V == 0) return;

    // Inicializar distancias y estados
    for (int i = 0; i < graph->V; i++) {
        Node* n = graph->arr[i].nodes;
        n->min_accumulated_loss = DBL_MAX;
        n->visited = 0;
        n->previous_node_id = -1;
    }

    Node* srcNode = nodeFind(graph, src_id);
    if (srcNode == NULL) {
        printf("Error: Nodo origen %d no encontrado.\n", src_id);
        return;
    }

    srcNode->min_accumulated_loss = 0; // OLT no suele tener perdida inicial en el origen

    MinHeap* minHeap = createMinHeap(graph->V);
    for (int i = 0; i < graph->V; i++) {
        minHeap->array[i] = graph->arr[i].nodes;
        minHeap->pos[graph->arr[i].nodes->id] = i;
    }
    minHeap->size = graph->V;

    // Mover el origen al inicio del heap
    decreaseKey(minHeap, src_id, 0.0);

    while (minHeap->size != 0) {
        Node* uNode = extractMin(minHeap);
        uNode->visited = 1;

        if (uNode->min_accumulated_loss == DBL_MAX) break;

        Edge* edge = uNode->adj_list;
        while (edge != NULL) {
            Node* vNode = nodeFind(graph, edge->target_id);
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

    // Output de resultados técnicos
    printf("\n+------+---------------------------+----------------+------------+\n");
    printf("| ID   | Descripcion               | Perdida (dB)   | Predecesor |\n");
    printf("+------+---------------------------+----------------+------------+\n");
    for (int i = 0; i < graph->V; i++) {
        Node* n = graph->arr[i].nodes;
        printf("| %-4d | %-25s | ", n->id, n->description);
        if (n->min_accumulated_loss == DBL_MAX) printf("%-14s | ", "INF");
        else printf("%-14.2f | ", n->min_accumulated_loss);
        
        if (n->previous_node_id == -1) printf("%-10s |\n", "N/A");
        else printf("%-10d |\n", n->previous_node_id);
    }
    printf("+------+---------------------------+----------------+------------+\n");

    freeMinHeap(minHeap);
}
