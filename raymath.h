#ifndef RAYMATH_H
#define RAYMATH_H

#ifndef TYPEDEF_H
#include "typedef.h"
#endif

#include <math.h>
#include <stdbool.h> /* Neededfor boolean datatype */

vector vectorSub(vector *v1, vector *v2);
float vectorDot(vector *v1, vector *v2);
vector vectorScale(float c, vector *v);
vector vectorAdd(vector *v1, vector *v2);
bool intersectRaySphere(ray *r, sphere *s, float *t);

#endif