#ifndef XML_C
#define XML_C

#include "XML.h"

struct Window *window = NULL;
struct Point *eye = NULL;
struct Light *lights[MAX_LIGHTS];
struct Object *objects[MAX_OBJECTS];
struct Group *groups[MAX_GROUPS];
struct Plane *planes[MAX_PLANES];
struct Texture *textures[MAX_TEXTURES];
int numObjects;
int numLights;
int numGroups;
int numPlanes;
int numTextures;
double Ia;
int reflection_levels;
int antialiasing_levels;
int transparency_levels;

struct Window *getWindow(xmlNodePtr cur) //scene cursor
{
	xmlChar* cxaux;
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"Window")))
		{
			double xmin =  atof(xmlGetProp(cur, (const xmlChar*)"xmin"));
			double xmax =  atof(xmlGetProp(cur, (const xmlChar*)"xmax"));
			double ymin =  atof(xmlGetProp(cur, (const xmlChar*)"ymin"));
			double ymax =  atof(xmlGetProp(cur, (const xmlChar*)"ymax"));
			return newWindow(xmin, xmax, ymin, ymax);
		}
		cur = cur->next;
	}
	return NULL;
}

struct Point *getEye(xmlNodePtr cur) //scene cursor
{
	xmlChar* cxaux;
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"Eye")))
		{
			double x =  atof(xmlGetProp(cur, (const xmlChar*)"x"));
			double y =  atof(xmlGetProp(cur, (const xmlChar*)"y"));
			double z =  atof(xmlGetProp(cur, (const xmlChar*)"z"));
			//free(cxaux);
			return newPoint(x,y,z);
		}
		cur = cur->next;
	}
	//free(cxaux);
	return NULL;
}

long double makeInfinite(char *value)
{
	if (strcmp(value,"+INF") == 0)
		return INFINITY;
	else if (strcmp(value,"-INF") == 0)
		return -INFINITY;
	else
		return atof(value);
}

struct Plane *findPlane(char* name)
{
	int i;
	for(i=0; i<numPlanes; i++)
	{
		if (strcmp(planes[i]->name, name)==0)
		{
			return planes[i];
		}
	}
	return NULL;
}

struct Texture *findTexture(char* name)
{
	int i;
	for(i=0; i<numTextures; i++)
	{
		if (strcmp(textures[i]->name,name)==0)
		{
			return textures[i];
		}
	}
	return NULL;
}


void copyFromPlanes(xmlNodePtr cur, struct Object *o)
{
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{	
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"Plane")))
		{
			char *from = xmlGetProp(cur, (const xmlChar*)"from");
			struct Plane *res = findPlane(from);
			if (res!=NULL)
			{
				o->planes[o->num_planes] = res;
				o->num_planes++;
			}
		}
		cur = cur->next;
	}
} 

