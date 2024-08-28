#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define segmentCount 32.0
#define max(x, y) (((x) > (y)) ? (x) : (y))
int failed=0;
int called=0;

int hashTableLength = 101;
int filledCount=0;
const float minFreeCellPercent=0.3;

typedef struct hashElem {
    char name[20];
    struct hashElem *next;
} HASH_ELEM;

int hash(char *name) {
    int i=0, hash = (name[0]) / segmentCount * (hashTableLength / segmentCount);
    while (name[i] != '\0') {
        hash += 33 * i * name[i];
        i++;
    }
    return hash % hashTableLength;
}

void add(char *name, HASH_ELEM **hashTable) {
    int index = hash(name);
    HASH_ELEM *new = (HASH_ELEM*) malloc (sizeof(HASH_ELEM));
    new->next = NULL;
    strcpy(new->name, name);

    if (hashTable[index] == NULL) {
        hashTable[index] = new;
    } else {
        HASH_ELEM *current = hashTable[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new;
    }
}

void resize(int newSize, HASH_ELEM ***hashTablePointer) {
    HASH_ELEM **hashTable = *hashTablePointer;
    int old = hashTableLength, index;
    hashTableLength = newSize;
    HASH_ELEM **newHashTable = (HASH_ELEM**) malloc (newSize * sizeof(HASH_ELEM*));
    for (int i=0; i<newSize; i++) {
        newHashTable[i] = NULL;
    }
    HASH_ELEM *current, *current2;

    for (int i=0; i<old; i++) {
        // printf("ppp\n");
        if (hashTable[i] == NULL)
            continue;
        
        current = hashTable[i];
        while (current != NULL) {
            index = hash(current->name);
            if (newHashTable[index] == NULL) {
                newHashTable[index] = current;
                newHashTable[index]->next = NULL;
            } else {
                current2 = hashTable[index];
                while (current2 != NULL) {
                    add(current2->name, newHashTable);
                    current2 = current->next;
                }
            }

            current = current->next;
        }
    }
    *hashTablePointer = newHashTable;
}

int find(HASH_ELEM **hashTable, char *name) {
    called++;
    int index = hash(name);
    if (hashTable[index] == NULL) {
        return 300;
    }

    HASH_ELEM *current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) 
            return 100;
        
        current = current->next;
        failed++;
    }
    return 200;
}

int main() {
    FILE *in = fopen("names.txt", "r");
    if (in == NULL) {
        printf("File with name names.txt wasnt found, make sure that it exits\n");
        return 1;
    }
    clock_t begin = clock();
    HASH_ELEM **hashTable = (HASH_ELEM**) malloc (hashTableLength * sizeof(HASH_ELEM*));
    for (int i=0; i<hashTableLength; i++) {
        hashTable[i] = NULL;
    }

    int i = 0;
    char input[20];
    int wordCount=0;

    while (fscanf(in, "%s", input) != EOF) {
        filledCount++;
        add(input, hashTable);
        if ((hashTableLength - filledCount) / ((float)hashTableLength) < minFreeCellPercent) 
            resize(hashTableLength*2, &hashTable);
    }
    
    fclose(in);
    FILE *inIndex = fopen("searchName.txt", "r");
    while (fscanf(inIndex, "%s", input) != EOF) {
        find(hashTable, input);
    }
    
    clock_t end = clock();
    printf("\n called -> %d \n", called);
    double time_spent = ((double)(end - begin)) / CLOCKS_PER_SEC * 1000;
    printf("%lf\n", time_spent);
    printf("failed -> %d\n", failed);
}