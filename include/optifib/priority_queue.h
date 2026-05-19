#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE
/*
 * Prototipos para implementacion de Min-Heap
 */

#include <stdbool.h>
#include <optifib/graph.h>


typedef struct _minheap {
    int size;
    int capacity;
    int* pos;
    Node** array;
} MinHeap;


MinHeap* createMinHeap(int capacity, int max_id);
void minHeapify(MinHeap* minHeap, int idx);
Node* extractMin(MinHeap* minHeap);


void decreaseKey(MinHeap* minHeap, int v, double new_loss);
bool isInMinHeap(MinHeap* minHeap, int v);
void freeMinHeap(MinHeap* minHeap);

#endif
