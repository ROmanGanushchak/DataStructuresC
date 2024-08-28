#include <stdio.h>
#include <stdlib.h>

#define debugPrint ((int)0)
#define DELETED ((void*) 1)
#define maxFilledTable 0.75
#define maxNonEmptyTable 0.85
#define elementsInSegment 128
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define printStr(s, isP) do { \
    if (isP) printf("\n%s", (s)); \
    else printf("%s", (s)); \
    (isP) = 1; \
} while(0)

int outputIndex = 1;
const unsigned int constants[] = {0xc34fd239, 0x1ba5c3ca, 0x59843dac, 0xfc32a78f, 0xa77b8fcb, 0x802ffac, 0xacf194dd, 0x05fceab1};
const unsigned int constantsCount = 8;
long long count = 0;

typedef struct hashElem {
    char name[16];
    char surname[16];
    char date[12];
    // int length;
    double balance;
} HashElem;

char cmpstr(char *str1, char*str2) {
    int i;
    for (i=0; str1[i] != '\0' && str2[i] != '\0'; i++) {
        if (str1[i] != str2[i])
            return 0;
    }

    if (str1[i] == '\0' && str2[i] == '\0')
        return 1;
    return 0;
}

char cmpdata(HashElem *data1, HashElem *data2) {
    // if (data1->length != data2->length) 
    //     return 0;
    
    if (cmpstr(data1->name, data2->name) && cmpstr(data1->surname, data2->surname) && cmpstr(data1->date, data2->date))
        return 1;
    return 0;
}

int lenstr(char *s) {
    int i;
    for (i=0; s[i] != '\0'; i++) {}
    return i;
}

void scandata(HashElem *data) {
    scanf("%s %s %s", data->name, data->surname, data->date);
    // data->length = lenstr(data->name) + lenstr(data->surname) + lenstr(data->date);
}

void scanDataBalane(HashElem *data, char *buffer) {
    scanf(" %s", buffer);

    for (int i=0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ',') {
            buffer[i] = '.';
            break;
        }
    }

    sscanf(buffer, "%lf", &data->balance);
}

typedef struct hashTable {
    HashElem **table;
    int length;
    int filled;
    int segmentCount;
    int deleted;
} HashTable;

int findNextPrime(int number) {
    number = number / 6 * 6 + 7;
    char isDivided=1;

    for (unsigned int divider, adder, num;; number+=6) {
        for (adder=0; adder<5; adder += 4) {
            num = number + adder;
            isDivided=0;
            for (divider=5; divider*divider <= num; divider += 2) {
                if (num % divider == 0) {
                    isDivided=1; 
                    break;
                }
            }

            if (!isDivided) return num;
        }
    }
}

unsigned int hash1(HashElem data) {
    unsigned int hash = constants[data.date[0] % constantsCount], current, dataHash=0;
    for (int i=0; data.date[i] != '\0'; i++)
        dataHash += data.date[i];

    char rotate = dataHash % 5;
    char rotate1 = dataHash % 9;
    char rotate2 = dataHash % 7;

    for (char i=0, j; data.name[i] != '\0'; i++) {
        current = 0;
        for (j=0; data.name[i] != '\0' && j<4; i++, j++) {
            current <<= 8;
            current += data.name[i];
        }
        i--;
        hash <<= rotate;
        hash ^= current;
        hash ^= current << rotate1 | current >> rotate2;
    }
    if (debugPrint) printf("hash1>%u ", hash);
    return hash;
}

unsigned int hash2(HashElem data) {
    unsigned int surHash=0;
    for (int i=0; data.surname[i] != '\0'; i++) 
        surHash += data.surname[i];
    
    if (debugPrint) printf("hash2>%u ", (constants[surHash%constantsCount] ^ ((surHash << (surHash % 16)) | surHash)));
    return constants[surHash%constantsCount] ^ ((surHash << (surHash % 16)) | surHash);
}

unsigned int transformHash1(unsigned int hash, HashTable hashTable) {
    return hash % hashTable.length;
}

unsigned int transformHash2(unsigned int hash, HashTable hashTable) {
    return max(hash % hashTable.length, 1);
}

void resizeTable(HashTable *hashTable, int newLength) {
    if (debugPrint) printf("\n--------RESIZE--------\n");
    unsigned int index, adder, oldLength = hashTable->length;
    newLength = findNextPrime(newLength);
    hashTable->segmentCount = max(1, hashTable->length / elementsInSegment);
    hashTable->length = newLength;
    hashTable->deleted = 0;
    
    HashElem **newTable = (HashElem **) calloc (newLength, sizeof(HashElem*));

    if (newTable == NULL) exit(1);

    for (int i=0; i<oldLength; i++) {
        if (hashTable->table[i] == NULL || hashTable->table[i] == DELETED)
            continue;
        
        index = transformHash1(hash1(*hashTable->table[i]), *hashTable);
        adder = transformHash2(hash2(*hashTable->table[i]), *hashTable);

        for (;; index = (index + adder) % hashTable->length) {
            if (newTable[index] == NULL) {
                newTable[index] = hashTable->table[i];
                break;
            }
        }
    }

    free(hashTable->table);
    hashTable->table = newTable;
}

