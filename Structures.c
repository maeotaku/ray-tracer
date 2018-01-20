#ifndef STRUCTURES_C
#define STRUCTURES_C

#include "constants.h"
#include "Structures.h"
#include "Color.c"
#include "UVPoint.c"
#include "Point.c"
#include "Vector.c"
#include "Window.c"
#include "Light.c"
#include "Intersection.c"
#include "Plane.c"
#include "Object.c"
#include "Common.c"
#include "Sphere.c"
#include "Polygon.c"
#include "Cylinder.c"
#include "Cone.c"
#include "Disk.c"
#include "BMP.c"



struct Group
{
	struct Object *objects[MAX_GROUP_OBJECTS];
	int num_objects;
	char *name;
};

struct Group *newGroup(char *name) {
    	struct Group *aux = (struct Group *) malloc(sizeof (struct Group));
	aux->num_objects = 0;
	aux->name = name;
	return aux;
}

void freeGroup(struct Group *g)
{
	if (g!=NULL)
	{
		int i;
		for (i=0; i<MAX_GROUP_OBJECTS; i++)
			free(g->objects[i]);
		free(g);
	}
}


/*
struct Object {
	int type;
	double kd; //brillo asociado del objeto
	double ka; //iluminacion ambiente del objeto
	double ks; //factor de relefixion difusa
	double kn; //indice de area de reflexion especular
	struct Color* color;
	void* shape;
	struct Intersection* (*intersectionFunction)(struct Point*, struct Vector*, struct Object*, void*);
	struct Vector* (*getVectorN)(struct Point *, void *);
	//void* (*move)(void *sp, long double x, long double y, long double z);
	//void* (*scale)(void *sp, long double x, long double y, long double z);
	void* (*transform)(void *sp, long double movex, long double movey, long double movez, long double scalex, long double scaley, long double scalez);
	int num_planes;
	struct Plane *planes[MAX_PLANES_PER_OBJECT];
};

struct Intersection *cut(struct Object *o, struct Intersection *i)
{
	int k=0;
	for(k=0; k<o->num_planes;k++)
	{
		long double res = o->planes[k]->ABC->x*i->i->x + o->planes[k]->ABC->y*i->i->y + o->planes[k]->ABC->z*i->i->z + o->planes[k]->D;
		if (res > 0)
			return NULL;
	}
	return i;
}

//struct Object *newObject(int t, double kd, double ka, double ks, double kn, struct Color* c, void* s,  struct Intersection* (*interf)(struct Point*, struct Vector*, struct Object*, void*), struct Vector* (*getN)(struct Point *, void *), void *(*move)(void *, long double, long double, long double), void *(*scale)(void *, long double, long double, long double))
struct Object *newObject(int t, double kd, double ka, double ks, double kn, struct Color* c, void* s,  struct Intersection* (*interf)(struct Point*, struct Vector*, struct Object*, void*), struct Vector* (*getN)(struct Point *, void *), void *(*transform)(void *, long double, long double, long double, long double, long double, long double))
{
	struct Object *aux = (struct Object *) malloc(sizeof(struct Object));
	aux-> type = t;
	aux-> color = c;
        aux-> shape = s;
	aux-> intersectionFunction = interf;
	aux-> getVectorN = getN;
	//aux-> move = move;
	//aux-> scale = scale;
	aux->transform = transform;
	aux-> kd = kd;
	aux-> ka = ka;
	aux -> ks =  ks; //indice refelxion especular
	aux -> kn = kn; //indice de area de reflexion especular
	aux-> num_planes = 0;
	return aux;
}

struct Object *copyObject(struct Object *o)
{
	struct Object *aux = (struct Object *) malloc(sizeof(struct Object));
	aux-> type = o->type;
	aux-> color = o->color;
        aux-> shape = o->shape;
	aux-> intersectionFunction = o->intersectionFunction;
	aux-> getVectorN = o->getVectorN;
	//aux-> move = o->move;
	//aux-> scale = o->scale;
	aux->transform = o->transform;
	aux-> kd = o->kd;
	aux-> ka = o->ka;
	aux -> ks =  o->ks; //indice refelxion especular
	aux -> kn = o->kn; //indice de area de reflexion especular
	aux-> num_planes = o->num_planes;
	int i;
	for(i=0; i<aux->num_planes; i++)
		aux->planes[i] = o->planes[i];
	return aux;
}





void print_object(struct Object *o) {
    printf("Object: %i, (%f, %f, %f), kd=%f ka=%f ks=%f %p\n", o->type, o->color->r, o->color->g, o->color->b, o->kd , o->ka, o->ks, o->shape);
}

*/

#endif
