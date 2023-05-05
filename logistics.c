#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char nama_barang[20];
    int jumlah_barang;
    struct Node *prev;
    struct Node *next;
} Node;

Node *head = NULL;

void insert(char *nama_barang, int jumlah_barang)
{
    Node *node = (Node *)malloc(sizeof(Node));
    strcpy(node->nama_barang, nama_barang);
    node->jumlah_barang = jumlah_barang;
    node->prev = NULL;
    node->next = head;
    if (head != NULL)
    {
        head->prev = node;
    }
    head = node;
    printf("Barang %s dengan jumlah %d berhasil ditambahkan.\n", nama_barang, jumlah_barang);
}

void delete(char *nama_barang)
{
    Node *current = head;
    while (current != NULL)
    {
        if (strcmp(current->nama_barang, nama_barang) == 0)
        {
            if (current->prev == NULL)
            {
                head = current->next;
            }
            else
            {
                current->prev->next = current->next;
            }
            if (current->next != NULL)
            {
                current->next->prev = current->prev;
            }
            free(current);
            printf("Barang %s berhasil dihapus.\n", nama_barang);
            return;
        }
        current = current->next;
    }
    printf("Barang %s tidak ditemukan.\n", nama_barang);
}

void search_by_name(char *nama_barang)
{
    Node *current = head;
    while (current != NULL)
    {
        if (strcmp(current->nama_barang, nama_barang) == 0)
        {
            printf("Barang %s dengan jumlah %d ditemukan.\n", current->nama_barang, current->jumlah_barang);
            return;
        }
        current = current->next;
    }
    printf("Barang %s tidak ditemukan.\n", nama_barang);
}

void search_by_quantity(int jumlah_barang)
{
    Node *current = head;
    while (current != NULL)
    {
        if (current->jumlah_barang == jumlah_barang)
        {
            printf("Barang %s dengan jumlah %d ditemukan.\n", current->nama_barang, current->jumlah_barang);
        }
        current = current->next;
    }
}

void display_new_item()
{
    Node *current = head;
    if (current == NULL)
    {
        printf("Belum ada barang yang ditambahkan.\n");
        return;
    }
    printf("Daftar barang yang baru ditambahkan:\n");
    while (current != NULL)
    {
        if (current->jumlah_barang == 0)
        {
            printf("%s\n", current->nama_barang);
        }
        current = current->next;
    }
}

void swap(Node *node1, Node *node2)
{
    int temp_jumlah_barang = node1->jumlah_barang;
    node1->jumlah_barang = node2->jumlah_barang;
    node2->jumlah_barang = temp_jumlah_barang;
    char temp_nama_barang[20];
    strcpy(temp_nama_barang, node1->nama_barang);
    strcpy(node1->nama_barang, node2->nama_barang);
    strcpy(node2->nama_barang, temp_nama_barang);
}

void sort_by_name()
{
    Node *current = head;
    while (current != NULL)
    {
        Node *min_node = current;
        Node *temp = current->next;
        while (temp != NULL)
        {
            if (strcmp(temp->nama_barang, min_node->nama_barang) < 0)
            {
                min_node = temp;
            }
            temp = temp->next;
        }
        if (min_node != current)
        {
            swap(current, min_node);
        }
        current = current->next;
    }
    printf("Daftar barang berhasil diurutkan berdasarkan nama.\n");
}

void sort_by_quantity()
{
    Node *current = head;
    while (current != NULL)
    {
        Node *max_node = current;
        Node *temp = current->next;
        while (temp != NULL)
        {
            if (temp->jumlah_barang > max_node->jumlah_barang)
            {
                max_node = temp;
            }
            temp = temp->next;
        }
        if (max_node != current)
        {
            swap(current, max_node);
        }
        current = current->next;
    }
    printf("Daftar barang berhasil diurutkan berdasarkan jumlah.\n");
}

int main()
{
    insert("palu", 0);
    insert("paku", 0);
    insert("baut", 0);
    int choice;
    char nama_barang[20];
    int jumlah_barang;

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Tambah barang\n");
        printf("2. Hapus barang\n");
        printf("3. Cari berdasarkan nama barang\n");
        printf("4. Cari berdasarkan jumlah barang\n");
        printf("5. Tampilkan barang yang baru ditambahkan\n");
        printf("6. Urutkan barang berdasarkan nama barang\n");
        printf("7. Urutkan barang berdasarkan jumlah barang\n");
        printf("8. Keluar\n");
        printf("Pilihan Anda: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Masukkan nama barang: ");
            scanf("%s", nama_barang);
            printf("Masukkan jumlah barang: ");
            scanf("%d", &jumlah_barang);
            insert(nama_barang, jumlah_barang);
            break;
        case 2:
            printf("Masukkan nama barang yang akan dihapus: ");
            scanf("%s", nama_barang);
            delete (nama_barang);
            break;
        case 3:
            printf("Masukkan nama barang yang akan dicari: ");
            scanf("%s", nama_barang);
            search_by_name(nama_barang);
            break;
        case 4:
            printf("Masukkan jumlah barang yang akan dicari: ");
            scanf("%d", &jumlah_barang);
            search_by_quantity(jumlah_barang);
            break;
        case 5:
            display_new_item();
            break;
        case 6:
            sort_by_name();
            break;
        case 7:
            sort_by_quantity();
            break;
        case 8:
            exit(0);
        default:
            printf("Pilihan tidak valid.\n");
        }
    }
    return 0;
}