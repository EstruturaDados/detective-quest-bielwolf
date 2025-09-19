#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala
{
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sa;

Sa* criarSala(const char* valor) {
    Sa* novo = (Sa*)malloc(sizeof(Sa));
    if (novo == NULL) {
        printf("Erro ao alocar memória\n");
        exit(1);
    }

    strcpy(novo->nome, valor);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

Sa* navegarPelaSalas(Sa* raiz, const char* opcao) {
    if (strcmp(opcao, "e") == 0) {
        if (raiz->esquerda != NULL) {
            printf("Você foi para a sala %s\n", raiz->esquerda->nome);
            return raiz->esquerda; 
        } else {
            printf("Não há sala à esquerda!\n");
        }
    } 
    else if (strcmp(opcao, "d") == 0) {
        if (raiz->direita != NULL) {
            printf("Você foi para a sala %s\n", raiz->direita->nome);
            return raiz->direita; 
        } else {
            printf("Não há sala à direita!\n");
        }
    } 
    else if (strcmp(opcao, "s") == 0) {
        printf("Saindo da exploração...\n");
    } 
    else {
        printf("Opção inválida!\n");
    }

    return raiz; 
}

void liberar(Sa* raiz) {
    if(raiz != NULL) {
        liberar(raiz->esquerda);
        liberar(raiz->direita);
        free(raiz);
    }
}



int main() {
    Sa* raiz = criarSala("Hall de entrada");
    Sa* cozinha = criarSala("Cozinha");
    Sa* biblioteca = criarSala("Biblioteca");
    Sa* despensa = criarSala("Despensa");
    Sa* salaLeitura = criarSala("Sala de leitura");

    raiz->esquerda = cozinha;
    raiz->direita = biblioteca;
    cozinha->esquerda = despensa;
    cozinha->direita = raiz;
    biblioteca->direita = salaLeitura;
    biblioteca->esquerda = raiz;

    char opcao;
    Sa* atual = raiz;


    printf("--- DETETIVE ---\n");
    printf("Bem-vindo ao jogo de exploração\n");
    printf("Você está na sala: %s\n", atual->nome);

   
    do {
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nVocê chegou a uma sala sem saídas: %s\n", atual->nome);
            break;
        }
        printf("Escolha uma opção por onde ir (esquerda = e / direita = d / sair = s): ");
        scanf(" %c", &opcao);

        switch (opcao)
        {
        case 'e':
            atual = navegarPelaSalas(atual, "e");
            break;
        case 'd':
            atual = navegarPelaSalas(atual, "d");
            break;
        case 's':
            atual = navegarPelaSalas(atual, "s");
            liberar(raiz);
            break;
        default:
            printf("Opção inválida!\n");
            break;
        }
    } while (opcao != 's');

    liberar(raiz);
    return 0;
}