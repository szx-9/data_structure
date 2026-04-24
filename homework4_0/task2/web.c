#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
ds：back，forward，res栈：模拟两个栈和一个存储容器
progress：
1接受输入：传入状态判断量：返回web指针（将指针放入栈中）
难点：需要对网站进行切分：反斜杠分割

每轮开始时，将cur进入res然后检索是否自增，然后计入vieworder
*/
typedef struct web {
	int viewNum;
	char name[85];//只读但可以经过初始化
}web;

typedef struct stk {
	web* Stk[105];//web指针数组,必须可修改！需要更改最值
	int top;//栈顶索引
}Stk;

enum {
	OP_VISIT = 1,
	OP_BACK = 2,
	OP_FORWARD=3,
	OP_QUIT_1=4,
	OP_QUIT_0=5
};

const web* inputDeal(int* type);
void push(Stk* stkname, const web* webPage);
void* pop(Stk* stkname);
void clearForward();
void renewRes(const web* webPage,int);
web* getTop(Stk* stkname);
int isEmpty(Stk* stkname);
int moveBack();
int moveForward();
void PrintAllWeb(int type);
void renewViewOrder(const web* webPage);
void release(Stk*);

Stk back, forward,res,viewOrder;
//res存储所有网页，按照从前到后的顺序，子网页一定出现在主网页后面，所以计算任务交给主网页，同时最后可以利用res进行释放
int maxIndexRes=-1,maxViewNum=0;//最多网站在res中的索引
char buf[100];//单行读入缓冲区


web* cur ;
web* resWeb = NULL;

int main()
{
	web* home = (web*)malloc(sizeof(web));
	strcpy(home->name, "https://www.baidu.com/");
	home->viewNum = 1;
	cur = home;
	
	int type = 0;
	int isValid = 1;//是有效的
	int isCreate = 0;//当前节点是否网页是否初次创建
	renewRes(cur,1);
	renewViewOrder(cur);

	while (1) {
		isCreate = 0;
		resWeb = inputDeal(&type);
		switch (type) {
		case OP_VISIT:
			push(&back, cur);
			cur = resWeb;
			clearForward();
			isCreate = 1;
			break;
		case OP_BACK:
			isValid=moveBack();
			break;
		case OP_FORWARD:
			isValid=moveForward();
			break;
		case OP_QUIT_0:
		case OP_QUIT_1:
			PrintAllWeb(type);
			release(&res);
			return 0;
		default:
			break;
		}
		if (type&&isValid) {
			//如果是有效轮则type大于零
			renewRes(cur,isCreate);//每轮开始将cur记录到res用于统计“网站”数量
			renewViewOrder(cur);
		}
	}
	return 0;
}
void release(Stk* stkname) {
	int len = stkname->top;
	if (!len)return;
	for (int i = 1; i < len; ++i) {//baidu是栈上变量不能free，最好统一
		free(stkname->Stk[i]);
	}
}
void renewViewOrder(const web* webPage) {
	push(&viewOrder, webPage);
}
const web* inputDeal(int *type) {//返回一个不可修改的web指针
	fgets(buf, sizeof(buf), stdin);
	buf[strlen(buf) - 1] = '\0';
	int returnType=0;//初始化，需要设置为零，如果这轮不是有效轮次return0
	switch (buf[0]) {
	case 'V':
		returnType = OP_VISIT;
		break;
	case '<':
		returnType = OP_BACK;
		break;
	case '>':
		returnType = OP_FORWARD;
		break;
	case 'Q':
		if (buf[strlen(buf) - 1] == '0')returnType = OP_QUIT_0;
		else returnType = OP_QUIT_1;
		break;
	dafault:
		break;
	}
	*type = returnType;

	if (returnType == OP_VISIT) {
		web* webPage = (web*)malloc(sizeof(web));
		if (webPage == NULL)return NULL;
		webPage->viewNum = 1;
		strcpy( webPage->name, buf+(int)strcspn(buf," ") + 1);
		return webPage;
	}
	return NULL;
}
void push(Stk* stkname, const web* webPage) {
	int top = stkname->top;
	stkname->Stk[top] = webPage;
	stkname->top++;
}
void* pop(Stk* stkname) {
	if (!stkname->top)printf("ERROR\n");
	stkname->top--;
}
void clearForward() {
	forward.top = 0;
}
void renewRes(const web* webPage,int isCreate) {
	//将一个web指针查找入栈，同时更新最值
	int lenRes = res.top;
	for (int i = 0; i < lenRes; ++i) {
		if (strstr( webPage->name, res.Stk[i]->name)!=NULL) {
			//查找子串
			res.Stk[i]->viewNum++;
			if (res.Stk[i]->viewNum > maxViewNum) {
				maxViewNum = res.Stk[i]->viewNum;
				maxIndexRes = i;
			}
			if (strcmp(webPage->name, res.Stk[i]->name)==0)return;//same->no push
			break;//subpage->push
		}
	}
	if(isCreate)//初次创建才入res
		res.Stk[res.top++] = webPage;
}
web* getTop(Stk* stkname) {
	if (!stkname->top) {
		printf("ERROR");
		return NULL;
	}
	return stkname->Stk[(stkname->top) - 1];
}
int isEmpty(Stk* stkname) {
	return stkname->top==0;//判空的逻辑
}
int moveBack() {
	//直接操作在全局的cur
	if (isEmpty(&back))return 0;

	push(&forward, cur);
	cur = getTop(&back);
	pop(&back);
	return 1;
}
int moveForward() {
	if (isEmpty(&forward))return 0;

	push(&back, cur);
	cur = getTop(&forward);
	pop(&forward);
	return 1;
}
void PrintAllWeb(int type) {
	int lenView = viewOrder.top;
	for (int i = 0; i < lenView; ++i) {
		printf("%s\n", viewOrder.Stk[i]->name);
	}
	if (type == OP_QUIT_0) {
		//额外打印最大值
		int index = 0;
		web* maxWeb = res.Stk[maxIndexRes];
		for (; maxWeb->name[index] != 'w'; ++index);

		for(;maxWeb->name[index]!='/';++index){
			printf("%c", maxWeb->name[index]);
		}
		printf(" %d", maxViewNum);
	}
}
