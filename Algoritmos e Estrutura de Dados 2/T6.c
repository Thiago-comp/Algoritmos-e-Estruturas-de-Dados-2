#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BYTE_RANGE 256
#define MAX_TREE_HT 256

// estrutura para frequencia de bytes
typedef struct {
    unsigned char byte;     
    unsigned long frequency; 
} Frequency;

// estrutura do no da arvore de huffman
typedef struct HuffmanNode {
    unsigned char byte;
    unsigned long frequency;    
    struct HuffmanNode *left, *right;
} HuffmanNode;

// estrutura para a fila de prioridade,heap
typedef struct {
    int size;           
    int capacity;       
    HuffmanNode **array; 
} MinHeap;

// estrutura para codigos de huffman
typedef struct {
    unsigned char byte;
    char *code;
} HuffmanCode;

// estrutura para escrever bits no arquivo
typedef struct {
    FILE *file; 
    unsigned char buffer;
    int bit_count;
} BitWriter;

// estrutura para ler bits do arquivo
typedef struct {
    FILE *file;        
    unsigned char buffer; 
    int bit_count;  
} BitReader;

// funcao para contar as frequencias dos bytes no arquivo
Frequency* countFrequencies(const char *filename) {
    // abre o arquivo em modo binario
    FILE *file = fopen(filename, "rb"); 
    // verifica se hove erro ao abrir o arquivo
    if (!file) {
        perror("erro ao abrir o arquivo para leitura");
        exit(EXIT_FAILURE);
    }
    // aloca memoria para frequencias 
    Frequency *freq = calloc(BYTE_RANGE, sizeof(Frequency)); 
    // verifica se houve erro na alocacao de memoria
    if (!freq) {
        perror("erro ao alocar memoria para frequencias");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // inicializa os bytes e frequencias
    for (int i = 0; i < BYTE_RANGE; i++) {
        freq[i].byte = (unsigned char)i;
        freq[i].frequency = 0;
    }

    int byte;
    // le cada byte do arquivo
    while ((byte = fgetc(file)) != EOF) { 
        freq[byte].frequency++; // incrementa a frequencia do byte
    }

    fclose(file);
    return freq;   // retorna o array de frequencias
}

// funcao para criar um novo no de huffman
HuffmanNode* createNode(unsigned char byte, unsigned long frequency) {
    HuffmanNode *node = malloc(sizeof(HuffmanNode)); // aloca memoria para o no
    // verifica se houve erro na alocacao de memoria
    if (!node) {
        perror("erro ao alocar memoria para no de huffman");
        exit(EXIT_FAILURE);
    }
    // faz as devidas atribuicoes necessarias
    node->byte = byte;
    node->frequency = frequency; 
    node->left = node->right = NULL; 
    return node; // retorna o no criado
}

// funcao para criar a heap
MinHeap* createMinHeap(int capacity) {
    MinHeap *minHeap = malloc(sizeof(MinHeap)); // aloca memoria para a heap
    // verifica se houve erro na alocacao
    if (!minHeap) {
        perror("erro ao alocar memoria para minheap");
        exit(EXIT_FAILURE);
    }
    // faz as atribuicoes corretas
    minHeap->size = 0; // inicializa o tamanho
    minHeap->capacity = capacity; 
    minHeap->array = malloc(capacity * sizeof(HuffmanNode*)); 
    // verifica se houve erro na alocacao do array
    if (!minHeap->array) {
        perror("erro ao alocar memoria para array da minheap");
        free(minHeap);
        exit(EXIT_FAILURE);
    }
    return minHeap; // retorna a heap criada
}

// funcao para trocar dois nos na heap
void swapHuffmanNode(HuffmanNode **a, HuffmanNode **b) {
    HuffmanNode *t = *a;
    *a = *b;
    *b = t;
}

// funcao para manter a propriedade da heap minima
void minHeapify(MinHeap *minHeap, int idx) {
    int smallest = idx; 
    int left = 2 * idx + 1; // indice do filho esquerdo
    int right = 2 * idx + 2; // indice do filho direito

    // verifica se o filho esquerdo tem menor frequencia
    if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
        smallest = left;

    // verifica se o filho direito tem menor frequencia
    if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
        smallest = right;

    // se o menor nao for o atual, troca e chama a funcao recursivamente
    if (smallest != idx) {
        swapHuffmanNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// funcao para verificar se a heap tem apenas um elemento
int isSizeOne(MinHeap *minHeap) {
    return (minHeap->size == 1);
}

// funcao para extrair o no com menor frequencia
HuffmanNode* extractMin(MinHeap *minHeap) {
    HuffmanNode *temp = minHeap->array[0]; // pega o menor elemento
    minHeap->array[0] = minHeap->array[minHeap->size - 1]; // move o ultimo para o primeiro
    minHeap->size--;
    minHeapify(minHeap, 0); // chama a funcao recurcivamente para heapficar a partir do topo
    return temp;
}

// funcao para inserir um no na heap minima
void insertMinHeap(MinHeap *minHeap, HuffmanNode *node) {
    minHeap->size++; 
    int i = minHeap->size - 1; // indice do novo no

    // sobe o no na heap enquanto a frequencia for menor que o pai
    while (i && node->frequency < minHeap->array[(i - 1) / 2]->frequency) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node; // coloca o no na posicao correta
}

// funcao para construir a heap minima
void buildMinHeap(MinHeap *minHeap) {
    int n = minHeap->size - 1; 
    // heapifica de baixo para cima
    for (int i = (n - 1) / 2; i >= 0; i--)
        minHeapify(minHeap, i);
}

// funcao para verificar se um no e folha
int isLeaf(HuffmanNode *node) {
    return !(node->left) && !(node->right);
}

// funcao para construir a arvore de huffman a partir das frequencias
HuffmanNode* buildHuffmanTree(Frequency *freq) {
    int nonZero = 0;
    // conta quantos bytes tem frequencia maior que zero
    for (int i = 0; i < BYTE_RANGE; i++) {
        if (freq[i].frequency > 0)
            nonZero++;
    }

    MinHeap *minHeap = createMinHeap(nonZero); // cria a heap com os nos folha
    for (int i = 0; i < BYTE_RANGE; i++) {
        if (freq[i].frequency > 0) {
            minHeap->array[minHeap->size++] = createNode(freq[i].byte, freq[i].frequency); // adiciona na heap
        }
    }

    buildMinHeap(minHeap); // constroi a heap minima

    // construi a arvore de huffman
    while (minHeap->size != 1) {
        HuffmanNode *left = extractMin(minHeap); // extrai o menor
        HuffmanNode *right = extractMin(minHeap); // extrai o segundo menor

        HuffmanNode *internal = createNode(0, left->frequency + right->frequency);
        // faz as atribuicoes corretas
        internal->left = left;   
        internal->right = right;

        insertMinHeap(minHeap, internal); // insere o no interno de volta na heap
    }

    HuffmanNode *root = extractMin(minHeap); // raiz da arvore
    // faz as liberacoes corretas de memoriads
    free(minHeap->array); 
    free(minHeap); 
    return root; 
}

// funcao para atribuir codigos de huffman aos bytes
void assignCodes(HuffmanNode *root, char *code, int top, HuffmanCode *huffCodes) {
    if (isLeaf(root)) { // se for folha
        // verifica se e um caso especial, apenas um byte presente
        if (top == 0) { 
            code[top++] = '0';
        }
        code[top] = '\0';
        huffCodes[root->byte].byte = root->byte; // atribui o byte
        huffCodes[root->byte].code = strdup(code); 
        return;
    }
    // verifica se tem filho esquerdo
    if (root->left) { 
        code[top] = '0'; 
        assignCodes(root->left, code, top + 1, huffCodes); // recursao para o filho esquerdo
    }

    if (root->right) { // se tiver filho direito
        code[top] = '1';
        assignCodes(root->right, code, top + 1, huffCodes); // chama recursivamente para o filho direito
    }
}

// funcao para obter todos os codigos de huffman
HuffmanCode* getHuffmanCodes(HuffmanNode *root) {
    HuffmanCode *huffCodes = malloc(BYTE_RANGE * sizeof(HuffmanCode)); // aloca memoria para os codigos
    if (!huffCodes) {
        perror("erro ao alocar memoria para codigos de huffman");
        exit(EXIT_FAILURE);
    }

    // faz as inicializacoes dos codigos
    for (int i = 0; i < BYTE_RANGE; i++) {
        huffCodes[i].byte = (unsigned char)i;
        huffCodes[i].code = NULL;
    }

    char code[MAX_TREE_HT];
    assignCodes(root, code, 0, huffCodes); // atribui os codigos

    return huffCodes; // retorna os codigos
}

// funcao para serializar a arvore de huffman 
void serializeHuffmanTree(HuffmanNode *root, FILE *out) {
    // verifica se e um no folha
    if (isLeaf(root)) { 
        unsigned char flag = 1; // flag para folha
        if (fwrite(&flag, sizeof(unsigned char), 1, out) != 1) { // verifica se houve erro ao escrever a flag
            perror("erro ao escrever flag de folha na arvore de huffman");
            exit(EXIT_FAILURE);
        }
        if (fwrite(&(root->byte), sizeof(unsigned char), 1, out) != 1) { // verifica se conseguiu escrever o byte
            perror("erro ao escrever byte da arvore de huffman");
            exit(EXIT_FAILURE);
        }
        return;
    } else { // se nao for folha, e um no interno
        unsigned char flag = 0; // flag para no interno
        if (fwrite(&flag, sizeof(unsigned char), 1, out) != 1) { // escreve a flag
            perror("erro ao escrever flag de no interno na arvore de huffman");
            exit(EXIT_FAILURE);
        }
        if (root->left)
            serializeHuffmanTree(root->left, out); // chama a funcao recursivamente para serializar o filho esquerdo
        if (root->right)
            serializeHuffmanTree(root->right, out); // chama a funcao recursivamente para serializar o filho direito
    }
}

// funcao para inicializar o bitwriter
BitWriter* createBitWriter(FILE *file) {
    BitWriter *bw = malloc(sizeof(BitWriter)); // aloca memoria para o bitwriter
    if (!bw) {
        perror("erro ao alocar bitwriter");
        exit(EXIT_FAILURE);
    }
    // faz as atribuicoes corretas e retorna o bitwriter
    bw->file = file;      
    bw->buffer = 0;
    bw->bit_count = 0; 
    return bw; // retorna o bitwriter
}

// escreve um bit no bitwriter
void writeBit(BitWriter *bw, int bit) {
    bw->buffer = (bw->buffer << 1) | (bit & 1); // adiciona o bit ao buffer
    bw->bit_count++;
    // verifica se o buffer esta cheio
    if (bw->bit_count == 8) { 
        if (fputc(bw->buffer, bw->file) == EOF) { // escreve o byte no arquivo
            perror("erro ao escrever byte no arquivo de saida");
            exit(EXIT_FAILURE);
        }
        // reseta o buffer e a contagem de bits
        bw->buffer = 0; 
        bw->bit_count = 0; 
    }
}

// escreve uma sequencia de bits no bitwriter
void writeBits(BitWriter *bw, const char *bits) {
    // loop para percorrer cada bit na string
    for (int i = 0; bits[i] != '\0'; i++) { 
        writeBit(bw, bits[i] - '0'); // escreve o bit
    }
}

// finaliza a escrita dos bits no bitwriter
void flushBits(BitWriter *bw) {
    if (bw->bit_count > 0) { // se houver bits no buffer
        bw->buffer <<= (8 - bw->bit_count); // preenche com zeros
        if (fputc(bw->buffer, bw->file) == EOF) { // escreve o byte no arquivo
            perror("erro ao finalizar a escrita dos bits no arquivo de saida");
            exit(EXIT_FAILURE);
        }
    }
}

// funcao para inicializar o bitreader
BitReader* createBitReader(FILE *file) {
    BitReader *br = malloc(sizeof(BitReader)); // aloca memoria para o bitreader
    if (!br) {
        perror("erro ao alocar bitreader");
        exit(EXIT_FAILURE);
    }
    // faz as atribuicoes e retorna o bitreader
    br->file = file;
    br->buffer = 0; 
    br->bit_count = 0;
    return br; // retorna o bitreader
}

// funcao para ler um bit do bitreader
int readBit(BitReader *br, int *bit) {
    if (br->bit_count == 0) { // se nao houver bits no buffer
        int byte = fgetc(br->file); // le um byte do arquivo
        if (byte == EOF)
            return 0;
        // faz as devidas atribuicoes 
        br->buffer = byte; 
        br->bit_count = 8; 
    }
    *bit = (br->buffer & 0x80) ? 1 : 0; // extrai o bit mais significativo
    br->buffer <<= 1; // desloca o buffer para a esquerda
    br->bit_count--; 
    return 1; // retorna 1 se teve sucesso ao ler o bit
}

// funcao para desserializar a arvore de huffman
HuffmanNode* deserializeHuffmanTreeHelper(FILE *file) {
    unsigned char flag;
    // le a flag
    if (fread(&flag, sizeof(unsigned char), 1, file) != 1) { 
        perror("erro ao ler flag da arvore de huffman");
        exit(EXIT_FAILURE);
    }

    if (flag == 1) { // verifica se e um no folha
        unsigned char byte;
        // le o byte
        if (fread(&byte, sizeof(unsigned char), 1, file) != 1) { 
            perror("erro ao ler byte da arvore de huffman");
            exit(EXIT_FAILURE);
        }
        HuffmanNode *leaf = createNode(byte, 0); // cria o no folha
        return leaf; 
    } else { // se nao for no folha, entao e um no interno
        HuffmanNode *node = createNode(0, 0); 
        node->left = deserializeHuffmanTreeHelper(file); // desserializa o filho da esquerda
        node->right = deserializeHuffmanTreeHelper(file); // desserializa o filho da direito
        return node;
    }
}

// funcao para desserializar a arvore de huffman
HuffmanNode* deserializeHuffmanTree(FILE *file) {
    return deserializeHuffmanTreeHelper(file);
}

// funcao para decodificar os dados usando a arvore de huffman
void decodeData(HuffmanNode *root, FILE *arq, FILE *out, unsigned long total_bits) {
    HuffmanNode *current = root;
    int bit;
    unsigned long bits_read = 0;

    BitReader *br = createBitReader(arq); // chama a funcoa para criar o bitreader

    while (bits_read < total_bits) { // fica no loop enquanto houver bits para ler
        // verifica se conseguiu ler o bit se nao sai
        if (!readBit(br, &bit)) { 
            break; 
        }
        bits_read++;
        // verifica se o bit e 1 para ir a direita ou 0 para a esquerda
        if (bit)
            current = current->right; // vai para a direita
        else
            current = current->left;  // vai para a esquerda
        // verifica se chegou a uma folha
        if (isLeaf(current)) { 
            // escreve o byte no arquivo de saida
            if (fputc(current->byte, out) == EOF) { 
                perror("erro ao escrever byte no arquivo de saida durante a descompactacao");
                exit(EXIT_FAILURE);
            }
            current = root; // reseta para a raiz
        }
    }

    free(br); // libera o bitreader
}

// funcao para liberar a arvore de huffman
void freeHuffmanTree(HuffmanNode *root) {
    if (root == NULL)
        return; // se o no for nulo, sai
    freeHuffmanTree(root->left);  // libera o filho esquerdo
    freeHuffmanTree(root->right); // libera o filho direito
    free(root);                    // libera o no atual
}

// funcao para compactar o arquivo
void compressFile(const char *input_filename) {
    printf("iniciando a compactacao de '%s'\n", input_filename);
    
    // chama as funcoes para contar frequecias, construir a arvore e o obter os codigos de huffman
    Frequency *freq = countFrequencies(input_filename);

    HuffmanNode *root = buildHuffmanTree(freq);

    HuffmanCode *huffCodes = getHuffmanCodes(root);

    // abre o arquivo de entrada novamente para leitura
    FILE *arq = fopen(input_filename, "rb");
    if (!arq) {
        perror("erro ao abrir o arquivo para leitura na compactacao");
        exit(EXIT_FAILURE);
    }

    // cria o nome do arquivo de saida com o sufixo .cmp
    char output_filename[strlen(input_filename) + 5];
    sprintf(output_filename, "%s.cmp", input_filename);
    printf("arquivo de saida: '%s'\n", output_filename);
    FILE *out = fopen(output_filename, "wb");
    if (!out) {
        perror("erro ao abrir o arquivo para escrita na compactacao");
        fclose(arq);
        exit(EXIT_FAILURE);
    }

    // serializa a arvore de huffman no arquivo de saida
    serializeHuffmanTree(root, out);

    // reserva espaço para o total de bits
    unsigned long total_bits = 0;
    long bits_position = ftell(out); // guarda a posicao atual
    if (bits_position == -1L) {
        perror("erro ao obter a posicao do arquivo");
        exit(EXIT_FAILURE);
    }

    // escreve um placeholder para total_bits (8 bytes)
    unsigned long placeholder = 0;
    if (fwrite(&placeholder, sizeof(unsigned long), 1, out) != 1) {
        perror("erro ao escrever placeholder de bits no arquivo de saida");
        exit(EXIT_FAILURE);
    }

    // inicializa o bitwriter para escrever os dados codificados
    BitWriter *bw = createBitWriter(out);

    // codifica os dados e escreve no arquivo, contando os bits
    int byte;
    while ((byte = fgetc(arq)) != EOF) { // fica em loop lendo cada byte do arquivo de entrada
        // verifica se existe o codigo
        if (huffCodes[byte].code == NULL) { 
            fprintf(stderr, "erro: codigo de huffman nao encontrado para o byte %d\n", byte);
            exit(EXIT_FAILURE);
        }
        writeBits(bw, huffCodes[byte].code); // escreve os bits no arquivo de saida
        total_bits += strlen(huffCodes[byte].code); // conta os bits escritos
    }

    // finaliza a escrita dos bits
    flushBits(bw);

    // atualiza o total_bits no arquivo
    long end_position = ftell(out);
    if (end_position == -1L) {
        perror("erro ao obter a posicao do arquivo apos escrita");
        exit(EXIT_FAILURE);
    }

    // retrocede para a posicao do placeholder
    if (fseek(out, bits_position, SEEK_SET) != 0) {
        perror("erro ao retroceder para a posicao do placeholder no arquivo");
        exit(EXIT_FAILURE);
    }

    // escreve o total de bits
    if (fwrite(&total_bits, sizeof(unsigned long), 1, out) != 1) {
        perror("erro ao escrever total_bits no arquivo de saida");
        exit(EXIT_FAILURE);
    }

    // avanca para o final do arquivo
    if (fseek(out, end_position, SEEK_SET) != 0) {
        perror("erro ao avancar para a posicao final do arquivo");
        exit(EXIT_FAILURE);
    }

    // libera recursos
    free(bw); 
    // fechar os arquivos de entrada e de saida
    fclose(arq); 
    fclose(out);

    // fica em loop para libera a memoria das estruturas
    for (int i = 0; i < BYTE_RANGE; i++) {
        if (huffCodes[i].code)
            free(huffCodes[i].code); // libera o codigo
    }
    // libera os recursos
    free(huffCodes);
    freeHuffmanTree(root); 
    free(freq);  

    printf("compactacao concluida com sucesso. arquivo gerado: '%s'\n", output_filename);
}

// funcao para descompactar o arquivo
void decompressFile(const char *input_filename) {
    printf("iniciando a descompactacao de '%s'\n", input_filename);
    
    // abre o arquivo de entrada para leitura
    FILE *in = fopen(input_filename, "rb");
    if (!in) {
        perror("erro ao abrir o arquivo para leitura na descompactacao");
        exit(EXIT_FAILURE);
    }

    // desserializa a arvore de huffman
    HuffmanNode *root = deserializeHuffmanTree(in);
    if (!root) {
        fprintf(stderr, "erro ao desserializar a arvore de huffman\n");
        fclose(in);
        exit(EXIT_FAILURE);
    }

    // le o total de bits
    unsigned long total_bits;
    if (fread(&total_bits, sizeof(unsigned long), 1, in) != 1) {
        perror("erro ao ler total_bits do arquivo de compactacao");
        fclose(in);
        exit(EXIT_FAILURE);
    }

    // cria o nome do arquivo de saida com o sufixo .dcmp
    char output_filename[strlen(input_filename) + 6]; 
    sprintf(output_filename, "%s.dcmp", input_filename);
    printf("arquivo de saida: '%s'\n", output_filename);
    FILE *out = fopen(output_filename, "wb");
    if (!out) {
        perror("erro ao abrir o arquivo para escrita na descompactacao");
        fclose(in);
        exit(EXIT_FAILURE);
    }

    // decodifica os dados e escreve no arquivo de saida
    decodeData(root, in, out, total_bits);

    // libera recursos
    // fecha os arquivos de entrada e de saida
    fclose(in);  
    fclose(out); 

    // libera a arvore de huffman
    freeHuffmanTree(root);

    printf("descompactacao concluida com sucesso. arquivo gerado: '%s'\n", output_filename);
}

int main(int argc, char *argv[]) {
    // verifica se o numero de argumentos esta correto
    if (argc != 3) {
        fprintf(stderr, "uso: %s <c|d> <nome_do_arquivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // verifica qual opcao foi desejada pelo usuario
    char option = argv[1][0];
    if (option != 'c' && option != 'd') {
        fprintf(stderr, "opcao invalida. use 'c' para compactar ou 'd' para descompactar.\n");
        return EXIT_FAILURE;
    }

    const char *input_filename = argv[2]; // nome do arquivo de entrada

    if (option == 'c') { // se for para compactar, chama a funcao de compactacao
        compressFile(input_filename); 

    } else { // senao, significa que e para descompactar e chama a funcao de descompactacao
        decompressFile(input_filename); 
    }

    return EXIT_SUCCESS; 
}