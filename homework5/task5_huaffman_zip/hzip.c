#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ========== 常量 ========== */
#define MAX_CHAR 256 // ASCII 0~127，考虑退化成链表，ascii范围内最大安全数值
#define MAX_NODE 256 // 总结点数 ≤ 2*128-1 = 255

enum
{
    IS_ZIP = 1,
    IS_UNZIP = 2,
    WRONG = 0,
    IS_FORMAT = 1,
    IS_EXTEN = 0,
    IS_INSERT = 1,
    NO_INSERT = 0,
    IS_TABLE = 1,
    IS_TEXT = 2
};

typedef struct tnode
{
    int ch_val, weight;
    struct tnode *r, *l;
} tnode;

typedef struct node
{
    tnode *tnodePtr;
    struct node *next;
} node;

tnode *Root = NULL;

/* ========== 编码表 ========== */
char code_bits[MAX_CHAR][MAX_CHAR]; // 每个字符的 Huffman 编码（位数组，高位在前）
int code_len[MAX_CHAR];             // 编码长度（bit 数）

/* ========== 字符频率 ========== */
int freq_all[MAX_CHAR]; // [0]=EOF 哨兵，[32..126]=可见字符

/* ========== 写缓冲区 ========== */
unsigned char wbuf = 0; // 1 字节缓冲区
int wbits_left = 8;     // 缓冲区剩余位数（8→0）

/* ========== 全局文件指针 ========== */
FILE *Src, *Obj;

/* ========== 函数声明 ========== */
void statCount(void);
void buildHuffmanTree(void); // TODO：构建 Huffman 树，返回根节点索引
void genHuffmanCodes(tnode *root, int depth);
// TODO：递归遍历树，生成编码
void makeHCode();
void write_code(char ch, int);
void atoHZIP(void); // TODO：压缩
void reverseCodePath(int len);
void tranIntToArray(unsigned int code_int, int code_len);
void rebuildTree(int cPIndex, int len, tnode *cur);
int dealByte(int codePath[]);
void atoUnzip(void); // TODO：解压
int parseCommand(int argc, char *argv[], char **filename);
void printError(int type);
void dealFilename(char **filename, int type);
int isInsert(tnode *cur, tnode *in);
void insertTnode(node *dummy, node *in);
tnode *buildTnode(int ch, int weight);
node *buildNode(tnode *tPtr);
node *buildUnitNode(node *l, node *r);
void viz_tree(tnode *root);
void viz_dot(tnode *root, const char *filename);

char tree_viz_json[4096] = "";

// 递归构建 JSON 字符串，写入缓冲区
void build_json(tnode *node, char *buf, size_t *offset, size_t buf_size)
{
    if (node == NULL)
    {
        *offset += snprintf(buf + *offset, buf_size - *offset, "null");
        return;
    }
    // 节点文本
    char text[128];
    snprintf(text, sizeof(text), "ch:%d, w:%d", node->ch_val, node->weight);

    *offset += snprintf(buf + *offset, buf_size - *offset,
                        "{\"text\":\"%s\", \"children\":[", text);

    build_json(node->l, buf, offset, buf_size);
    *offset += snprintf(buf + *offset, buf_size - *offset, ",");
    build_json(node->r, buf, offset, buf_size);

    *offset += snprintf(buf + *offset, buf_size - *offset, "]}");
}

// 调用这个函数来填充全局缓冲区
void viz_tree(tnode *root)
{
    size_t offset = 0;
    memset(tree_viz_json, 0, sizeof(tree_viz_json));
    build_json(root, tree_viz_json, &offset, sizeof(tree_viz_json));
}

// 生成 Graphviz DOT 文件，可在 VSCode 中安装 Graphviz 扩展后直接预览
void viz_dot_r(tnode *node, FILE *fp)
{
    if (!node) return;
    if (node->l)
    {
        fprintf(fp, "  n%p [label=\"ch:%d\\nw:%d\"];\n",
                (void *)node, node->ch_val, node->weight);
        fprintf(fp, "  n%p [label=\"ch:%d\\nw:%d\"];\n",
                (void *)node->l, node->l->ch_val, node->l->weight);
        fprintf(fp, "  n%p -> n%p [label=\" 0\"];\n",
                (void *)node, (void *)node->l);
        viz_dot_r(node->l, fp);
    }
    if (node->r)
    {
        fprintf(fp, "  n%p [label=\"ch:%d\\nw:%d\"];\n",
                (void *)node, node->ch_val, node->weight);
        fprintf(fp, "  n%p [label=\"ch:%d\\nw:%d\"];\n",
                (void *)node->r, node->r->ch_val, node->r->weight);
        fprintf(fp, "  n%p -> n%p [label=\" 1\"];\n",
                (void *)node, (void *)node->r);
        viz_dot_r(node->r, fp);
    }
    // 叶节点：无子节点，只输出自身（node->l 和 node->r 都为 NULL 时上面不进入，这里补上）
    if (!node->l && !node->r)
    {
        fprintf(fp, "  n%p [label=\"ch:%d\\nw:%d\"];\n",
                (void *)node, node->ch_val, node->weight);
    }
}

