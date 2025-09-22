#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_TABELA 10

// ----------------- Estruturas -----------------

typedef struct Pistas {
    char nome[50];
    struct Pistas* esquerda;
    struct Pistas* direita;
} Pi;

typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
    Pi* pista;
} Sa;

typedef struct {
    char pista[50];
    char suspeito[50];
    int ocupado; // 0 = livre, 1 = ocupado, -1 = removido
} Entrada;

Entrada tabela_linear[TAMANHO_TABELA];

// ----------------- Funções utilitárias -----------------

int funcao_hash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) 
        soma += chave[i];
    return soma % TAMANHO_TABELA;
}

Sa* criarSala(const char* valor, Pi* pista) {
    Sa* novo = (Sa*)malloc(sizeof(Sa));
    if (!novo) {
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

// ----------------- Árvore de pistas (BST) -----------------

Pi* inserirPista(Pi* pista, const char* valor) {
    if (!pista)
        return criarPista(valor);
    if (strcmp(valor, pista->nome) < 0)
        pista->esquerda = inserirPista(pista->esquerda, valor);
    else
        pista->direita = inserirPista(pista->direita, valor);
    return pista;
}

void exibirPistas(Pi* pista) {
    if (pista) {
        exibirPistas(pista->esquerda);
        printf("%s\n", pista->nome);
        exibirPistas(pista->direita);
    }
}

// ----------------- Hash linear -----------------

void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = funcao_hash(pista);
    for(int i = 0; i < TAMANHO_TABELA; i++) {
        int pos = (indice + i) % TAMANHO_TABELA;
        if (tabela_linear[pos].ocupado == 0 || tabela_linear[pos].ocupado == -1) {
            strcpy(tabela_linear[pos].pista, pista);
            strcpy(tabela_linear[pos].suspeito, suspeito);
            tabela_linear[pos].ocupado = 1;
            return;
        }
    }
    printf("Tabela cheia. Não foi possível inserir '%s'.\n", pista);
}

char* encontrarSuspeito(const char* pista) {
    int indice = funcao_hash(pista);
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        int pos = (indice + i) % TAMANHO_TABELA;
        if (tabela_linear[pos].ocupado == 0)
            return "Suspeito não ligado a pista";
        if (tabela_linear[pos].ocupado == 1 && strcmp(tabela_linear[pos].pista, pista) == 0)
            return tabela_linear[pos].suspeito;
    }
    return "Suspeito não encontrado";
}

// ----------------- Exploração das salas -----------------

Sa* explorarSalas(Sa* raiz, Pi** pistasColetadas, char opcao) {
    if (opcao == 'e') {
        if (raiz->esquerda)
            raiz = raiz->esquerda;
        else {
            printf("Não há salas à esquerda!\n");
            return raiz;
        }
    } else if (opcao == 'd') {
        if (raiz->direita)
            raiz = raiz->direita;
        else {
            printf("Não há salas à direita!\n");
            return raiz;
        }
    } else if (opcao == 's') {
        printf("Saindo da exploração...\n");
        return raiz;
    } else {
        printf("Opção inválida!\n");
        return raiz;
    }

    printf("Você entrou na sala: %s\n", raiz->nome);

    if (raiz->pista) {
        printf("Você encontrou uma pista: %s\n", raiz->pista->nome);
        *pistasColetadas = inserirPista(*pistasColetadas, raiz->pista->nome);
        raiz->pista = NULL;
    }

    return raiz;
}

// ----------------- Verificação final -----------------

void verificarSuspeitoFinal(const char* suspeito) {
    int contagem = 0;
    for(int i = 0; i < TAMANHO_TABELA; i++) {
        if (tabela_linear[i].ocupado == 1 && strcmp(tabela_linear[i].suspeito, suspeito) == 0)
            contagem++;
    }

    if (contagem >= 2)
        printf("\nParabéns! Você encontrou o culpado!\n");
    else
        printf("\nO suspeito não é o culpado!\n");
}

// ----------------- Liberação de memória -----------------

void liberar(Sa* raiz) {
    if (!raiz) return;
    liberar(raiz->esquerda);
    liberar(raiz->direita);
    free(raiz);
}

void liberarPi(Pi* pista) {
    if (!pista) return;
    liberarPi(pista->esquerda);
    liberarPi(pista->direita);
    free(pista);
}

// ----------------- Main -----------------

int main() {

    // Criar pistas
    Pi* pista1 = criarPista("Chave dourada");
    Pi* pista2 = criarPista("Mapa antigo");
    Pi* pista3 = criarPista("Carta misteriosa");
    Pi* pista4 = criarPista("Diário escondido");
    Pi* pista5 = criarPista("Pedra gravada");

    // Criar salas
    Sa* raiz = criarSala("Hall de entrada", NULL);
    Sa* cozinha = criarSala("Cozinha", pista1);
    Sa* biblioteca = criarSala("Biblioteca", pista2);
    Sa* despensa = criarSala("Despensa", pista3);
    Sa* salaLeitura = criarSala("Sala de leitura", pista4);
    Sa* salaSecreta = criarSala("Sala secreta", pista5);

    // Montar mapa
    raiz->esquerda = cozinha;
    raiz->direita = biblioteca;
    cozinha->esquerda = despensa;
    despensa->esquerda = salaSecreta;
    biblioteca->direita = salaLeitura;

    // Associar pistas a suspeitos
    inserirNaHash("Chave dourada", "Lucas");
    inserirNaHash("Mapa antigo", "Amanda");
    inserirNaHash("Carta misteriosa", "Lucas");
    inserirNaHash("Diário escondido", "Bruno");
    inserirNaHash("Pedra gravada", "Alice");

    char opcao;
    Sa* atual = raiz;
    Pi* pistasColetadas = NULL;

    printf("--- DETETIVE ---\n");
    printf("Bem-vindo ao jogo de exploração\n");
    printf("Você está na sala: %s\n", atual->nome);

    // Loop de exploração
    do {
        printf("\nEscolha uma opção: (esquerda = e / direita = d / sair = s / ver pistas = v): ");
        scanf(" %c", &opcao);

        switch(opcao) {
            case 'e':
            case 'd':
            case 's':
                atual = explorarSalas(atual, &pistasColetadas, opcao);
                break;

            case 'v':
                printf("\nPistas coletadas:\n");
                if (pistasColetadas)
                    exibirPistas(pistasColetadas);
                else
                    printf("Nenhuma pista coletada ainda.\n");
                break;

            default:
                printf("Opção inválida!\n");
                break;
        }
    } while(opcao != 's');

    // Fase de acusação
    char suspeito[50];
    printf("\nDigite o suspeito que você deseja acusar: ");
    fgets(suspeito, sizeof(suspeito), stdin);

    suspeito[strcspn(suspeito, "\n")] = '\0';


    verificarSuspeitoFinal(suspeito);

    // Liberar memória
    liberar(raiz);
    liberarPi(pistasColetadas);

    return 0;
}
