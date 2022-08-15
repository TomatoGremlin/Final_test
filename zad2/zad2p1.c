#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

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

int main(int argc, char** argp){
    int n = argc; // number of processes
    
    int fd[2];
    
    if(pipe(fd) == -1){
        perror("pipe");
        return EXIT_FAILURE;
    }

    int totalChild=0;
    
    for (int i = 1; i < n; i++)
    {
        pid_t process_split = fork();
        if (-1 == process_split)
        {
            perror("fork");
            return (EXIT_FAILURE);
        }

        if (0 == process_split)
        {
            // child
            int Cur_child_sum = File_op(argp[i]);
            printf("%s - %d", argp[i] , Cur_child_sum);
            totalChild += Cur_child_sum;
        
            read(fd[0], &Cur_child_sum, sizeof(Cur_child_sum));
            write(fd[1], &Cur_child_sum, sizeof(Cur_child_sum));
            close(fd[0]);
            close(fd[1]);

            return 0;
        }
    }
    // parent
    for (int i = 0; i < n; i++) wait(NULL);
    
    int sum;
    read(fd[0], &totalChild, sizeof(totalChild));
    write(fd[1], &sum, sizeof(sum));

    close(fd[0]);
    close(fd[1]);

    printf("Total sum - %d", sum);

    return 0;
}