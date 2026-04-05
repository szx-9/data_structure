#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
#define MaxLen 125
#define MaxName 52
#define MaxAuthor 22
#define MaxPub 32
#define MaxDate 12

/*
1打开文件
2while 以循环读入一行为标准，将全部信息缓存到buf中


3 （不能在main函数while循环中创建结构体，生命周期仅在循环内），
main函数中创建一个结构体指针dummy并用malloc分配内存，

读取功能（读取文件或者控制台输入流）：
	首先利用buf和fgets读取某一行的全部信息，将其传入函数，在函数内存为信息申请内存
	并将链表尾某个节点（从头开始遍历，遇到第一个顺序大的，则利用front指针，进行插入）
	（需特判结尾）
查找功能：
	输入buf，从链表头节点开始遍历，遇到含有的项，直接调用print
删除：
	从头开始遍历，遇到target调用free，同时更新lenList
*/

typedef struct {
	char* name;
	char* author;
	char* publish;
	char* date;
	int lenName;
	int lenAuthor;
	int lenPub;
	int lenDate;
	struct infoNode* next;//每一个变量都要用分号
}infoNode;//创建结构体infoNode用于存储书信息，



void insertInfoOrder(infoNode* dummy, int lenList, char*);
void findInfo(infoNode*, int, char*);
void printInfo(infoNode*);
void delete(infoNode*, int*, char*);
void freeInfo(infoNode*);
void printInfoTofile(FILE*,infoNode*);

char buf[MaxLen];//全局设置，用于读取一行的全部信息或者增加信息
int main()
{
	FILE* fin = fopen("books.txt", "r");//只读取
	FILE* fout = fopen("ordered.txt", "w");
	infoNode* dummy=(infoNode*)malloc(sizeof(infoNode));//默认构造为空，指针全部空，需要进行next指向
	dummy->next = NULL;
	infoNode* head = dummy;

	int lenList = 0;	
	while ( fgets(buf, sizeof(buf), fin) ) {//此处用sizeof代替硬编码，保证不会
		//溢出，同时注意只能传入数组而指针不行，因为指针内存默认
		insertInfoOrder(dummy, lenList, buf);
		lenList++;
	}
	int type;
	while (1 ) {//scanf返回值是成功读取的个数
		scanf("%d", &type);
		getchar();
		switch (type) {
		case 0:
			goto a;
		case 1:
			fgets(buf, sizeof(buf), stdin);
			insertInfoOrder(dummy, lenList, buf);
			lenList++;
			break;
		case 2:
			scanf("%s", buf);//fgets会默认吸收换行符，需要手动去除
			getchar();
			findInfo(dummy, lenList, buf);
			break;
		case 3:
			scanf("%s", buf);//不会将空白符读入na
			getchar();
			delete(dummy, &lenList, buf);
			break;
		}
	}
a:	dummy = dummy->next;
	while (dummy) {
	//对所有 内存进行释放
		infoNode* res = dummy;
		dummy = dummy->next;
		printInfoTofile(fout, res);
		freeInfo(res);
	}
	free(head);
	fclose(fout);
	fclose(fin);
	return 0;
}


void printInfoTofile(FILE* fout, infoNode* cur) {
	int lenName = cur->lenName;
	int lenAuthor = cur->lenAuthor;
	int lenPub = cur->lenPub;
	int lenDate = cur->lenDate;
	fprintf(fout,"%s", cur->name);
	for (int i = 0; i < 50 - lenName; ++i) {
		fprintf(fout," ");
	}
	fprintf(fout,"%s", cur->author);
	for (int i = 0; i < 20 - lenAuthor; ++i) {
		fprintf(fout," ");
	}
	fprintf(fout,"%s", cur->publish);
	for (int i = 0; i < 30 - lenPub; ++i) {
		fprintf(fout," ");
	}
	fprintf(fout,"%s", cur->date);
	for (int i = 0; i < 10 - lenDate; ++i) {
		fprintf(fout," ");
	}
	fprintf(fout,"\n");
}


void freeInfo(infoNode* cur) {
//将cur申请的所有内存全部free
	free(cur->name);
	free(cur->author);
	free(cur->publish);
	free(cur->date);
	free(cur);
}

