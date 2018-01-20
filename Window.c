#ifndef WINDOW
#define WINDOW

struct Window
{
	struct Point *bottom_left;
	struct Point *top_right;
};

struct Window *newWindow(double xmin, double xmax, double ymin, double ymax)
{
	struct Window *aux = (struct Window *) malloc(sizeof(struct Window));
	aux->bottom_left = newPoint(xmin, ymin, 0);
	aux->top_right = newPoint(xmax, ymax, 0);
	return aux;
}

void print_window(struct Window *window)
{
	if (window != NULL)
	{
		printf("Window: xmin=%Lf ymin=%Lf xmax=%Lf ymax=%Lf\n", window->bottom_left->x,window->bottom_left->y, window->top_right->x,window->top_right->y);
	}
}

#endif
