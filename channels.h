#ifndef CHANNELS_H 
#define CHANNELS_H

#include "image.h"

typedef struct Channels {
  int *Y;
  int *Cb;
  int *Cr;
} Channels_t;

Channels_t* createChannels(Pixel* Image, int width, int height);

int* getY(Channels_t* channels);

int* getCb(Channels_t* channels);

int* getCr(Channels_t* channels);

void destroyChannels(Channels_t* channels);

#endif
