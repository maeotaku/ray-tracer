
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "OpenGLPrimitives.c"
#include "XML.c"
//#include "Avs.c"
//#include "PNG.c"





#define H_SIZE 1008 //width
#define V_SIZE 567  //height


struct Color ***fb;//[H_SIZE][V_SIZE];
int countAliasing[H_SIZE][V_SIZE];
//struct Texture *finalScene;

int res;
//int times = 1;

char scenefile[100];
char outputfile[100];


void specialKey(int key, int _x, int _y) {
    double boost = 0;
    if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
        boost += 0.5;
    }
    switch (key) {
        case GLUT_KEY_DOWN:
            break;
        case GLUT_KEY_UP:
            break;
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:
            break;
        case GLUT_KEY_HOME:
            break;
        case GLUT_KEY_PAGE_DOWN:
            break;
        case GLUT_KEY_PAGE_UP:
            break;
        case GLUT_KEY_END:
            break;
    }
}

void keyboard(unsigned char key, int _x, int _y) {
    printf("keydown \n");
    if (key == 27)
        exit(0);

}

void initCountAliasing()
{
	int i,j;
	for (i=0; i<H_SIZE; i++)
		for (j=0; j<V_SIZE; j++)
			countAliasing[i][j] = 0;
}

void initFB()
{
	fb = (struct Color ***)malloc(H_SIZE * sizeof(struct Color **));
	int i;
	for (i = 0; i < H_SIZE; i++) 
	{
		fb[i] = (struct Color **)malloc(V_SIZE * sizeof(struct Color*));
	}
}

struct Intersection* first_intersection(struct Point *e, struct Vector *d) {
    if (numObjects >= 1) {
        struct Intersection *i = NULL;
	struct Intersection *head = NULL;
	int a = 0;
        for (a = 0; a < numObjects; a++) 
	{
		if (objects[a]!=NULL && objects[a]->shape!=NULL)
		{
			i = objects[a]->intersectionFunction(e, d, objects[a], objects[a]->shape);
			if (i != NULL)
			{
				if (i->t > EPSILON) { //no me nteresan las cosas que estan detras del ojo
					head = addIntersection(head,i);
					//freeIntersection(i);
				}
				else
					freeIntersection(i);
			}

		}
        }
	return head;
    }
    return NULL;
}


//vector de la luz hacia el punto de interseccion
struct Vector *getL(struct Intersection *i, struct Light *light) {
	struct Vector *aux = newVector(light->center->x - i->i->x, light->center->y - i->i->y, light->center->z - i->i->z);
	long double distance = getNorm(aux);
	aux->x = aux->x/distance;
	aux->y = aux->y/distance;
	aux->z = aux->z/distance;
	return aux;
}

//vector R relejo del vector L
struct Vector *getRe(struct Vector *N, struct Vector *L) //get vector R for reflexion especular
{
	//R = 2N (N.L) - L
	long double dot_product = getDotProduct(L,N);
	return newVector( (N->x * 2.0 * dot_product)-L->x, (N->y * 2.0 * dot_product)-L->y, (N->z*2.0*dot_product)-L->z );
	//return takeAwayVector(multiplyVector(multiplyVector(N, 2.0), getDotProduct(L,N)), L); 
}


struct Vector *getRf(struct Vector *N, struct Vector *V) //get vector R for reflejo
{
	//R = 2 (N.V) * N - V
	long double dot_product = getDotProduct(V,N);
	return newVector( (N->x * 2.0 * dot_product)-V->x, (N->y * 2.0 * dot_product)-V->y, (N->z*2.0*dot_product)-V->z );
	//return takeAwayVector(multiplyVector(multiplyVector(N, 2.0), getDotProduct(L,N)), L); 
}


long double getFatt(long double distance, double c1, double c2, double c3)
{
	long double den = (c1 + (c2*distance) + (c3*powl(distance, 2.0)));
	if (den==0)
		return 1.0;
	long double aux = 1.0 / den;
	if (aux > 1.0)
		return 1.0;
	else
		return aux;
}

long double slash(long double k)
{
	if (k > 1.0)
            	return 1.0;
	else if (k < 0.0)
		return 0.0;
	else return k;
}

