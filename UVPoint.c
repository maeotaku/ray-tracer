#ifndef UVPOINT
#define	UVPOINT

struct UVPoint
{
	long double u;
	long double v;
};

struct UVPoint *newUVPoint(long double u, long double v)
{
	struct UVPoint *aux = (struct UVPoint *) malloc(sizeof(struct UVPoint));																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																							
	aux-> u = u;																																																																																																																						
	aux-> v = v;
	return aux;
}

void freeUVPoint(struct UVPoint *uv)
{
	if (uv!=NULL)
		free(uv);
}

struct UVPoint *moveUVPoint(struct UVPoint *p, long double u, long double v)
{
	return newUVPoint(p->u+u, p->v+v);
}

#endif
