#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define MAX 50
#define DATA "data.txt"
#define CATEGORIES "categories.txt"
#define HASH_SIZE 26

typedef struct Product
{
    char name[MAX];
    int quantity;
    char category[MAX];
} Product;

typedef struct Node
{
    Product product;
    struct Node *prev;
    struct Node *next;
} Node;

Node *create_node(Product product)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->product = product;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void insert_head(Node **head, Product product)
{
    Node *node = create_node(product);
    node->next = *head;
    if (*head != NULL)
    {
        (*head)->prev = node;
    }
    *head = node;
}

void insert_tail(Node **head, Node **tail, Product product)
{
    Node *node = create_node(product);
    if (*head == NULL)
    {
        *head = node;
        *tail = node;
        return;
    }
    (*tail)->next = node;
    node->prev = *tail;
    *tail = node;
}

void print_list(Node *head)
{
    int counter = 1;
    printf("%-2s %-20s %-10s %-10s\n", "No", "Name", "Quantity", "Category");
    while (head != NULL)
    {
        printf("%-2d %-20s %-10d %-10s\n", counter, head->product.name, head->product.quantity, head->product.category);
        head = head->next;
        counter++;
    }
}

Node *search_by_name(Node *head, char *name)
{
    while (head != NULL)
    {
        if (strcmp(head->product.name, name) == 0)
        {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

Node *search_by_categories(Node *head, char *category)
{
    while (head != NULL)
    {
        if (strcmp(head->product.category, category) == 0)
        {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

void delete_node(Node **head, Node *node)
{
    if (*head == node)
    {
        *head = node->next;
    }
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    free(node);
}

void increase_quantity(Node *node, int amount)
{
    node->product.quantity += amount;
}

void decrease_quantity(Node *node, int amount)
{
    if (node->product.quantity >= amount)
    {
        node->product.quantity -= amount;
    }
    else
    {
        printf("Error: Not enough quantity in the product\n");
    }
}

void save_data(Node *head)
{
    FILE *fp = fopen(DATA, "w");
    if (fp == NULL)
    {
        printf("Error: Cannot open file %s\n", DATA);
        return;
    }
    while (head != NULL)
    {
        fprintf(fp, "%s,%d,%s\n", head->product.name, head->product.quantity, head->product.category);
        head = head->next;
    }
    fclose(fp);
}

void load_data(Node **head, Node **tail)
{
    FILE *fp = fopen(DATA, "r");
    if (fp == NULL)
    {
        printf("Error: Cannot open file %s\n", DATA);
        return;
    }
    char line[200];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (line[strlen(line) - 1] != '\n')
        {
            printf("Error: Line too long\n");
            return;
        }
        char *name = NULL, *category = NULL;
        name = strdup("");
        category = strdup("");
        int quantity;
        sscanf(line, "%[^,],%d,%[^,\n]", name, &quantity, category);
        Product product = {0};
        strcpy(product.name, name);
        strcpy(product.category, category);
        product.quantity = quantity;
        insert_tail(head, tail, product);
        free(name);
        free(category);
    }

    fclose(fp);
}

Node *merge(Node *left, Node *right, int order, int type)
{
    if (left == NULL)
        return right;
    if (right == NULL)
        return left;

    Node *result = NULL;
    switch (type)
    {
    case 1:
        if ((strcmp(left->product.name, right->product.name) <= 0 && order == 1) ||
            (strcmp(left->product.name, right->product.name) >= 0 && order == -1))
        {
            result = left;
            result->next = merge(left->next, right, order, type);
        }
        else
        {
            result = right;
            result->next = merge(left, right->next, order, type);
        }
        break;
    case 2:
        if ((strcmp(left->product.category, right->product.category) <= 0 && order == 1) ||
            (strcmp(left->product.category, right->product.category) >= 0 && order == -1))
        {
            result = left;
            result->next = merge(left->next, right, order, type);
        }
        else
        {
            result = right;
            result->next = merge(left, right->next, order, type);
        }
        break;
    case 3:
        if ((left->product.quantity <= right->product.quantity && order == 1) ||
            (left->product.quantity >= right->product.quantity && order == -1))
        {
            result = left;
            result->next = merge(left->next, right, order, type);
        }
        else
        {
            result = right;
            result->next = merge(left, right->next, order, type);
        }
        break;
    }

    result->next->prev = result;
    return result;
}

void split(Node *head, Node **left, Node **right)
{
    Node *fast = head;
    Node *slow = head;
    while (fast->next != NULL && fast->next->next != NULL)
    {
        fast = fast->next->next;
        slow = slow->next;
    }

    *left = head;
    *right = slow->next;
    slow->next = NULL;
}

void merge_sort(Node **head_ref, int order, int type)
{
    Node *head = *head_ref;
    if (head == NULL || head->next == NULL)
        return;

    Node *left;
    Node *right;
    split(head, &left, &right);

    merge_sort(&left, order, type);
    merge_sort(&right, order, type);

    *head_ref = merge(left, right, order, type);
}