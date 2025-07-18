#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "gfx.h"
#include <math.h>

// estrutura utilizada para criacao das funcoes e procedimentos
typedef struct no
{
    struct no *prox, *ant; // ponteiro para o proximo elemento
    int val;
    int cabeca;
} No;

typedef struct dup
{
    struct dup *prox, *ant; // ponteiro para o proximo elemento e para o anterior
    int val;
} Dup;

// insercao em lista ordenada simplesmente encadeada
void inserir_ordeS(No **lista, int num)
{
    No *novo = malloc(sizeof(No)); // aloca o novo no
    if (!novo)                     // verifica se a alocacao de memoria teve sucesso
    {
        printf("Erro ao alocar memória");
        return;
    }

    novo->val = num;
    novo->prox = NULL; // O proximo do novo no sera inicializado como NULL

    if (*lista == NULL || num <= (*lista)->val) // Se a lista este vazia ou o novo elemento e menor ou igual ao primeiro da lista
    {
        novo->prox = *lista; // O proximo do novo no aponta para o antigo inicio da lista
        *lista = novo;       // O inicio da lista agora e o novo no
        return;
    }

    No *atual = *lista;
    while (atual->prox != NULL && num > atual->prox->val) // Percorre a lista até encontrar a posição correta para inserir o novo elemento
    {
        atual = atual->prox;
    }

    novo->prox = atual->prox; // O proximo do novo no aponta para o proximo do no atual
    atual->prox = novo;       // O proximo do no atual passa a ser o novo no
}
// remocao na lista simplesmente encadeada
void remove_ordeS(No **lista, int num)

{
    No *remover, *aux;
    if (*lista == NULL) // verifica se a lista esta vazia
        return;
    aux = *lista;
    if (aux->val == num) // verifica se o numero que eu quero remover e igual ao primeiro no da lista
    {
        *lista = (*lista)->prox;
        free(aux); // desaloca a memoria
        return;
    }
    else
    {
        while (aux->prox && aux->prox->val != num) // looping para encontrar o no que esta com o valor que eu desejo remover
            aux = aux->prox;
        if (aux->prox) // garante que o proximo do auxiliar esta com o elemento que deseja remover

        {
            remover = aux->prox;       // como o proximo do aux e igual ao numero que eu quero remover, eu removo ele
            aux->prox = remover->prox; // e coloco o proximo do aux para apontar para o prox de remover
        }
    }
}
// busca em lista simplesmente encadeada
No *busca_ordeS(No **lista, int num)
{
    No *aux, *no = NULL;
    aux = *lista;
    if (*lista == NULL)
        return NULL;
    while (aux && aux->val != num) // condicao de parada
        aux = aux->prox;
    if (aux) // garante que a falha do while foi na segunda opcao
        no = aux;
    return no; // retorna o no que foi buscado
}
// insercao em lista circular ordenada
void inserir_ordeC(No **circ, int num)
{
    No *aux, *novo = malloc(sizeof(No));
    novo->val = num;
    aux = *circ; // faz o auxiliar receber o conteudo da lista
    if (novo)
    {
        if (*circ == NULL || novo->val < (*circ)->val)
        {
            if (*circ == NULL) // Se a lista estiver vazia
            {
                novo->prox = novo; // Torna o novo no apontar para si mesmo
                *circ = novo;      // muda o inicio da lista para o novo no
            }
            else
            {
                while (aux->prox != *circ)
                    aux = aux->prox;
                novo->prox = *circ; // Faz o prox do novo no apontar para o inicio da lista
                *circ = novo;       // muda o inicio da lista para o novo no
                aux->prox = novo;   // muda o ultimo no para apontar para o novo no
            }
        }
        else
        {
            while (aux->prox != *circ && novo->val > aux->prox->val)
                aux = aux->prox;
            novo->prox = aux->prox; // Faz o novo no apontar para o proximo da lista
            aux->prox = novo;       // muda o prox de aux para apontar para o novo no
        }
    }
}
// remocao em lista circular
void remove_ordeC(No **circ, int num)
{
    if (*circ == NULL) // verifica se a lista esta vazia
    {
        return;
    }

    No *remover = NULL;
    No *aux = *circ;

    // Encontrar o no a ser removido
    while (aux->prox != *circ && aux->val != num)
    {
        aux = aux->prox;
    }

    // Se o no a ser removido foi encontrado
    if (aux->val == num)
    {
        remover = aux; // removo o elemento

        // Se o no a ser removido e o unico no da lista
        if (aux->prox == aux)
        {
            free(remover); // desaloco o remover
            *circ = NULL;  // e o primeiro elemento agora recebe NULO
            return;
        }

        // Se o no a ser removido e o primeiro no da lista
        if (remover == *circ)
        {
            No *ultimo = remover;         // crio um no para o ultimo elemento da lista
            while (ultimo->prox != *circ) // looping para andar na lista
            {
                ultimo = ultimo->prox;
            }
            *circ = remover->prox; // aponto o incio da lista para o segundo elemento da lista
            ultimo->prox = *circ;  // e removo o primeiro elemento
        }
        else
        {
            // Se o no a ser removido e o ultimo no da lista
            if (remover->prox == *circ)
            {
                while (aux->prox != remover)
                {
                    aux = aux->prox; // andar na lista
                }
                aux->prox = *circ;
            }
            else
            {
                // No a ser removido nao e o primeiro nem o ultimo
                while (aux->prox != remover)
                {
                    aux = aux->prox; // ando na lista ate encontrar o no a que eu quero remover
                }
                aux->prox = remover->prox; // como o elemento que eu quero remover esta em aux proximo, eu removo ele e aponto o proximo dele para o proximo de remover
            }
        }
        free(remover); // desaloca a memoria
    }
}

