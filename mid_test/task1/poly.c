#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long
/*
真实头节点，需要每次插入时返回头结点，
虚拟头结点：全局设置dummy（需要特殊标识），每次传入dummy，然后对dummynext进行操作，

*/
typedef struct node
{
    int coff;  // 系数
    int index; // 指数
    struct node *next;
} node;

int cnt = 0;

node *buildNode(int coff, int index)
{
    node *cur = (node *)malloc(sizeof(node));
    cur->next = NULL;
    cur->coff = coff;
    cur->index = index;
    return cur;
}

node *insert(node *head, node *cur)
{
    // 将cur插入链表，并返回头节点
    if (head == NULL)
    {
        return cur;
    }
    node *front = NULL;
    node *move = head;
    while (move != NULL)
    {
        cnt++;
        if (cur->index > move->index)
        {
            // 将cur插入到move前
            if (front == NULL)
            {
                cur->next = move;
                return cur;
            }
            front->next = cur;
            cur->next = move;
            return head;
        }
        else if (cur->index == move->index)
        {
            move->coff += cur->coff;
            if (move->coff == 0)
            {
                if (front == NULL)
                {
                    return move->next;
                }
                front->next = move->next;
            }
            return head;
        }
        front = move;
        move = move->next;
    }
    // 此时遍历到末尾
    front->next = cur;
    return head;
}

void printans(node *head)
{
    if (head == NULL)
    {
        printf("ERROR_PRINT\n");
        return;
    }
    node *cur = head;
    while (cur != NULL)
    {
        if (cur->coff)
        {
            printf("%d %d ", cur->coff, cur->index);
        }
        cur = cur->next;
    }
    printf("%d", cnt);
    return;
}

int main()
{
    int coff = 0, index = 0;
    node *head = NULL;
    while (scanf(" %d %d", &coff, &index))
    {
        if (coff == 0 && index == -1)
        {
            break;
        }
        head = insert(head, buildNode(coff, index));
    }
    printans(head);
    return 0;
}