struct Color *color_or_texture(struct Object *object, struct UVPoint *uv)
{
	if (object->texture == NULL || uv == NULL)
	{
		//freeUVPoint(uv);
		return object->color;
	}
	else
	{	
		int u = (int)(uv->u * object->texture->w);// % (long double)object->texture->w);
		int v = (int)(uv->v * object->texture->h);// % (long double)object->texture->h);
		freeUVPoint(uv);
		if (u >= object->texture->w || v >= object->texture->h || u < 0 || v<0)
			return object->color;
		//printf("%Lf %Lf %d %d \n", uv->u, uv->v, u, v);
		
		return object->texture->colors[u][v];
	}
}

struct Color *de_que_color(struct Point *e, struct Vector *d, int reflection_levels) 
{
	struct Color *c = newColor(0.0,0.0,0.0);
	int t_levels = transparency_levels;
	struct Intersection* first = first_intersection(e, d);
	struct Intersection* head = first;
	if (first == NULL) 
	{
		freeIntersection(first);
		return newColor(0.6, 0.8, 0.8);
    	} 
	else 
	{
		long double Fatt;
		long double distance_to_light;
		long double dot_product_i;
		long double dot_product_e;
		while(first!=NULL)
		{
			struct Vector *N = first->object->getVectorN(first->i, first->object->shape);
			if (getDotProduct(d,N) > EPSILON) //vector normal debe apuntar hacia donde esta el ojo
			{
				//N = invertVector(N);
				N->x = N->x * -1;		
				N->y = N->y * -1;
				N->z = N->z * -1;
			}
			struct Vector *V = invertVector(d); //vector del punto de interseccion hacia el ojo, al reves de el rayo del ojo
			int k;
			long double fl = 0; //factor de luminosidad
			long double fe = 0; //factor de reflexion especular
			//long double ff = 0; //factor of reflection
			struct Vector *L = NULL;
			struct Vector *Re = NULL;
			struct Vector *Rf = NULL;
			struct Vector *aux = NULL;
			for (k = 0; k < numLights; k++) 
			{
				aux = newVector(lights[k]->center->x - first->i->x, lights[k]->center->y - first->i->y, lights[k]->center->z - first->i->z);
				distance_to_light = getNorm(aux);
				L = newVector(aux->x / distance_to_light, aux->y / distance_to_light, aux->z / distance_to_light);
				Re = getRe(N, L);
				dot_product_i = getDotProduct(L, N);
				dot_product_e = getDotProduct(Re, V);
			    	if (dot_product_i > EPSILON && dot_product_e > EPSILON)
			    	{
					Fatt = getFatt(distance_to_light,  lights[k]->c1, lights[k]->c2, lights[k]->c3 );
					struct Intersection *shadow_object = first_intersection(first->i, L);
					if (shadow_object == NULL || (shadow_object!=NULL && shadow_object->t > distance_to_light)) 
					{
						fl = fl + (dot_product_i * first->object->kd * lights[k]->ip * Fatt);
						fe = fe + (powl(dot_product_e, first->object->kn) * first->object->ks * lights[k]->ip * Fatt);
					}
					freeIntersection(shadow_object);
				}
				freeVector(L);
				freeVector(Re);
				freeVector(aux);
			}
			fl = slash(fl + (Ia * first->object->ka));
			fe = slash(fe);
			struct Color *reflection_color = NULL;
			if (reflection_levels > 0 && first->object->o2 != 0) //mientras hayan niveles de reflejo y la constante para reflejos tenga valor
			{
				Rf = getRe(N, V); //vector going to objects used in reflection
				reflection_color = de_que_color(first->i, Rf, reflection_levels -1);
				c->r = c->r + (color_or_texture(first->object, first->object->getUV(first->object->shape, first, N))->r*fl)+(reflection_color->r*first->object->o2);
				c->g = c->g + (color_or_texture(first->object, first->object->getUV(first->object->shape, first, N))->g*fl)+(reflection_color->g*first->object->o2);
				c->b = c->b + (color_or_texture(first->object, first->object->getUV(first->object->shape, first, N))->b*fl)+(reflection_color->b*first->object->o2);
			}
			else
			{
				c->r = c->r + color_or_texture(first->object, first->object->getUV(first->object->shape, first, N))->r*fl+fe;
				c->g = c->g + color_or_texture(first->object, first->object->getUV(first->object->shape, first, N))->g*fl+fe;
				c->b = c->b + color_or_texture(first->object, first->object->getUV(first->object->shape, first, N))->b*fl+fe;
			}
			freeColor(reflection_color);
			freeVector(N);
			freeVector(V);
			freeVector(Rf);

			if (first->object->o3==0.0 || t_levels<0)
				break;
			first = first->next; //for transparency		 	
			t_levels--;
		}
		freeIntersection(head);
		return c;
	}
}


