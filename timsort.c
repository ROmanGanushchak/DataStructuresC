#include <stdlib.h>
#include <stdio.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define cmp(a, b) ((a < b) ? 1 : 0)
void insertSort(int *edges, int size) {
    int index, temp;

    for (int i=1; i<size; i++) {
        for (index = i; index != 0 && cmp(edges[index-1], edges[index]); index--) {
            temp = edges[index];
            edges[index] = edges[index-1];
            edges[index-1] = temp;
        }
    }
}

void mergeNeighbors(int *arr, int edgeIndex, int size) {
    int left = 0, right = edgeIndex, top=0;
    int *sorted = (int*) malloc (size * sizeof(int));

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

void timSort(int *arr, int size) {
    int currentSize = 15;
    for (int i=0; i<size; i+=currentSize) 
        insertSort(arr + i, min(currentSize, size-i));
    
    for (currentSize *= 2; currentSize < size*2; currentSize *= 2) {
        for (int i=0; i<size; i += currentSize) {
            mergeNeighbors(arr + i, min(currentSize/2, size-i), min(currentSize, size-i));
        }
    }
}

int main() {
    int arr[] = {3, 2, 1, 7, 4, 5, 3, 5, 6, 3};
    timSort(arr, 10);
    for (int i=0; i<10; i++) {
        printf("%d ", arr[i]);
    }
}