#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned short bfType;
  unsigned int bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned int bfOffBits;
} BMPFILEHEADER;
 
typedef struct {
  unsigned int biSize;
  int biWidth;
  int biHeight;
  unsigned short biPlanes;
  unsigned short biBitCount;
  unsigned int   biCompression;
  unsigned int   biSizeImage;
  int biXPelsPerMeter;
  int biYPelsPerMeter;
  unsigned int   biClrUsed;
  unsigned int   biClrImportant;
} BMPINFOHEADER;

typedef struct {
  unsigned char R;
  unsigned char G;
  unsigned char B;      
} Pixel;

void leituraHeader(FILE *F, BMPFILEHEADER *H) {
  fread(&H->bfType, sizeof(unsigned short), 1, F);
  fread(&H->bfSize, sizeof(unsigned int), 1, F);
  fread(&H->bfReserved1, sizeof(unsigned short), 1, F);
  fread(&H->bfReserved2, sizeof(unsigned short), 1, F);
  fread(&H->bfOffBits, sizeof(unsigned int), 1, F);
}

void leituraInfoHeader(FILE *F, BMPINFOHEADER *H) {
  fread(&H->biSize, sizeof(unsigned int), 1, F);
  fread(&H->biWidth, sizeof(int), 1, F);
  fread(&H->biHeight, sizeof(int), 1, F);
  fread(&H->biPlanes, sizeof(unsigned short), 1, F);
  fread(&H->biBitCount, sizeof(unsigned short), 1, F);
  fread(&H->biCompression, sizeof(unsigned int), 1, F);
  fread(&H->biSizeImage, sizeof(unsigned int), 1, F);
  fread(&H->biXPelsPerMeter, sizeof(int), 1, F);
  fread(&H->biYPelsPerMeter, sizeof(int), 1, F);
  fread(&H->biClrUsed, sizeof(unsigned int), 1, F);
  fread(&H->biClrImportant, sizeof(unsigned int), 1, F);
}

void printFileHeader(BMPFILEHEADER *header) {
  printf("File Header:\n");
  printf("bfType: 0x%X\n", header->bfType);  // Deve ser 'BM' (0x4D42 em little-endian)
  printf("bfSize: %u bytes\n", header->bfSize);
  printf("bfReserved1: %u\n", header->bfReserved1);  // Deve ser 0
  printf("bfReserved2: %u\n", header->bfReserved2);  // Deve ser 0
  printf("bfOffBits: %u bytes\n", header->bfOffBits);  // Offset para os dados de pixel
}

void printInfoHeader(BMPINFOHEADER *header) {
  printf("\nInfo Header:\n");
  printf("biSize: %u\n", header->biSize);
  printf("biWidth: %d\n", header->biWidth);
  printf("biHeight: %d\n", header->biHeight);
  printf("biPlanes: %u\n", header->biPlanes);
  printf("biBitCount: %u\n", header->biBitCount);
  printf("biCompression: %u\n", header->biCompression);
  printf("biSizeImage: %u\n", header->biSizeImage);
  printf("biXPelsPerMeter: %d\n", header->biXPelsPerMeter);
  printf("biYPelsPerMeter: %d\n", header->biYPelsPerMeter);
  printf("biClrUsed: %u\n", header->biClrUsed);
  printf("biClrImportant: %u\n", header->biClrImportant);
}

void loadBMPImage(FILE *input, BMPINFOHEADER InfoHeader, Pixel *Image){
  int i, j, tam;
 
  tam = InfoHeader.biHeight * InfoHeader.biWidth;
  fseek(input, 54, SEEK_SET); //skipping the header (54 bytes)
   
  for (i=0; i < tam; i++){
    Image[i].B = fgetc(input);
    Image[i].G = fgetc(input);
    Image[i].R = fgetc(input);
  }
}

struct ListNode {
  int difference;
  int frequence;
  struct ListNode* next;
};

typedef struct ListNode ListNode_t;

struct List {
  ListNode_t* first;
  ListNode_t* last;
};

typedef struct List List_t;

List_t* createList() {
  List_t* list = (List_t*) malloc(sizeof(List_t));

  if (list == NULL) {
    printf("Invalid list (NULL) on createList.");
    return NULL;
  }

  list->first = NULL;
  list->last = NULL;

  return list;
};

void listInsert(List_t* list, int difference) {
  if (list == NULL) {
    printf("Invalid list (NULL) for listInsert.");
    return;
  }

  ListNode_t* newNode = (ListNode_t*) malloc(sizeof(ListNode_t));
  newNode->difference = difference;
  newNode->frequence = 0;
  newNode->next = NULL;

  if (list->first == NULL) {
    list->first = newNode;
    list->last = newNode;
    return;
  }

  list->last->next = newNode;
  list->last = newNode;
}

void printList(List_t* list) {
  if (list == NULL) {
    printf("Invalid list (NULL) for printList.");
    return;
  }

  int i = 1;
  ListNode_t* aux = list->first;
  while (aux != NULL) {
    printf("NODE %d: (diff > %i; freq > %i)\n", i, aux->difference, aux->frequence);

    aux = aux->next;

    i++;
  }
  printf("\n");
}

void destroyList(List_t* list) {
  if (list == NULL) {
    printf("Invalid list (NULL) for destroyList.");
    return;
  }

  ListNode_t* aux = list->first;
  while (aux != NULL) {
    list->first = aux->next;
    free(aux);
    aux = list->first;
  }

  free(list);
}

int main() {
  char* inputFilePath = "./256x256.bmp";
  FILE* inputBmpFile = fopen(inputFilePath, "rb");
  if (!inputBmpFile) {
    printf("Error opening file %s\n", inputFilePath);
    return 1;
  }

  BMPFILEHEADER fileHeader;
  leituraHeader(inputBmpFile, &fileHeader);
  printFileHeader(&fileHeader);

  BMPINFOHEADER infoHeader;
  leituraInfoHeader(inputBmpFile, &infoHeader);
  printInfoHeader(&infoHeader);

  Pixel *Image;
  Image = (Pixel *) malloc((infoHeader.biWidth * infoHeader.biHeight) * sizeof(Pixel));
  loadBMPImage(inputBmpFile, infoHeader, Image);

  char* outputFilePath = "./output.bmp";
  FILE* bmpOutputFile = fopen(outputFilePath, "wb");
  if(!bmpOutputFile) {
    printf("Error opening output file %s\n", outputFilePath);
    return 1;
  }
                  
  fseek(inputBmpFile, 0, SEEK_SET);
  
  // Reescrevendo o cabeçalho do arquivo BMP
  for (int i = 0; i < 54; i++) {
    fputc(fgetc(inputBmpFile), bmpOutputFile);
  }

  // Compressão simples: armazenando a diferença entre os pixels
  // O primeiro pixel é armazenado diretamente
  fputc(Image[0].B, bmpOutputFile);
  fputc(Image[0].G, bmpOutputFile);
  fputc(Image[0].R, bmpOutputFile);

  // Armazenando a diferença entre os pixels consecutivos
  // O primeiro pixel já foi armazenado, então começamos do segundo
  for (int i = 1; i < (infoHeader.biHeight * infoHeader.biWidth); i++) {
    fputc(Image[i].B - Image[i - 1].B, bmpOutputFile);
    fputc(Image[i].G - Image[i - 1].G, bmpOutputFile);
    fputc(Image[i].R - Image[i - 1].R, bmpOutputFile);
  }

  fclose(inputBmpFile);
  fclose(bmpOutputFile);

  return 0;
}
