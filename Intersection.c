#ifndef INTERSECTION
#define INTERSECTION

struct Intersection {
    struct Object* object;
    long double t;
    struct Point *i;
    struct Intersection* next;
    //struct Intersection* prev;

};

struct Intersection *newIntersection(struct Object* o, long double t, struct Intersection* n, struct Intersection* p, struct Point *pintersection) {
    struct Intersection *aux = (struct Intersection *) malloc(sizeof (struct Intersection));
    aux-> object = o;
    aux-> next = n;
    //aux-> prev = p;
    aux -> t = t;
    aux -> i = pintersection;
    return aux;
}

void freeIntersection(struct Intersection *i)
{
	if (i!=NULL )
	{
		freeIntersection(i->next);
		i->object = NULL;
		//i->next = NULL;
		freePoint(i->i);
		free(i);
	}
}

/*
void freeIntersections(struct Intersection *i)
{
	if (i!=NULL)
	{
		freeIntersections(i->next);
	}
	freeIntersection(i);
}
*/


struct Intersection *addIntersection(struct Intersection *head, struct Intersection *new)
{
	if (head==NULL && new==NULL)
		return NULL;
	if (head==NULL)
		return new;
	if (new==NULL)
		return head;
	if (new->t <= head->t) 
	{ 
		new->next = head; 
		head = new; 
		return head; 
	}
	struct Intersection *curr = head; // start at the begining node. 
	while ( curr->next != NULL ) 
	{ 
		if (new->t < curr->next->t) 
		{ 
			if(new->t > curr->t) 
			{ 
				
				new->next = curr->next; 
				curr->next = new; 
				//freeIntersection(curr);
				return head;
			} 
		}
		curr = curr->next;
	} 
	curr->next = new; 
	//freeIntersection(curr);
	return head; 

}

struct Intersection *addIntersections(struct Intersection *head, struct Intersection *new)
{
	if (head==NULL && new==NULL)
		return NULL;
	if (head==NULL)
		return new;
	if (new==NULL)
		return head;
	struct Intersection *aux = new;
	struct Intersection *next = NULL;//aux->next;
	while(aux!=NULL)
	{
		next = aux->next;
		head = addIntersection(head, aux);
		aux = next;
	}
	freeIntersection(aux);
	return head;
}

void print_intersection(struct Intersection *i)
{
	if (i!=NULL)
		printf("Intersection: t=%Lf\n", i->t);
}

void print_intersections(struct Intersection *head)
{
	struct Intersection *aux = head;
	while(aux!=NULL)
	{
		print_intersection(aux);
		aux=aux->next;
	}
}




#endif
