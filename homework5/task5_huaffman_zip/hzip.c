/* ============================================================
   【总改】全部重写，核心变更：
   1. 删除 node 链表包装，改用数组构建 Huffman 树
   2. atoHZIP: 头部存入原文件大小(4字节)，重写比特打包逻辑
   3. atoUnzip: 完整实现解压
   4. 修复 createHTree 中 node/tnode 类型混用、->right/->next 混用等 bug
   5. 修复 atoHZIP 中 fputc(byte, Obj) 写成 fputc(byte 下标) 的 bug
   6. 修复 while((rec=fgetc)) 在 rec==0 时误退出的 bug
   ============================================================ */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 256   // 编码最大长度（Huffman 树最深 127，256 足够）
#define MAXNAME 200

enum {
    IS_ZIP = 1,
    IS_UNZIP = 2,
    WRONG = 0,
    IS_FORMAT = 1,   // 保留，供 printError 使用
    IS_EXTEN = 0      // 保留，供 printError 使用
};

/* ---------- 数据结构 ---------- */
typedef struct tnode {
    int c;                  // 字符值，内部节点为 -1
    int weight;             // 权重（频次）
    struct tnode *left, *right;
} tnode;

/* 【删】移除了 node 结构体（链表包装），改为在 createHTree 中用 tnode* 数组 */

int Ccount[128] = {0};                  // 字符频次
tnode *Root = NULL;                     // Huffman 树根
char HCode[128][MAXSIZE] = {{0}};       // Huffman 编码表（字符串）
FILE *Src, *Obj;

/* ---------- 函数声明 ---------- */
void statCount(void);
tnode *buildUnitTnode(tnode *l, tnode *r);   // 【改】返回 tnode*（原为 node*）
void createHTree(void);                       // 【改】用数组而非链表
int  isLeafNode(tnode *cur);
void fTraversal(tnode *curr, int recIndex);
void makeHCode(void);
void atoHZIP(void);                           // 【改】重写比特打包
void atoUnzip(void);                          // 【改】完整实现
void printError(int type);
int  parseCommand(int argc, char *argv[], char **filename);
void freeTree(tnode *root);
void dealFilename(char **filename, int type);
int  getVaildFre(void);

/* ============================================================
   main
   ============================================================ */
int main(int argc, char *argv[])
{
    char *filename = NULL;
    int type = parseCommand(argc, argv, &filename);
    if (!type)
        return 0;

    Src = fopen(filename, "rb");
    if (!Src) {
        fprintf(stderr, "Cannot open input file!\n");
        return 1;
    }
    dealFilename(&filename, type);
    Obj = fopen(filename, "wb");
    if (!Obj) {
        fprintf(stderr, "Cannot open output file!\n");
        fclose(Src);
        return 1;
    }

    if (type == IS_ZIP) {
        statCount();     // 【改】只在压缩时统计频次
        createHTree();   // 【改】只在压缩时建树
        makeHCode();     // 【改】只在压缩时生成编码
        atoHZIP();
    } else {
        atoUnzip();      // 解压：直接从文件头读码表
    }

    freeTree(Root);
    free(filename);
    fclose(Src);
    fclose(Obj);
    return 0;
}

/* ============================================================
   dealFilename — 替换扩展名
   ============================================================ */
void dealFilename(char **filename, int type)
{
    char *ext = strrchr(*filename, '.');
    if (!ext) {
        printf("ERROR_DEALNAME\n");
        return;
    }
    char *nameBuf = (char *)malloc(MAXNAME);
    char *cur = *filename;
    int idx = 0;
    while (cur != ext)
        nameBuf[idx++] = *cur++;
    nameBuf[idx] = '\0';
    if (type == IS_ZIP)
        strcat(nameBuf, ".hzip");
    else
        strcat(nameBuf, ".txt");

    *filename = nameBuf;   // 【注】原 argv 指向的内容不再使用，新内存由 caller free
}

/* ============================================================
   printError / parseCommand
   ============================================================ */
void printError(int type)
{
    if (type == IS_FORMAT)
        printf("Usage: hzip.exe [-u] <filename>\n");
    else
        printf("File extension error!\n");
}

int parseCommand(int argc, char *argv[], char **filename)
{
    if (argc == 1 || argc > 3) {
        printError(IS_FORMAT);
        return WRONG;
    }
    /* -u 解压模式: hzip -u xxx.hzip */
    if (!strcmp(argv[1], "-u") && argc == 3) {
        char *ext = strrchr(argv[2], '.');
        if (!ext || strcmp(ext, ".hzip")) {
            printError(IS_EXTEN);
            return WRONG;
        }
        *filename = argv[2];
        return IS_UNZIP;
    }
    /* 压缩模式: hzip xxx.txt */
    if (argc == 2 && strstr(argv[1], ".")) {
        char *ext = strrchr(argv[1], '.');
        if (!ext || strcmp(ext, ".txt")) {
            printError(IS_EXTEN);
            return WRONG;
        }
        *filename = argv[1];
        return IS_ZIP;
    }
    printError(IS_FORMAT);
    return WRONG;
}

