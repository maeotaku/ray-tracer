#ifndef VECTOR
#define	VECTOR

struct Vector
{
	long double x,y,z;
};


struct Vector *newVector(long double x, long double y, long double z)
{
	struct Vector *aux = (struct Vector *) malloc(sizeof(struct Vector));
	aux-> x = x;
	aux-> y = y;
	aux-> z = z;
	return aux;
}

struct Vector *invertVector(struct Vector *v)
{
	return newVector(v->x * -1, v->y * -1, v->z *-1);
}

struct Vector *copyVector(struct Vector *v)
{
	return newVector(v->x, v->y, v->z);
}

void freeVector(struct Vector *v)
{
	if (v!=NULL)
		free(v);
}

/*
struct Vector *multiplyVector(struct Vector *v, long double k)
{
	return newVector(v->x * k, v->y * k, v->z * k);
}

struct Vector *takeAwayVector(struct Vector *l, struct Vector *r)
{
	return newVector(l->x - r->x,l->y - r->y, l->z - r->z);
}
*/


void print_vector(struct Vector *vec)
{
	if (vec != NULL)
	{
		printf("Vector: x=%Lf y=%Lf z=%Lf\n", vec->x, vec->y, vec->z);
	}
}

struct Vector *getCrossProduct(struct Vector *v, struct Vector *w)
{
	return newVector(v->y*w->z-w->y*v->z, v->z*w->x - v->x*w->z, v->x*w->y - w->x*v->y);
}

long double getDotProduct(struct Vector *l, struct Vector *r)
{
	return (l->x*r->x) + (l->y*r->y) +  (l->z*r->z);
}

long double getNorm(struct Vector *vector)
{
	return sqrt(powl(vector->x,2.0)+powl(vector->y,2.0)+powl(vector->z,2.0));
}


struct Vector *normalize(struct Vector *vector)
{
	long double norm = getNorm(vector);
	if (norm == 0.0)
		return vector;	
	return newVector(vector->x/norm, vector->y/norm, vector->z/norm);
}

/*
void normalize(struct Vector *vector)
{
	long double norm = getNorm(vector);
	if (norm == 0.0)
		return;// vector;	
	//return newVector(vector->x/norm, vector->y/norm, vector->z/norm);
	vector->x = vector->x / norm;
	vector->y = vector->y / norm;
	vector->z = vector->z / norm;
}
*/

#endif
