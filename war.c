// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

#include <stdio.h> // Biblioteca para entrada e saída de dados
#include <string.h> // Biblioteca para manipulação de strings
#include <stdbool.h> // Biblioteca para usar o tipo booleano
#include <stdlib.h> // Biblioteca para usar a função rand

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

#define MAX_COUNTRIES 5 // Define o número máximo de territórios
#define MAX_STR_LENGTH 50 // Define o tamanho máximo de uma string

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

struct Country { // Define a estrutura de um território
    char name[MAX_STR_LENGTH];
    char army[MAX_STR_LENGTH];
    int troops;
};

struct Action { // Define ações do menu
    int key; // Define o dígio para acessar a ação
    char name[MAX_STR_LENGTH]; // Define o nome da ação
    void (*function)(struct Country *countries, int totalCountries); // Define a função que será executada
};

// --- Protótipos das Funções ---
// Função do nível novato:
void nivelNovato(void);
void nivelAventureiro(void);
void nivelMestre(void);
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
void getInputString(char *output);
void getInputInt(int *output);
void clearInputBuffer(void);
// Funções de lógica principal do jogo:
void attackCountry(struct Country *countries, int totalCountries);
void exitGame(struct Country *countries, int totalCountries);
// Função utilitária:
int rollDice(void);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    int version;

    printf("Selecione a versão do jogo: \n");
    printf("1. Novato\n");
    printf("2. Aventureiro\n");
    printf("3. Mestre\n");
    getInputInt(&version);
    if (version == 1) {
        nivelNovato();
    } else if (version == 2) {
        nivelAventureiro();
    } else if (version == 3) {
        nivelMestre();
    }
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

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

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.

// NIVEL NOVATO

// Função para limpar o buffer de entrada do teclado
// Para evitar problemas com leituras consecutivas de scanf e getchar
// Pois quando o scanf é usado múltiplas vezes por um loop
// o getchar não limpa o buffer, e o próximo scanf lê o caractere anterior
// então é necessário limpar o buffer com a função clearInputBuffer
void clearInputBuffer() { 
    int c; // Variável para armazenar o caractere lido
    while (
        (
            c = getchar() // define que c recebe o último caractere lido
        ) != '\n' && c != EOF // se o caractere lido não for uma nova linha e não for o final do arquivo, continue lendo
    );
}

void getInputString(char *output) { // Captura a string do input do usuário
    fgets(output, MAX_STR_LENGTH, stdin);
    output[strcspn(output, "\n")] = '\0'; // Remove o caractere de nova linha
}

void getInputInt(int *output) { // Captura o valor inteiro do input do usuário
    scanf("%d", output);
    clearInputBuffer();
}

void nivelNovato() {
    struct Country countries[MAX_COUNTRIES]; // Vetor de territórios
    int totalCountries = 0; // Contador de territórios cadastrados

    do {
        // Printa o menu do nível novato e título do jogo
        // Indicando o que o usuário deve fazer em seguida
        printf("---------------------------------\n");
        printf("         🧩 Nível Novato         \n");
        printf("             WAR GAME            \n");
        printf("---------------------------------\n");
        printf("Vamos começar a montar o jogo!\n");
        printf("Cadastre 5 territórios\n");

        for (int i = 0; i < 5; i++) { // Loop para cadastrar 5 territórios

            // Printa o índice do território que está sendo cadastrado
            printf("---- Cadastrando território %d: ----\n", i + 1);

            // Printa a indicação de que a informação a ser capturada é o nome do território
            printf("Nome do território: \n");
            getInputString(countries[i].name);

            // Printa a indicação de que a informação a ser capturada é o exército do território
            printf("Exército do território (ex. Verde, Azul, etc.): \n");
            getInputString(countries[i].army);

            // Printa a indicação de que a informação a ser capturada é a quantidade de tropas do território
            printf("Quantidade de tropas do território: \n");
            getInputInt(&countries[i].troops);

            // Incrementa o contador de territórios cadastrados
            totalCountries++;
        }
    } while (totalCountries < 5); // Loop para continuar cadastrando territórios até que o total de territórios cadastrados seja igual a 5


    printf("=================================\n");
    printf("     Territórios cadastrados:    \n");
    printf("=================================\n");

    for (int i = 0; i < totalCountries; i++) {
        // Percorre o vetor de territórios e printa as informações de cada um
        printf("Território %d:\n", i + 1);
        printf("- Nome: %s\n", countries[i].name);
        printf("- Exército: %s\n", countries[i].army);
        printf("- Tropas: %d\n", countries[i].troops);
        printf("---------------------------------\n");
    }

    // Faz com que a aplicação aguarde o usuário pressionar enter antes de parar a execução;
    printf("Aperte 'Enter' para sair\n");
    getchar();

    return;
};