void viz_dot(tnode *root, const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) return;
    fprintf(fp, "digraph Huffman {\n  node [fontname=\"Courier New\"];\n");
    viz_dot_r(root, fp);
    fprintf(fp, "}\n");
    fclose(fp);
}

int main(int argc, char *argv[])
{
    char *filename = NULL;
    int type = parseCommand(argc, argv, &filename);
    if (!type)
        return 0;

    Src = fopen(filename, "rb");
    dealFilename(&filename, type);
    Obj = fopen(filename, "wb");

    if (type == IS_ZIP)
    {
        /* ---- 压缩流水线 ---- */
        statCount();
        buildHuffmanTree();
        makeHCode();
        atoHZIP();
    }
    else
    {
        /* ---- 解压流水线 ---- */
        atoUnzip(); // TODO：读码表 + 建树 + 解码
    }

    fclose(Src);
    fclose(Obj);
    return 0;
}
void printError(int type) // 打印命令行错误信息
{
    if (type == IS_FORMAT)
        printf("Usage: hzip.exe [-u] <filename>\n");
    else
        printf("File extension error!\n");
}

int parseCommand(int argc, char *argv[], char **filename) // 解析命令行指令并输出文件名
{
    if (argc == 1 || argc > 3)
    {
        printError(IS_FORMAT);
        return WRONG;
    }
    if (!strcmp(argv[1], "-u") && argc == 3)
    {
        char *ext = strrchr(argv[2], '.');
        if (!ext || strcmp(ext, ".hzip"))
        {
            printError(IS_EXTEN);
            return WRONG;
        }
        *filename = argv[2];
        return IS_UNZIP;
    }
    if (argc == 2 && strstr(argv[1], "."))
    {
        char *ext = strrchr(argv[1], '.');
        if (!ext || strcmp(ext, ".txt"))
        {
            printError(IS_EXTEN);
            return WRONG;
        }
        *filename = argv[1];
        return IS_ZIP;
    }
    printError(IS_FORMAT);
    return WRONG;
}

void dealFilename(char **filename, int type) // 再处理文件名
{
    char *ext = strrchr(*filename, '.');
    if (!ext)
    {
        printf("ERROR_DEALNAME\n");
        return;
    }
    if (type == IS_ZIP && strcmp(ext, ".txt"))
    {
        printError(IS_EXTEN);
        return;
    }
    if (type == IS_UNZIP && strcmp(ext, ".hzip"))
    {
        printError(IS_EXTEN);
        return;
    }

    char *nameBuf = (char *)malloc(200);
    char *cur = *filename;
    int idx = 0;
    while (cur != ext)
        nameBuf[idx++] = *cur++;
    nameBuf[idx] = '\0';
    strcat(nameBuf, (type == IS_ZIP) ? ".hzip" : ".txt");
    *filename = nameBuf;
}

