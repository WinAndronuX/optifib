/*
 * Implementacion del grafo mediante una Lista de Adyaciencia
 */

#include <stdio.h>
#include <stdlib.h>
#include <optifib/graph.h>



Graph* create_graph(int capacity) {
    Graph* g = malloc(sizeof(Graph));
    g->capacity = capacity;
    g->count = 0;
    g->nodes = malloc(sizeof(Node) * capacity);
    return g;
}

void add_node(Graph* g, int id, NodeType type) {

    if (g->count >= g->capacity) {
        printf("Error: Grafo lleno (Capacidad maxima alcanzada)\n");
        return;
    }

    Node* newNode = &g->nodes[g->count];

    newNode->id = id;
    newNode->type = type;
    newNode->adj_list = NULL;

    g->count++;
}

void add_edge(Node* source, int target_id, float distance) {

    Edge* newEdge = (Edge*) malloc(sizeof(Edge));

    if (newEdge == NULL) return;

    newEdge->target_id = target_id;
    newEdge->distance_km = distance;

    newEdge->next = source->adj_list;
    source->adj_list = newEdge;
}

Node* find_node(Graph* g, int id) {
    for (int i = 0; i < g->count; i++) {
        if (g->nodes[i].id == id) {
            return &g->nodes[i];
        }
    }
    return NULL;
}