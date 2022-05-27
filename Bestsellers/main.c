#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


typedef struct Item {
    size_t id, count;
    char name[100];
} Item;


size_t list_size = 100, num_elements = 0;
Item *items;
int top_num;

/* Simple linear search function for finding if an item with requested name exists */
long searchItem(const char *name, Item *list, size_t item_num) {
    for (size_t i = 0; i < item_num; i++)
        if (strcmp(list[i].name, name) == 0)
            return i;
    return -1;
}


/* Comparator to sort Items */
int cmp(const void *left, const void *right) {
    return ((Item *) left)->count < ((Item *) right)->count;
}


/* Minimum of two */
size_t min(size_t left, size_t right) {
    if (left > right) return right;
    return left;
}


/* A function to get the range of positions of an element; */
size_t res[2];

size_t *getPos(Item *list, size_t position, size_t range) {
    res[1] = range + 1;
    for (size_t i = 0; i <= position; i++)
        if (list[i].count == list[position].count) {
            res[0] = i + 1;
            break;
        }
    for (size_t i = position; i <= range; i++)
        if (list[i].count != list[position].count) {
            res[1] = i;
            break;
        }
    return res;
}


void addItem() {
    char item_name[100], whitespace;
    if (scanf("%99s%c", item_name, &whitespace) != 2 || (whitespace != ' ' && whitespace != '\n')) {
        printf("Invalid input.\n");
        free(items);
        exit(1);
    }
    long index = searchItem(item_name, items, num_elements);
    if (index != -1) {
        items[index].count += 1;
    } else {
        // Realloc if out of space:
        if (num_elements >= list_size) {
            list_size *= 2;
            items = (Item *) realloc(items, (list_size * sizeof(Item)));
        }
        Item tmpItem;
        tmpItem.count = 1;
        strcpy(tmpItem.name, item_name);
        items[num_elements++] = tmpItem;
    }
}

void processRequest(char operation) {
    size_t top_sellers_count = 0;
    qsort(items, num_elements, sizeof(Item), cmp);
    size_t pos = top_num, last_in_top_count = items[top_num - 1].count;
    for (size_t i = pos; i < num_elements; i++)
        if (items[i].count == last_in_top_count)
            pos += 1;
        else break;
    for (size_t i = 0; i < min(pos, num_elements); i++) {
        if (operation == '#') {
            size_t *positions = getPos(items, i, pos);
            if (positions[0] != positions[1]) {
                printf("%zu.", positions[0]);
                printf("-%zu.", positions[1]);
            } else printf("%zu.", positions[1]);
            printf(" %s, %zux\n", items[i].name, items[i].count);
        }
        top_sellers_count += items[i].count;
    }
    printf("Top sellers: sold %ld items\n", top_sellers_count);
}

void invalidInput() {
    printf("Invalid input.\n");
    free(items);
    exit(1);
}


int main() {
    items = (Item *) malloc(sizeof(Item) * list_size);
    char operation;
    printf("Top N:\n");
    scanf("%d", &top_num);
    if (top_num <= 0) invalidInput();
    printf("Requests:\n");

    while (1) {
        /* Operation input */
        int scan_int = scanf(" %c", &operation);
        if (scan_int == EOF) break;
        if (scan_int != 1) invalidInput();

        /* Addition */
        if (operation == '+') {
            addItem();
        }

        /* Best + num sold */
        else if (operation == '#' || operation == '?') {
            processRequest(operation);
        }

        /* Invalid operation symbol */
        else invalidInput();
    }


    free(items);
    return 0;
}