void statCount(void) // 统计字符
{
    int ch;
    freq_all[0] = 1; // EOF 哨兵
    while ((ch = fgetc(Src)) != EOF)
        freq_all[ch]++;
}
int isInsert(tnode *cur, tnode *in) // 判断是否插入结点
{
    // 判断 in是否插入到游标结点前(传入对应的tnode)
    if (cur->weight > in->weight)
        return IS_INSERT;
    if (cur->weight == in->weight)
    {
        if (in->ch_val == -1)
            return NO_INSERT;
        if (in->ch_val < cur->ch_val)
            return IS_INSERT;
        return NO_INSERT;
    }
    return NO_INSERT;
}
void insertTnode(node *dummy, node *in) // 将in插入到当前升序链表中
{
    node *head = dummy->next;
    if (head == NULL)
    {
        dummy->next = in;
        return;
    }
    node *front = dummy;
    while (head)
    {
        if (isInsert(head->tnodePtr, in->tnodePtr))
        {
            front->next = in;
            in->next = head;
            return;
        }
        front = head;
        head = head->next;
    }
    front->next = in;
}
tnode *buildTnode(int ch, int weight)
{
    tnode *newNode = (tnode *)malloc(sizeof(tnode));
    newNode->ch_val = ch, newNode->weight = weight;
    newNode->l = newNode->r = NULL;
    return newNode;
}
node *buildNode(tnode *tPtr)
{
    node *newN = (node *)malloc(sizeof(node));
    newN->next = NULL;
    newN->tnodePtr = tPtr;
    return newN;
}
node *buildUnitNode(node *l, node *r) // 创建huffman树中的父节点
{
    // front used for left child
    l->next = r->next = NULL;
    tnode *father = (tnode *)malloc(sizeof(tnode));
    father->weight = l->tnodePtr->weight + r->tnodePtr->weight;
    father->ch_val = -1;
    father->l = l->tnodePtr;
    father->r = r->tnodePtr;
    return buildNode(father);
}

void buildHuffmanTree(void) // 创建huffman树
{
    node dummy; // 栈上哑结点
    dummy.tnodePtr = NULL;
    dummy.next = NULL;
    /* 1. 创建叶节点 */
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (freq_all[i] > 0)
        {
            int ch = i, w = freq_all[i];
            insertTnode(&dummy, buildNode(buildTnode(ch, w)));
        }
    }
    // 进行从头开始的Huffman树构建
    while (dummy.next && dummy.next->next)
    {
        //
        node *l = dummy.next;
        node *r = l->next;
        dummy.next = r->next;
        insertTnode(&dummy, buildUnitNode(l, r));
        free(l), free(r);
    }
    // 此时只剩一个node
    Root = dummy.next->tnodePtr;
}

char path[MAX_CHAR];
void genHuffmanCodes(tnode *root, int depth) // recursion得到所有huffman编码
{
    // 深度从零开始，进行递归
    if (root->l == NULL && root->r == NULL)
    {
        // TODO: 叶节点：将 path_bits 中 depth 位拷贝到 code_bits[ch_val[root]]
        //       code_len[ch_val[root]] = depth;
        //       注意：depth=0 时（单字符），编码长度=1，编码='0'
        path[depth] = '\0';
        strcpy(code_bits[(char)root->ch_val], path);
        code_len[root->ch_val] = depth;
        return;
    }
    // TODO: 左子树 — path_bits[depth] 位写 0，递归
    // TODO: 右子树 — path_bits[depth] 位写 1，递归
    if (root->l)
    {
        path[depth] = '0';
        genHuffmanCodes(root->l, depth + 1);
    }
    if (root->r)
    {
        path[depth] = '1';
        genHuffmanCodes(root->r, depth + 1);
    }
}

void makeHCode()
{
    genHuffmanCodes(Root, 0);
}

void write_code(char ch, int type) // 根据type，输出一个字节的huffman码（缓冲区状态在 全局不需要维护）
{
    // ch是当前从FILE中读取的字符
    // 打印码表：每次都要输出完整字节
    int lenCh = code_len[ch];
    if (type == IS_TABLE)
    {
        fputc(ch, Obj);
        fputc(lenCh, Obj);
    }

    int cI = 0;
    for (; cI < lenCh; ++cI)
    {
        if (!wbits_left)
        {
            fputc(wbuf, Obj);
            wbuf = 0;
            wbits_left = 8;
        }
        wbuf = wbuf | (code_bits[ch][cI] - '0') << (wbits_left - 1);
        wbits_left--;
    }
    if (type == IS_TABLE && wbits_left != 8)
    {
        // 打印码表需要输出完整字节
        fputc(wbuf, Obj);
        wbuf = 0;
        wbits_left = 8;
    }
}

void atoHZIP(void) // 压缩，利用write_code进行输出即可
{
    int lenTable = 0;
    for (int i = 0; i < MAX_CHAR; ++i)
    {
        if (freq_all[i])
            lenTable++;
    }
    fputc(lenTable, Obj);

    for (int i = 0; i < MAX_CHAR; ++i)
    {
        if (freq_all[i])
            write_code(i, IS_TABLE);
    }

    rewind(Src); // 回跳原点,打印原文
    int ch = 0;
    while (1)
    {
        ch = fgetc(Src);
        if (ch == -1)
        {
            write_code(0, IS_TEXT);
            break;
        }
        write_code(ch, IS_TEXT);
    }
    if (wbits_left != 8)
    {
        // 最后原文可能有字节
        fputc(wbuf, Obj);
    }

    // 打印原文
}

