#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ll long long
struct node
{
    ll num;
    char* word;
    struct node *ls,*rs;
};
typedef struct node nd;

char lower(char ch)
{
    if(!((ch >= 'A' && ch <= 'Z')||(ch >= 'a' && ch <= 'z'))) return '0';
    if(ch >= 'A' && ch <= 'Z') return ch-'A'+'a';
    return ch;
}

nd* bst_build(char word[])
{
    int i,len = strlen(word);
    nd* p = (nd*)malloc(sizeof(nd));
    p->word = (char*)malloc(sizeof(char)*(len+1));
    strcpy(p->word,word);
    p->num = 1;
    p->ls = p->rs = NULL;
    return p;
}

nd* bst_insert(nd* t,nd* s)
{
    if(t == NULL) return s;
    int sgn = strcmp(s->word,t->word);
    if(sgn == 0) t->num += s->num,free(s);
    else if(sgn < 0)
    {
        if(t->ls == NULL) t->ls = s;
        else bst_insert(t->ls,s);
    }
    else
    {
        if(t->rs == NULL) t->rs = s;
        else bst_insert(t->rs,s);
    }
    return t;
}

void bst_dfs(nd* t)
{
    if(t == NULL) return ;
    bst_dfs(t->ls);
    printf("%s %lld\n",t->word,t->num);
    bst_dfs(t->rs);
}

int n,x,i;
char c,tmp[10000];
int main()
{
    nd* root = NULL;
    FILE* in = fopen("article.txt","r");
    while((c = fgetc(in)) != EOF)
    {
        if(lower(c) == '0') continue;
        i = 1;
        tmp[0] = lower(c);
        while((c = fgetc(in)) != EOF && lower(c) != '0')
        {
            tmp[i++] = lower(c);
        }
        tmp[i] = '\0';
        root = bst_insert(root,bst_build(tmp));
    }
    if(root != NULL)
    {
        printf("%s",root->word);
        if(root->rs != NULL)
        {
            printf(" %s",root->rs->word);
            if(root->rs->rs != NULL)
            {
                printf(" %s",root->rs->rs->word);
            }
        }
        printf("\n");
    }
    bst_dfs(root);
    return 0;
}