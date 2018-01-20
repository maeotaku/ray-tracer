#ifndef POLYGON_H
#define POLYGON_H

#include <math.h>

struct Polygon
{
	struct Plane *plane;
	int whichEliminate;
	struct Point *points[MAX_POINTS];
	struct UVPoint *uvpoints[MAX_POINTS];
	int num_points;
	//calculations for texture
	struct Vector *U;
	struct Vector *V;
	struct Point *x0;
	long double height, width;
	
	
};

struct Polygon *newPolygon() {
    	struct Polygon *aux = (struct Polygon *) malloc(sizeof (struct Polygon));
	aux->num_points = 0;
	return aux;
}


struct Polygon *copyPolygon(struct Polygon *p)
{
	struct Polygon *newp = newPolygon();
	newp->plane = p->plane;
	newp->whichEliminate = p->whichEliminate;
	newp->num_points = p->num_points;
	newp->x0 = p->x0;
	int i;
	for(i=0; i<newp->num_points; i++)
	{
		newp->points[i] = p->points[i];
	}
	return newp;
}


struct Polygon *preparePolygonTextureRelated(struct Polygon *polygon)
{
	int k;
	struct Point *x0, *x1, *x2, *x3;
	if (polygon->num_points > 1)
	{
		x0 = newPoint( polygon->points[0]->x, polygon->points[0]->y, polygon->points[0]->z);
		x2 = newPoint( polygon->points[0]->x, polygon->points[0]->y, polygon->points[0]->z);
		for (k=1; k<polygon->num_points; k++)
		{
			if (polygon->points[k]->x < x0->x)
				x0->x = polygon->points[k]->x;
			if (polygon->points[k]->x > x2->x)
				x2->x = polygon->points[k]->x;

			if (polygon->points[k]->y < x0->y)
				x0->y = polygon->points[k]->y;
			if (polygon->points[k]->y > x2->y)
				x2->y = polygon->points[k]->y;

			if (polygon->points[k]->z < x0->z)
				x0->z = polygon->points[k]->z;
			if (polygon->points[k]->z > x2->z)
				x2->z = polygon->points[k]->z;
		}
	}
	x1 = newPoint(x2->x, x0->y, x0->z);
	x3 = newPoint(x0->x, x2->y, x0->z);
	polygon->x0 = x0;
	polygon->height = (long double)sqrt(powl(x3->x - x0->x,2.0) + powl(x3->y - x0->y,2.0) + powl(x3->z - x0->z,2.0));
	polygon->width = (long double)sqrt(powl(x1->x - x0->x,2.0) + powl(x1->y - x0->y,2.0) + powl(x1->z - x0->z,2.0));
	//printf("%Lf \n", polygon->height);//sqrt(262144.0));//powl(x3->x - x0->x,2.0) + powl(x3->y - x0->y,2.0) + powl(x3->z - x0->z,2.0));
	//print_point(x0, "x0");
	//print_point(x1, "x1");
	//print_point(x2, "x2");
	//print_point(x3, "x3");
	polygon->U = newVector((x1->x - x0->x) / polygon->width, (x1->y - x0->y) / polygon->width, (x1->z - x0->z) / polygon->width);
	//printf("%Lf %Lf \n", polygon->height, polygon->width);
	polygon->V = getCrossProduct(polygon->U, polygon->plane->ABC);
	//polygon->V = newVector((x1->x - x0->x) / polygon->height, (x1->y - x0->y) / polygon->height, (x1->z - x0->z) / polygon->height);
	//print_vector(polygon->U);
	//print_vector(polygon->V);
	freePoint(x1);
	freePoint(x2);
	freePoint(x3);
	return polygon;
}


struct Polygon *initPolygon(struct Polygon *polygon)
{
	polygon->plane = newPlane3Points(polygon->points[0], polygon->points[1], polygon->points[2], NULL);
	polygon = preparePolygonTextureRelated(polygon);
	if (abs(polygon->plane->ABC->x) > abs(polygon->plane->ABC->y) && abs(polygon->plane->ABC->x) > abs(polygon->plane->ABC->z))
	{
		polygon->whichEliminate = ELIMINATE_X;
		int k=0;
		for (k=0; k<polygon->num_points; k++)
			polygon->uvpoints[k] = newUVPoint(polygon->points[k]->y, polygon->points[k]->z);
	}
	else if (abs(polygon->plane->ABC->y) > abs(polygon->plane->ABC->x) && abs(polygon->plane->ABC->y) > abs(polygon->plane->ABC->z))
	{
		polygon->whichEliminate = ELIMINATE_Y;
		int k=0;
		for (k=0; k<polygon->num_points; k++)
			polygon->uvpoints[k] = newUVPoint(polygon->points[k]->x, polygon->points[k]->z);
	}
	else
	{
		polygon->whichEliminate = ELIMINATE_Z;
		int k=0;
		for (k=0; k<polygon->num_points; k++)
			polygon->uvpoints[k] = newUVPoint(polygon->points[k]->x, polygon->points[k]->y);
	}
	return polygon;
}


