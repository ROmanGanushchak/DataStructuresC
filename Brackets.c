#include <stdlib.h>
#include <stdio.h>

/*problem is to find the deepest bracket*/

#define MAX_INPUT_VALUE 100000

char getIndex(char character) {
    switch (character) {
        case '(': return 0;
        case '{': return 1;
        case '<': return 2;
        case '[': return 3;
        case ')': return 4;
        case '}': return 5;
        case '>': return 6;
        case ']': return 7;
        
        default:
            return 100;
    }
}

int main(){
    int n;
    scanf("%d", &n);

    char *s = (char*) malloc (MAX_INPUT_VALUE * sizeof(char));
    char *stack = s;
    int stackTop, index, mistakeCount, i;

    for (int iteration=0; iteration < n; iteration++) {
        scanf("%s", s);
        stackTop = 0; mistakeCount = 0;

        for (i=0; s[i] != '\0'; i++) {
            index = getIndex(s[i]);
            if (index < 4) stack[stackTop++] = index;
            else if (stackTop == 0) mistakeCount++;
            else {
                if (stack[stackTop-1] != index-4) mistakeCount++; 
                stackTop--;
            }
        }  

        if (iteration == 0) printf("%d", mistakeCount + stackTop);
        else printf("\n%d", mistakeCount + stackTop);
    } 

    free(s);

    return 0;
}