void printCountries(struct Country *countries, char *title, int totalCountries) {
    printf("=================================\n");
    printf("%s\n", title);
    printf("=================================\n");

    for (int i = 0; i < totalCountries; i++) {
        // Percorre o vetor de territórios e printa as informações de cada um
        printf("%d. %s | %s | Tropas: %d\n", i + 1, countries[i].name, countries[i].army, countries[i].troops);
    }
    printf("---------------------------------\n");
}

void mountPresetCountries(struct Country *countries) {
    strcpy(countries[0].name, "Brasil");
    strcpy(countries[0].army, "Verde");
    countries[0].troops = 3;
    strcpy(countries[1].name, "Argentina");
    strcpy(countries[1].army, "Azul");
    countries[1].troops = 3;
    strcpy(countries[2].name, "Uruguai");
    strcpy(countries[2].army, "Amarelo");
    countries[2].troops = 3;
    strcpy(countries[3].name, "Paraguai");
    strcpy(countries[3].army, "Vermelho");
    countries[3].troops = 3;
    strcpy(countries[4].name, "Chile");
    strcpy(countries[4].army, "Roxo");
    countries[4].troops = 3;

    return;
}

int generateCountries(struct Country *countries) {
    int totalCountries = 0;

    do {
        for (int i = 0; i < 5; i++) { // Loop para cadastrar 5 territórios

            // Printa o índice do território que está sendo cadastrado
            printf("---- Cadastrando território %d: ----\n", i + 1);

            // Printa a indicação de que a informação a ser capturada é o nome do território
            printf("Nome do território: \n");
            getInputString(countries[i].name);

            // Printa a indicação de que a informação a ser capturada é o exército do território
            printf("Exército do território (ex. Verde, Azul, etc.): \n");
            getInputString(countries[i].army);

            // Printa a indicação de que a informação a ser capturada é a quantidade de tropas do território
            printf("Quantidade de tropas do território: \n");
            getInputInt(&countries[i].troops);

            // Incrementa o contador de territórios cadastrados
            totalCountries++;
        }
    } while (totalCountries < 5); // Loop para continuar cadastrando territórios até que o total de territórios cadastrados seja igual a 5

    return totalCountries;
}

int rollDice() {
    return rand() % 6 + 1;
}

