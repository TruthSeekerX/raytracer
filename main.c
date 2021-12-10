/* A simple ray tracer */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include "raytracer.h"
#include "fileio.h"
#include "raymath.h"
#include "typedef.h"



int main(int argc, char *argv[])
{
    uint32_t count = 0;
    /* Will contain the raw image */
    unsigned char imgdata[3 * WIDTH * HEIGHT];

    /* If no given argument THREAD_NUM = 1, MAX number of threads = HEIGHT */
    int THREAD_NUM = argc > 1 ? atoi(argv[1]) > HEIGHT ? HEIGHT : atoi(argv[1]) : 1;
    THREAD_NUM = thread_cal(HEIGHT, THREAD_NUM);
    printf("threads NO. = %d\n", THREAD_NUM);

    tdata data[THREAD_NUM];
    pthread_t threads[THREAD_NUM];

    /* initialise binary semaphore */
    sem_t mutex;
    if (sem_init(&mutex, 0, 1) != 0)
    {
        printf("Error! Semaphore initializtion failed.\n");
        return 1;
    }

    /* create threads */
    for (int i = 0; i < THREAD_NUM; i++)
    {
        /* For the last part of rows numbers might not be the same as before*/
        data[i].thread_no = i + 1;
        data[i].imgdata = imgdata;
        data[i].rows = HEIGHT / THREAD_NUM;
        data[i].rows_start = i * HEIGHT / THREAD_NUM;
        data[i].cols = WIDTH;
        data[i].cols_start = 0;
        data[i].index = i * data[i].rows * data[i].cols * 3;
        data[i].count = &count;
        data[i].mutex = &mutex;
        int errnum = pthread_create(&threads[i], NULL, (void *)&raycal, (void *)&data[i]);
        if (errnum != 0)
        {
            printf("Error, fails to create threads!\n");
            return 1;
        }
    }

    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex);
    saveppm("raytrace_threads.ppm", imgdata, WIDTH, HEIGHT);
    printf("height=%d, width=%d\n", HEIGHT, WIDTH);
    printf("Number of sub-pixels counted are:%u\n(pixel count=%u)\n", count, count / 3);

    return 0;
}

