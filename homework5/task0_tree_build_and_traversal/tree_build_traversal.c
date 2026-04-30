#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 32

struct tnode {            //Huffman树结构
    char c;
    int weight;        //树节点权重，叶节点为字符和它的出现次数
    struct tnode* left, * right;
};
int Ccount[128] = { 0 };        //存放每个字符的出现次数，如Ccount[i]表示ASCII值为i的字符出现次数 
struct tnode* Root = NULL;     //Huffman树的根节点
char HCode[128][MAXSIZE] = { {0} }; //字符的Huffman编码，如HCode['a']为字符a的Huffman编码（字符串形式） 
int Step = 0;            //实验步骤 
FILE* Src, * Obj;

void statCount();        //步骤1：统计文件中字符频率
void createHTree();        //步骤2：创建一个Huffman树，根节点为Root 
void makeHCode();        //步骤3：根据Huffman树生成Huffman编码
void atoHZIP();         //步骤4：根据Huffman编码将指定ASCII码文本文件转换成Huffman码文件

void print1();            //输出步骤1的结果
void print2(struct tnode* p);    //输出步骤2的结果 
void print3();            //输出步骤3的结果
void print4();            //输出步骤4的结果

int main()
{
    if ((Src = fopen("input.txt", "r")) == NULL) {
        fprintf(stderr, "%s open failed!\n", "input.txt");
        return 1;
    }
    if ((Obj = fopen("output.txt", "w")) == NULL) {
        fprintf(stderr, "%s open failed!\n", "output.txt");
        return 1;
    }
    scanf("%d", &Step);        //输入当前实验步骤 

    statCount();            //实验步骤1：统计文件中字符出现次数（频率）
    (Step == 1) ? print1() : 1;       //输出实验步骤1结果    
    createHTree();            //实验步骤2：依据字符频率生成相应的Huffman树
    (Step == 2) ? print2(Root) : 2;     //输出实验步骤2结果    
    makeHCode();        //实验步骤3：依据Root为树的根的Huffman树生成相应Huffman编码
    (Step == 3) ? print3() : 3;       //输出实验步骤3结果
    (Step >= 4) ? atoHZIP(), print4() : 4;//实验步骤4：据Huffman编码生成压缩文件，并输出实验步骤4结果    

    fclose(Src);
    fclose(Obj);
    return 0;
}

//【实验步骤1】开始 
void statCount()
{
    //统计文本中src的字符出现频率
    char ch;
    Ccount[0] = 1;
    while ((ch = getc(Src)) != EOF) {
        if (ch == '\n') {
            continue;
        }
        Ccount[ch]++;
    }
}





//【实验步骤1】结束

//【实验步骤2】开始
int cmp(const void* a, const void* b) {
    //注意此处我们将权重大的放在后面，故所有条件与题设相反
    struct tnode *c1 = *(struct tnode**)a;
    struct tnode * d = *(struct tnode**)b;
    if (c1->weight > d->weight)return -1;
    else if (c1->weight < d->weight)return 1;
    else {
       //两种：正常排序权重相等，按照c；另一种是新插入：c标记为-1
        if (c1->c < 0 && d->c < 0) {
            //均是新生成的，则更小的是新的
            return c1->c < d->c ? -1 : 1;
        }
        if (c1->c <0) 
            return -1;
        if (d->c <0)
            return 1;
        return c1->c < d->c ? 1 : -1;
    }
    return 0;
}
struct tnode* buildUnitTnode(struct tnode* l,struct tnode* r,int cnt) {
    struct tnode* father = (struct tnode*)malloc(sizeof(struct tnode));
    father->left = l;
    father->right = r;
    father->c = -1-cnt;
    father->weight = l->weight + r->weight;
    return father;
}
void createHTree()
{
    
    //根据字符weight构造huffman树
    struct tnode* forest[130];
    int fIndex = 0;
    for (int i = 0; i < 128; ++i) {
        if (Ccount[i] > 0) {
            struct tnode* curr = (struct tnode*)malloc(sizeof(struct tnode));
            curr->c =i;
            curr->left = curr->right = NULL;
            curr->weight = Ccount[i];
            forest[fIndex++] = curr;
        }
    }

    qsort(forest, fIndex, sizeof(struct tnode*), cmp);
    int cnt = 0;//新生成的第几个结点
    while (fIndex!=1) {
        struct tnode* lChild = forest[--fIndex];
        struct tnode* rChild = forest[--fIndex];
        forest[fIndex++] = buildUnitTnode(lChild, rChild,cnt);
        qsort(forest, fIndex, sizeof(struct tnode*), cmp);
        cnt++;
    }
    Root = forest[0];

}





