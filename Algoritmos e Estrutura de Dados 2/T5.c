#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// Funcao para ler o vetor de inteiros do arquivo binario
int32_t *LerVetor(FILE *arquivo, size_t *tamanho)
{
    int32_t temp;
    size_t capacidade = 10; // Capacidade inicial do vetor
    size_t contador = 0;

    // Aloca memoria para o vetor
    int32_t *vetor = malloc(capacidade * sizeof(int32_t));
    if (vetor == NULL)
    {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    // Le os inteiros do arquivo
    while (fread(&temp, sizeof(int32_t), 1, arquivo) == 1)
    {
        // Redimensiona o vetor se necessario
        if (contador >= capacidade)
        {
            capacidade *= 2;
            int32_t *novo_vetor = realloc(vetor, capacidade * sizeof(int32_t));
            if (novo_vetor == NULL)
            {
                free(vetor); // Libera a memoria original antes de sair
                perror("Erro ao realocar memória");
                exit(EXIT_FAILURE);
            }
            vetor = novo_vetor;
        }
        vetor[contador++] = temp;
    }

    // Ajusta o tamanho final do vetor
    *tamanho = contador;


    if (contador < capacidade)
    {
        int32_t *vetor_final = realloc(vetor, contador * sizeof(int32_t));
        if (vetor_final != NULL)
        {
            vetor = vetor_final;
        }
        // Se realloc falhar, mantemos o vetor original que ja possui espaco suficiente
    }

    return vetor;
}
// funcao para realizar a troca de valores 
void troca(int32_t *a, int32_t *b)
{
    int32_t aux = *a;
    *a = *b;
    *b = aux;
}
// algoritmo de ordenacao por bolha, o menos eficiente de todos
void BubbleSort(int32_t *vetor, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - 1; j++)
        {
            if (vetor[j] > vetor[j + 1]) // faz a verificacao para poder realizar a troca 
            {
                troca(&vetor[j], &vetor[j + 1]);
            }
        }
    }
}
//algoritmo de ordenacao por bolha melhorado, ainda e um pouco ineficiente
void BubbleSortMelhor(int32_t *vetor, int size)
{
    // flag para verificar se houve trocas na ultima passagem
    int flag = 1;
    int n1 = size;
    // aux guarda a posicao da ultima troca
    int aux = size;
    while (flag)
    {
        // inicializa a flag para nao haver mais trocas
        int j = 0;
        flag = 0;
        while (j < n1 - 1)
        {
            // compara elementos adjacentes
            if (vetor[j] > vetor[j + 1])
            {
                // realiza a troca
                troca(&vetor[j], &vetor[j + 1]);
                // mostra que houve troca
                flag = 1;
                aux = j + 1;
            }
            j++;
        }
        // atualiza o limite para a proxima passagem
        n1 = aux;
    }
}

// funcao para ordenar o vetor usando insertion sort
void InsertionSort(int32_t *vetor, int size)
{
    for (int i = 0; i < size; i++)
    {
        int32_t prov = vetor[i];
        int32_t valor = vetor[i];
        // inicia a comparacao com os elementos anteriores
        int j = i - 1;
        while (j >= 0 && valor < vetor[j])
        {
            // move o elemento para a direita
            vetor[j + 1] = vetor[j];
            j = j - 1;
        }
        // insere o elemento na posicao correta
        vetor[j + 1] = prov;
    }
}

