#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#define THREAD_COUNT 3

void *routine1(void* arg){
    char* MSG= (char*)arg;
    printf("%s\n", MSG);
    printf("Process PID = %d, Thread ID = %ld\n\n", getpid(), pthread_self());
}

int main(int argc, char **argp)
{
    unsigned N = atoi(argp[1]);
    unsigned M = atoi(argp[2]);
    pthread_t threads [THREAD_COUNT ];

    for (size_t j = 0; j < THREAD_COUNT ; j++)
    {
        if (pthread_create(&threads[j], NULL, routine1, NULL))
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
    return 0;
}