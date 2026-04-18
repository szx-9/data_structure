#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
void releaseAll();
void Print_list();
void renewWin_minus();
void renewWaitTime();
void enterWindow();
void renewWin_add();
void enqueue();
void dequeue();
/*
progress：
输入timeSpan后：输入customNum数组->进入循环周期

每个周期：
1检查是否有新客户（当前周期数跟给定原始周期数）：
	有：入队&&根据等待人数判断加开窗口（多少个）
	无：跳过


（窗口数更新后：）
2队列用户出队进入窗口（wNum cNum双重判断 ）
	如果可以：遍历所有无人窗口（id0），同时更新所有窗口的服务时长
		入窗口（只用id和levelTime）&& 出队

（进入窗口完毕后）
3对等待队列实现自增

4当前周期结束：
	缩减窗口：满足人数条件 && 有空窗口 && 原窗口保留（实际上只需要进行一次判断：因为减少的人数最大值也不可能使得直接关掉两个窗口）
*/

/*
主要维护等待队列和窗口
*/

typedef struct custom {
	int id, levelTime, waitTime;
}cs;

typedef struct Node {
	cs* csInfo;//队列用户信息
	struct Node* next;
}Node;
void releaseNode(Node*);

cs window[6];
int wCsNum = 0;
int wOpNum = 3;

Node *qHead=NULL,*qRear;//注意野指针问题
int qNum = 0;

int curspan = 1, timespan;

int csNum[1005];//每个周期新用户数
int curTotal = 0;//当前进入过队列的人总数
int ans[10005];

int main()
{
	window[4].id = window[5].id = -1;
	qRear = (Node*)malloc(sizeof(Node));
	qRear->next = qHead;//空队列：next存储qHead指向地址NULL

	scanf("%d", &timespan);
	for (int i = 1; i < timespan; ++i) {
		scanf("%d", &csNum[i]);
	}
	do {
		renewWin_add();
		enterWindow();
		renewWaitTime();
		renewWin_minus();
		curspan++;
	} while (!(qNum<=0&&curspan>timespan));

	Print_list();
	releaseAll();

	return 0;
}
void releaseAll() {
	if (!qHead)return;
	Node* cur = qHead;
	while (cur) {
		Node* res = cur;
		cur = cur->next;
		releaseNode(res);
	}
}
void Print_list() {
	for (int i = 1; i <= curTotal; ++i) {
		printf("%d : %d\n", i, ans[i]);
	}
}
void renewWin_minus() {
	if (qNum < wOpNum * 7 && wOpNum>3 && wOpNum>3 ) {
		wOpNum--;
		for (int i = 1; i < 6; ++i) {
			if (!window[i].id) {
				window[i].id = -1;
				break;
			}
		}
	}
}
void renewWaitTime() {
	if (!qHead) return;
	Node* cur = qHead;
	while (cur) {
		(cur->csInfo->levelTime)++;
		cur = cur->next;
	}

}
void enterWindow() {
	for (int i = 1; i < 6; ++i) {
		if (!window[i].id && qHead!=NULL) {//空窗口且队列非空
			window[i].id = qHead->csInfo->id;
			window[i].levelTime = qHead->csInfo->levelTime;
			ans[qHead->csInfo->id] = qHead->csInfo->waitTime;
			
			Node* res = qHead;
			qHead = qHead->next;
			releaseNode(res);
		}
		window[i].levelTime--;
		if(!(window[i].levelTime) ){
			window[i].id = -1;
			wOpNum--;
			wCsNum--;
		}
	}
}
void releaseNode(Node* a) {
	free(a->csInfo);
	free(a);
}
void renewWin_add() {
	if (curspan > timespan)return;
	int newCsNum = csNum[curspan];
	for (int i = 0; i < newCsNum; ++i) {
		enqueue();
	}

	while (qNum >= 7 * wOpNum && wOpNum<5 ) {
		wOpNum++;
		for (int i = 1; i < 6; ++i) {
			if (window[i].id == -1) {
				window[i].id = 0;
			}
		}
	}
}
void enqueue() {
	Node* newCs = (Node*)malloc(sizeof(Node));
	newCs->csInfo = (cs*)malloc(sizeof(cs));
	newCs->csInfo->id = (curTotal++) + 1;
	scanf("%d", &newCs->csInfo->levelTime);
	newCs->csInfo->waitTime = 0;
	newCs->next = NULL;

	qRear->next = newCs;
	qRear = newCs;
	qNum++;
}
void dequeue() {
	if (qRear->next == qHead)return;
	Node* res = qHead;
	qHead = qHead->next;
	
	free(res->csInfo);
	free(res);
}




