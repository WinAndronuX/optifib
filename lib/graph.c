/*
 * Implementacion del grafo mediante una Lista de Adyaciencia
 */

#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <optifib/graph.h>
#include <stdio.h>

Graph* graphCreate(const char* name) {
    Graph* g = (Graph*) malloc(sizeof(Graph));
    if (g == NULL) return NULL;

    g->V = 0;
    g->arr = NULL;
    g->name = strdup(name);

    return g;
}


void graphPrint(Graph* g) {
    if (g == NULL || g->V == 0) {
        printf("\n|ERROR|La red %s esta vacia\n", g ? g->name : "");
        return;
    }
    printf("\n");
    printf("\n");    printf("\n\t\t=== TOPOLOGIA DE LA RED OPTICA: %s ===\n\n", g->name);
    printf("\t\t  --- INFRAESTRUCTURA (NODOS) ---\n");
    printf("┌──────┬──────────────────────┬────────────────────────────────┬────────────┐\n");
    printf("│ %-4s │ %-20s │ %-31s │ %-11s │\n", "ID", "TIPO DE EQUIPO", "DESCRIPCIÓN", "PÉRDIDA");
    printf("├──────┼──────────────────────┼────────────────────────────────┼────────────┤\n");

    for (int i = 0; i < g->V; i++) {
        Node* n = g->arr[i].nodes;
        if (n != NULL) {
            printf("│ %-4d │ %-20s │ %-30.30s │ %5.2f dB   │\n",
              n->id,
              NodeTypeStr[n->civil_type],
              n->description,
              n->intrinsic_loss_db);

        }
    }

    printf("└──────┴──────────────────────┴────────────────────────────────┴────────────┘\n");

    printf("\n\n");

    printf("\t\t--- CONEXIONES DE FIBRA ÓPTICA (ARISTAS) ---\n");
    printf("┌──────┬──────┬───────────────────┬─────────────┬────────────┐\n");
    printf("│ %-4s │ %-4s │ %-17s │ %-11s │ %-10s │\n", "ORIG", "DEST", "TIPO DE FIBRA", "DISTANCIA", "ATENUACIÓN");
    printf("├──────┼──────┼───────────────────┼─────────────┼────────────┤\n");
    for (int i = 0; i < g->V; i++) {

        Node* n = g->arr[i].nodes;
        Edge* e = n->adj_list;
        while (e != NULL) {
            if (e->source_id < e->target_id) {
                printf("│ %-4d │ %-4d │ %-17s │ %7.2f km  │ %5.2f dB   │\n",
                    e->source_id,
                    e->target_id,
                    FiberDeploymentStr[e->type],
                    e->distance_km,
                    e->link_loss_db);
            }
            e = e->next;
        }
    }


    printf("└──────┴──────┴───────────────────┴─────────────┴────────────┘\n");

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
    newNode->civil_type = type;
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
    newEdge->link_loss_db = link_loss * distance;


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
    if (g->name != NULL) free(g->name);
    free(g);
}

GraphList* initGrapList() {

    GraphList* list = malloc(sizeof(GraphList));
    if (list == NULL)
        return NULL;

    list->size = 0;
    list->cap = 0;
    list->arr = NULL;

    list->add = _graphlistAdd;
    list->get = _graphlistGet;
    list->print = _graphlistPrint;
    list->destroy = _graphlistDestroy;

    return list;
}

void _graphlistAdd(GraphList* self, Graph* graph) {

    if (self->arr == NULL) {
        self->arr = calloc(1, sizeof(Graph*));
        self->cap = 1;
    } else if (self->size + 1 > self->cap) {
        self->arr = realloc(self->arr, sizeof(Graph*) * self->cap * 2);
        self->cap *= 2;
    }

    self->arr[self->size] = graph;
    self->size++;
}

Graph* _graphlistGet(GraphList* self, int indice) {
    if (indice < 0 || indice >= self->size) return NULL;
    return self->arr[indice];
}

void _graphlistPrint(GraphList* self) {

    int i = 0;
    if (self->size == 0) {
        printf("\nNo hay mapas cargados.\n");
        return;
    }

    printf("\n--- Listado de Mapas ---\n");
    for (i = 0; i < self->size; i++)
        printf("%d - %s\n", i, self->arr[i]->name);
}

void _graphlistDestroy(GraphList* self) {

    int i = 0;

    for (i = 0; i < self->size; i++){
        graphFree(self->arr[i]);
        self->arr[i] = NULL;
    }
    if (self->arr) free(self->arr);
    free(self);
}
