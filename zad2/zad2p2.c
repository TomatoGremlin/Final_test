#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>


int onesCount(int mask)
{
    int ones = 0;
    for (size_t j = 0; j < sizeof(mask) * 8; j++)
    {
        if (mask & (1ull << j))
        {
            ones++;
        }
    }
    return ones;
}

int File_op(char* name){
    FILE *read = fopen(name, "r");
    if (read == NULL)
    {
        fprintf(stderr, "%s - not exists!\n", name);
        exit(-1);
    }
    char new_line = getc(read);
    int line_counter;
    while (new_line != EOF)
    {
        if (new_line == '\n')
            line_counter++;
        new_line = getc(read);
    }

    rewind(read);
    int number[line_counter];

    for (size_t i = 0; i < line_counter; i++)
    {
        fscanf(read, "%d", &number[i]);
    }

    fclose(read);

    int bits = 0;
    for (size_t i = 0; i < line_counter; i++)
    {
        bits += onesCount(number[i]);
    }
    return bits; //bits in current file
}
int totalChild=0;
pthread_mutex_t mux;

void *routine1(void* string){
    char* arg = (char*)string;
    int Cur_child_sum = File_op(arg);
    printf("%s - %d\n", arg, Cur_child_sum);
    pthread_mutex_lock (&mux);
    totalChild += Cur_child_sum;
    pthread_mutex_unlock (&mux);

}

int main(int argc, char **argp)
{
    int n = argc-1; 
    pthread_t threads [n];

    for (size_t j = 0; j < n ; j++)
    {
        if (pthread_create(&threads[j], NULL, routine1, argp[j+1]))
        {
            perror("thread create");
            return EXIT_FAILURE;
        }
    }
    for (size_t j = 0; j < n ; j++)
    {
        if (pthread_join(threads[j], NULL))
        {
            perror("join");
            return EXIT_FAILURE;
        }
    }

    printf("Total sum - %d\n", totalChild);
    pthread_mutex_destroy(&mux);
    return 0;
}
