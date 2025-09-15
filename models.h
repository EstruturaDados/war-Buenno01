#ifndef MODELS_H
#define MODELS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR_LENGTH 50

struct Country {
    char name[MAX_STR_LENGTH];
    char army[MAX_STR_LENGTH];
    int troops;
};

struct Action {
    int key;
    char name[MAX_STR_LENGTH];
    void (*function)(struct Country *, int, struct Action *);
};

// Estrutura para definir campos de entrada
struct InputField {
    char label[MAX_STR_LENGTH];
    char type[20]; // "string", "int", "float"
};

// Estrutura para armazenar dados coletados
struct CollectedData {
    char stringValue[MAX_STR_LENGTH];
    int intValue;
    char type[20];
};

#endif