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
    FILE *read_binary = fopen(name, "rb");
    if (read_binary == NULL)
    {
        fprintf(stderr, "%s - not exists!\n", name);
        return -1;
    }
    int number[10];

    while (fread(number, sizeof(int), number, read_binary) != EOF)
    {
    }

    fclose(read_binary);

    int bits;
    for (size_t i = 0; i <10; i++)
    {
        bits += onesCount(number[i]);
    }
    return bits; //bits in current file
}

void *routine1(void* string){
    char* arg = (char*)string;
    int Cur_child_sum = File_op(arg);
    printf("%s - %d", arg, Cur_child_sum);
    totalChild += Cur_child_sum;

    read(fd[0], &Cur_child_sum, sizeof(Cur_child_sum));
    write(fd[1], &Cur_child_sum, sizeof(Cur_child_sum));
    close(fd[0]);
    close(fd[1]);
}

int main(int argc, char **argp)
{
    int n = argc; 
    int totalChild=0;
    
    int fd[2];
    unsigned N = atoi(argp[1]);
    unsigned M = atoi(argp[2]);
    pthread_t threads [n-1];

    for (size_t j = 1; j < n ; j++)
    {
        if (pthread_create(&threads[j], NULL, routine1, argp[i]))
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
    return 0;
}