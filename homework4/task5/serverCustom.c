#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
#define MAX 10005
void dequeue();
void enqueue();
void renewWindow_add();
void enterWindow();
void renewWaitTime();
void renewWindow_minus();
void Print();
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
typedef struct cusInfo {
	int id;
	int levelTime;
	int waitTime;
}cusInfo;

int customNum[1005];//存储每个周期新客户数
cusInfo waitQueue[10005];//等待队列
cusInfo window[6];//模拟窗口
int ans[10005];

int timespan;
int curTotal=0;//当前已经有多少人进队（到访）
int curspan = 1;

int wNum=0;//维护窗口人数
int wOpNum = 3;//维护窗口数量

int qFront = 0, qRear = -1,qNum=0;//两者均为实际队首队尾,因为循环队列，所以最好额外维护一下总数

int main()
{
	//win预处理
	window[4].id = -1;
	window[5].id = -1;

	scanf("%d", &timespan);
	for (int i = 1; i <= timespan; ++i) {//!!!!注意索引模式
		scanf("%d", &customNum[i]);
	}


	do {
		renewWindow_add();
		enterWindow();
		renewWaitTime();
		renewWindow_minus();

		curspan++;
	} while (qNum>0||curspan<=timespan);//!!!!判定结束时，需要满足curspan>timespan&&cNum==0 

	Print();

	return 0;
}
void Print() {
	for (int i = 1; i <= curTotal; ++i) {
		printf("%d : %d\n", i, ans[i]);
	}
}

void renewWindow_minus() {
	if( wOpNum*7>qNum && wOpNum>3 && wOpNum > wNum){
		//！！！！：满足人数，原窗口不关，有空窗
		wOpNum--;
		wNum--;
		
		for (int i = 1; i <6; ++i) {
			if (!window[i].id) {
				window[i].id = -1;
				break;
			}
		}
	}
}


void renewWaitTime() {
	//对等待队列中的等待时间实现自增，
	//！！！！循环队列需要额外维护总数（用于判空）
	if (qNum <= 0){
		return ;
	}
	int index = qFront;
	int end = qRear;
	while (index != end) {
		(waitQueue[index].waitTime)++;
		index++;
		index %= MAX;
	}
	//!!!!end位置也需要更新等待时间
	(waitQueue[end].waitTime)++;
}

void enterWindow() {
	for (int i = 1; i < 6 && qNum>0; ++i) {
		if (!window[i].id && qNum) {
			//当前位置空，且无人
			window[i].id = waitQueue[qFront].id;
			window[i].levelTime = waitQueue[qFront].levelTime;
			//进入窗口后waitTime无用，可以不用传入
			
			ans[waitQueue[qFront].id] = waitQueue[qFront].waitTime;//在入窗口时就更新ans数组
			
			dequeue();
			wNum++;
		}
		(window[i].levelTime)--;
		if (!window[i].levelTime) {
			//清出窗口
			wNum--;
			window[i].id = 0;
		}

	}
}

void renewWindow_add() {
	if (curspan <= timespan) {
		int newCusNum = customNum[curspan];
		//执行相应次数的入队操作
		for (int i = 0; i < newCusNum; ++i) {
			enqueue();
		}

		//通过等待人数判断是否需要加开窗口
		//！！！！不假设入队，静态过程 
		while(qNum >= 7 * wOpNum && wOpNum<5) {
			//人数满足；有空窗口
			for (int i = 1; i < 6; ++i) {
				if (window[i].id == -1) {
					wOpNum++;
					window[i].id = 0;
					break;
				}
			}
		}
	}
}

void enqueue() {
	qRear++;
	if (qRear >= MAX)qRear %= MAX;
	
	scanf("%d", &waitQueue[qRear].levelTime);
	waitQueue[qRear].id = curTotal + 1;
	waitQueue[qRear].waitTime = 0;

	qNum++;//维护队长度

	curTotal ++;
}

void dequeue() {
	if (qNum<=0)return;//循环队列额外维护
	qNum--;
	qFront++;
	if (qFront >= MAX)qFront %= MAX;
}