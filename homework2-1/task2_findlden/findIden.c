#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*



*/
typedef struct charNode {
	char iden[10];
	int is;//判断是否有出现过
	struct charNode* next;
}charNode;

int isChar(char);

char buf[50];
char vari[10];

int isChar(char);
void isExist(charNode*, char*);
int main()
{
	FILE* fin = fopen("int.c", "r");
	fscanf(fin, "%[^;]", buf);//将这个；之前内容读入buf中

	charNode* dummy = (charNode*)malloc(sizeof(charNode));
	dummy->next = NULL;
	charNode* copy = dummy;

	int bufIndex = 0;
	int lenBuf = strlen(buf);
	while (1) {
		if (buf[bufIndex++] == '{') {
			break;
		}
	}
	//此时bufindex指向 { 之后的字符：换行符？

	//此时指向int后的空格
	charNode* curs = dummy;
	while (bufIndex < lenBuf) {
		if (isChar(buf[bufIndex])) {
			//如果是字母，说明应该对当前变量进行遍历，会自动跳过空格
			charNode* cur = (charNode*)malloc(sizeof(charNode));
			cur->is = 0;
			cur->next = NULL;
			int firstPos = bufIndex;
			while ( bufIndex<lenBuf &&isChar(buf[bufIndex])) {
				cur->iden[bufIndex - firstPos] = buf[bufIndex];
				bufIndex++;
			}
			cur->iden[bufIndex - firstPos] = '\0';//此时bufindex指向空格或者分隔符
			curs->next = cur;
			curs = cur;
		}

		bufIndex++;
	}
	charNode* int_vari = dummy->next;
	dummy -> next = dummy->next->next;
	free(int_vari);
	//此时所有变量类型已经存入dummy的链表，
	//接下来进行逐行读入，此时变量名一定不会进行跨行存在
	//此时fin指向下一个应该读取的字符

	/*
	首先输入到buf中，利用index进行遍历，遍历到字母时，进入内层循环，检查得到一个字符串，并输入到vari中进入函数去判断，是否重合
	*/
	while (fgets(buf, sizeof(buf), fin)) {
		//逐行去判断
		int index = 0;
		int len = strlen(buf);
		while (index < len) {
			if (isChar(buf[index])) {
				//index读到了第一个字符
				int firstPos = index;
				vari[index - firstPos] = buf[index];
				while (index <len && isChar(buf[index])) {
					vari[index - firstPos] = buf[index];
					index++;
				}
				vari[index - firstPos] = '\0';
				//此时对于当前变量名，输入到vari完毕，
				isExist(dummy, vari);
				
			}
			index++;
		}
	}
	dummy = dummy->next;
	while (dummy) {
		if (!dummy->is) {
			printf("%s", dummy->iden);
			printf("\n");
		}
		dummy = dummy->next;

	}
	copy = copy -> next;
	while (copy) {
		charNode* res = copy;
		copy = copy->next;
		free(res);
	}
	free(copy);


	fclose(fin);
	return 0;
}
void isExist(charNode* dummy, char* vari) {
//dummy给出整个链表，vari是用来比对的字符
	dummy = dummy->next;
	while (dummy) {
		if (!strcmp(dummy->iden, vari)) {
			//两者相等
			dummy->is = 1;
			return;
		}
		dummy = dummy->next;
	}

}

int isChar(char c) {
	if (c >= 'a' && c <= 'z')return 1;;
	if (c >= 'A' && c <= 'Z')return 1;
	return 0;
}