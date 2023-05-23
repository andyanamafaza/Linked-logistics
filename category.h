#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define MAX 50
#define DATA "data.txt"
#define CATEGORIES "categories.txt"
#define HASH_SIZE 26

typedef struct Category
{
    char name[MAX];
    struct Category *next;
} Category;

typedef struct HashTable
{
    Category *head;
    Category *tail;
} HashTable;

int hash(char ch)
{
    ch = tolower(ch);
    return ch - 'a';
}

void initialize(HashTable *hashTable)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hashTable[i].head = NULL;
        hashTable[i].tail = NULL;
    }
}

void load_categories(HashTable *hashTable)
{
    FILE *fp = fopen(CATEGORIES, "r");
    if (fp == NULL)
    {
        printf("Error: Cannot open file\n");
        return;
    }

    char line[MAX];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line[strcspn(line, "\n")] = 0;
        char first = line[0];
        int index = hash(first);
        Category *category = (Category *)malloc(sizeof(Category));
        strcpy(category->name, line);
        category->next = NULL;

        if (hashTable[index].head == NULL)
        {
            hashTable[index].head = category;
            hashTable[index].tail = category;
        }
        else
        {
            hashTable[index].tail->next = category;
            hashTable[index].tail = category;
        }
    }
    fclose(fp);
}

void save_categories(HashTable *hashTable)
{
    FILE *fp = fopen(CATEGORIES, "w");
    if (fp == NULL)
    {
        printf("Error: Cannot open file\n");
        return;
    }

    for (int i = 0; i < HASH_SIZE; i++)
    {
        Category *current = hashTable[i].head;
        while (current != NULL)
        {
            fprintf(fp, "%s\n", current->name);
            current = current->next;
        }
    }

    fclose(fp);
}

void delete_category(HashTable *hashTable, const char *input)
{
    int index = hash(input[0]);
    Category *current = hashTable[index].head;
    Category *previous = NULL;

    while (current != NULL)
    {
        if (strcmp(current->name, input) == 0)
        {
            if (previous == NULL)
            {
                hashTable[index].head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            if (hashTable[index].tail == current)
            {
                hashTable[index].tail = previous;
            }

            free(current);
            printf("Category deleted: %s\n", input);
            return;
        }

        previous = current;
        current = current->next;
    }

    printf("Category not found: %s\n", input);
}

void print_categories(HashTable *hashTable)
{
    int count = 1;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        Category *current = hashTable[i].head;
        while (current != NULL)
        {
            printf("%d. %s\n", count, current->name);
            count++;
            current = current->next;
        }
    }
}

void add_category(HashTable *hashTable, const char *input)
{
    int index = hash(input[0]);
    Category *category = (Category *)malloc(sizeof(Category));
    strcpy(category->name, input);
    category->next = NULL;

    if (hashTable[index].head == NULL)
    {
        hashTable[index].head = category;
        hashTable[index].tail = category;
    }
    else
    {
        hashTable[index].tail->next = category;
        hashTable[index].tail = category;
    }
}

char *search_category(HashTable *hashTable, const char *input)
{
    int index = hash(input[0]);
    Category *current = hashTable[index].head;
    while (current != NULL)
    {
        if (strcmp(current->name, input) == 0)
        {
            return current->name;
        }
        current = current->next;
    }
    return NULL;
}