char add(HashElem *data, HashTable *hashTable) {
    HashElem **table = hashTable->table;
    unsigned int index = transformHash1(hash1(*data), *hashTable);
    unsigned int adder = transformHash2(hash2(*data), *hashTable);
    char result;
    int indexToAdd = -1;
    
    for (;; index = (index + adder) % hashTable->length) {
        count++;
        if (debugPrint) printf("\nindex -> %d ", index);
        if (debugPrint) {
            if (table[index] == NULL) printf("null ");
            else if (table[index] == DELETED) printf("deleted ");
            else printf("%s %s ", table[index]->name, table[index]->surname);
        }

        if (table[index] == NULL) {
            if (indexToAdd == -1)
                indexToAdd = index;
            break;
        } else if (table[index] == DELETED) {
            if (indexToAdd == -1)
                indexToAdd = index;
        } else if (cmpdata(data, table[index])) {
            indexToAdd = -1;
            break;
        }
    }

    if (indexToAdd == -1) return 0;

    if (table[indexToAdd] == DELETED) 
        hashTable->deleted--;
    hashTable->table[indexToAdd] = data;
    hashTable->filled++;

    if (hashTable->filled / ((float)hashTable->length) > maxFilledTable) {
        if (debugPrint) printf("\n--RESIZECALLED--\n");
        resizeTable(hashTable, hashTable->length*1.45);
    }else if ((hashTable->filled+hashTable->deleted) / hashTable->length > maxNonEmptyTable)
        resizeTable(hashTable, hashTable->length*1);
    
    return 1;
}

int findIndex(HashElem *data, HashTable hashTable) {
    unsigned int index = transformHash1(hash1(*data), hashTable);
    unsigned int adder = transformHash2(hash2(*data), hashTable);

    for (;; index = (index + adder) % hashTable.length) {
        count++;
        if (hashTable.table[index] == NULL)
            return -1;

        if (hashTable.table[index] != DELETED && cmpdata(hashTable.table[index], data)) 
            return index;
    }
}

int main() {
    HashTable hashTable = {.filled=0, .deleted=0, .length=7}; 
    hashTable.table = (HashElem**) calloc (hashTable.length, sizeof(HashElem*));
    hashTable.segmentCount = max(1, hashTable.length / elementsInSegment);
    HashTable *hashTableP = &hashTable;

    char type, buffer[32], isPrinted=0;
    int index, commaIndex, temp, decimal;
    HashElem *data = (HashElem*) malloc (sizeof(HashElem));
    HashElem *dataHolder = (HashElem*) malloc (sizeof(HashElem));

    while (scanf("%c ", &type) != EOF) {
        printf("Type -> %c", type);
        switch (type) {
            case 'i':
                scandata(data);
                scanDataBalane(data, buffer);
                if (debugPrint) printf("next i %s %s %s %lf -> ", data->name, data->surname, data->date, data->balance);

                if (!add(data, hashTableP)) {
                    printStr("insert failed", isPrinted);
                } else {
                    data = (HashElem*) malloc (sizeof(HashElem));
                    if (data == NULL) exit(1);
                }
                if (debugPrint) printf("\n");
                break;
            
            case 's':
                scandata(dataHolder);
                index = findIndex(dataHolder, hashTable);
                if (debugPrint) printf("next i %s %s %s -> ", dataHolder->name, dataHolder->surname, dataHolder->date);

                if (index == -1) {
                    printStr("search failed", isPrinted);
                } else {
                    temp = (int) (hashTable.table[index]->balance+0.005);
                    if (isPrinted) {
                        printf("\n%d,", temp);
                    } else {
                        printf("%d,", temp);
                        isPrinted++;
                    }

                    decimal = (int)((hashTable.table[index]->balance - temp) * 100 + 0.5);
                    if (decimal < 10) 
                        printf("0%d", decimal);
                    else
                        printf("%d", decimal);
                }
                if (debugPrint) printf("\n");
                break;
            
            case 'u':
                scandata(dataHolder);
                scanDataBalane(dataHolder, buffer);
                if (debugPrint) printf("next i %s %s %s %lf -> ", dataHolder->name, dataHolder->surname, dataHolder->date, dataHolder->balance);
                index = findIndex(dataHolder, hashTable);
                if (index == -1 || hashTable.table[index]->balance + dataHolder->balance < 0) 
                    printStr("update failed", isPrinted);
                else 
                    hashTable.table[index]->balance += dataHolder->balance;
                
                if (debugPrint) printf("\n");
                break;
            
            case 'd':
                scandata(dataHolder);
                if (debugPrint) printf("next i %s %s %s -> ", dataHolder->name, dataHolder->surname, dataHolder->date);
                index = findIndex(dataHolder, hashTable);

                if (index == -1) 
                    printStr("delete failed", isPrinted);
                else {
                    free(hashTable.table[index]);
                    hashTable.table[index] = DELETED;
                    hashTable.filled--;
                    hashTable.deleted++;
                }
                if (debugPrint) printf("\n");
                break;
            case 'f':
                return 0;
        }
    }

    free(data);
    free(dataHolder);
    for (int i=0; i<hashTable.length; i++) {
        if (hashTable.table[i] != NULL && hashTable.table[i] != DELETED)
            free(hashTable.table[i]);
    }
    printf("Count -> %d", count);
    free(hashTable.table);
    return 0;
}

// case 'f':
//                 printf("Hash %d", hashTable.length);
//                 FILE *hash = fopen("hashTable.txt", "w");
//                 for (int i=0; i<hashTable.length; i++) {
//                     if (hashTable.table[i].state == 0) fprintf(hash, "null\n");
//                     if (hashTable.table[i].state == 1) fprintf(hash, "deleted\n");
//                     if (hashTable.table[i].state == 2) fprintf(hash, "filllllllllllllllllled\n");
//                 }
//                 fclose(hash);
//                 return 0;