#ifndef WAR_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "models.h"
#include "user-interface.h"

#define WAR_H

#define MAX_COUNTRIES 5
#define MAX_STR_LENGTH 50

void attackCountry(struct Country *countries, int totalCountries);
void exitGame(struct Country *countries, struct Action *actions);
void mountPresetCountries(struct Country *countries);
int declareCountries(struct Country *countries, int usePresetCountries);
int rollDice(void);
struct Country *alocateCountriesMemory();
void freeCountriesMemory(struct Country *countries);
void freeActionMemory(struct Action *actions);

void freeCountriesMemory(struct Country *countries) {
    free(countries);
}

void freeActionMemory(struct Action *actions) {
    free(actions);
}

// Função para alocar memória para os territórios
struct Country *alocateCountriesMemory() {
    struct Country *countries = (struct Country *)calloc(MAX_COUNTRIES, sizeof(struct Country));

    if (countries == NULL) {
        printf("Erro ao alocar memória para os territórios\n");
        exit(1);
    }

    return countries;
}

void exitGame(struct Country *countries, struct Action *actions) {
    freeActionMemory(actions);
    freeCountriesMemory(countries);
    printf("Saindo do jogo...\n");
    exit(0);
    return;
}

// Função para rolar um dado
int rollDice() {
    return rand() % 6 + 1;
}

// Função para atacar um território
void attackCountry(struct Country *countries, int totalCountries) {
    int attackerIndex = 0;
    int defenderIndex = 0;
    printCountries(countries, "MAPA ATUAL", totalCountries);

    struct InputField territoryFields[] = {
        {"Número do território atacante", "int"},
        {"Número do território defensor", "int"}
    };

    struct CollectedData *territoryData = collectUserInputs(territoryFields, 2, "Batalha");

    if (territoryData == NULL) {
        printTitle("Erro ao coletar dados da batalha\n");
        return;
    }

    attackerIndex = territoryData[0].intValue;
    defenderIndex = territoryData[1].intValue;

    freeCollectedData(territoryData);

    struct Country *attacker = &countries[attackerIndex - 1];
    struct Country *defender = &countries[defenderIndex - 1];

    if (attacker->army == NULL || defender->army == NULL || strcmp(attacker->army, defender->army) == 0) {
        printTitle("Atacante e defensor não podem ser do mesmo exército\n");
        return;
    }

    if (attacker->troops < 2) {
        printTitle("Atacante não pode ter menos de 2 tropas\n");
        return;
    }

    int attackerRoll = rollDice();
    int defenderRoll = rollDice();

    if (attackerRoll > defenderRoll) {
        defender->troops--;
        if (defender->troops <= 0) {
            strcpy(defender->army, attacker->army);
            defender->troops += 1;
            attacker->troops -= 1;
        }

    } else {
        attacker->troops--;
    }

    int winnerIsAttacker = attackerRoll > defenderRoll;
    int winnerConquered = defender->troops <= 0;

    printAttackResult(attacker, defender, attackerRoll, defenderRoll, winnerIsAttacker, winnerConquered);

    printf("Aperte 'Enter' para continuar\n");
    getchar();

    printCountries(countries, "MAPA ATUAL", totalCountries);
}

// Função para declarar os territórios
int declareCountries(struct Country *countries, int usePresetCountries) {
    int totalCountries = 0;

    if (usePresetCountries) {
        mountPresetCountries(countries);
        totalCountries = MAX_COUNTRIES;
        return totalCountries;
    }

    for (int i = 0; i < MAX_COUNTRIES; i++) {
        // Loop para cadastrar 5 territórios
        printTitle("Cadastro de Territórios");

        // Define os campos para coleta de dados do território
        struct InputField territoryFields[] = {
            {"Nome do território", "string"},
            {"Exército do território (ex. Verde, Azul, etc.)", "string"},
            {"Quantidade de tropas do território", "int"}
        };
        
        // Coleta os dados do usuário
        struct CollectedData *territoryData = collectUserInputs(territoryFields, 3, "Cadastro de Território");
        
        if (territoryData != NULL) {
            // Copia os dados coletados para a estrutura do país
            strcpy(countries[i].name, territoryData[0].stringValue);
            strcpy(countries[i].army, territoryData[1].stringValue);
            countries[i].troops = territoryData[2].intValue;
            
            // Libera a memória dos dados coletados
            freeCollectedData(territoryData);
            
            // Incrementa o contador de territórios cadastrados
            totalCountries++;
        } else {
            printf("Erro ao coletar dados do território %d\n", i + 1);
            break;
        }
    }

    return totalCountries;
}

// Função para montar os territórios pré-definidos
void mountPresetCountries(struct Country *countries) {
    strcpy(countries[0].name, "Ásia");
    strcpy(countries[0].army, "Verde");
    countries[0].troops = 3;
    strcpy(countries[1].name, "África");
    strcpy(countries[1].army, "Azul");
    countries[1].troops = 3;
    strcpy(countries[2].name, "América");
    strcpy(countries[2].army, "Amarelo");
    countries[2].troops = 3;
    strcpy(countries[3].name, "Oceania");
    strcpy(countries[3].army, "Vermelho");
    countries[3].troops = 3;
    strcpy(countries[4].name, "Europa");
    strcpy(countries[4].army, "Roxo");
    countries[4].troops = 3;

    return;
}

#endif
