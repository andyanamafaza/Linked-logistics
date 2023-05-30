#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "category.h"
#include "product.h"

#define MAX 50
#define DATA "data.txt"
#define CATEGORIES "categories.txt"
#define HASH_SIZE 26

int main()
{
    Node *head = NULL;
    Node *tail = NULL;
    HashTable hashTable[HASH_SIZE];
    initialize(hashTable);

    system("cls");
    load_categories(hashTable);
    printf("Loading data...\n");
    load_data(&head, &tail);
    int choice;
    do
    {
        printf("\n===== INVENTORY MANAGEMENT =====\n");
        printf("1. Add new categories\n");
        printf("2. Add new product\n");
        printf("3. Change product quantity\n");
        printf("4. Delete product\n");
        printf("5. Delete categories\n");
        printf("6. Search product\n");
        printf("7. Sort product\n");
        printf("8. Print all products\n");
        printf("9. Print all categories\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            system("cls");
            char category[MAX];
            printf("Enter category name to add: ");
            scanf(" %49[^\n]", category);
            if (search_category(hashTable, category) != NULL)
            {
                printf("Category already exists!\n");
            }
            else
            {
                add_category(hashTable, category);
                printf("Category added successfully!\n");
            }
            break;
        }
        case 2:
        {
            system("cls");
            Product product;
            int place;
            int valid1 = 0;
            int valid2 = 0;
            do
            {
                printf("Add new product in the beginning or end of the list\n");
                printf("1. Beginning\n");
                printf("2. End\n");
                printf("Enter your choice: ");
                char input[1];
                scanf("%s", input);
                place = atoi(input);
                if (place == 1 || place == 2)
                {
                    valid1 = 1;
                }
                else
                {
                    printf("Invalid choice!\n");
                }
            } while (valid1 == 0);
            printf("Enter product name: ");
            scanf(" %49[^\n]", product.name);
            if (search_by_name(head, tail, product.name) != NULL)
            {
                printf("Product already exists!\n");
                break;
            }
            printf("Enter product quantity: ");
            scanf("%d", &product.quantity);
            do
            {
                print_categories(hashTable);
                printf("Enter product category: ");
                scanf(" %49[^\n]", product.category);
                if (search_category(hashTable, product.category) != NULL)
                {
                    valid2 = 1;
                }
                else
                {
                    printf("Category not found!\n");
                }
            } while (valid2 == 0);
            switch (place)
            {
            case 1:
                insert_head(&head, product);
                break;
            case 2:
                insert_tail(&head, &tail, product);
                break;
            }
            printf("Product %s with quantity %d and category %s added successfully!\n", product.name, product.quantity, product.category);
            break;
        }
        case 3:
        {
            system("cls");
            char name[MAX];
            int type;
            int amount;
            int valid = 0;
            int valid2 = 0;
            do
            {
                printf("Increase or decrease product quantity\n");
                printf("1. Increase\n");
                printf("2. Decrease\n");
                printf("Enter your choice: ");
                char input[1];
                scanf("%s", input);
                type = atoi(input);
                if (type == 1 || type == 2)
                {
                    valid = 1;
                }
                else
                {
                    printf("Invalid choice!\n");
                }
            } while (valid == 0);
            do
            {
                printf("Enter product name: ");
                scanf(" %49[^\n]", name);
                Node *node = search_by_name(head, tail, name);
                if (node != NULL)
                {
                    switch (type)
                    {
                    case 1:
                        printf("Enter amount to increase: ");
                        scanf("%d", &amount);
                        increase_quantity(node, amount);
                        printf("Product quantity increased successfully!\n");
                        break;
                    case 2:
                        printf("Enter amount to decrease: ");
                        scanf("%d", &amount);
                        decrease_quantity(node, amount);
                        printf("Product quantity decreased successfully!\n");
                        break;
                    }
                    valid2 = 1;
                }
                else
                {
                    printf("Product not found!\n");
                }
            } while (valid2 == 0);
            break;
        }
        case 4:
        {
            system("cls");
            char name[MAX];
            printf("Enter product name to delete: ");
            scanf(" %49[^\n]", name);
            Node *node = search_by_name(head, tail, name);
            if (node != NULL)
            {
                delete_node(&head, node);
                printf("Product deleted successfully!\n");
            }
            else
            {
                printf("Product not found!\n");
            }
            break;
        }
        case 5:
        {
            system("cls");
            char category[MAX];
            printf("Enter category name to delete: ");
            scanf(" %49[^\n]", category);
            if (search_category(hashTable, category) != NULL)
            {
                delete_category(hashTable, category);
                Node *node1 = head;
                Node *node2 = tail;
                while (node1 != NULL && node2 != NULL && node1 != node2->next)
                {
                    if (strcmp(node1->product.category, category) == 0)
                    {
                        delete_node(&head, node1);
                    }
                    if (strcmp(node2->product.category, category) == 0)
                    {
                        delete_node(&tail, node2);
                    }
                    node1 = node1->next;
                    node2 = node2->prev;
                }
                printf("Category deleted successfully!\n");
            }
            else
            {
                printf("Category not found!\n");
            }
            break;
        }
        case 6:
        {
            system("cls");
            char name[MAX];
            int valid = 0;
            int input;
            do
            {
                printf("Search by name or category\n");
                printf("1. Name\n");
                printf("2. Category\n");
                printf("Enter your choice: ");
                char input1[1];
                scanf("%s", input1);
                input = atoi(input1);
                if (input == 1 || input == 2)
                {
                    valid = 1;
                }
                else
                {
                    printf("Invalid choice!\n");
                }
            } while (valid == 0);
            switch (input)
            {
            case 1:
            {
                printf("Enter product name to search: ");
                scanf(" %49[^\n]", name);
                Node *node = search_by_name(head, tail, name);
                if (node != NULL)
                {
                    printf("Product found!\n");
                    printf("Name: %s\n", node->product.name);
                    printf("Quantity: %d\n", node->product.quantity);
                    printf("Category: %s\n", node->product.category);
                }
                else
                {
                    printf("Product not found!\n");
                }
                break;
            }
            case 2:
            {
                printf("Enter category name to search: ");
                scanf(" %49[^\n]", name);
                search_by_categories(head, name);
            }
            }
            break;
        }
        case 7:
        {
            system("cls");
            int order, type;
            int valid1 = 0;
            int valid2 = 0;
            do
            {
                printf("Select sorting order:\n");
                printf("1. Ascending\n");
                printf("2. Descending\n");
                printf("Enter your choice: ");
                char input1[1];
                scanf("%s", input1);
                order = atoi(input1);
                if (order == 1 || order == 2)
                {
                    valid1 = 1;
                }
                else
                {
                    printf("Invalid choice!\n");
                }
            } while (valid1 == 0);
            do
            {
                printf("Select sorting type:\n");
                printf("1. Name\n");
                printf("2. Category\n");
                printf("3. Quantity\n");
                printf("Enter your choice: ");
                char input2[1];
                scanf("%s", input2);
                type = atoi(input2);
                if (type == 1 || type == 2 || type == 3)
                {
                    valid2 = 1;
                    merge_sort(&head, order, type);
                    printf("Products sorted successfully!\n");
                }
                else
                {
                    printf("Invalid choice!\n");
                }
            } while (valid2 == 0);
            break;
        }
        case 8:
        {
            system("cls");
            if (head != NULL)
            {
                printf("All Products:\n");
                print_list(head);
            }
            else
            {
                printf("No products available!\n");
            }
            break;
        }
        case 9:
        {
            system("cls");
            printf("Categories:\n");
            print_categories(hashTable);
            break;
        }
        case 10:
        {
            save_data(head);
            save_categories(hashTable);
            printf("Exiting program... Goodbye!\n");
            break;
        }
        default:
        {

            printf("Invalid choice! Please try again.\n");
            break;
        }
        }
    } while (choice != 10);

    return 0;
}