struct Object *getObject(xmlNodePtr cur)
{
	struct Object *res = NULL;
	if ((!xmlStrcmp(cur->name, (const xmlChar*)"Object")))
	{
		long double r =  atof(xmlGetProp(cur, (const xmlChar*)"r"));
		long double g =  atof(xmlGetProp(cur, (const xmlChar*)"g"));
		long double b =  atof(xmlGetProp(cur, (const xmlChar*)"b"));
		double kd =  atof(xmlGetProp(cur, (const xmlChar*)"kd"));
		double ka =  atof(xmlGetProp(cur, (const xmlChar*)"ka"));
		double ks =  atof(xmlGetProp(cur, (const xmlChar*)"ks"));
		double kn =  atof(xmlGetProp(cur, (const xmlChar*)"kn"));
		double o1 =  atof(xmlGetProp(cur, (const xmlChar*)"o1"));
		double o2 =  atof(xmlGetProp(cur, (const xmlChar*)"o2"));
		double o3 =  atof(xmlGetProp(cur, (const xmlChar*)"o3"));
		int objectType = atoi(xmlGetProp(cur, (const xmlChar*)"type"));
		
		struct Texture *texture = NULL;
		texture = findTexture(xmlGetProp(cur, (const xmlChar*)"texture"));
		if (texture!=NULL)
			printf("%d %d\n", texture->w, texture->h);
		xmlNodePtr pcur = cur->xmlChildrenNode;
		switch(objectType)
		{
			case SPHERE:
			{
				long double xc =  atof(xmlGetProp(cur, (const xmlChar*)"xc"));
				long double yc =  atof(xmlGetProp(cur, (const xmlChar*)"yc"));
				long double zc =  atof(xmlGetProp(cur, (const xmlChar*)"zc"));
				double radio =  atof(xmlGetProp(cur, (const xmlChar*)"radio"));
				res = newObject(	SPHERE, kd, ka, ks, kn, o1,o2,o3,newColor(r,g,b),texture, 
							newSphere(newPoint(xc,yc,zc), radio), 
							&sphereIntersection, &getSphereVectorN, 
							&transformSphere, &getSphereUV);
				break;
			}
			case CYLINDER:
			{
				long double xc =  atof(xmlGetProp(cur, (const xmlChar*)"xc"));
				long double yc =  atof(xmlGetProp(cur, (const xmlChar*)"yc"));
				long double zc =  atof(xmlGetProp(cur, (const xmlChar*)"zc"));
				double radio =  atof(xmlGetProp(cur, (const xmlChar*)"radio"));
				long double qx =  atof(xmlGetProp(cur, (const xmlChar*)"qx"));
				long double qy =  atof(xmlGetProp(cur, (const xmlChar*)"qy"));
				long double qz =  atof(xmlGetProp(cur, (const xmlChar*)"qz"));
				long double d1 = makeInfinite(xmlGetProp(cur, (const xmlChar*)"d1"));
				long double d2 = makeInfinite(xmlGetProp(cur, (const xmlChar*)"d2"));
				res = newObject(	CYLINDER, kd, ka, ks, kn, o1,o2,o3,newColor(r,g,b),texture, 
							newCylinder(newPoint(xc,yc,zc), radio, normalize(newVector(qx,qy,qz)), d1, d2), 
							&cylinderIntersection, &getCylinderVectorN, 
							&transformCylinder, &getCylinderUV);
				break;
			}
			case DISK:
			{
				long double xc =  atof(xmlGetProp(cur, (const xmlChar*)"xc"));
				long double yc =  atof(xmlGetProp(cur, (const xmlChar*)"yc"));
				long double zc =  atof(xmlGetProp(cur, (const xmlChar*)"zc"));
				double radio1 =  atof(xmlGetProp(cur, (const xmlChar*)"radio1"));
				double radio2 =  atof(xmlGetProp(cur, (const xmlChar*)"radio2"));
				long double nx =  atof(xmlGetProp(cur, (const xmlChar*)"nx"));
				long double ny =  atof(xmlGetProp(cur, (const xmlChar*)"ny"));
				long double nz =  atof(xmlGetProp(cur, (const xmlChar*)"nz"));
				struct Point *center = newPoint(xc,yc,zc);
				res = newObject(	DISK, kd, ka, ks, kn, o1,o2,o3,newColor(r,g,b),texture, 
							newDisk(center, newPlane1Point1Normal(center, newVector(nx,ny,nz), NULL), radio1, radio2), 
							&diskIntersection, &getDiskVectorN, 
							&transformDisk, &getDiskUV);
				break;
			}
			case CONE:
			{
				long double xc =  atof(xmlGetProp(cur, (const xmlChar*)"xc"));
				long double yc =  atof(xmlGetProp(cur, (const xmlChar*)"yc"));
				long double zc =  atof(xmlGetProp(cur, (const xmlChar*)"zc"));
				long double radio =  atof(xmlGetProp(cur, (const xmlChar*)"radio"));
				long double distance =  atof(xmlGetProp(cur, (const xmlChar*)"distance"));
				long double qx =  atof(xmlGetProp(cur, (const xmlChar*)"qx"));
				long double qy =  atof(xmlGetProp(cur, (const xmlChar*)"qy"));
				long double qz =  atof(xmlGetProp(cur, (const xmlChar*)"qz"));
				long double d1 = makeInfinite(xmlGetProp(cur, (const xmlChar*)"d1"));
				long double d2 = makeInfinite(xmlGetProp(cur, (const xmlChar*)"d2"));
				res = newObject(	CONE, kd, ka, ks, kn, o1,o2,o3,newColor(r,g,b),texture, 
							newCone(newPoint(xc,yc,zc), normalize(newVector(qx,qy,qz)), d1, d2, distance, radio), 
							&coneIntersection, &getConeVectorN, 
							&transformCone, &getConeUV);
				break;
			}
			case POLYGON:
			{

				struct Polygon *polygon = newPolygon();
				xmlNodePtr pcur = cur->xmlChildrenNode;
				while (pcur != NULL)
				{
					if ((!xmlStrcmp(pcur->name, (const xmlChar*)"Point")))
					{
						double x =  atof(xmlGetProp(pcur, (const xmlChar*)"x"));
						double y =  atof(xmlGetProp(pcur, (const xmlChar*)"y"));
						double z =  atof(xmlGetProp(pcur, (const xmlChar*)"z"));
						polygon->points[polygon->num_points] = newPoint(x,y,z);
						polygon->num_points++;
					}
					pcur = pcur->next;
				}
				free(pcur);
				if (polygon->num_points>=3) // at least 3 points to define the plane
				{		
					initPolygon(polygon);
					res = newObject(POLYGON, kd, ka, ks, kn, o1,o2,o3,newColor(r,g,b),texture, 
							polygon, 
							&polygonIntersection, &getPolygonVectorN, 
							&transformPolygon, &getPolygonUV);
				}
				break;
			}
		}
	}
	if (res!=NULL)
	{
		copyFromPlanes(cur, res);
	}
	return res;
}

