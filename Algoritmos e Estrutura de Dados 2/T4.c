#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "gfx.h"

// #define F 0
// #define V 1

struct s_arq_no
{

    int32_t chave : 28;

    int32_t bal : 2;

    uint32_t esq : 1;

    uint32_t dir : 1;
};

struct s_no
{
    int32_t chave : 28;

    int32_t bal : 2;

    int32_t reservado : 2; /* sem uso */

    struct s_no *esq;

    struct s_no *dir;
};

// funcao para construir a arvore AVL
struct s_no *ConstruirArvore(FILE *arquivo)
{
    struct s_arq_no dado;
    if (fread(&dado, sizeof(struct s_arq_no), 1, arquivo) != 1) // verifica se houve problema na leitura do arquivo (nao ter conteudo nele)
    {
        return NULL;
    }
    struct s_no *novo = malloc(sizeof(struct s_no)); // alocacao de memoria para uma variavel do tipo struct s_no

    if (novo == NULL) // verifica se houve erro ao alocar memoria
    {
        printf("erro ao alocar memoria");
    }
    novo->chave = dado.chave; // preencho o campo chave do meu no alocado com o campo chave da struct pre estabelecida
    novo->bal = dado.bal;
    novo->reservado = 0;
    if (dado.esq == 1)
    {                                         // verifico se existe subarvore esquerda
        novo->esq = ConstruirArvore(arquivo); // preenche a subarvore esquerda
    }
    else
    {
        novo->esq = NULL;
    }
    if (dado.dir == 1)
    {                                         // verifico se existe subarvore direita
        novo->dir = ConstruirArvore(arquivo); // preenche a subarvore direita
    }
    else
    {
        novo->dir = NULL;
    }
    return novo;
}

// funcao para leitura do arquivo
struct s_no *LerArquivo()
{
    printf("informe o nome do arquivo e sua extensao:"); // faco o usuario poder informar qual arquivo ele quer ler
    printf("\n");
    char arq[20];          // cria um array de caracteres com capacidade para 20 elementos para armazenar o arquivo fornecido pelo usuario
    fgets(arq, 19, stdin); // le o vetor de caracteres
    arq[strcspn(arq, "\n")] = 0;
    FILE *arquivo = fopen(arq, "rb"); // abro o arquivo

    if (arquivo == NULL) // verifica se houve algum problema na abertura do arquivo
    {
        printf("Erro ao abrir o arquivo");
        return NULL;
    }
    printf("\nArquivo aberto com sucesso\n");
    struct s_no *raiz = ConstruirArvore(arquivo);
    fclose(arquivo); // fecha o arquivo
    return raiz;     // retorna a raiz com a arvore construida
}

void SalvarArvore(struct s_no *raiz, FILE *arquivo)
{

    if (raiz == NULL) // verifica se existe algum conteudo na raiz
    {
        return;
    }
    struct s_arq_no dado;
    dado.chave = raiz->chave;
    dado.bal = raiz->bal;
    if (raiz->esq != NULL) // verifica se a esquerda e nula e faz as atribuicoes
    {
        dado.esq = 1;
    }
    else
        dado.esq = 0;

    if (raiz->dir != NULL) // verifica se a direita e nula e faz as atribuicoes
    {
        dado.dir = 1;
    }
    else
    {
        dado.dir = 0;
    }

    fwrite(&dado, sizeof(struct s_arq_no), 1, arquivo); // grava no arquivo a estrutura dado

    // desce recursivamente a arvore e salva as subarvores, esquerda e direita
    if (raiz->esq != NULL)
    {
        SalvarArvore(raiz->esq, arquivo); // desce para a esquerda
    }
    if (raiz->dir != NULL)
    {
        SalvarArvore(raiz->dir, arquivo); // desce para a direita
    }
}

void GravarArquivo(struct s_no *raiz)
{
    printf("Informe o nome do arquivo e sua extensão: \n");
    char arq[20];
    fgets(arq, 19, stdin); // le o arquivo inserido pelo usuario
    arq[strcspn(arq, "\n")] = 0;

    FILE *arquivo = fopen(arq, "wb"); // abre o arquivo do tipo wb, para escrita no formato binario

    if (arquivo == NULL)
    { // verifica se existe conteudo no arquivo
        printf("Erro ao abrir o arquivo para gravação.");
        return;
    }

    SalvarArvore(raiz, arquivo);
    printf("\nArquivo salvo com sucesso\n");
    fclose(arquivo); // fecho o arquivo
}

