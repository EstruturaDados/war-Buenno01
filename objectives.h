#ifndef OBJECTIVES_H
#define OBJECTIVES_H

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "models.h"
#include "user-interface.h"

struct Objective *checkObjectiveCompletion(struct Objective *objectives, int totalObjectives, struct Country *countries, int totalCountries, struct Country *attacker);
void pickObjective(struct Objective *objectives, int totalObjectives);
struct Objective *createObjectives(struct Country *countries, int totalCountries);

struct Objective *createObjectives(struct Country *countries, int totalCountries) {    
    struct Objective *objectives = malloc(totalCountries * sizeof(struct Objective));

    if (objectives == NULL) {
        printf("Erro ao alocar memória para as missões\n");
        exit(1);
    }

    char existingArmies[totalCountries][MAX_STR_LENGTH];

    if (existingArmies == NULL) {
        printf("Erro ao alocar memória para os exércitos\n");
        exit(1);
    }

    for (int i = 0; i < totalCountries; i++) {
        strcpy(existingArmies[i], countries[i].army);
    }

    for (int i = 0; i < totalCountries; i++) {
        int random = rand() % 2;
        strcpy(objectives[i].army, countries[i].army);
        objectives[i].completed = false;
        objectives[i].id = i;
        if (random == 0) {
            objectives[i].type = DESTROY_ARMY;
            int targetIndex;
            char target[MAX_STR_LENGTH];

            while (strcmp(existingArmies[targetIndex], "") == 0 && 
            strcmp(existingArmies[targetIndex], countries[i].army) != 0) {
                targetIndex = rand() % totalCountries;
                strcpy(target, existingArmies[targetIndex]);
            }

            strcpy(objectives[i].target, target);
            strcpy(existingArmies[targetIndex], "");
            objectives[i].targetCount = 0;
        } else {
            objectives[i].type = CONQUER_TERRITORIES;
            objectives[i].targetCount = 3;
            strcpy(objectives[i].target, countries[i].army);
        }
    }

    for (int i = 0; i < totalCountries; i++) {
        printObjective(&objectives[i]);
    }

    return objectives;
}

struct Objective *checkObjectiveCompletion(struct Objective *objectives, int totalObjectives, struct Country *countries, int totalCountries, struct Country *attacker) {
    struct Objective *attackerObjective = NULL;

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

    for (int i = 0; i < totalCountries; i++) {
        if (strcmp(countries[i].army, attackerObjective->target) == 0) {
            targetTerritories++;
            break;
        }
    }

    if (attackerObjective->type == DESTROY_ARMY && targetTerritories <= 0) {
        attackerObjective->completed = true;
    } else if (attackerObjective->type == CONQUER_TERRITORIES && targetTerritories >= attackerObjective->targetCount) {
        attackerObjective->completed = true;
    }

    return attackerObjective;
}

#endif