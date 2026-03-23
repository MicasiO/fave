#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_str_arr(str_arr* arr) {
    arr->size = 0;
    arr->capacity = 4;
    arr->items = (char**)calloc(arr->capacity, sizeof(*arr->items));

    if (arr->items == NULL) {
        perror("Failed to allocate memory for string array");
        return 1;
    }

    return 0;
}

int push_str_arr(str_arr* arr, const char* str) {
    if (arr->size == arr->capacity) {
        int new_capacity = arr->capacity * 2;

        char** temp = realloc(arr->items, sizeof(char*) * new_capacity);

        if (temp == NULL) {
            perror("Failed to resize array");
            return 1;
        }

        arr->items = temp;
        arr->capacity = new_capacity;
    }

    arr->items[arr->size] = strdup(str);

    if (arr->items[arr->size] == NULL) {
        return 1;
    }

    arr->size++;
    return 0;
}

int pop_str_arr(str_arr* arr, const char* str) {
    int found_index = -1;

    for (int i = 0; i < arr->size; i++) {
        if (strcmp(arr->items[i], str) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        return 1;
    }

    free(arr->items[found_index]);

    for (int i = found_index; i < arr->size - 1; i++) {
        arr->items[i] = arr->items[i + 1];
    }

    arr->size--;

    arr->items[arr->size] = NULL;

    return 0;
}

void free_str_arr(str_arr* arr) {
    if (arr == NULL) {
        return;
    }

    for (int i = 0; i < arr->size; i++) {
        if (arr->items[i] != NULL) {
            free(arr->items[i]);
        }
    }

    free(arr->items);

    arr->items = NULL;
    arr->capacity = 0;
    arr->size = 0;
}