// funcao para busca de uma chave
struct s_no *busca(int x, struct s_no **raiz, int *f)

{
    if (*raiz == NULL)
    { // verifica se a arvore e vazia
        *f = 0;
        return NULL;
    }
    struct s_no *pai = NULL; // Inicializa o ponteiro pai
    while (*raiz != NULL)
    {
        if (x == (*raiz)->chave) // verifica se o valor procurado e igual ao valor da raiz
        {
            *f = 1;
            return pai;
        }
        else if (x < (*raiz)->chave) // verifica se o valor procurado e menor que o valor da raiz
        {
            if ((*raiz)->esq == NULL) // verifica se existe subarvore esquerda
            {
                *f = 2;
                pai = NULL;
                return pai;
            }
            pai = *raiz;          // Atualiza o pai antes de descer a subarvore esquerda
            *raiz = (*raiz)->esq; // desce para a subarvore esquerda da arvore
        }
        else // conclui-se entao que o valor que estou querendo procurar e maior que a raiz
        {
            if ((*raiz)->dir == NULL) // verifica se existe subarvore direita
            {
                *f = 3;
                pai = NULL;
                return pai;
            }
            pai = *raiz;          // Atualiza o pai antes de descer a direita
            *raiz = (*raiz)->dir; // desce para a direita da subarvore
        }
    }
    return pai;
}

void inicioNo(struct s_no **pt, int x)
{
    *pt = malloc(sizeof(struct s_no)); // alocacao de memoria para um no
    if (*pt == NULL)                   // verifica se houve erro na alocacao de memoria
    {
        printf("erro ao alocar memoria");
    }

    (*pt)->esq = NULL; // preenche a esquerda com nulo
    (*pt)->dir = NULL;
    (*pt)->chave = x;
    (*pt)->bal = 0; // preenche o campo bal
}

void caso1(struct s_no **pt, int *h)
{
    struct s_no *ptu = (*pt)->esq;

    if (ptu->bal == -1) // rotacao simples a direita
    {
        (*pt)->esq = ptu->dir;
        ptu->dir = *pt;

        // atualizacao dos balancos:
        (*pt)->bal = 0;
        ptu->bal = 0;

        *pt = ptu;
    }
    else // rotacao dupla a direita
    {
        struct s_no *ptv = ptu->dir;
        ptu->dir = ptv->esq;
        ptv->esq = ptu;
        (*pt)->esq = ptv->dir;
        ptv->dir = *pt;

        // atualizacao dos balancos:
        if (ptv->bal == -1)
        {
            (*pt)->bal = 1;
            ptu->bal = 0;
        }
        else if (ptv->bal == 0)
        {
            (*pt)->bal = 0;
            ptu->bal = 0;
        }
        else // ptv->bal == 1
        {
            (*pt)->bal = 0;
            ptu->bal = -1;
        }

        ptv->bal = 0;
        *pt = ptv;
    }

    *h = 0; // a arvore esta balanceada apos a rotacao
}

void caso2(struct s_no **pt, int *h)
{
    struct s_no *ptu = (*pt)->dir;

    if (ptu->bal == 1) // rotacao simples a esquerda
    {
        (*pt)->dir = ptu->esq;
        ptu->esq = *pt;

        // atualizacao dos balancos:
        (*pt)->bal = 0;
        ptu->bal = 0;

        *pt = ptu;
    }
    else // rotacao dupla a esquerda
    {
        struct s_no *ptv = ptu->esq;
        ptu->esq = ptv->dir;
        ptv->dir = ptu;
        (*pt)->dir = ptv->esq;
        ptv->esq = *pt;

        // atualizacao dos balancos:
        if (ptv->bal == 1)
        {
            (*pt)->bal = -1;
            ptu->bal = 0;
        }
        else if (ptv->bal == 0)
        {
            (*pt)->bal = 0;
            ptu->bal = 0;
        }
        else // ptv->bal == -1
        {
            (*pt)->bal = 0;
            ptu->bal = 1;
        }

        ptv->bal = 0;
        *pt = ptv;
    }

    *h = 0; // a altura nao mudou apos a rotacao
}

