/*
 * Gestion de persistencia
 */

#include <optifib/common.h>
#include <optifib/persistence.h>
#include <stdio.h>
#include <string.h>


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

    int id;
    char type[20], desc[50];
    double atenuation_db;

    int i = fscanf(r->nodes, "%d,%s,%s,%lf\n", &id, type, desc, &atenuation_db);

    if (i == EOF)
        return MAP_STATUS_IO_ERR;
    else if (i != 4)
        return MAP_STATUS_OP_ERR;

    n->id = id;

    if (strcmp(type, "POLE") == 0)
        n->type = NODE_POLE;
    else if (strcmp(type, "MANHOLE") == 0)
        n->type = NODE_MANHOLE;
    else if (strcmp(type, "SPLICE_ENCLOSURE") == 0)
        n->type = NODE_SPLICE_ENCLOSURE;
    else if (strcmp(type, "DISTRIBUTION_HUB") == 0)
        n->type = NODE_DISTRIBUTION_HUB;
    else if (strcmp(type, "OLT") == 0)
        n->type = NODE_OLT;
    else
        return MAP_STATUS_OP_ERR;

    strcpy(n->description, desc);
    n->intrinsic_loss_db = atenuation_db;

    return MAP_STATUS_OP_OK;
}

MapStatusOp readEdge(MapReader* r, Edge* e) {

    int source_id, target_id;
    char type[20];
    double distance_km;

    int i = fscanf(r->edges, "%d,%d,%s,%lf\n", &source_id, &target_id, type, &distance_km);

    if (i == EOF)
        return MAP_STATUS_IO_ERR;
    else if (i != 4)
        return MAP_STATUS_OP_ERR;

    e->source_id = source_id;
    e->target_id = target_id;

    if (strcmp(type, "AERIAL") == 0)
        e->type = FIBER_AERIAL;
    else if (strcmp(type, "UNDERGROUND"))
        e->type = FIBER_UNDERGROUND;
    else
        return MAP_STATUS_OP_ERR;

    e->distance_km = distance_km;

    return MAP_STATUS_OP_OK;
}

MapStatusOp writeNode(MapWriter* w, Node* n) {
    int i = fprintf(w->nodes, "%d,%s,%s,%lf\n", n->id, NodeTypeStr[n->type], n->description, n->intrinsic_loss_db);

    return (i == 4)? MAP_STATUS_OP_OK : MAP_STATUS_IO_ERR;
}

MapStatusOp writeEdge(MapWriter* w, Edge* e) {
    int i = fprintf(w->edges, "%d,%d,%s,%lf\n", e->source_id, e->target_id, FiberDeploymentStr[e->type], e->distance_km);

    return (i == 4)? MAP_STATUS_OP_OK : MAP_STATUS_IO_ERR;
}


void closeMap(struct Map* m) {
    fclose(m->nodes);
    fclose(m->edges);
}
