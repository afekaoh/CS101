#include <string.h>
#include <stdio.h>


#define SIZE 8



void printBorad(char *arr[SIZE],int rows){
    for (int i = 0; i < rows; ++i) {
        int len=(int)strlen(arr[i]);
        printf("%s",arr[i]);
//        for (int j = 0; j <len ; ++j) {
//            printf("|%c|",arr[i][j]);
//        }
//        printf("\n");
        }
    }


typedef struct {
    char srcPiece;
    char srcRow;
    char srcCol;
    int srcI;
    int srcJ;
    char destPiece;
    char destRow;
    char destCol;
    int destI;
    int destJ;
}Move;

void creatBorad( char *fem, const char *delim,char *borad[SIZE]) {
    int i = 0;
    char *p = strtok (fem, delim);
    char *arr[SIZE];
    while (p != NULL)
    {
        arr[i++] = p;
        p = strtok (NULL, "/");
    }
    for (i = 0; i <SIZE ;i++) {
        sprintf(borad[i],"%s", arr[i]);
    }
}

int main() {
    char fem[] ="hello/world/Hi/there/a/a/a/a/a";
    char delim[]="/";
    char *borad[SIZE];
    creatBorad(fem, delim,borad);

printBorad(borad,SIZE);
    return 0;
    }
