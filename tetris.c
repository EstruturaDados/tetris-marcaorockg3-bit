#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ----------- STRUCT PEÇA -----------
typedef struct {
    char nome;
    int id;
} Peca;

// ----------- FILA CIRCULAR -----------
typedef struct {
    Peca itens[TAM_FILA];
    int inicio, fim, qtd;
} Fila;

// ----------- PILHA -----------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

int contadorID = 0;

// ----------- GERAR PEÇA -----------
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

// ----------- FILA -----------
void initFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
}

int filaVazia(Fila *f) { return f->qtd == 0; }
int filaCheia(Fila *f) { return f->qtd == TAM_FILA; }

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->itens[f->fim] = p;
    f->qtd++;
}

Peca dequeue(Fila *f) {
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

// ----------- PILHA -----------
void initPilha(Pilha *p) { p->topo = -1; }

int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }

void push(Pilha *p, Peca x) {
    if (!pilhaCheia(p))
        p->itens[++p->topo] = x;
}

Peca pop(Pilha *p) {
    return p->itens[p->topo--];
}

// ----------- EXIBIR -----------
void exibir(Fila *f, Pilha *p) {
    printf("\n=========================\n");

    printf("Fila: ");
    for (int i = 0; i < f->qtd; i++) {
        int pos = (f->inicio + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[pos].nome, f->itens[pos].id);
    }

    printf("\nPilha (Topo -> Base): ");
    if (pilhaVazia(p)) printf("Vazia");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }

    printf("\n=========================\n");
}

// ----------- TROCA SIMPLES -----------
void trocarTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\nNão é possível trocar!\n");
        return;
    }

    int pos = f->inicio;
    Peca temp = f->itens[pos];
    f->itens[pos] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\nTroca realizada!\n");
}

// ----------- TROCA MÚLTIPLA -----------
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->qtd < 3 || p->topo < 2) {
        printf("\nNão há peças suficientes para troca múltipla!\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int posFila = (f->inicio + i) % TAM_FILA;
        int posPilha = p->topo - i;

        Peca temp = f->itens[posFila];
        f->itens[posFila] = p->itens[posPilha];
        p->itens[posPilha] = temp;
    }

    printf("\nTroca múltipla realizada!\n");
}

// ----------- MAIN -----------
int main() {
    Fila fila;
    Pilha pilha;
    int op;

    srand(time(NULL));

    initFila(&fila);
    initPilha(&pilha);

    // inicializa fila
    for (int i = 0; i < TAM_FILA; i++)
        enqueue(&fila, gerarPeca());

    do {
        exibir(&fila, &pilha);

        printf("\n1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar topo fila/pilha\n");
        printf("5 - Troca múltipla (3 peças)\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &op);

        switch (op) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca p = dequeue(&fila);
                    printf("\nJogou: [%c %d]\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }

            case 2: {
                if (!pilhaCheia(&pilha)) {
                    Peca p = dequeue(&fila);
                    push(&pilha, p);
                    printf("\nReservada: [%c %d]\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca());
                } else {
                    printf("\nPilha cheia!\n");
                }
                break;
            }

            case 3: {
                if (!pilhaVazia(&pilha)) {
                    Peca p = pop(&pilha);
                    printf("\nUsou da pilha: [%c %d]\n", p.nome, p.id);
                } else {
                    printf("\nPilha vazia!\n");
                }
                break;
            }

            case 4:
                trocarTopo(&fila, &pilha);
                break;

            case 5:
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (op != 0);

    return 0;
}