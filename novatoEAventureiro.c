#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pistas {
    char nome[50];
    struct Pistas* esquerda;
    struct Pistas* direita;
} Pi;

typedef struct Sala
{
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
    Pi* pista;
} Sa;


Sa* criarSala(const char* valor, Pi* pista ) {
    Sa* novo = (Sa*)malloc(sizeof(Sa));
    if (novo == NULL) {
        printf("Erro ao alocar memória\n");
        exit(1);
    }

    strcpy(novo->nome, valor);
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->pista = pista;
    return novo;
}

Pi* criarPista(const char* nome) {
    Pi* nova = (Pi*)malloc(sizeof(Pi));
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

Pi* inserir(Pi* pista, const char* valor) {
    if(pista == NULL) {
        return criarPista(valor);
    }

    if(strcmp(valor, pista->nome) < 0) {
        pista->esquerda = inserir(pista->esquerda, valor);
    } else {
        pista->direita = inserir(pista->direita, valor);
    }
    return pista;
}

Sa* explorarSalasComPistas(Sa* raiz, Pi** pista, char opcao) {
    if (opcao == 'e') {
        if (raiz->esquerda != NULL) {
            raiz = raiz->esquerda;
        } else {
            printf("Não há salas à esquerda");
            return raiz;
        }
    } 
    else if (opcao == 'd') {
        if (raiz->direita != NULL) {
            raiz = raiz->direita;
        } else {
            printf("Não há salas à direita!\n");
            return raiz;
        }
    } 
    else if (opcao == 's') {
        printf("Saindo da exploração...\n");
        return raiz;
    } 
    else {
        printf("Opção inválida!\n");
        return raiz;
    }

    printf("Você entrou na sala: %s\n", raiz->nome);

    if(raiz->pista != NULL) {
        printf("Você encontrou uma pista: %s\n", raiz->pista->nome);
        *pista = inserir(*pista, raiz->pista->nome);
        raiz->pista = NULL;
    }
    return raiz;

}

void exibirPistas(Pi* pista) {
    if(pista != NULL) {
        exibirPistas(pista->esquerda);
        printf("%s ", pista->nome);
        exibirPistas(pista->direita);
    }
}

void liberar(Sa* raiz) {
    if(raiz != NULL) {
        liberar(raiz->esquerda);
        liberar(raiz->direita);
        free(raiz);
    }
}

void liberarPi(Pi* pista) {
    if(pista != NULL) {
        liberar(pista->esquerda);
        liberar(pista->direita);
        free(pista);
    }
}


int main() {

    Pi* pista1 = criarPista("Chave dourada");
    Pi* pista2 = criarPista("Mapa antigo");
    Pi* pista3 = criarPista("Carta misteriosa");
    Pi* pista4 = criarPista("Diário escondido");
    Pi* pista5 = criarPista("Pedra gravada");

    Sa* raiz = criarSala("Hall de entrada", NULL);
    Sa* cozinha = criarSala("Cozinha", pista1);
    Sa* biblioteca = criarSala("Biblioteca", pista2);
    Sa* despensa = criarSala("Despensa", pista3);
    Sa* salaLeitura = criarSala("Sala de leitura", pista4);
    Sa* salaSecreta = criarSala("Sala secreta", pista5);

    raiz->esquerda = cozinha;
    raiz->direita = biblioteca;
    cozinha->esquerda = despensa;
    biblioteca->direita = salaLeitura;
    despensa->esquerda = salaSecreta;

    char opcao;
    Sa* atual = raiz;
    Pi* pistas = NULL;

    printf("--- DETETIVE ---\n");
    printf("Bem-vindo ao jogo de exploração\n");
    printf("Você está na sala: %s\n", atual->nome);

   
    do {
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nVocê chegou a uma sala sem saídas: %s\n", atual->nome);
            break;
        }

        printf("\nEscolha uma opção: (esquerda = e / direita = d / sair = s / ver pistas = v): ");
        scanf(" %c", &opcao);

        switch(opcao) {
            case 'e':
            case 'd':
            case 's':
                atual = explorarSalasComPistas(atual, &pistas, opcao);
                break;

            case 'v':
                printf("Pistas coletadas:\n");
                if (pistas != NULL)
                    exibirPistas(pistas);
                else
                    printf("Nenhuma pista coletada ainda.\n");
                printf("\n");
                break;

            default:
                printf("Opção inválida!\n");
                break;
        }

    } while (opcao != 's');

    liberar(raiz);
    liberarPi(pistas);

    return 0;
}