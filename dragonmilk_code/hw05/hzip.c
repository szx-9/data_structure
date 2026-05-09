#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct hzip
{
    struct hzip *ls , *rs;
    char c;
    int num;
}hz;
int buk[512];
hz *que[117200];
char code[256][50];
FILE *in,*out;

hz* build(int ch,int num)
{
    hz* p = (hz*)malloc(sizeof(hz));
    p->c = (char) ch;
    p->num = num;
    p->ls = p->rs = NULL;
    return p;
}

int cmp(hz* a,hz* b)
{
    if(a->num != b->num) return a->num-b->num;
    return (int)(a->c)-(int)(b->c);
}

void bsort(int L,int R)
{
    int i,j;
    hz* tmp;
    for(i = L ; i < R ; ++i)
        for(j = R-1 ; j > i ; --j)
            if(cmp(que[j-1],que[j]) > 0) tmp = que[j-1],que[j-1] = que[j] , que[j] = tmp; 
    /*printf("list:");
    for(i = L ; i < R ; ++i) printf("%c(%d) ",que[i]->c,que[i]->num);
    printf("\n");*/
}

char tmp[50];
void getcode(hz* t,int d)
{
    if(t == NULL) return ;
    if(t->ls == NULL && t->rs == NULL)
    {
        tmp[d] = '\0';
        printf("%c : %s \n",t->c,tmp);
        return ;
    }
    tmp[d] = '0';
    getcode(t->ls,d+1);
    tmp[d] = '1';
    getcode(t->rs,d+1);
    return ;
}

char textout[110];

int main(int argc,char** argv)
{
    if(argc == 2)
    {
        if(strcmp(argv[0],"hzip") != 0)
        {
            printf("Usage: hzip.exe [-u] <filename>");
            exit(-1);
        } 
        int len = strlen(argv[1]);
        if(!(len > 4 && argv[1][len-1] == 't' && argv[1][len-2] == 'x' && argv[1][len-3] == 't' && argv[len-4] == '.'))
        {
            printf("File extension error!");
            exit(-1);
        }
        in = fopen(argv[1],"r");
        if(in == NULL)
        {
            printf("Can't open the file!");
            exit(-1);
        }
        strcpy(textout,argv[1]);
        textout[len-3] = 'h',textout[len-2]='z',textout[len-1]='i',textout[len]='p',textout[len+1]='\0';
        out = fopen(textout,"w");
        int i,ch,left = 0,right = 0,count = 0;
        hz* p = NULL , *q = NULL , *r = NULL;
        buk[0] = 1;
        while((ch = fgetc(in)) != EOF) buk[ch]++;
        for(i = 0 ; i < 128 ; ++i)
            if(buk[i] != 0) 
                que[right++] = build(i,buk[i]),count++;
        
        while(right-left > 1)
        {
            bsort(left,right);
            p = que[left++],q = que[left++];
            r = build(127,p->num+q->num);
            r->ls = p,r->rs = q;
            que[right++] = r;
        }
        r = que[left];
        getcode(r,0);
        fprintf(out,"%c",(char)count);
        int j,k,now;
        char tmp;
        for(i = 0 ; i < 128 ; ++i)
        {
            if(buk[i] == 0) continue;
            fprintf(out,"%c",(char)i);
            len = strlen(code[i]);
            fprintf(out,"%c",(char)(len));
            k = 0;
            for(j = 0 ; j < (len-1)/8+1 ; ++j)
            {
                tmp = 0,now = 0;
                while(now < 8 && k < len)
                {
                    tmp|=code[i][k++]-'0';
                    tmp<<=1;
                    now++;
                }
                while(now < 8)
                {
                    tmp<<=1;
                    now++;
                }
                fprintf(out,"%c",tmp);
            }
        }
    }

    return 0;
}