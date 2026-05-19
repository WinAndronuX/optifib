#ifndef DIJKSTRA_H
#define DIJKSTRA_H
/*
 * Estructuras y prototipos para la implementacion del Algoritmo de Dijkstra
 */

#include <limits.h>
#include <optifib/graph.h>


#define INF INT_MAX

extern double current_olt_power_dbm;
extern double current_onu_sensitivity_dbm;

void dijkstra(Graph* graph, int src);

#endif
