#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PERANGKAT 100
#define MAX_STACK 50

typedef struct {
    char nama[30];
    int watt_per_menit;
} DaftarPerangkat;

typedef struct {
    char nama[30];
    int watt_per_menit;
    int durasi;
    int total_konsumsi;
} PerangkatAktif;

typedef struct Node {
    int totalKonsumsi;
    char hari[15];
    struct Node* next;
} Node;

typedef struct {
    char perangkat[30];
    int watt;
} Pengurangan;

typedef struct {
    Pengurangan data[MAX_STACK];
    int top;
} Stack;

// Stack
void initStack(Stack* s) {
    s->top = -1;
}

int isStackFull(Stack* s) {
    return s->top == MAX_STACK - 1;
}

int isStackEmpty(Stack* s) {
    return s->top == -1;
}

void push(Stack* s, char nama[], int watt) {
    if (!isStackFull(s)) {
        s->top++;
