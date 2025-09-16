#include <stdio.h> // Biblioteca para entrada e saída de dados
#include <string.h> // Biblioteca para manipulação de strings
#include <stdbool.h> // Biblioteca para usar o tipo booleano
#include <stdlib.h> // Biblioteca para usar a função rand
#include "models.h" // Biblioteca para modelos de dados
#include "user-interface.h" // Biblioteca para funções de interface do usuário
#include "objectives.h" // Biblioteca para funções de missões
#include "war.h" // Biblioteca para funções de jogo
void nivelMestre(void);

// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // Inicializa o gerador de números aleatórios com o tempo atual
    srand(time(NULL));

    int version;

    nivelMestre();

    return 0;
}

// Ação do usuário para imprimir o mapa
void printMapAction(struct Country *countries, int totalCountries, struct Action *actions, struct Objective *objectives) {
    printCountries(countries, "MAPA ATUAL", totalCountries);
}

// Ação do usuário para atacar um território
void attackAction(struct Country *countries, int totalCountries, struct Action *actions, struct Objective *objectives) {
    attackCountry(countries, totalCountries, actions, objectives);
}

// Ação do usuário para imprimir as missões
void printObjectivesAction(struct Country *countries, int totalCountries, struct Action *actions, struct Objective *objectives) {
    printCountries(countries, "MAPA ATUAL", totalCountries);

    // Define que será coletado um numero inteiro para o território controlado pelo jogador
    struct InputField objectivesFields[] = {
        {"Número do território controlado pelo jogador:", "int"}
    };

    // Coleta os dados do usuário retornando um array com uma posição
    // a primeira posição é o número do território controlado pelo jogador
    struct CollectedData *territoryData = collectUserInputs(objectivesFields, 1, "Missões");

    if (territoryData == NULL) {
        // se o usuário não digitou um número válido, imprime um erro
        printTitle("Erro ao coletar dados das missões\n");
    }

    // atribui o valor coletado das estruturas de input para a variável
    int territoryIndex = territoryData[0].intValue;

    freeCollectedData(territoryData);

    // inicializa uma variável para armazenar o exército do território controlado pelo jogador
    char targetArmy[MAX_STR_LENGTH];
    strcpy(targetArmy, countries[territoryIndex - 1].army);

    struct Objective *objective = NULL;

    // Loop para buscar a missão do território controlado pelo jogador
    for (int i = 0; i < totalCountries; i++) {
        if (strcmp(countries[i].army, targetArmy) == 0) {
            objective = &objectives[i];
            break;
        }
    }

    if (objective != NULL) {
        // se a missão foi encontrada, imprime a missão
        printObjective(objective);
    }

}

// Ação do usuário para sair do jogo
void exitAction(struct Country *countries, int totalCountries, struct Action *actions, struct Objective *objectives) {
    exitGame(countries, actions, objectives);
}

// inicializa um formulario para cadastrar os territórios
void countryGenerateOptions(struct Country *countries, int *totalCountries) {
    printf("Deseja cadastrar os territórios manualmente? (s/N)\n");

    // aloca memória para armazenar a resposta do usuário
    char *input = malloc(MAX_STR_LENGTH * sizeof(char));

    getInputString(input);

    if (input[0] == 's' || input[0] == 'S') {
        // se o usuário escolheu cadastrar os territórios manualmente, chama a função para cadastrar os territórios
        *totalCountries = declareCountries(countries, 0);
    } else {
        // se o usuário escolheu usar os territórios pré-definidos, chama a função para cadastrar os territórios
        *totalCountries = declareCountries(countries, 1);
    }

    free(input);
    
    // imprime o mapa cadastrado
    printCountries(countries, "MAPA CADASTRADO", *totalCountries);
}

void nivelMestre() {
    // aloca memória para os territórios
    struct Country *countries = alocateCountriesMemory();
    
    // define as ações do menu
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

    // inicializa um formulario para cadastrar os territórios
    countryGenerateOptions(countries, &totalCountries);

    // cria as missões
    struct Objective *objectives = createObjectives(countries, totalCountries);

    // variavel para armazenar a ação escolhida no menu
    struct Action *action;

    while (1) {
        // renderiza o menu e retorna a ação escolhida
        action = renderMenu(actions, 4);

        if (action == NULL) {
            printf("Opção inválida\n");
            continue;
        }

        // executa a ação escolhida usando o ponteiro para a função da ação
        action->function(countries, totalCountries, actions, objectives);
    }

    return;
}
