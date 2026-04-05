#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
//安全限制必须要在所有include前
struct node {
    int n;
    struct node* link;
};
//void Print(struct node*);
//void reverse(int* num, int len);
//void change(int n, int *num, int len);

int main()
{
    int n;
    struct node* list = NULL, * tail=NULL, * q=NULL;
    //list为真正头节点，tail为实际最后一个节点

    scanf("%d", &n);
    while (n != -1) {//n为输入的待插入数字
        q = (struct node*)malloc(sizeof(struct node));
        q->n = n;
        q->link = NULL;//申请节点



        if (list == NULL)
            list = q;
        else
            tail->link = q;
        tail = q;

        scanf("%d", &n);
    }//建立链表完成

    //访问并输出链表，同时输出数据

    struct node* cur = list;
    struct node* res = list;
    //cur用于遍历，res用于实时释放
    while (cur) {
        res = cur;
        cur = cur->link;
       //计算res中数字的位数
        int digit = 0;
        int copy = res->n;//用于copy计算digit
        while (copy) {
            digit++;
            copy /= 10;
        }
        int div = 1;//确定最高位权重
        for (int i = 1; i < digit; ++i) {
            div *= 10;
        }
        int num = res->n;
        switch (digit % 3) {
        case 1:
            printf("%d", num / div);
            num %= div;
            div /= 10;
            digit--;
            break;
        case 2:
            printf("%d", num / div);
            num %= div;
            div /= 10;
            printf("%d", num / div);
            num %= div;
            div /= 10;

            digit -= 2;

            break;
        
        case 0:
            printf("%d", num / div);
            num %= div;
            div /= 10;
            printf("%d", num / div);
            num %= div;
            div /= 10;
            printf("%d", num / div);
            num %= div;
            div /= 10;
            digit -= 3;

            break;

        }
        int cnt = 3;
        for (int i = 0; i < digit; ++i) {
            if (cnt == 3) {
                printf(",");
                cnt = 0;
            }
            printf("%d", num/div);
            num %= div;
            div /= 10;
            cnt++;
        }

        printf(" ");



    }
}
//void reverse(int* arr, int len) {
//    int l = 0, r = len - 1;
//    while (l < r) {
//        int tmp = arr[l];
//        arr[l] = arr[r];
//        arr[r] = tmp;
//     
//        l++;
//        r--;
//    }
//    return;
//}
//
//
//void change(int n, int* arr ,int len) {
//    //将一个带有数位的数字转化成数组存入,n表示实际数字，len是数位长度即数组长度
//
//    for (int i = 0; i < len; ++i) {
//        arr[i] = n % 10;
//        n = n/10;
//    }
//    reverse(arr, len);
//
//    return;
//}
//
//
//
//void Print(struct node* p) {
//    //按要求输出某个数字
//    int m = 0;//数位
//    int res = p->n;//用于记录数字大小
//    while ( res  ) {
//        res = res / 10;
//        m++;
//    }
//    
//
//    change(p->n, num, m);
//
//    int numIndex = 0;
//    switch (m % 3) {
//    case 1:
//        printf("%d", num[numIndex++]);
//        break;
//    case 2:
//        printf("%d%d", num[numIndex++], num[numIndex++]);
//        break;
//    case 0:
//        printf("%d%d%d", num[numIndex++], num[numIndex++], num[numIndex++]);
//        break;
//    }
//    int cnt = 3;//统一设置cnt为3，则如果可以进入循环，则输出一个逗号
//    for (; numIndex < m; numIndex++) {
//        if (cnt==3) {
//            printf(",");
//            cnt = 0;
//        }
//        printf("%d", num[numIndex]);
//        cnt++;
//    }
//   
//    
//    printf(" ");
//    return;
//}