struct Color *de_que_color_per_pixel(int i, int j, long double factorx, long double factory)
{
	long double Xw = ((long double)i + factorx)*(window->top_right->x - window->bottom_left->x) / H_SIZE + window->bottom_left->x;
	long double Yw = ((long double)j + factory)*(window->top_right->y - window->bottom_left->y) / V_SIZE + window->bottom_left->y;
	struct Vector *w, *d;
	//vector que pasa por la ventana equivalente al pixel actual
	w = newVector(Xw, Yw, 0.0);
	struct Vector *aux = newVector(w->x - eye->x, w->y - eye->y, w->z - eye->z);
	d = normalize(aux);
	//busca el color del vector que pasa desde el ojo, por la ventana, hacia la escena
	//se pasa d normalizado
	struct Color *color = de_que_color(eye, d, reflection_levels);
	freeVector(aux);
	freeVector(w);
	freeVector(d);
	return color;
}


struct Color *getAntialiasing(int i, int j, long double mini, long double minj, long double factor, int cont)
{
	struct Color *central = de_que_color_per_pixel(i,j,mini+(factor/2),minj+(factor/2));
	if (cont==0)
	{
		countAliasing[i][j]++;
		return central;
	}
	struct Color *corner1 = de_que_color_per_pixel(i,j,mini,minj);
	struct Color *corner2 = de_que_color_per_pixel(i,j,mini+factor,minj);
	struct Color *corner3 = de_que_color_per_pixel(i,j,mini,minj+factor);
	struct Color *corner4 = de_que_color_per_pixel(i,j,mini+factor,minj+factor);
	struct Color *aux = newColor(0,0,0);
	if (colorSim(central, corner1) >= SIM_RGB)
	{
		aux->r = aux->r + corner1->r + central->r;
		aux->g = aux->g + corner1->g + central->g;
		aux->b = aux->b + corner1->b + central->b;
		freeColor(corner1);
		countAliasing[i][j]+=2;
	}
	else
	{
		freeColor(corner1);
		struct Color *res = getAntialiasing(i, j, mini, minj, factor/2.0, cont-1);
		aux->r = aux->r + res->r;
		aux->g = aux->g + res->g;
		aux->b = aux->b + res->b;
		countAliasing[i][j]++;
		freeColor(res);
	}
	if (colorSim(central, corner2) >= SIM_RGB)
	{
		aux->r = aux->r + corner2->r + central->r;
		aux->g = aux->g + corner2->g + central->g;
		aux->b = aux->b + corner2->b + central->b;
		freeColor(corner2);
		countAliasing[i][j]+=2;
	}
	else
	{
		freeColor(corner2);
		struct Color *res = getAntialiasing(i, j, mini + (factor/2.0), minj, factor/2.0, cont-1);
		aux->r = aux->r + res->r;
		aux->g = aux->g + res->g;
		aux->b = aux->b + res->b;
		countAliasing[i][j]++;
		freeColor(res);
	}
	if (colorSim(central, corner3) >= SIM_RGB)
	{
		aux->r = aux->r + corner3->r + central->r;
		aux->g = aux->g + corner3->g + central->g;
		aux->b = aux->b + corner3->b + central->b;
		freeColor(corner3);
		countAliasing[i][j]+=2;
	}
	else
	{
		freeColor(corner3);
		struct Color *res = getAntialiasing(i, j, mini, minj + (factor/2.0), factor/2.0, cont-1);
		aux->r = aux->r + res->r;
		aux->g = aux->g + res->g;
		aux->b = aux->b + res->b;
		countAliasing[i][j]++;
		freeColor(res);
	}
	if (colorSim(central, corner4) >= SIM_RGB)
	{
		aux->r = aux->r + corner4->r + central->r;
		aux->g = aux->g + corner4->g + central->g;
		aux->b = aux->b + corner4->b + central->b;
		freeColor(corner4);
		countAliasing[i][j]+=2;
	}
	else
	{
		freeColor(corner4);
		struct Color *res = getAntialiasing(i, j, mini + (factor/2.0), minj + (factor/2.0), factor/2.0, cont-1);
		aux->r = aux->r + res->r;
		aux->g = aux->g + res->g;
		aux->b = aux->b + res->b;
		countAliasing[i][j]++;
		freeColor(res);
	}
	freeColor(central);
	return aux;
}




