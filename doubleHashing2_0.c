#include <stdio.h>
#include <stdlib.h>

// Get-Content input.txt | .\doubleHashing2_0.exe | Out-File output.txt
// cmd.exe /c "doubleHashing2_0.exe < input.txt > output.txt"

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

const unsigned int constants[] = {0xc34fd239, 0x1ba5c3ca, 0x59843dac, 0xfc32a78f, 0xa77b8fcb, 0x802ffac, 0xacf194dd, 0x05fceab1};
const unsigned int constantsCount = 8;

typedef struct hashElem {
    char name[16];
    char surname[16];
    char date[12];
    // int length;
    int balance[2];
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
        hash ^= current << rotate1;
        hash ^= current >> rotate2;
    }
    
    return hash;
}

unsigned int hash2(HashElem data) {
    return constants[data.date[4]%constantsCount] ^ ((data.surname[0] << 8) + data.date[0]);
}

unsigned int transformHash1(unsigned int hash, HashTable hashTable) {
    return hash % hashTable.length;
}

unsigned int transformHash2(unsigned int hash, HashTable hashTable) {
    return max(hash % hashTable.length, 1);
}

void resizeTable(HashTable *hashTable, int newLength) {
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
    
    for (;; index = (index + adder) % hashTable->length) {
        if (table[index] == NULL || table[index] == DELETED) {
            if (table[index] == DELETED) 
                hashTable->deleted--;
            
            hashTable->table[index] = data;

            hashTable->filled++;
            result = 1;
            break;
        }

        if (cmpdata(data, table[index])) {
            result = 0;
            break;
        }
    }

    if (hashTable->filled / ((float)hashTable->length) > maxFilledTable)
        resizeTable(hashTable, hashTable->length*1.4);
    else if ((hashTable->filled) / hashTable->length > maxNonEmptyTable)
        resizeTable(hashTable, hashTable->length*1.1);
    
    return result;
}

int findIndex(HashElem *data, HashTable hashTable) {
    unsigned int index = transformHash1(hash1(*data), hashTable);
    unsigned int adder = transformHash2(hash2(*data), hashTable);

    for (;; index = (index + adder) % hashTable.length) {
        if (hashTable.table[index] == NULL)
            return -1;

        if (hashTable.table[index] != DELETED && cmpdata(hashTable.table[index], data)) 
            return index;
    }
}

int main() {
    HashTable hashTable = {.filled=0, .deleted=0, .length=101}; 
    hashTable.table = (HashElem**) calloc (hashTable.length, sizeof(HashElem*));
    hashTable.segmentCount = max(1, hashTable.length / elementsInSegment);
    HashTable *hashTableP = &hashTable;

    char type, buffer[32], isPrinted=0;
    int index, commaIndex, temp, decimal;
    HashElem *data = (HashElem*) malloc (sizeof(HashElem));
    HashElem *dataHolder = (HashElem*) malloc (sizeof(HashElem));

    while (scanf("%c ", &type) != EOF) {
        switch (type) {
            case 'i':
                scandata(data);
                scanDataBalane(data, buffer);
                add(data, hashTableP);
                data = (HashElem*) malloc (sizeof(HashElem));
                if (data == NULL) exit(1);
                break;
            
            case 's':
                scandata(dataHolder);
                index = findIndex(dataHolder, hashTable);

                if (index == -1) {
                    printStr("search failed", isPrinted);
                } else {
                    temp = (int) hashTable.table[index]->balance;
                    if (isPrinted) {
                        printf("\n%d,", temp);
                    } else {
                        printf("%d,", temp);
                        isPrinted++;
                    }

                    decimal = (int)((hashTable.table[index]->balance - temp) * 100 + 0.5);
                    if (hashTable.table[index]->balance - temp < 0.1) 
                        printf("0%d", decimal);
                    else
                        printf("%d", decimal);
                }
                break;
            
            case 'u':
                scandata(dataHolder);
                scanDataBalane(dataHolder, buffer);
                index = findIndex(dataHolder, hashTable);
                if (index == -1 || hashTable.table[index]->balance + dataHolder->balance < 0) 
                    printStr("update failed", isPrinted);
                else 
                    hashTable.table[index]->balance += dataHolder->balance;
                
                break;
            
            case 'd':
                scandata(dataHolder);
                index = findIndex(dataHolder, hashTable);

                if (index == -1) 
                    printStr("delete failed", isPrinted);
                else {
                    free(hashTable.table[index]);
                    hashTable.table[index] = DELETED;
                    hashTable.filled--;
                    hashTable.deleted++;
                }
                break;
        }
    }

    free(data);
    free(dataHolder);
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