#ifndef FILESTREAM_H
#define FILESTREAM_H

#include <stdio.h>
#include <stdlib.h>

/*---Function Prototypes---*/
static void readLine(FILE *file, char *line);


static void readLine(FILE *file, char *line) {
    fgets(line, sizeof(line), file);
}




#endif