void insAVL(int x, struct s_no **pt, int *h)
{
    if ((*pt) == NULL) // verifica se tem conteudo na arvore, caso nao, inicializo um no
    {
        inicioNo((pt), x);
        *h = 1; // apos inserir um no, houve alteracao na altura da arvore
    }
    else // se ja existe algum no na arvore
    {
        if (x == (*pt)->chave) // verifica se o valor do no que vou inserir ja esta na arvore, caso sim, retorno uma mensagem mostrando falha na insercao
        {
            printf("no ja esta na arvore");
            return;
        }
        if (x < (*pt)->chave) // verifica se o valor do no que vou inserir e menor que o valor da raiz da arvore
        {
            insAVL(x, &((*pt)->esq), h); // desco para a subarvore esquerda recursivamente
            if (*h)                      // verifica se houve alteracao na altura
            {
                switch ((*pt)->bal) // analisa os casos do valor do balanceamento
                {
                case 1:
                    (*pt)->bal = 0; // atualizo o balanceamento
                    *h = 0;         // nao houve alteracao na altura
                    break;
                case 0:
                    (*pt)->bal = -1; // atualizo o balanceamento
                    break;
                case -1:
                    caso1(pt, h); // chamo a funcao caso 1 para fazer o rebalanceamento
                    break;
                }
            }
        }
        else
        {
            insAVL(x, &((*pt)->dir), h); // desco para a subarvore direita recursivamente
            if (*h)
            {
                switch ((*pt)->bal) // analisa os casos do valor do balanceamento
                {
                case -1:
                    (*pt)->bal = 0; // atualizo o balanceamento
                    *h = 0;         // marca que nao houve alteracao na altura
                    break;
                case 0:
                    (*pt)->bal = 1; // atualizo o balanceamento
                    break;
                case 1:
                    caso2(pt, h); // chamo a funcao caso 2 para fazer o rebalanceamento
                    break;

                default:
                    break;
                }
            }
        }
    }
}

// funcao para encontrar o menor no da arvore
struct s_no *BuscaMenor(struct s_no *raiz)
{
    struct s_no *temp = raiz;
    if (raiz == NULL) // verifica se a raiz e nula
    {
        return NULL;
    }
    else
    {
        while (temp->esq != NULL) // looping para enquanto tiver subarvore direita, eu vou com minha "raiz" para a esquerda
        {
            temp = temp->esq;
        }
    }
    return temp; // retorna a raiz, que esta com o menor elemento localizado no ultimo elemento a esquerda
}

void RotacaoEsquerdaRemocao(struct s_no **raiz, int *altura)
{
    struct s_no *aux = (*raiz)->dir; // salva o filho direito do no atual
    struct s_no *aux2;

    if (aux->bal < 0) // verifica se o balanceamento do filho direito e negativo
    {
        // rotacao dupla a esquerda
        // e atualizacao dos ponteiros
        aux2 = aux->esq;
        aux->esq = aux2->dir;
        aux2->dir = aux;        // o aux se torna filho direito de aux2
        (*raiz)->dir = aux2->esq; // filho esquerdo de aux2 se torna o filho direito do no atual
        aux2->esq = (*raiz);

        if (aux2->bal == 1) // ajusta os balanceamentos baseado no balanceamento de aux2
        {
            (*raiz)->bal = -1;
        }
        else
        {
            (*raiz)->bal = 0;
        }

        if (aux2->bal == -1) // ajusta os balanceamentos de aux
        {
            aux->bal = 1;
        }
        else
        {
            aux->bal = 0;
        }

        (*raiz) = aux2; // o aux2 se torna o novo no atual
        (*raiz)->bal = 0;
        *altura = 1; // indica que a altura foi alterada
    }
    else
    {
        // Rotacao simples a esquerda
        (*raiz)->dir = aux->esq; // filho esquerdo do aux se torna o filho direito do no atual
        aux->esq = (*raiz);      // o no atual se torna filho esquerdo de aux

        if (aux->bal == 0) // ajusta os balanceamentos baseados no balanceamento do aux
        {
            (*raiz)->bal = 1;
            aux->bal = -1;
            *altura = 0; // indica que a altura nao foi alterada
        }
        else
        {
            (*raiz)->bal = 0;
            aux->bal = 0;
            *altura = 1; // indica que a altura foi alterada
        }

        (*raiz) = aux; // o aux se torna o novo no atual
    }
}

