#ifndef BLOCK_H 
#define BLOCK_H

#define BLOCK_SIZE 8

typedef struct Blocks {
  double*** data;
  int totalBlocks;
} Blocks_t;

typedef struct IntBlocks {
  int*** data;
  int totalBlocks;
} IntBlocks_t;

/**
 * Aloca e retorna um bloco 8x8 de valores double.
 * 
 * @return Ponteiro para matriz 8x8 alocada dinamicamente.
 */
double** createBlock();

/**
 * Aloca e retorna um bloco 8x8 de valores inteiros.
 * 
 * @return Ponteiro para matriz 8x8 alocada dinamicamente.
 */
int** createIntBlock();

/**
 * Divide dois inteiros com arredondamento para cima.
 * 
 * @param a Dividendo
 * @param b Divisor
 * @return Resultado da divisão com ceil.
 */
int ceilDiv(int a, int b);

/**
 * Divide um canal (vetor de pixels) em blocos 8x8.
 * 
 * @param channel Vetor de inteiros representando pixels de um canal.
 * @param width Largura da imagem.
 * @param height Altura da imagem.
 * @return Ponteiro para estrutura Blocks_t contendo blocos 8x8.
 */
Blocks_t* createBlocks(int* channel, int width, int height);

/**
 * Cria uma estrutura IntBlocks_t contendo blocos 8x8 de inteiros.
 * 
 * @param totalBlocks Número total de blocos.
 * @return Ponteiro para estrutura IntBlocks_t.
 */
IntBlocks_t* createIntBlocks(int totalBlocks);

/**
 * Aplica a Transformada Discreta do Cosseno (DCT) em um bloco 8x8.
 * 
 * @param input Bloco 8x8 de entrada.
 * @return Novo bloco 8x8 contendo os coeficientes DCT.
 */
double** getDctBlock(double** input);

/**
 * Aplica DCT a todos os blocos de uma estrutura Blocks_t.
 * 
 * @param blocks Ponteiro para estrutura Blocks_t com os blocos.
 * @return Nova estrutura Blocks_t com blocos transformados.
 */
Blocks_t* getDctBlocks(Blocks_t* blocks);

/**
 * Aplica a Transformada Inversa do Cosseno (IDCT) em um bloco 8x8.
 * 
 * @param input Bloco 8x8 com coeficientes DCT.
 * @return Novo bloco 8x8 com os valores reconstruídos.
 */
double** getIdctBlock(double** input);

/**
 * Aplica IDCT a todos os blocos de uma estrutura Blocks_t.
 * 
 * @param blocks Ponteiro para estrutura Blocks_t com DCTs.
 * @return Nova estrutura Blocks_t com blocos reconstruídos.
 */
Blocks_t* getIdctBlocks(Blocks_t* blocks);

/**
 * Quantiza um bloco 8x8 utilizando uma tabela de quantização.
 * 
 * @param input Bloco 8x8 de entrada.
 * @param quantizationTable Tabela 8x8 com os divisores.
 * @return Bloco 8x8 de inteiros quantizados.
 */
int** getQuantizedBlock(double** input, int quantizationTable[8][8]);

/**
 * Aplica quantização a todos os blocos da estrutura Blocks_t.
 * 
 * @param blocks Blocos de entrada.
 * @param quantizationTable Tabela de quantização 8x8.
 * @return Estrutura com blocos quantizados.
 */
IntBlocks_t* getQuantizedBlocks(Blocks_t* blocks, int quantizationTable[8][8]);

/**
 * Desfaz a quantização de um bloco 8x8.
 * 
 * @param input Bloco 8x8 de inteiros quantizados.
 * @param quantizationTable Tabela 8x8 usada na quantização.
 * @return Bloco 8x8 reconstruído em double.
 */
double** getDequantizedBlock(int** input, int quantizationTable[8][8]);

/**
 * Aplica desquantização em todos os blocos da estrutura IntBlocks_t.
 * 
 * @param blocks Blocos quantizados.
 * @param quantizationTable Tabela usada na quantização.
 * @return Estrutura Blocks_t com os blocos reconstruídos.
 */
Blocks_t* getDequantizedBlocks(IntBlocks_t* blocks, int quantizationTable[8][8]);

/**
 * Libera memória de um bloco 8x8 de doubles.
 */
void destroyBlock(double** block);

/**
 * Libera todos os blocos de uma estrutura Blocks_t.
 */
void destroyBlocks(Blocks_t* blocks);

/**
 * Libera memória de um bloco 8x8 de inteiros.
 */
void destroyIntBlock(int** block);

/**
 * Libera todos os blocos de uma estrutura IntBlocks_t.
 */
void destroyIntBlocks(IntBlocks_t* blocks);

#endif
