#ifndef DISK_H
#define DISK_H

struct Disk
{
	struct Point *center;
	struct Plane *plane;
	double radio1;
	double radio2;
};

struct Disk *newDisk(struct Point *center, struct Plane *plane, double radio1, double radio2) 
{
    	struct Disk *aux = (struct Disk *) malloc(sizeof (struct Disk));
	aux->center = center;
	aux->plane = plane;
	aux->radio1 = radio1;
	aux->radio2 = radio2;
	return aux;
}


struct Disk *copyDisk(struct Disk *p)
{
	struct Disk *newp = newDisk(p->center, p->plane, p->radio1, p->radio2 );
	return newp;
}

struct UVPoint *getDiskUV(void *sp, struct Intersection *i, struct Vector *N)
{
	//TODO
	return NULL;
}

void *transformDisk (void *sp, long double movex, long double movey, long double movez, long double scalex, long double scaley, long double scalez)
{
	/*
	struct Disk *polygon = copyPolygon((struct Polygon*)sp);
	int i;
	for(i=0; i<polygon->num_points; i++)
		polygon->points[i] = newPoint(polygon->points[i]->x*scalex+movex, polygon->points[i]->y*scaley+movey, polygon->points[i]->z*scalez+movez);
	initPolygon(polygon);
	return polygon;
	*/
	//TODO
	return NULL;
}

struct Vector* getDiskVectorN(struct Point *i, void *sp)
{
	struct Disk* p = (struct Disk*)sp;
	return copyVector(p->plane->ABC);
}

struct Intersection* diskIntersection(struct Point *e, struct Vector *d, struct Object* o, void *sp) {
	struct Disk* p = (struct Disk*)sp;
	long double dot_product = getDotProduct(p->plane->ABC, d);
	if (dot_product == 0)
		return NULL;
	else
	{
		long double t = (-1*(p->plane->ABC->x*e->x + p->plane->ABC->y*e->y + p->plane->ABC->z*e->z + p->plane->D)) / dot_product;
		if (t<0)
			return NULL;
		else //si pega con el plano
		{
			struct Point *i = newPoint(e->x+t*d->x, e->y+t*d->y, e->z+t*d->z);
			long double distance = powl(i->x - p->center->x, 2.0) + powl(i->y - p->center->y, 2.0) + powl(i->z - p->center->z, 2.0);
			if (powl(p->radio1,2.0) <= distance && distance <= powl(p->radio2,2.0))
			{
				//free(i);	
				return cut(o, newIntersection(o, t, NULL, NULL, i));
			}
			free(i);
			return NULL;
		}
	}
}

#endif
