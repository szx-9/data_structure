/* ============================================================
   Huffman 压缩/解压 — hzip.c
   框架重构：数组法 Huffman 树 + 流式位输出
   TODO 标记的函数由你来补全实现
   ============================================================ */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ========== 常量 ========== */
#define MAX_CHAR 128          // ASCII 0~127，考虑退化成链表，ascii范围内最大安全数值
#define MAX_NODE 256          // 总结点数 ≤ 2*128-1 = 255

enum {
    IS_ZIP   = 1,
    IS_UNZIP = 2,
    WRONG    = 0,
    IS_FORMAT = 1,
    IS_EXTEN  = 0
};

/* ========== Huffman 树（数组表示） ========== */
int  lch[MAX_NODE], rch[MAX_NODE];   // 左右孩子索引，-1 = 无
int  freq[MAX_NODE];                 // 节点权重
int  ch_val[MAX_NODE];               // 叶节点字符（内部节点无效）
int  node_cnt;                       // 已分配节点数

/* ========== 编码表 ========== */
unsigned char code_bits[MAX_CHAR][16];  // 每个字符的 Huffman 编码（位数组，高位在前）
int code_len[MAX_CHAR];                 // 编码长度（bit 数）

/* ========== 字符频率 ========== */
int freq_all[MAX_CHAR];   // [0]=EOF 哨兵，[32..126]=可见字符

/* ========== 写缓冲区 ========== */
unsigned char wbuf;       // 1 字节缓冲区
int wbits_left;           // 缓冲区剩余位数（8→0）

/* ========== 全局文件指针 ========== */
FILE *Src, *Obj;

/* ========== 函数声明 ========== */
void statCount(void);
int  buildHuffmanTree(void);                          // TODO：构建 Huffman 树，返回根节点索引
void genHuffmanCodes(int root, int depth,
                     unsigned char *path_bits);       // TODO：递归遍历树，生成编码
void makeHCode(int root);
void write_bit(FILE *f, int bit);
void flush_bits(FILE *f);
void write_code(FILE *f, int c);                      // TODO：写入字符 c 的完整编码
void atoHZIP(void);                                   // TODO：压缩
void atoUnzip(void);                                  // TODO：解压
int  parseCommand(int argc, char *argv[], char **filename);
void printError(int type);
void dealFilename(char **filename, int type);

/* ============================================================
   main：流程纲要
   ============================================================ */
int main(int argc, char *argv[])
{
    char *filename = NULL;
    int type = parseCommand(argc, argv, &filename);
    if (!type) return 0;

    Src = fopen(filename, "rb");
    if (!Src) { fprintf(stderr, "Cannot open input file!\n"); return 1; }
    dealFilename(&filename, type);
    Obj = fopen(filename, "wb");
    if (!Obj) { fprintf(stderr, "Cannot open output file!\n"); fclose(Src); return 1; }

    if (type == IS_ZIP) {
        /* ---- 压缩流水线 ---- */
        statCount();                              // ① 统计频率（保留你的实现）
        int root = buildHuffmanTree();            // ② TODO：构建树
        makeHCode(root);                          // ③ 生成编码（调用你的 genHuffmanCodes）
        atoHZIP();                                // ④ TODO：写码表 + 编码压缩
    } else {
        /* ---- 解压流水线 ---- */
        atoUnzip();                               // TODO：读码表 + 建树 + 解码
    }

    fclose(Src);
    fclose(Obj);
    free(filename);
    return 0;
}

