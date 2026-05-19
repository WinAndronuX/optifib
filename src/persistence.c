/*
 * Gestion de persistencia
 */

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

    char nodesFilename[128], edgesFilename[128];
    const char* paths[] = { "mapdata", "../mapdata", "../../mapdata" };
    FILE *fnodes = NULL, *fedges = NULL;
    int path_idx = 0;

    for (path_idx = 0; path_idx < 3; path_idx++) {
        snprintf(nodesFilename, sizeof(nodesFilename), "%s%s%s_nodes.csv", paths[path_idx], PATH_DIV, name);
        snprintf(edgesFilename, sizeof(edgesFilename), "%s%s%s_edges.csv", paths[path_idx], PATH_DIV, name);

        fnodes = fopen(nodesFilename, (mode == MODE_PARSER) ? "r" : "w");
        if (fnodes != NULL) {
            fedges = fopen(edgesFilename, (mode == MODE_PARSER) ? "r" : "w");
            if (fedges != NULL) break; // Encontramos ambos archivos
            fclose(fnodes);
            fnodes = NULL;
        }
    }

    if (fnodes == NULL || fedges == NULL) {
        printf("\n|ERROR| No se pudieron abrir los archivos para '%s'.\n", name);
        printf("Ruta intentada: %s\n", nodesFilename);
        if (mode == MODE_PARSER) perror("Motivo del fallo");
        return NULL;
    }

    struct Map* m = (struct Map*)malloc(sizeof(struct Map));
    if (m == NULL) {
        fclose(fnodes);
        fclose(fedges);
        return NULL;
    }

    m->name = strdup(name);
    m->nodes = fnodes;
    m->edges = fedges;

    return m;
}

