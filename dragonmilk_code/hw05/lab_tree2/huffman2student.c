//文件压缩-Huffman实现
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 32

struct tnode {					//Huffman树结构
	char c;		
	int weight;					//树节点权重，叶节点为字符和它的出现次数
	struct tnode *left,*right;
} ; 
int Ccount[128]={0};			//存放每个字符的出现次数，如Ccount[i]表示ASCII值为i的字符出现次数 
struct tnode *Root=NULL; 		//Huffman树的根节点
char HCode[128][MAXSIZE]={{0}}; //字符的Huffman编码，如HCode['a']为字符a的Huffman编码（字符串形式） 
int Step=0;						//实验步骤 
FILE *Src, *Obj;
	
void statCount();				//步骤1：统计文件中字符频率
void createHTree();				//步骤2：创建一个Huffman树，根节点为Root 
void makeHCode();				//步骤3：根据Huffman树生成Huffman编码
void atoHZIP(); 				//步骤4：根据Huffman编码将指定ASCII码文本文件转换成Huffman码文件

void print1();					//输出步骤1的结果
void print2(struct tnode *p);	//输出步骤2的结果 
void print3();					//输出步骤3的结果
void print4();					//输出步骤4的结果


int main()
{
	if((Src=fopen("input.txt","r"))==NULL) {
		fprintf(stderr, "%s open failed!\n", "input.txt");
		return 1;
	}
	if((Obj=fopen("output.txt","w"))==NULL) {
		fprintf(stderr, "%s open failed!\n", "output.txt");
		return 1;
	}
	scanf("%d",&Step);					//输入当前实验步骤 
	
	statCount();						//实验步骤1：统计文件中字符出现次数（频率）
	if(Step==1) 
		print1(); 			//输出实验步骤1结果	
	createHTree();						//实验步骤2：依据字符频率生成相应的Huffman树
	if(Step==2) 
		print2(Root); 		//输出实验步骤2结果	
	makeHCode();				   		//实验步骤3：依据Root为树的根的Huffman树生成相应Huffman编码
	if(Step==3) 
		print3(); 			//输出实验步骤3结果
	if(Step>=4) 
		atoHZIP(),print4(); 	//实验步骤4：据Huffman编码生成压缩文件，并输出实验步骤4结果	

	fclose(Src);
	fclose(Obj);

    return 0;
} 

//【实验步骤1】开始 
char str[1000000];
int strcnt;
void statCount()
{
	char ch; 
	Ccount[0] = 1;
	while((ch = fgetc(Src)) != EOF)
	{
		if(ch == '\0') break;
		if(ch != '\n') Ccount[ch]++,str[strcnt++] = ch;
	}
}

//【实验步骤1】结束

//【实验步骤2】开始

void insertNode(struct tnode **nodes, int *count, struct tnode *node) {
    int i = *count - 1;
    

    while (i >= 0 && ((nodes[i]->weight > node->weight) || (nodes[i]->weight == node->weight && nodes[i]->c > node->c))) 
	{
        nodes[i + 1] = nodes[i];
        i--;
    }
    nodes[i + 1] = node;
    (*count)++;
}
struct tnode *nodes[512] = {NULL};
void createHTree() {
    
    int count = 0 , i;

    // 创建初始叶节点并排序
    for (i = 0; i < 128; i++) {
        if (Ccount[i] > 0) {
            struct tnode *p = (struct tnode*)malloc(sizeof(struct tnode));
            p->c = i;
            p->weight = Ccount[i];
            p->left = p->right = NULL;
            insertNode(nodes, &count, p);
        }
    }
	
    // 合并节点构建Huffman树
    while (count > 1) {
        struct tnode *left = nodes[0];
        struct tnode *right = nodes[1];

        struct tnode *merged = (struct tnode*)malloc(sizeof(struct tnode));
        merged->weight = left->weight + right->weight;
        merged->left = left;
        merged->right = right;
        merged->c = 127;

        // 移除前两个节点
        for (i = 2; i < count; i++) {
            nodes[i - 2] = nodes[i];
        }
        count -= 2;
        // 插入合并节点，保持有序性
        insertNode(nodes, &count, merged);
    }

    Root = (count == 1) ? nodes[0] : NULL;
}

//【实验步骤2】结束

//【实验步骤3】开始
char tmp[32];
void dfs(struct tnode* t,int d)
{
	if(t == NULL) return ;
	if(t->left == NULL && t->right == NULL) 
	{
		int i;
		tmp[d] = '\0';
		for(i = 0 ; i <= d ; ++i) HCode[t->c][i] = tmp[i];
		return ;
	}
	tmp[d] = '0';
	dfs(t->left,d+1);
	tmp[d] = '1';
	dfs(t->right,d+1);
}
void makeHCode()
{
	dfs(Root,0);
} 

//【实验步骤3】结束

//【实验步骤4】开始

int hc;
char s[1000000]={0};
void atoHZIP()
{
	int i , len = 0;
	for(i = 0 ; i < strcnt ; ++i)
	{
		strcat(s,HCode[str[i]]); 
	}
	strcat(s,HCode[0]);
	strcat(s,"");
	//printf("%s\n",s);
	for(i=0; s[i] != '\0'; i++) {
	
	    hc = (hc << 1) | (s[i]-'0');
	
		if((i+1)%8 == 0) {
				
			fputc(hc,Obj);     //输出到目标（压缩）文件中
	
			printf("%x",hc);   //按十六进制输出到屏幕上
			hc = 0;
		}
	}	
}

//【实验步骤4】结束

void print1()
{
	int i;
	printf("NUL:1\n");
	for(i=1; i<128; i++)
		if(Ccount[i] > 0)
			printf("%c:%d\n", i, Ccount[i]);
}

void print2(struct tnode *p)
{
	if(p != NULL){
		if((p->left==NULL)&&(p->right==NULL)) 
			switch(p->c){
				case 0: printf("NUL ");break;
				case ' ':  printf("SP ");break;
				case '\t': printf("TAB ");break;
				case '\n':  printf("CR ");break;
				default: printf("%c ",p->c); break;
			}
		print2(p->left);
		print2(p->right);
	}
}

void print3()
{
	int i;
	
	for(i=0; i<128; i++){
		if(HCode[i][0] != 0){
			switch(i){
				case 0: printf("NUL:");break;
				case ' ':  printf("SP:");break;
				case '\t': printf("TAB:");break;
				case '\n':  printf("CR:");break;
				default: printf("%c:",i); break;
			}
			printf("%s\n",HCode[i]);
		}
	}
} 

void print4()
{
	long int in_size, out_size;
	
	fseek(Src,0,SEEK_END);
	fseek(Obj,0,SEEK_END);
	in_size = ftell(Src);
	out_size = ftell(Obj);
	
	printf("\n原文件大小：%ldB\n",in_size);
	printf("压缩后文件大小：%ldB\n",out_size);
	printf("压缩率：%.2f%%\n",(float)(in_size-out_size)*100/in_size);
}