void delete(infoNode* dummy, int* lenList, char* buf) {
//对于dummy链表中所有含有buf的节点进行删除、
	infoNode* front = dummy;
	if (!lenList)return;
	
	dummy = dummy->next;
	while (dummy) {//dummy作为游标指针，遍历直到NULL
		if (strstr(dummy->name, buf)) {
			dummy = dummy->next;
			freeInfo(front->next);
			front->next=dummy;
			(*lenList)--;
			continue;//重要，否则dummy会变成NULL
		}
		front = dummy;
		dummy = dummy->next;//此处有null访问

	}
}

void printInfo(infoNode* cur) {
//输出一个节点的内容
	int lenName = cur->lenName;
	int lenAuthor = cur->lenAuthor;
	int lenPub = cur->lenPub;
	int lenDate = cur->lenDate;
	printf("%s", cur->name);
	for (int i = 0; i < 50 - lenName; ++i) {
		printf(" ");
	}
	printf("%s", cur->author);
	for (int i = 0; i < 20 - lenAuthor; ++i) {
		printf(" ");
	}
	printf("%s", cur->publish);
	for (int i = 0; i < 30 - lenPub; ++i) {
		printf(" ");
	}
	printf("%s", cur->date);
	for (int i = 0; i < 10 - lenDate; ++i) {
		printf(" ");
	}
	printf("\n");

}

void findInfo(infoNode* dummy, int lenList, char* buf) {
//从链表中查找所有带有关键字的信息，并进行输出
	while (lenList--) {
		dummy = dummy->next;
		if (strstr(dummy->name, buf)) {//如果找到了
			printInfo(dummy);
		}
	}
}



void insertInfoOrder(infoNode* dummy, int lenList,char* buf) {
	//dummy与len保证给出了图书系统的链表，cur是当前在外部申请的新info节点地址
	infoNode* cur=(infoNode*)malloc(sizeof(infoNode));
	//1首先为cur申请内存
	cur->name = (char*)malloc(MaxName);//malloc释放的时候根据申请时的字节进行释放，不用担心泄露
	cur->author = (char*)malloc(MaxAuthor);//->会根据结构体内存分布进行访问内存
	cur->publish = (char*)malloc(MaxPub);
	cur->date = (char*)malloc(MaxDate);
	cur->next = NULL;//地址空，即暂时不指向任何元素
	
	int bufIndex = 0;//用于索引buf数组，
	//2处理buf数组中的info
	for (int i = 0; i < MaxName+1; ++i) {
		if (buf[i] == ' ') {
			cur->name[i] = '\0';
			cur->lenName = i;
			i++;
			bufIndex = i;
			break;
		}
		else {
			cur->name[i] = buf[i + bufIndex];
		}
	}
	for (int i = 0; i < MaxAuthor + 1; ++i) {
		if (buf[i+bufIndex] == ' ') {
			cur->author[i] = '\0';
			cur->lenAuthor = i;

			i++;
			bufIndex += i;
			break;
		}
		else {
			cur->author[i] = buf[i + bufIndex];
		}
	}
	for (int i = 0; i < MaxPub+1; ++i) {
		if (buf[i+bufIndex] == ' ') {
			cur->publish[i] = '\0';
			cur->lenPub = i;
			i++;
			bufIndex += i;
			break;
		}
		else {
			cur->publish[i] = buf[i + bufIndex];
		}
	}
	for (int i = 0; i < MaxDate + 1; ++i) {
		if (buf[i + bufIndex] == '\n') {
			cur->date[i] = '\0';
			cur->lenDate = i;
			i++;
			bufIndex += i;
			break;
		}
		else {
			cur->date[i] = buf[i + bufIndex];
		}
	}
	//info输入完毕，根据图书
	infoNode* front = dummy;
	if (!lenList) {//只有(*dummy)此时直接加入
		dummy->next = cur;//（*cur)代表
		return;
	}
	int flag = 0;//标记是否在最后一个位置前是否插入
	while (lenList--) {//保证遍历，每次进来先后置
		dummy = dummy->next; 
		int ans = strcmp(dummy->name, cur->name);
		if (ans > 0) {//首次出现，说明前一个跟当前直接按插入
			flag = 1;
			break;
		}
		front = dummy;
	}
	if (!flag) {
		dummy->next = cur;
	}
	else {
		cur->next = dummy;
		front->next = cur;
	}
}
