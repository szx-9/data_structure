#include <stdio.h>
#include <stdlib.h>

struct ListNode
{
    int val;
    struct ListNode *next;
};

struct ListNode *reverseRange(struct ListNode *head, int start, int end)
{
    struct ListNode dummy;
    dummy.next = head;
    struct ListNode *prev = &dummy,*cur=head,*nex=head->next,*fir=NULL,*sec=NULL;
    int cnt = 1;
    while(cnt<=end){
        if(cnt==start){
            fir = prev;
            sec = cur;
        }
        if(cnt>start){
            cur->next = prev;
        }
        prev = cur;
        cur = nex;
        if(nex){
            nex = nex->next;
        }
        cnt++;
    }
    fir->next = prev;
    sec->next = cur;
    return dummy.next;
}

// 创建链表
struct ListNode *createList(int arr[], int n)
{
    struct ListNode *head = NULL, *tail = NULL;
    for (int i = 0; i < n; i++)
    {
        struct ListNode *node = (struct ListNode *)malloc(sizeof(struct ListNode));
        node->val = arr[i];
        node->next = NULL;
        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
    }
    return head;
}

// 打印链表
void printList(struct ListNode *head)
{
    while (head)
    {
        printf("%d ", head->val);
        head = head->next;
    }
    printf("\n");
}

// 释放链表
void freeList(struct ListNode *head)
{
    while (head)
    {
        struct ListNode *tmp = head;
        head = head->next;
        free(tmp);
    }
}

int main()
{
    int n, start, end;
    scanf("%d", &n);
    int *arr = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }
    scanf("%d %d", &start, &end);
    struct ListNode *head = createList(arr, n);
    head = reverseRange(head, start, end);
    printList(head);
    freeList(head);
    free(arr);
    return 0;
}
