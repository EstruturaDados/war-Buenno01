#ifndef WAR_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "models.h"
#include "user-interface.h"
#include "objectives.h"

#define WAR_H

#define MAX_COUNTRIES 5
#define MAX_STR_LENGTH 50

// pré-declaração das funções
void attackCountry(struct Country *countries, int totalCountries, struct Action *actions, struct Objective *objectives);
void exitGame(struct Country *countries, struct Action *actions, struct Objective *objectives);
void mountPresetCountries(struct Country *countries);
int declareCountries(struct Country *countries, int usePresetCountries);
int rollDice(void);
struct Country *alocateCountriesMemory();
void freeCountriesMemory(struct Country *countries);
void freeActionMemory(struct Action *actions);
void freeObjectiveMemory(struct Objective *objectives);

// funções de liberação de memória
void freeCountriesMemory(struct Country *countries) {
    free(countries);
}

void freeActionMemory(struct Action *actions) {
    free(actions);
}

void freeObjectiveMemory(struct Objective *objectives) {
    free(objectives);
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

// Função para sair do jogo e liberar a memória
void exitGame(struct Country *countries, struct Action *actions, struct Objective *objectives) {
    if (actions != NULL) {
        freeActionMemory(actions);
    }
    if (countries != NULL) {
        freeCountriesMemory(countries);
    }
    if (objectives != NULL) {
        freeObjectiveMemory(objectives);
    }
    printf("Saindo do jogo...\n");
    exit(0);
    return;
}

// Função para rolar um dado
int rollDice() {
    return rand() % 6 + 1;
}

// Função para atacar um território
void attackCountry(struct Country *countries, int totalCountries, struct Action *actions, struct Objective *objectives) {
    int attackerIndex = 0;
    int defenderIndex = 0;
    printCountries(countries, "MAPA ATUAL", totalCountries);

    struct InputField territoryFields[] = {
        {"Número do território atacante", "int"},
        {"Número do território defensor", "int"}
    };

    // coleta os dados do usuário retornando um array com duas posições
    // a primeira posição é o número do território atacante
    // a segunda posição é o número do território defensor
    struct CollectedData *territoryData = collectUserInputs(territoryFields, 2, "Batalha");

    if (territoryData == NULL) {
        printTitle("Erro ao coletar dados da batalha\n");
        return;
    }

    // atribui os valores coletados das estruturas de input para as variáveis
    attackerIndex = territoryData[0].intValue;
    defenderIndex = territoryData[1].intValue;

    freeCollectedData(territoryData);

    // utiliza os índices coletados para buscar os territórios atacante e defensor
    struct Country *attacker = &countries[attackerIndex - 1];
    struct Country *defender = &countries[defenderIndex - 1];

    // verifica se o atacante e o defensor são do mesmo exército
    if (attacker->army == NULL || defender->army == NULL || strcmp(attacker->army, defender->army) == 0) {
        printTitle("Atacante e defensor não podem ser do mesmo exército\n");
        return;
    }

    // O atacante só pode atacar se tiver pelo menos 2 tropas
    if (attacker->troops < 2) {
        printTitle("Atacante não pode ter menos de 2 tropas\n");
        return;
    }

    int attackerRoll = rollDice();
    int defenderRoll = rollDice();
    int winnerIsAttacker = 0;
    int winnerConquered = 0;

    if (attackerRoll > defenderRoll) {
        // quando o atacante vence, o defensor perde 1 tropa
        defender->troops--;
        winnerIsAttacker = 1;
        if (defender->troops <= 0) {
            // quando o defensor perde todas as tropas, o território é conquistado pelo atacante
            strcpy(defender->army, attacker->army);
            defender->troops += 1;
            attacker->troops -= 1;
            winnerConquered = 1;
        }
    } else {
        // quando o defensor vence, o atacante perde 1 tropa
        attacker->troops--;
    }

    // imprime o resultado da batalha
    printAttackResult(attacker, defender, attackerRoll, defenderRoll, winnerIsAttacker, winnerConquered);

    // verifica se a missão foi completada
    struct Objective *objective = checkObjectiveCompletion(objectives, totalCountries, countries, totalCountries, attacker);

    if (objective != NULL && objective->completed) {
        // se a missão foi completada, imprime o resultado e sai do jogo
        printTitle("Missão completada!\n");
        printObjective(objective);

        printf("Aperte 'Enter' para continuar\n");
        getchar();

        exitGame(countries, actions, objectives);
    }

    printf("Aperte 'Enter' para continuar\n");
    getchar();

    // imprime o mapa atual atualizado
    printCountries(countries, "MAPA ATUAL", totalCountries);
}

// Função para declarar os territórios
int declareCountries(struct Country *countries, int usePresetCountries) {
    int totalCountries = 0;

    // se o usuário escolheu usar os territórios pré-definidos, monta os territórios e retorna o total de territórios
    if (usePresetCountries) {
        mountPresetCountries(countries);
        totalCountries = MAX_COUNTRIES;
        return totalCountries;
    }

    // Loop para cadastrar 5 territórios
    for (int i = 0; i < MAX_COUNTRIES; i++) {
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
