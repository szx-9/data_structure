#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#define STOP_LIST_INIT_SIZE 1000 // 停用表初始size
#define MAX_WORD_LEN 64
#define BLOCK_SIZE 4096
/*==全局量==*/
double alpha = 0; // 比例系数
int curId = 0;

/*==全局文件指针==*/
FILE *f_stop;
FILE *f_doc;

/*==全局缓冲区==*/

/*==数据结构容器==*/
/*=停用词动态数组及方法=*/
typedef struct stop_list
{
    char **list;
    int size, capacity;
} vector_stop;
vector_stop stop_list; // 实例化

void stop_list_init()
{
    stop_list.size = 0;
    stop_list.capacity = STOP_LIST_INIT_SIZE;
    stop_list.list = (char **)malloc(sizeof(char *) * stop_list.capacity);
    if (stop_list.list == NULL)
    {
        printf("ERROR_MALLOC_STOP_LIST\n");
        exit(1);
    }
    for (int i = 0; i < stop_list.capacity; ++i)
    {
        stop_list.list[i] = (char *)malloc(sizeof(char) * MAX_WORD_LEN);
        if (stop_list.list[i] == NULL)
        {
            printf("ERROR_MALLOC_STOP_WORD\n");
            exit(1);
        }
    }
}
void stop_list_insert(FILE *f_stop)
{
    while (1)
    {
        if (stop_list.size >= stop_list.capacity)
        {
            stop_list.capacity *= 2;
            char **tmp = (char **)realloc(stop_list.list, sizeof(char *) * stop_list.capacity);
            if (tmp == NULL)
            {
                printf("ERROR_REALLOC_STOP\n");
                exit(1);
            }
            stop_list.list = tmp;
            for (int i = stop_list.size; i < stop_list.capacity; ++i)
            {
                stop_list.list[i] = (char *)malloc(sizeof(char) * MAX_WORD_LEN);
                if (stop_list.list[i] == NULL)
                {
                    printf("ERROR_MALLOC_STOP_WORD\n");
                    exit(1);
                }
            }
        }
        if (fscanf(f_stop, " %63s", stop_list.list[stop_list.size]) != 1)
            break;
        stop_list.size++;
    }
}
/*==忽略大小写的字符串比较==*/
int stricmp_custom(const char *s1, const char *s2)
{
    while (*s1 && *s2)
    {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2)
            return c1 - c2;
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

/*==bsearch比较函数，配合 stricmp_custom==*/
int bsearch_cmp(const void *a, const void *b)
{
    const char *sa = *(const char **)a;
    const char *sb = *(const char **)b;
    return stricmp_custom(sa, sb);
}

/*==句子信息结构体==*/
typedef struct pair // 语义稀疏向量对
{
    int id;  // 单词id
    int cnt; // 单词句中个数
} pair;
typedef struct sentence // 句子元信息
{
    int id;                 // 句子id
    int word_cnt;           // 单词总数
    int char_cnt;           // 字母有效字符个数
    int cap_cnt, punct_cnt; // 大写字母，标点统计
    double vowel_rate_sum;  // 元音率总和
    double var_mean;        // 当前均值
    double M2;              // 偏差累积量
    double structure[7];    // 结构向量
    pair *semantic;         // 语义向量，具体长度根据partial hash分配
    struct sentence *next;  // 内置链表指向下一个
} sentence_info;
sentence_info *sentence_cur = NULL; // 句子元信息头结点

void sentence_create()
{
    sentence_info *nS = (sentence_info *)malloc(sizeof(sentence_info));
    if (nS == NULL)
    {
        printf("ERROR_MALLOC_SENTENCE\n");
        exit(1);
    }
    nS->id = curId++;
    nS->word_cnt = nS->char_cnt = nS->cap_cnt = nS->punct_cnt = 0;
    nS->vowel_rate_sum = nS->var_mean = nS->M2 = 0;
    nS->semantic = NULL;
    nS->next = sentence_cur;
    sentence_cur = nS;
}

/*==词池容器==*/
typedef struct blockList
{
    char *head;         // 头结点
    int size, capacity; // 通过size进行遍历
    struct blockList *next;
} blockList;
blockList *pool_cur = NULL; // 实例化词池

void pool_create()
{
    blockList *newPool = (blockList *)malloc(sizeof(blockList));
    if (newPool == NULL)
    {
        printf("ERROR_MALLOC_POOL\n");
        exit(1);
    }
    newPool->size = 0;
    newPool->capacity = BLOCK_SIZE;
    newPool->head = (char *)malloc(sizeof(char) * newPool->capacity);
    if (newPool->head == NULL)
    {
        printf("ERROR_MALLOC_POOL_HEAD\n");
        exit(1);
    }
    newPool->next = pool_cur;
    pool_cur = newPool;
}

char *pool_insert(const char *string, int len) // 插入一个有效单词，转小写存入，返回写入位置
{ 
    if (pool_cur == NULL)
        pool_create(); // 注意这里安全性，这样可以不进行初始化，便于统一性
    if (pool_cur->size + len >= pool_cur->capacity)
        pool_create();
    char *dest = pool_cur->head + pool_cur->size;
    int i = 0;
    do {
        dest[i] = tolower((unsigned char)string[i]);
        i++;
    } while (string[i - 1] != '\0');
    pool_cur->size += i;
    return dest;
}

int main()
{

    return 0;
}