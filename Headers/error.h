#pragma once

#define ERRORS(ERROR, FORMAT, ...) {                                                                                                        \
    int macroErrorCode = ERROR;                                                                                                             \
    if(macroErrorCode) {                                                                                                                    \
        fprintf(stderr, "%s -> %s -> %i -> Error(%i):\n\t" FORMAT "\n", __FILE__, __func__, __LINE__, macroErrorCode, ##__VA_ARGS__);  \
    }                                                                                                                                       \
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
