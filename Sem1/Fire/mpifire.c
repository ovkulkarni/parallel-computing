#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#define WIDTH 200
#define HEIGHT 150
#define TREE 'T'
#define FIRE 'F'

typedef struct F {
    int row;
    int col;
} Fire;


void printforest(char f[WIDTH][HEIGHT]){
    printf("/");
    for(int i = 0; i < WIDTH; i++){
        printf("-");
    }
    printf("\\\n");
    for(int i = 0; i < WIDTH; i++){
        printf("|");
        for(int j = 0; j < WIDTH; j++){
            printf("%c", f[i][j]);
        }
        printf("|\n");
    }
    printf("\\");
    for(int i = 0; i < WIDTH; i++){
        printf("-");
    }
    printf("/\n");
}

void fillforest(char f[WIDTH][HEIGHT], float threshold){
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            if((rand() % 1000) / 1000.0 < threshold){
                f[i][j] = TREE;
            }
            else {
                f[i][j] = ' ';
            }
        }
    }
}


float lightforest(char f[WIDTH][HEIGHT]){
    Fire* currfire[WIDTH * HEIGHT] = { NULL };
    Fire* newfire[WIDTH * HEIGHT] = { NULL };
    int index = 0;
    for(int i = 0; i < HEIGHT; i++){
        if(f[i][0] == TREE){
            f[i][0] = FIRE;
            Fire* new = (Fire*)malloc(sizeof(Fire));
            new -> row = i;
            new -> col = 0;
            currfire[index] = new;
            index++;
        }
    }

    int steps = 0;
    while(index != 0){
        index = 0;
        int newindex = 0;
        while(currfire[index] != NULL){
            Fire* curr = currfire[index];
            if((curr -> row) - 1 > -1){
                if(f[curr -> row - 1][curr -> col] == TREE){
                    Fire* new = (Fire*)malloc(sizeof(Fire));
                    f[curr -> row - 1][curr -> col] = FIRE;
                    new -> row = curr -> row - 1;
                    new -> col = curr -> col;
                    newfire[newindex] = new;
                    newindex++;
                }
            }
            if((curr -> row) + 1 < WIDTH){
                if(f[curr -> row + 1][curr -> col] == TREE){
                    Fire* new = (Fire*)malloc(sizeof(Fire));
                    f[curr -> row + 1][curr -> col] = FIRE;
                    new -> row = curr -> row + 1;
                    new -> col = curr -> col;
                    newfire[newindex] = new;
                    newindex++;
                }
            }
            if((curr -> col) - 1 > -1){
                if(f[curr -> row][curr -> col - 1] == TREE){
                    Fire* new = (Fire*)malloc(sizeof(Fire));
                    f[curr -> row][curr -> col - 1] = FIRE;
                    new -> row = curr -> row;
                    new -> col = curr -> col - 1;
                    newfire[newindex] = new;
                    newindex++;
                }
            }
            if((curr -> col) + 1 < HEIGHT){
                if(f[curr -> row][curr -> col + 1] == TREE){
                    Fire* new = (Fire*)malloc(sizeof(Fire));
                    f[curr -> row][curr -> col + 1] = FIRE;
                    new -> row = curr -> row;
                    new -> col = curr -> col + 1;
                    newfire[newindex] = new;
                    newindex++;
                }
            }
            index++;
        }
        if(index != 0){
            steps++;
            int z = 0;
            while(currfire[z] != NULL){
                free(currfire[z]);
                z++;
            }
            memcpy(currfire, newfire, sizeof(newfire));
            Fire* newerfire[WIDTH * HEIGHT] = { NULL };
            memcpy(newfire, newerfire, sizeof(newerfire));
        }
    }
    // printforest(f);
    return (steps * 1.0) / WIDTH;
}

float run_probability(float prob){
    char forest[WIDTH][HEIGHT];
    float total_steps = 0.0;
    float avg_steps = 0.0;
    float avgs[1000] = {0};
    int j = 0;
    for(j = 1; j < 1000; j++){
        fillforest(forest, prob);
        total_steps += lightforest(forest);
        avg_steps = total_steps / j;
        avgs[j] = avg_steps;
        int x = 10;
        for(x = 10; x > 0; x--){
            if(fabsf(avgs[j-1] - avgs[j-1-x]) > 0.001)
                break;
        }
        if(x == 0)
            break;
    }
    return avg_steps;
}

int main(int argc, char* argv[]){
    int rank;
    int size;
    MPI_Status status;
    int tag = 0;
    int rseed;
    float stop = -11111.0f;
    rseed = time( NULL );
    srand(rseed);
    MPI_Init(      &argc          , &argv );
    MPI_Comm_size( MPI_COMM_WORLD , &size );
    MPI_Comm_rank( MPI_COMM_WORLD , &rank );
    if(rank == 0){
        float c = 0.01f;
        float result[2];
        float p = 0.0f;
        int s = 0;
        int r = 0;
        float end = 1.001f;
        for(int i = 1; i < size; i++){
            MPI_Send(&p, 1, MPI_FLOAT, i, tag, MPI_COMM_WORLD);
            s++;
            p += c;
        }
        while(p <= end || s > r){
            MPI_Recv(result, 2, MPI_FLOAT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            r++;
            int x = status.MPI_SOURCE;
            printf("%f\t%f\n", result[0], result[1]);
            if(p <= end) {
                MPI_Send(&p, 1, MPI_FLOAT, x, tag, MPI_COMM_WORLD);
                s++;
                p += c;
            }
        }
        for(int i = 0; i < size; i++){
            MPI_Send(&stop, 1, MPI_FLOAT, i, tag, MPI_COMM_WORLD);
        }
    }
    else {
        float ret[2];
        while(1){
            MPI_Recv(&ret[0], 1, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);
            if(ret[0] == stop){
                break;
            }
            ret[1] = run_probability(ret[0]);
            MPI_Send(ret, 2, MPI_FLOAT, 0, tag, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}
