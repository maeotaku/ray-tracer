#ifndef OPENGLPRIMITIVES_C
#define OPENGLPRIMITIVES_C

#include "OpenGLPrimitives.h"


void plot(int x, int y, double r, double g, double b)
{
	glColor3f(r,g,b);
	glBegin (GL_POINTS);
	glVertex2i (x,y);
        glEnd();
	
}


void glPrintText(int x, int y, double r, double g, double b, void *font, char *string)
{
	glColor3f( r, g, b );
	glRasterPos2f(x, y);
	int len, i;
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}

#endif
