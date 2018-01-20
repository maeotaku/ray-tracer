#ifndef TEXTURE_H
#define TEXTURE_H

struct Texture
{
	char* name;
	struct Color ***colors;
	int w, h;	
};

struct Texture *newTexture(char* name, int w, int h, struct Color ***colors)
{
    	struct Texture *aux = (struct Texture *) malloc(sizeof (struct Texture));
	aux->name=name;
	aux->w = w;
	aux->h = h;
	aux->colors = colors;
	return aux;
}

print_texture(struct Texture *t)
{
	int i,j;
	printf("Name: %s Witdh: %d Height: %d \n",t->name, t->w, t->h);
	for (i=0; i<t->w; i++)
		for (j=0; j<t->h; j++)
			printf("%Lf %Lf %Lf\n", t->colors[i][j]->r, t->colors[i][j]->g, t->colors[i][j]->b);
}

#endif
