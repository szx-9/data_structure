#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ll long long

#define max_exp 1000000
ll num_st[max_exp],num_cnt,op_cnt;
char suf[max_exp],op_st[max_exp];
int num_check(char ch)
{
    if(ch >= '0' && ch <= '9') return 1;
    return 0;
}
int op_check(char ch)
{
    if(ch == '(') return 1;
    if(ch == '+' || ch == '-') return 2;
    if(ch == '*' || ch == '/') return 3;
    if(ch == ')') return 4;
    return 0;
}
char* num_transfer(ll x)
{
    int i = 0,j;
    char tmp;
    char* ans = (char*)malloc(sizeof(char) * 20);
    while(x)
    {
        ans[i++] = x%10+'0';
        x/=10;
    }
    ans[i] = '\0';
    for(i = i-1 , j = 0 ; j < i ; ++j,--i)
    {
        tmp = ans[i];
        ans[i] = ans[j];
        ans[j] = tmp;
    }
    return ans;
}
char* sufexp(char a[])
{
    int len = strlen(a),i,j;
    char *ans = (char*) malloc(sizeof(char) * (len+1));
    for(i = 0 ; i < len ; ++i)
    {
        if(num_check(a[i]))
        {
            ll tmp = 0;
            while(i < len && num_check(a[i]))
            {
                tmp = tmp*10+a[i]-'0';
                ++i;
            }
            num_st[num_cnt++] = tmp;
        }
        else if(op_check(a[i]))
        {
            if(op_cnt <= 0 || op_check(op_st[op_cnt-1]) < op_check(a[i])) op_st[op_cnt++] = a[i];
            else
            {
                ll x,y;
                char op;
                if(a[i] == ')')
                {
                    while(op_cnt > 0 && op_st[op_cnt-1] != '(')
                    {
                        y = num_st[--num_cnt];
                        x = num_st[--num_cnt];
                        op = op_st[--op_cnt];
                        strcat(ans,num_transfer(x));
                        strcat(ans,num_transfer(y));
                        strcat(ans,op);
                        op_cnt--;
                    }
                }
                else while(op_cnt > 0 && op_check(op_st[op_cnt-1]) >= op_check(a[i]))
                {

                }
            }
        }
    }

}


char s[1000000];
int main()
{
    scanf("%s",s);
    printf("%s",sufexp(s));
    return 0;
}