/* ============================================================
   parseCommand / printError / dealFilename（保留你原有实现，微调）
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
    if (argc == 1 || argc > 3) { printError(IS_FORMAT); return WRONG; }
    if (!strcmp(argv[1], "-u") && argc == 3) {
        char *ext = strrchr(argv[2], '.');
        if (!ext || strcmp(ext, ".hzip")) { printError(IS_EXTEN); return WRONG; }
        *filename = argv[2];
        return IS_UNZIP;
    }
    if (argc == 2 && strstr(argv[1], ".")) {
        char *ext = strrchr(argv[1], '.');
        if (!ext || strcmp(ext, ".txt")) { printError(IS_EXTEN); return WRONG; }
        *filename = argv[1];
        return IS_ZIP;
    }
    printError(IS_FORMAT);
    return WRONG;
}

void dealFilename(char **filename, int type)
{
    char *ext = strrchr(*filename, '.');
    if (!ext) { printf("ERROR_DEALNAME\n"); return; }
    if (type == IS_ZIP && strcmp(ext, ".txt"))   { printError(IS_EXTEN); return; }
    if (type == IS_UNZIP && strcmp(ext, ".hzip")) { printError(IS_EXTEN); return; }

    char *nameBuf = (char *)malloc(200);
    char *cur = *filename;
    int idx = 0;
    while (cur != ext) nameBuf[idx++] = *cur++;
    nameBuf[idx] = '\0';
    strcat(nameBuf, (type == IS_ZIP) ? ".hzip" : ".txt");
    *filename = nameBuf;
}

/* ============================================================
   statCount（保留你原有实现）
   题目要求：EOF 哨兵 count=1，参与编码
   ============================================================ */
void statCount(void)
{
    int ch;
    freq_all[0] = 1;           // EOF 哨兵
    while ((ch = fgetc(Src)) != EOF)
        freq_all[ch]++;
}

/* ============================================================
   buildHuffmanTree — TODO
   ─────────────────────────────────────────────
   用数组法构建 Huffman 树，返回根节点索引。
   提示步骤：
     1. 遍历 freq_all[0..127]，为 freq>0 的字符创建叶节点
     2. 用插入排序建一个 list[] 数组（存节点下标），按 (freq, ch) 升序
     3. while (list_size > 1):
          - 取 list[0], list[1] 作为左右孩子
          - 创建新内部节点：freq=两者之和, lch=左, rch=右
          - 删除前两个元素，将新节点插入到正确位置（稳定）
     4. 返回 list[0]
   注意：
     - 插入排序的稳定性保证与参考 exe 逐字节一致
     - 如果只有一个叶节点，需要特殊处理（建一个哑父节点，给 0 编码）
     - 节点总数 ≤ node_cnt（初始 = 叶节点数）
   ============================================================ */
int buildHuffmanTree(void)
{
    int list[MAX_NODE];
    int list_size = 0;

    /* 1. 创建叶节点 */
    node_cnt = 0;
    for (int i = 0; i < MAX_CHAR; i++) {
        if (freq_all[i] > 0) {
            // TODO: 分配新节点，设置 lch/rch = -1, ch_val, freq
            // TODO: 把节点下标加入 list
        }
    }

    /* 2. 初始排序（升序：freq 小的在前；相同按 ch_val 小的在前） */
    // TODO: 插入排序（或其他稳定排序）list

    /* 3. 主循环：合并两个最小节点 */
    // TODO: while (list_size > 1) { ... }

    /* 4. 单字符特判 */
    // TODO: 如果初始只有 1 个叶节点

    return list[0];  // 根节点下标
}

/* ============================================================
   genHuffmanCodes — TODO
   ─────────────────────────────────────────────
   递归遍历树，为每个叶节点生成 Huffman 编码（存到 code_bits / code_len）。
   参数：
     - root:   当前节点下标
     - depth:  当前深度（= 已生成的编码位数）
     - path_bits: 路径位数组（调用方分配 unsigned char path[16]）
   规则：左走写 0，右走写 1，到达叶节点时 copy 到全局表。
   ============================================================ */
void genHuffmanCodes(int root, int depth, unsigned char *path_bits)
{
    if (lch[root] == -1 && rch[root] == -1) {
        // TODO: 叶节点：将 path_bits 中 depth 位拷贝到 code_bits[ch_val[root]]
        //       code_len[ch_val[root]] = depth;
        //       注意：depth=0 时（单字符），编码长度=1，编码='0'
        return;
    }
    // TODO: 左子树 — path_bits[depth] 位写 0，递归
    // TODO: 右子树 — path_bits[depth] 位写 1，递归
}

