#ifndef IMAGE_H 
#define IMAGE_H

#pragma pack(push, 1)

/**
 * Estrutura obtida em aula que representa o cabeçalho principal de um arquivo BMP (14 bytes).
 * Os campos definem o tipo de arquivo e a posição onde começam os dados de imagem.
 */
typedef struct {
  unsigned short bfType;      ///< Tipo do arquivo (deve ser 'BM')
  unsigned int bfSize;        ///< Tamanho total do arquivo em bytes
  unsigned short bfReserved1; ///< Reservado (deve ser 0)
  unsigned short bfReserved2; ///< Reservado (deve ser 0)
  unsigned int bfOffBits;     ///< Offset (em bytes) até os dados de imagem
} BMPFILEHEADER;

/**
 * Estrutura obtida em aula que representa o cabeçalho de informações da imagem BMP (40 bytes).
 * Define as dimensões, cores e características da imagem armazenada.
 */
typedef struct {
  unsigned int biSize;           ///< Tamanho da estrutura (40 bytes)
  int biWidth;                   ///< Largura da imagem (em pixels)
  int biHeight;                  ///< Altura da imagem (em pixels)
  unsigned short biPlanes;       ///< Número de planos (deve ser 1)
  unsigned short biBitCount;     ///< Bits por pixel (geralmente 24 para RGB)
  unsigned int biCompression;    ///< Tipo de compressão (0 = sem compressão)
  unsigned int biSizeImage;      ///< Tamanho dos dados da imagem
  int biXPelsPerMeter;           ///< Resolução horizontal
  int biYPelsPerMeter;           ///< Resolução vertical
  unsigned int biClrUsed;        ///< Número de cores usadas
  unsigned int biClrImportant;   ///< Número de cores importantes
} BMPINFOHEADER;

/**
 * Representa um pixel RGB, com componentes Red, Green e Blue. Obtida em aula.
 */
typedef struct {
  int R;  ///< Canal vermelho (0–255)
  int G;  ///< Canal verde (0–255)
  int B;  ///< Canal azul (0–255)
} Pixel;

/**
 * Lê o cabeçalho principal do arquivo BMP. Obtida em aula.
 *
 * @param F Ponteiro para o arquivo BMP aberto.
 * @param H Ponteiro para a estrutura BMPFILEHEADER a ser preenchida.
 */
void leituraHeader(FILE *F, BMPFILEHEADER *H);

/**
 * Lê o cabeçalho de informações da imagem BMP. Obtida em aula.
 *
 * @param F Ponteiro para o arquivo BMP aberto.
 * @param H Ponteiro para a estrutura BMPINFOHEADER a ser preenchida.
 */
void leituraInfoHeader(FILE *F, BMPINFOHEADER *H);

/**
 * Imprime os dados do cabeçalho principal BMP no terminal.
 *
 * @param header Ponteiro para estrutura BMPFILEHEADER.
 */
void printFileHeader(BMPFILEHEADER *header);

/**
 * Imprime os dados do cabeçalho de informações da imagem BMP no terminal.
 *
 * @param header Ponteiro para estrutura BMPINFOHEADER.
 */
void printInfoHeader(BMPINFOHEADER *header);

/**
 * Lê os dados de imagem RGB do arquivo BMP e preenche o vetor de pixels.
 * Se a compressão for com perdas, aplica padding para que a imagem tenha dimensões múltiplas de 8.
 *
 * @param input Ponteiro para o arquivo BMP aberto.
 * @param InfoHeader Cabeçalho com as dimensões da imagem.
 * @param Image Vetor de pixels a ser preenchido.
 * @param compressionType Tipo de compressão (1 = sem perdas, 2 = com perdas).
 */
void loadBMPImage(FILE *input, BMPINFOHEADER InfoHeader, Pixel *Image, int compressionType);

#endif
