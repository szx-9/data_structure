#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>

// 生成Type类型，名为 VecName 的动态数组
#define DEFINE_VECTOR(Type, VecName)                                           \
    typedef struct                                                             \
    {                                                                          \
        Type *data;   /* 元素存储区 */                                         \
        int size;     /* 当前元素个数 */                                       \
        int capacity; /* 已分配容量 */                                         \
    } VecName;                                                                 \
                                                                               \
    /* 初始化：分配初始容量 init_cap。malloc 失败时 capacity 置 0 */           \
    static inline void VecName##_init(VecName *v, int init_cap)                \
    {                                                                          \
        v->data = (Type *)malloc(init_cap * sizeof(Type));                     \
        v->size = 0;                                                           \
        v->capacity = (v->data) ? init_cap : 0;                                \
    }                                                                          \
                                                                               \
    /* 尾部插入，自动扩容（容量翻倍） */                                       \
    static inline void VecName##_push_back(VecName *v, Type value)             \
    {                                                                          \
        if (v->size == v->capacity)                                            \
        {                                                                      \
            v->capacity = (v->capacity == 0) ? 1 : v->capacity * 2;            \
            Type *tmp = (Type *)realloc(v->data, v->capacity * sizeof(Type));  \
            if (tmp)                                                           \
                v->data = tmp;                                                 \
        }                                                                      \
        if (v->size < v->capacity)                                             \
            v->data[v->size++] = value;                                        \
    }                                                                          \
                                                                               \
    /* 获取当前元素个数 */                                                     \
    static inline int VecName##_size(const VecName *v)                         \
    {                                                                          \
        return v->size;                                                        \
    }                                                                          \
                                                                               \
    /* 释放 data 数组。若 Type 内含指向堆的指针，调用者需先遍历元素手动清理 */ \
    static inline void VecName##_free(VecName *v)                              \
    {                                                                          \
        free(v->data);                                                         \
        v->data = NULL;                                                        \
        v->size = v->capacity = 0;                                             \
    }

int main()
{

    return 0;
}