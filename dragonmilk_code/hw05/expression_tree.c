#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct node
{
    int val;
    char op;
    struct node *ls,*rs;
};
typedef struct node nd;

nd* build(long long val,char op,nd* ls,nd* rs)
{
    nd* p = (nd*)malloc(sizeof(nd));
    p->op = op;
    p->val = val;
    p->ls = ls;
    p->rs = rs;
    return p;
}

int number(char ch)
{
    if(ch >= '0' && ch <= '9') return 1;
    return 0;
}

int sign(char ch)
{
    if(ch == ')') return 1;
    if(ch == '+' || ch == '-') return 2;
    if(ch == '*' || ch == '/' ) return 3;
    if(ch == '(') return 4;
    return 0;
}
nd *num[1000000];
long long tmp,top,st_cnt,s_cnt;
char s[1000000],st[1000000];

long long opt(long long x,long long y,char c)
{
    if(c == '+') return x+y;
    if(c == '-') return x-y;
    if(c == '*') return x*y;
    if(c == '/') return x/y;
    return 0;
}

int main()
{
    char c,op;
    long long x,y;
    scanf("%c",&c);
    while(1)
    {
        if(c == '=') break;
        if(number(c))
        {
            tmp = 0;
            while(number(c))
            {
                tmp = tmp*10+c-'0';
                scanf("%c",&c);
            }
            num[top++]=build(tmp,' ',NULL,NULL);
        }
        else if(sign(c))
        {
            if(c != '(' && st_cnt > 0  && sign(c) <= sign(st[st_cnt-1]))
            {
                if(c == ')')
                {
                    while(st[st_cnt-1] != '(')
                    {
                        x = num[top-2]->val;
                        y = num[top-1]->val;
                        op = st[--st_cnt];
                        num[top-2] = build(opt(x,y,op),op,num[top-2],num[top-1]);
                        top--;
                    }
                    st_cnt--;
                }
                else while(st_cnt > 0 && st[st_cnt-1] != '(' && sign(c) <= sign(st[st_cnt-1]))
                {
                    x = num[top-2]->val;
                    y = num[top-1]->val;
                    op = st[--st_cnt];
                    num[top-2] = build(opt(x,y,op),op,num[top-2],num[top-1]);
                    top--;
                }
            }
            if(c != ')')st[st_cnt++] = c;
            scanf("%c",&c);
        }
        else scanf("%c",&c);
    }
    while(st_cnt > 0)
    {
        x = num[top-2]->val;
        y = num[top-1]->val;
        op = st[--st_cnt];
        num[top-2] = build(opt(x,y,op),op,num[top-2],num[top-1]);
        top--;
    }
    if(st_cnt == 0)
    {
        if(num[0]->op != ' ') 
        {
            printf("%c ",num[0]->op);
            if(num[0]->ls != NULL) 
            {
                if(num[0]->ls->op != ' ') printf("%c ",num[0]->ls->op);
                else printf("%d ",num[0]->ls->val);
                
            }
            if(num[0]->rs != NULL)
            {
                if(num[0]->rs->op != ' ') printf("%c ",num[0]->rs->op);
                else printf("%d ",num[0]->rs->val);
            }
            printf("\n");
        }
        printf("%d",num[0]->val);
    }
    return 0;
}