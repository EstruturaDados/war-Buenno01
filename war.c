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

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Função do nível novato:
void nivelNovato(void);
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    nivelNovato();
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

#include <stdio.h> // Biblioteca para entrada e saída de dados
#include <string.h> // Biblioteca para manipulação de strings

#define MAX_COUNTRIES 5 // Define o número máximo de territórios
#define MAX_STR_LENGTH 50 // Define o tamanho máximo de uma string

struct Country { // Define a estrutura de um território
    char name[MAX_STR_LENGTH];
    char army[MAX_STR_LENGTH];
    int troops;
};

// Função para limpar o buffer de entrada do teclado
// Para evitar problemas com leituras consecutivas de scanf e getchar
// Pois quand o scanf usado múltiplas vezes por um loop
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
            fgets(countries[i].name, MAX_STR_LENGTH, stdin);

            // Remove o caractere de nova linha do nome do território
            countries[i].name[strcspn(countries[i].name, "\n")] = '\0';

            // Printa a indicação de que a informação a ser capturada é o exército do território
            printf("Exército do território (ex. Verde, Azul, etc.): \n");
            fgets(countries[i].army, MAX_STR_LENGTH, stdin);

            // Remove o caractere de nova linha do exército do território
            countries[i].army[strcspn(countries[i].army, "\n")] = '\0';

            // Printa a indicação de que a informação a ser capturada é a quantidade de tropas do território
            printf("Quantidade de tropas do território: \n");
            scanf("%d", &countries[i].troops);

            // Limpa o buffer de entrada do teclado possibilitando a próxima captura de dados
            clearInputBuffer();

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
