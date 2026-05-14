#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 全局写缓冲区（用于压缩时按位输出） */
static unsigned char write_byte = 0;
static int write_bits_left = 8;

/* Huffman树节点数组（压缩时构建） */
#define MAX_NODES 512
static int freq[MAX_NODES];
static unsigned char ch[MAX_NODES];
static int left[MAX_NODES];
static int right[MAX_NODES];
static int node_count;

/* 字符编码存储 */
static unsigned char code_bits[256][32];
static int code_len[256];

/* 函数声明 */
void write_bits(FILE *f, const unsigned char *bits, int len);
void flush_bits(FILE *f);
void build_codes(int node, int depth, unsigned char *path);
void compress(const char *filename);
void decompress(const char *filename);
char *get_ext(char *filename);

/* 按位写入（高位在前） */
void write_bits(FILE *f, const unsigned char *bits, int len) {
    for (int i = 0; i < len; i++) {
        int byte_idx = i / 8;
        int bit_in_byte = 7 - (i % 8);
        int bit = (bits[byte_idx] >> bit_in_byte) & 1;
        if (write_bits_left == 0) {
            fputc(write_byte, f);
            write_byte = 0;
            write_bits_left = 8;
        }
        if (bit) {
            write_byte |= (1 << (write_bits_left - 1));
        }
        write_bits_left--;
    }
}

/* 将未满的字节补0输出，重置写缓冲区 */
void flush_bits(FILE *f) {
    if (write_bits_left < 8) {
        fputc(write_byte, f);
        write_byte = 0;
        write_bits_left = 8;
    }
}

/* 递归生成Huffman编码，path按位记录（高位在前） */
void build_codes(int node, int depth, unsigned char *path) {
    if (left[node] == -1 && right[node] == -1) {
        int c = ch[node];
        code_len[c] = depth;
        if (depth > 0) {
            int bytes = (depth + 7) / 8;
            memcpy(code_bits[c], path, bytes);
        }
        return;
    }
    if (left[node] != -1) {
        int byte_idx = depth / 8;
        int bit_in_byte = 7 - (depth % 8);
        path[byte_idx] &= ~(1 << bit_in_byte);      /* 写0 */
        build_codes(left[node], depth + 1, path);
    }
    if (right[node] != -1) {
        int byte_idx = depth / 8;
        int bit_in_byte = 7 - (depth % 8);
        path[byte_idx] |= (1 << bit_in_byte);       /* 写1 */
        build_codes(right[node], depth + 1, path);
    }
}

