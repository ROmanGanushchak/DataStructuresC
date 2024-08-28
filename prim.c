#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


#define REALLOCK_ADDER 4
#define ll long long
#define uint unsigned int
#define ushort unsigned short
#define uchar unsigned char
#define None 2147483647
#define Chosen 2147483646
#define MaxLong (long long) 9223372036854775807
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define minFieldPercent 0.4

//sort
typedef struct {
    int v1;
    int v2;
} Edge;

typedef struct {
    Edge *arr;
    int top;
    ll cap;
} EdgeStack;

#define cmp(a, b) ((a.v1 > b.v1 || (a.v1 == b.v1 && a.v2 > b.v2)) ? 1 : 0)
void insertSort(Edge *edges, int size) {
    int index;
    Edge temp;

    for (int i=1; i<size; i++) {
        for (index = i; index != 0 && cmp(edges[index-1], edges[index]); index--) {
            temp = edges[index];
            edges[index] = edges[index-1];
            edges[index-1] = temp;
        }
    }
}

void mergeNeighbors(Edge *arr, int edgeIndex, int size) {
    int left = 0, right = edgeIndex, top=0;
    Edge *sorted = (Edge*) malloc (size * sizeof(Edge));

    while (left < edgeIndex && right < size) {
        if (cmp(arr[left], arr[right])) // left > right 
            sorted[top++] = arr[right++];
        else 
            sorted[top++] = arr[left++];
    }

    while (left < edgeIndex) 
        sorted[top++] = arr[left++];
    
    while (right < size) 
        sorted[top++] = arr[right++];
    
    for (int i=0; i<size; i++) 
        arr[i] = sorted[i];
    
    free(sorted);
}

void timSort(Edge *arr, int size) {
    int currentSize = 15;
    for (int i=0; i<size; i+=currentSize) 
        insertSort(arr + i, min(currentSize, size-i));
    
    for (currentSize *= 2; currentSize < size*2; currentSize *= 2) {
        for (int i=0; i<size; i += currentSize) {
            mergeNeighbors(arr + i, min(currentSize/2, size-i), min(currentSize, size-i));
        }
    }
}

// heap
typedef struct {
    int parent;
    int indexInHeap;
    ll weight;
} VertextInfo;

typedef struct {
    int vertex;
} HeapNode;

typedef struct {
    VertextInfo *vertexInfo;
    HeapNode *nodes;
    int top;
    int cap;
} Heap;

Heap createHeap(int vertexCount, int capacity) {
    Heap heap = {.top = 0, .cap = capacity};
    heap.nodes = (HeapNode*) malloc (heap.cap * sizeof(HeapNode));
    heap.vertexInfo = (VertextInfo*) malloc (vertexCount * sizeof(VertextInfo));
    for (int i=0; i<vertexCount; i++) {
        heap.vertexInfo[i].parent = -1;
        heap.vertexInfo[i].weight = MaxLong;
        heap.vertexInfo[i].indexInHeap = None;
    }

    return heap;
}

void freeHeap(Heap heap) {
    free(heap.vertexInfo);
    free(heap.nodes);
}

void swap(Heap heap, int index1, int index2) {
    heap.vertexInfo[heap.nodes[index1].vertex].indexInHeap = index2;
    heap.vertexInfo[heap.nodes[index2].vertex].indexInHeap = index1;

    HeapNode temp = heap.nodes[index1];
    heap.nodes[index1] = heap.nodes[index2];
    heap.nodes[index2] = temp;
}

void balanceRight(Heap heap, int index) {
    ll weight, weight1, weight2;

    while (2*index + 1 < heap.top) {
        weight =  heap.vertexInfo[heap.nodes[index].vertex].weight;
        weight1 = heap.vertexInfo[heap.nodes[2*index+1].vertex].weight;
        weight2 = heap.vertexInfo[heap.nodes[2*index+2].vertex].weight;

        if (2*index + 2 == heap.top) {
            if (weight1 < weight) 
                swap(heap, index, 2*index+1);
            break;
        } 
        else if (weight1 < weight || weight2 < weight){
            if (weight1 < weight2) {
                swap(heap, index, 2*index+1);
                index = 2*index+1;
            } else {
                swap(heap, index, 2*index+2);
                index = 2*index+2;
            }
        } else break;
    }
}

void balanceLeft(Heap heap, int index) {
    while (index != 0 && heap.vertexInfo[heap.nodes[index].vertex].weight < heap.vertexInfo[heap.nodes[(index-1) / 2].vertex].weight) {
        swap(heap, index, (index-1) / 2);
        index = (index - 1) / 2;
    }
}

HeapNode getLeast(Heap *heap) {
    HeapNode res = heap->nodes[0];
    heap->vertexInfo[res.vertex].indexInHeap = None;
    heap->nodes[0] = heap->nodes[--heap->top];

    balanceRight(*heap, 0);

    return res;
}

