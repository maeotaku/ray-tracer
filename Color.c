
#ifndef COLOR_H
#define	COLOR_H


struct Color
{
	long double r,g,b,a;
};


struct Color *newColor(long double r, long double g, long double b)
{
	struct Color *aux = (struct Color *) malloc(sizeof(struct Color));
	aux-> r = r;
	aux-> g = g;
	aux-> b = b;
	return aux;
}

struct Color *newColorWithAlpha(long double r, long double g, long double b, long double a)
{
	struct Color *aux = (struct Color *) malloc(sizeof(struct Color));
	aux-> r = r;
	aux-> g = g;
	aux-> b = b;
	aux-> a = a;
	return aux;
}

void freeColor(struct Color *c)
{
	if (c!=NULL)
		free(c);
}

long double colorSim(struct Color *c1, struct Color *c2)
{
	return sqrt(powl(c2->r - c1->r, 2.0) + powl(c2->g - c1->g,2.0) + powl(c2->b - c1->b, 2.0));
}


void print_color(struct Color *c)
{
	if (c!=NULL)
		printf("Color: (%Lf, %Lf, %Lf, %Lf)\n", c->r, c->g, c->b, c->a);
}

struct Color ***initColorMatrix(int w, int h)
{
	struct Color ***colors = (struct Color ***)malloc(h * sizeof(struct Color **));
	int i;
	for (i = 0; i < h; i++) 
	{
		colors[i] = (struct Color **)malloc(w * sizeof(struct Color*));
	}
	return colors;
}


#endif
