#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define u8 unsigned char
#define u32 unsigned int
#define PREF_SIZE 7

typedef enum {
    PTR = 0,
    BUFF = 1,
} states;

typedef struct {
    u8 state;
    char pref[PREF_SIZE];
    u32 len;
    u32 cap;
    char* str;
} PTR_GSTR;

#define BUFF_SIZE sizeof(PTR_GSTR)-2
typedef struct {
    u8 state;
    u8 len;
    char str[BUFF_SIZE];
} BUFF_GSTR;

// all states of gstr have to have a u8 field state as first field
typedef union {
    PTR_GSTR pstr;
    BUFF_GSTR bstr;
} gstr;

typedef struct {
    char* arr;
    int len;
} STR;

gstr new_gstr(const char *str, u32 size) {
    gstr g;
    if (size < BUFF_SIZE) {
        g.bstr = (BUFF_GSTR){.state=BUFF, .len=size};
        memcpy(g.bstr.str, str, size);
        g.bstr.str[size] = '\0';
    } else {
        g.pstr = (PTR_GSTR){.state=PTR, .len=size, .cap=size+1};
        memcpy(g.pstr.pref, str, min(size, PREF_SIZE-1));
        if (size < PREF_SIZE) 
            g.pstr.pref[size] = '\0';
        g.pstr.str = (char*) malloc (size+1);
        memcpy(g.pstr.str, str, size);
        g.pstr.str[size] = '\0';
    }
    return g;
}

STR get_gstr(gstr *str) {
    if (str->bstr.state == PTR) 
        return (STR){.arr=str->pstr.str, .len=str->pstr.len};
    return (STR){.arr=str->bstr.str, .len=str->bstr.len};
}

int gstr_cmp(gstr g1, gstr g2) {
    char* p1 = (g1.pstr.state == PTR) ? g1.pstr.pref : g1.bstr.str;
    char* p2 = (g2.pstr.state == PTR) ? g2.pstr.pref : g2.bstr.str;
    int i=0;
    for (; i<PREF_SIZE && p1[i] == p2[i] && p1[i] != '\0'; i++);
    if (i != PREF_SIZE) return p1[i] - p2[i];
    STR s1 = get_gstr(&g1), s2 = get_gstr(&g2);
    for (int n=min(s1.len, s2.len); i < n && s1.arr[i] == s2.arr[i] && s1.arr[i] != '\0'; i++);
    return s1.arr[i] - s2.arr[i];
}

void gstr_reserve(gstr *str, int size) {
    if ((str->pstr.state == PTR && str->pstr.cap >= size) || (str->pstr.state == BUFF && BUFF_SIZE > size)) return;
    if (str->pstr.state == BUFF) {
        char *newstr = (char*) malloc (size+1);
        memcpy(newstr, str->bstr.str, str->bstr.len);
        newstr[str->bstr.len] = '\0';
        str->pstr = (PTR_GSTR){.state=PTR, .len=str->bstr.len, .cap=size+1, .str=newstr};
        memcpy(str->pstr.pref, newstr, min(str->pstr.len+1, PREF_SIZE));
    } else {
        str->pstr.str = realloc(str->pstr.str, size+1);
        str->pstr.cap = size+1;
    }
}

void gstr_assign(gstr *str, char* newstr, u32 size) {

}

int main() {
    printf("Buff: %d, PTR: %d, GSTR: %d\n", sizeof(BUFF_GSTR), sizeof(PTR_GSTR), sizeof(gstr));
    gstr s1 = new_gstr("something1", 10), s2 = new_gstr("something1", 9);
    gstr_reserve(&s1, 1000);
    printf("%s, %d, %d\n", s1.pstr.str, s1.pstr.cap, gstr_cmp(s1, s2));
    return 0;
}