#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#define ll long long
#define MAX 50
#define MAX_NAME 32
#define MAX_EDGE 15
#define MAX_LINE 25
#define MAX_NODE 50
#define MAX_SIZE_ANS 100
/*
读取信息：
    行读取：利用sscanf读取站台名和是否为换乘

*/
typedef struct edge_info edge;
typedef struct node_info node;

struct edge_info
{
    char weight;
    node *l_node;
} ;
struct node_info
{
    int n_id, l_id, isTra;  // 结点id，线路id，是否换乘
    int mst, visited;       // 最短距离，是否访问
    struct node_info *prev; // 前驱结点
    char name[MAX_NAME];
    edge link[MAX_EDGE];
    int index; // link的容量
} ;

// 全局指针
node *Start = NULL, *End = NULL;               // 起点，终a点
char start_name[MAX_NAME], end_name[MAX_NAME]; // 存储起点重点名字
node line_map[MAX_LINE][MAX_NODE];             // 线路数组
FILE *fin = NULL;                              //

char buf[MAX];          // 文件读取缓冲区
int stations[MAX_LINE]; // 每个线路有多少站点
node *path[MAX_SIZE_ANS];
int p_index = 0;

// 全局变量
int line_num = -1; // 线路个数
int n_id = 0;      // 全局结点id,同时记录数量

void reverse(int size){
    int l = 0, r = size - 1;
    while(l<r){
        node *tmp = path[l];
        path[l] = path[r];
        path[r] = tmp;
        l++, r--;
    }
}
void insert_edge(node *cur_node, int weight, node *inserted)
{ // 在cur结点中插入inserted
    int index = cur_node->index;
    cur_node->link[index].weight = weight;
    cur_node->link[index].l_node = inserted;
    (cur_node->index)++;
}
void dealTran(node *target, int row, int col)
{ // 在给定范围内检索目标换乘站，并进行插入，权重为0
    const char *target_name = target->name;
    for (int i = 0; i <= row; ++i)
    {
        int cur_col = i == row ? col : stations[i];
        for (int j = 0; j < cur_col; ++j)
        {
            if (!strcmp(line_map[i][j].name, target_name) && line_map[i][j].isTra)
            {
                insert_edge(&line_map[i][j], 0, target);
                insert_edge(target, 0, &line_map[i][j]);
            }
        }
    }
}
node *find_closest(int row)
{
    // 找到当前结点中距离起点最近的，未访问结点，返回地址,外部标记visited & 判断是否是end
    node *ret = NULL;
    int min_val = INT_MAX;
    for (int i = 0; i < row; ++i)
    {
        int col = stations[i];
        for (int j = 0; j < col; ++j)
        {
            if (!line_map[i][j].visited && line_map[i][j].mst <= min_val)
            {
                min_val = line_map[i][j].mst;
                ret = &line_map[i][j];
            }
        }
    }
    if(ret==NULL){
        printf("ERROR_FIND");
        exit(1);
    }
    return ret;
}
void renew(node *cur)
{
    // 更新与当前cur结点相连的所有结点mst和prev
    if(cur->mst==INT_MAX){
        printf("ERROR_RENEW\n");
        exit(1);
    }
    for (int i = 0; i < cur->index; ++i)
    {
        edge* cur_edge = &cur->link[i];
        node* cur_node = cur_edge->l_node;
        if(!cur_edge->l_node->visited&&cur_node->mst>cur_edge->weight+cur->mst){
            //如果当前结点未访问&&最小距离大于本轮结点的mst+两者之间的权重
            cur_node->mst = cur_edge->weight + cur->mst;
            cur_node->prev = cur;
        }
    }
}
void Print(int size){
    printf("%s", Start->name);
    int cur_line = Start->l_id;
    int st_cnt = 0;
    for (int i = 0; i < size;++i){
        if(path[i]->l_id!=cur_line){
            //当前结点线路不是原线路
            printf("-%d(%d)-%s",  cur_line, st_cnt,path[i]->name);
            cur_line = path[i]->l_id;
            st_cnt = 0;
        }
        else{
            st_cnt++;
        }
    }
    if(st_cnt){
        printf("-%d(%d)-%s", cur_line, st_cnt, End->name);
    }
}


int main()
{
    scanf(" %s %s", start_name, end_name);
    fin = fopen("bgstations.txt", "r");

    fgets(buf, sizeof(buf), fin);
    sscanf(buf, " %d", &line_num);

    for (int i = 0; i < line_num; ++i)
    {
        int l_id = -1, st_num = 0; // 当前线路的id，总站台数
        fgets(buf, sizeof(buf), fin);
        sscanf(buf, " %d %d", &l_id, &st_num);
        stations[l_id] = st_num;
        for (int j = 0; j < st_num; ++j)
        { // 额外处理最后一次
            // 进入本轮线路读取，j作为index
            fgets(buf, sizeof(buf), fin);
            sscanf(buf, " %s %d", line_map[l_id][j].name, &line_map[l_id][j].isTra);
            if (j == st_num - 1 && !strcmp(line_map[l_id][j].name, line_map[l_id][0].name))
            {
                // 环线
                insert_edge(&line_map[l_id][j - 1], 1, &line_map[l_id][0]);
                insert_edge(&line_map[l_id][0], 1, &line_map[l_id][j - 1]);
                line_map[l_id][j].l_id = 0;
                break;
            }
            if (!strcmp(line_map[l_id][j].name, start_name))
            { // 需避开if语句，防止将废弃数组位置作为开头
                Start = &line_map[l_id][j];
            }
            if (!strcmp(line_map[l_id][j].name, end_name))
            {
                End = &line_map[l_id][j];
            }
            line_map[l_id][j].n_id = n_id++;
            line_map[l_id][j].l_id = l_id;
            line_map[l_id][j].index = 0;
            line_map[l_id][j].mst = INT_MAX;
            line_map[l_id][j].visited = 0; // 没有访问过、
            line_map[l_id][j].prev = NULL;
            if (j > 0)
            { // 除头结点，执行对前一个和当前结点的edge插入
                insert_edge(&line_map[l_id][j - 1], 1, &line_map[l_id][j]);
                insert_edge(&line_map[l_id][j], 1, &line_map[l_id][j - 1]);
            }
            if (line_map[l_id][j].isTra)
            { // 如果是换乘,对所有换乘站直接edge插入
                dealTran(&line_map[l_id][j], l_id, j);
            }
        }
    }
    Start->mst = 0;//此时只有起点到自己距离是0，其余都是INT_MAX
    while(1){
        node* cur_ori_node = find_closest(MAX_LINE);
        cur_ori_node->visited = 1;
        renew(cur_ori_node);
        if (cur_ori_node==End)
        {//当前找到最短结点是终点
            break;
        }
    }
    //从终点开始输入
    node *res = End;
    while(res!=Start){
        if (res == NULL)
        {
            printf("ERROR_PATH\n");
            exit(1);
        }
        path[p_index++] = res;
        res = res->prev;
    }
    reverse(p_index);
    Print(p_index);

    fclose(fin);
    return 0;
}