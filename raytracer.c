#include "raytracer.h"
#include "typedef.h"
#include "raymath.h"
#include <stdbool.h>
#include <pthread.h>
//#include <semaphore.h>


int thread_cal(int height, int thread_num)
{
    while (true)
    {
        if (height % thread_num == 0)
        {
            return thread_num;
        }
        thread_num++;
    }
}

/* initialise data */
void data_init(material *materials, sphere *spheres, light *lights)
{
    materials[0].diffuse.red = 1;
    materials[0].diffuse.green = 0;
    materials[0].diffuse.blue = 0;
    materials[0].reflection = 0.2;

    materials[1].diffuse.red = 0;
    materials[1].diffuse.green = 1;
    materials[1].diffuse.blue = 0;
    materials[1].reflection = 0.5;

    materials[2].diffuse.red = 0;
    materials[2].diffuse.green = 0;
    materials[2].diffuse.blue = 1;
    materials[2].reflection = 0.9;

    spheres[0].pos.x = 200;
    spheres[0].pos.y = 300;
    spheres[0].pos.z = 0;
    spheres[0].radius = 150;
    spheres[0].material = 0;

    spheres[1].pos.x = 600;
    spheres[1].pos.y = 600;
    spheres[1].pos.z = 0;
    spheres[1].radius = 150;
    spheres[1].material = 1;

    spheres[2].pos.x = 1000;
    spheres[2].pos.y = 240;
    spheres[2].pos.z = 0;
    spheres[2].radius = 150;
    spheres[2].material = 2;

    lights[0].pos.x = 0;
    lights[0].pos.y = 240;
    lights[0].pos.z = -100;
    lights[0].intensity.red = 1;
    lights[0].intensity.green = 1;
    lights[0].intensity.blue = 1;

    lights[1].pos.x = 3200;
    lights[1].pos.y = 3000;
    lights[1].pos.z = -500;
    lights[1].intensity.red = 0.6;
    lights[1].intensity.green = 0.7;
    lights[1].intensity.blue = 1;

    lights[2].pos.x = 600;
    lights[2].pos.y = 0;
    lights[2].pos.z = -100;
    lights[2].intensity.red = 0.3;
    lights[2].intensity.green = 0.5;
    lights[2].intensity.blue = 1;
}

/* */
void raycal(void *ptr)
{
    tdata *data = (tdata *)ptr;
    ray r;
    material materials[3];
    sphere spheres[3];
    light lights[3];
    data_init(materials, spheres, lights);

    int x, y;
    for (y = data->rows_start; y < data->rows_start + data->rows; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {

            float red = 0;
            float green = 0;
            float blue = 0;

            int level = 0;
            float coef = 1.0;

            r.start.x = x;
            r.start.y = y;
            r.start.z = -2000;

            r.dir.x = 0;
            r.dir.y = 0;
            r.dir.z = 1;

            do
            {
                /* Find closest intersection */
                float t = 20000.0f;
                int currentSphere = -1;

                unsigned int i;
                for (i = 0; i < 3; i++)
                {
                    if (intersectRaySphere(&r, &spheres[i], &t))
                        currentSphere = i;
                }
                if (currentSphere == -1)
                    break;

                vector scaled = vectorScale(t, &r.dir);
                vector newStart = vectorAdd(&r.start, &scaled);

                /* Find the normalfor this new vector at the point of intersection */
                vector n = vectorSub(&newStart, &spheres[currentSphere].pos);
                float temp = vectorDot(&n, &n);
                if (temp == 0)
                    break;

                temp = 1.0f / sqrtf(temp);
                n = vectorScale(temp, &n);

                /* Find the material to determine the colour */
                material currentMat = materials[spheres[currentSphere].material];

                /* Find the value of the light at this point */
                unsigned int j;
                for (j = 0; j < 3; j++)
                {
                    light currentLight = lights[j];
                    vector dist = vectorSub(&currentLight.pos, &newStart);
                    if (vectorDot(&n, &dist) <= 0.0f)
                        continue;
                    float t = sqrtf(vectorDot(&dist, &dist));
                    if (t <= 0.0f)
                        continue;

                    ray lightRay;
                    lightRay.start = newStart;
                    lightRay.dir = vectorScale((1 / t), &dist);

                    /* Lambert diffusion */
                    float lambert = vectorDot(&lightRay.dir, &n) * coef;
                    red += lambert * currentLight.intensity.red * currentMat.diffuse.red;
                    green += lambert * currentLight.intensity.green * currentMat.diffuse.green;
                    blue += lambert * currentLight.intensity.blue * currentMat.diffuse.blue;
                }
                /* Iterate over the reflection */
                coef *= currentMat.reflection;

                /* The reflected ray start and direction */
                r.start = newStart;
                float reflect = 2.0f * vectorDot(&r.dir, &n);
                vector tmp = vectorScale(reflect, &n);
                r.dir = vectorSub(&r.dir, &tmp);

                level++;

            } while ((coef > 0.0f) && (level < 15));

            data->imgdata[(x + y * WIDTH) * 3 + 0] = (unsigned char)min(red * 255.0f, 255.0f);
            data->imgdata[(x + y * WIDTH) * 3 + 1] = (unsigned char)min(green * 255.0f, 255.0f);
            data->imgdata[(x + y * WIDTH) * 3 + 2] = (unsigned char)min(blue * 255.0f, 255.0f);

            /* Lock (Increament the lock value), if already locked this thread goes to blocked state */
            pthread_mutex_lock(data->mutex);
            /* critical section - counter for sub-pixels */
            *data->count += 3;
            /* Unlock (Decreament the lock value.) */
            pthread_mutex_unlock(data->mutex);            


            /* Using Semaphore */
            /* wait / DOWN */
            //sem_wait(data->mutex);
            /* critical section*/
            /* counter for how many sub-pixels are calculated*/
            //*data->count += 3;
            /* signal / UP */
            //sem_post(data->mutex);
        }
    }
    pthread_exit(0);
}