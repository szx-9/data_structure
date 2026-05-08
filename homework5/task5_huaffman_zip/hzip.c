#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
main函数命令行参数：
 终端中执行：程序名 + 空格分隔的多个参数
test.exe 100 hello "C language"
此处argc=4,argv存储每一个参数文本，自定义参数带空格需要双引号，最后会用NULL标记

>:表示命令行表示，即紧跟的字段是可执行或者可以在命令行中运行的文件例如.exe这是一般不会将.exe写出
但是argv[0]是这个字段，所以有效参数需要从1开始，也就是说argc最小就是1
*/
#define MAXSIZE 256  //! 从32扩大到256，树深度安全上限（最深127，256足够）
#define MAXNAME 200


enum {
    IS_ZIP = 1,//压缩模式
    IS_UNZIP = 2,//解压模式
    WRONG=0,
    IS_FORMAT = 1,
    IS_EXTEN = 0
};
typedef struct tnode {            //Huffman树结构
    int c;
    int weight;        //树节点权重，叶节点为字符和它的出现次数
    struct tnode* left, * right;
}tnode;
int Ccount[128] = { 0 };        //存放每个字符的出现次数，如Ccount[i]表示ASCII值为i的字符出现次数 
struct tnode* Root = NULL;     //Huffman树的根节点
char HCode[128][MAXSIZE] = { {0} }; //字符的Huffman编码，如HCode['a']为字符a的Huffman编码（字符串形式） 
int Step = 0;            //实验步骤 
FILE* Src, * Obj;

void statCount();        //步骤1：统计文件中字符频率
void createHTree();        //步骤2：创建一个Huffman树，根节点为Root 
void makeHCode();        //步骤3：根据Huffman树生成Huffman编码
void atoHZIP();         //步骤4：根据Huffman编码将指定ASCII码文本文件转换成Huffman码文件

