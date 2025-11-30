#include <stdio.h>

int main() {
    FILE *file = fopen("example.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char buffer[256];  // line buffer
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("Line: %s", buffer); // `fgets` includes the newline
    }

    fclose(file);
    return 0;
}
