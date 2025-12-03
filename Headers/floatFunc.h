#ifndef FLOAT_H
#define FLOAT_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isFloat(char[]);
int isDigit(char);
float toFloat(char[]);

/**
 * @brief isFloat function
 * @param line[] Description of line[]
 * @return Description of return value
 */
int isFloat(char line[])
{
    for (int i = 0; i < strlen(line); i++)
    {
        if (!isDigit(line[i]) && line[i] != '.' && line[i] != '\n' && !(i == 0 && line[i] == '-'))
        {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief isDigit function
 * @param c Description of c
 * @return Description of return value
 */
int isDigit(char c){
    if ('0' <= (int) c && (int) c  <= '9') { /* Is between 0 and 9 (48 and 57 using ascii) */
        return 1; /* True */
    } else {
        return 0; /* False */
    }
}

/**
 * @brief toFloat function
 * @param line[] Description of line[]
 * @return Description of return value
 */
float toFloat(char line[])
{
    int point = strlen(line);
    float num = 0;
    int divisor;
    int negative = (line[0] == '-') ? 1 : 0;

    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == '.')
        {
            point = i;
        }

        else if (line[i] == '\n' || line[i] == '\0')
        {
            break;
        }
        else
        {
            num *= 10;
            num += line[i] - '0';
        }
    }
    divisor = strlen(line) - point;
    for (int i = 0; i < divisor - 1; i++)
    {
        num /= 10;
    }
    return (!negative) ? num : num * -1;
}

#endif