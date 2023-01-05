#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "parsing.c"

#define BUFFER_SIZE 4096

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{

    int nbytes, fdpai[2], fdfilho[2] ;
    pid_t pid;
    char line[BUFFER_SIZE], line1[BUFFER_SIZE];
    memset(line, '\0', BUFFER_SIZE);
    memset(line1, '\0', BUFFER_SIZE);

    if (pipe(fdpai) ||pipe(fdfilho))
    {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        /* parent */
        close(fdpai[WRITE_END]);
        close(fdfilho[READ_END]);

        if(read(STDIN_FILENO, line, BUFFER_SIZE) < 0){
            perror("read input error\n");   
            return 1;
        }

        if ((nbytes = write(fdfilho[WRITE_END], line, strlen(line))) < 0)
        {
            fprintf(stderr, "Unable to write to pipe: \n");
        }
        close(fdfilho[WRITE_END]);

        if(read(fdpai[READ_END], line, BUFFER_SIZE) < 0){
            perror("read child message error\n");
            exit(EXIT_FAILURE);
        }
        close(fdpai[READ_END]);
        /* wait for child and exit */
        if (waitpid(pid, NULL, 0) < 0)
        {
            fprintf(stderr, "Cannot wait for child: \n");
        }
        //read(fdpai[READ_END], line, BUFFER_SIZE);
        write(STDOUT_FILENO,line,strlen(line));

    }
    else
    {
    /* child */
    close(fdfilho[WRITE_END]);        
    close(fdpai[READ_END]);

        //printf("Child process with pid %d\n", getpid());
        //printf("Receiving message from parent (pid %d):\n", getppid());
        if ((nbytes = read(fdfilho[READ_END], line1, BUFFER_SIZE)) < 0)
        {
            fprintf(stderr, "Unable to read from CHILD: \n");
        }
        close(fdfilho[READ_END]);
        

    char* filename;
    //no arguments, just stdin
    if(argc < 2) {
        filename = NULL;
    }
    
    else {
        printf("Usage: ...bla bla");
        return -1;
    }
    /* 
        Text to send 
    
    */
    long textLength;
    int lines;
    char* text = readFile(filename, &lines, &textLength);


    /*
        read cypher file

    */
    char* cypherText = readFile("cypher.txt", &lines, &textLength);

    char cypherOld[lines][32];
    char cypherNew[lines][32];

    char * delimiter = " \n";
    char * cypherWords;
    int cypherSyze = 0;
    int newWord = 0;

    //Create cypher arrays
    cypherWords = strtok(cypherText, delimiter);
    while(cypherWords != NULL) {
        if(!newWord) {
            strcpy(cypherOld[cypherSyze], cypherWords);
            newWord = 1;
        }
        else {
            strcpy(cypherNew[cypherSyze], cypherWords);
            newWord = 0;
            cypherSyze++;
        }

        cypherWords = strtok(NULL, delimiter);
    }   

    /*
        Decypher/Cypher Text

    */

    //copy original text to cyphered
    char *cyphered = malloc( strlen(text) + 1 );
    strcpy(cyphered, text);
     

    //cypher text
    for(int i = 0; i < cypherSyze; i++) {
        cyphered = cypher(cyphered, cypherOld[i], cypherNew[i]);
    }

    //uncypher text
    if(strcmp(cyphered, text) == 0){
        for(int i = 0; i < cypherSyze; i++) {
        cyphered = cypher(cyphered, cypherNew[i], cypherOld[i]);
        }
    }
        // Sent result to parent
    write(fdpai[WRITE_END], cyphered, BUFFER_SIZE);
    close(fdpai[WRITE_END]);
    }

    return 0;
}