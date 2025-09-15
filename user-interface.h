#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "models.h"

#define MAX_STR_LENGTH 50
#define HEADER_BORDER_CHAR '='
#define SEPARATOR_BORDER_CHAR '-'

void printTitle(char *title);
void printMultilineTitle(char *title);
void printCenteredText(char *text, int total_width, char border_char);
int isHyphenChar(char c);
int splitTextByHyphens(char *text, char lines[][MAX_STR_LENGTH], int maxLines);
int containsHyphen(char *text);
void printMenuItem(int key, char *name);
struct Action *renderMenu(struct Action *actions, int totalActions);
void printCountries(struct Country *countries, char *title, int totalCountries);
void printCountry(struct Country *country, int index);
void printAttackResult(struct Country *winner, struct Country *loser, int winnerRoll, int loserRoll, int winnerIsAttacker, int winnerConquered);
void printSeparator(int width, char border_char);
void printObjective(struct Objective *objective);
void getInputString(char *output);
void getInputInt(int *output);
void clearInputBuffer(void);
struct CollectedData* collectUserInputs(struct InputField *fields, int fieldCount, char *title);
void freeCollectedData(struct CollectedData *data);
void printCollectedData(struct CollectedData *data, int count);

void printObjective(struct Objective *objective) {
    printf("Missão %d: %s\n", objective->id + 1, objective->name);
    printf("Descrição: %s\n", objective->description);
    printf("Completada: %s\n", objective->completed ? "Sim" : "Não");
    printf("Exército: %s\n", objective->army);
    printf("Tipo: %s\n", objective->type == DESTROY_ARMY ? "Destruir Exército" : "Conquistar Territórios");
    printf("Alvo: %s\n", objective->target);
    printf("Quantidade de alvos: %d\n", objective->targetCount);
    printSeparator(35, SEPARATOR_BORDER_CHAR);
}

// Função para imprimir o resultado da batalha
void printAttackResult(struct Country *winner, struct Country *loser, int winnerRoll, int loserRoll, int winnerIsAttacker, int winnerConquered) {
    printTitle("Resultado da Batalha");
    printf("Atacante: %s | Rolou: %d\n", winner->name, winnerRoll);
    printf("Defensor: %s | Rolou: %d\n", loser->name, loserRoll);

    if (winnerIsAttacker) {
        printf("Atacante venceu a batalha!\n");
        if (winnerConquered) {
            printf("Atacante CONQUISTOU o território!\n");
        }
    } else {
        printf("Defensor venceu a batalha!\n");
    }

    printSeparator(35, SEPARATOR_BORDER_CHAR);
}

// Função para imprimir um território
void printCountry(struct Country *country, int index) {
    printf("%d. %s | Exército: %s | Tropas: %d\n", index, country->name, country->army, country->troops);
}

// Função para imprimir todos os territórios
void printCountries(struct Country *countries, char *title, int totalCountries) {
    printTitle(title);
    for (int i = 0; i < totalCountries; i++) {
        printCountry(&countries[i], i + 1);
    }
    printSeparator(35, SEPARATOR_BORDER_CHAR);
}

// Função para imprimir texto centralizado dentro de uma largura dada com caracteres de borda
void printCenteredText(char *text, int total_width, char border_char) {
    int text_length = strlen(text);
    int available_space = total_width - 2;
    
    if (text_length > available_space) {
        text_length = available_space;
    }
    
    int left_padding = (available_space - text_length) / 2;
    int right_padding = available_space - text_length - left_padding;
    
    // Imprime a borda esquerda
    printf("%c", border_char);
    
    // Imprime os espaços à esquerda para centralizar o texto
    for (int i = 0; i < left_padding; i++) {
        printf(" ");
    }
    
    // Imprime o texto (truncado se necessário)
    for (int i = 0; i < text_length; i++) {
        printf("%c", text[i]);
    }
    
    // Imprime os espaços à direita para centralizar o texto
    for (int i = 0; i < right_padding; i++) {
        printf(" ");
    }
    
    // Imprime a borda direita
    printf("%c\n", border_char);
}

// Função auxiliar para verificar se um caractere é um tipo de hífen
int isHyphenChar(char c) {
    return (c == '-');
}

// Função para dividir texto por diferentes tipos de hífens
int splitTextByHyphens(char *text, char lines[][MAX_STR_LENGTH], int maxLines) {
    int lineCount = 0;
    int currentPos = 0;
    int lineStart = 0;
    int textLen = strlen(text);
    
    for (int i = 0; i <= textLen && lineCount < maxLines; i++) {
        // Se encontrou um hífen ou chegou ao final da string
        if (i == textLen || isHyphenChar(text[i])) {
            // Copia a substring atual
            int lineLen = i - lineStart;
            if (lineLen > 0) {
                strncpy(lines[lineCount], &text[lineStart], lineLen);
                lines[lineCount][lineLen] = '\0';
                
                // Remove espaços no início e fim
                char *start = lines[lineCount];
                while (*start == ' ' || *start == '\t') start++;
                
                int len = strlen(start);
                while (len > 0 && (start[len-1] == ' ' || start[len-1] == '\t')) {
                    start[len-1] = '\0';
                    len--;
                }
                
                // Move o texto limpo para o início da linha se necessário
                if (start != lines[lineCount]) {
                    strcpy(lines[lineCount], start);
                }
                
                // Só adiciona se a linha não estiver vazia
                if (strlen(lines[lineCount]) > 0) {
                    lineCount++;
                }
            }
            
            // Próxima linha começa após o hífen
            lineStart = i + 1;
        }
    }
    
    return lineCount;
}