// funcao de intercalar usada no Merge sort
void intercalar(int32_t *vetor, int *temp, int ini1, int ini2, int fim2)
{
    int fim1 = ini2 - 1;
    int nro = 0;
    // indice para o vetor temporario
    int ind = ini1;
    // intercalacao das duas metades
    while (ini1 <= fim1 && ini2 <= fim2)
    {
        // compara elementos das duas metades
        if (vetor[ini1] < vetor[ini2])
        {
            // copia o elemento menor para o vetor temporario
            temp[ind] = vetor[ini1];
            ini1 = ini1 + 1;
        }
        else
        {
            temp[ind] = vetor[ini2];
            ini2 = ini2 + 1;
        }
        ind = ind + 1;
        nro = nro + 1;
    }
    // copia os elementos restantes da primeira metade, se houver
    while (ini1 <= fim1)
    {
        temp[ind] = vetor[ini1];
        ini1 = ini1 + 1;
        ind = ind + 1;
        nro = nro + 1;
    }
    // copia os elementos restantes da segunda metade, se houver
    while (ini2 <= fim2)
    {
        temp[ind] = vetor[ini2];
        ini2 = ini2 + 1;
        ind = ind + 1;
        nro = nro + 1;
    }
    // copia os elementos ordenados de volta para o vetor original
    for (int i = 0; i < nro; i++)
    {
        vetor[fim2 - nro + 1 + i] = temp[fim2 - nro + 1 + i];
    }
}
// funcao de ordenacao merge sort
void MergeSort(int32_t *vetor, int *temp, int esq, int dir)
{
    // verifica se o subvetor tem mais de um elemento
    if (esq < dir)
    {
        int centro = (esq + dir) / 2;
        // ordena a primeira metade
        MergeSort(vetor, temp, esq, centro);
        // ordena a segunda metade
        MergeSort(vetor, temp, centro + 1, dir);
        // intercalacao das duas metades ordenadas
        intercalar(vetor, temp, esq, centro + 1, dir);
    }
}

int pivo(int32_t *vetor, int ini, int fim)
{
    int meio = ini + (fim - ini) / 2;

    // Ordena os elementos ini, meio e fim
    if (vetor[ini] > vetor[meio]){
        troca(&vetor[ini], &vetor[meio]);
    }
    if (vetor[ini] > vetor[fim]){
        troca(&vetor[ini], &vetor[fim]);
    }
    if (vetor[meio] > vetor[fim]){
        troca(&vetor[meio], &vetor[fim]);
    }
    // Move o pivo para a posicao fim
    troca(&vetor[meio], &vetor[fim]);

    // Retorna o indice fim, onde o pivo esta localizado agora
    return fim;
}

// Quick sort com pivo no ultimo elemento 
void QuickSortUltimoElem(int32_t *vetor, int ini, int fim)
{
    // verifica se o intervalo tem menos de dois elementos
    if (fim - ini < 2)
    {
        // se tiver exatamente dois elementos, compara e troca se necessario
        if (fim - ini == 1)
        {
            if (vetor[ini] > vetor[fim])
                troca(&vetor[ini], &vetor[fim]);
        }
    }
    else
    {
        // escolhe o pivo como o ultimo elemento
        int32_t key = vetor[fim];
        int i = ini;
        int j = fim - 1;
        // particiona o vetor em menores e maiores que o pivo
        while (j >= i)
        {
            // encontra o primeiro elemento maior ou igual ao pivo
            while (i <= j && vetor[i] < key)
            {
                i++;
            }
            // encontra o primeiro elemento menor ou igual ao pivo a partir do fim
            while (i <= j && vetor[j] > key)
            {
                j--;
            }
            // se os indices nao se cruzaram, troca os elementos
            if (j >= i)
            {
                troca(&vetor[i], &vetor[j]);
                i++;
                j--;
            }
        }
        // coloca o pivo na posicao correta
        troca(&vetor[i], &vetor[fim]);
        // ordena a parte esquerda do pivo
        QuickSortUltimoElem(vetor, ini, i - 1);
        // ordena a parte direita do pivo
        QuickSortUltimoElem(vetor, i + 1, fim);
    }
}

