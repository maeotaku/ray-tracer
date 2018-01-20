#ifndef BMP_H
#define BMP_H

//#include "Color.c"
#include "Texture.c"

//static const int DEBUG = 1;

struct bmpHeaderStruct {
		//if this struc is changed, it will change the
		//pointer locations and likely screw everything up
		//uint16_t magicNum;
		uint32_t bmpSize;
		uint32_t unused;
		uint32_t arrayOffset;
		uint32_t headerLen;
		uint32_t width;          //12h
		uint32_t height;         //16h
		uint16_t colorPlanes;
		uint16_t bpp;            //1Ah
	};

//read bitmap pixel data
	struct bmpDataStruct {
		uint8_t blue;
		uint8_t green;
		uint8_t red;
	};

void framebuffer_export_bmp(int width, int height, struct Color ***colors,const char* path )
{
	FILE *f;
	size_t w = width;
	size_t h = height;
	unsigned int x, y, r, g, b, i, j;
	unsigned char *img = NULL;
	int filesize = 54 + 3 * w * h; //w is your image width, h is image height, both int
	img = (unsigned char *) malloc(3 * w * h);
	memset(img, 0, sizeof(img));

	for (i = 0; i < w; i++)
	{
		for (j = 0; j < h; j++)
		{
			x = i;
			y = (h - 1) - j;
			//r = //fb->buffer[j][i].r;
			//g = //fb->buffer[j][i].g;
			//b = //fb->buffer[j][i].b;
			r = (unsigned char)(colors[y][x]->r * 255);
			g = (unsigned char)(colors[y][x]->g * 255);
			b = (unsigned char)(colors[y][x]->b * 255);
			img[(x + y * w) * 3 + 2] = (unsigned char) (r);
			img[(x + y * w) * 3 + 1] = (unsigned char) (g);
			img[(x + y * w) * 3 + 0] = (unsigned char) (b);
		}
	}

	unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };
	unsigned char bmppad[3] = { 0, 0, 0 };

	bmpfileheader[2] = (unsigned char) (filesize);
	bmpfileheader[3] = (unsigned char) (filesize >> 8);
	bmpfileheader[4] = (unsigned char) (filesize >> 16);
	bmpfileheader[5] = (unsigned char) (filesize >> 24);

	bmpinfoheader[4] = (unsigned char) (w);
	bmpinfoheader[5] = (unsigned char) (w >> 8);
	bmpinfoheader[6] = (unsigned char) (w >> 16);
	bmpinfoheader[7] = (unsigned char) (w >> 24);
	bmpinfoheader[8] = (unsigned char) (h);
	bmpinfoheader[9] = (unsigned char) (h >> 8);
	bmpinfoheader[10] = (unsigned char) (h >> 16);
	bmpinfoheader[11] = (unsigned char) (h >> 24);

	f = fopen(path, "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	for (i = 0; i < h; i++)
	{
		fwrite(img + (w * (h - i - 1) * 3), 3, w, f);
		fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
	}
	fclose(f);
}



struct Texture *readTextureFromFile(char*name, char* filename) {
	FILE * fileHandle = fopen(filename, "rb");
	struct bmpHeaderStruct bmpHeader;

	//read header
	fseek(fileHandle, 2 , SEEK_SET); //skips the magic num
	fread(&bmpHeader, 31, 1, fileHandle);

	if (1) {
		printf("[function main] raw header reading without endian checking\n");
		printf("item  : hex, dec\n");
		//printf("%x\n", bmpHeader.magicNum);
		//printf("bsize : %x %d\n", bmpHeader.bmpSize, bmpHeader.bmpSize);

		printf("offset: %x, %d\n", bmpHeader.arrayOffset, bmpHeader.arrayOffset);
		printf("hlen  : %x, %d\n", bmpHeader.headerLen, bmpHeader.headerLen);
		printf("width : %x, %d\n", bmpHeader.width, bmpHeader.width);
		printf("height: %x, %d\n", bmpHeader.height, bmpHeader.height);
		printf("cplane: %x, %d\n", bmpHeader.colorPlanes, bmpHeader.colorPlanes);
		printf("bpp   : %x, %d\n", bmpHeader.bpp, bmpHeader.bpp);
	}

	struct bmpDataStruct bmpData;

	//5 dimension pixelArray: height, width, blue, green red
	//unsigned int pixelArray[bmpHeader.width][bmpHeader.height][2];
	unsigned int width = 0;
	unsigned int height = (bmpHeader.height - 1); //height = bmpHeader.height;
	struct Color ***colors = initColorMatrix(bmpHeader.height, bmpHeader.width);
	fseek(fileHandle, bmpHeader.arrayOffset, SEEK_SET); //skips the magic num
	int isData = 0; //always 1, is zero at EOF
	while(isData = fread(&bmpData, 1, 3, fileHandle) ) {
		//printf("%d %d\n", height, width);
		colors[ width ][(bmpHeader.height - 1) - height] = newColor((long double)bmpData.red / 255.0, (long double)bmpData.green / 255.0, (long double)bmpData.blue / 255.0);
		//printf("%d %d\n", height, width);
		//pixelArray[width][height][0] = bmpData.blue;
		//pixelArray[width][height][1] = bmpData.green;
		//pixelArray[width][height][2] = bmpData.red;
		//if (DEBUG) printf("%d %d %d %d %d\n", width, height, pixelArray[width][height][0], pixelArray[width][height][1], pixelArray[width][height][2]);
		//if (DEBUG) printf("%d %d %d %d %d \n", width, height, bmpData.blue, bmpData.green, bmpData.red);
		//00 00 FF = Red     FF FF FF = White
		//00 FF 00 = Green   00 00 00 = Black
		//FF 00 00 = Blue

		//increment counter
		width += 1;
		if(width  >= bmpHeader.width ) {
			width = 0;
			height -= 1;
		}

	}

	fclose(fileHandle);

	return newTexture(name, bmpHeader.width, bmpHeader.height, colors);
	//return NULL;
}

/*
uint16_t endian_16(uint16_t li) {
	//16 bit or 2 byte endian swap
	int swapped = ((li>>8)&0x00ff) | ((li<<8)&0xff00);

	if (DEBUG) printf("[function endian_16] given %X returning %X\n", li, swapped); //debug
	return swapped;
}

uint32_t endian_32(uint32_t li) {

	//32 bit or 4 byte endian swap
	int swapped = ((li>>24)&0xff) | // move byte 3 to byte 0
	          	  ((li<<8)&0xff0000) | // move byte 1 to byte 2
	          	  ((li>>8)&0xff00) | // move byte 2 to byte 1
	          	  ((li<<24)&0xff000000); // byte 0 to byte 3

	if (DEBUG) printf("[function endian_32] given %X returning %X\n", li, swapped); //debug
	return swapped;
}
*/

#endif
