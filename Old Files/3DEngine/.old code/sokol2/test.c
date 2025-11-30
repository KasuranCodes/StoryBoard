#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

void main() {
    int nums = {'1', '2', '3', '4', '5'};
    size_t size = 5;
    char value = '6';
    // char* new_nums[] = {"6", "7", "8", "9", "10"};
    // *nums = *new_nums;
    append((void**)&nums, &size, &value, sizeof(value));
    for (int i = 0; i < size; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");

}