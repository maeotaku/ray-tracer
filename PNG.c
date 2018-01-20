#define PNG_DEBUG 3
#include <png.h>

struct Texture
{
	struct Color ***colors;
	int w, h;	
	png_structp png_ptr;
	png_byte color_type;
	png_byte bit_depth;
	png_infop info_ptr;
	png_bytep *row_pointers;
};

/*
double colorIntToDouble(int n)
{
	return (double)n / 255.0;
}

char doubleToInt(double n)
{
	return (char)n * 255;
}
*/

struct Texture *newTexture(int w, int h, png_structp png_ptr, png_bytep *row_pointers, png_byte color_type,	png_byte bit_depth)
{
    	struct Texture *aux = (struct Texture *) malloc(sizeof (struct Texture));
	aux->w = w;
	aux->h = h;
	aux->row_pointers = row_pointers;
	aux->color_type = color_type;
	aux->bit_depth = bit_depth;
	aux->png_ptr = png_ptr;
	return aux;
}

struct Texture *read_png_file(char* file_name)
{
	png_bytep * row_pointers;
	png_structp png_ptr;
	int number_of_passes;
	png_byte color_type;
	png_byte bit_depth;
	png_infop info_ptr;
	int width, height;

        char header[8];    // 8 is the maximum size that can be checked

        // open file and test for it being a png 
        FILE *fp = fopen(file_name, "rb");
        if (!fp)
                return NULL;//abort_("[read_png_file] File %s could not be opened for reading", file_name);
        fread(header, 1, 8, fp);
        if (png_sig_cmp(header, 0, 8))
                return NULL;//abort_("[read_png_file] File %s is not recognized as a PNG file", file_name);


        // initialize stuff 
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
                return NULL;//abort_("[read_png_file] png_create_read_struct failed");

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
                return NULL; //abort_("[read_png_file] png_create_info_struct failed");

        if (setjmp(png_jmpbuf(png_ptr)))
                return NULL; //abort_("[read_png_file] Error during init_io");

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);

        number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);


        // read file 
        if (setjmp(png_jmpbuf(png_ptr)))
                return NULL; //abort_("[read_png_file] Error during read_image");

        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	int y;
        for (y=0; y<height; y++)
                row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

        png_read_image(png_ptr, row_pointers);

        fclose(fp);
	return newTexture(width, height, png_ptr, row_pointers, color_type, bit_depth);
}

/*
inline void setRGB(png_byte *ptr, float val)
{
	int v = (int)(val * 768);
	if (v < 0) v = 0;
	if (v > 768) v = 768;
	int offset = v % 256;

	if (v<256) {
		ptr[0] = 0; ptr[1] = 0; ptr[2] = offset;
	}
	else if (v<512) {
		ptr[0] = 0; ptr[1] = offset; ptr[2] = 255-offset;
	}
	else {
		ptr[0] = offset; ptr[1] = 255-offset; ptr[2] = 0;
	}
}

int writeFB(char* filename, int width, int height, struct Color ***colors)
{
    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte ** row_pointers = NULL;
    int status = -1;
    int pixel_size = 3;
    int depth = 8;
    
    fp = fopen (filename, "wb");
    if (! fp) {
        goto fopen_failed;
    }

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }
    
    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }
    

    if (setjmp (png_jmpbuf (png_ptr))) {
        goto png_failure;
    }
    
    png_set_IHDR (png_ptr,
                  info_ptr,
                  width,
                  height,
                  depth,
                  PNG_COLOR_TYPE_RGB,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);
    
    row_pointers = png_malloc (png_ptr, height * sizeof (png_byte *));
    for (y = 0; y < height; ++y) {
        png_byte *row = 
            png_malloc (png_ptr, sizeof (uint8_t) * width * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < width; ++x) {
            //pixel_t * pixel = pixel_at (bitmap, x, y);
            //pixel_t * pixel = pixel_at (bitmap, x, y);
	*row++ = (uint8_t)(colors[x][y]->r * 255.0); 
	*row++ = (uint8_t)(colors[x][y]->g * 255.0); 
	*row++ = (uint8_t)(colors[x][y]->b * 255.0); 
}
    }
    

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

   
    status = 0;
    
    for (y = 0; y < height; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);
    
 png_failure:
 png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);
 png_create_write_struct_failed:
    fclose (fp);
 fopen_failed:
    return status;
}
*/

			
/*
int writeImage(char* filename, struct Texture *t)
{

	int width = t->w;
	int height = t->h;
	float *buffer = t->pixels;
	int code = 0;
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep row;
	
	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 1;
		goto finalise;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	char* title = "Scene";
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep) malloc(3 * width * sizeof(png_byte));

	// Write image data
	int x, y;
	png_byte **row_pointers = png_malloc (png_ptr, height * sizeof (png_byte *));
	for (y = 0; y < height; ++y) 
	{
		png_byte *row = png_malloc (png_ptr, sizeof (uint8_t) * width * 3);
		row_pointers[y] = row;
		for (x = 0; x < width; ++x) 
		{
			//pixel_t * pixel = pixel_at (bitmap, x, y);
			*row++ = buffer[y*width + x]; 
			*row++ = buffer[y*width + x +1]; 
			*row++ = buffer[y*width + x + 2]; 
		}
	}
	png_set_rows (png_ptr, info_ptr, row_pointers);
	// End write
	png_write_end(png_ptr, NULL);

	finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);

	return code;
}
*/


float *createMandelbrotImage(int width, int height, float xS, float yS, float rad, int maxIteration)
{
	float *buffer = (float *) malloc(width * height * sizeof(float));
	if (buffer == NULL) {
		fprintf(stderr, "Could not create image buffer\n");
		return NULL;
	}

	// Create Mandelbrot set image

	int xPos, yPos;
	float minMu = maxIteration;
	float maxMu = 0;

	for (yPos=0 ; yPos<height ; yPos++)
	{
		float yP = (yS-rad) + (2.0f*rad/height)*yPos;

		for (xPos=0 ; xPos<width ; xPos++)
		{
			float xP = (xS-rad) + (2.0f*rad/width)*xPos;

			int iteration = 0;
			float x = 0;
			float y = 0;

			while (x*x + y+y <= 4 && iteration < maxIteration)
			{
				float tmp = x*x - y*y + xP;
				y = 2*x*y + yP;
				x = tmp;
				iteration++;
			}

			if (iteration < maxIteration) {
				float modZ = sqrt(x*x + y*y);
				float mu = iteration - (log(log(modZ))) / log(2);
				if (mu > maxMu) maxMu = mu;
				if (mu < minMu) minMu = mu;
				buffer[yPos * width + xPos] = mu;
			}
			else {
				buffer[yPos * width + xPos] = 0;
			}
		}
	}

	// Scale buffer values between 0 and 1
	int count = width * height;
	while (count) {
		count --;
		buffer[count] = (buffer[count] - minMu) / (maxMu - minMu);
	}

	return buffer;
}