/* ============================================================
   步骤1: statCount — 统计字符频次
   ============================================================ */
void statCount(void)
{
    int ch;
    while ((ch = getc(Src)) != EOF)
        Ccount[ch]++;
    /* 【改】删除了 Ccount[0]=1（不再需要哨兵，文件头存有原文件大小） */
}

/* ============================================================
   步骤2: createHTree — 用数组构建 Huffman 树
   【改】不再使用 node 链表 + insertSort，改为：
   1) 收集所有叶节点到 tnode* 数组
   2) 每轮线性扫描找两个最小权重节点
   3) 合并后放回数组
   时间复杂度 O(m²)，m ≤ 128，完全足够
   ============================================================ */
tnode *buildUnitTnode(tnode *l, tnode *r)   // 【改】返回 tnode*（原为 node*）
{
    tnode *father = (tnode *)malloc(sizeof(tnode));
    father->left  = l;
    father->right = r;
    father->c     = -1;
    father->weight = l->weight + r->weight;
    return father;
}

void createHTree(void)
{
    /* 1. 收集所有出现过的字符 → 叶节点数组 */
    tnode *nodes[128];
    int n = 0;
    for (int i = 0; i < 128; i++) {
        if (Ccount[i] > 0) {
            tnode *leaf = (tnode *)malloc(sizeof(tnode));
            leaf->c      = i;
            leaf->weight = Ccount[i];
            leaf->left = leaf->right = NULL;
            nodes[n++] = leaf;
        }
    }

    /* 空文件保护：手动造一个哨兵节点，避免 Root==NULL */
    if (n == 0) {
        tnode *dummy = (tnode *)malloc(sizeof(tnode));
        dummy->c = -1;
        dummy->weight = 0;
        dummy->left = dummy->right = NULL;
        Root = dummy;
        return;
    }

    /* 2. 迭代合并最小两节点 */
    while (n > 1) {
        /* 找两个最小权重的下标 */
        int m1 = 0, m2 = 1;
        if (nodes[m1]->weight > nodes[m2]->weight) {
            int t = m1; m1 = m2; m2 = t;
        }
        for (int i = 2; i < n; i++) {
            if (nodes[i]->weight < nodes[m1]->weight) {
                m2 = m1;
                m1 = i;
            } else if (nodes[i]->weight < nodes[m2]->weight) {
                m2 = i;
            }
        }

        /* 合并 m1, m2 → 父节点 */
        tnode *parent = buildUnitTnode(nodes[m1], nodes[m2]);

        /* parent 放回 m1 位置，m2 用末尾元素覆盖，n-- */
        nodes[m1] = parent;
        nodes[m2] = nodes[n - 1];
        n--;
    }
    Root = nodes[0];
}

/* ============================================================
   步骤3: makeHCode — 前序遍历生成 Huffman 编码
   ============================================================ */
int isLeafNode(tnode *cur)
{
    return (cur->left == NULL && cur->right == NULL);
}

char rec[MAXSIZE];   // 遍历路径缓冲区

void fTraversal(tnode *curr, int recIndex)
{
    if (curr == NULL) return;

    if (isLeafNode(curr)) {
        rec[recIndex] = '\0';
        if (recIndex == 0)          // 单节点树：编码为 "0"
            strcpy(HCode[curr->c], "0");
        else
            strcpy(HCode[curr->c], rec);
        return;
    }
    if (curr->left) {
        rec[recIndex] = '0';
        fTraversal(curr->left, recIndex + 1);
    }
    if (curr->right) {
        rec[recIndex] = '1';
        fTraversal(curr->right, recIndex + 1);
    }
}

void makeHCode(void)
{
    fTraversal(Root, 0);
}

/* ============================================================
   步骤4: atoHZIP — 压缩
   【改】重写：
   - 头部先写 4 字节原文件大小，再写码表
   - 比特打包统一用「位缓冲」模型，大幅简化逻辑
   ============================================================ */
int getVaildFre(void)
{
    int cnt = 0;
    for (int i = 0; i < 128; i++)
        if (HCode[i][0]) cnt++;
    return cnt;
}

static void writeBitsMSB(const char *code, int len, FILE *out)
{
    /* 将 len 位 0/1 字符串按 MSB-first 写入文件 */
    unsigned char byte = 0;
    int bits = 0;
    for (int i = 0; i < len; i++) {
        byte = (unsigned char)((byte << 1) | (code[i] - '0'));
        bits++;
        if (bits == 8) {
            fputc(byte, out);
            byte = 0;
            bits = 0;
        }
    }
    if (bits > 0) {
        byte <<= (8 - bits);
        fputc(byte, out);
    }
}

