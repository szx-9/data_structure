#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
struct node {
    int n;
    struct node* link;
};

int main()
{
    int n;
    struct node* list = NULL, * tail=NULL, * q=NULL;

    scanf("%d", &n);
    while (n != -1) {
        q = (struct node*)malloc(sizeof(struct node));
        q->n = n;
        q->link = list;
        if (list == NULL)
            list = q;
        else
            tail->link = q;
        tail = q;
        scanf("%d", &n);
    }
    //list表示头节点，tail指向当前尾节点，自动建好了已经
    struct node* cur = list;
    struct node* front = tail;
    if (cur->link == NULL) {
        printf("%d", cur->n);
        return 0;
    }

    while (cur->link->link!=cur) {
        struct node* res = cur;
        cur = cur->link->link;
        printf("%d ", res->n);
        front->link = res->link;
        free(res);
        front = front->link;

    }
    printf("%d ", cur->n);
    printf("%d", front->n);
    front->link = NULL;
    free(cur);
    free(front);

    return 0;
}