#ifndef TYPEDEF_H
#define TYPEDEF_H

#define min(a, b) (((a) < (b)) ? (a) : (b))

#include <stdint.h>
#include <semaphore.h>
/* Width and height of out image */
#define WIDTH 2000
#define HEIGHT 1000

/* The vector structure */
typedef struct
{
    float x, y, z;
} vector;

/* The sphere */
typedef struct
{
    vector pos;
    float radius;
    int material;
} sphere;

/* The ray */
typedef struct
{
    vector start;
    vector dir;
} ray;

/* Colour */
typedef struct
{
    float red, green, blue;
} colour;

/* Material Definition */
typedef struct
{
    colour diffuse;
    float reflection;
} material;

/* Lightsource definition */
typedef struct
{
    vector pos;
    colour intensity;
} light;

/* Attributes pass to each thread*/
typedef struct
{
    uint16_t thread_no;
    unsigned char *imgdata;
    uint16_t rows;
    uint16_t rows_start;
    uint16_t cols;
    uint16_t cols_start;
    uint32_t index;
    uint32_t *count;
    sem_t *mutex;
} tdata;

#endif