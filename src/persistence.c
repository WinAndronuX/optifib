/*
 * Gestion de persistencia
 */

#include <optifib/persistence.h>
#include <stdio.h>


struct Map {
    const char* name; // prefix
    FILE* nodes; // prefix_nodes.csv
    FILE* edges; // prefix_edges.csv
};

struct Map* loadMap(const char* name, MapMode mode) {

    if (sizeof(name) > 39 * sizeof(char))
        return NULL;

    char nodesFilename[50], edgesFilename[50];

    snprintf(nodesFilename, sizeof(nodesFilename), "%s_nodes.csv", name);
    snprintf(edgesFilename, sizeof(edgesFilename), "%s_edges.csv", name);

    FILE* fnodes = fopen(nodesFilename, (mode == MODE_PARSER)? "r" : "w");
    if (fnodes == NULL)
        return NULL;

    FILE* fedges = fopen(edgesFilename, (mode == MODE_PARSER)? "r" : "w");
    if (fedges == NULL)
        return NULL;

    struct Map* m = {};

    m->name = name;
    m->nodes = fnodes;
    m->edges = fedges;

    return m;
}

MapStatusOp readNode(MapReader* r, Node* n) {


    return MAP_STATUS_OP_OK;
}

MapStatusOp readEdge(MapReader* r, Edge* e) {


    return MAP_STATUS_OP_OK;
}

MapStatusOp writeNode(MapWriter* w, Node* n) {


    return MAP_STATUS_OP_OK;
}

MapStatusOp writeEdge(MapWriter* w, Edge* e) {


    return MAP_STATUS_OP_OK;
}


void closeMap(struct Map* m) {
    fclose(m->nodes);
    fclose(m->edges);
}
