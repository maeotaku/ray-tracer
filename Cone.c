#ifndef CONE_H
#define CONE_H

struct Cone
{
	struct Point *center;
	struct Vector *Q; //direction vector on the axis
	long double d1, d2;
	long double distance, radio; //distance and radio/radio for proportion
};

struct Cone *newCone(struct Point *center, struct Vector *Q, long double d1, long double d2, long double distance, long double radio) 
{
    	struct Cone *aux = (struct Cone *) malloc(sizeof (struct Cone));
	aux->center = center;
	aux->Q = Q; 
	aux->d1 = d1;
	aux->d2 = d2;
	aux->distance = distance;
	aux->radio = radio;
	return aux;
}

struct Cone *copyCone(struct Cone *p)
{
	return newCone(p->center, p->Q, p->d1, p->d2, p->distance, p->radio);
}

struct UVPoint *getConeUV(void *sp, struct Intersection *i, struct Vector *N)
{
	//TODO
	return NULL;
}

void *transformCone(void *sp, long double movex, long double movey, long double movez, long double scalex, long double scaley, long double scalez)
{
	/*
	struct Cone *c = copyCone((struct Cone*)sp);
	c->center->x += movex;
	c->center->y += movey;
	c->center->z += movez;
	return c;
	*/
	return NULL;
}


// Función que calcula la distancia de la intersección con un cilindro
struct Intersection* coneIntersection(struct Point *e, struct Vector *d, struct Object* o, void *sp) 
{
	struct Cone* p = (struct Cone*)sp;
	long double A = p->Q->x * p->Q->y;
	long double B = p->Q->x * p->Q->z;
	long double C = p->Q->y * p->Q->z;

	long double a, b, c;
	long double D,E,F,G,H,I,J,K;

  	D = (powl(p->Q->x, 2.0) * d->x) + (A * d->y) + (B * d->z) - d->x;
	E = (powl(p->Q->y, 2.0) * d->y) + (A * d->x) + (C * d->z) - d->y;
	F = (powl(p->Q->z, 2.0) * d->z) + (B * d->x) + (C * d->y) - d->z;
  	G = e->x - p->center->x;
  	H = e->y - p->center->y;
  	I = e->z - p->center->z;
	J = powl((p->radio / p->distance),2.0);
  	K = (p->Q->x * d->x) + (p->Q->y * d->y) + (p->Q->z * d->z);

  	a = powl(D, 2.0) + powl(E, 2.0) + powl(F, 2.0);
	a -= J * powl(K,2.0);

  	b = 2 * ((p->center->x * D) + (p->center->y * E) + (p->center->z * F) - (e->x * D) - (e->y * E) - (e->z * F)
  			 + (G * ((powl(p->Q->x,2.0) * D) + (E * A) + (F * B)))
  			 + (H * ((D * A) + (powl(p->Q->y,2.0) * E) + (F * C)))
  			 + (I * ((D * B) + (E * C) + (powl(p->Q->z,2.0) * F))));
  	b -= 2 * J * ((p->Q->x * K * G) + (p->Q->y * K * H) + (p->Q->z * K * I));

  	c = powl(p->center->x,2.0) + powl(p->center->y,2.0) + powl(p->center->z,2.0) + powl(e->x,2.0) + powl(e->y,2.0) + powl(e->z, 2.0);
  	c += powl(G,2.0) * (powl(p->Q->x,4.0) + powl(A,2.0) + powl(B,2.0));
  	c += powl(H,2.0) * (powl(p->Q->y,4.0) + powl(A,2.0) + powl(C,2.0));
  	c += powl(I,2.0) * (powl(p->Q->z,4.0) + powl(B,2.0) + powl(C,2.0));
  	c += 2 * (G * ((p->center->x * powl(p->Q->x,2.0)) - (e->x * powl(p->Q->x,2.0)) + (p->center->y * A) - (e->y * A) + (p->center->z * B) - (e->z * B)));
  	c += 2 * (H * ((p->center->x * A) - (e->x * A) + (p->center->y * powl(p->Q->y,2.0)) - (e->y * powl(p->Q->y,2.0)) + (p->center->z * C) - (e->z * C)));
  	c += 2 * (I * ((p->center->x * B) - (e->x * B) + (p->center->y * C) - (e->y * C) + (p->center->z * powl(p->Q->z,2.0)) - (e->z * powl(p->Q->z,2.0))));
  	c += 2 * G * H * ((powl(p->Q->x,2.0) * A) + (powl(p->Q->y,2.0) * A) + (B * C));
  	c += 2 * G * I * ((powl(p->Q->x,2.0) * B) + (A * C) + (powl(p->Q->z,2.0) * B));
  	c += 2 * H * I * ((A * B) + (powl(p->Q->y,2.0) * C) + (powl(p->Q->z,2.0) * C));
  	c += -2 * ((p->center->x * e->x) + (p->center->y * e->y) + (p->center->z * e->z));
  	c -= 2 * J * ((A * G * H) + (B * G * I) + (C * H * I));
  	c -= J * ((powl(p->Q->x,2.0) * powl(G,2.0)) + (powl(p->Q->y,2.0) * powl(H,2.0)) + (powl(p->Q->z,2.0) * powl(I,2.0)));

