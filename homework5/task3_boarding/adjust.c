#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
#define dataType info
#define MAX 200
/*
*/
enum {
	LEF = 1,
	MID = 2,
	RIG=3,
	NO_MATCH=0,
	INSERT_NO=0,
	INSERT_YES=1
};
typedef struct info {
	int id,lid,mid,rid,tra;
}info;//结点的信息:对于分叉节点需要额外记录左中右的id让后续遍历插入时有比对

typedef struct tnode {
	dataType* data;
	struct tnode* l, * r, * m;
}tnode;

typedef struct boardInfo {
	ll tra;
	int id;
}boIn;


typedef struct queue {
	int head, tail;
	tnode* que[MAX];
	int len;
}MyQue;

MyQue qbfs = {
	.head = 1,
	.tail = 0,
	.len=0
};
void enqueue(tnode* cur){
	if (cur == NULL)return;//bfs遍历时有用
	qbfs.tail = (qbfs.tail + 1) >= MAX ? (qbfs.tail + 1) % MAX
		:qbfs.tail + 1;
	qbfs.len++;
	if (qbfs.len >= MAX) {
		printf("ERROR_OVERFLOW");
		return;
	}
	qbfs.que[qbfs.tail] = cur;
}
void dequeue() {
	if (!qbfs.len) {
		printf("ERROR_DEQUEUE");
		return;
	}
	qbfs.que[qbfs.head] = NULL;
	qbfs.len--;
	qbfs.head = (qbfs.head + 1) >= MAX ? (qbfs.head + 1) % MAX :
		qbfs.head + 1;
}
tnode* getHead() {
	if (!qbfs.len) {
		printf("ERROR_GET\n");
		return NULL;
	}
	return qbfs.que[qbfs.head];
}
int isEmpty() {
	if (qbfs.len)return 0;
	return 1;
}

int sbNum = 0;//stand by num

tnode* treeInit() {
	tnode* dummy = (tnode*)malloc(sizeof(tnode));
	dummy->l=dummy->r=dummy->m=NULL;//注意初始化指针
	dummy->data = (dataType*)malloc(sizeof(dataType));
	//TODO:标记dummy结点
	dummy->data->id = -1;
	return dummy;
}
//TODO:根据data决定传参
dataType* buildDnode(int id,int lid,int mid,int rid) {
	dataType* cur = (dataType*)malloc(sizeof(dataType));
	cur->id = id;
	cur->lid = lid;
	cur->mid = mid;
	cur->rid = rid;
	if (cur->id < 100)sbNum++;
	return cur;
}

tnode* buildTnode(dataType* dnode) {
	tnode* cur = (tnode*)malloc(sizeof(tnode));
	cur->l = cur->r = cur->m = NULL;
	cur->data = dnode;
	return cur;
}

int isMatch(tnode* dest, tnode* src) {
	//TODO:判断存在结点dest和src的关系
	if (dest == NULL) {
		printf("ERROR_MATCH\n");
		return NO_MATCH;
	}
	if (dest->data->lid == src->data->id
		&& dest->l == NULL) {
		dest->l = src;
		return LEF;
	}
	if (dest->data->mid == src->data->id
		&& dest->m == NULL) {
		dest->m = src;
		return MID;
	}if (dest->data->rid == src->data->id
		&& dest->r == NULL) {
		dest->r = src;
		return RIG;
	}
	return NO_MATCH;
}

int insert(tnode* dummy, tnode* cur) {
	//将cur根据某种规则中插入到dummy
	if (dummy == NULL) {
		printf("ERROR_INSERT_DUMMY_NULL\n");
		return INSERT_NO;
	}
	tnode* head = dummy->data->id==-1?  dummy->m:dummy;
	if (head== NULL) {
		dummy->m = cur;
		return INSERT_NO;
	}
	int rel = 0;
	if ((rel = isMatch(head, cur))) {
		switch (rel) {
		case LEF:
			head->l = cur;
			break;
		case MID:
			head->m = cur;
			break;
		case RIG:
			head->r = cur;
			break;
		default:
			printf("ERROR_INSERT\n");
			return INSERT_NO;
		}
		return INSERT_YES;
	}
	//如果当前结点的对应分支非空，则直接进入搜索
	int state = 0;
	if (head->l) {
		state = insert(head->l, cur);
		if (state == 1)return INSERT_YES;//判断条件
	}
	if (head->m) {
		state = insert(head->m, cur);
		if (state == 1)return INSERT_YES;
	}
	if (head->r) {
		state = insert(head->r, cur);
		if (state == 1)return INSERT_YES;
	}
	return INSERT_NO;
}

