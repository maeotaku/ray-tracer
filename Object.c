#ifndef OBJECT_H
#define OBJECT_H

#include "Intersection.c"

struct Object {
	struct Texture *texture;
	int type;
	double kd; //brillo asociado del objeto
	double ka; //iluminacion ambiente del objeto
	double ks; //factor de relefixion difusa
	double kn; //indice de area de reflexion especular
	double o1; //object color
	double o2; //color reflejo
	double o3; //color transparency
	struct Color* color;
	void* shape;
	struct Intersection* (*intersectionFunction)(struct Point*, struct Vector*, struct Object*, void*);
	struct Vector* (*getVectorN)(struct Point *, void *);
	//void* (*move)(void *sp, long double x, long double y, long double z);
	//void* (*scale)(void *sp, long double x, long double y, long double z);
	void* (*transform)(void *sp, long double movex, long double movey, long double movez, long double scalex, long double scaley, long double scalez);
	struct UVPoint* (*getUV) (void *, struct Intersection *, struct Vector *);
	int num_planes;
	struct Plane *planes[MAX_PLANES_PER_OBJECT];
};

void freeObject(struct Object *o)
{
	if (o!=NULL)
	{
		printf("free object\n");
		free(o->color);
		//printf("1\n");
		//free(o->intersectionFunction);
		//printf("2\n");
		//free(o->getVectorN);
		//printf("3\n");
		//free(o->transform);
		//printf("4\n");
		int i;
		for (i=0; i<MAX_PLANES_PER_OBJECT; i++)
			freePlane(o->planes[i]);
		free(o);
	}
}

struct Intersection *cut(struct Object *o, struct Intersection *i)
{
	int k=0;
	for(k=0; k<o->num_planes;k++)
	{
		
		long double res = o->planes[k]->ABC->x*i->i->x + o->planes[k]->ABC->y*i->i->y + o->planes[k]->ABC->z*i->i->z + o->planes[k]->D;
		if (res > 0)
		{
			freeIntersection(i);
			return NULL;
		}
	}
	return i;
}

//struct Object *newObject(int t, double kd, double ka, double ks, double kn, struct Color* c, void* s,  struct Intersection* (*interf)(struct Point*, struct Vector*, struct Object*, void*), struct Vector* (*getN)(struct Point *, void *), void *(*move)(void *, long double, long double, long double), void *(*scale)(void *, long double, long double, long double))
struct Object *newObject(	int t, double kd, double ka, double ks, double kn, double o1, double o2, double o3, 
				struct Color* c, struct Texture *texture, void* s,  
				struct Intersection* (*interf)(struct Point*, struct Vector*, struct Object*, void*), 
				struct Vector* (*getN)(struct Point *, void *), 
				void *(*transform)(void *, long double, long double, long double, long double, long double, long double),
				struct UVPoint* (*getUV) (void *, struct Intersection *, struct Vector *))
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
	aux->getUV = getUV;
	aux-> kd = kd;
	aux-> ka = ka;
	aux->o1 = o1;
	aux->o2 = o2;
	aux->o3 = o3;
	aux->texture = texture;
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
	aux->texture = o->texture;
	aux->getUV = o->getUV;
	aux-> kd = o->kd;
	aux-> ka = o->ka;
	aux -> ks =  o->ks; //indice refelxion especular
	aux -> kn = o->kn; //indice de area de reflexion especular
	aux->o1 = o->o1;
	aux->o2 = o->o2;
	aux->o3 = o->o3;
	aux-> num_planes = o->num_planes;
	int i;
	for(i=0; i<aux->num_planes; i++)
		aux->planes[i] = o->planes[i];
	return aux;
}


void print_object(struct Object *o) {
    printf("Object: %i, (%Lf, %Lf, %Lf), kd=%f ka=%f ks=%f %p\n", o->type, o->color->r, o->color->g, o->color->b, o->kd , o->ka, o->ks, o->shape);
}

#endif
