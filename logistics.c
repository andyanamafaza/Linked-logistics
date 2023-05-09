#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX 50
#define DATA "data.txt"
#define CATEGORIES "categories.txt"

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

void insert_tail(Node **head, Product product)
{
    Node *node = create_node(product);
    if (*head == NULL)
    {
        *head = node;
        return;
    }
    Node *current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = node;
    node->prev = current;
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

void load_categories(char categories[][MAX], int *size)
{
    FILE *fp = fopen("category.txt", "r");
    if (fp == NULL)
    {
        printf("Error: Cannot open file\n");
        return;
    }
    char line[MAX];
    int i = 0;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line[strcspn(line, "\n")] = 0;
        strcpy(categories[i], line);
        i++;
    }
    *size = i;
    fclose(fp);
}

void save_categories(char categories[][MAX], int size)
{
    FILE *fp = fopen("category.txt", "w");
    if (fp == NULL)
    {
        printf("Error: Cannot open file\n");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "%s\n", categories[i]);
    }
    fclose(fp);
}

void delete_category(char categories[][MAX], int *size, int index)
{
    for (int i = index; i < *size - 1; i++)
    {
        strcpy(categories[i], categories[i + 1]);
    }
    *size -= 1;
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

void load_data(Node **head)
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
        insert_tail(head, product);
        free(name);
        free(category);
    }

    fclose(fp);
}

void swap_nodes(Node **head_ref, Node *node1, Node *node2)
{
    if (node1 == node2)
    {
        return;
    }

    Node *temp1 = node1->prev;
    Node *temp2 = node2->next;
    node1->prev = node2;
    node2->next = node1;
    if (temp1 != NULL)
    {
        temp1->next = node2;
    }
    else
    {
        *head_ref = node2;
    }
    node2->prev = temp1;
    if (temp2 != NULL)
    {
        temp2->prev = node1;
    }
    node1->next = temp2;
}

void sort_by_name(Node **head_ref, int order)
{
    int swapped, i;
    Node *node1;
    Node *node2 = NULL;

    if (*head_ref == NULL)
    {
        return;
    }

    do
    {
        swapped = 0;
        node1 = *head_ref;

        while (node1->next != node2)
        {
            if ((strcmp(node1->product.name, node1->next->product.name) > 0 && order == 1) || (strcmp(node1->product.name, node1->next->product.name) < 0 && order == -1))
            {
                swap_nodes(head_ref, node1, node1->next);
                swapped = 1;
            }
            else
            {
                node1 = node1->next;
            }
        }
        node2 = node1;
    } while (swapped);
}

void sort_by_category(Node **head_ref, int order)
{
    int swapped, i;
    Node *node1;
    Node *node2 = NULL;

    if (*head_ref == NULL)
    {
        return;
    }

    do
    {
        swapped = 0;
        node1 = *head_ref;

        while (node1->next != node2)
        {
            if ((strcmp(node1->product.category, node1->next->product.category) > 0 && order == 1) || (strcmp(node1->product.category, node1->next->product.category) < 0 && order == -1))
            {
                swap_nodes(head_ref, node1, node1->next);
                swapped = 1;
            }
            else
            {
                node1 = node1->next;
            }
        }
        node2 = node1;
    } while (swapped);
}

void sort_by_quantity(Node **head_ref, int order)
{
    int swapped, i;
    Node *node1;
    Node *node2 = NULL;

    if (*head_ref == NULL)
    {
        return;
    }

    do
    {
        swapped = 0;
        node1 = *head_ref;

        while (node1->next != node2)
        {
            if ((node1->product.quantity > node1->next->product.quantity && order == 1) || (node1->product.quantity < node1->next->product.quantity && order == -1))
            {
                swap_nodes(head_ref, node1, node1->next);
                swapped = 1;
            }
            else
            {
                node1 = node1->next;
            }
        }
        node2 = node1;
    } while (swapped);
}

