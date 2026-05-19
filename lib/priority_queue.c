/*
 * Implementacion manual del Min-Heap
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "optifib/priority_queue.h"

void swapMinHeapNode(MinHeap* minHeap, int idx1, int idx2) {

    Node* temp = minHeap->array[idx1];
    minHeap->array[idx1] = minHeap->array[idx2];
    minHeap->array[idx2] = temp;


    minHeap->pos[minHeap->array[idx1]->id] = idx1;
    minHeap->pos[minHeap->array[idx2]->id] = idx2;
}


MinHeap* createMinHeap(int capacity, int max_id) {
    MinHeap* minHeap = malloc(sizeof(MinHeap));
    if (!minHeap) return NULL;

    minHeap->size = 0;
    minHeap->capacity = capacity;
    // El arreglo pos debe ser lo suficientemente grande para contener el ID maximo + 1
    minHeap->pos = (int*) malloc((max_id + 1) * sizeof(int));
    minHeap->array = (Node**) malloc(capacity * sizeof(Node*));

    return minHeap;
}


void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;


    if (left < minHeap->size &&
        minHeap->array[left]->min_accumulated_loss < minHeap->array[smallest]->min_accumulated_loss) {
        smallest = left;
        }


    if (right < minHeap->size &&
        minHeap->array[right]->min_accumulated_loss < minHeap->array[smallest]->min_accumulated_loss) {
        smallest = right;
        }


    if (smallest != idx) {
        swapMinHeapNode(minHeap, smallest, idx);
        minHeapify(minHeap, smallest);
    }
}

Node* extractMin(MinHeap* minHeap) {
    if (minHeap->size == 0) return NULL;

    Node* root = minHeap->array[0];


    Node* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root->id] = minHeap->capacity;
    minHeap->pos[lastNode->id] = 0;


    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(MinHeap* minHeap, int v, double new_loss) {
    int i = minHeap->pos[v];

    minHeap->array[i]->min_accumulated_loss = new_loss;

    while (i && minHeap->array[i]->min_accumulated_loss < minHeap->array[(i - 1) / 2]->min_accumulated_loss) {
        swapMinHeapNode(minHeap, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

bool isInMinHeap(MinHeap* minHeap, int v) {

    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}

void freeMinHeap(MinHeap* minHeap) {
    if (!minHeap) return;
    if (minHeap->pos) free(minHeap->pos);
    if (minHeap->array) free(minHeap->array);
    free(minHeap);
}