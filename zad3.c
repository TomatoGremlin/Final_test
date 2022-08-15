#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <math.h>
#define THREAD_COUNT 3

unsigned N;
unsigned M;
double result = 0;

void *routine1(void* arg){
    double **matrix = (double**)arg;
    double sum;
   
    for (int i = 0; i < N/THREAD_COUNT; i++)
    {
        for (int j = 0; j < M; j++)
        {
            sum += matrix[N][M]*matrix[N][M];
        }
    }
    result += sqrt(sum); 
}

int main(int argc, char **argp)
{
    unsigned N = atoi(argp[1]);
    unsigned M = atoi(argp[2]);

    double matrix [ N ][ M ];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
           matrix[N][M] = (double)atoi(argp[i]);
        }
    }
    
    pthread_t threads [THREAD_COUNT];
    for (size_t j = 0; j < THREAD_COUNT ; j++)
    {
        if (pthread_create(&threads[j], NULL, routine1, matrix))
        {
            perror("thread create");
            return EXIT_FAILURE;
        }
    }

    for (size_t j = 0; j < THREAD_COUNT ; j++)
    {
        if (pthread_join(threads[j], NULL))
        {
            perror("join");
            return EXIT_FAILURE;
        }
    }

    FILE *f = fopen(argp[argc-1], "w+");
    if (f == NULL)
    {
        fprintf(stderr, "Could not open file\n");
        return -1;
    }
    for (size_t i = 0; i < N; i++)
    {
        fprintf(f,"%lf\n", result);
    }
    
    fclose(f);
 
    return 0;
}