int main()
{
    Node *head = NULL;
    char categories[MAX][MAX];
    int size = 0;
    load_categories(categories, &size);
    load_data(&head);
    system("cls");
    while (1)
    {
        printf("\n===== INVENTORY MANAGEMENT =====\n");
        printf("1. Add new product\n");
        printf("2. Add new categories\n");
        printf("3. Change product quantity\n");
        printf("4. Delete product\n");
        printf("5. Delete categories\n");
        printf("6. Search product\n");
        printf("7. Sort product\n");
        printf("8. Print all products\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        printf("\n");
        switch (choice)
        {
        case 1:
        {
            system("cls");
            Product product = {0};
            int input, categories_input;
            printf("Add new product in the head of the linked list or tail?\n");
            printf("1. Head\n");
            printf("2. Tail\n");
            scanf("%d", &input);

            if (input == 1)
            {
                printf("Enter name: ");
                scanf("%49s", product.name);
                printf("Enter quantity: ");
                scanf("%d", &product.quantity);
                printf("Choose product category:\n");
                for (int i = 0; i < size; i++)
                {
                    printf("%d. %s\n", i + 1, categories[i]);
                }
                scanf("%d", &categories_input);
                strcpy(product.category, categories[categories_input - 1]);
                insert_head(&head, product);
                save_data(head);
                printf("Product added at the head successfully\n");
            }
            else if (input == 2)
            {
                printf("Enter name: ");
                scanf("%49s", product.name);
                printf("Enter quantity: ");
                scanf("%d", &product.quantity);
                printf("Choose product category:\n");
                for (int i = 0; i < size; i++)
                {
                    printf("%d. %s\n", i + 1, categories[i]);
                }
                scanf("%d", &categories_input);
                strcpy(product.category, categories[categories_input - 1]);
                insert_tail(&head, product);
                save_data(head);
                printf("Product added at the tail successfully\n");
            }
            else
            {
                printf("Invalid input\n");
            }
            break;
        }
        case 2:
        {
            system("cls");
            char temp[MAX];
            printf("Enter new category: ");
            scanf("%49s", temp);
            strcpy(categories[size], temp);
            size++;
            save_categories(categories, size);
            printf("Category added successfully\n");
            break;
        }
        case 3:
        {
            system("cls");
            printf("Increase or decrease quantity?\n");
            printf("1. Increase\n");
            printf("2. Decrease\n");
            int input;
            scanf("%d", &input);
            if (input == 1)
            {
                char name[MAX];
                printf("Enter product name: ");
                scanf("%s", name);
                Node *node = search_by_name(head, name);
                if (node != NULL)
                {
                    int amount;
                    printf("Enter amount to increase: ");
                    scanf("%d", &amount);
                    increase_quantity(node, amount);
                    save_data(head);
                    printf("Quantity increased successfully\n");
                }
                else
                {
                    printf("Product not found\n");
                }
            }
            else if (input == 2)
            {
                char name[MAX];
                printf("Enter product name: ");
                scanf("%s", name);
                Node *node = search_by_name(head, name);
                if (node != NULL)
                {
                    int amount;
                    printf("Enter amount to decrease: ");
                    scanf("%d", &amount);
                    decrease_quantity(node, amount);
                    save_data(head);
                    printf("Quantity decreased successfully\n");
                }
                else
                {
                    printf("Product not found\n");
                }
            }
            else
            {
                printf("Invalid input\n");
            }
            break;
        }
        case 4:
        {
            system("cls");
            char name[MAX];
            printf("Enter product name: ");
            scanf("%s", name);
            Node *node = search_by_name(head, name);
            if (node != NULL)
            {
                delete_node(&head, node);
                save_data(head);
                printf("Product deleted successfully\n");
            }
            else
            {
                printf("Product not found\n");
            }
            break;
        }
        case 5:
        {
            system("cls");
            printf("Choose category to delete:\n");
            for (int i = 0; i < size; i++)
            {
                printf("%d. %s\n", i + 1, categories[i]);
            }
            int input;
            scanf("%d", &input);
            delete_category(categories, &size, input - 1);
            save_categories(categories, size);
            break;
        }
        case 6:
        {
            system("cls");
            char temp[MAX];
            printf("Search product by name or category?\n");
            printf("1. Name\n");
            printf("2. Category\n");
            int input;
            scanf("%d", &input);
            if (input == 1)
            {
                printf("Enter name: ");
                scanf("%s", temp);
                Node *node = search_by_name(head, temp);
                if (node != NULL)
                {
                    printf("\n=== SEARCH RESULTS ===\n");
                    printf("%-20s %-10s %-10s\n", "Name", "Quantity", "Category");
                    printf("%-20s %-10d %-10s\n", node->product.name, node->product.quantity, node->product.category);
                }
                else
                {
                    printf("Product not found\n");
                }
            }
            else if (input == 2)
            {
                printf("Choose category to search:\n");
                for (int i = 0; i < size; i++)
                {
                    printf("%d. %s\n", i + 1, categories[i]);
                }
                int input;
                scanf("%d", &input);
                Node *node = search_by_categories(head, categories[input - 1]);
                printf("\n=== SEARCH RESULTS ===\n");
                if (node == NULL)
                {
                    printf("Product with category %s not found\n", temp);
                }
                else
                {
                    printf("%-20s %-10s %-10s\n", "Name", "Quantity", "Category");
                }
                while (node != NULL)
                {
                    printf("%-20s %-10d %-10s\n", node->product.name, node->product.quantity, node->product.category);
                    node = search_by_categories(node->next, categories[input - 1]);
                }
            }
            else
            {
                printf("Invalid input\n");
            }
            break;
        }
        case 7:
        {
            system("cls");
            printf("Sort product by name, category, or quantity?\n");
            printf("1. Name\n");
            printf("2. Category\n");
            printf("3. Quantity\n");
            int input, input2;
            scanf("%d", &input);
            if (input == 1)
            {
                printf("Ascending or descending?\n");
                printf("1. Ascending\n");
                printf("2. Descending\n");
                scanf("%d", &input2);
                if (input2 == 1)
                {
                    sort_by_name(&head, 1);
                    save_data(head);
                    printf("Sorted by name in ascending order\n");
                }
                else if (input2 == 2)
                {
                    sort_by_name(&head, -1);
                    save_data(head);
                    printf("Sorted by name in descending order\n");
                }
                else
                {
                    printf("Invalid input\n");
                }
            }
            else if (input == 2)
            {
                printf("Ascending or descending?\n");
                printf("1. Ascending\n");
                printf("2. Descending\n");
                scanf("%d", &input2);
                if (input2 == 1)
                {
                    sort_by_category(&head, 1);
                    save_data(head);
                    printf("Sorted by category in ascending order\n");
                }
                else if (input2 == 2)
                {
                    sort_by_category(&head, -1);
                    save_data(head);
                    printf("Sorted by category in descending order\n");
                }
                else
                {
                    printf("Invalid input\n");
                }
            }
            else if (input == 3)
            {
                printf("Ascending or descending?\n");
                printf("1. Ascending\n");
                printf("2. Descending\n");
                scanf("%d", &input2);
                if (input2 == 1)
                {
                    sort_by_quantity(&head, 1);
                    save_data(head);
                    printf("Sorted by quantity in ascending order\n");
                }
                else if (input2 == 2)
                {
                    sort_by_quantity(&head, -1);
                    save_data(head);
                    printf("Sorted by quantity in descending order\n");
                }
                else
                {
                    printf("Invalid input\n");
                }
            }
            else
            {
                printf("Invalid input\n");
            }
            break;
        }
        case 8:
        {
            system("cls");
            printf("\n=============== ALL PRODUCTS ===============\n");
            print_list(head);
            break;
        }
        case 9:
        {
            save_categories(categories, size);
            save_data(head);
            exit(0);
        }
        default:
        {
            printf("Invalid choice\n");
        }
        }
    }
    return 0;
}