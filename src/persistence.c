/*
 * Gestion de persistencia
 */

#include <linux/limits.h>
#include <optifib/common.h>
#include <optifib/persistence.h>
#include <optifib/utils.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct Map {
    const char* name; // prefix
    FILE* nodes; // prefix_nodes.csv
    FILE* edges; // prefix_edges.csv
};

struct Map* loadMap(const char* name, MapMode mode) {
    if (strlen(name) > 39)
        return NULL;

    char nodesFilename[64], edgesFilename[64];
    snprintf(nodesFilename, sizeof(nodesFilename), "mapdata%s%s_nodes.csv", PATH_DIV, name);
    snprintf(edgesFilename, sizeof(edgesFilename), "mapdata%s%s_edges.csv", PATH_DIV, name);

    FILE* fnodes = fopen(nodesFilename, (mode == MODE_PARSER) ? "r" : "w");
    if (fnodes == NULL)
        return NULL;

    FILE* fedges = fopen(edgesFilename, (mode == MODE_PARSER) ? "r" : "w");
    if (fedges == NULL) {
        fclose(fnodes);
        return NULL;
    }

    struct Map* m = (struct Map*)malloc(sizeof(struct Map));
    if (m == NULL) {
        fclose(fnodes);
        fclose(fedges);
        return NULL;
    }

    m->name = name;
    m->nodes = fnodes;
    m->edges = fedges;

    return m;
}

MapStatusOp readNode(MapReader* r, Node* n) {
    int id;
    char type[32], desc[64];
    double atenuation_db;

    // Usar %[^,] para leer cadenas hasta la coma
    int i = fscanf(r->nodes, "%d,%[^,],%[^,],%lf\n", &id, type, desc, &atenuation_db);

    if (i == EOF)
        return MAP_STATUS_IO_ERR;
    else if (i != 4)
        return MAP_STATUS_OP_ERR;

    n->id = id;

    if (strcmp(type, "POLE") == 0)
        n->type = NODE_POLE;
    else if (strcmp(type, "MANHOLE") == 0)
        n->type = NODE_MANHOLE;
    else if (strcmp(type, "OLT") == 0)
        n->type = NODE_OLT;
    else
        return MAP_STATUS_OP_ERR;

    n->civil_type = n->type; // Sincronizar con el tipo civil cargado
    strncpy(n->description, desc, 49);
    n->description[49] = '\0';
    n->intrinsic_loss_db = atenuation_db;

    return MAP_STATUS_OP_OK;
}

MapStatusOp readEdge(MapReader* r, Edge* e) {
    int source_id, target_id;
    char type[32];
    double distance_km;

    int i = fscanf(r->edges, "%d,%d,%[^,],%lf\n", &source_id, &target_id, type, &distance_km);

    if (i == EOF)
        return MAP_STATUS_IO_ERR;
    else if (i != 4)
        return MAP_STATUS_OP_ERR;

    e->source_id = source_id;
    e->target_id = target_id;

    if (strcmp(type, "AERIAL") == 0)
        e->type = FIBER_AERIAL;
    else if (strcmp(type, "UNDERGROUND") == 0)
        e->type = FIBER_UNDERGROUND;
    else
        return MAP_STATUS_OP_ERR;

    e->distance_km = distance_km;

    return MAP_STATUS_OP_OK;
}

MapStatusOp writeNode(MapWriter* w, Node* n) {
    // Se guarda el tipo civil original, no el equipo instalado en memoria
    int i = fprintf(w->nodes, "%d,%s,%s,%.2f\n", n->id, NodeTypeStr[n->civil_type], n->description, 0.0);
    return (i > 0) ? MAP_STATUS_OP_OK : MAP_STATUS_IO_ERR;
}

MapStatusOp writeEdge(MapWriter* w, Edge* e) {
    int i = fprintf(w->edges, "%d,%d,%s,%lf\n", e->source_id, e->target_id, FiberDeploymentStr[e->type], e->distance_km);
    return (i > 0) ? MAP_STATUS_OP_OK : MAP_STATUS_IO_ERR;
}

void closeMap(struct Map* m) {
    if (m == NULL) return;
    if (m->nodes) fclose(m->nodes);
    if (m->edges) fclose(m->edges);
    free(m);
}
