#include<stdio.h>
#define ll long long
struct node
{
    int height;
    ll val;
    struct node *ls,*rs;
};
typedef struct node nd;

nd* bst_build(ll val)
{
    nd* p = (nd*)malloc(sizeof(nd));
    p->val = val;
    p->height = 1;
    p->ls = p->rs = NULL;
    return p;
}

void bst_insert(nd* t,nd* s)
{
    if(t == NULL) return ;
    if(s->val < t->val)
    {
        if(t->ls == NULL) t->ls = s,s->height = t->height+1;
        else bst_insert(t->ls,s);
    }
    else
    {
        if(t->rs == NULL) t->rs = s,s->height = t->height+1;
        else bst_insert(t->rs,s);
    }
}

nd* bst_search(nd* t,ll val)
{
    if(t == NULL) return NULL;
    if(val < t->val) return bst_search(t->ls,val);
    if(val > t->val) return bst_search(t->rs,val);
    return t;
}

void bst_middfs(nd* t)
{
    if(t == NULL) return ;
    if(t->ls == NULL && t->rs == NULL) 
    {
        printf("%lld %d\n",t->val,t->height);
        return ;
    }
    bst_middfs(t->ls);
    bst_middfs(t->rs);
}

int main()
{
    int n,x,i;
    scanf("%d",&n);
    scanf("%d",&x);
    nd* root = bst_build(x);
    for(i = 2 ; i <= n ; ++i)
    {
        scanf("%d",&x);
        bst_insert(root,bst_build(x));
    }
    bst_middfs(root);
    return 0;
}