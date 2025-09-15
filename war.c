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
    int version;

    nivelMestre();

    return 0;
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// Funções auxiliares para as ações do menu
void printMapAction(struct Country *countries, int totalCountries, struct Action *actions) {
    printCountries(countries, "MAPA ATUAL", totalCountries);
}

void attackAction(struct Country *countries, int totalCountries, struct Action *actions) {
    attackCountry(countries, totalCountries);
}

void exitAction(struct Country *countries, int totalCountries, struct Action *actions) {
    exitGame(countries, actions);
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
    
    struct Action actions[3] = {
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
        action = renderMenu(actions, 3);

        if (action == NULL) {
            printf("Opção inválida\n");
            continue;
        }

        action->function(countries, totalCountries, actions);
    }

    return;
}