void getObjects(xmlNodePtr cur) //scene cursor
{
	numObjects = 0;
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		struct Object *res = getObject(cur);
		if (res!=NULL)
		{
			objects[numObjects] = res;
			numObjects++;	
		}
		cur = cur->next;
	}
}


void getLights(xmlNodePtr cur) //scene cursor
{
	numLights = 0;
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"Light")))
		{

		    double x =  atof(xmlGetProp(cur, (const xmlChar*)"x"));
                    double y =  atof(xmlGetProp(cur, (const xmlChar*)"y"));
                    double z =  atof(xmlGetProp(cur, (const xmlChar*)"z"));
		    double ip =  atof(xmlGetProp(cur, (const xmlChar*)"ip"));
		    double c1 =  atof(xmlGetProp(cur, (const xmlChar*)"c1"));
                    double c2 =  atof(xmlGetProp(cur, (const xmlChar*)"c2"));
                    double c3 =  atof(xmlGetProp(cur, (const xmlChar*)"c3"));
                    lights[numLights] = newLight(newPoint(x,y,z), ip, c1, c2, c3);
                    numLights++;
		}
		cur = cur->next;
	}
}

void getPlanes(xmlNodePtr cur) //scene cursor
{
	numPlanes = 0;
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"Plane")))
		{
			long double xc =  atof(xmlGetProp(cur, (const xmlChar*)"xc"));
			long double yc =  atof(xmlGetProp(cur, (const xmlChar*)"yc"));
			long double zc =  atof(xmlGetProp(cur, (const xmlChar*)"zc"));
			long double nx =  atof(xmlGetProp(cur, (const xmlChar*)"nx"));
			long double ny =  atof(xmlGetProp(cur, (const xmlChar*)"ny"));
			long double nz =  atof(xmlGetProp(cur, (const xmlChar*)"nz"));
			char* name =  xmlGetProp(cur, (const xmlChar*)"name");
			struct Point *center = newPoint(xc,yc,zc);
			planes[numPlanes] = newPlane1Point1Normal(center, newVector(nx,ny,nz), name);
			numPlanes++;
		}
		cur = cur->next;
	}
}

