#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long
/*

*/
char buf[5];
int stk[10000];
int inde = 0;

int main()
{
    int n;//操作数量
    scanf("%d", &n);
    for (int i = 0; i < n;++i){
        scanf(" %s", buf);
        if(buf[0]=='C'){
            inde--;
        }
        else if(buf[0]=='D'){
            int val = stk[inde - 1] * 2;
            stk[inde++] = val;
        }
        else if(buf[0]=='+'){
            int val = stk[inde - 1] + stk[inde - 2];
            stk[inde++] = val;
        }
        else{
            int num = atoi(buf);
            stk[inde++] = num;
        }
    }
    int sum = 0;
    for (int i = 0; i < inde;++i){
        sum += stk[i];
    }
    printf("%d", sum);
    return 0;
}