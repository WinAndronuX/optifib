#ifndef GRAPH_H
#define GRAPH_H
/*
 * Estructuras Node, Edge y prototipos para grafos
 */

#include <optifib/common.h>


typedef struct _edge {
    int target_id;
    int source_id;
    FiberDeployment type;
    double distance_km;
    double link_loss_db;
    struct _edge* next;
} Edge;

typedef struct _node {
    int id;
    char description[50];
    NodeType type;
    NodeType civil_type;
    double intrinsic_loss_db;

    Edge* adj_list;
    double min_accumulated_loss;
    int previous_node_id;
    int visited;
} Node;

typedef struct _adjlist {
    Node* nodes;
} Adjlist;

typedef struct _graph {
    int V;
    Adjlist* arr;
    char* name;
} Graph;

typedef struct _graphlist {
    Graph** arr;
    int size;
    int cap;
    void (*add)(struct _graphlist* self, Graph* graph);
    Graph* (*get)(struct _graphlist* self, int indice);
    void (*print)(struct _graphlist* self);
    void (*destroy)(struct _graphlist* self);
} GraphList;

Graph* graphCreate(const char* name);
void graphFree(Graph* g);
void graphPrint(Graph* g);

void nodeAdd(Graph* g, int id, const char* description, NodeType type, double intrinsic_loss);
void edgeAdd(Node* source, int target_id, FiberDeployment type, double distance, double link_loss);

Node* nodeFind(Graph* g, int id);

GraphList* initGrapList();
void _graphlistAdd(GraphList* self, Graph* graph);
Graph* _graphlistGet(GraphList* self, int indice);
void _graphlistPrint(GraphList* self);
void _graphlistDestroy(GraphList* self);

#endif