void getTextures(xmlNodePtr cur) //scene cursor
{
	numTextures = 0;
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"Texture")))
		{
			char* name =  xmlGetProp(cur, (const xmlChar*)"name");
			char* filename =  xmlGetProp(cur, (const xmlChar*)"filename");
			textures[numTextures] = readTextureFromFile(name, filename);
			numTextures++;
		}
		cur = cur->next;
	}
}


void getGroups(xmlNodePtr cur)
{
	numGroups = 0;
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"Group")))
		{
			char *name =  xmlGetProp(cur, (const xmlChar*)"name");
			struct Group *group = newGroup(name);
			xmlNodePtr pcur = cur->xmlChildrenNode;
			while (pcur != NULL)
			{
				struct Object *res = getObject(pcur);
				if (res!=NULL)
				{
					group->objects[group->num_objects] = res;
					group->num_objects++;
				}
				pcur= pcur->next;
			}
			groups[numGroups] = group;
			numGroups++;
		}
		cur=cur->next;
	}
}


struct Group *findGroup(char* name)
{
	int i;
	for(i=0; i<numGroups; i++)
	{
		if (strcmp(groups[i]->name,name)==0)
		{
			return groups[i];
		}
	}
	return NULL;
}

void copyFromGroups(xmlNodePtr cur)
{
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"Copy")))
		{
			char *from = xmlGetProp(cur, (const xmlChar*)"from");
			struct Group *res = findGroup(from);
			if (res!=NULL)
			{
				long double movex = atof(xmlGetProp(cur, (const xmlChar*)"movex"));
				long double movey = atof(xmlGetProp(cur, (const xmlChar*)"movey"));
				long double movez = atof(xmlGetProp(cur, (const xmlChar*)"movez"));
				long double scalex = atof(xmlGetProp(cur, (const xmlChar*)"scalex"));
				long double scaley = atof(xmlGetProp(cur, (const xmlChar*)"scaley"));
				long double scalez = atof(xmlGetProp(cur, (const xmlChar*)"scalez"));
				int k=0;
				for (k=0; k<res->num_objects; k++)
				{
					objects[numObjects] =  copyObject(res->objects[k]);
					objects[numObjects]->shape = objects[numObjects]->transform(objects[numObjects]->shape, movex,movey,movez, scalex, scaley, scalez);
					numObjects++;
				}
			}
		}
		cur = cur->next;
	}
} 


/*
void getEnvLight(xmlNodePtr cur) 
{
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar*)"Ia")))
		{
		    Ia =  atof(xmlGetProp(cur, (const xmlChar*)"value"));
		}
		cur = cur->next;
	}
}
*/

void print_objects()
{
	int i;
	for (i=0;i<numObjects; i++)
	{
		print_object(objects[i]);
	}
}

void print_lights()
{
	int i;
	for (i=0;i<numLights; i++)
		print_light(lights[i]);
}

void freeGroups()
{
	int i;
	for (i=0; i<numGroups; i++)
		freeGroup(groups[i]); //groups dont needed
}


static void parseDoc(char *docname)
{
 
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);

	if (doc == NULL )
	{
		printf("No se pudo parsear el documento\n");
		exit(1);
	}
 
	cur = xmlDocGetRootElement(doc); //gets scene

	if (cur == NULL)
	{
		xmlFreeDoc(doc);
		printf("Documento sin nada\n");
		exit(1);
	}
	Ia = atof(xmlGetProp(cur, (const xmlChar*)"Ia"));
	reflection_levels = atof(xmlGetProp(cur, (const xmlChar*)"reflection_levels"));
	antialiasing_levels = atof(xmlGetProp(cur, (const xmlChar*)"antialiasing_levels"));
	transparency_levels = atof(xmlGetProp(cur, (const xmlChar*)"transparency_levels"));
	window = getWindow(cur);
	eye = getEye(cur);
	getGroups(cur);
	getLights(cur);
	getPlanes(cur);
	getTextures(cur);
	getObjects(cur);
	copyFromGroups(cur);
	print_window(window);
	print_point(eye, "Eye");
	print_objects();
	print_lights();
	printf("Enviromental Light: %f\n", Ia);

	xmlFreeDoc(doc);
	
	return;
}

#endif
