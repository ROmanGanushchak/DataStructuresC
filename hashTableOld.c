#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define segmentCount 32
#define max(x, y) (((x) > (y)) ? (x) : (y))
int failed=0;


typedef struct hashElem {
    char name[20];
    int length;
    int indexHash;
    int adderHash;
    char state;
} HASH_ELEM;

int findNextPrime(int number) {
    number = number / 2 * 2 + 1;
    
    for (;;number+=2) {
        bool isDivided=false;
        for (int divider=3; divider<number; divider += 2) {
            if (number % divider == 0) {
                isDivided=true; 
                break;
            }
        }

        if (!isDivided) 
            return number;
    }
}

int hash1(char *name, int nameSize) {
    int hash = 0;
    for (int i=0; i<nameSize; i++) {
        hash += 33 * i * name[i];
    }
    return hash;
}

int hash2(char *name, int nameSize) {
    return max(name[0] + name[1], 1);
}

int resizeTable(HASH_ELEM **hashTable, int *hashTableLength, int newLength) {
    newLength = findNextPrime(newLength);
    HASH_ELEM *newHashTable = (HASH_ELEM *) malloc (newLength * sizeof(HASH_ELEM));
    for (int i=0; i<newLength; i++) {
        newHashTable[i].state = 0;
    }

    for (int i=0; i<(*hashTableLength); i++) {
        if ((*hashTable)[i].state != 2)
            continue;
        
        int index = ((*hashTable)[i].indexHash + ((*hashTable)[i].indexHash) / segmentCount * (newLength / segmentCount)) % newLength;
        int adder = (*hashTable)[i].adderHash % newLength;
        if (adder == 0) {
            adder++;
        }

        do {
            if (newHashTable[index].state == 0) {
                newHashTable[index].state = 2;
                strncpy(newHashTable[index].name, (*hashTable)[i].name, ((*hashTable)[i].length)+1);
                newHashTable[index].length = (*hashTable)[i].length;
                newHashTable[index].indexHash = (*hashTable)[i].indexHash;
                newHashTable[index].adderHash = (*hashTable)[i].adderHash;
                break;
            }

            index = (index + adder) % newLength;
        } while (1);
    }

    *hashTableLength = newLength;
    free(*hashTable);
    *hashTable = newHashTable;
    return newLength;
}

void addName(char *name, HASH_ELEM *hashTable, int hashTableLength, int *tableFilled) {
    int nameLength = strlen(name);
    int indexHash = hash1(name, nameLength);
    int adderHash = hash2(name, nameLength);
    int index = (indexHash + indexHash / segmentCount * (hashTableLength / segmentCount)) % hashTableLength;
    int adder = adderHash % hashTableLength;
    if (adder == 0)
        adder++;
    
    while (1) {
        if (hashTable[index].state == 0 || hashTable[index].state == 1) {
            strcpy(hashTable[index].name, name);
            hashTable[index].length = nameLength;
            hashTable[index].indexHash = indexHash;
            hashTable[index].adderHash = adderHash;
            hashTable[index].state = 2;
            (*tableFilled)++;
            return;
        }
        index = (index + adder) % hashTableLength;
    }
}

int findIndex(char *name, HASH_ELEM *hashTable, int hashTableLength) {
    int nameLength = strlen(name);
    int indexHash = hash1(name, nameLength);
    int index = (indexHash + indexHash / segmentCount * (hashTableLength / segmentCount)) % hashTableLength;
    int adder = hash2(name, nameLength) % hashTableLength;
    int checkCount = 0;
    
    while (1) {
        if (hashTable[index].state == 0)
            return -1;

        if (hashTable[index].length == nameLength && (strcmp(hashTable[index].name, name) == 0)) 
            return index;

        index = (index + adder) % hashTableLength;
        checkCount++;
        if (checkCount == hashTableLength)
            return -1;
        failed++;
    }
}

int main() {
    FILE *in = fopen("names.txt", "r");
    if (in == NULL) {
        printf("File with name names.txt wasnt found, make sure that it exits\n");
        return 1;
    }
    int hashTableLength = 101;
    int filledCount=0;
    float minFreeCellPercent=0.35;

    HASH_ELEM *hashTable = (HASH_ELEM*) malloc (hashTableLength * sizeof(HASH_ELEM));
    for (int i=0; i<hashTableLength; i++) {
        hashTable[i].state = 0;
    }

    int i = 0;
    char input[20];
    int wordCount=0;
    clock_t begin = clock();

    while (fscanf(in, "%s", input) != EOF) {
        wordCount++;
        addName(input, hashTable, hashTableLength, &filledCount);
        if ((hashTableLength - filledCount) / (float)hashTableLength < minFreeCellPercent) 
            resizeTable(&hashTable, &hashTableLength, hashTableLength*2);
    }

    fclose(in);
    FILE *inIndex = fopen("searchName.txt", "r");
    while (fscanf(inIndex, "%s", input) != EOF) {
        findIndex(input, hashTable, hashTableLength);
    }
    
    clock_t end = clock();
    printf("\n");
    double time_spent = ((double)(end - begin)) / CLOCKS_PER_SEC * 1000;
    printf("%lf\n", time_spent);
    printf("faile -? %d\n", failed);
    FILE *out = fopen("failed.txt", "w");
    
    for (int i=0; i < hashTableLength; i++) {
        if (hashTable[i].state == 0) {
            fprintf(out, "null\n");
        } else 
            fprintf(out, "%s\n", hashTable[i].name);
    }

    return 0;
}