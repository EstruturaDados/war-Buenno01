#ifndef MODELS_H
#define MODELS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STR_LENGTH 50

// enum para definir o tipo de missão
enum ObjectiveType {
    DESTROY_ARMY,
    CONQUER_TERRITORIES
};

// estrutura para definir o país
struct Country {
    char name[MAX_STR_LENGTH];
    char army[MAX_STR_LENGTH];
    int troops;
};

// estrutura para definir a missão
struct Objective {
    int id;
    char name[MAX_STR_LENGTH];
    char description[MAX_STR_LENGTH];
    bool completed;
    char army[MAX_STR_LENGTH];
    enum ObjectiveType type;
    char target[MAX_STR_LENGTH];
    int targetCount;
};

// estrutura para definir a ação
struct Action {
    int key;
    char name[MAX_STR_LENGTH];
    void (*function)(struct Country *, int, struct Action *, struct Objective *);
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