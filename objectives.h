#ifndef OBJECTIVES_H
#define OBJECTIVES_H

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "models.h"
#include "user-interface.h"

// pré-declaração das funções
struct Objective *checkObjectiveCompletion(struct Objective *objectives, int totalObjectives, struct Country *countries, int totalCountries, struct Country *attacker);
void pickObjective(struct Objective *objectives, int totalObjectives);
struct Objective *createObjectives(struct Country *countries, int totalCountries);

// Função para criar as missões
struct Objective *createObjectives(struct Country *countries, int totalCountries) {    
    // aloca memória para as missões
    struct Objective *objectives = malloc(totalCountries * sizeof(struct Objective));

    if (objectives == NULL) {
        printf("Erro ao alocar memória para as missões\n");
        exit(1);
    }

    // aloca memória para os exércitos
    char existingArmies[totalCountries][MAX_STR_LENGTH];

    if (existingArmies == NULL) {
        printf("Erro ao alocar memória para os exércitos\n");
        exit(1);
    }

    // copia os exércitos para o array de exércitos
    for (int i = 0; i < totalCountries; i++) {
        strcpy(existingArmies[i], countries[i].army);
    }

    // Loop para criar as missões
    for (int i = 0; i < totalCountries; i++) {
        // gera um número aleatório para definir o tipo de missão
        int random = rand() % 2;

        // copia o exército do loop atual para a missão
        strcpy(objectives[i].army, countries[i].army);

        // define que a missão não foi completada e um id
        objectives[i].completed = false;
        objectives[i].id = i;

        // se o número aleatório for 0, define que a missão é destruir o exército
        if (random == 0) {
            objectives[i].type = DESTROY_ARMY;
            int targetIndex;

            // aloca memória para o alvo
            char target[MAX_STR_LENGTH];

            // Loop para encontrar um alvo para a missão
            // o alvo não pode ser o mesmo exército do loop atual e não pode ser vazio
            while (strcmp(existingArmies[targetIndex], "") == 0 && 
            strcmp(existingArmies[targetIndex], countries[i].army) != 0) {
                // gera um número aleatório para definir o alvo
                targetIndex = rand() % totalCountries;
                // copia o alvo para a variabel intermediaria
                strcpy(target, existingArmies[targetIndex]);
            }

            // copia o alvo para a missão
            strcpy(objectives[i].target, target);
            // define o alvo como vazio na lista que gerencia os exércitos já usados como alvos
            strcpy(existingArmies[targetIndex], "");
            // define a quantidade de alvos como 0
            objectives[i].targetCount = 0;
        } else {
            // se o número aleatório for 1, define que a missão é conquistar os territórios
            objectives[i].type = CONQUER_TERRITORIES;
            objectives[i].targetCount = 3;
            strcpy(objectives[i].target, countries[i].army);
        }
    }

    // Loop para imprimir as missões
    for (int i = 0; i < totalCountries; i++) {
        printObjective(&objectives[i]);
    }

    return objectives;
}

// Função para verificar se a missão foi completada
struct Objective *checkObjectiveCompletion(struct Objective *objectives, int totalObjectives, struct Country *countries, int totalCountries, struct Country *attacker) {
    struct Objective *attackerObjective = NULL;

    // Loop para buscar a missão do atacante
    for (int i = 0; i < totalObjectives; i++) {
        if (strcmp(objectives[i].army, attacker->army) == 0) {
            attackerObjective = &objectives[i];
            break;
        }
    }

    if (attackerObjective == NULL) {
        return NULL;
    }

    int targetTerritories = 0;

    // Loop para buscar os territórios alvo da missão
    for (int i = 0; i < totalCountries; i++) {
        if (strcmp(countries[i].army, attackerObjective->target) == 0) {
            targetTerritories++;
            break;
        }
    }

    if (attackerObjective->type == DESTROY_ARMY && targetTerritories <= 0) {
        // se a missão é destruir o exército e não foi encontrado nenhum território alvo,
        // a missão é completada, já que o exército foi destruído e por isso não há
        // mais territórios pertencentes ao alvo da missão
        attackerObjective->completed = true;
    } else if (attackerObjective->type == CONQUER_TERRITORIES && targetTerritories >= attackerObjective->targetCount) {
        // se a missão é conquistar os territórios e a quantidade de territórios
        // alvo foi atingida, a missão é completada
        attackerObjective->completed = true;
    }

    return attackerObjective;
}

#endif