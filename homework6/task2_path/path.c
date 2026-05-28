#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long
#define SIZE 1005
/*

*/
typedef struct node{
    int e_id, n_id;
    struct node *next;
} node;
typedef struct info{
    node *head;
} info;

int vertex = 0, edge = 0;
info v_array[SIZE];//邻接表头vertex
int visited[SIZE];
int path[SIZE];
int mindex = 0;
void Print(int mindex){
    for (int i = 0; i < mindex;++i)
        printf("%d ", path[i]);
    printf("\n");
}
int is_access(node* cur){
    return !visited[cur->n_id];
}
void depth_search(int cur_id,int index){
    if(cur_id==vertex-1){
        Print(index);
        return;
    }
    info cur_info = v_array[cur_id];
    node *cur = cur_info.head;
    while(cur){
        if(is_access(cur)){
            path[index] = cur->e_id;//注意index逻辑，局部or全局，这里不要轻易执行自增
            visited[cur->n_id]=1;
            depth_search(cur->n_id, index+1);
            visited[cur->n_id] = 0;
        }
        cur = cur->next;
    }
}
node* buildNode(int e_id,int n_id){
    node *cur = (node *)malloc(sizeof(node));
    cur->next = NULL;
    cur->e_id = e_id;
    cur->n_id = n_id;
    return cur;
}
void insert_info(int n_id,node* inNode){
    if(v_array[n_id].head==NULL){
        v_array[n_id].head = inNode;
        return;
    }
    node *cur = v_array[n_id].head,*front=NULL;
    while(cur!=NULL){
        if(inNode->e_id<cur->e_id){
            if(front==NULL){
                inNode->next = cur;
                v_array[n_id].head = inNode;
                return;
            }
            front->next = inNode;
            inNode->next = cur;
            return;
        }
        front = cur;
        cur = cur->next;
    }
    front->next = inNode;
}
int main()
{
    scanf("%d%d", &vertex, &edge);
    for (int i = 0; i < edge;++i){
        int e_id = 0;
        int n1 = 0, n2 = 0;
        scanf("%d%d%d", &e_id, &n1, &n2);
        insert_info(n1, buildNode(e_id, n2));
        insert_info(n2, buildNode(e_id, n1));
    }
    visited[0] = 1;
    depth_search(0, mindex);
    return 0;
}