struct UVPoint *getPolygonUV(void *sp, struct Intersection *i, struct Vector *N)
{
	struct Polygon *polygon = (struct Polygon*)sp;
	struct Vector *aux = newVector(i->i->x - polygon->x0->x, i->i->y - polygon->x0->y, i->i->z - polygon->x0->z);
	long double u = abs(getDotProduct(polygon->U, aux)) / polygon->width;
	long double v = abs(getDotProduct(polygon->V, aux)) / polygon->height;
	freeVector(aux);
	//printf("%Lf %Lf\n", u, v);
	return newUVPoint(u,v);
}

void *transformPolygon (void *sp, long double movex, long double movey, long double movez, long double scalex, long double scaley, long double scalez)
{
	struct Polygon *polygon = copyPolygon((struct Polygon*)sp);
	int i;
	for(i=0; i<polygon->num_points; i++)
		polygon->points[i] = newPoint(polygon->points[i]->x*scalex+movex, polygon->points[i]->y*scaley+movey, polygon->points[i]->z*scalez+movez);
	polygon = initPolygon(polygon);
	for(i=0;i<polygon->num_points; i++)
		free(polygon->points[i]);
	return polygon;
}

struct Vector* getPolygonVectorN(struct Point *i, void *sp)
{
	struct Polygon* p = (struct Polygon*)sp;
	return copyVector(p->plane->ABC);
}

int trivialReject(long double u0, long double v0, long double u1, long double v1)
{
	if (u0 < 0 && u1 < 0)
		return TRUE;
	else if (v0 > 0 && v1 > 0)
		return TRUE;
	else if (v0 < 0 && v1 < 0)
		return TRUE;
	return FALSE;
}

int trivialAccept(long double u0, long double v0, long double u1, long double v1)
{
	if ((v0 > 0 && v1 < 0) && (u0 > 0 && u1 > 0))
		return TRUE;
	else if ((v0 < 0 && v1 > 0) && (u0 > 0 && u1 > 0))
		return TRUE;
	return FALSE;
}

int acceptComplicated(long double u0, long double v0, long double u1, long double v1)
{
	if ((u1-u0)==0)
		return FALSE;
	long double m = (v1-v0) / (u1-u0);
	if(m==0)	
		return FALSE;
	long double b = v0 - (u0 * m);
	long double x = -b / m;
	if (x >= 0)
		return TRUE;
	return FALSE;
}

struct Intersection* polygonIntersection(struct Point *e, struct Vector *d, struct Object* o, void *sp) {
	struct Polygon* p = (struct Polygon*)sp;
	long double dot_product = getDotProduct(p->plane->ABC, d);
	if (dot_product == 0)
		return NULL;
	else
	{
		long double t = (-1*(p->plane->ABC->x*e->x + p->plane->ABC->y*e->y + p->plane->ABC->z*e->z + p->plane->D)) / dot_product;
		if (t<=0.0)
			return NULL;
		else
		{
			
			struct Point *i = newPoint(e->x+t*d->x, e->y+t*d->y, e->z+t*d->z);
			long double u;
			long double v;
			switch(p->whichEliminate)
			{
				case ELIMINATE_X:
					u =i->y; v=i->z;	
					break;
				case ELIMINATE_Y:
					u = i->x; v=i->z;	
					break;
				case ELIMINATE_Z:
					u = i->x; v=i->y;	
					break;
			}
			int k=0;
			struct UVPoint *aux[p->num_points];
			for (k=0; k<p->num_points; k++)
				aux[k] = moveUVPoint(p->uvpoints[k],-1.0*u,-1.0*v);
			int k0 = 0;
			int k1 = 1;
			int contWalls = 0;
			for (k0=0; k0 < p->num_points-1; k0++)
			{
				if (trivialAccept(aux[k0]->u, aux[k0]->v, aux[k1]->u, aux[k1]->v))
					contWalls++;
				else if (!trivialReject(aux[k0]->u, aux[k0]->v, aux[k1]->u, aux[k1]->v))
					if (acceptComplicated(aux[k0]->u, aux[k0]->v, aux[k1]->u, aux[k1]->v))
						contWalls++;
				k1++;
			}
			if (trivialAccept(aux[p->num_points-1]->u, aux[p->num_points-1]->v, aux[0]->u, aux[0]->v))
				contWalls++;
			else if (!trivialReject(aux[p->num_points-1]->u, aux[p->num_points-1]->v, aux[0]->u, aux[0]->v))
				if (acceptComplicated(aux[p->num_points-1]->u, aux[p->num_points-1]->v, aux[0]->u, aux[0]->v))
					contWalls++;
			for (k=0; k<p->num_points; k++)
				free(aux[k]);
			if (contWalls%2!=0)
			{
				return newIntersection(o, t, NULL, NULL, i);
			}
			else 
			{
				freePoint(i);
				return NULL;
			}
			
		}
	}
}



#endif