// quick sort com pivo em um elemento aleatorio
void QuickSortElemAlea(int32_t *vetor, int ini, int fim)
{
    // verifica se o intervalo tem menos de dois elementos
    if (fim - ini < 2)
    {
        // se tiver exatamente dois elementos, compara e troca se necessario
        if (fim - ini == 1)
        {
            if (vetor[ini] > vetor[fim])
                troca(&vetor[ini], &vetor[fim]);
        }
    }
    else
    {
        // inicializa a geracao de numeros aleatorios
        srand(time(NULL));
        // escolhe um indice de pivo aleatorio
        int indicePivo = ini + rand() % (fim - ini + 1);
         // troca o pivo aleatorio com o ultimo elemento
        troca(&vetor[indicePivo], &vetor[fim]);
        int32_t key = vetor[fim];
        int i = ini;
        int j = fim - 1;
        // particiona o vetor em menores e maiores que o pivo
        while (j >= i)
        {
            // looping para encontrar o primeiro elemento maior ou igual o pivo
            while (i <= j && vetor[i] < key)
            {
                i++;
            }
            // looping para encontrar o primeiro elemento menor ou igual o pivo a partir do fim
            while (i <= j && vetor[j] > key)
            {
                j--;
            }
             // se os indices nao se cruzaram, troca os elementos
            if (j >= i)
            {
                troca(&vetor[i], &vetor[j]);
                i++;
                j--;
            }
        }
        // coloca o pivo na posicao correta
        troca(&vetor[i], &vetor[fim]);
        // faz as ordenacoes de forma recursiva
        QuickSortElemAlea(vetor, ini, i - 1);
        QuickSortElemAlea(vetor, i + 1, fim);
    }
}

// quick sort com pivo sendo a mediana de 3
void QuickSortMed3(int32_t *vetor, int ini, int fim)
{
    while (fim - ini >= 1)
    { 
        if (fim - ini == 1)
        {
            if (vetor[ini] > vetor[fim])
            {
                troca(&vetor[ini], &vetor[fim]);
            }
            return;
        }
        // escolhe o pivo usando a mediana de tres
        int mediana = pivo(vetor, ini, fim);
        int i = ini;
        int j = fim - 1;
        int key = vetor[fim];

        // Particionamento
        while (j >= i)
        {
            while (i <= j && vetor[i] < key)
            {
                i++;
            }
            while (i <= j && vetor[j] > key)
            {
                j--;
            }
            // se os indices nao se cruzaram, troca os elementos
            if (j >= i)
            {
                troca(&vetor[i], &vetor[j]);
                i++;
                j--;
            }
        }
        troca(&vetor[i], &vetor[fim]); // Coloca o pivo na posicao correta

        // Processa a menor particao primeiro para evitar estouro da pilha
        if ((i - 1 - ini) < (fim - (i + 1)))
        {
            QuickSortMed3(vetor, ini, i - 1);
            ini = i + 1; 
        }
        else
        {
            QuickSortMed3(vetor, i + 1, fim);
            fim = i - 1;
        }
    }
}
// funcao para descer no heap
void descer(int vetor[], int tamanho, int i)
{
    int maior = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;
    // verifica se o filho esquerdo e maior que o elemento atual
    if (esquerda < tamanho && vetor[esquerda] > vetor[maior])
        maior = esquerda;
     // verifica se o filho direito e maior que o elemento atual ou o filho esquerdo
    if (direita < tamanho && vetor[direita] > vetor[maior])
        maior = direita;
    // se o maior nao for o elemento atual, troca e continua ajustando o heap
    if (maior != i)
    {
        troca(&vetor[i], &vetor[maior]);
        // chama recursivamente para ajustar a subarvore afetada
        descer(vetor, tamanho, maior);
    }
}

// Funcao para construir o heap maximo a partir de um array desordenado
void arranjar(int vetor[], int tamanho)
{
    int inicio = tamanho / 2 - 1;
    // ajusta cada subarvore para formar um heap maximo
    for (int i = inicio; i >= 0; i--)
    {
        descer(vetor, tamanho, i);
    }
}

// Funcao principal de ordenacao HeapSort
void heapSort(int vetor[], int tamanho)
{
    // primeiro transforma o vetor em um heap maximo
    arranjar(vetor, tamanho);
    // remove o maior elemento (na raiz do heap) e ajusta o heap
    for (int i = tamanho - 1; i > 0; i--)
    {
        troca(&vetor[0], &vetor[i]);
        // reconstroi o heap maximo para o restante do vetor
        descer(vetor, i, 0);
    }
}