void atoHZIP(void)
{
    /* --- 取得原文件大小 --- */
    fseek(Src, 0, SEEK_END);
    long origSize = ftell(Src);
    rewind(Src);
    fputc((int)( origSize        & 0xFF), Obj);
    fputc((int)((origSize >> 8)  & 0xFF), Obj);
    fputc((int)((origSize >> 16) & 0xFF), Obj);
    fputc((int)((origSize >> 24) & 0xFF), Obj);

    /* --- 写码表 --- */
    int cnt = getVaildFre();
    fputc(cnt, Obj);                     // 码表条目数

    for (int i = 0; i < 128; i++) {
        if (HCode[i][0]) {
            int codeLen = (int)strlen(HCode[i]);
            fputc(i, Obj);               // 字符
            fputc(codeLen, Obj);         // 码长
            writeBitsMSB(HCode[i], codeLen, Obj);  // 编码比特
        }
    }

    /* --- 写编码数据 --- */
    unsigned char bitBuf = 0;
    int bitCnt = 0;
    int ch;
    while ((ch = fgetc(Src)) != EOF) {   // 【改】检查 EOF 而非真值
        char *code = HCode[ch];
        for (int j = 0; code[j]; j++) {
            bitBuf = (unsigned char)((bitBuf << 1) | (code[j] - '0'));
            bitCnt++;
            if (bitCnt == 8) {
                fputc(bitBuf, Obj);
                bitBuf = 0;
                bitCnt = 0;
            }
        }
    }
    /* 冲刷剩余位 */
    if (bitCnt > 0) {
        bitBuf <<= (8 - bitCnt);
        fputc(bitBuf, Obj);
    }
}

/* ============================================================
   步骤5: atoUnzip — 解压
   【改】完整实现
   ============================================================ */
void atoUnzip(void)
{
    /* --- 读头部：4 字节原文件大小 --- */
    long origSize = 0;
    origSize |= (long)fgetc(Src);
    origSize |= (long)fgetc(Src) << 8;
    origSize |= (long)fgetc(Src) << 16;
    origSize |= (long)fgetc(Src) << 24;

    /* --- 读码表，构建解码 trie --- */
    int tableCnt = fgetc(Src);

    tnode *decRoot = (tnode *)malloc(sizeof(tnode));
    decRoot->c = -1;
    decRoot->left = decRoot->right = NULL;

    for (int i = 0; i < tableCnt; i++) {
        int ch      = fgetc(Src);          // 字符
        int codeLen = fgetc(Src);          // 码长

        /* 从文件逐字节读出 codeLen 个比特，拼成 0/1 字符串 */
        char codeStr[MAXSIZE] = {0};
        int byte = 0;
        for (int b = 0; b < codeLen; b++) {
            if (b % 8 == 0)
                byte = fgetc(Src);         // 读下一个字节
            int bit = (byte >> (7 - (b % 8))) & 1;
            codeStr[b] = (char)('0' + bit);
        }
        codeStr[codeLen] = '\0';

        /* 沿路径插入解码树 */
        tnode *cur = decRoot;
        for (int j = 0; j < codeLen; j++) {
            if (codeStr[j] == '0') {
                if (!cur->left) {
                    cur->left = (tnode *)malloc(sizeof(tnode));
                    cur->left->c = -1;
                    cur->left->left = cur->left->right = NULL;
                }
                cur = cur->left;
            } else {
                if (!cur->right) {
                    cur->right = (tnode *)malloc(sizeof(tnode));
                    cur->right->c = -1;
                    cur->right->left = cur->right->right = NULL;
                }
                cur = cur->right;
            }
        }
        cur->c = ch;   // 叶子节点标记字符
    }

    /* --- 解码 --- */
    long decoded = 0;
    tnode *cur = decRoot;
    int byte = fgetc(Src);
    int bitsLeft = 8;

    while (decoded < origSize) {
        if (bitsLeft == 0) {
            byte = fgetc(Src);
            bitsLeft = 8;
        }
        int bit = (byte >> (bitsLeft - 1)) & 1;
        bitsLeft--;

        cur = (bit == 0) ? cur->left : cur->right;

        if (cur && cur->left == NULL && cur->right == NULL) {
            fputc(cur->c, Obj);
            decoded++;
            cur = decRoot;
        }
    }

    freeTree(decRoot);
}

/* ============================================================
   freeTree — 递归释放 Huffman 树
   ============================================================ */
void freeTree(tnode *root)
{
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