int codePath[32]; // 存储码表中的编码
int codePIndex = 0;
int currCh = 0;
tnode dummy = {
    .ch_val = -1,//防止与EOF混淆
    .weight = 0,
    .r = NULL,
    .l = NULL};
tnode *treeCur = &dummy; // 树索引
void reverseCodePath(int len)
{
    for (int l = 0, r = len - 1; l <= r; l++, r--)
    {
        int tmp = codePath[r];
        codePath[r] = codePath[l];
        codePath[l] = tmp;
    }
}
void tranIntToArray(unsigned int code_int, int codeLen) // 将int编码转换成数组
{
    for (int i = 0; i < codeLen; ++i)
    {
        codePath[i] = code_int & 1;
        code_int >>= 1;
    }
    reverseCodePath(codeLen);
}
void rebuildTree(int cPIndex, int len, tnode *cur) // 根据编码路径重建huffman树
{
    // 根据数组在cur游标树结点上重建
    if (cPIndex >= len)
        return;
    if (!codePath[cPIndex])
    {
        // left zero
        if (cur->l)
        {
            rebuildTree(cPIndex + 1, len, cur->l);
            return;
        }
        // BUGFIX: 内部节点 ch_val=-1，与 buildUnitNode 保持一致，避免与 EOF 哨兵(0) 混淆
        cur->l = buildTnode(cPIndex + 1 == len ? currCh : -1, 0);
        rebuildTree(cPIndex + 1, len, cur->l);
        return;
    }
    if (cur->r)
    {
        rebuildTree(cPIndex + 1, len, cur->r);
        return;
    }
    cur->r = buildTnode(cPIndex + 1 == len ? currCh : -1, 0);
    rebuildTree(cPIndex + 1, len, cur->r);
    return;
}

int dealByte(int codePath[])
{
    // 每次处理 1 字节（8 bit），treeCur 保持跨字节的树遍历状态
    int ans = 1;
    for (int i = 0; i < 8; ++i)
    {
        // BUGFIX: 先检查子节点是否存在再跳转，避免码表损坏时 NULL 解引用导致段错误
        if (codePath[i] == 1)
        {
            if (treeCur->r == NULL) return 0; // 路径断裂，提前终止解压
            treeCur = treeCur->r;
        }
        else
        {
            if (treeCur->l == NULL) return 0;
            treeCur = treeCur->l;
        }
        if (treeCur->l == NULL && treeCur->r == NULL)
        {
            fputc(treeCur->ch_val, Obj);
            if (!treeCur->ch_val)
                ans = 0;
            treeCur = &dummy; // 命中叶节点，重置到根准备下一段编码
        }
    }
    return ans;
}

void atoUnzip(void)
{
    rewind(Src);
    int lenTable = fgetc(Src);
    for (int i = 0; i < lenTable; ++i)
    {
        int ch_val = fgetc(Src); // key
        currCh = ch_val;
        int code_len = fgetc(Src); // 码长
        unsigned int code_int = 0; // 存储编码为int：码表是整个字节存储
        // BUGFIX: ceil(code_len/8) 的正确写法，避免 code_len 为 8 的倍数时多读 1 字节
        int byte_cnt = (code_len + 7) / 8;
        for (int j = 0; j < byte_cnt; ++j)
        {
            code_int = code_int << 8 | fgetc(Src);
        }
        // BUGFIX: 多余位移需取模，code_len%8==0 时不右移
        code_int = code_int >> ((8 - (code_len % 8)) % 8);
        // 建树
        tranIntToArray(code_int, code_len);
        rebuildTree(0, code_len, &dummy);
    }

    // 根据实际编码内容，反编码原文
    /*
    1字节缓冲区：储存当前一个字节（可能是分散）
        每次需要处理完一个字节中的所有信息：即如果中间有到达叶节点需要输出然后重置treeCur,然后继续遍历treeCur
    设置一个treeCur用于索引当前遍历位置
    */

    int flag = 1;
    while (flag)
    {
        wbuf = fgetc(Src);
        tranIntToArray(wbuf, 8);
        flag = dealByte(codePath);
    }
}
