#include <stdio.h>
#include <stdlib.h>


#include "objimp.h"

int main(void) {
    Object obj;
    loadObject("Models/blue42/blue42.obj", &obj);
    for (int i = 0; i < obj.orderSize; i += 3) {
        printf("vertex %2d | %5d %5d %5d\n", i, obj.vertexOrder[i], obj.vertexOrder[i + 1], obj.vertexOrder[i + 2]);
    }
}