MapStatusOp readNode(MapReader* r, Node* n) {
    int id;
    char type_civil[32], type_equip[32], desc[64];
    double atenuation_db;

    int i = fscanf(r->nodes, "%d,%[^,],%[^,],%[^,],%lf\n", &id, type_civil, type_equip, desc, &atenuation_db);

    if (i == EOF)
        return MAP_STATUS_IO_ERR;
    else if (i != 5)
        return MAP_STATUS_OP_ERR;

    n->id = id;

    if (strcmp(type_civil, "POLE") == 0) n->civil_type = NODE_POLE;
    else if (strcmp(type_civil, "MANHOLE") == 0) n->civil_type = NODE_MANHOLE;
    else if (strcmp(type_civil, "OLT") == 0) n->civil_type = NODE_OLT;
    else n->civil_type = NODE_POLE;

    // Mapear tipo equipo
    if (strcmp(type_equip, "POLE") == 0) n->type = NODE_POLE;
    else if (strcmp(type_equip, "MANHOLE") == 0) n->type = NODE_MANHOLE;
    else if (strcmp(type_equip, "OLT") == 0) n->type = NODE_OLT;
    else if (strcmp(type_equip, "SPLICE_ENCLOSURE") == 0) n->type = NODE_SPLICE_ENCLOSURE;
    else if (strcmp(type_equip, "CONNECTION") == 0) n->type = NODE_CONNECTION;
    else if (strcmp(type_equip, "DISTRIBUTION_HUB") == 0) n->type = NODE_DISTRIBUTION_HUB;
    else if (strcmp(type_equip, "SPLITTER_1_2") == 0) n->type = NODE_SPLITTER_1_2;
    else if (strcmp(type_equip, "SPLITTER_1_8") == 0) n->type = NODE_SPLITTER_1_8;
    else if (strcmp(type_equip, "SPLITTER_1_16") == 0) n->type = NODE_SPLITTER_1_16;
    else n->type = n->civil_type;

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
    int i = fprintf(w->nodes, "%d,%s,%s,%s,%.2f\n", 
        n->id, 
        NodeTypeStr[n->civil_type], 
        NodeTypeStr[n->type], 
        n->description, 
        n->intrinsic_loss_db);
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

Graph* loadGraph(const char* name) {
    struct Map* m = loadMap(name, MODE_PARSER);
    if (m == NULL) return NULL;

    Graph* g = graphCreate(name);
    if (g == NULL) {
        closeMap(m);
        return NULL;
    }

    Node tempNode;
    while (readNode(m, &tempNode) == MAP_STATUS_OP_OK) {
        nodeAdd(g, tempNode.id, tempNode.description, tempNode.civil_type, tempNode.type, tempNode.intrinsic_loss_db);
    }

    Edge tempEdge;
    while (readEdge(m, &tempEdge) == MAP_STATUS_OP_OK) {
        Node* src = nodeFind(g, tempEdge.source_id);
        Node* dest = nodeFind(g, tempEdge.target_id);

        if (src && dest) {
            double coef = (tempEdge.type == FIBER_AERIAL) ? 0.35 : 0.40;
            edgeAdd(src, tempEdge.target_id, tempEdge.type, tempEdge.distance_km, coef);
            edgeAdd(dest, tempEdge.source_id, tempEdge.type, tempEdge.distance_km, coef);
        }
    }

    closeMap(m);
    return g;
}

MapStatusOp saveGraph(Graph* g) {
    if (g == NULL) return MAP_STATUS_OP_ERR;

    struct Map* m = loadMap(g->name, MODE_SERIALIZER);
    if (m == NULL) return MAP_STATUS_IO_ERR;

    for (int i = 0; i < g->V; i++) {
        writeNode(m, g->arr[i].nodes);
    }

    for (int i = 0; i < g->V; i++) {
        Node* n = g->arr[i].nodes;
        Edge* e = n->adj_list;
        while (e != NULL) {
            if (e->source_id < e->target_id) {
                writeEdge(m, e);
            }
            e = e->next;
        }
    }

    closeMap(m);
    return MAP_STATUS_OP_OK;
}

MapStatusOp exportReport(Graph* g, int originId, int destId, double totalLoss, double powerOut, double powerIn, double sensitivity) {
    if (g == NULL) return MAP_STATUS_OP_ERR;

    char filename[128];
    snprintf(filename, sizeof(filename), "reports/reporte_%s_dest%d.txt", g->name, destId);

    FILE* f = fopen(filename, "w");
    
    if (f == NULL) {
        #ifdef _WIN32
            system("mkdir reports 2>nul");
        #else
            system("mkdir -p reports");
        #endif
        f = fopen(filename, "w");
    }

    if (f == NULL) {
        
        snprintf(filename, sizeof(filename), "reporte_%s_dest%d.txt", g->name, destId);
        f = fopen(filename, "w");
    }

    if (f == NULL) return MAP_STATUS_IO_ERR;

    fprintf(f, "====================================================\n");
    fprintf(f, "          REPORTE DE SIMULACION OPTIFIB             \n");
    fprintf(f, "====================================================\n\n");
    fprintf(f, "Mapa: %s\n", g->name);
    fprintf(f, "Origen (OLT) ID: %d\n", originId);
    fprintf(f, "Destino (ONU) ID: %d\n\n", destId);

    fprintf(f, "--- RESULTADOS DE POTENCIA ---\n");
    fprintf(f, "Potencia de Salida (OLT):    %.2f dBm\n", powerOut);
    fprintf(f, "Perdida Total Calculada:     %.2f dB\n", totalLoss);
    fprintf(f, "Potencia en el Destino:      %.2f dBm\n", powerIn);
    fprintf(f, "Umbral de Sensibilidad (ONU):%.2f dBm\n", sensitivity);
    fprintf(f, "ESTADO: %s\n\n", (powerIn >= sensitivity) ? "VIABLE" : "NO VIABLE / SENAL CRITICA");

    fprintf(f, "--- DETALLE DE NODOS ---\n");
    fprintf(f, "| %-4s | %-20s | %-12s |\n", "ID", "TIPO", "PERDIDA (dB)");
    fprintf(f, "----------------------------------------------------\n");
    for (int i = 0; i < g->V; i++) {
        Node* n = g->arr[i].nodes;
        fprintf(f, "| %-4d | %-20s | %12.2f |\n", n->id, NodeTypeStr[n->type], n->intrinsic_loss_db);
    }

    fprintf(f, "\nReporte generado por OptiFib v0.0.1\n");
    fclose(f);

    printf("\n[INFO] Reporte exportado exitosamente a '%s'\n", filename);
    return MAP_STATUS_OP_OK;
}
