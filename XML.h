#ifndef XML_H
#define XML_H

#include <stdio.h>
#include <stdlib.h>
#include <libxml2/libxml/xmlreader.h>  //lectura de xml
#include <libxml2/libxml/tree.h> //creacion de arboles xml
#include <libxml2/libxml/parser.h> //parser de xml
#include <libxml2/libxml/xinclude.h> 
#include <libxml2/libxml/xmlIO.h> //libreria de entrada/salida y de memoria respecto a xml
#include "Structures.c"

struct Window *window;
struct Point *eye;
struct Light *lights[];

struct Object *objects[];
int numObjects;
int numLights;
double Ia;

struct Window *getWindow(xmlNodePtr cur);
struct Point *getEye(xmlNodePtr cur);

void getObjects(xmlNodePtr cur);
void getEnvLight(xmlNodePtr cur);
void getLights(xmlNodePtr cur);
void print_objects();
void print_lights();

static void parseDoc(char *docname);

#endif
