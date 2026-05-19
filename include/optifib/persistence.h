#ifndef PERSISTENCE_H
#define PERSISTENCE_H
/*
 * Estructuras y prototipos para persistencia
 */

#include <optifib/common.h>
#include <optifib/graph.h>

struct Map;

typedef struct Map MapReader;
typedef struct Map MapWriter;

typedef enum {
    MODE_PARSER,
    MODE_SERIALIZER,
} MapMode;

typedef enum {
    MAP_STATUS_OP_OK,
    MAP_STATUS_OP_ERR,
    MAP_STATUS_OP_PROHIBITED,
    MAP_STATUS_IO_ERR
} MapStatusOp;

struct Map* loadMap(const char* name, MapMode mode);

Graph* loadGraph(const char* name);
MapStatusOp saveGraph(Graph* g);

MapStatusOp readNode(MapReader* r, Node* n);
MapStatusOp readEdge(MapReader* r, Edge* e);

MapStatusOp writeNode(MapWriter* w, Node* n);
MapStatusOp writeEdge(MapWriter* w, Edge* e);

void closeMap(struct Map* m);

MapStatusOp exportReport(Graph* g, int originId, int destId, double totalLoss, double powerOut, double powerIn, double sensitivity);

#endif
