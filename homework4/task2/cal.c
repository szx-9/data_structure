#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
首先
栈设计（数组）
利用联合体，int和char，
读取输入时：
1遇到数字直接搜索到了符号为止，然后带着这个符号去2，或者=break
2遇到左括号直接入栈，右括号则将栈顶符号弹出然后加入表达式，同时遍历将左括号删除
3遇到操作符：如果栈空或者栈顶不是操作符直接入栈，
如果栈顶 操作符优先级大于当前 需要将栈顶操作符弹出然后将当前入栈

当遍历完毕表达式遇到=直接break，

然后利用你后缀表达式新建一个ans联合体数组



*/

typedef struct element {
	double num;
	char op;
}element;//全局0初始化

enum {
	ADD=2,
	DEL=3,
	MUL=6,
	DIV=7,
	LEF=1,
	RIG=8
};


char stk[1000];//存放转化过程中的各种符号的辅助栈，只用char即可
element ans[1000];//存放逆波兰表达式的数组
char resNum[6];

double output[50];

int isLower(int a, int b);
int isCalOp(int a);
double cal(double, double, int);
int isOp(int);
int main()
{
	char input;
	int stkTop = 0, ansTop=0;//都是待插入位置
	int resTop = 0;
	int outTop = 0;
	int type;
	while (1) {
		resTop = 0;//每一轮重置数字缓冲区
		scanf(" %c", &input);//input作为字符类型，存储当前输入
		if (input == '=') {
			//将stk中所有符号弹出装入
			while (stkTop>0) {
				ans[ansTop++].op = stk[--stkTop];
			}
			break;
		}
		if (isdigit(input)) {//判断是否是字符形式的数字
			
			do {
				resNum[resTop++] = input;
				scanf(" %c", &input);
			} while (isdigit(input));
			
			//此时input中存储应该处理的符号
			resNum[resTop] = '\0';
			int cur = atoi(resNum);//得到当前数字
			ans[ansTop++].num = cur;//将当前数位的答案存入ans中

		}
		//此时如果input原本是数字，现在就是字符
		if ( type=isOp(input) ) {//以type的值作为判断，空白符直接跳过
			if (!stkTop||type==LEF) {
				//栈空 左括号：直接入栈
				stk[stkTop++]= input;//这里应该是将具体符号传入而不是type
				continue;
			}
			
			else if (type == RIG) {
				//右括号，此时应该进行出栈操作，直到栈顶是左括号
				char curOp;
				do {
					curOp = stk[--stkTop];
					ans[ansTop++].op = curOp;
					
				} while (  isCalOp(isOp(stk[stkTop-1]))  );
				//此时stk-1遇到左括号跳出，
				--stkTop;//将栈顶设置为左括号处
			}
			else {
				//遇到某个操作符,不是左右括号
				int curTop = stk[stkTop - 1];
				int curTopType = isOp(curTop);
				if(! isCalOp(curTopType) ){
				//如果栈顶不是操作符,入栈
					stk[stkTop++] = input;
				}
				else {
					//如果栈顶的操作符优先级高于或者等于自己：即当前操作符优先级更高的反面
					if(stkTop>0 && ! isLower(isOp(stk[stkTop-1]), type )) {
						ans[ansTop++].op = stk[--stkTop];
					}
					stk[stkTop++] = input;
				}
			}
		}
	}
 	//此时ans中存储逆波兰表达式,需要遍历ans，进行对output输出
	int ansIndex = 0;
	while (ansIndex < ansTop) {
		char curType = ans[ansIndex].op;//直接取出op，如果是数字则为零，否则不是零,
		if (curType) {
			//只异常处理操作符，数字直接填入就行
			outTop--;//将index更新成插入位置
			double fri = output[outTop - 1], sec = output[outTop];
			output[outTop - 1] = cal(fri, sec, isOp(curType));//注意此处参数设置，不要type和实际类型混用
		}
		else {
			//当前是数字
			output[outTop++] = ans[ansIndex].num;
		}
		ansIndex++;
	}
	printf("%.2f", output[0]);

	return 0;
}
int isLower(int a, int b) {
	if (a <= b-2)return 1;
	return 0;
}

int isCalOp(int a) {
	//判断a是不是真实算子
	if (a >= ADD && a <= DIV)return 1;
	return 0;
}

double cal(double a, double b, int op) {
	switch (op) {
	case ADD:
		return a + b;
	case DEL:
		return a - b;
	case MUL:
		return a * b;
	case DIV:
		return a / b;
	}
}

int isOp(int input) {
	if (input == '+')return ADD;
	if (input == '-')return DEL;
	if (input == '*')return MUL;
	if (input == '/')return DIV;
	if (input == '(')return LEF;
	if (input == ')')return RIG;
	return 0;//space返回0
}