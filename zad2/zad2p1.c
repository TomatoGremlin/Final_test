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

int main(int argc, char** argp){
    int n = argc; // number of processes
    
    int fd[2];
    
    if(pipe(fd) == -1){
        perror("pipe");
        return EXIT_FAILURE;
    }

    int totalChild=0;
    int Cur_child_sum =0;
    
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
            close(fd[0]);
            Cur_child_sum = File_op(argp[i]);
            printf("%s - %d\n", argp[i], Cur_child_sum);
            read(fd[0], &totalChild, sizeof(totalChild));
            totalChild += Cur_child_sum;

            write(fd[1], &totalChild, sizeof(totalChild));
            close(fd[1]);
            
            return 0;
        }
         
    }
    // parent
    close(fd[1]);
    for (int i = 0; i < n; i++) wait(NULL);
    
    read(fd[0], &totalChild, sizeof(totalChild));

    int sum = totalChild;

    printf("Total sum - %d\n", sum);

    return 0;
}