/* 压缩文件 */
void compress(const char *filename) {
    /* 1. 统计频率 */
    int freq_all[256] = {0};
    FILE *fin = fopen(filename, "rb");
    if (!fin) {
        fprintf(stderr, "File open error\n");
        exit(1);
    }
    int c;
    while ((c = fgetc(fin)) != EOF) {
        freq_all[c]++;
    }
    fclose(fin);
    freq_all[0] = 1;   /* 结束符 */

    /* 2. 创建叶子节点 */
    node_count = 0;
    for (int i = 0; i < 256; i++) {
        if (freq_all[i] > 0) {
            ch[node_count] = (unsigned char)i;
            freq[node_count] = freq_all[i];
            left[node_count] = right[node_count] = -1;
            node_count++;
        }
    }

    /* 3. 排序列表初始化 */
    int list[MAX_NODES];
    int list_size = node_count;
    for (int i = 0; i < list_size; i++) list[i] = i;

    /* 按频率升序，频率相同按ASCII升序 */
    for (int i = 0; i < list_size - 1; i++) {
        for (int j = i + 1; j < list_size; j++) {
            int fi = freq[list[i]], fj = freq[list[j]];
            int ci = ch[list[i]], cj = ch[list[j]];
            if (fi > fj || (fi == fj && ci > cj)) {
                int tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
        }
    }

    /* 4. 构建Huffman树 */
    if (list_size == 1) {
        /* 单字符情况：创建伪内部节点，左孩子为叶子，编码为0 */
        int leaf = list[0];
        ch[node_count] = 0;
        freq[node_count] = freq[leaf];
        left[node_count] = leaf;
        right[node_count] = -1;
        list[0] = node_count;
        node_count++;
    } else {
        while (list_size > 1) {
            int left_idx = list[0];
            int right_idx = list[1];
            ch[node_count] = 0;
            freq[node_count] = freq[left_idx] + freq[right_idx];
            left[node_count] = left_idx;
            right[node_count] = right_idx;
            int new_idx = node_count++;

            /* 移除前两个节点 */
            for (int i = 0; i < list_size - 2; i++) list[i] = list[i + 2];
            list_size -= 2;

            /* 插入新节点（等频率时插入到同频率节点之后） */
            int pos = list_size;
            while (pos > 0 && freq[list[pos - 1]] > freq[new_idx]) pos--;
            for (int i = list_size; i > pos; i--) list[i] = list[i - 1];
            list[pos] = new_idx;
            list_size++;
        }
    }
    int root = list[0];

    /* 5. 生成编码 */
    unsigned char path[32] = {0};
    build_codes(root, 0, path);

    /* 6. 写压缩文件 */
    char outname[256];
    strcpy(outname, filename);
    char *dot = strrchr(outname, '.');
    if (dot) *dot = '\0';
    strcat(outname, ".hzip");
    FILE *fout = fopen(outname, "wb");
    if (!fout) {
        fprintf(stderr, "File open error\n");
        exit(1);
    }

    /* 码表大小 */
    unsigned char table_size = 0;
    for (int i = 0; i < 256; i++) if (freq_all[i] > 0) table_size++;
    fputc(table_size, fout);

    /* 码表项 */
    for (int i = 0; i < 256; i++) {
        if (freq_all[i] > 0) {
            fputc(i, fout);                       /* ASCII码 */
            fputc(code_len[i], fout);             /* 码长   */
            write_bits(fout, code_bits[i], code_len[i]);
            flush_bits(fout);                     /* 补齐整字节 */
        }
    }

    /* 压缩数据 */
    fin = fopen(filename, "rb");
    while ((c = fgetc(fin)) != EOF) {
        write_bits(fout, code_bits[c], code_len[c]);
    }
    fclose(fin);
    write_bits(fout, code_bits[0], code_len[0]); /* 结束符 */
    flush_bits(fout);
    fclose(fout);
}

/* 解压文件 */
void decompress(const char *filename) {
    FILE *fin = fopen(filename, "rb");
    if (!fin) {
        fprintf(stderr, "File open error\n");
        exit(1);
    }

    int table_size = fgetc(fin);
    if (table_size == EOF) {
        fprintf(stderr, "Invalid hzip file\n");
        exit(1);
    }

    /* 解压树初始化 */
    int left_d[512], right_d[512], leaf_char[512];
    for (int i = 0; i < 512; i++) {
        left_d[i] = right_d[i] = -1;
        leaf_char[i] = -1;
    }
    int node_cnt = 1;  /* 0号节点为根 */

    /* 读取码表并建树 */
    for (int i = 0; i < table_size; i++) {
        int ascii = fgetc(fin);
        int len = fgetc(fin);
        int bytes = (len + 7) / 8;
        unsigned char buf[32] = {0};
        fread(buf, 1, bytes, fin);

        int node = 0;
        for (int j = 0; j < len; j++) {
            int byte_idx = j / 8;
            int bit_in_byte = 7 - (j % 8);
            int bit = (buf[byte_idx] >> bit_in_byte) & 1;
            if (bit == 0) {
                if (left_d[node] == -1) left_d[node] = node_cnt++;
                node = left_d[node];
            } else {
                if (right_d[node] == -1) right_d[node] = node_cnt++;
                node = right_d[node];
            }
        }
        leaf_char[node] = ascii;
    }

    /* 输出文件名 */
    char outname[256];
    strcpy(outname, filename);
    char *dot = strrchr(outname, '.');
    if (dot) *dot = '\0';
    strcat(outname, ".txt");
    FILE *fout = fopen(outname, "wb");
    if (!fout) {
        fprintf(stderr, "File open error\n");
        exit(1);
    }

    /* 解码 */
    int bits_left = 0;
    unsigned char byte_buf = 0;
    int node = 0;
    while (1) {
        if (bits_left == 0) {
            int c = fgetc(fin);
            if (c == EOF) break;
            byte_buf = (unsigned char)c;
            bits_left = 8;
        }
        int bit = (byte_buf >> 7) & 1;
        byte_buf <<= 1;
        bits_left--;

        node = (bit == 0) ? left_d[node] : right_d[node];
        if (node == -1) break;  /* 错误 */
        if (left_d[node] == -1 && right_d[node] == -1) {
            if (leaf_char[node] == 0) break; /* 结束符 */
            fputc(leaf_char[node], fout);
            node = 0;
        }
    }
    fclose(fout);
    fclose(fin);
}

/* 提取文件扩展名（小写） */
char *get_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    return dot ? dot + 1 : "";
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(get_ext(argv[1]), "txt") != 0) {
            printf("File extension error!\n");
            return 1;
        }
        compress(argv[1]);
    } else if (argc == 3) {
        if (strcmp(argv[1], "-u") != 0) {
            printf("Usage: hzip.exe [-u] <filename>\n");
            return 1;
        }
        if (strcmp(get_ext(argv[2]), "hzip") != 0) {
            printf("File extension error!\n");
            return 1;
        }
        decompress(argv[2]);
    } else {
        printf("Usage: hzip.exe [-u] <filename>\n");
        return 1;
    }
    return 0;
}