#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
#define MAX 50
/*
有效字母：大小写字母，下划线，数字，
*/

typedef struct Node {
	char* name;
	struct Node* next;
}Node;

char buf[40];

int isValid(int c);
void inputNode(Node* dummy);

int main()
{
	Node dummy;
	dummy.next = NULL;
	char input;
	int bufIndex = 0;
	int flagChar = 0;
	while (scanf("%c", &input)!=EOF) {
		if (input == ';') {
			//可能有未处理的字符
			if (bufIndex) {
				buf[bufIndex] = '\0';
				if (bufIndex && flagChar) {
					inputNode(&dummy);//将缓存的字符存入
					//buf不为零时进入
				}
			}

			break;
		}
		if (isValid(input)) {
			buf[bufIndex++] = input;
			if (isalpha(input)) {
				flagChar = 1;
			}
		}
		else {
			//当前字符不是有效字符
			buf[bufIndex] = '\0';
			if (bufIndex&&flagChar) {
			inputNode(&dummy);//将缓存的字符存入
			//buf不为零时进入
			}
			bufIndex = 0;
			flagChar = 0;
		}
	}
	Node* cur = &dummy;
	cur = cur->next;
	while (cur != NULL) {
		printf("%s ", cur->name);
		cur = cur->next;
	}

	cur = &dummy;
	cur = cur->next;
	Node* res;
	while (cur != NULL) {
		res = cur;
		cur = cur->next;
		free(res->name);
		free(res);
	}
	return 0;
}
void inputNode(Node* dummy) {
	//将当前提取的buf字符放到数组里
	Node* cur = (Node*)malloc(sizeof(Node));
	cur->name = (char*)malloc(sizeof(char) * MAX);
	cur->next = NULL;
	strcpy(cur->name, buf);
	
	Node* front = dummy;//前驱是dummy
	Node* move = dummy->next;//后继是move

	while (move != NULL) {
		int order = strcmp(cur->name, move->name);
		if (order < 0) {
			//找到第一个小于Node的
			cur->next = move;
			front->next = cur;
			return;
		}
		else if(order == 0){
			free(cur);
			return;
		}
		front = move;
		move = move->next;
	}
	front->next = cur;
}

int isValid(int c) {
	if (isdigit(c) || c == '_' || isalpha(c)) {
		return 1;
	}
	return 0;
}