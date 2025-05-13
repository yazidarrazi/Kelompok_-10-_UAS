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
        strcpy(s->data[s->top].perangkat, nama);
        s->data[s->top].watt = watt;
    }
}

void tampilkanStack(Stack* s) {
    printf("\nHistori Pengurangan Pemakaian:\n");
    for (int i = s->top; i >= 0; i--) {
        printf("- %s (pengurangan: %d watt)\n", s->data[i].perangkat, s->data[i].watt);
    }
}

// Linked List
void tambahRiwayat(Node** head, int konsumsi, char hari[]) {
    Node* baru = (Node*)malloc(sizeof(Node));
    baru->totalKonsumsi = konsumsi;
    strcpy(baru->hari, hari);
    baru->next = NULL;

    if (*head == NULL) {
        *head = baru;
    } else {
        Node* temp = *head;
        while (temp->next) temp = temp->next;
        temp->next = baru;
    }
}

void tampilkanRiwayat(Node* head) {
    printf("\nRiwayat Konsumsi Harian:\n");
    while (head) {
        printf("- %s: %d watt\n", head->hari, head->totalKonsumsi);
        head = head->next;
    }
}

void hapusRiwayat(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Merge Sort
void merge(PerangkatAktif arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    PerangkatAktif L[n1], R[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
