#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
#define MAX 200
/*

*/
typedef struct tnode {
	int num;
	struct tnode* l;
	struct tnode* r;
}tnode;
//看错题了，实际上不是层序遍历，只输出叶结点
//typedef struct queue {
//	int head, tail;
//	tnode* que[MAX];
//	int len;
//}MyQue;
//
//MyQue qbfs = {
//	.head = 1,
//	.tail = 0,
//	.len=0
//};
//void enqueue(tnode* cur){
//	qbfs.tail = (qbfs.tail + 1) >= MAX ? (qbfs.tail + 1) % MAX
//		:qbfs.tail + 1;
//	qbfs.len++;
//	if (qbfs.len >= MAX) {
//		printf("ERROR_OVERFLOW");
//		return;
//	}
//	qbfs.que[qbfs.tail] = cur;
//}
//void dequeue() {
//	if (!qbfs.len) {
//		printf("ERROR_DEQUEUE");
//		return;
//	}
//	qbfs.que[qbfs.head] = NULL;
//	qbfs.head = (qbfs.head + 1) >= MAX ? (qbfs.head + 1) % MAX :
//		qbfs.head + 1;
//}
//tnode* getHead() {
//	if (!qbfs.len) {
//		printf("ERROR_GET\n");
//		return;
//	}
//	return qbfs.que[qbfs.head];
//}


void fTra(tnode* head,int num) {
	//num记录层数
	if (head->l == NULL && head->r == NULL) {
		printf("%d %d\n", head->num, num);
		return;
	}
	if(head->l){
		fTra(head->l, num + 1);
	}
	if (head->r) {
		fTra(head->r, num + 1);
	}
}

tnode* treeInit() {
	tnode* dummy = (tnode*)malloc(sizeof(tnode));
	dummy->l=dummy->r = NULL;
	dummy->num = -1;
	return dummy;
}

tnode* buildTnode(int num) {
	tnode* cur = (tnode*)malloc(sizeof(tnode));
	cur->l = cur->r = NULL;
	cur->num = num;
	return cur;
}

void insert(tnode* dummy, tnode* cur) {
	tnode* head = dummy->r;
	if (head == NULL) {
		dummy->r = cur;
		return;
	}
	while (head) {
		if (cur->num < head->num) {
			if (head->l == NULL) {
				head->l = cur;
				return;
			}
			head = head->l;
		}
		else {
			if (head->r == NULL) {
				head->r = cur;
				return;
			}
			head = head->r;
		}
	}
}
int main()
{
	int n; scanf("%d", &n);
	tnode* dummy = treeInit();
	while (n--) {
		int num;
		scanf("%d", &num);
		insert(dummy, buildTnode(num));
	}
	fTra(dummy->r,1);
	return 0;
}