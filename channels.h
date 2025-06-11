#ifndef CHANNELS_H 
#define CHANNELS_H

#include "image.h"

/**
 * Estrutura que armazena os canais separados Y, Cb e Cr da imagem.
 * Cada canal é representado como um vetor de inteiros.
 */
typedef struct Channels {
  int *Y;   ///< Canal de luminância (brilho)
  int *Cb;  ///< Canal de crominância azul
  int *Cr;  ///< Canal de crominância vermelha
} Channels_t;

/**
 * Converte uma imagem RGB em canais Y, Cb e Cr.
 * 
 * @param Image Vetor de Pixels no formato RGB.
 * @param width Largura da imagem.
 * @param height Altura da imagem.
 * @return Ponteiro para estrutura Channels_t com os canais convertidos.
 */
Channels_t* createChannels(Pixel* Image, int width, int height);

/**
 * Retorna o ponteiro para o canal Y (luminância).
 * 
 * @param channels Ponteiro para estrutura Channels_t.
 * @return Ponteiro para vetor de inteiros correspondente ao canal Y.
 */
int* getY(Channels_t* channels);

/**
 * Retorna o ponteiro para o canal Cb (crominância azul).
 * 
 * @param channels Ponteiro para estrutura Channels_t.
 * @return Ponteiro para vetor de inteiros correspondente ao canal Cb.
 */
int* getCb(Channels_t* channels);

/**
 * Retorna o ponteiro para o canal Cr (crominância vermelha).
 * 
 * @param channels Ponteiro para estrutura Channels_t.
 * @return Ponteiro para vetor de inteiros correspondente ao canal Cr.
 */
int* getCr(Channels_t* channels);

/**
 * Libera a memória alocada para os canais Y, Cb e Cr.
 * 
 * @param channels Ponteiro para estrutura Channels_t a ser destruída.
 */
void destroyChannels(Channels_t* channels);

#endif