void printError(int type);
int findChar(char* buf, char ch);
int parseCommand(int,char* argv[], char**);
int getVaildFre();

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
void freeTree(tnode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void dealFilename(char** filename,int type);

int main(int argc,char* argv[])
{
    char* filename = NULL;
    int type = parseCommand(argc,argv,&filename);
    if (!type)return 0;

    Src = fopen(filename, "r");
    if (!Src) { fprintf(stderr, "Cannot open input file!\n"); return 1; }  //! fopen失败检查
    dealFilename(&filename, type);
    Obj = fopen(filename, "wb");
    if (!Obj) { fprintf(stderr, "Cannot open output file!\n"); return 1; } //! fopen失败检查
    
    statCount();//统计字符数
    createHTree();//创建huffmantree
    makeHCode();//依据前序遍历生成huffman编码
    if ( type== IS_ZIP) {
        atoHZIP();
    }
    else {
        return 0;
    }
   // print3();
    
   freeTree(Root);
    free(filename);

    fclose(Src);
    fclose(Obj);
    return 0;
}
void dealFilename(char** filename,int type){
    //将filename中的扩展名替换成.hzip或者.txt
    if(type==IS_ZIP) {
        char* ext = NULL;
        if ((ext = strrchr(*filename, '.')) != NULL) {
            if (strcmp(ext, ".txt")) {
                printError(IS_EXTEN);
                return;
            }
            //提取用户名，
            char* nameBuf = (char*)malloc(sizeof(char) * MAXNAME);
            char* cur = *filename;
            int bufIndex = 0;
            while (cur != ext) {
                nameBuf[bufIndex++] = *cur;
                cur++;
            }
            nameBuf[bufIndex] = '\0';
            strcat(nameBuf, ".hzip");
            *filename = nameBuf;
        }
    }
    else {
        char* ext = NULL;
        if ((ext = strrchr(*filename, '.')) != NULL) {
            if (strcmp(ext, ".hzip")) {
                printError(IS_EXTEN);
                return;
            }
            //提取用户名，
            char* nameBuf = (char*)malloc(sizeof(char) * MAXNAME);
            char* cur = *filename;
            int bufIndex = 0;
            while (cur != ext) {
                nameBuf[bufIndex++] = *cur;
                cur++;
            }
            nameBuf[bufIndex] = '\0';
            strcat(nameBuf, ".txt");
            *filename = nameBuf;
        }
    }

}
void printError(int type) {
    type == IS_FORMAT ? printf("Usage: hzip.exe [-u] <filename>\n") : printf("File extension error!\n");
}

int findChar(char* buf, char ch) {
    int len = strlen(buf);
    for (int i = 0; i < len; ++i) {
        if (buf[i] == ch)return i;
    }
    return -1;
}

int parseCommand(int argc,char* argv[], char** filename) {
    //需要额外提取filename通过指针返回
    if (argc == 1||argc>3) {
        printError(IS_FORMAT);
        return WRONG;
    }
    //第一个有效参数，-u或者直接文件
    if (!strcmp(argv[1], "-u")&&argc==3) {
        //解压
        char* ext = NULL;
        if ((ext = strrchr(argv[2], '.')) != NULL) {
            if (strcmp(ext, ".hzip")) {
                printError(IS_EXTEN);
                return WRONG;
            }
            *filename = argv[2];  //! 直接指向argv，不用malloc（dealFilename会分配新内存）
            return IS_UNZIP;
        }
        printError(IS_FORMAT);
        return WRONG;
    }
    else if (strstr(argv[1], ".")!=NULL&&argc==2) {
        //压缩
        char* ext = NULL;
        if ( (ext=strrchr(argv[1],'.')) != NULL) {
            if (strcmp(ext,".txt")) {
                printError(IS_EXTEN);
                return WRONG;
            }
            *filename = argv[1];  //! 直接指向argv，不用malloc（dealFilename会分配新内存）
            return IS_ZIP;
        }

    }
    printError(IS_FORMAT);
    return WRONG;
}



//【实验步骤1】开始 
void statCount()
{
    //统计文本中src的字符出现频率
    int ch;  //! 必须是int，getc返回0~255或EOF(-1)，signed char会把0xFF误判为EOF
    Ccount[0] = 1;
    while ((ch = getc(Src)) != EOF) {
        Ccount[ch]++;
    }
}
//【实验步骤1】结束

//【实验步骤2】开始
int cmp(const void* a, const void* b) {
    //注意此处我们将权重大的放在后面，故所有条件与题设相反
    struct tnode* c1 = *(struct tnode**)a;
    struct tnode* d = *(struct tnode**)b;
    if (c1->weight > d->weight)return -1;
    else if (c1->weight < d->weight)return 1;
    else {
        //两种：正常排序权重相等，按照c；另一种是新插入：c标记为-1
        if (c1->c < 0 && d->c < 0) {
            //均是新生成的，则更小的是新的
            return c1->c < d->c ? -1 : 1;
        }
        if (c1->c < 0)
            return -1;
        if (d->c < 0)
            return 1;
        return c1->c < d->c ? 1 : -1;
    }
    return 0;
}
struct tnode* buildUnitTnode(struct tnode* l, struct tnode* r, int cnt) {
    struct tnode* father = (struct tnode*)malloc(sizeof(struct tnode));
    father->left = l;
    father->right = r;
    father->c = -1 - cnt;
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
            curr->c = i;
            curr->left = curr->right = NULL;
            curr->weight = Ccount[i];
            forest[fIndex++] = curr;
        }
    }

    qsort(forest, fIndex, sizeof(struct tnode*), cmp);
    int cnt = 0;//新生成的第几个结点
    while (fIndex != 1) {
        struct tnode* lChild = forest[--fIndex];
        struct tnode* rChild = forest[--fIndex];
        forest[fIndex++] = buildUnitTnode(lChild, rChild, cnt);
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
    if (isLeafNode(curr)) {
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
//! charSeq 改为在 atoHZIP 中动态分配，精确计算所需大小，不再用固定8MB数组
int getVaildFre() {
    //遍历HCode找到
    int cnt = 0;
    for (int i = 0; i < 128; ++i) {
        if (HCode[i][0]) {
            cnt++;
        }
    }
    return cnt;
}
void atoHZIP()
{
    int cnt = getVaildFre();//计算有效字符，即码表长度
    fputc(cnt, Obj);//输出码表长度

    for (int i = 0; i < 128; ++i) {
        if (HCode[i][0]) {
            //先输出ascii码
            fputc(i, Obj);

            //输出码长
            fputc(strlen(HCode[i]), Obj);

            //将一个数组模拟的8bit数字转换成一个无符号的字节
            unsigned char hc = 0;
            for (int j = 0; j < 8; ++j) {
                hc = ((hc << 1) )|( (HCode[i][j] - '0')>=0? (HCode[i][j] - '0'):0);//不足这里会自动补位
            }
            fputc(hc, Obj);
        }
    }

    //! 动态计算charSeq所需大小：每个字符的频次 × Huffman码长
    int charSeqSize = 1; // '\0'
    for (int i = 0; i < 128; ++i) {
        if (Ccount[i] > 0) {
            charSeqSize += Ccount[i] * (int)strlen(HCode[i]);
        }
    }
    char* charSeq = (char*)malloc(charSeqSize);  //! 动态分配，精确大小，不会溢出
    if (!charSeq) { fprintf(stderr, "Memory allocation failed!\n"); return; }

    rewind(Src);//跳回开头
    int target;  //! 必须是int，fgetc返回0~255或EOF(-1)
    char *p = charSeq;
    while ((target = fgetc(Src)) != EOF) {//对读入的文件字符按照HCode中进行比对
        char*code = HCode[target];
        while(*code) {
            *p++ = *code++;
        }
    }
    //! 追加文件结束符的Huffman码（不用strcat，避免重复扫描）
    char* code = HCode[0];
    while(*code){
        *p++ = *code++;
    }
    *p = '\0';

    unsigned char hc = 0;//注意HCode不连续分布，需要全部遍历
    int j = 0;
    for (; charSeq[j] != '\0'; ++j) {
        hc = (hc << 1) | (int)(charSeq[j] - '0');
        if ((j + 1) % 8 == 0) {
            fputc(hc, Obj);
            hc = 0;
        }
    }
    //此时j的个数是真正统计了多少字符，算上了结尾符
    if (j % 8) {
        hc = hc << (8 - j % 8);//注意位移量
        fputc(hc, Obj);
    }
    free(charSeq);  //! 释放动态分配的charSeq
}