int main(int argc, char *argv[])
{
    // Verifica se o numero correto de argumentos foi fornecido
    if (argc != 4)
    {
        printf("quantidade de argumentos errados");
        return EXIT_FAILURE;
    }

    int opcao = atoi(argv[1]);
    const char *input_filename = argv[2];
    const char *output_filename = argv[3];

    // Abre o arquivo de entrada para leitura binaria
    FILE *arquivo_input = fopen(input_filename, "rb");
    if (arquivo_input == NULL)
    {
        perror("Erro ao abrir o arquivo binário de entrada");
        return EXIT_FAILURE;
    }

    // Le o vetor do arquivo de entrada
    size_t tamanho;
    int32_t *vetor = LerVetor(arquivo_input, &tamanho);
    fclose(arquivo_input);

    if (tamanho == 0)
    {
        fprintf(stderr, "O arquivo de entrada está vazio ou não contém dados válidos.\n");
        free(vetor);
        return EXIT_FAILURE;
    }

    // Aloca memoria para o vetor temporario usado no MergeSort, se necessario
    int32_t *temp = NULL;
    if (opcao == 4)
    { // MergeSort requer vetor temporario
        temp = malloc(tamanho * sizeof(int32_t));
        if (temp == NULL)
        {
            perror("Erro ao alocar memória para temp");
            free(vetor);
            return EXIT_FAILURE;
        }
    }
    clock_t inicio_tempo;
    clock_t fim_tempo;

    // Aplica o algoritmo de ordenacao selecionado
    switch (opcao)
    {
    case 1: // bubble sort
        inicio_tempo = clock();
        BubbleSort(vetor, (int)tamanho);
        fim_tempo = clock();
        break;
    case 2: // bubble sort melhor
        inicio_tempo = clock();
        BubbleSortMelhor(vetor, (int)tamanho);
        fim_tempo = clock();
        break;
    case 3: // insertion sort
        inicio_tempo = clock();
        InsertionSort(vetor, (int)tamanho);
        fim_tempo = clock();
        break;
    case 4: // merge sort
        inicio_tempo = clock();
        MergeSort(vetor, temp, 0, (int)tamanho - 1);
        fim_tempo = clock();
        break;
    case 5: // quick sort pivo no ultimo elemento
        inicio_tempo = clock();
        QuickSortUltimoElem(vetor, 0, (int)tamanho - 1);
        fim_tempo = clock();
        break;
    case 6: // quicksort pivo elemento aleatorio
        inicio_tempo = clock();
        QuickSortElemAlea(vetor, 0, (int)tamanho - 1);
        fim_tempo = clock();
        break;
    case 7: // quick sort pivo mediana de 3
        inicio_tempo = clock();
        QuickSortMed3(vetor, 0, (int)tamanho - 1);
        fim_tempo = clock();
        break;
    case 8: // heap sort
        inicio_tempo = clock();
        heapSort(vetor, (int)tamanho);
        fim_tempo = clock();
        break;
    default: // qualquer numero que nao corresponda aos algoritmos de ordenacao(1 a 8)
        printf("ordenacao invalida");
        free(vetor);
        if (temp)
            free(temp);
        return EXIT_FAILURE;
    }

    // Libera o vetor temporario, se alocado
    if (temp)
    {
        free(temp);
    }

    // Abre o arquivo de saida para escrita binaria
    FILE *arquivo_output = fopen(output_filename, "wb");
    if (arquivo_output == NULL)
    {
        perror("Erro ao abrir o arquivo binário de saída");
        free(vetor);
        return EXIT_FAILURE;
    }

    // Escreve o vetor ordenado diretamente no arquivo de saida
    size_t elementos_escritos = fwrite(vetor, sizeof(int32_t), tamanho, arquivo_output);
    if (elementos_escritos != tamanho)
    {
        perror("Erro ao escrever dados no arquivo de saída");
        fclose(arquivo_output);
        free(vetor);
        return EXIT_FAILURE;
    }

    fclose(arquivo_output);
    printf("Vetor ordenado foi escrito '%s' com sucesso.\n", output_filename);
    printf("Tempo de execucao: %.6f segundos\n", (double)(fim_tempo - inicio_tempo) / CLOCKS_PER_SEC);

    // Libera a memoria alocada para o vetor
    free(vetor);

    return EXIT_SUCCESS;
}