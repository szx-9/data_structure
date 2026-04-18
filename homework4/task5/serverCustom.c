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
全局变量和结构体
结构体：
	用户编号，服务时间，等待时间

ans数组：存储最后所有用户的waittime

timespan：给定的时间周期
customNum数组：存储各个时间周期内到达用户数
curSpanCus数组：存储当前周期内所有等待用户的所有信息（front rear维护队列，循环队列）
维护变量curTotal:截止目前有多少人已经进入，用来算每一轮开始curSpan中第一个人的编号

windows数组：模拟窗口
	wNum:维护窗口人数，
	wrear:维护窗口数量（因为窗口位置可能不连续）
	标记：id
	无人可用窗口0：
	废弃窗口-1：
最多5个窗口：所以win只用5个位置
0(这一位不用) 0 0 0 -1 -1


progress：
输入timeSpan后：输入customNum数组->进入循环周期；for循环条件需要指定（不是有限的，需要特定条件）

每个周期：
1检查是否有新客户（当前周期数跟给定原始周期数）：
	有：从curSpanCus队尾开始循环相应次：一个一个创建用户（详见下），更新队尾（取模）
		利用curTotal更新头索引->用于创建用户对象（在数组空间中）
		利用存储的人数，将所有用户的编号，初始等待时间0，服务等级时间，输入curSpan数组

	有：（0人也算有新客户）判断是否加开窗口（以及增加几个，最多两个）
		加开窗口：首先统计
	否：（不再进行窗口增加）

（窗口数更新后：）
2队列用户出队进入窗口（先判断是否可以：wNum和wrear关系&&等待队列是否为空 ）
	如果可以：遍历中判断无人窗口（0），
		先将等待队列中的队首信息复制到窗口中
		进行出队操作（！！！！任何出队操作都要验证队是否为空
	进入窗口后直接更新服务时间

（进入窗口完毕后）
3对等待队列中的人进行等待时间更新
	遍历队列：进行时间自增

4当前周期结束：
	对窗口状态更新
	判断当前是否需要缩减窗口（while：是否持续做）
		如果需要：更新窗口状态和窗口人数
输出ans
*/
typedef struct cusInfo {
	int id;
	int levelTime;
	int waitTime;
}curInfo;

int customNum[1005];
curInfo curSpanCus[10005];//维护当前周期内所有用户信息,模拟队列
curInfo window[6];//模拟窗口
int ans[10005];//存储waittime

int timespan;
int curTotal=0;
int wNum=0;//维护窗口人数
int wrear = 3;//维护窗口数量
int cfront = 0, crear = -1,cNum=0;//两者均为实际队首队尾,因为循环队列，所以最好额外维护一下总数
int curspan = 1;//当前周期

int main()
{
	//win预处理
	window[4].id = -1;
	window[5].id = -1;

	scanf("%d", &timespan);
	for (int i = 1; i <= timespan; ++i) {//!!!!注意程序中数组的下标类型
		scanf("%d", &customNum[i]);
	}


	do {
		renewWindow_add();
		enterWindow();
		renewWaitTime();
		renewWindow_minus();

		curspan++;
	} while (cNum>0||curspan<=timespan);//!!!!判定结束时，需要满足curspan>timespan&&cNum==0   任意一个不满足都不结束

	Print();

	return 0;
}
void Print() {
	for (int i = 1; i <= curTotal; ++i) {
		printf("%d : %d\n", i, ans[i]);
	}
}

void renewWindow_minus() {
	//!!!!原有窗口不能关，
	while( wrear*7>cNum && wrear>3 && wrear > wNum){
		//!!!!等于7时不能关
		//满足人数，原窗口不关，有空窗

		wrear--;//窗口数减少
		wNum--;//窗口人数显然也要减少
		
		for (int i = 1; i < 6; ++i) {
			if (!window[i].id) {
				window[i].id = -1;
				break;
			}
		}
	}
}


void renewWaitTime() {
	//对等待队列中的等待时间实现自增，
	// !!!!循环队列不能按照此逻辑访问
	//!!!!需要用cNum判断是否队列为空，这里是循环队列，索引不能信
	if (cNum <= 0){
		return ;
	}
	int index = cfront;
	int end = crear;
	while (index != end) {
		(curSpanCus[index].waitTime)++;
		index++;
		index %= MAX;
	}
	//!!!!end位置也需要更新等待时间
	(curSpanCus[end].waitTime)++;
}

void enterWindow() {
	/*窗口数更新后：
		判断是否有用户可以入队：先判断wNum
		如果有遍历win
		对于win中空缺窗口进行入队，并更新curSpan中队列（出队）
		同时更新wNum
		最后遍历等待队列，对waittime执行自增

		!!!!进入窗口时有出队操作，需要对队列判空
	*/
	
	
	for (int i = 1; i < 6 && cNum>0; ++i) {
		//遍历整个窗口，找到id0进入win队列
		if (!window[i].id && cNum) {
			//当前位置空，无人
			window[i].id = curSpanCus[cfront].id;
			window[i].levelTime = curSpanCus[cfront].levelTime;
			window[i].waitTime = curSpanCus[cfront].waitTime;
			ans[curSpanCus[cfront].id] = curSpanCus[cfront].waitTime;//在入窗口时就更新ans数组
			
			dequeue();
			wNum++;
		}
		(window[i].levelTime)--;
		if (!window[i].levelTime) {
			wNum--;
			window[i].id = 0;
		}

	}
}

void renewWindow_add() {
	/*检查是否有新客户（当前周期数跟给定原始周期数）：
		有：从curSpanCus队尾开始循环相应次：一个一个创建用户（详见下），更新队尾（取模）
		利用curTotal更新头索引->用于创建用户对象（在数组空间中）
		利用存储的人数，将所有用户的编号，初始等待时间0，服务等级时间，输入curSpan数组

		有：（0人也算有新客户）判断是否加开窗口（以及增加几个，最多两个）
		否：（不再进行窗口增加）
	*/
	
	if (curspan <= timespan) {
		//在curSpanCus中入队
		int newCusNum = customNum[curspan];//当前周期的新用户
		//执行相应次数的入队操作
		for (int i = 0; i < newCusNum; ++i) {
			enqueue();
		}
		
		//！！！！新用户到达时：不需要考虑是否缩减窗口，因为如果可以缩减窗口会在周期中部执行
		//通过实际等待人数判断是否需要加开窗口
		int realWaitNum = cNum - (wrear-wNum);//此处是假设，空出位置有人入队
		while(realWaitNum >= 7 * wrear) {
			//现在需要开窗口，没有没废弃窗口？
			//遍历所有-1废弃窗口如果有则新增，没有则不进行任何操作
			for (int i = 1; i < 6; ++i) {
				if (window[i].id == -1) {
					wrear++;
					window[i].id = 0;//更新窗口状态
					break;
				}
			}
			realWaitNum--;//加开一个窗口后需要假想人数减少
		}
		//此时窗口数更新完毕
	}
}

void enqueue() {
	crear++;
	if (crear >= MAX)crear %= MAX;
	scanf("%d", &curSpanCus[crear].levelTime);

	curSpanCus[crear].id = curTotal + 1;//！！！！这里注意尽量用最后统一给total自增的方式

	curSpanCus[crear].waitTime = 0;

	cNum++;//维护队长度

	curTotal ++;
}

void dequeue() {
	if (cNum<=0)return;//循环队列额外维护
	cNum--;
	cfront++;
	if (cfront >= MAX)cfront %= MAX;
}