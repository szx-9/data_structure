#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*


*/
typedef struct location {
	int dis;
	char name[25];
}locInfo;

typedef struct carriage {
	int id;//车厢编号
	locInfo dest;
}car;

typedef struct stack {
	int top;
	car track[55];//track数组用于模拟轨道
}stk;

stk A, B, C;
int aPushNum = 0;

locInfo recLocInfo[55];//存储地名信息的数组
int recIndex = 0;//地名数组的索引
char buf[25];

int destNum;
int carNum;

void printId(int id);
void Print();
int returnNumOfPop();
void I_LocationInfo();
void I_CarriageInfo();
int returnDisOfDest(char* dest);
void push(stk* a, car* cur);
void pop(stk* a);
car* getTop(stk* a);

int main()
{
	I_LocationInfo();
	I_CarriageInfo();
	int cnt = 0;
	
	while(A.top) {
		push(&B, getTop(&A));
		pop(&A);
	}
	
	for (; cnt < carNum; ++cnt) {	
		int numPop = returnNumOfPop();
		for (int i = 0; i < numPop; ++i) {
			push(&A, getTop(&B));
			aPushNum++;
			pop(&B);
		}

		if (numPop == 1)continue;
		push(&C, getTop(&A));
		pop(&A);
		for (int i = 0; i < numPop - 1; ++i) {
			push(&B, getTop(&A));
			pop(&A);
		}
		push(&A, getTop(&C));
		pop(&C);
		aPushNum++;
	}

	Print();
	

	return 0;
}
void printId(int id) {
	int copy = id;
	int i=0;
	while (id) {
		i++;
		id /= 10;
	}
	for (int j = 0; j <4-i; ++j) {
		printf("0");
	}
	printf("%d ", copy);
}
void Print() {
	int len = A.top;
	for (int i = 0; i < len; ++i) {
		printId(A.track[i].id);
	}
	printf("\n%d", aPushNum);

}
int returnNumOfPop() {
	int cnt = 1;
	int maxDis = 0;
	int maxIndex = -1;
	for (int i = B.top-1; i >=0; --i) {
		if (B.track[i].dest.dis > maxDis) {
			maxDis = B.track[i].dest.dis;
			maxIndex = i;
		}
	}
	return B.top-maxIndex;//pop的次数
}

void I_LocationInfo() {
	scanf("%d", &destNum);
	for (int i = 0; i < destNum; ++i) {
		scanf(" %s %d", recLocInfo[i].name,&recLocInfo[i].dis);
	}
}
void I_CarriageInfo() {
	scanf("%d", &carNum);
	for (int i = 0; i < carNum; ++i) {
		car* newCarriage = (car*)malloc(sizeof(car));
		if (newCarriage == NULL) {
			printf("ERROR_CARPOINTER\n");
		}
		scanf(" %s", buf);
		int id = atoi(buf);
		newCarriage->id = id;
		
		scanf(" %s", buf);
		strcpy(newCarriage->dest.name, buf);

		int dis = returnDisOfDest(buf);
		if (!dis) {
			printf("ERROR_DIS\n");
			return;
		}
		newCarriage->dest.dis = dis;
		push(&A, newCarriage);
		
		free(newCarriage);
	}
}
int returnDisOfDest(char* dest) {
	//返回buf代表的地名的里程
	for (int i = 0; i < destNum; ++i) {
		if (!strcmp(buf, recLocInfo[i].name)) {
			return recLocInfo[i].dis;
		}
	}
	return 0;
}
void push(stk* a, car* cur) {
	if (cur == NULL)return;
	a->track[a->top].dest = cur->dest;
	a->track[a->top].id = cur->id;
	a->top++;
}
void pop(stk* a) {
	if (!a->top) {
		printf("ERROR_POP\n");
		return;
	}
	a->top--;
}
car* getTop(stk *a) {
	if (!a->top) {
		printf("ERROR_GETTOP\n");
		return;
	}
	return &(a->track[a->top - 1]);
}
