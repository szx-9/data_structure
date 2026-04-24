#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

*/
enum {
	hori = 1,//规定向上和向左为正
	vert = 3
};
typedef struct path {
	int dir;
	int step;
}My_path;

typedef struct stk {
	int top;
	My_path* path[1000];
}Stk;

Stk pathStk;

void Print();
int pushPathNode();
int isSameDir(My_path* pathNode);
My_path* buildPathNode(int dir, int step);
void push(My_path* pathNode);
My_path* getTop();
void pop();
int main()
{
	int isEND = 1;
	while (isEND = pushPathNode());
	Print();

	return 0;
}
void Print() {
	My_path* curr;
	while ((curr = getTop()) != NULL) {
		pop();
		int dir = curr->step > 0 ? curr->dir + 1 : curr->dir;//注意最后是要反方向的路线，所以是>0
		int step = abs(curr->step);
		printf("%d-%d ", dir, step);
	}
}
int pushPathNode() {
	//读取插入一个path节点信息,返回是否读取成功
	int dir= -1,step=-1;
	scanf(" %d-%d", &dir, &step);
	if (!(dir || step)) {
		return 0;
	}
	My_path* curr = buildPathNode(dir, step);
	if (!pathStk.top||!isSameDir(curr)) {
		push(curr);
		return 1;
	}
	//存在回溯关系
	getTop()->step += curr->step;
	if (!getTop()->step) {
		//步长为零，
		pop();
	}
	free(curr);
	return 1;
}
int isSameDir(My_path* pathNode) {
	//判断当前节点是否与栈顶存在回溯关系
	My_path* stkTop = getTop();
	if (stkTop->dir == pathNode->dir)return 1;
	return 0;
}
My_path* buildPathNode(int dir,int step) {
	My_path* newPathNode = (My_path*)malloc(sizeof(My_path));
	if (newPathNode == NULL) {
		printf("ERROR_MALLOC\n");
		return NULL;
	}
	newPathNode->step = (dir&1) ? step : -step;
	newPathNode->dir =dir<3? hori:vert  ;//dir是奇数时step为正
	return newPathNode;
}
void pop() {
	if (!pathStk.top) {
		printf("ERROR_POP\n");
	}
	pathStk.top--;
}
void push(My_path* pathNode) {
	pathStk.path[pathStk.top++] = pathNode;
}
My_path* getTop() {
	if (!pathStk.top) {
		return NULL;
	}
	return pathStk.path[pathStk.top - 1];
}