// busca em lista circular ordenada
No *busca_ordeC(No **circ, int num)
{
    No *aux, *no = NULL;
    aux = *circ;
    // Percorre a lista circular
    if (*circ == NULL)
        return NULL;
    do
    {
        if (aux->val == num)
        {
            no = aux;
            break; // encerra o loop se encontrar o valor
        }
        aux = aux->prox;
    } while (aux != *circ); // Condicao de parada vai ate voltar ao inicio da lista

    return no; // retorna o no buscado
}
// insercao em lista duplamente encadeada
void inserir_D(Dup **dupla, int num)
{
    Dup *novo = malloc(sizeof(Dup));
    novo->val = num;
    if ((*dupla)->prox == NULL)
    {                          // verifica se a lista esta vazia
        (*dupla)->prox = novo; // faz a lista que era vazia receber o novo no inserido
        novo->prox = NULL;     // como a lista so tem um elemento, o proximo vai ser nulo
        novo->ant = *dupla;    // o anterior do elemento inserido aponta para o no cabeca
    }
    else
    {
        Dup *aux = (*dupla)->prox;
        while (aux->prox != NULL) // looping para percorrer ate o final da lista
        {
            aux = aux->prox;
        }
        aux->prox = novo; // mudo o proximo do auxilar para o novo no
        novo->ant = aux;
        novo->prox = NULL; // como a insercao e no fim, o proximo do novo no vai ser nulo
    }
}
// remocao em lista duplamente encadeada
void remover_D(Dup **dupla, int num)
{
    Dup *remover, *aux;
    aux = (*dupla)->prox;
    if ((*dupla)->prox == NULL) // verifica se a lista esta vazia
    {
        return;
    }
    // verifica se o unico elemento que ta na lista e o que se deseja remover
    if (aux->val == num && aux->prox == NULL)
    {
        free(aux);
        (*dupla)->prox = NULL; // Atualiza o ponteiro do no cabeca para NULL
        return;
    }
    else if (aux->val == num) // verifica se o numero que se deseja remover e o primeiro no da lista
    {
        remover = aux;
        (*dupla)->prox = remover->prox; // o no cabeca aponta para o proximo do elemento removido
        aux = remover->prox;            // o primeiro elemento apos o no cabeca aponta para o proximo do elemento removido
        aux->ant = remover->ant;
        free(remover); // desaloca o no
        return;
    }
    while (aux->prox != NULL && aux->prox->val != num) // looping para ir ate o final da lista ou ate encontrar o elemento que se deseja remover
    {
        aux = aux->prox; // caminha na lista
    }
    if (aux->prox) // garante que a falha do while foi na segunda opcao, fazendo com que aux->proximo seja igual ao valor que se deseja remover
    {
        remover = aux->prox;
        aux->prox = remover->prox; // o proximo do aux aponta para o proximo do elemento removido
        if (remover->prox != NULL)
        {
            remover->prox->ant = aux; // O anterior do proximo elemento aponta para o anterior do elemento removido
        }
        free(remover);
    }
}
Dup *busca_D(Dup **dupla, int num)
{
    Dup *aux, *no = NULL;
    aux = (*dupla)->prox;
    if (*dupla == NULL)
        return NULL;
    while (aux != NULL && aux->val != num) // condicao de parada
        aux = aux->prox;
    if (aux != NULL) // garante que a condicao de parada falhou na segunda opcao
        no = aux;
    return no; // retorna o numero buscado
}
// procedimento para empilhar
void Empilhar_Pilha(No **topo, int num)
{
    printf("\n");
    No *novo = malloc(sizeof(No)); // alocacao do novo no
    novo->cabeca = num;
    novo->prox = *topo;
    *topo = novo; // mudo o topo da pilha para o no que vou inserir
}
// desempilha, remove o ultimo elemento inserido
void Desempilhar_Pilha(No **topo)
{
    No *remover;
    if (*topo != NULL)
    {
        remover = *topo;       // remove o topo
        *topo = (*topo)->prox; // faz o antigo topo receber o proximo no da pilha
        free(remover);
        if (*topo == NULL)
        {
            printf("\n");
        }
    }
}
// insere elemento na ultima posicao da fila
void inserir_fila(No **inicioF, int num)
{
    No *aux, *novo = malloc(sizeof(No)); // alocacao de memoria
    novo->val = num;
    if (*inicioF == NULL) // verifica se a fila esta vazia
    {
        *inicioF = novo; // coloco a pilha como sendo o novo no
        novo->prox = NULL;
    }
    else
    {
        aux = *inicioF;
        while (aux->prox != NULL) // looping para chegar ate o ultimo elemento
        {
            aux = aux->prox; // percorrer a fila
        }
        aux->prox = novo; // pego o proximo ultimo elemento e coloco ele como sendo o novo no
        novo->prox = NULL;
    }
}
// remove o primeiro elemento inserido na fila
void remover_fila(No **inicioF)
{
    No *remover;
    if (*inicioF == NULL) // verifica se a fila esta vazia
    {
        printf("fila vazia\n");
    }
    else
    {
        remover = *inicioF;          // remove o primeiro elemento
        *inicioF = (*inicioF)->prox; // faz o primeiro elemento apontar para o proximo
        free(remover);               // desaloca o no
    }
    if ((*inicioF)->prox == NULL) // verifica se foi removido o ultimo elemento
    {
        printf("fila agora está vazia");
    }
}
// imprime a lista simplesmente encadeada ordenada
void imprimir_S(No *lista)
{
    if (lista == NULL) // verifica se a lista esta vazia
    {
        printf("\nLista vazia.\n");
        return;
    }
    printf("\n\tLista: ");
    No *aux = lista;
    do
    {
        printf("%d ", aux->val);
        aux = aux->prox;
    } while (aux != NULL); // faz o looping ate chegar o final da lista
    printf("\n");
}
// imprime a lista circular ordenada
void imprimir_C(No *circ)
{
    if (circ == NULL) // verifica se a lista esta vazia
    {
        printf("\nLista vazia.\n");
        return;
    }
    printf("\n\tLista: ");
    No *aux = circ;
    do
    {
        printf("%d ", aux->val);
        aux = aux->prox;
    } while (aux != circ); // faz o looping ate dar uma volta completa na lista circular

    printf("\n");
}
// imprime lista duplamente encadeada
void imprimir_D(Dup *dupla)
{
    if ((dupla)->prox == NULL) // verifica se a lista esta vazia
    {
        printf("\nLista vazia.\n");
        return;
    }
    printf("\n\tLista: ");
    Dup *aux = dupla->prox;
    do
    {
        printf("%d ", aux->val);
        aux = aux->prox;
    } while (aux != NULL); // faz o looping ate chegar no final da lista
    printf("\n\n");
}
// imprime a pilha
void imprimir_pilha(No *topo)
{
    printf("\n");
    No *aux = topo;

    do
    {
        printf("%d \n", aux->cabeca); // impressao dos nos
        aux = aux->prox;
    } while (aux != NULL); // condicao de parada
}
// imprime a fila
void imprimir_fila(No *inicioF)
{
    printf("\n\tFila: ");
    No *aux = inicioF;
    do
    {
        printf("%d ", aux->val); // impressao dos nos
        aux = aux->prox;
    } while (aux != NULL); // condicao de parada

    printf("\n");
}

