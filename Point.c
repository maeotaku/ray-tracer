#ifndef POINT
#define	POINT

struct Point
{
	long double x,y,z;
};


struct Point *newPoint(long double x, long double y, long double z)
{
	struct Point *aux = (struct Point *) malloc(sizeof(struct Point));
	aux-> x = x;
	aux-> y = y;
	aux-> z = z;
	return aux;
}


struct Point *movePoint(struct Point *p, long double x, long double y, long double z)
{
	return newPoint(p->x+x, p->y+y, p->z+z);
}

struct Point *scalePoint(struct Point *p, long double x, long double y, long double z)
{
	return newPoint(p->x*x, p->y*y, p->z*z);
}


void print_point(struct Point *point, char* point_name)
{
	if (point != NULL)
	{
		printf("%s: x=%Lf y=%Lf z=%Lf\n", point_name, point->x, point->y, point->z);
	}
}


void freePoint(struct Point *p)
{
	if (p!=NULL)
		free(p);
}
#endif
