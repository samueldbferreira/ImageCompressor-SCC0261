#include <stdio.h>
#include <stdlib.h>
#include "channels.h"

// Converte imagem RGB para canais Y, Cb e Cr
Channels_t* createChannels(Pixel* Image, int width, int height) {
  Channels_t* channels = (Channels_t*) malloc(sizeof(Channels_t));
  if (channels == NULL) {
    printf("Invalid channels (NULL) on createChannels.\n");
    return NULL;
  }

  int* Y = (int*) malloc(width * height * sizeof(int));
  int* Cb = (int*) malloc(width * height * sizeof(int));
  int* Cr = (int*) malloc(width * height * sizeof(int));

  if (Y == NULL || Cb == NULL || Cr == NULL) {
    printf("Memory allocation failed for channels.\n");
    return NULL;
  }

  // Conversão RGB → YCbCr (com offset +128 nos canais Cb e Cr)
  for (int i = 0; i < (width * height); i++) {  
    Y[i]  = (int)( 0.299000 * Image[i].R + 0.587000 * Image[i].G + 0.114000 * Image[i].B);
    Cb[i] = (int)(-0.168736 * Image[i].R - 0.331264 * Image[i].G + 0.500000 * Image[i].B + 128);
    Cr[i] = (int)( 0.500000 * Image[i].R - 0.418688 * Image[i].G - 0.081312 * Image[i].B + 128);
  }

  channels->Y = Y;
  channels->Cb = Cb;
  channels->Cr = Cr;

  return channels;
}

// Retorna ponteiro para o canal Y
int* getY(Channels_t* channels) {
  if (channels == NULL) {
    printf("Invalid channels (NULL) for getY.\n");
    return NULL;
  }
  return channels->Y;
}

// Retorna ponteiro para o canal Cb
int* getCb(Channels_t* channels) {
  if (channels == NULL) {
    printf("Invalid channels (NULL) for getCb.\n");
    return NULL;
  }
  return channels->Cb;
}

// Retorna ponteiro para o canal Cr
int* getCr(Channels_t* channels) {
  if (channels == NULL) {
    printf("Invalid channels (NULL) for getCr.\n");
    return NULL;
  }
  return channels->Cr;
}

// Libera memória alocada para os canais Y, Cb e Cr
void destroyChannels(Channels_t* channels) {
  if (channels == NULL) {
    printf("Invalid channels (NULL) for destroyChannels.\n");
    return;
  }

  free(channels->Y);
  free(channels->Cb);
  free(channels->Cr);
  free(channels);
}
