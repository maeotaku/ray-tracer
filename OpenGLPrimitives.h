#ifndef OPENGLPRIMITIVES_H
#define OPENGLPRIMITIVES_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string.h>


void plot(int x, int y, double r, double g, double b);

void glPrintText(int x, int y, double r, double g, double b, void *font, char *string);

#endif