#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "gfx.h"

typedef struct s_arq_no
{
    int32_t chave : 30;
    uint32_t esq : 1;
    uint32_t dir : 1;
} arq;

// struct utilizada nas funcoes da arvore
typedef struct no
{
    struct no *dir, *esq;
    int valor;
} No;

// funcao para construir a arvore utilizando um arquivo binario
No *construirArvore(FILE *arquivo)
{
    arq dado;
    if (fread(&dado, sizeof(arq), 1, arquivo) != 1) // verifica se existe conteudo no arquivo
    {
        return NULL;
    }

    No *novo = (No *)malloc(sizeof(No)); // alocacao de memoria para preenchimento da arvore
    if (novo == NULL)                    // verifica se deu certo a alocacao de memoria
    {
        printf("Erro ao alocar memória para o no");
    }
    novo->valor = dado.chave; // preencho o campo valor do meu no alocado com o campo chave da struct pre estabelecida

    if (dado.esq == 1) // verifica se existe esquerda
    {
        novo->esq = construirArvore(arquivo); // preenche a esquerda
    }
    else
    {
        novo->esq = NULL;
    }

    if (dado.dir == 1) // verifica se existe direita
    {
        novo->dir = construirArvore(arquivo); // preenche a direita
    }
    else
    {
        novo->dir = NULL;
    }

    return novo;
}
// funcao para leitura do arquivo
No *lerArquivo()
{
    printf("\ninforme o nome do arquivo com sua extensao!!"); // pede ao usuario o nome do arquivo
    printf("\n");
    char arq[20];          // cria um array de caracteres com capacidade para 20 elementos para armazenar o arquivo fornecido pelo usuario
    fgets(arq, 19, stdin); // le o arquivo
    arq[strcspn(arq, "\n")] = 0;
    FILE *arquivo = fopen(arq, "rb"); // abre o arquivo
    if (arquivo == NULL)              // verifica se deu certo a arbertura do arquivo
    {
        printf("Erro ao abrir o arquivo\n");
        return NULL;
    }

    No *raiz = construirArvore(arquivo);

    fclose(arquivo); // fecha o arquivo
    return raiz;     // retorna a raiz com a arvore construida
}
// funcao para buscar um no na arvore
No *busca(int x, No **raiz, int *f)
{
    if (*raiz == NULL)
    { // verifica se existe algum elemento na arvore
        *f = 0;
        return NULL;
    }
    No *pai = NULL; // Inicialmente o ponteiro pai e nulo
    while (*raiz != NULL)
    {
        if (x == (*raiz)->valor) // verifica se o valor de x e igual ao valor da raiz
        {
            *f = 1;
            return pai;
        }
        else if (x < (*raiz)->valor) // verifica se o valor de x e menor que o valor que o valor da raiz
        {
            if ((*raiz)->esq == NULL) // verifica se existe subarvore esquerda
            {
                *f = 2;
                pai = NULL;
                return pai;
            }
            pai = *raiz;          // Atualiza o pai antes de descer a esquerda
            *raiz = (*raiz)->esq; // desce para a esquerda da arvore
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
// funcao para encontrar o maior no da arvore
No *BuscaMaior(No *raiz)
{
    No *temp = raiz;
    if (raiz == NULL) // verifica se a raiz e nula
    {
        return NULL;
    }
    else
    {
        while (temp->dir != NULL) // looping para enquanto tiver subarvore direita, eu vou com minha "raiz" para a direita
        {
            temp = temp->dir;
        }
    }
    return temp; // retorna a raiz, que esta com o maior elemento localizado no ultimo elemento a direita
}
// funcao para encontrar o menor no da arvore
No *BuscaMenor(No *raiz)
{
    No *temp = raiz;
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
// funcao para busca de um sucessor para um no da arvore
No *BuscaSucessor(No *x, No *raiz)
{
    if (x == NULL) // verifica se o retorno da minha funcao de busca para o valor que se deseja encontrar o sucessor e nulo
    {
        return NULL;
    }
    else
    {
        if (x->dir) // verifica se o retorno da funcao da busca para o valor que se deseja o sucessor possui subarvore direita
        {
            return BuscaMenor(x->dir); // retorno o menor elemento da subarvore direita, sendo assim o sucessor
        }
        else // retorno da funcao de busca para o valor que se deseja encontrar o sucessor nao possui subarvore direita
        {
            No *sucessor = NULL;
            No *atual = raiz;
            while (atual != NULL) // looping para enquanto existir um elemento na arvore
            {
                if (x->valor < atual->valor) // verifica se o valor que eu quero encontrar o sucessor e menor do que minha raiz
                {
                    sucessor = atual; // caso o valor que eu procuro o sucessor for menor que a raiz, entao seu sucessor e a raiz
                    atual = atual->esq;
                }
                else if (x->valor > atual->valor) // verifica se o valor que eu quero encontrar o sucessor e maior do que a raiz
                {
                    atual = atual->dir; // caminho para a direita da arvore
                }
                else // caso nao seja nem menor e nem maior
                {
                    break;
                }
            }
            return sucessor; // retorno o no sucessor
        }
    }
}
// funcao para buscar um predecessor na arvore
No *BuscaPredecessor(No *x, No *raiz)
{
    if (x == NULL) // verifica se o retorno da minha funcao de busca para o valor que se deseja encontrar o predecessor e nulo
    {
        return NULL;
    }
    else
    {
        if (x->esq) // verifica se o retorno da funcao da busca para o valor que se deseja encontrar o predecessor possui subarvore direita
        {
            return BuscaMaior(x->esq); // retorno o maior elemento da subarvore esquerda, sendo assim o predecessor
        }
        else // retorno da funcao de busca para o valor que se deseja encontrar o sucessor nao possui subarvore direita
        {
            No *predecessor = NULL;
            No *atual = raiz;
            while (atual != NULL) // looping para enquanto existir um elemento na arvore
            {
                if (x->valor > atual->valor) // verifica se o valor que eu quero encontrar o sucessor e maior do que a raiz
                {
                    predecessor = atual; // caso o valor que eu procuro o predecessor for maior que a raiz, entao seu predecessor e a raiz
                    atual = atual->dir;
                }
                else if (x->valor < atual->valor) // verifica se o valor que eu quero encontrar o predecessor e menor do que a raiz
                {
                    atual = atual->esq; // desco para a esquerda da arvore
                }
                else // caso nao seja nem menor e nem maior
                {
                    break;
                }
            }

            return predecessor; // retorna o no predecessor
        }
    }
}
// funcao para inserir um elemento na arvore
void insercao(int x, No **raiz)
{
    No *ptRaiz = *raiz;
    int f;
    busca(x, &ptRaiz, &f); // utilizo a funcao de busca para, de acordo com o valor de f, realizar a insercao no local correto da arvore
    if (f == 1)            // verifica se o elemento que eu quero inserir ja existe na arvore, aparacendo uma mensagem de erro
    {
        printf("insercao invalida\n");
        return;
    }
    No *novo = (No *)malloc(sizeof(No)); // alocacao de memoria para o no que eu quero inserir
    novo->valor = x;                     // preencho o campo valor do novo no com o valor do elemento que eu quero inserir
    novo->esq = NULL;
    novo->dir = NULL;
    if (f == 0) // verifica a raiz e nula, nao tem elemento na arvore
    {
        *raiz = novo;
    }
    else if (f == 2) // verifica se a subarvore da esquerda e nula
    {
        ptRaiz->esq = novo;
    }
    else // verifica se a subarvore da direita e nula
    {
        ptRaiz->dir = novo;
    }
}
// funcao para realizar a troca de duas subarvores
void Transplant(No **raiz, No *u, No *v){
    No *pu = NULL;
    if(u == NULL) //verifica se o o no que eu quero fazer a troca e nulo
        return;
    if(u!= *raiz ){ // verifica se o no que eu quero fazer a troca e diferente do primeiro no da arvore
        int f;
        No *aux = *raiz;
        pu = busca(u->valor,&aux ,&f); // no pai recebe o retorno da funcao de busca, fazendo as atribuicoes corretas ao no pai
    }
    if (pu == NULL) // verifica se o no pai tem algum conteudo
    {
        *raiz = v;
    }
    else if (u == pu->esq) // verifica se o no que quero realizar a troca e igual a ao no da esquerda do pai
    {
        pu->esq = v; // atribui a esquerda do pai
    }
    else{ 
        pu->dir = v; // atribui a direita do pai
    }
}

// funcao para remover um no da arvore
void Delete(No *z, No **T)
{
    int f;
    No *y;
    No *py;

    if(z == NULL){ 
        return;
    }
    if(*T){
        if (z->esq == NULL){ // verifica se nao tem filho da esquerda
            Transplant(T, z, z->dir); // realiza a troca com a subarvore direita
        }
        else 
            if (z->dir == NULL)  // verifica se nao tem filho da direita
                Transplant(T, z, z->esq); //realiza a troca com a subarvore esquerda
            else
            {
                y = BuscaMenor(z->dir); // realiza a busca do menor elemento entre os maiores que estao a direita
                No *aux = *T;
                py = busca(y->valor, &aux, &f); 
                if (py != z) //verifica se o pai de y e diferente de z
                {
                    Transplant(T, y, y->dir); // troca y com a subarvore direita de y
                    y->dir = z->dir;
                }
                Transplant(T, z, y); // realiza a troca de z com y
                y->esq = z->esq;
            }     
    }
    else{
        printf("arvore esta vazia.\n");
    }

}


//PARTE GRAFICA =============================================================================================
// funcao auxiliar para desenhar um no da arvore
void draw_node(No* raiz, int x, int y, int largura_no, int altura_no, int espacamento) {
    if (raiz == NULL) {
        return;
    }

    // Desenha o retangulo para representar o no
    gfx_rectangle(x , y, x + largura_no, y + altura_no);
    
    // Desenha o texto dentro do no
    char texto[18];
    snprintf(texto, sizeof(texto), "%d", raiz->valor);
    gfx_text(x + 5, y + 2, texto);


    // Desenha a linha e o no esquerdo
    if (raiz->esq != NULL) {
        gfx_line(x + largura_no / 2, y + altura_no , x - espacamento + largura_no / 2 + 10, y + altura_no + altura_no /2);
        draw_node(raiz->esq, x - espacamento  , y + altura_no + 20, largura_no, altura_no, espacamento / 2 );
    }

    // Desenha a linha e o no direito
    if (raiz->dir != NULL) {
        gfx_line(x + largura_no / 2, y + altura_no , x + espacamento  + largura_no / 2 - 10, y + altura_no + altura_no / 2);
        draw_node(raiz->dir, x + espacamento , y + altura_no + 20, largura_no, altura_no , espacamento / 2 );
    }
}

// Função principal para desenhar a arvore
void draw_tree(No* raiz, int largura_no, int altura_no) {
    gfx_clear();
    gfx_set_color(255, 255, 255);
    gfx_set_font_size(18);
    int inicio_x = 930; // para conseguir colocar os nos no centro da tela
    int inicio_y = 50;
    gfx_text(inicio_x - 75, inicio_y - 30, "Arvore Binaria de Busca"); // texto principal da tela
    int espacamento_inicial = 480; // Espacamento inicial entre nos
    draw_node(raiz, inicio_x, inicio_y, largura_no, altura_no, espacamento_inicial);
    gfx_paint();
}

//DESALOCACAO DE MEMORIA =================================================================================================

//funcao para desalocar
void desaloca_arvore(No *raiz){
    if(raiz == NULL){ // verifica se a arvore e vazia
        return;
    }
    desaloca_arvore(raiz->esq); // desce para a esquerda recursivamente
    desaloca_arvore(raiz->dir); // desce para a direita recursivamente 
    free(raiz); // libera a memoria       
    
}

// menu principal ===========================================================================================
void menu(){
    No *raiz = lerArquivo();
    if(raiz == NULL){ 
        return;
    }
    int f;
    int opcao;
    No *r = raiz;
    int x;
    do{
        draw_tree(raiz, 60, 40);
        printf("\n0- sair\n1- busca maior\n2- busca menor\n3- busca chave\n4- busca sucessor\n5- busca predecessor\n6- insercao\n7- remocao\n");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 0: // sair do programa
            break;

        case 1: // busca do maior elemento da arvore 
            system("clear");
            No *maior = BuscaMaior(raiz);
            printf("o maior e: %d", maior->valor);
            break;

        case 2: // busca do menor elemento da arvore
            system("clear");
            No *menor = BuscaMenor(raiz);
            printf("o menor e: %d", menor->valor);
            break;
        
        case 3: // busca de uma chave qualquer da arvore
            system("clear");
            printf("informe o valor que deseja buscar: ");
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
        
        case 4: // busca do sucessor de um no na arvore
            system("clear");
            printf("informe o valor que deseja o sucessor: ");
            scanf("%d", &x);
            r = raiz;
            busca(x, &r, &f);
            if(f != 1){
                printf("%d nao tem sucessor na arvore\n", x);
            }
            else{
                No *sucessor = BuscaSucessor(r, raiz);
                if (sucessor != NULL)
                {
                    printf("sucessor do %d: %d\n", x, sucessor->valor);
                }
                else
                {
                    printf("%d nao tem sucessor na arvore\n", x);
                }
            }
            
            break;

        case 5: // busca do predecessor de um no na arvore
            system("clear");
            printf("informe o valor que deseja o predecessor: ");
            scanf("%d", &x);
            r = raiz;
            busca(x, &r, &f);
            if(f != 1){
                printf("%d nao tem predecessor na arvore\n", x);
            }
            else{
                No *predecessor = BuscaPredecessor(r, raiz);

                if (predecessor != NULL)
                {
                    printf("predecessor do %d: %d\n", x, predecessor->valor);
                }
                else
                {
                    printf("%d nao tem predecessor na arvore\n", x);
                }
            }
            break;

        case 6: // insercao de um elemento na arvore
            system("clear");
            printf("informe o valor que deseja inserir: ");
            scanf("%d", &x);
            insercao(x, &raiz);
            break;
        
        case 7: // remocao de um elemento da arvore
            system("clear");
            printf("informe o valor que deseja remover: ");
            scanf("%d", &x);
            r = raiz;
            busca(x, &r, &f);
            if(f != 1){
                printf("no nao esta na arvore\n");
            }
            else{
                Delete(r,&raiz);
            }
            break;

        default: // caso a opcao seja diferente das que se deve colocar
            if (opcao != 0)
                printf("\nopcao invalida\n");
            break;

        }
    }while(opcao!= 0);
    desaloca_arvore(raiz);
}


int main()
{

    gfx_init(1920, 1080, "Desenhos");

    menu();
    
    gfx_quit();
  
    return 0;
}