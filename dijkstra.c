#include <stdio.h>
#include <stdlib.h>

#define DebugPrint ((int) 0)
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define isLessNode(a, b) (((a.value) < (b.value)) ? (1) : (((a.value == b.value) && (a.length < b.length)) ? 1 : 0))

typedef struct node {
    int id;
    int value;
    int length;
} Node;

typedef struct queue {
    Node *tree;
    int top;
    int capacity;
} StackPriorityQueue;

Node getLeast(StackPriorityQueue *queue) {
    Node rez = queue->tree[0], temp;

    queue->tree[0] = queue->tree[--queue->top];
    int index = 0;
    while (2*index + 1 < queue->top) {
        if (2*index + 1 == queue->top - 1) {
            if (queue->tree[2*index+1].value < queue->tree[index].value) {
                temp = queue->tree[index];
                queue->tree[index] = queue->tree[2*index+1];
                queue->tree[2*index+1] = temp;
            }
            break;
        }

        if (isLessNode(queue->tree[2*index+1], queue->tree[index]) || isLessNode(queue->tree[2*index+2], queue->tree[index])) {
            if (isLessNode(queue->tree[2*index+1], queue->tree[2*index+2])) {
                temp = queue->tree[index];
                queue->tree[index] = queue->tree[2*index+1];
                queue->tree[2*index+1] = temp;
                index = 2*index + 1;
            } else {
                temp = queue->tree[index];
                queue->tree[index] = queue->tree[2*index+2];
                queue->tree[2*index+2] = temp;
                index = 2*index + 2;
            }
        } else break;
    }

    return rez;
}

void addToStackQueue(StackPriorityQueue *queue, Node node) {
    Node temp;
    int index = queue->top, parentIndex = (queue->top - 1) / 2;
    queue->tree[queue->top++] = node;

    while (parentIndex != -1 && isLessNode(queue->tree[index], queue->tree[parentIndex])) { // queue->tree[parentIndex].value > queue->tree[index].value
        temp = queue->tree[index];
        queue->tree[index] = queue->tree[parentIndex];
        queue->tree[parentIndex] = temp;

        index = parentIndex;
        parentIndex = (parentIndex - 1) / 2;
    }
}

typedef struct data {
    int id;
    int weight;
} Data;

typedef struct stack {
    int *arr;
    int top;
    int cap;
} Stack;

typedef struct datastack {
    Data *arr;
    int top;
    int cap;
} DataStack;

typedef struct graph {
    DataStack *adj;
    int nodeCount;

    StackPriorityQueue *queue;
    int *distances;
    int *parents;
} Graph;

void initializeGraph(Graph *graph, int nodeCount) {
    graph->nodeCount = nodeCount;
    graph->adj = (DataStack*) malloc (nodeCount * sizeof(DataStack));

    graph->queue = (StackPriorityQueue*) malloc (sizeof(StackPriorityQueue));
    graph->queue->capacity = nodeCount;
    graph->queue->top = 0;
    graph->queue->tree = (Node*) malloc (2 * graph->queue->capacity * sizeof(Node));

    for (int i=0; i<nodeCount; i++) {
        graph->adj[i].cap = 4;
        graph->adj[i].arr = (Data*) malloc (graph->adj[i].cap * sizeof(Data));
        graph->adj[i].top = 0;
    }

    graph->distances = (int*) malloc (nodeCount * sizeof(int));
    graph->parents = (int*) malloc (nodeCount * sizeof(int));
}

void outputGraph(Graph graph) {
    for (int i=0; i<graph.nodeCount; i++) {
        printf("%d %d -> ", i, graph.adj[i].top);
        for (int index=0; index<graph.adj[i].top; index++) {
            printf("(%d %d) ", graph.adj[i].arr[index].id, graph.adj[i].arr[index].weight);
        }printf("\n");
    }
}