// PARTE GRAFICA ===========================================================================================================================

// desenha a seta
void draw_arrow(int x, int y, int comprimento, int tamanho_ponta)
{
    // Desenha o corpo da seta
    gfx_line(x, y, x + comprimento, y);

    // Desenha a ponta da seta
    gfx_line(x + comprimento - tamanho_ponta, y - tamanho_ponta, x + comprimento, y);
    gfx_line(x + comprimento - tamanho_ponta, y + tamanho_ponta, x + comprimento, y);
}
void draw_vertical_line(int x, int y, int comprimento)
{

    gfx_line(x, y, x, y + comprimento);
}
// desenha a seta para a esquerda ========================================================================================================
void draw_left_arrow(int x, int y, int comprimento, int tamanho_ponta)
{
    // Desenha o corpo da seta para a esquerda
    gfx_line(x + comprimento, y, x, y);

    // Desenha a ponta da seta para a esquerda
    gfx_line(x + tamanho_ponta, y - tamanho_ponta, x, y);
    gfx_line(x + tamanho_ponta, y + tamanho_ponta, x, y);
}
// desenha a letra grega lambida =========================================================================================================
void draw_lambda(int x, int y, int tamanho)
{
    int segmento = tamanho / 2;
    // Desenha a linha inclinada para baixo
    gfx_line(x + segmento, y, x + tamanho, y + tamanho);

    // Desenha a linha vertical
    gfx_line(x + segmento + 5, y + segmento - 2, x + 10, y + tamanho);
}
// desenha a lista simplesmente encadeada ================================================================================================
void desenhar_lista_encadeada(No *lista, int inicio_x, int inicio_y, int largura_no, int altura_no, int espacamento)
{
    No *aux = lista;
    int x = inicio_x;
    int y = inicio_y;
    int cont = 0;

    gfx_clear();
    gfx_set_font_size(20);
    gfx_text(x + largura_no - 30, y - 50, "Lista Simplesmente Encadeada");

    // Desenha ate 10 nos ou ate o final da lista
    while (aux != NULL && cont < 10)
    {
        // Desenha o retangulo representando o no
        draw_arrow(x + largura_no, y + (altura_no / 2), 38, 10);
        gfx_rectangle(x, y, x + largura_no, y + altura_no);

        // Desenha o texto dentro do retangulo representando o valor do no
        char texto[20];
        snprintf(texto, 5, "%d", aux->val);
        gfx_text(x + 5, y + 2, texto);

        // Atualiza a posicao para o proximo no
        x += largura_no + espacamento;
        aux = aux->prox;
        cont++;
    }

    draw_lambda(x + largura_no - 60, y + (altura_no / 2) - 15, 30);

    gfx_paint();
}
// desenha a lista duplamente encadeada ===================================================================================================
void desenhar_lista_circular(No *circ, int inicio_x, int inicio_y, int largura_no, int altura_no, int espacamento)
{
    No *aux = circ;
    int x = inicio_x;
    int y = inicio_y;
    int cont = 0;
    int primeiro_no_x = 0;
    int cont2 = 1;
    gfx_clear();
    gfx_set_font_size(20);
    gfx_text(x + largura_no - 30, y - 50, "Lista Circular");

    if (circ == NULL) // verifica se a lista e nula se for, desenho o simbolo de vazio
    {
        draw_lambda(x + largura_no - 55, y + (altura_no / 2) - 15, 30);
    }
    else
    {
        No *ultimo = aux;
        // Encontrar o ultimo no da lista
        while (ultimo->prox != circ)
            ultimo = ultimo->prox;
        primeiro_no_x = x;

        // Desenha ate 10 nos ou ate o final da lista (volta ao primeiro no para lista circular)
        do
        {
            if (cont < 10)
            {
                // Desenha o retangulo representando o no e a seta
                draw_arrow(x + largura_no, y + (altura_no / 2), 38, 10);
                gfx_rectangle(x, y, x + largura_no, y + altura_no);
                if (aux->prox == circ)
                {
                    draw_vertical_line(x + largura_no + 38, y + (altura_no / 2), 50);
                    gfx_line(primeiro_no_x + 17, y + 65, x + largura_no + 38, y + 65);
                    draw_vertical_line(primeiro_no_x + 17, y + (altura_no / 2) + 17, 33);
                }
                // Desenha o texto dentro do retangulo representando o valor do no
                char texto[20];
                snprintf(texto, 5, "%d", aux->val);
                gfx_text(x + 5, y + 2, texto);

                // Atualiza a posicao para o proximo no
                x += largura_no + espacamento;
                aux = aux->prox;
                cont++;
                cont2++;
            }
            else
            {
                return;
            }

        } while (aux != circ);
    }
    gfx_paint();
}
// desenha a lista duplamente encadeada ===================================================================================================
void desenhar_lista_Dupla(Dup *dupla, int inicio_x, int inicio_y, int largura_no, int altura_no, int espacamento)
{
    Dup *aux = dupla->prox;
    int x = inicio_x;
    int y = inicio_y;
    int cont = 0;

    gfx_clear();
    gfx_set_font_size(20);
    gfx_text(x + largura_no - 30, y - 50, "Lista Duplamente Encadeada");

    gfx_rectangle(x, y, x + largura_no + 10, y + altura_no + 10);
    draw_left_arrow(x + largura_no - 90, y + (altura_no / 2) + 5, 38, 10);
    draw_lambda(x + largura_no - 125, y + (altura_no / 2) - 12, 34);
    if (aux)
    {
        draw_arrow(x + largura_no + 10, y + (altura_no / 2) - 5, 38, 10);
        draw_left_arrow(x + largura_no + 10, y + (altura_no / 2) + 12, 38, 10);
    }
    // Desenha ate 10 nos ou ate o final da lista
    while (aux != NULL)
    {
        if (cont < 9)
        {
            x += largura_no + espacamento;
            // Desenha o retangulo representando o no
            gfx_rectangle(x, y, x + largura_no + 10, y + altura_no + 10);

            if (aux->prox != NULL)
            {
                draw_arrow(x + largura_no + 10, y + (altura_no / 2) - 5, 38, 10);
                draw_left_arrow(x + largura_no + 10, y + (altura_no / 2) + 12, 38, 10);
            }

            // Desenha o texto dentro do retangulo representando o valor do no
            gfx_set_font_size(22);
            char texto[20];
            snprintf(texto, 5, "%d", aux->val);
            gfx_text(x + 8, y + 10, texto);
        }
        else
        {
            return;
        }
        aux = aux->prox;
        cont++;
    }
    draw_arrow(x + largura_no + 10, y + (altura_no / 2) + 7, 38, 10);
    draw_lambda(x + largura_no + 42, y + (altura_no / 2) - 11, 34);

    gfx_paint();
}
// desenha a pilha ===================================================================================================================
void desenhar_pilha(No *topo, int inicio_x, int inicio_y, int largura_no, int altura_no, int espacamento)
{
    No *aux = topo;
    int x = inicio_x;
    int y = inicio_y;
    int cont = 0;

    gfx_clear();
    gfx_set_font_size(20);
    gfx_text(x + largura_no / 2 - 30, y - 50, "Pilha(LIFO)");

    if (aux == NULL)
    { // verifica se a pilha e nula e desenha o lambda para indicar topo da pilha
        draw_lambda(x + largura_no - 41, y + cont * (altura_no + espacamento) + altura_no / 2 - 10, 30);
    }
    // Desenha ate 10 nos ou ate o final da pilha
    while (aux != NULL && cont < 10)
    {
        // Desenha o retangulo representando o no e a seta
        gfx_rectangle(x, y + cont * (altura_no + espacamento), x + largura_no, y + cont * (altura_no + espacamento) + altura_no);

        // Desenha o texto dentro do retangulo representando o valor do no
        char texto[20];
        snprintf(texto, 5, "%d", aux->cabeca);
        gfx_text(x + 5, y + cont * (altura_no + espacamento) + 5, texto);

        // Atualiza a posicao para o proximo no
        aux = aux->prox;
        cont++;
    }

    gfx_paint();
}
// desennha a fila ===================================================================================================================
void desenhar_fila(No *inicioF, int inicio_x, int inicio_y, int largura_no, int altura_no, int espacamento)
{
    No *aux = inicioF;
    int x = inicio_x;
    int y = inicio_y;
    int cont = 0;

    gfx_clear();
    gfx_set_font_size(20);
    gfx_text(x + largura_no - 30, y - 50, "Fila(FIFO)");

    // Desenha ate 10 nos ou ate o final da fila
    while (aux != NULL && cont < 10)
    {
        // Desenha o retangulo representando o no
        draw_arrow(x + largura_no, y + (altura_no / 2), 38, 10);
        gfx_rectangle(x, y, x + largura_no, y + altura_no);

        // Desenha o texto dentro do retangulo representando o valor do no
        char texto[20];
        snprintf(texto, 5, "%d", aux->val);
        gfx_text(x + 5, y + 2, texto);

        // Atualiza a posicao para o proximo no
        x += largura_no + espacamento;
        aux = aux->prox;
        cont++;
    }

    draw_lambda(x + largura_no - 60, y + (altura_no / 2) - 15, 30);

    gfx_paint();
}