	long double discriminant = powl(b, 2.0) - 4.0 * a*c;
	struct Intersection *ti1 = NULL;
	struct Intersection *ti2 = NULL;
	long double t1, t2;
	if (discriminant < EPSILON) //sin soluciones
		return NULL;
	else //if (discriminant >= EPSILON) //2  soluciones
	{
		struct Intersection *res = NULL;
		t1 = (-b + sqrt(discriminant)) / (2.0 * a);
		t2 = (-b - sqrt(discriminant)) / (2.0 * a);
		if (t1 < EPSILON && t2 < EPSILON) //b1 yb2 estan por detras del ojo
		{
			return NULL;
		}
		ti2 = newIntersection(o, t2, NULL, NULL, newPoint(e->x+t2*d->x, e->y+t2*d->y, e->z+t2*d->z));
		ti1 = newIntersection(o, t1, NULL, NULL, newPoint(e->x+t1*d->x, e->y+t1*d->y, e->z+t1*d->z));
		if (p->d1 > p->d2) //bad distances
		{
			if (t1 > EPSILON)
			{
				res = addIntersection(res, cut(o, ti1));
			}
			if (t2 > EPSILON)
			{
				res = addIntersection(res, cut(o, ti2));
			}
		}
		else if (p->d1 != -INFINITY && p->d1 != INFINITY && p->d2 != -INFINITY && p->d2 != INFINITY) //normal cylinder with 2 distances
		{
			struct Vector *aux = NULL;
			aux = newVector(ti1->i->x - p->center->x, ti1->i->y - p->center->y, ti1->i->z - p->center->z);
			long double distance1 = getDotProduct(p->Q, aux);
			free(aux);
			aux = newVector(ti2->i->x - p->center->x, ti2->i->y - p->center->y, ti2->i->z - p->center->z);
			long double distance2 = getDotProduct(p->Q, aux);
			free(aux);
			if ((p->d1 < distance1 && distance1 < p->d2) && !(p->d1 < distance2 && distance2 < p->d2))
			{
				res = addIntersection(res, cut(o, ti1));
			}
			else if (!(p->d1 < distance1 && distance1 < p->d2) && (p->d1 < distance2 && distance2 < p->d2))
			{
				res = addIntersection(res, cut(o, ti2));
			}
			else if ((p->d1 < distance1 && distance1 < p->d2) && (p->d1 < distance2 && distance2 < p->d2))
			{
				
				if (t1 > EPSILON)
				{
					res = addIntersection(res, cut(o, ti1));
				}
				if (t2 > EPSILON)
				{
					res = addIntersection(res, cut(o, ti2));
				}
			}
		}
		return res;
	}
}

struct Vector* getConeVectorN(struct Point *i, void *sp)
{
	struct Cone* s = (struct Cone*)sp;
	long double temp = (((i->x - s->center->x)*s->Q->x) + ((i->y - s->center->y)*s->Q->y) + ((i->z - s->center->z)*s->Q->z));
	long double xm = s->center->x + s->Q->x*temp;
	long double ym = s->center->y + s->Q->y*temp;
	long double zm = s->center->z + s->Q->z*temp;
	long proportion = powl((s->radio / s->distance),2.0);

	return normalize(newVector(	(xm-i->x)-(proportion * temp * s->Q->x),
					(ym-i->y)-(proportion * temp * s->Q->y), 	
					(zm-i->z)-(proportion * temp * s->Q->z))); 	
}

#endif