void attackCountry(struct Country *countries, int totalCountries) {
    int attackerIndex = 0;
    int defenderIndex = 0;
    printCountries(countries, "           MAPA ATUAL            ", totalCountries);
    printf("Escolha o território atacante: \n");
    getInputInt(&attackerIndex);
    printf("Escolha o território defensor: \n");
    getInputInt(&defenderIndex);

    struct Country *attacker = &countries[attackerIndex - 1];
    struct Country *defender = &countries[defenderIndex - 1];

    if (attacker->army == NULL || defender->army == NULL || strcmp(attacker->army, defender->army) == 0) {
        printf("Atacante e defensor não podem ser do mesmo exército\n");
        return;
    }

    if (attacker->troops < 2) {
        printf("Atacante não pode ter menos de 2 tropas\n");
        return;
    }

    int attackerRoll = rollDice();
    int defenderRoll = rollDice();

    if (attackerRoll > defenderRoll) {
        defender->troops--;
        printf("ATACANTE venceu a batalha!\n");
        printf("Defensor rolou: %d\n", defenderRoll);
        printf("Atacante rolou: %d\n", attackerRoll);
        printf("Defensor perdeu 1 tropa\n");
        if (defender->troops <= 0) {
            strcpy(defender->army, attacker->army);
            defender->troops += 1;
            attacker->troops -= 1;
            printf("Atacante CONQUISTOU o território\n");
        }

    } else {
        attacker->troops--;
        printf("DEFENSOR venceu a batalha!\n");
        printf("Defensor rolou: %d\n", defenderRoll);
        printf("Atacante rolou: %d\n", attackerRoll);
        printf("Atacante perdeu 1 tropa\n");
    }

    printf("Aperte 'Enter' para continuar\n");
    getchar();

    printCountries(countries, "           MAPA ATUAL            ", totalCountries);
}

void exitGame(struct Country *countries, int totalCountries) {
    freeCountriesMemory(countries);
    printf("Saindo do jogo...\n");
    exit(0);
    return;
}

void menu(struct Action *actions, int totalActions, struct Country *countries, int totalCountries) {
    printf("---------------------------------\n");
    printf("             MENU               \n");
    printf("---------------------------------\n");
    printf("Escolha uma ação: \n");

    for (int i = 0; i < totalActions; i++) {
        printf("%d. %s\n", actions[i].key, actions[i].name);
    }

    int action;

    getInputInt(&action);

    struct Action *currentAction = NULL;

    for (int i = 0; i < totalActions; i++) {
        if (actions[i].key == action) {
            currentAction = &actions[i];
            break;
        }
    }

    if (currentAction == NULL || currentAction->function == NULL) {
        printf("Ação inválida\n");
    } else {
        currentAction->function(countries, totalCountries);
    }
}

struct Country *alocateCountriesMemory() {
    struct Country *countries = (struct Country *)calloc(MAX_COUNTRIES, sizeof(struct Country));

    if (countries == NULL) {
        printf("Erro ao alocar memória para os territórios\n");
        exit(1);
    }

    return countries;
}

void freeCountriesMemory(struct Country *countries) {
    free(countries);
}

void nivelAventureiro() {
    // Printa o menu do nível aventureiro e título do jogo
    // Indicando o que o usuário deve fazer em seguida
    printf("---------------------------------\n");
    printf("     ♟️ Nível Aventureiro        \n");
    printf("             WAR GAME            \n");
    printf("---------------------------------\n");
    printf("Vamos começar a montar o jogo!\n");
    printf("Cadastre 5 territórios\n");

    struct Country *countries = alocateCountriesMemory();

    if (countries == NULL) {
        printf("Erro ao alocar memória para os territórios\n");
        exit(1);
    }

    int totalCountries;

    printf("Deseja cadastrar os territórios manualmente? (s/N)\n");

    char input[MAX_STR_LENGTH];

    getInputString(input);
    if (input[0] == 's' || input[0] == 'S') {
        totalCountries = generateCountries(countries);
    } else {
        mountPresetCountries(countries);
        totalCountries = 5;
    }
    
    printCountries(countries, "         MAPA CADASTRADO         ", totalCountries);

    struct Action actions[3] = {
        {2, "Ver mapa", &printCountries},
        {1, "Atacar", &attackCountry},
        {0, "Sair", &exitGame},
    };
    int action;
    while (1) {
        menu(actions, 3, countries, totalCountries);
    }

    return;
}

void nivelMestre() {
    printf("---------------------------------\n");
    printf("           Nível Mestre          \n");
    printf("             WAR GAME            \n");
    printf("---------------------------------\n");
    printf("NÃO IMPLEMENTADO AINDA\n");
    printf("Aperte 'Enter' para sair\n");
    getchar();

    return;
}