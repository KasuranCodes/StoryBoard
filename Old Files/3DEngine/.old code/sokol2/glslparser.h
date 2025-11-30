#ifndef GLSLPARSER_H
#define GLSLPARSER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---Function Prototypes---*/
static void parseShader(const char *filepath, char **vs_src, char **fs_src);


/*---Shader Parser---*/
static void parseShader(const char *filepath, char **vs_src, char **fs_src) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        *vs_src = NULL;
        *fs_src = NULL;
        return;
    }
    
    /*---LoCAL Declarables---*/
    char* buffer = NULL; // Buffer to hold the shader source code
    int mode = 0; // 0 = no shader, 1 = vertex shader, 2 = fragment shader
    int read = 0;
    size_t len = 0;
    *vs_src = (char *)calloc(1024, 1);
    *fs_src = (char *)calloc(1024, 1);
    

    while ((read = getline(&buffer, &len, file)) != EOF) {
        // printf("%s", buffer); // Print the line read from the file
        
        if (strncmp(buffer, "//#vertex\n", 11) == 0) {
            mode = 1; // Set vertex shader mode
        } 
        else if (strncmp(buffer, "//#fragment\n", 13) == 0) {
            mode = 2; // Set fragment shader mode
        }
        else {   
            if (mode == 1) {
                strcat(*vs_src, buffer);
            } 
            else if (mode == 2) {
                strcat(*fs_src, buffer);
            }
        }
        // strcpy(buffer, ""); // Initialize buffer to empty string
    }

    free(buffer);
    fclose(file);

}
#endif // GLSLPARSER_H