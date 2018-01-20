#ifndef COMMON
#define COMMON

struct Intersection *getInterBasedOnDiscriminant(long double a, long double b, long double c, struct Point *e, struct Vector *d, struct Object *o)
{
	long double discriminant = powl(b, 2.0) - 4.0 * a*c;
	if (discriminant < EPSILON) //sin soluciones
		return NULL;
	else 
	{
		if (discriminant >= EPSILON) //2  soluciones
		{
			long double t1, t2;
			t1 = (-b + sqrt(discriminant)) / (2.0 * a);
			t2 = (-b - sqrt(discriminant)) / (2.0 * a);
			if (t1 < EPSILON && t2 < EPSILON) //b1 yb2 estan por detras del ojo
				return NULL;
			else 
			{
				if (t1 >= t2)
					return newIntersection(o, t2, NULL, NULL, newPoint(e->x+t2*d->x, e->y+t2*d->y, e->z+t2*d->z));
				else
					return newIntersection(o, t1, NULL, NULL, newPoint(e->x+t1*d->x, e->y+t1*d->y, e->z+t1*d->z));
			}
		} 
	}
	return NULL;
}


#endif
