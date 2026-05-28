#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long
#define Q_SIZE 128
#define NORMAL 1
#define DEL 2
/*

*/
int vertex = 0, edge = 0, del = 0;
int matrix[105][105];
int visited[105];
int path[105];
int Mindex = 0;
int que[Q_SIZE];
int head = 1, rear = 0,size=0;
int have_edge(int a, int b)
{
    return matrix[a][b] && matrix[b][a];
}
void depth_search(int cur_id,int type)
{
    path[Mindex++] = cur_id;
    visited[cur_id] = 1;
    for (int i = 0; i < vertex; ++i)
    {
        if(type==DEL&&i==del)
            continue;
        if (have_edge(cur_id, i)&&visited[i]!=1)
        {
            depth_search(i,type);
        }
    }
}
void enqueue(int id){
    rear=(rear+1)&(Q_SIZE-1);
    que[rear] = id;
    size++;
    visited[id] = 1;
}
void dequeue(){
    if(size<=0)
        printf("ERROR_DEQUEUE\n"), exit(1);
    que[head] = 0;
    head=(head+1)%Q_SIZE;
    size--;
}
void width_search(int type){
    while(size!=0){
        int cur_id = que[head];
        for (int i = 0; i < vertex;++i){
            if(type==DEL&&i==del)
                continue;
            if(have_edge(cur_id,i)&&visited[i]!=1){
                enqueue(i);
            }
        }
        path[Mindex++] = cur_id;
        dequeue();
    }
}
void reset_visit(){
    for (int i = 0; i < vertex;++i){
        visited[i] = 0;
    }
}
void Print(){
    for (int i = 0; i < Mindex;++i){
        printf("%d ", path[i]);
    }
    printf("\n");
}
int main()
{
    scanf("%d%d", &vertex, &edge);
    for (int i = 0; i < edge; ++i)
    {
        int a = 0, b = 0;
        scanf("%d%d", &a, &b);
        matrix[a][b] = 1;
        matrix[b][a] = 1;
    }
    scanf("%d", &del);
    // deep-first
    depth_search(0,NORMAL);
    Print();
    Mindex = 0;
    reset_visit();

    //width_search
    enqueue(0);
    width_search(NORMAL);
    Print();
    Mindex = 0;
    reset_visit();

    depth_search(0, DEL);
    Print();
    Mindex = 0;
    reset_visit();


    rear = 0, head = 1, size = 0;
    enqueue(0);
    width_search(DEL);
    Print();

    return 0;
}