void RotacaoDireitaRemocao(struct s_no **raiz, int *altura)
{
    struct s_no *aux = (*raiz)->esq; // salva o filho esquerdo do no atual
    struct s_no *aux2;

    if (aux->bal > 0) // verifica se o balanceamento do filho esquerdo e positivo
    {
        // rotacao dupla a direita
        // atualiza os ponteiros dos nos
        aux2 = aux->dir;
        aux->dir = aux2->esq;
        aux2->esq = aux;
        (*raiz)->esq = aux2->dir;
        aux2->dir = (*raiz);

        if (aux2->bal == -1) // ajusta os balanceamentos
        {
            (*raiz)->bal = 1;
        }
        else
        {
            (*raiz)->bal = 0;
        }

        if (aux2->bal == 1) // faz os ajustes de balanceamentos
        {
            aux->bal = -1;
        }
        else
        {
            aux->bal = 0;
        }

        (*raiz) = aux2; // o aux2 se torna o novo no atual
        (*raiz)->bal = 0;
        *altura = 1; // indica que a altura foi alterada
    }
    else
    {
        // Rotacao simples a direita
        (*raiz)->esq = aux->dir; // filho direito de aux se torna o filho esquerdo do no atual
        aux->dir = (*raiz);      // o no atual se torna filho direito de aux

        if (aux->bal == 0) // ajusta os balanceamentos baseado no balanceamento de aux
        {
            (*raiz)->bal = -1;
            aux->bal = 1;
            *altura = 0; // indica que a altura nao foi alterada
        }
        else
        {
            (*raiz)->bal = 0;
            aux->bal = 0;
            *altura = 1; // indica que a altura foi alterada
        }

        (*raiz) = aux; // o aux se torna o novo no atual
    }
}

// funcao para remover nos de uma arvore avl
void delete(int n, struct s_no **raiz, int *altura)
{
    // verifica se chegou ao final da arvore e não encontrou o no
    if (*raiz == NULL)
    {
        printf("Nó %d não encontrado ou árvore vazia!\n", n);
        *altura = 0; // sem alteracao de altura
        return; // encerra a funcao
    }
    struct s_no *temp;

    // verifica se o valor de n e maior que a chave do no atual, remocao na subarvore direita
    if (n > (*raiz)->chave)
    {
        delete(n, &(*raiz)->dir, altura);

        // verifica se houve alteracao na altura da subarvore
        if (*altura)
        {
            // atualiza o balanceamento do no apos a remocao na subarvore direita
            switch ((*raiz)->bal)
            {
            case 1:
                (*raiz)->bal = 0;
                *altura = 1; // define que houve alteracao na altura
                break;
            case 0:
                (*raiz)->bal = -1;
                *altura = 0; // nao houve alteracao na altura
                break;
            case -1:
                RotacaoDireitaRemocao(raiz, altura); // realiza rotacao a direita
                break;
            }
        }
    }
    // se o no a ser removido foi encontrado
    else if (n == (*raiz)->chave)
    {
        if ((*raiz)->esq == NULL)
        {
            temp = *raiz;
            *raiz = (*raiz)->dir;
            free(temp); // libera a memoria do no removido
            *altura = 1;     // houve alteracao na altura
        }
        else if ((*raiz)->dir == NULL)
        {
            temp = *raiz;
            *raiz = (*raiz)->esq;
            free(temp); // libera a memoria
            *altura = 1;     // houve alteracao na altura
        }
        else
        {
            temp = BuscaMenor((*raiz)->dir);          // busca o sucessor na subarvore direita
            (*raiz)->chave = temp->chave;             // substitui a chave do no atual
            delete(temp->chave, &(*raiz)->dir, altura); // remove o sucessor da subarvore direita

            if (*altura)
            {
                // atualiza o balanceamento do no apos a remocao do sucessor
                switch ((*raiz)->bal)
                {
                case 1:
                    (*raiz)->bal = 0;
                    *altura = 1; // houve alteracao na altura
                    break;
                case 0:
                    (*raiz)->bal = -1;
                    *altura = 0; // nao houve alteracao na altura
                    break;
                case -1:
                    RotacaoDireitaRemocao(raiz, altura); // realiza rotacao a direita
                    break;
                }
            }
        }
    }
    // verifica se o valor de n e menor que a chave do no atual, remocao na subarvore esquerda
    else if (n < (*raiz)->chave)
    {
        delete(n, &(*raiz)->esq, altura);

        // verifica se houve alteracao na altura da subarvore
        if (*altura)
        {
            // atualiza o balanceamento do no apos a remocao na subarvore esquerda
            switch ((*raiz)->bal)
            {
            case -1:
                (*raiz)->bal = 0;
                *altura = 1; // houve alteracao na altura
                break;
            case 0:
                (*raiz)->bal = 1;
                *altura = 0; // nao houve alteracao na altura
                break;
            case 1:
                RotacaoEsquerdaRemocao(raiz, altura); // realiza rotacao a esquerda
                break;
            }
        }
    }
}

// PARTE GRAFICA =========================================================================
// funcao auxiliar para desenhar um no da arvore

