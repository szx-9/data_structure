#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

易错点是定义各种key hash时，容易下标错误
还有内存释放的时机：题目交代了，有时候会忘掉


*/


typedef struct node {
	char item;
	struct node* next;
}node;

char key[200];//密钥,存放最后的链表值
int hash[200];//记录可见字符是否已经出现过（key中或者在遍历字母时）
char buf[37];//读入key

char code[200];//下标是元字符，值是对应密码

node* CreateList(char key[], int keyLen, node** tail1);
void encode(char code[], node* head, node* tail);
int isVisible(char);

int main()
{
	fgets(buf, sizeof(buf), stdin);
	int bufLen = strlen(buf);
	int keyIndex = 32;
	for (int i = 0; i < bufLen; ++i) {
		if (!isVisible(buf[i]))continue;
		if (!hash[buf[i]]) {
			//没出现过
			hash[buf[i]] = 1;
			key[keyIndex++] = buf[i];
		}
	}
	for (int j = 32; j <= 126; ++j) {
		//检查所有可见字符
		if (!hash[j]) {
			hash[j] = 1;
			key[keyIndex++] = j;
		}

	}
	/*for (int i = 32; i <=126; ++i) {
		printf("%c", key[i]);
	}*/

	node* tail = NULL;
	node* head = CreateList(key, keyIndex,&tail);
	//此时链表组装完成

	encode(code, head, tail);
	//此时已经处理完毕code密码集

	/*for (int i = 32; i <= 126; ++i) {
		printf("%c", i);
	}
	printf("\n");
	for (int i = 32; i <= 126; ++i) {
		printf("%c", code[i]);
	}*/


	FILE* fin = fopen("in.txt", "r");
	FILE* fout = fopen("in_crpyt.txt", "w");
	int ch;
	while (  ( ch=fgetc(fin) ) !=-1) {
		//如果等于-1说明文件末尾:赋值语句的值是ch
		if (isVisible(ch)) {
			//需要翻译
			fputc(code[ch], fout);
		}
		else {
			fputc(ch, fout);
		}
	}
	fclose(fin);
	fclose(fout);

	return 0;
}
node* CreateList(char key[],int keyLen,node** tail1) {
	node* dummy = (node*)malloc(sizeof(node));//虚拟头节点
	node* tail = dummy, *cur=NULL;
	//!!!!注意key是有范围的，即32-126为有效范围,即key总是有固定多的元素数量
	
	
	for (int i = 32; i <= 126; ++i) {
		cur = (node*)malloc(sizeof(node));
		cur->next = NULL;
		cur->item = key[i];
		tail->next = cur;
		tail = cur;
	}
	//此时cur指向最后一个
	*tail1 = cur;
	cur->next = dummy->next;
	node* head = dummy->next;

	free(dummy);
	return head;
}

void encode(char code[], node* head,node* tail) {
	//显然key是所有链表元素，所以链表长度就keyLen
	int copy = 126 - 32 + 1;
	int keyLen = copy;
	node* cur = head;
	node* front = tail;
	node* res = NULL;//用于暂存节点释放
	int first = (int)head->item;

	while (copy--) {
		//对于每一个字符都要找到自己的密码，共keyLen个字符
		if (!copy) {
			//此时是最后一个字符
			code[cur->item] = first;
			free(cur);
			break;
		}
		keyLen--;
		int resVal = (int)cur->item;
		int moveNum = (resVal-1) %keyLen ;//需要移动的步数
		front->next = cur->next;
		
		res = cur;
		cur = cur->next;
		free(res);

		for (int i = 0; i < moveNum; ++i) {
			front = cur;
			cur = cur->next;
		}
		//此时cur指向resVal的值
		code[resVal] = cur->item;
	}
	//此时code密码集处理完毕
	return;
}

int isVisible(char ch) {
	if (ch >= 32 && ch <= 126)return 1;
	return 0;
}