void decreaseKey(Heap heap, int vertex, int newWeight, int newParent) {
    heap.vertexInfo[vertex].parent = newParent;
    heap.vertexInfo[vertex].weight = newWeight;

    balanceLeft(heap, heap.vertexInfo[vertex].indexInHeap);
}

void addToHeap(Heap *heap, int vertex, ll weight, int parent) {
    if (heap->vertexInfo[vertex].indexInHeap != None) {
        if (weight < heap->vertexInfo[vertex].weight)
            decreaseKey(*heap, vertex, weight, parent);
        return;
    }

    heap->vertexInfo[vertex].parent = parent;
    heap->vertexInfo[vertex].weight = weight;

    heap->vertexInfo[vertex].indexInHeap = heap->top;
    heap->nodes[heap->top].vertex = vertex;

    balanceLeft(*heap, heap->top);

    if (++heap->top == heap->cap) {
        heap->cap += REALLOCK_ADDER;
        heap->nodes = realloc(heap->nodes, heap->cap * sizeof(HeapNode));
    }
}

// graph
typedef struct {
    int vertex;
    ll weight;
} Adj;

typedef struct { // can be only malloced, cant be placed in heap
    int top;
    int cap;
    Adj *arr;
} AdjList;

typedef struct {
    AdjList *adj;
    int vertexCount;
} Graph;

Graph createGraph(int vertexCount) {
    Graph graph = {.vertexCount = vertexCount};
    graph.adj = (AdjList*) malloc (vertexCount * sizeof(AdjList));
    for (int i=0; i<vertexCount; i++) {
        graph.adj[i].top = 0;
        graph.adj[i].cap = REALLOCK_ADDER;
        graph.adj[i].arr = (Adj*) malloc (REALLOCK_ADDER * sizeof(Adj));
    }

    return graph;
}

void freeGraph(Graph *graph) {
    for (int i=0; i<graph->vertexCount; i++) {
        free(graph->adj[i].arr);
    }
    free(graph->adj);
    graph->adj = NULL;
}

EdgeStack findSpaningTree(Graph graph, int start) {
    Heap heap = createHeap(graph.vertexCount, graph.vertexCount);
    addToHeap(&heap, start, 0, -1);

    EdgeStack edges = {.top = 0, .cap = 4};
    edges.arr = (Edge*) malloc (edges.cap * sizeof(Edge));
    ll totalSize = 0;

    int deleted;
    while (heap.top != 0) {
        int deleted = getLeast(&heap).vertex;

        if (heap.vertexInfo[deleted].parent != -1) {
            edges.arr[edges.top].v1 = min(deleted, heap.vertexInfo[deleted].parent);
            edges.arr[edges.top].v2 = max(deleted, heap.vertexInfo[deleted].parent);
            totalSize += heap.vertexInfo[deleted].weight;

            if (++edges.top == edges.cap) {
                edges.cap += REALLOCK_ADDER;
                edges.arr = realloc(edges.arr, edges.cap * sizeof(Edge));
            }
        }

        heap.vertexInfo[deleted].indexInHeap = Chosen;

        for (int i=0; i<graph.adj[deleted].top; i++) {
            int vertex = graph.adj[deleted].arr[i].vertex;
            if (heap.vertexInfo[vertex].indexInHeap != Chosen && graph.adj[deleted].arr[i].weight < heap.vertexInfo[vertex].weight) 
                addToHeap(&heap, vertex, graph.adj[deleted].arr[i].weight, deleted);
        }
    }

    freeHeap(heap);
    timSort(edges.arr, edges.top);
    
    edges.cap = totalSize;
    return edges;
}

char addEdge(Graph graph, int ver1, int ver2, ll weight) {
    if (ver1 == ver2) return 0;

    for (int i=0; i<graph.adj[ver1].top; i++) {
        if (graph.adj[ver1].arr[i].vertex == ver2)
            return 0;
    }

    graph.adj[ver1].arr[graph.adj[ver1].top].vertex = ver2;
    graph.adj[ver1].arr[graph.adj[ver1].top++].weight = weight;

    graph.adj[ver2].arr[graph.adj[ver2].top].vertex = ver1;
    graph.adj[ver2].arr[graph.adj[ver2].top++].weight = weight;

    if (graph.adj[ver1].top == graph.adj[ver1].cap) {
        graph.adj[ver1].cap += REALLOCK_ADDER;
        graph.adj[ver1].arr = realloc(graph.adj[ver1].arr, graph.adj[ver1].cap * sizeof(Adj));
    }

    if (graph.adj[ver2].top == graph.adj[ver2].cap) {
        graph.adj[ver2].cap += REALLOCK_ADDER;
        graph.adj[ver2].arr = realloc(graph.adj[ver2].arr, graph.adj[ver2].cap * sizeof(Adj));
    }

    return 1;
}

