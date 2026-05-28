#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long
/*

*/

int que[100];
int head = 1, rear = 0;
void enqueue(int val){
    rear++;
    rear %= 100;
    que[rear] = val;
}
void dequeue(){
    que[head] = 0;
    head++;
    head %= 100;
}
int getTop(){
    return que[head];
}
int box[105];
int inde = 0;
int main()
{
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n;++i){
        int num;
        scanf("%d", &num);
        enqueue(num);
    }
    for (int i = 0; i < n;++i){
        scanf("%d", &box[i]);
    }
    int size = n;//总数
    while(size){
        int flag = 0;
        for (int i = 0; i < size;++i){
            if(que[head]==box[inde]){
                dequeue();
                inde++;
                size--;
                flag = 1;
                break;
            }
            else{
                enqueue(getTop());
                dequeue();
            }
        }
        if(!flag){
            printf("%d", size);
            return 0;
        }
    }
    printf("%d", size);
    return 0;
}