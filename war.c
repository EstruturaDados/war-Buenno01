#include <stdio.h> // Biblioteca para entrada e saída de dados
#include <string.h> // Biblioteca para manipulação de strings
#include <stdbool.h> // Biblioteca para usar o tipo booleano
#include <stdlib.h> // Biblioteca para usar a função rand
#include "models.h" // Biblioteca para modelos de dados
#include "user-interface.h" // Biblioteca para funções de interface do usuário
#include "objectives.h" // Biblioteca para funções de missões
#include "war.h" // Biblioteca para funções de jogo
void nivelMestre(void);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // Inicializa o gerador de números aleatórios com o tempo atual
    srand(time(NULL));

    int version;

    nivelMestre();

    return 0;
}

void printMapAction(struct Country *countries, int totalCountries, struct Action *actions, struct Objective *objectives) {
    printCountries(countries, "MAPA ATUAL", totalCountries);
}

void attackAction(struct Country *countries, int totalCountries, struct Action *actions, struct Objective *objectives) {
    attackCountry(countries, totalCountries, actions, objectives);
}

void printObjectivesAction(struct Country *countries, int totalCountries, struct Action *actions, struct Objective *objectives) {
    printCountries(countries, "MAPA ATUAL", totalCountries);
    struct InputField objectivesFields[] = {
        {"Número do território controlado pelo jogador:", "int"}
    };
    struct CollectedData *territoryData = collectUserInputs(objectivesFields, 1, "Missões");

    if (territoryData == NULL) {
        printTitle("Erro ao coletar dados das missões\n");
    }

    int territoryIndex = territoryData[0].intValue;

    freeCollectedData(territoryData);

    char targetArmy[MAX_STR_LENGTH];
    strcpy(targetArmy, countries[territoryIndex - 1].army);

    struct Objective *objective = NULL;

    for (int i = 0; i < totalCountries; i++) {
        if (strcmp(countries[i].army, targetArmy) == 0) {
            objective = &objectives[i];
            break;
        }
    }

    if (objective != NULL) {
        printObjective(objective);
    }

}

void exitAction(struct Country *countries, int totalCountries, struct Action *actions, struct Objective *objectives) {
    exitGame(countries, actions, objectives);
}

void countryGenerateOptions(struct Country *countries, int *totalCountries) {
    printf("Deseja cadastrar os territórios manualmente? (s/N)\n");

    char *input = malloc(MAX_STR_LENGTH * sizeof(char));

    getInputString(input);
    if (input[0] == 's' || input[0] == 'S') {
        *totalCountries = declareCountries(countries, 0);
    } else {
        *totalCountries = declareCountries(countries, 1);
    }

    free(input);
    
    printCountries(countries, "MAPA CADASTRADO", *totalCountries);
}

void nivelMestre() {
    // Printa o menu do nível aventureiro e título do jogo
    // Indicando o que o usuário deve fazer em seguida
    // Aloca a memória para os territórios
    struct Country *countries = alocateCountriesMemory();
    
    struct Action actions[4] = {
        {3, "Ver missões", printObjectivesAction},
        {2, "Ver mapa", printMapAction},
        {1, "Atacar", attackAction},
        {0, "Sair", exitAction}
    };

    int totalCountries = 0;

    printTitle("♟️ Nível Mestre - WAR GAME");
    printf("Vamos começar a montar o jogo!\n");
    printf("Cadastre 5 territórios\n");

    if (countries == NULL) {
        printf("Erro ao alocar memória para os territórios\n");
        exit(1);
    }

    countryGenerateOptions(countries, &totalCountries);

    struct Objective *objectives = createObjectives(countries, totalCountries);

    struct Action *action;

    while (1) {
        action = renderMenu(actions, 4);

        if (action == NULL) {
            printf("Opção inválida\n");
            continue;
        }

        action->function(countries, totalCountries, actions, objectives);
    }

    return;
}
