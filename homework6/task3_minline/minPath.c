#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#define ll long long
/*
prim算法核心：三步
1找到当前非树节点中到树的距离最近的节点node
2将node加入到visited
3更新所有与node联通结点的mst(没有联通的不要，因为没有变化)
*/
typedef struct info{
    int id, weight;
} info;

int vertex = 0, edge = 0;
info **matirx=NULL;
info *mst = NULL;//维护各个节点到生成树的最短距离与编号
int *visited = NULL;//维护所有已经在树中节点
int *ans = NULL;
int ansIndex = 0;
int sum = 0;

void applyMemory(int vertex){
    matirx = (info **)malloc(sizeof(info *) * vertex);
    for (int i = 0; i < vertex;++i){
        matirx[i] = (info *)calloc(vertex,sizeof(info));//初始化为零
    }
}
int findNode(){
    //找到当前未访问结点中，距离最近的
    int n_id = -1;
    int min_len = INT_MAX;
    for (int i = 0; i < vertex;++i){
        if(visited[i])
            continue;
        if(mst[i].weight<=min_len){
            n_id = i;
            min_len = mst[i].weight;
        }
    }
    return n_id;
}
void renewMst(int n_id){
    info *renew_array = matirx[n_id];
    for (int i = 0; i < vertex;++i){
        if(renew_array[i].weight==0&&renew_array[i].id==0)
            continue;
        if(renew_array[i].weight<mst[i].weight)
            mst[i].weight = renew_array[i].weight,mst[i].id=renew_array[i].id;
    }
}
int cmp(const void* a,const void * b){
    return *(ll*)a - *(ll*)b;
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
    mst = (info *)malloc(vertex*sizeof(info));//这里的类型
    ans=(int*)malloc((vertex-1)*sizeof(int));
    visited = (int *)calloc(vertex,sizeof(int));
    for (int i = 0; i < vertex;++i){
        mst[i] .weight= INT_MAX;
        mst[i].id = -1;
    }
    for (int i = 0; i < vertex;++i){
        //此处考虑零，所以需要进行vertex次，每次会将一个加入最小生成树，初始有一个结点
        int n_id = findNode();
        if(i)ans[ansIndex++] = mst[n_id].id,sum+=mst[n_id].weight;
        visited[n_id] = 1;
        renewMst(n_id);
    }
    qsort(ans, vertex - 1, sizeof(int), cmp);
    printf("%d\n", sum);
    for (int i = 0; i < ansIndex;++i){
        printf("%d ", ans[i]);
    }
    return 0;
}