int dijkstraDistances(Graph graph, int start, int end) {
    for (int i=0; i<graph.nodeCount; i++) {
        graph.distances[i] = -1;
        graph.parents[i] = -1;
    }
    
    graph.distances[start] = 0;
    StackPriorityQueue queue = *graph.queue;
    queue.tree[0].id = start;
    queue.tree[0].value = 0;
    queue.tree[0].length = 0;
    queue.top = 1;

    while (queue.top != 0) {
        Node deleted = getLeast(&queue);
        if (deleted.id == end)
            return deleted.value;
        
        if (graph.distances[deleted.id] != -1 && graph.distances[deleted.id] < deleted.value)
            continue;

        graph.distances[deleted.id] = deleted.value;
        for (int i = 0; i < graph.adj[deleted.id].top; i++) {
            Node elem = {.id = graph.adj[deleted.id].arr[i].id, .value = graph.adj[deleted.id].arr[i].weight};

            if (graph.distances[elem.id] == -1 || graph.distances[elem.id] > deleted.value + elem.value) {
                graph.distances[elem.id] = deleted.value + elem.value;
                graph.parents[elem.id] = deleted.id;
                elem.value = deleted.value + elem.value;
                elem.length = deleted.length + 1;
                addToStackQueue(&queue, elem);
            }
        }
    }

    return -1;
}

char addEdge(Graph graph, int ver1, int ver2, int weight) {
    if (ver1 == ver2) return 0;

    for (int i=0; i<graph.adj[ver1].top; i++) {
        if (graph.adj[ver1].arr[i].id == ver2)
            return 0;
    }

    graph.adj[ver1].arr[graph.adj[ver1].top].id = ver2;
    graph.adj[ver1].arr[graph.adj[ver1].top++].weight = weight;

    graph.adj[ver2].arr[graph.adj[ver2].top].id = ver1;
    graph.adj[ver2].arr[graph.adj[ver2].top++].weight = weight;

    if (graph.adj[ver1].top == graph.adj[ver1].cap) {
        graph.adj[ver1].cap += 4;
        graph.adj[ver1].arr = realloc(graph.adj[ver1].arr, graph.adj[ver1].cap * sizeof(Data));
    }

    if (graph.adj[ver2].top == graph.adj[ver2].cap) {
        graph.adj[ver2].cap += 4;
        graph.adj[ver2].arr = realloc(graph.adj[ver2].arr, graph.adj[ver2].cap * sizeof(Data));
    }

    return 1;
}

char update(Graph graph, int ver1, int ver2, int weight) {
    if (ver1 == ver2) return 0;
    char wasFound = 0;

    int i;
    for (i=0; i<graph.adj[ver1].top; i++) {
        if (graph.adj[ver1].arr[i].id == ver2) {
            wasFound = 1;
            break;
        }
    }

    if (!wasFound || graph.adj[ver1].arr[i].weight + weight < 0) 
        return 0;
    else
        graph.adj[ver1].arr[i].weight += weight;

    wasFound = 0;
    for (i=0; i<graph.adj[ver2].top; i++) {
        if (graph.adj[ver2].arr[i].id == ver1) {
            wasFound = 1;
            break;
        }
    }

    if (!wasFound) {
        printf("In update no second\n");
        exit(1);
    }

    graph.adj[ver2].arr[i].weight += weight;

    return 1;
}