//【实验步骤2】结束

//【实验步骤3】开始
char rec[MAXSIZE];
int recIndex = 0;
int isLeafNode(struct tnode* cur) {
    if (cur->right == NULL && cur->left == NULL)return 1;
    return 0;
}
void fTraversal(struct tnode* curr) {
    if (isLeafNode(curr) ){
        //此时rec中记录了path
        rec[recIndex] = '\0';
        strcpy(HCode[curr->c], rec);
        recIndex--;
    }
    int res = recIndex;//缓存当前数组索引
    if (curr->left) {
        rec[recIndex++] = '0';
        fTraversal(curr->left);
    }
    recIndex = res;
    if (curr->right) {
        rec[recIndex++] = '1';
        fTraversal(curr->right);
    }


}
void makeHCode()
{
    fTraversal(Root);

}





//【实验步骤3】结束

//【实验步骤4】开始
char charSeq[128 * MAXSIZE];
int seqIndex = 0;
void atoHZIP()
{
    rewind(Src);//跳过开头
    char target;
    while ((target = fgetc(Src)) != EOF) {
        strcat(charSeq, HCode[target]);
    }
    strcat(charSeq, HCode[0]);//需要手动添加EOF：这里规定是0
    unsigned char hc = 0;//注意HCode不连续分布，需要全部遍历
    int j = 0;
    for (; charSeq[j] != '\0'; ++j) {
        hc = (hc << 1) | (int)(charSeq[j] - '0');
        if ((j + 1) % 8 == 0) {
            fputc(hc, Obj);
            printf("%x", hc);
        }
    }
    //此时j的个数是真正统计了多少字符，算上了结尾符
    if (j % 8) {
        hc = hc << (j % 8);
        fputc(hc, Obj);
        printf("%x", hc);
    }
}





//【实验步骤4】结束

void print1()
{
    int i;
    printf("NUL:1\n");
    for (i = 1; i < 128; i++)
        if (Ccount[i] > 0)
            printf("%c:%d\n", i, Ccount[i]);
}

void print2(struct tnode* p)
{
    if (p != NULL) {
        if ((p->left == NULL) && (p->right == NULL))
            switch (p->c) {
            case 0: printf("NUL "); break;
            case ' ':  printf("SP "); break;
            case '\t': printf("TAB "); break;
            case '\n':  printf("CR "); break;
            default: printf("%c ", p->c); break;
            }
        print2(p->left);
        print2(p->right);
    }
}

void print3()
{
    int i;

    for (i = 0; i < 128; i++) {
        if (HCode[i][0] != 0) {
            switch (i) {
            case 0: printf("NUL:"); break;
            case ' ':  printf("SP:"); break;
            case '\t': printf("TAB:"); break;
            case '\n':  printf("CR:"); break;
            default: printf("%c:", i); break;
            }
            printf("%s\n", HCode[i]);
        }
    }
}

void print4()
{
    long int in_size, out_size;

    fseek(Src, 0, SEEK_END);
    fseek(Obj, 0, SEEK_END);
    in_size = ftell(Src);
    out_size = ftell(Obj);

    printf("\n原文件大小：%ldB\n", in_size);
    printf("压缩后文件大小：%ldB\n", out_size);
    printf("压缩率：%.2f%%\n", (float)(in_size - out_size) * 100 / in_size);
}
