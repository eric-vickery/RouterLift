#include <Arduino.h>
#include <stdint.h>

#include "utils.h"

char* trimString(char* text)
{
    char* newTextStart = text;

    // Find the first non-space character on the front of the string
    int i;
    for (i = 0; i < strlen(text); i++)
    {
        if (*(text+i) != ' ')
        {
            break;
        }
    }

    if (i < strlen(text))
    {
        newTextStart = text + i;
    }

    // Trim any whitespace off the end
    for (i = strlen(text) - 1; i > 0; i--)
    {
        if (*(text+i) == ' ')
        {
            *(text+i) = 0;
        }
    }
    return newTextStart;
}