#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

#define LIM 100


int main(int argc, char *argv[]) {


    char *fileName1;
    char *fileName2;
    int Matriz1[LIM][LIM];
    int Matriz2[LIM][LIM];

    //only print number of phrases
    if (argc == 3) {
        fileName1 = malloc(strlen(argv[1]) + 1);
        strcpy(fileName1, argv[1]);
        fileName2 = malloc(strlen(argv[2]) + 1);
        strcpy(fileName2, argv[2]);
    }

        //Wrong input
    else {
        printf("Wrong input !!\n");
        return -1;
    }



    //File1
    FILE *file1;
    file1 = fopen(fileName1, "r");
    if (file1 == NULL) {
        printf("file 1 could not be opened\n");
        return -1;
    }

    //File 2
    FILE *file2;
    file2 = fopen(fileName2, "r");
    if (file2 == NULL) {
        printf("file 2 could not be opened\n");
        return -1;
    }


    int N = 0; //linhas
    int M = 0; //colunas
    int N1 = 0; // linhas Matriz1
    int M1 = 0; // colunas Matriz1
    char vezes; // caracter "X"
    fscanf(file1, "%d %c %d\n", &N1, &vezes, &M1);
    int N2 = 0;//linhas Matrix 2
    int M2 = 0;//colunas Matrix 2
    fscanf(file2, "%d %c %d\n", &N2, &vezes, &M2);

    if (N1 != N2 || M1 != M2) {
        printf("Wrong Matrix\n");
        return -1;
    } else {
        N = N1;
        M = M1;

    }

    int *ptr1 = mmap(NULL, ((N * M)) * sizeof(int),
                     PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (ptr1 == MAP_FAILED) {
        printf("Map1 failled\n");
    }
    int *ptr2 = mmap(NULL, ((N * M)) * sizeof(int),
                     PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (ptr2 == MAP_FAILED) {
        printf("Map2 failled\n");
    }
    int *ptr3 = mmap(NULL, ((N * M)) * sizeof(int),
                     PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (ptr3 == MAP_FAILED) {
        printf("Map3 failled\n");
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            fscanf(file1, "%d", &Matriz1[i][j]);
    fclose(file1);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            fscanf(file2, "%d", &Matriz2[i][j]);
    fclose(file2);


    int w = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            ptr1[w] = Matriz1[i][j];
            w++;
        }
    }
    w = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            ptr2[w] = Matriz2[i][j];
            w++;
        }
    }


    for (int i = 0; i < M; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            int j = i;
            while (j < N * M) {
                ptr3[j] = ptr1[j] + ptr2[j];
                j += M;
            }
        } else {
            waitpid(pid,NULL,0);
            exit(0);
        }
    }

    printf("%d %c %d \n", N, vezes, M);
    w = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("%d ", ptr3[w]);
            w++;
        }
        printf("\n");
    }


    return 0;
}
