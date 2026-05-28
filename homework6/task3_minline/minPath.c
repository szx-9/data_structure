#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long
/*

*/
typedef struct info{
    int id, weight;
} info;
int vertex = 0, edge = 0;
info **matirx=NULL;
void applyMemory(int vertex){
    matirx = (info **)malloc(sizeof(info *) * vertex);
    for (int i = 0; i < vertex;++i){
        matirx[i] = (info *)malloc(sizeof(info) * vertex);
    }
}

int main()
{
    scanf("%d%d", &vertex, &edge);
    applyMemory(vertex);
    for (int i = 0; i < edge;++i){
        int e_id = 0, weight = 0, n1_id = 0, n2_id = 0;
        scanf("%d%d%d%d", &e_id, &n1_id, &n2_id, &weight);
        matirx[n2_id][n1_id].id=matirx[n1_id][n2_id].id = e_id;
        matirx[n2_id][n1_id].weight=matirx[n1_id][n2_id].weight = weight;
    }
    //prim


    return 0;
}