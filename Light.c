#ifndef LIGHT
#define LIGHT

struct Light {
    struct Point *center;
    double ip,c1,c2,c3;
};

struct Light *newLight(struct Point *center, double ip, double c1, double c2, double c3) {
    struct Light *aux = (struct Light *) malloc(sizeof (struct Light));
    aux-> center = center;
    aux-> ip = ip;
    aux->c1 = c1;
    aux->c2 = c2;
    aux->c3 = c3;
    return aux;
}

void print_light(struct Light *light) {
    printf("Light xc=%Lf yc=%Lf zc=%Lf ip=%f c1=%f c2=%f c3=%f\n", light->center->x, light->center->y, light->center->z, light->ip, light->c1, light->c2, light->c3);
}


#endif
