#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
ds：辅助栈：存储转换的RP表达式，元素数字或者操作符，
progress:
1数字：一直读取到符号，然后数字入栈后判断符号，
2有效符号（notspace）：
	左括号直接入栈，

*/
enum {
	LEF=1,
	
	ADD=3,
	MIN=4,
	
	MUL=6,
	DIV=7,
	
	RIG=8,
	IS_SIGN=9,//符号
	IS_NUM=10,//数字
};

typedef struct element {
	int type;//判断当前元素的类型,
	double data;//存储数字或者符号，根据type决定
}ele;

typedef struct stk {
	ele stk[200];//栈
	int top;
}Stk;
Stk opRes,exRes, ans;//存储表达式和答案计算

void pop(Stk* stkname) {
	if (!stkname->top) {
		printf("ERROR_POP\n");
		return;
	}
	stkname->top--;
}
void push(Stk* stkname,ele element) {
	stkname->stk[stkname->top].type = element.type;
	stkname->stk[stkname->top++].data = element.data;
}
ele getTop(Stk* stkname) {
	//获取栈顶的元素
	return stkname->stk[stkname->top - 1];
}
ele buildEle(int type,double data) {
	ele returnEle;
	returnEle.type = type;
	if (type == IS_SIGN) {
		//对于符号，data是具体字符需要转化成enum
		int rt=0;
		switch ((int)data) {
		case '+':
			rt = ADD; break;
		case '-':
			rt = MIN; break;
		case '*':
			rt = MUL; break;
		case '/':
			rt = DIV; break;
		case '(':
			rt = LEF; break;
		case ')':
			rt = RIG; break;
		defalut:
			printf("ERROR_BUILD\n");
		}
		returnEle.data = rt;
		return returnEle;
	}
	returnEle.data = data;
	return returnEle;
}
int returnType(int cI) {
	//判断当前输入的字符属于什么类型数据
	if (isdigit(cI))return IS_NUM;
	if (cI == '+' || cI == '-' || cI == '*' ||
		cI == '/' || cI == '(' || cI == ')' || cI == '=')return IS_SIGN;
	printf("ERROR_RETURNTYPE\n");
	return 0;
}
int isEmpty(Stk* stkname) {
	return stkname->top == 0;
}
int isLower(ele a, ele b) {
	//a是否严格优先级低于b
	if (b.data-2>=a.data)return 1;//a严格比b优先
	return 0;
}
double cal(double a, double b, int op) {
	switch (op) {
	case ADD:
		return a + b;
	case MIN:
		return a - b;
	case MUL:
		return a * b;
	case DIV:
		return a / b;
	default:
		printf("ERROR_CAL\n");
	}
	return 0;
}

char bufNum[6];//存储一个数字用于atoi
int bufIndex = 0;
int main()
{
	int curInput;
	while (1) {
		bufIndex = 0;
		scanf(" %c", &curInput);
		if (curInput == '=') {
			//将辅助栈opRes中所有符号弹出
		exit:
			while (!isEmpty(&opRes)) {
				push(&exRes, getTop(&opRes));
				pop(&opRes);
			}
			break;
		}
		if (returnType(curInput) == IS_NUM) {
			bufNum[bufIndex++] = curInput;
			scanf(" %c", &curInput);
			while (isdigit(curInput)) {
				bufNum[bufIndex++] = curInput;
			}
			//curInput现在指向一个非数字符号,需要直接入判断符号的环节
			bufNum[bufIndex] = '\0';
			int num = atoi(bufNum);
			push(&exRes, buildEle(IS_NUM, num));
		}
		if (returnType(curInput) == IS_SIGN) {
			if (curInput == '=')goto exit;

			ele curEle = buildEle(IS_SIGN, curInput);

			if (isEmpty(&opRes) || curInput == '(') {
				push(&opRes, buildEle(IS_SIGN, curInput));
			}
			else if (curInput == ')') {
				//需要弹出左右括号之间所有符号，入栈
				while (getTop(&opRes).data != LEF) {
					//没有遇到左括号
					push(&exRes, getTop(&opRes));
					pop(&opRes);
				}
				//当前栈顶是左括号
				pop(&opRes);
			}
			else {//运算符处理
				//top优先级高或者相同，则top poppush，cur push to opRes，
				//cur优先级严格高（即top严格低），则cur push  to opRes
				if (isLower(getTop(&opRes), curEle)) {
					push(&opRes, curEle);
				}
				else {
					push(&exRes, getTop(&opRes));
					pop(&opRes);
					push(&opRes, curEle);
				}
			}
		}
	}
	//exRes存储表达式
	int lenEx = exRes.top;
	for (int i = 0; i < lenEx; ++i) {
		ele curEle = exRes.stk[i];
		if (curEle.type == IS_NUM) {
			push(&ans, curEle);
		}
		else {
			ele sec = getTop(&ans);
			pop(&ans);
			ele fir = getTop(&ans);
			pop(&ans);
			push(&ans, buildEle(IS_NUM, cal(fir.data, sec.data, curEle.type)));
		}
	}
	printf("%.2f", ans.stk[0].data);

	return 0;
}