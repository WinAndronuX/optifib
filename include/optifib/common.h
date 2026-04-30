#ifndef COMMON_H
#define COMMON_H
/*
 * Archivo header para constantes y enums
 */

typedef enum {        // Atenuacion:
    FIBER_AERIAL,     // 0.35 dB/km
    FIBER_UNDERGROUND // 0.40 dB/km
} FiberDeployment;

extern const char* FiberDeploymentStr[];

typedef enum {                   // ATENUACION (Si el splitter es origen es 0.0)
    NODE_POLE,                   // 0.0 dB
    NODE_MANHOLE,                // 0.0 dB
    NODE_SPLICE_ENCLOSURE,       // 0.1 dB
    NODE_DISTRIBUTION_HUB,       // 0.5 dB
    NODE_OLT,                    // Origen
    NODE_SPLITTER_1_2,           // 3.5 dB
    NODE_SPLITTER_1_8,           // 10.5 dB
    NODE_SPLITTER_1_16           // 13.8 dB
} NodeType;

extern const char* NodeTypeStr[];

#endif