// Função para imprimir múltiplas linhas de título
void printMultilineTitle(char *title) {
    int border_width = 35;
    char lines[10][MAX_STR_LENGTH]; // Array de strings para armazenar as linhas
    
    // Divide o texto usando a nova função
    int lineCount = splitTextByHyphens(title, lines, 10);
    
    // Imprime a borda superior
    printSeparator(border_width, HEADER_BORDER_CHAR);
    
    // Imprime cada linha centralizada
    for (int i = 0; i < lineCount; i++) {
        printCenteredText(lines[i], border_width, HEADER_BORDER_CHAR);
    }
    
    // Imprime a borda inferior
    printSeparator(border_width, HEADER_BORDER_CHAR);
}

// Função auxiliar para verificar se o texto contém algum tipo de hífen
int containsHyphen(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isHyphenChar(text[i])) {
            return 1;
        }
    }
    return 0;
}

// Função para imprimir o título centralizado
void printTitle(char *title) {
    // Verifica se o título contém algum tipo de hífen
    if (containsHyphen(title)) {
        // Se contém hífen, usa a função multilinhas
        printMultilineTitle(title);
    } else {
        // Se não contém hífen, usa o método simples
        int border_width = 35;
        
        // Imprime a borda superior
        printSeparator(border_width, HEADER_BORDER_CHAR);
        
        // Imprime o título centralizado
        printCenteredText(title, border_width, HEADER_BORDER_CHAR);
        
        // Imprime a borda inferior
        printSeparator(border_width, HEADER_BORDER_CHAR);
    }
}

// Função para imprimir o item do menu
void printMenuItem(int key, char *name) {
    printf("%d. %s\n", key, name);
}

// Função para imprimir o menu
struct Action *renderMenu(struct Action *actions, int totalActions) {
    printTitle("Escolha uma ação");
    for (int i = 0; i < totalActions; i++) {
        printMenuItem(actions[i].key, actions[i].name);
    }
    printSeparator(35, SEPARATOR_BORDER_CHAR);

    int choice;

    getInputInt(&choice);

    struct Action *currentAction = NULL;

    for (int i = 0; i < totalActions; i++) {
        if (actions[i].key == choice) {
            currentAction = &actions[i];
            break;
        }
    }

    return currentAction;
}

// Função para limpar o buffer de entrada do teclado
void clearInputBuffer() { 
    int c; // Variável para armazenar o caractere lido
    while (
        (
            c = getchar() // define que c recebe o último caractere lido
        ) != '\n' && c != EOF // se o caractere lido não for uma nova linha e não for o final do arquivo, continue lendo
    );
}
// Função para capturar a string do input do usuário
void getInputString(char *output) {
    fgets(output, MAX_STR_LENGTH, stdin);
    output[strcspn(output, "\n")] = '\0'; // Remove o caractere de nova linha
}

// Função para capturar o valor inteiro do input do usuário
void getInputInt(int *output) {
    scanf("%d", output);
    clearInputBuffer();
}

// Função para imprimir o separador
void printSeparator(int width, char border_char) {
    for (int i = 0; i < width; i++) {
        printf("%c", border_char);
    }
    printf("\n");
}

// Função principal para coletar entradas do usuário
struct CollectedData* collectUserInputs(struct InputField *fields, int fieldCount, char *title) {
    // Aloca memória para armazenar os dados coletados
    struct CollectedData *collectedData = (struct CollectedData*)malloc(fieldCount * sizeof(struct CollectedData));
    
    if (collectedData == NULL) {
        printf("Erro: Não foi possível alocar memória para os dados coletados.\n");
        return NULL;
    }
    
    printTitle(title);
    
    // Itera através de cada campo definido
    for (int i = 0; i < fieldCount; i++) {
        printf("%s: ", fields[i].label);
        
        // Copia o tipo para o dado coletado
        strcpy(collectedData[i].type, fields[i].type);
        
        // Verifica o tipo de entrada e coleta adequadamente
        if (strcmp(fields[i].type, "string") == 0) {
            getInputString(collectedData[i].stringValue);
            // Limpa os outros valores
            collectedData[i].intValue = 0;
            
        } else if (strcmp(fields[i].type, "int") == 0) {
            getInputInt(&collectedData[i].intValue);
            // Limpa os outros valores
            strcpy(collectedData[i].stringValue, "");
            
        } else {
            printf("Erro: Tipo de campo '%s' não reconhecido. Usando string como padrão.\n", fields[i].type);
            getInputString(collectedData[i].stringValue);
            strcpy(collectedData[i].type, "string");
            collectedData[i].intValue = 0;
        }
    }

    printSeparator(35, SEPARATOR_BORDER_CHAR);
    
    return collectedData;
}

// Função auxiliar para liberar a memória dos dados coletados
void freeCollectedData(struct CollectedData *data) {
    if (data != NULL) {
        free(data);
    }
}

// Função auxiliar para imprimir os dados coletados (para debug/confirmação)
void printCollectedData(struct CollectedData *data, int count) {
    printTitle("Dados Coletados");
    
    for (int i = 0; i < count; i++) {
        printf("Campo %d (%s): ", i + 1, data[i].type);
        
        if (strcmp(data[i].type, "string") == 0) {
            printf("'%s'\n", data[i].stringValue);
        } else if (strcmp(data[i].type, "int") == 0) {
            printf("%d\n", data[i].intValue);
        }
    }
    printSeparator(35, SEPARATOR_BORDER_CHAR);
}

#endif