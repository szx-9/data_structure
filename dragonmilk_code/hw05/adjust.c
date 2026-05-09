#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node
{
    int Q,height,s[10];
};
typedef struct node nd;

int head = 0,tail = 0,ans_cnt = 0;
int que[10000],prt[10000],ans[10000],t[10];
nd *map[10000];

nd* build(int id,int s[])
{
    if(map[id] == NULL) map[id] = (nd*) malloc(sizeof(nd));
    int i = 0 , tg = 0;
    for(i = 0 ; s[i] != -1 ; ++i) if(s[i] < 100) tg = 1;
    for(i = 0 ; s[i] != -1 ; ++i)
    {
        if(map[s[i]] == NULL)
        {
            t[0]=t[1]=t[2]=t[3]=-1;
            map[s[i]] = build(s[i],t);
        }
        map[id]->s[i] = s[i];
        map[s[i]]->height = map[id]->height+tg;
    }
    map[id]->s[i] = -1;
    return map[id];
}

void push(int id){que[tail++] = id;}
int pop()
{
    if(head >= tail) return -1;
    head++;
    return que[head-1];
}

int main()
{
    int i,id,Q,cnt = 0,s[4];
    while(scanf("%d",&id) != EOF && id != -1)
    {
        cnt = 0 ;
        while(scanf("%d",&s[cnt]) != EOF && s[cnt++] != -1);
        build(id,s);
    }
    cnt = 0;
    while(scanf("%d %d",&id,&Q) != EOF) 
    {
        map[id]->Q = Q;
        prt[cnt++] = id;
        for(i = cnt-1 ; i > 0 ; --i)
        {
            if((Q > map[prt[i-1]]->Q) || (Q == map[prt[i-1]]->Q && id < prt[i-1])) prt[i] = prt[i-1] , prt[i-1] = id;
            else break;
        }
    }
    push(100);
    int r;
    while((r = pop()) != -1)
    {
        if(r < 100) ans[ans_cnt++] = r;
        for(i = 0 ; map[r]->s[i] != -1 ; ++i)
            if(map[r]->s[i] <  100) push(map[r]->s[i]);
        for(i = 0 ; map[r]->s[i] != -1 ; ++i)
            if(map[r]->s[i] >= 100) push(map[r]->s[i]);
    }

    for(i = 0 ; i < ans_cnt ; ++i) printf("%d->%d\n",prt[i],ans[i]);
    return 0;
}