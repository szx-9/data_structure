#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#define STOP_LIST_INIT_SIZE 1000//停用表初始size
#define MAX_WORD_LEN 64

/*==全局量==*/
double alpha = 0;//比例系数

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
vector_stop stop_list;//实例化

void stop_list_init(){
    stop_list.size = 0;
    stop_list.capacity = STOP_LIST_INIT_SIZE;
    stop_list.list = (char **)malloc(sizeof(char *) * stop_list.capacity);
    if (stop_list.list == NULL) {
        printf("ERROR_MALLOC_STOP_LIST\n");
        exit(1);
    }
    for (int i = 0; i < stop_list.capacity; ++i) {
        stop_list.list[i] = (char *)malloc(sizeof(char) * MAX_WORD_LEN);
        if (stop_list.list[i] == NULL) {
            printf("ERROR_MALLOC_STOP_WORD\n");
            exit(1);
        }
    }
}
void stop_list_insert(FILE* f_stop){
    while (1) {
        if (stop_list.size >= stop_list.capacity) {
            stop_list.capacity *= 2;
            char **tmp = (char **)realloc(stop_list.list, sizeof(char *) * stop_list.capacity);
            if (tmp == NULL) {
                printf("ERROR_REALLOC_STOP\n");
                exit(1);
            }
            stop_list.list = tmp;
            for (int i = stop_list.size; i < stop_list.capacity; ++i) {
                stop_list.list[i] = (char *)malloc(sizeof(char) * MAX_WORD_LEN);
                if (stop_list.list[i] == NULL) {
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
int stricmp_custom(const char *s1, const char *s2) {
    while (*s1 && *s2) {
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
int bsearch_cmp(const void *a, const void *b) {
    const char *sa = *(const char **)a;
    const char *sb = *(const char **)b;
    return stricmp_custom(sa, sb);
}

int main()
{

    return 0;
}