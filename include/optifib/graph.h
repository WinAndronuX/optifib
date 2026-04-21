#ifndef GRAPH_H
#define GRAPH_H
/*
 * Estructuras Node, Edge y prototipos para grafos
 */

#include <optifib/common.h>


typedef struct _edge {
    int target_id;
    FiberDeployment type;
    double distance_km;
    double link_loss_db;
    struct _edge* next;
} Edge;

typedef struct _node {
    int id;
    char description[50];
    NodeType type;
    double intrinsic_loss_db;

    Edge* adj_list;
    double min_accumulated_loss;
    int previous_node_id;
    int visited;
} Node;

#endif
