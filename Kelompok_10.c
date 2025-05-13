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

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].total_konsumsi >= R[j].total_konsumsi)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(PerangkatAktif arr[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Greedy
int greedyOptimisasi(PerangkatAktif perangkat[], int n, int target, Stack* stack) {
    int penghematan = 0;
    printf("\nPengurangan Pemakaian Perangkat untuk Mencapai Target Penghematan:\n");

    for (int i = 0; i < n && penghematan < target; i++) {
        int pengurangan_watt = perangkat[i].total_konsumsi;
        int pengurangan_durasi = (pengurangan_watt > target - penghematan) ? (target - penghematan) / perangkat[i].watt_per_menit : perangkat[i].durasi;

        int dikurang = perangkat[i].watt_per_menit * pengurangan_durasi;
        printf("- %s: Pengurangan durasi %d menit (Pengurangan konsumsi: %d watt)\n", perangkat[i].nama, pengurangan_durasi, dikurang);

        penghematan += dikurang;
        push(stack, perangkat[i].nama, dikurang);
    }

    return penghematan;
}

void tampilkanTabelKonsumsi(PerangkatAktif arr[], int n) {
    printf("\n%-20s %-15s %-15s %-20s\n", "Nama Perangkat", "Durasi (menit)", "Watt/menit", "Total Konsumsi");
    printf("-------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-20s %-15d %-15d %-20d\n", arr[i].nama, arr[i].durasi, arr[i].watt_per_menit, arr[i].total_konsumsi);
    }
}

// Linear Search
void cariPerangkatKonsumsiTinggi(PerangkatAktif arr[], int n, int batas) {
    printf("\nPerangkat dengan konsumsi lebih dari %d watt:\n", batas);
    int ditemukan = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i].total_konsumsi > batas) {
            printf("- %s (%d watt)\n", arr[i].nama, arr[i].total_konsumsi);
            ditemukan = 1;
        }
    }
    if (!ditemukan) {
        printf("Tidak ada perangkat dengan konsumsi melebihi %d watt.\n", batas);
    }
}

// Tambah perangkat baru
void tambahPerangkat(DaftarPerangkat daftar[], int* jumlah_daftar) {
    if (*jumlah_daftar >= MAX_PERANGKAT) {
        printf("Daftar perangkat penuh.\n");
        return;
    }
    char nama[30];
    int watt;
    int ditemukan = 0;

    printf("Masukkan nama perangkat baru: ");
    scanf("%s", nama);

    for (int i = 0; i < *jumlah_daftar; i++) {
        if (strcasecmp(daftar[i].nama, nama) == 0) {
            printf("Perangkat dengan nama '%s' sudah ada di daftar.\n", nama);
            ditemukan = 1;
            break;
        }
    }

    if (ditemukan) {
        char pilihan;
        int valid = 0;
        do {
            printf("Apakah Anda ingin mengedit watt perangkat ini? (y/n): ");
            scanf(" %c", &pilihan);
            
            while(getchar() != '\n');
            
            pilihan = tolower(pilihan);
            if (pilihan == 'y' || pilihan == 'n') {
                valid = 1;
            } else {
                printf("Input tidak valid. Harap masukkan 'y' atau 'n'.\n");
            }
        } while (!valid);

        if (pilihan == 'y') {
            printf("Masukkan watt baru perangkat '%s': ", nama);
