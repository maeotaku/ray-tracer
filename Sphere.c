#ifndef SPHERE_H
#define SPHERE_H

struct Sphere 
{
    struct Point *center;
    double radio;
};

struct Sphere *newSphere(struct Point *center, double radio) {
    struct Sphere *aux = (struct Sphere *) malloc(sizeof (struct Sphere));
    aux-> center = center;
    aux-> radio = radio;
    return aux;
}

void freeSphere(struct Sphere *s)
{
	if (s!=NULL)
	{
		free(s->center);
		free(s);
	}
}

struct Sphere *copySphere(struct Sphere *p)
{
	return newSphere(p->center, p->radio);
}

/*
void *moveSphere(void *sp, long double x, long double y, long double z)
{
	struct Sphere *sphere = (struct Sphere*)sp;
	sphere->center->x += x;
	sphere->center->y += y;
	sphere->center->z += z;
	return sphere;
}

void *scaleSphere(void *sp, long double x, long double y, long double z)
{
	struct Sphere *sphere = (struct Sphere*)sp;
	sphere->center->x *= x;
	sphere->center->y *= y;
	sphere->center->z *= z;
	return sphere;
}
*/

struct UVPoint *getSphereUV(void *sp, struct Intersection *i, struct Vector *N)
{
	//TODO
	return NULL;
}

void *transformSphere(void *sp, long double movex, long double movey, long double movez, long double scalex, long double scaley, long double scalez)
{
	struct Sphere *sphere = copySphere((struct Sphere*)sp);
	sphere->center->x = sphere->center->x*scalex+movex;
	sphere->center->y = sphere->center->y*scaley+movey;
	sphere->center->z = sphere->center->y*scalez+movez;
	return sphere;
}


struct Vector* getSphereVectorN(struct Point *i, void *sp)
{
	struct Sphere* s = (struct Sphere*)sp;
	return newVector((i->x-s->center->x)/s->radio, (i->y-s->center->y)/s->radio, (i->z-s->center->z)/s->radio);
}

struct Intersection* sphereIntersection(struct Point *e, struct Vector *d, struct Object* o, void *sp) {
	struct Sphere* s = (struct Sphere*)sp;
	long double a, b, c;
	a = 1;
	b = 2 * ((d->x * (e->x - s->center->x)) + (d->y * (e->y - s->center->y)) + (d->z * (e->z - s->center->z)));
	c = powl((e->x - s->center->x), 2.0) + powl((e->y - s->center->y), 2.0) + powl((e->z - s->center->z), 2.0) - powf(s->radio, 2.0);
	long double discriminant = powl(b, 2.0) - 4.0 * a*c;
	struct Intersection *res = NULL;
	if (discriminant < EPSILON) //sin soluciones
	{
		return NULL;
	}
	else if (discriminant > EPSILON) //2  soluciones
	{
		long double t1, t2;
		t1 = (-b + sqrt(discriminant)) / (2.0 * a);
		t2 = (-b - sqrt(discriminant)) / (2.0 * a);
		if (t1 < EPSILON && t2 < EPSILON) //b1 yb2 estan por detras del ojo
		{
			return NULL;
		}
		else 
		{
			if (t1 > EPSILON)
				res = addIntersection(res, cut(o, newIntersection(o, t1, NULL, NULL, newPoint(e->x+t1*d->x, e->y+t1*d->y, e->z+t1*d->z))));
			if (t2 > EPSILON)
				res = addIntersection(res, cut(o, newIntersection(o, t2, NULL, NULL, newPoint(e->x+t2*d->x, e->y+t2*d->y, e->z+t2*d->z))));
		}
	}
	return res;
}

void print_sphere(struct Sphere *sphere) {
    printf("Sphere xc=%Lf yc=%Lf zc=%Lf radio=%f\n", sphere->center->x, sphere->center->y, sphere->center->z, sphere->radio);
}


#endif
