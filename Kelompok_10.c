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
            scanf("%d", &watt);
            for (int i = 0; i < *jumlah_daftar; i++) {
                if (strcasecmp(daftar[i].nama, nama) == 0) {
                    daftar[i].watt_per_menit = watt;
                    printf("Watt perangkat '%s' berhasil diubah menjadi %d watt/menit.\n", nama, watt);
                    break;
                }
            }
        }
    } else {
        printf("Masukkan konsumsi watt/menit perangkat tersebut: ");
        scanf("%d", &watt);
        strcpy(daftar[*jumlah_daftar].nama, nama);
        daftar[*jumlah_daftar].watt_per_menit = watt;
        (*jumlah_daftar)++;
        printf("Perangkat '%s' berhasil ditambahkan.\n", nama);
    }
}

void editWattPerangkat(DaftarPerangkat daftar[], int jumlah_daftar) {
    char nama[30];
    char input[100];
    int watt_baru;
    int ditemukan = 0;

    printf("Masukkan nama perangkat yang ingin diubah watt-nya: ");
    scanf("%s", nama);

    for (int i = 0; i < jumlah_daftar; i++) {
        if (strcasecmp(daftar[i].nama, nama) == 0) {
            printf("Perangkat '%s' ditemukan.\n", daftar[i].nama);
            printf("Watt saat ini: %d watt/menit\n", daftar[i].watt_per_menit);

            int valid = 0;
            while (!valid) {
                printf("Masukkan watt baru perangkat '%s': ", daftar[i].nama);
                fgets(input, sizeof(input), stdin);  

                if (input[0] == '\n') {
                    fgets(input, sizeof(input), stdin);
                }

                if (sscanf(input, "%d", &watt_baru) == 1 && watt_baru > 0) {
                    valid = 1;
                    daftar[i].watt_per_menit = watt_baru;
                    printf("Watt perangkat '%s' berhasil diubah menjadi %d watt/menit.\n", daftar[i].nama, watt_baru);
                } else {
                    printf("Input tidak valid. Harus berupa angka lebih dari 0.\n");
                }
            }

            ditemukan = 1;
            break;
        }
    }

    if (!ditemukan) {
        printf("Perangkat dengan nama '%s' tidak ditemukan.\n", nama);
    }
}


// Main Program
int main() {
    DaftarPerangkat daftar[MAX_PERANGKAT] = {
        {"AC", 10}, {"Kulkas", 2}, {"TV", 2}, {"Laptop", 1},
        {"Mesin Cuci", 8}, {"Dispenser", 5}, {"Lampu", 1}, {"Rice Cooker", 7},
        {"Setrika", 5}, {"Pompa Air", 4}, {"Kompor Listrik", 17}, {"Microwave", 13},
        {"CCTV", 1}, {"Kipas Angin", 2}, {"Vacuum Cleaner", 10}
    };
    int jumlah_daftar = 15;
    PerangkatAktif aktif[MAX_STACK];
    Stack stack, rekomendasi;
    Node* riwayat = NULL;
    int jumlah_aktif = 0, menu;
    int target = 0;
    char input[50];
    char hari[15];

    initStack(&stack);
    initStack(&rekomendasi);

    do {
        printf("\n=== MENU UTAMA ===\n");
        printf("1. Rekomendasi Penghematan Listrik\n");
        printf("2. Tampilkan Riwayat Konsumsi\n");
        printf("3. Cari Perangkat dengan Konsumsi di Atas Batas\n");
        printf("4. Tambah Perangkat ke Daftar\n");
        printf("5. Edit Watt Perangkat\n");
        printf("6. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &menu);
        getchar(); 

        switch (menu) {
            case 1: {
                jumlah_aktif = 0;
                int total = 0;

                printf("\nDaftar Perangkat:\n");
                for (int i = 0; i < jumlah_daftar; i++) {
                    printf("%d. %s (%d watt/menit)\n", i + 1, daftar[i].nama, daftar[i].watt_per_menit);
                }

                int pilihan;
                while (1) {
                    printf("\nMasukkan pilihan perangkat (1-%d) atau '0' untuk selesai: ", jumlah_daftar);
                    fgets(input, sizeof(input), stdin);
                    if (sscanf(input, "%d", &pilihan) != 1 || pilihan < 0 || pilihan > jumlah_daftar) {
                        printf("Pilihan tidak valid.\n");
                        continue;
                    }
                    if (pilihan == 0) break;

                    DaftarPerangkat* p = &daftar[pilihan - 1];
                    int durasi;
                    printf("Durasi penggunaan (menit): ");
                    scanf("%d", &durasi);
                    getchar(); 

                    PerangkatAktif aktifPerangkat;
                    strcpy(aktifPerangkat.nama, p->nama);
                    aktifPerangkat.watt_per_menit = p->watt_per_menit;
                    aktifPerangkat.durasi = durasi;
                    aktifPerangkat.total_konsumsi = durasi * p->watt_per_menit;

                    aktif[jumlah_aktif++] = aktifPerangkat;
                    total += aktifPerangkat.total_konsumsi;
                }

                printf("Masukkan hari penggunaan (format: DD-MM-YYYY): ");
int day, month, year;
while (1) {
    if (scanf("%d-%d-%d", &day, &month, &year) != 3) {
        printf("Format tidak valid. Silahkan masukkan tanggal (DD-MM-YYYY): ");
       
        while (getchar() != '\n');
        continue;
    }
    
    // Validasi tanggal
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
        printf("Tanggal tidak valid. Silahkan masukkan tanggal yang benar (DD-MM-YYYY): ");
        continue;
    }

    snprintf(hari, sizeof(hari), "%02d-%02d-%04d", day, month, year);
    break;
}
getchar(); 
tambahRiwayat(&riwayat, total, hari);

                printf("\nTotal Konsumsi: %d watt\n", total);
                printf("Target Penghematan (watt): ");
                scanf("%d", &target);
                getchar(); 

                mergeSort(aktif, 0, jumlah_aktif - 1);
                tampilkanTabelKonsumsi(aktif, jumlah_aktif);
                int total_hemat = greedyOptimisasi(aktif, jumlah_aktif, target, &rekomendasi);

                printf("\nTotal penghematan yang bisa dilakukan: %d watt\n", total_hemat);
                tampilkanStack(&rekomendasi);
                break;
            }

            case 2:
                tampilkanRiwayat(riwayat);
                break;
            
            case 3: {
                int batas;
                int valid = 0;
                char input[50];

                do {
                    printf("Masukkan batas konsumsi watt: ");
                    fgets(input, sizeof(input), stdin);
        
                if (sscanf(input, "%d", &batas) == 1) {
                    valid = 1;
                } else {
                    printf("Input tidak valid. Harap masukkan angka.\n");
                }
                } while (!valid);
    
                 cariPerangkatKonsumsiTinggi(aktif, jumlah_aktif, batas);
                 break;
            }
    
            case 4:
                tambahPerangkat(daftar, &jumlah_daftar);
                break;
            case 5:
                editWattPerangkat(daftar, jumlah_daftar);
                break;
            case 6:
                printf("Terima kasih. Program selesai.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
                break;
        }
    } while (menu != 6);
