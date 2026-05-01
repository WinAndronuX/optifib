/*
 * Implementacion del grafo mediante una Lista de Adyaciencia
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <optifib/graph.h>


Graph* graphCreate() {
    Graph* g = (Graph*) malloc(sizeof(Graph));
    if (g == NULL) return NULL;

    g->V = 0;
    g->arr = NULL;

    return g;
}


void nodeAdd(Graph* g, int id, const char* description, NodeType type, double intrinsic_loss) {
    if (g == NULL) return;
    Adjlist* temp_arr = (Adjlist*) realloc(g->arr, sizeof(Adjlist) * (g->V + 1));
    if (temp_arr == NULL) return;

    g->arr = temp_arr;


    g->arr[g->V].nodes = (Node*) malloc(sizeof(Node));
    if (g->arr[g->V].nodes == NULL) return;

    Node* newNode = g->arr[g->V].nodes;


    newNode->id = id;
    newNode->type = type;
    newNode->intrinsic_loss_db = intrinsic_loss;


    strncpy(newNode->description, description, 49);
    newNode->description[49] = '\0';


    newNode->adj_list = NULL;


    newNode->min_accumulated_loss = DBL_MAX;
    newNode->previous_node_id = -1;
    newNode->visited = 0;

    g->V++;
}


void edgeAdd(Node* source, int target_id, FiberDeployment type, double distance, double link_loss) {
    if (source == NULL) return;

    Edge* newEdge = (Edge*) malloc(sizeof(Edge));
    if (newEdge == NULL) return;


    newEdge->source_id = source->id;
    newEdge->target_id = target_id;
    newEdge->type = type;
    newEdge->distance_km = distance;
    newEdge->link_loss_db = link_loss;


    newEdge->next = source->adj_list;
    source->adj_list = newEdge;
}


Node* nodeFind(Graph* g, int id) {
    if (g == NULL || g->arr == NULL) return NULL;

    for (int i = 0; i < g->V; i++) {

        if (g->arr[i].nodes->id == id) {
            return g->arr[i].nodes;
        }
    }
    return NULL;
}


void graphFree(Graph* g) {
    if (g == NULL) return;

    for (int i = 0; i < g->V; i++) {
        Node* current_node = g->arr[i].nodes;
        if (current_node != NULL) {

            Edge* current_edge = current_node->adj_list;
            while (current_edge != NULL) {
                Edge* temp_edge = current_edge;
                current_edge = current_edge->next;
                free(temp_edge);
            }

            free(current_node);
        }
    }


    if (g->arr != NULL) free(g->arr);
    free(g);
}