int isLeaf(tnode* cur) {
	if (cur->l == NULL && cur->m == NULL && cur->r == NULL)return 1;
	return 0;
}

int res[5];//缓存一行数据
int resIndex = 0;
void buildTree(tnode* dummy) {
	//input：根据题目具体
	tnode* head = dummy;

	int input=0;
	do {
		scanf("%d", &input);
		if (input == -1) {
			if (resIndex==0) {
				break;
			}
			if (resIndex == 4) {
				int id = res[0], lid = res[1], mid = res[2], rid = res[3];
				insert(head, buildTnode(buildDnode(id, lid, mid, rid)));
				if (lid < 100) 
					insert(head, buildTnode(buildDnode(lid, 0, 0, 0)));
				if (mid < 100)
					insert(head, buildTnode(buildDnode(mid, 0, 0, 0)));
				if (rid < 100)
					insert(head, buildTnode(buildDnode(rid, 0, 0, 0)));
			}
			else if (resIndex == 3) {
				int id = res[0], lid = res[1], rid = res[2];
				insert(head, buildTnode(buildDnode(id, lid, 0, rid)));
				if (lid < 100)
					insert(head, buildTnode(buildDnode(lid, 0, 0, 0)));
				if (rid < 100)
					insert(head, buildTnode(buildDnode(rid, 0, 0, 0)));
			}
			else if(resIndex==2){
				int id = res[0], mid = res[1];
				insert(head, buildTnode(buildDnode(id, 0, mid, 0)));
				if (mid < 100)
					insert(head, buildTnode(buildDnode(mid, 0, 0, 0)));
			}
			else if(resIndex==1){
				printf("ERROR_INSERT_RES\n");
				return;
			}

			resIndex = 0;
			continue;
		}
		res[resIndex++] = input;

	} while (!(resIndex==1&&input==-1));
}

boIn boardId[105];//存储各个登机口流量
int cmp(const void* a, const void* b) {
	boIn* c = (boIn*)a;
	boIn* d = (boIn*)b;
	if (c->tra > d->tra)return -1;
	else if (c->tra < d->tra)return 1;
	else {
		if (c->id < d->id)return -1;
		else if (c->id > d->id)return 1;
		else {
			printf("ERROR_CMP\n");
			return 0;
		}
	}

}
void dealBoardInfo() {
	int id = 0, tra = 0;
	for (int i = 0; i < sbNum; ++i) {
		scanf(" %d %d", &id, &tra);
		boardId[i].id = id;
		boardId[i].tra = tra;
	}
	
	qsort(boardId, sbNum, sizeof(boIn), cmp);//注意这里的sizeof元素类型
}

int levelId[105];//记录层序遍历的id顺序
int lIndex = 0;
void levelOrderTra(tnode* dummy) {
	tnode* head = dummy->m;
	enqueue(head);
	do {
		tnode *curr=getHead();
		dequeue();//注意顺序：均要出队
		if (isLeaf(curr)) {
			levelId[lIndex++] = curr->data->id;
			continue;
		}
		enqueue(curr->l);
		enqueue(curr->m);
		enqueue(curr->r);

	} while (!isEmpty());//逻辑是非空

}

void release(tnode* head) {
	if (head == NULL)return;
	release(head->l);
	release(head->m);
	release(head->r);
	free(head->data);
	free(head);
}
int main()
{
	tnode* dummy = treeInit();
	buildTree(dummy);
	
	dealBoardInfo();
	

	levelOrderTra(dummy);


	for (int i = 0; i < sbNum; ++i) {
		printf("%d->%d\n", boardId[i].id, levelId[i]);
	}
	release(dummy->m);
	free(dummy);
	return 0;
}