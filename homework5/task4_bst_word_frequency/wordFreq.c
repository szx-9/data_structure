#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
#define dataType info
#define MAX 20
/*

*/
enum {
	INSERT_Y = 1,
	INSERT_N = 0
};
typedef struct info {
	char* word;
	int cnt;
}info;

typedef struct tnode {
	dataType* data;
	struct tnode* l, * r;
}tnode;

tnode* treeInit() {
	tnode* dummy = (tnode*)malloc(sizeof(tnode));
	dummy->l = dummy->r = NULL;//注意初始化指针
	dummy->data = (dataType*)malloc(sizeof(dataType));
	dummy->data->word = (char*)malloc(sizeof(char) * MAX);
	//TODO:标记dummy结点
	dummy->data->cnt = -1;
	
	return dummy;
}



//TODO:根据data决定传参
dataType* buildDnode(char *buf) {
	dataType* cur = (dataType*)malloc(sizeof(dataType));
	cur->word = (char*)malloc(sizeof(char) * MAX);
	cur->cnt = 1;
	strcpy(cur->word, buf);
	return cur;
}
void release(tnode* cur) {
	free(cur->data->word);
	free(cur);
}
tnode* buildTnode(dataType* dnode) {
	tnode* cur = (tnode*)malloc(sizeof(tnode));
	cur->l = cur->r = NULL;
	cur->data = dnode;
	return cur;
}

int insertBst(tnode* dummy, tnode* cur) {
	//将cur插入到bst中
	if (dummy == NULL) {
		printf("ERROR_INSERT\n");
		return INSERT_N;
	}
	tnode* head = dummy->data->cnt==-1? dummy->r:dummy;
	if (head == NULL) {
		dummy->r = cur;
		return INSERT_Y;
	}
	int state = 0;
	int order = strcmp(head->data->word, cur->data->word);
	if (order > 0 ) {
		if (head->l == NULL) {
			head->l = cur;
			return INSERT_Y;
		}
		else{
			state = insertBst(head->l, cur);
			if (state == 1)return INSERT_Y;
		}
		
	}
	if (order < 0 ) {
		if (head->r==NULL) {
			head->r = cur;
			return INSERT_Y;
		}
		else{
			state = insertBst(head->r, cur);
			if (state == 1)return INSERT_Y;
		}
	}
	if (order == 0) {
		head->data->cnt++;
		release(cur);
		return INSERT_Y;
	}
	printf("ERROR_INSERT\n");
	return INSERT_N;
}

char buf[20];
int bufIndex = 0;
tnode* buildBstTree(FILE* fin,tnode *dummy) {
	char ch;
	while ( (ch=fgetc(fin) )!=EOF ) {
		if (isalpha(ch)) {
			do {
				buf[bufIndex++] =tolower(ch);
				ch = fgetc(fin);
			} while (isalpha(ch));
			//ch现在是非字母符号
			buf[bufIndex] = '\0';
			insertBst(dummy, buildTnode(buildDnode(buf)));

			bufIndex = 0;

		}

	}

}
void mTra(tnode* head) {
	//对实际head进行中序遍历
	if (head == NULL) {
		return;
	}
	mTra(head->l);
	printf("%s %d\n", head->data->word, head->data->cnt);
	mTra(head->r);
}

void printFre(tnode* dummy) {
	tnode* head = dummy->r;
	int flag = 0;
	while (flag < 3 && head != NULL) {
		printf("%s ", head->data->word);
		head = head->r;
		flag++;
	}
	printf("\n");

	mTra(dummy->r);//注意dummy和head混用问题
}
void releaseAll(tnode* dummy) {
	if (dummy == NULL)return;
	releaseAll(dummy->l);
	releaseAll(dummy->r);//注意顺序
	release(dummy);

}

int main()
{
	FILE* fin = fopen("article.txt", "r");
	tnode* dummy = treeInit();
	buildBstTree(fin,dummy);
	printFre(dummy);
	fclose(fin);
	
	releaseAll(dummy->r);
	free(dummy);
	return 0;
}