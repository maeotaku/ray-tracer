//sudo apt-get install libpng-dev
//-lpng

#ifndef AVS_H
#define AVS_H

#include "stdio.h"
#include "stdlib.h"

#define SWAP(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX(x) (*(unsigned *)&(x) = \
         SWAP(*(unsigned *)&(x)))


struct Texture *readAVS(char* filename)
{
	int width, height; 
	int i,j;
	int a,r,g,b;
	FILE *fptr;
	RGBA **image;

	if ((fptr = fopen(filename,"r")) == NULL) {
		exit(-1);
	}

	fread(&width,sizeof(int),1,fptr);
	width = FIX(width);
	//printf("Width: %d\n",width);
	fread(&height,sizeof(int),1,fptr);
	height = FIX(height);
	//printf("Height: %d\n",height);

	//struct RGBA *pic[height][width];
	image = initRGBA(width, height);
	//struct RGBA *pointer = pic;	

	/* Read the ARGB for each pixel */
	for (j=0;j<height;j++) {
		for (i=0;i<width;i++) {

			image[i][j].a = fgetc(fptr);
			image[i][j].r = fgetc(fptr);
			image[i][j].g = fgetc(fptr);
			image[i][j].b = fgetc(fptr);
			/*
			if (image[i][j].a == EOF || image[i][j].g == EOF || image[i][j].r == EOF || image[i][j].b == EOF) {
				fprintf(stderr,"Unexpected end of file\n");
				exit(-1);
			}
			*/
			//printf("%d %d %d %d\n", image[i][j].a, image[i][j].r, image[i][j].g, image[i][j].b );
		}
	}

	fclose(fptr);
	return newTexture(image, width, height);
}

void writeTexture(char* filename, struct Texture *t)
{
	int lenght = t->w * t->h;
	char buffer[lenght];
	FILE *fd = fopen(filename, "w");
	fwrite(&t->w, sizeof(int), 1, fd);
	fwrite(&t->h, sizeof(int), 1, fd);
	int i,j;
	for (i = 0; i < t->h; i++)
		for (j = 0; j < t->w; j++)
		{
			int aux = doubleToInt(t->colors[i][j]->a);
			fwrite(&aux, sizeof(int), 1, fd);
			aux = doubleToInt(t->colors[i][j]->r);
			fwrite(&aux, sizeof(int), 1, fd);
			aux = doubleToInt(t->colors[i][j]->g);
			fwrite(&aux, sizeof(int), 1, fd);
			aux = doubleToInt(t->colors[i][j]->b);
			fwrite(&aux, sizeof(int), 1, fd);
		}
	fclose(fd);
}


/*
void writeAVS(char* filename, int width, int height, RGBA **buffer)
{
   int i,j;
   int a,r,g,b;
   FILE *fptr;
/*
   int width,height;

   if ((fptr = fopen(filename,"r")) == NULL) 
   {
      fprintf(stderr,"Failed to open input file \"%s\"\n",filename);
      exit(-1);
   }
*/
   //fread(&width,sizeof(int),1,fptr);
//   width = FIX(width);
   //fprintf(stderr,"Width: %d\n",width);
   //fread(&height,sizeof(int),1,fptr);
  // height = FIX(height);
   //fprintf(stderr,"Height: %d\n",height);

   /* Read the ARGB for each pixel */
/*
   for (j=0;j<height;j++) {
      for (i=0;i<width;i++) {

         /* Read the current pixel */
/*
         a = image[i][j].a;
         r = image[i][j].r;
         g = image[i][j].g;
         b = image[i][j].b;
         //if (a == EOF || g == EOF || r == EOF || b == EOF) {
         //   fprintf(stderr,"Unexpected end of file\n");
         //   exit(-1);
         //}
         putchar(r);
         putchar(g);
         putchar(b);
      }
   }
   fclose(fptr);
}
*/

#endif