char update(Graph graph, int ver1, int ver2, ll weight) {
    if (ver1 == ver2) return 0;
    char wasFound = 0;

    int i;
    for (i=0; i<graph.adj[ver1].top; i++) {
        if (graph.adj[ver1].arr[i].vertex == ver2) {
            wasFound = 1;
            break;
        }
    }

    if (!wasFound || graph.adj[ver1].arr[i].weight + weight < 0) 
        return 0;
    else
        graph.adj[ver1].arr[i].weight += weight;

    for (i=0; i<graph.adj[ver2].top; i++) {
        if (graph.adj[ver2].arr[i].vertex == ver1) 
            break;
    }

    graph.adj[ver2].arr[i].weight += weight;
    return 1;
}

char delete(Graph graph, int ver1, int ver2) {
    if (ver1 == ver2) return 0;
    char wasFound = 0;
    int i;

    for (i=0; i<graph.adj[ver1].top; i++) {
        if (graph.adj[ver1].arr[i].vertex == ver2) {
            wasFound = 1;
            break;
        }
    }
    if (!wasFound) return 0;
    graph.adj[ver1].arr[i] = graph.adj[ver1].arr[--graph.adj[ver1].top];
    // if (graph.adj[ver1].top / (double) graph.adj[ver1].cap < minFieldPercent) {
    //     graph.adj[ver1].cap /= 2;
    //     graph.adj[ver1].arr = realloc(graph.adj[ver1].arr, graph.adj[ver1].cap * sizeof(Adj));
    // }

    for (i=0; i<graph.adj[ver2].top; i++) {
        if (graph.adj[ver2].arr[i].vertex == ver1) 
            break;
    }

    graph.adj[ver2].arr[i] = graph.adj[ver2].arr[--graph.adj[ver2].top];
    // if (1 - graph.adj[ver2].top / (double) graph.adj[ver2].cap < minFieldPercent) {
    //     graph.adj[ver2].cap /= 2;
    //     graph.adj[ver2].arr = realloc(graph.adj[ver1].arr, graph.adj[ver2].cap * sizeof(Adj));
    // }

    return 1;
}

void outputGraph(Graph graph) {
    for (int i=0; i<graph.vertexCount; i++) {
        printf("%d -> ", i);
        for (int j=0; j<graph.adj[i].top; j++) 
            printf("(%d %ld), ", graph.adj[i].arr[j].vertex, graph.adj[i].arr[j].weight);
        printf("\n");
    }
}

int main() {
    int n;
    scanf("%d", &n);

    Graph graph = createGraph(n);

    char type, wasPrinted = 0;
    int v1, v2;
    ll weight; 

    while (scanf(" %c ", &type) != EOF) {
        switch (type) {
            case '(':
                scanf("%d, %d, %lld)", &v1, &v2, &weight);
                if (!addEdge(graph, v1, v2, weight)) {
                    if (wasPrinted)
                        printf("\ninsert %d %d failed", v1, v2);
                    else {
                        printf("insert %d %d failed", v1, v2);
                        wasPrinted = 1;
                    }
                }
                break;

            case 'i':
                scanf("%d %d %lld", &v1, &v2, &weight);

                if (!addEdge(graph, v1, v2, weight)) {
                    if (wasPrinted)
                        printf("\ninsert %d %d failed", v1, v2);
                    else {
                        printf("insert %d %d failed", v1, v2);
                        wasPrinted = 1;
                    }
                }
                break;
            
            case 'u':
                scanf("%d %d %lld", &v1, &v2, &weight);
                if (!update(graph, v1, v2, weight)) {
                    if (wasPrinted)
                        printf("\nupdate %d %d failed", v1, v2);
                    else {
                        printf("update %d %d failed", v1, v2);
                        wasPrinted = 1;
                    }
                }
                break;
            
            case 'd':
                scanf("%d %d", &v1, &v2);
                if (!delete(graph, v1, v2)) {
                    if (wasPrinted)
                        printf("\ndelete %d %d failed", v1, v2);
                    else {
                        printf("delete %d %d failed", v1, v2);
                        wasPrinted = 1;
                    }
                }
                break;
            
            case 's':
                scanf("%d", &v1);
                EdgeStack edges = findSpaningTree(graph, v1);
                if (edges.top == 0) {
                    if (wasPrinted)
                        printf("\nsearch %d failed", v1);
                    else {
                        printf("search %d failed", v1);
                        wasPrinted = 1;
                    }
                } else {
                    if (wasPrinted)
                        printf("\n");
                    else
                        wasPrinted = 1;

                    printf("%lld: [(%d, %d)", edges.cap, edges.arr[0].v1, edges.arr[0].v2);
                    for (int i=1; i<edges.top; i++) {
                        printf(", (%d, %d)", edges.arr[i].v1, edges.arr[i].v2);
                    }
                    printf("]");
                }
                free(edges.arr);
                break;
            
            case 'o':
                outputGraph(graph);
                break;
        }
    }

    freeGraph(&graph);
}