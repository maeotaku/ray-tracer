#ifndef PLANE
#define PLANE

struct Plane
{
	char* name;
	struct Vector *ABC;
	long double D;
};

struct Plane *newPlane3Points(struct Point *a, struct Point *b, struct Point *c, char* name) 
{
    	struct Plane *aux = (struct Plane *) malloc(sizeof (struct Plane));
	struct Vector *v = newVector(b->x - a->x, b->y - a->y, b->z - a->z);
	struct Vector *w = newVector(c->x - a->x, c->y - a->y, c->z - a->z);
	struct Vector *aux2 = getCrossProduct(v,w);
	aux->ABC = normalize(aux2);
	aux->D = (-1*aux->ABC->x*a->x) + (-1*aux->ABC->y*a->y) + (-1*aux->ABC->z*a->z);
	aux->name = name;
	free(v);
	free(w);
	free(aux2);
	//free(a);
	//free(b);
	//free(c);
	return aux;
}
/*
struct Plane *copyPlane(struct Plane *p)
{
	struct Plane *newp = newPlaneABCD(p->ABC, p->D);
}
*/

void freePlane(struct Plane *p)
{
	if (p!=NULL)
	{
		free(p->name);
		freeVector(p->ABC);
		free(p);
	}
}

struct Plane *newPlane1Point1Normal(struct Point *a, struct Vector *normal, char* name) 
{
    	struct Plane *aux = (struct Plane *) malloc(sizeof (struct Plane));
	aux->ABC = normalize(normal);
	aux->D = (-1*aux->ABC->x*a->x) + (-1*aux->ABC->y*a->y) + (-1*aux->ABC->z*a->z);
	aux->name  = name;
	//free(a);
	freeVector(normal);
	//free(name);
	return aux;
}



struct Plane *newPlaneABCD(struct Vector *ABC, long double D, char* name)
{
	struct Plane *aux = (struct Plane *) malloc(sizeof (struct Plane));
	aux->ABC = ABC;
	aux->D = D;
	aux->name=name;
	return aux;
}

#endif