void rayTrace()
{
	fb = initColorMatrix(V_SIZE, H_SIZE);
	int i, j;
	long double factor;
	factor = 1.0;
	for (j = 0; j < V_SIZE; j++) {
		for (i = 0; i < H_SIZE; i++) {
			struct Color *color = getAntialiasing(i, j, 0.0, 0.0, 1.0, antialiasing_levels);
			color->r = color->r / countAliasing[i][j];
			color->g = color->g / countAliasing[i][j];
			color->b = color->b / countAliasing[i][j];
			if (color != NULL)
			{
				fb[i][j] = color;
			}
		}
	}
	
}

void draw_scene() {
	
	glClear(GL_COLOR_BUFFER_BIT); //clean screen
	int i, j;
	for (i = 0; i < H_SIZE; i++) 
		for (j = 0; j < V_SIZE; j++) 
			if (fb[i][j] != NULL)
				plot(i, j, fb[i][j]->r, fb[i][j]->g, fb[i][j]->b);
	glFlush();
}

void draw()
{
		struct Texture *t = readTextureFromFile("lena", "lena.bmp");
	int i,j;
	glClear(GL_COLOR_BUFFER_BIT); //clean screen
	for (i=0;i<t->h;i++)
		for (j=0;j<t->w;j++)
			plot(i, j, t->colors[i][j]->r, t->colors[i][j]->g, t->colors[i][j]->b);
	glFlush();
}

int main(int argc, char** argv) 
{
	/*
	struct Intersection *head = NULL;
	struct Intersection *a = newIntersection(NULL, 25, NULL, NULL, NULL);
	struct Intersection *b = newIntersection(NULL, 10, NULL, NULL, NULL);
	struct Intersection *c = newIntersection(NULL, 30, NULL, NULL, NULL);
	struct Intersection *d = newIntersection(NULL, 5, NULL, NULL, NULL);
	head = addIntersections(NULL, NULL);
	//print_intersections(head);
	//head = addIntersections(head,a);
	head = addIntersections(head,d);
	print_intersections(head);
	head = addIntersections(head,b);
	print_intersections(head);
	head = addIntersections(head,a);
	print_intersections(head);
	head = addIntersections(head,c);
	print_intersections(head);
	*/
	
	//printf("%d %d %d %d\n",sizeof(float), sizeof(double), sizeof(png_byte), sizeof(char));
	if (argc != 3)
	{
		printf("Uso: ./rt <scene> <output>\n");
		exit(1);
	}
	sscanf(argv[1],"%s", &scenefile);
	sscanf(argv[2],"%s", &outputfile);

	
	//print_texture(t);
	//struct Texture *t = read_png_file("lol.png");
	/*
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(512, 512);
	printf("%i", glutCreateWindow("Ray Tracer"));

	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(-0.5, 512 + 0.5, -0.5, 512 + 0.5);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(draw);
	glutMainLoop();
	*/

	
	parseDoc(scenefile);
	printf("Archivo leido...\n");
	rayTrace();
	//framebuffer_export_bmp(V_SIZE, H_SIZE, fb, outputfile); //BMP
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(H_SIZE, V_SIZE);
	printf("%i", glutCreateWindow("Ray Tracer"));

	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(-0.5, H_SIZE + 0.5, -0.5, V_SIZE + 0.5);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(draw_scene);
	glutMainLoop();
	
	return 0;
	
	
}