char delete(Graph graph, int ver1, int ver2) {
    if (ver1 == ver2) return 0;
    char wasFound = 0;
    int i;

    for (i=0; i<graph.adj[ver1].top; i++) {
        if (graph.adj[ver1].arr[i].id == ver2) {
            wasFound = 1;
            break;
        }
    }
    if (!wasFound) return 0;

    for (--graph.adj[ver1].top; i<graph.adj[ver1].top; i++) 
        graph.adj[ver1].arr[i] = graph.adj[ver1].arr[i+1];

    wasFound = 0;
    for (i=0; i<graph.adj[ver2].top; i++) {
        if (graph.adj[ver2].arr[i].id == ver1) {
            wasFound = 1;
            break;
        }
    }
    
    if (!wasFound) {
        printf("In delete no second\n");
        exit(1);
    }

    for (--graph.adj[ver2].top; i<graph.adj[ver2].top; i++) 
        graph.adj[ver2].arr[i] = graph.adj[ver2].arr[i+1];

    return 1;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    getchar();

    Graph graph;
    initializeGraph(&graph, n);

    Stack path = {.cap = 5, .top = 0};
    path.arr = (int*) malloc (path.cap * sizeof(int));

    char wasPrinted=0;
    int ver1, ver2, weight;
    for (int i=0; i<m; i++) {
        scanf(" (%d, %d, %d)", &ver1, &ver2, &weight);
        if (!addEdge(graph, ver1, ver2, weight)) {
            if (wasPrinted)
                printf("\ninsert %d %d failed", ver1, ver2);
            else {
                printf("insert %d %d failed", ver1, ver2);
                wasPrinted = 1;
            }
        }
    }

    char type;
    while (scanf("%c ", &type) != EOF) {
        // printf("Next\n");
        switch (type) {
            case 'i':
                scanf("%d %d %d", &ver1, &ver2, &weight);
                // getchar();
                if (DebugPrint)
                    printf("i %d %d %d\n", ver1, ver2, weight);
                
                if (!addEdge(graph, ver1, ver2, weight)) {
                    if (wasPrinted)
                        printf("\ninsert %d %d failed", ver1, ver2);
                    else {
                        printf("insert %d %d failed", ver1, ver2);
                        wasPrinted = 1;
                    }
                }
                if (DebugPrint)
                    printf("i finished\n");
                break;
            
            case 'u':
                scanf("%d %d %d", &ver1, &ver2, &weight);
                // getchar();
                if (DebugPrint)
                    printf("u %d %d %d\n", ver1, ver2, weight);
                if (!update(graph, ver1, ver2, weight)) {
                    if (wasPrinted)
                        printf("\nupdate %d %d failed", ver1, ver2);
                    else {
                        printf("update %d %d failed", ver1, ver2);
                        wasPrinted = 1;
                    }
                }
                if (DebugPrint)
                    printf("u finished\n");
                break;
            
            case 'd':
                scanf("%d %d", &ver1, &ver2);
                // getchar();
                if (DebugPrint)
                    printf("d %d %d\n", ver1, ver2);
                if (!delete(graph, ver1, ver2)) {
                    if (wasPrinted)
                        printf("\ndelete %d %d failed", ver1, ver2);
                    else {
                        printf("delete %d %d failed", ver1, ver2);
                        wasPrinted = 1;
                    }
                }
                if (DebugPrint)
                    printf("d finished\n");
                break;
            
            case 's':
                scanf("%d %d", &ver1, &ver2);
                // getchar();
                if (DebugPrint)
                    printf("s %d %d\n", ver1, ver2);

                int distance = dijkstraDistances(graph, ver1, ver2);
                if (distance == -1 || ver1 == ver2) {
                    if (wasPrinted)
                        printf("\nsearch %d %d failed", ver1, ver2);
                    else {
                        printf("search %d %d failed", ver1, ver2);
                        wasPrinted = 1;
                    }
                } else {
                    if (wasPrinted)
                        printf("\n");
                    else 
                        wasPrinted = 1;
                    
                    path.top = 0;
                    int current = ver2;
                    while (current != ver1) {
                        path.arr[path.top++] = current;
                        current = graph.parents[current];

                        if (path.top == path.cap) {
                            path.cap += 4;
                            path.arr = realloc(path.arr, path.cap * sizeof(int));
                        }
                    }
                    
                    printf("%d: [%d", distance, ver1);
                    for (int i=path.top-1; i>=0; i--)
                        printf(", %d", path.arr[i]);
                    printf("]");
                }
                if (DebugPrint)
                    printf("s finished\n");
                break;
            
            case 'o':
                outputGraph(graph);
        }
        // printf("Switch finished\n");
    }

    free(graph.distances);
    free(graph.queue->tree);
    for (int i=0; i<graph.nodeCount; i++)
        free(graph.adj[i].arr);
    free(graph.adj);
}