/* ============================================================
   makeHCode：编码生成入口
   ============================================================ */
void makeHCode(int root)
{
    unsigned char path[16] = {0};
    genHuffmanCodes(root, 0, path);
}

/* ============================================================
   write_bit / flush_bits：流式位输出（已实现，直接使用）
   ─────────────────────────────────────────────
   write_bit: 向文件输出 1 bit（高位在前）
   flush_bits: 将缓冲区剩余位补 0 写出
   用法：
     write_bit(f, 1);          // 输出 '1'
     write_code(f, ascii);     // 输出某字符的完整编码（你需要实现）
     flush_bits(f);            // 结束时冲刷
   ============================================================ */
void write_bit(FILE *f, int bit)
{
    wbuf = (unsigned char)((wbuf << 1) | (bit & 1));
    wbits_left--;
    if (wbits_left == 0) {
        fputc(wbuf, f);
        wbuf = 0;
        wbits_left = 8;
    }
}

void flush_bits(FILE *f)
{
    if (wbits_left < 8) {
        wbuf = (unsigned char)(wbuf << wbits_left);   // 低位补 0
        fputc(wbuf, f);
        wbuf = 0;
        wbits_left = 8;
    }
}

/* ============================================================
   write_code — TODO
   ─────────────────────────────────────────────
   将字符 c 对应的 Huffman 编码按位写入文件。
   直接调用 write_bit() 循环 code_len[c] 次即可。
   ============================================================ */
void write_code(FILE *f, int c)
{
    // TODO: for i in 0..code_len[c]-1:
    //         提取 code_bits[c] 的第 i 位（高位在前）
    //         调用 write_bit(f, bit)
}

/* ============================================================
   atoHZIP — TODO
   ─────────────────────────────────────────────
   压缩输出，分为两步：
     1. 写码表：
        - 1 字节：tblCnt（码表条目数，freq_all[i]>0 的个数）
        - 对每个 freq_all[i]>0 的字符：
            fputc(i)                          // ASCII 码
            fputc(code_len[i])                // 码长
            write_code(Obj, i)                // 编码位串
            flush_bits(Obj)                   // 补齐整字节
     2. 写压缩数据：
        - rewind(Src)
        - 逐字符读 Src，write_code(Obj, ch)
        - write_code(Obj, 0)                  // EOF 哨兵
        - flush_bits(Obj)                     // 最后冲刷
   ============================================================ */
void atoHZIP(void)
{
    /* 1. 写码表 */
    // TODO: 统计 tblCnt，写入 fputc(tblCnt, Obj)
    // TODO: 遍历 0..127，对 freq_all[i]>0 的写 (i, code_len[i], code_bits[i])

    /* 2. 写压缩数据 */
    // TODO: rewind(Src)，逐字符 write_code，最后写哨兵+flush
}

/* ============================================================
   atoUnzip — TODO
   ─────────────────────────────────────────────
   解压，分为两步：
     1. 读码表 → 重建解码 Huffman 树（数组法）：
        - 读 1 字节 tblCnt
        - 对每条目：读 ascii, codeLen, code_bits 字节串
        - 沿路径插入节点（0 走左，1 走右）
        - 叶节点设置 ch_val = ascii
     2. 解码数据：
        - 从根节点出发，逐位读（MSB 优先）
        - 到达叶 → 遇到 0 号字符（EOF）停止，否则 fputc(ch, Obj)
        - 注意处理 file end 保护
   ============================================================ */
void atoUnzip(void)
{
    // TODO: 全部由你实现
    // 提示结构：
    //   1. 初始化解码树：node_cnt=1, lch[0]=rch[0]=-1（0 号节点=根）
    //   2. 读码表，沿路径建树
    //   3. 逐位读取 Src，在解码树上走，叶节点输出
    //   4. 遇到 ch=0 的叶节点 → 停止
}