// menu de escolha para o usuario ======================================================================================================
void menu_principal()
{
    int opcao, escolha;
    No *lista = NULL;
    No *circ = NULL;
    No *topo = NULL;
    No *inicioF = NULL;
    No *buscado;
    Dup *procurado;
    Dup *dupla = malloc(sizeof(Dup));
    dupla->prox = NULL;
    dupla->ant = NULL;
    do
    {
        printf("\n0 - sair\n1 - listaSimplesmenteEncadeadaOrdenada\n2 - listaEncadeadaCircularOrdenada\n3 - listaDuplamenteEncadeada\n4 - Pilha\n5 - Fila\n");
        scanf("%d", &opcao);
        int num;
        system("clear");
        switch (opcao)
        {
        // escolha de listaSimplesmenteEncadeadaOrdenada
        case 1:
            do
            {
                desenhar_lista_encadeada(lista, 80, 80, 50, 30, 40);
                printf("\n0- sair\n1- Inserir\n2- Remover\n3- Buscar\n");
                scanf("%d", &escolha);
                switch (escolha)
                {
                case 0:
                    break;
                case 1:
                    printf("digite o numero: \n");
                    scanf("%d", &num);
                    inserir_ordeS(&lista, num);
                    break;
                case 2:
                    printf("digite um numero: \n");
                    scanf("%d", &num);
                    remove_ordeS(&lista, num);
                    break;
                case 3:
                    printf("digite um numero: \n");
                    scanf("%d", &num);
                    buscado = busca_ordeS(&lista, num);
                    if (buscado)
                        printf("elemento encontrado");
                    else
                        printf("elemento nao encontrado");
                    break;
                default:
                    printf("escolha invalida\n");
                    break;
                }
                imprimir_S(lista);
            } while (escolha != 0);
            break;
        // escolha de listaEncadeadaCircularOrdenada
        case 2:
            do
            {
                desenhar_lista_circular(circ, 80, 80, 50, 30, 40);
                printf("\n0 - sair\n1- Inserir\n2- Remover\n3- Buscar\n");
                scanf("%d", &escolha);
                switch (escolha)
                {
                case 0:
                    break;
                case 1:
                    printf("digite o numero: \n");
                    scanf("%d", &num);
                    inserir_ordeC(&circ, num);
                    break;
                case 2:
                    printf("digite um numero: \n");
                    scanf("%d", &num);
                    remove_ordeC(&circ, num);
                    break;
                case 3:
                    printf("digite um numero: \n");
                    scanf("%d", &num);
                    buscado = busca_ordeC(&circ, num);
                    if (buscado)
                        printf("elemento encontrado");
                    else
                        printf("elemento nao encontrado");
                    break;
                default:
                    printf("escolha invalida");
                    break;
                }
                imprimir_C(circ);
            } while (escolha != 0);
            break;
        // escolha de listaDuplamenteEncadeada
        case 3:
            do
            {
                desenhar_lista_Dupla(dupla, 80, 80, 50, 30, 52);
                printf("\n0 - sair\n1- Inserir\n2- Remover\n3- Buscar\n");
                scanf("%d", &escolha);
                switch (escolha)
                {
                case 0:
                    break;
                case 1:
                    printf("digite o numero: \n");
                    scanf("%d", &num);
                    inserir_D(&dupla, num);
                    break;
                case 2:
                    printf("digite um numero: \n");
                    scanf("%d", &num);
                    remover_D(&dupla, num);
                    break;
                case 3:
                    printf("digite um numero: \n");
                    scanf("%d", &num);
                    procurado = busca_D(&dupla, num);
                    if (procurado)
                        printf("elemento encontrado");
                    else
                        printf("elemento nao encontrado");
                    break;
                default:
                    printf("escolha invalida");
                    break;
                }
                imprimir_D(dupla);
            } while (escolha != 0);
            break;
        // escolha de pilha
        case 4:
            do
            {
                desenhar_pilha(topo, 80, 80, 50, 30, 0);
                printf("\n0 - sair\n1- Empilhar\n2- Desempilhar\n");
                scanf("%d", &escolha);
                switch (escolha)
                {
                case 0:
                    break;
                case 1:
                    printf("digite o numero: \n");
                    scanf("%d", &num);
                    Empilhar_Pilha(&topo, num);
                    break;
                case 2:
                    Desempilhar_Pilha(&topo);
                    break;
                default:
                    printf("escolha invalida");
                    break;
                }
                if (escolha != 0 && topo != NULL)

                    imprimir_pilha(topo);

                else if (topo == NULL)

                    printf("A pilha está vazia\n");

            } while (escolha != 0);
            break;
        // escolha de fila
        case 5:
            do
            {
                desenhar_fila(inicioF, 80, 80, 50, 30, 40);
                printf("\n0 - sair\n1- Inserir\n2- Remover\n");
                scanf("%d", &escolha);
                switch (escolha)
                {
                case 0:
                    break;
                case 1:
                    printf("digite o numero: \n");
                    scanf("%d", &num);
                    inserir_fila(&inicioF, num);
                    break;
                case 2:
                    Desempilhar_Pilha(&inicioF);
                    break;
                default:
                    printf("escolha invalida\n");
                    break;
                }
                if (escolha != 0 && inicioF != NULL)

                    imprimir_fila(inicioF);

                else if (inicioF == NULL)

                    printf("A fila está vazia\n");

            } while (escolha != 0);
            break;
        default:
            break;
            if (opcao != 0)
                printf("opcao invalida\n");
            break;
        }

    } while (opcao != 0);
}
// DESALOCACAO DE MEMORIA ===========================================================================================================
void desalocar_lista(No **lista)
{
    No *aux;
    while (*lista != NULL) // percorre a lista ate o final
    {
        aux = *lista;
        *lista = (*lista)->prox;
        free(aux); // libera a memoria do no atual
    }
}
void desalocar_circ(No **circ)
{
    if (*circ == NULL) // Verifica se a lista esta vazia
        return;
    No *aux;
    No *inicio = (*circ)->prox;
    while ((*circ)->prox != inicio) // Enquanto nao chegamos ao inicio novamente
    {
        aux = *circ;
        *circ = (*circ)->prox; // Avanca para o proximo no
        free(aux);             // Libera a memoria do no anterior
    }
    free(*circ); // Libera a memoria do ultimo no
}
void desalocar_dupla(Dup **dupla)
{
    Dup *aux;
    while ((*dupla)->prox != NULL) // percorre a lista ate o final
    {
        aux = (*dupla)->prox;
        aux = aux->prox;
        free(aux); // libera a memoria do no atual
    }
}
void desalocar_pilha(No **topo)
{
    No *aux;
    while (*topo != NULL)
    {
        aux = *topo; // recebe o primeiro no
        *topo = (*topo)->prox;
        free(aux); // libera a memoria do no atual
    }
}
void desalocar_fila(No **inicioF)
{
    No *aux;
    while (*inicioF != NULL)
    {
        aux = *inicioF; // recebe o primeiro no
        *inicioF = (*inicioF)->prox;
        free(aux); // libera a memoria do no atual
    }
}
int main()
{
    No *lista = NULL;
    No *circ = NULL;
    No *topo = NULL;
    No *inicioF = NULL;
    Dup *dupla = malloc(sizeof(Dup));
    dupla->prox = NULL;
    dupla->ant = NULL;

    gfx_init(1180, 590, "Desenhos");

    gfx_set_color(255, 255, 255);

    menu_principal();

    gfx_quit();

    desalocar_lista(&lista);
    desalocar_circ(&circ);
    desalocar_dupla(&dupla);
    desalocar_pilha(&topo);
    desalocar_fila(&inicioF);

    return 0;
}