void draw_node(struct s_no *raiz, int x, int y, int largura_no, int altura_no, int espacamento)
{
    if (raiz == NULL)
    {
        return;
    }

    // Desenha o retangulo para representar o no
    gfx_rectangle(x, y, x + largura_no, y + altura_no);

    // Desenha o texto dentro do no
    char texto[18];
    snprintf(texto, sizeof(texto), "%d", raiz->chave);
    gfx_set_font_size(18);
    gfx_text(x + 15, y + 6, texto);

    char texto1[18];
    gfx_set_font_size(13);
    snprintf(texto1, sizeof(texto1), "(%d)", raiz->bal);
    gfx_text(x + 40, y + 21, (texto1));

    // Desenha a linha e o no esquerdo
    if (raiz->esq != NULL)
    {
        gfx_line(x + largura_no / 2, y + altura_no, x - espacamento + largura_no / 2 + 10, y + altura_no + altura_no / 2);
        draw_node(raiz->esq, x - espacamento, y + altura_no + 20, largura_no, altura_no, espacamento / 2);
    }

    // Desenha a linha e o no direito
    if (raiz->dir != NULL)
    {
        gfx_line(x + largura_no / 2, y + altura_no, x + espacamento + largura_no / 2 - 10, y + altura_no + altura_no / 2);
        draw_node(raiz->dir, x + espacamento, y + altura_no + 20, largura_no, altura_no, espacamento / 2);
    }
}

// Função principal para desenhar a arvore
void draw_tree(struct s_no *raiz, int largura_no, int altura_no)
{
    gfx_clear();
    gfx_set_color(255, 255, 255);
    gfx_set_font_size(18);
    int inicio_x = 930; // para conseguir colocar os nos no centro da tela
    int inicio_y = 50;
    gfx_text(inicio_x - 75, inicio_y - 30, "Arvore Binaria de Busca"); // texto principal da tela
    int espacamento_inicial = 480;                                     // Espacamento inicial entre nos
    draw_node(raiz, inicio_x, inicio_y, largura_no, altura_no, espacamento_inicial);
    gfx_paint();
}

// DESALOCACAO DE MEMORIA ========================================================================================
// funcao para desalocar
void desaloca_arvore(struct s_no **raiz)
{
    if (*raiz == NULL)
    { // verifica se a arvore e vazia
        return;
    }
    desaloca_arvore(&(*raiz)->esq); // desce para a esquerda recursivamente
    desaloca_arvore(&(*raiz)->dir); // desce para a direita recursivamente
    free(*raiz);                    // libera a memoria
    *raiz = NULL;
}

// MENU PRINCIPAL==========================================================================

void menu()
{
    struct s_no *raiz = NULL;
    int f, h;
    int opcao;
    int x;
    struct s_no *r = raiz;
    do
    {
        draw_tree(raiz, 60, 40);
        printf("\n0- Sair\n1- Busca\n2- Insercao\n3- Remocao\n4- Ler Arquivo\n5- Salvar Arquivo\n6- Desalocar Arvore\n");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 0:
            break;
        case 1: // busca de um elemento na arvore
            system("clear");
            printf("informe o valor que deseja buscar\n");
            scanf("%d", &x);
            r = raiz;
            busca(x, &r, &f);
            if (f == 1)
            {
                printf("Valor %d encontrado na árvore.\n", x);
            }
            else
            {
                printf("Valor %d não encontrado na árvore.\n", x);
            }
            break;

        case 2: // insercao de um elemento na arvore
            system("clear");
            printf("informe o valor que deseja inserir\n");
            scanf("%d", &x);
            h = 0;
            insAVL(x, &raiz, &h);
            break;

        case 3: // remocao de um elemento da arvore
            system("clear");
            printf("informe o valor que deseja remover: ");
            scanf("%d", &x);
            h = 0;
            delete(x, &raiz, &h);
            break;

        case 4: // leitura de um arquivo binario
            system("clear");
            getchar();
            raiz = LerArquivo();
            break;

        case 5: // gravacao de um arquivo binario
            system("clear");
            char nomeArquivo[50];
            fgets(nomeArquivo, 49, stdin);
            GravarArquivo(raiz);
            // return;
            break;

        case 6: // desalocao da arvore
            system("clear");
            desaloca_arvore(&raiz);

        default:
            break;
        }
    } while (opcao != 0);
    desaloca_arvore(&raiz);
}

int main()
{

    gfx_init(1920, 1080, "Desenhos");

    menu();

    gfx_quit();

    return 0;
}