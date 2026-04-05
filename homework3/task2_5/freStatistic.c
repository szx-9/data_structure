#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

*/
typedef struct node {
	char word[20];
	int cnt;
	struct node* next;
}node;

node* CreateList(FILE* fin);
int main()
{
	FILE* fin = fopen("article.txt", "r");
	/*if (fin == NULL) {
		perror("Error opening file");
		return 1;
	}
	
	char ch;
	while ((ch = fgetc(fin))) {
		printf("%c", ch);
	}*/


	node* head = CreateList(fin);
	//head已经创建完毕，只需要一遍输出，一边释放即可
	node* res = NULL;
	while (head) {
		res = head;
		head = head->next;
		printf("%s %d\n", res->word, res->cnt);
		free(res);
	}

	fclose(fin);
	return 0;
}
node* CreateList(FILE* fin) {
	node* head=NULL, *front=NULL;//只定义head作为头节点
	int ch;

	node* cur = NULL;
	while ( (ch = fgetc(fin) ) !=EOF  ) {
		if (isalpha(ch)) {
			//如果是字符
			node* move = head;
			front = NULL;//每次循环都放空

			cur = (node*)malloc(sizeof(node));
			cur->next = NULL;
			cur->cnt = 1;
			int wordIndex = 0;
			cur->word[wordIndex++] = tolower(ch);
			while ( (ch = fgetc(fin)) != EOF && isalpha(ch)) {
				cur->word[wordIndex++] = tolower(ch);
			}
			cur->word[wordIndex] = '\0';
			//此时fin指向空白符
			//
			// 验证节点是否已经存在，可以在插入排序中检测，
			// 原则上遇到第一个比节点字典序大的插入，
			// 但是如果遇到相等字典序，则执行增加，释放操作
			// 
			//创建节点后按字典序插入
			if (head==NULL) {
				head= cur;
			}
			else {
				//从head开始进行插入排序
				while(move) {
					int type = strcmp(cur->word, move->word);
					if (!type) {
						(move->cnt)++;
						free(cur);
						goto a;
					}
					else if (type > 0) {
						front = move;
						move = move->next;
					}
					else {
						break;
					}
				}//move如果遍历到了末尾
				//此时move位置应该是cur插入位置
				cur->next = move;
				if (front) {
					//front非空，说明现在是第一个
					front->next = cur;
				}
				else {
					//front空
					head = cur;
				}
			}

		}
		//不是字符不做处理
	a:continue;
	}
	return head;
}