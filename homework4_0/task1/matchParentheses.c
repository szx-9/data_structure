#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
typedef struct info {
	int sign;
	int row;
}info;
void PrintAns();
void PrintError(int sign, int row);
int dealRightParen(int matchSign);
int dealLeftParen(FILE* fin);
void skipQuotationMark(FILE* fin, int matchSign);
void skipNote(FILE* fin);
info getTop(info* flagStk);
void popFlag();
void push(int sign, int row);
/*
数据结构:符号栈（匹配括号），存储栈（记录答案）
忽略：字符常量（双，单引号内容），注释（单行多行）

progress：
1读取字符：判断分支

2
	1/：注释，单行和多行
	2单引号或者双引号：匹配位置
	3括号
	4其他
*/

info flagStk[220];
int ansStk[220];


int curRowNum = 1;
int ansTop = 0, flagTop = 0;//待插入位置
int main()
{
	FILE* fin = fopen("example.c", "r");
	int curChar;
	int state = 1;
	while ((curChar = getc(fin)) != EOF) {
		switch (curChar) {
		case '/':
			skipNote(fin);
			break;
		case '\'':
		case'\"':
			skipQuotationMark(fin, curChar);
			break;
		case '(':
			push(curChar, curRowNum);
			ansStk[ansTop++] = '(';
			break;
		case '{':
			state=dealLeftParen(fin);
			if (!state) {
				goto a;
			}
			break;
		case '\n':
			curRowNum++;
			break;
		case ')':
		case '}':
			state=dealRightParen(curChar);
			if (!state) {
				goto a;
			}
			break;
		default:
			break;
		}
	}

	if (!isEmpty()) {
		PrintError(getTop(flagStk).sign, getTop(flagStk).row);
		return 0;
	}
	PrintAns();
	a:
	fclose(fin);
	return 0;
}
void PrintAns() {
	for (int i = 0; i < ansTop; ++i) {
		printf("%c", ansStk[i]);
	}

}
void PrintError(int sign, int row) {
	printf("without maching '%c' at line %d\n", sign, row);
}
int dealRightParen(int matchSign) {
	if (isEmpty())return 0;
	if ((matchSign == ')' && getTop(flagStk).sign != '(')
		|| (matchSign == '}' && getTop(flagStk).sign != '{')) {
		PrintError(matchSign, curRowNum);
		return 0;
	}
	ansStk[ansTop++] = matchSign;
	popFlag();
	return 1;
}

int dealLeftParen(FILE* fin) {
	//处理{
	info curTop = getTop(flagStk);
	if (curTop.sign == '(') {
		PrintError(curTop.sign, curTop.row);
		return 0;
	}
	//遇到{，入栈
	ansStk[ansTop++] = '{';
	int sign = '{';
	push(sign,curRowNum);//只有“（”需要row字段
	return 1;
}

void skipQuotationMark(FILE* fin, int matchSign) {
	int buf;
	while ((buf = getc(fin)) != matchSign) {
		if (buf == '\\') {
			getc(fin);
		}
	}
}

void skipNote(FILE* fin) {
	//注释不需要处理转义字符
	int type = fgetc(fin);
	int buf;
	if (type == '/') {
		//单行注释
		while ((buf = getc(fin) != '\n'));
		curRowNum++;
		return;
	}
	while( (buf = getc(fin) )!= EOF) {
		if (buf == '*') {
			if (getc(fin) == '/')
				return;
			fseek(fin, -1, SEEK_CUR);//回调
		}
		else if (buf == '\n')curRowNum++;
	}
}
info getTop(info* flagStk) {
	return flagStk[flagTop - 1];
}
void popFlag() {
	if (!flagTop) {
		printf("ERROR\n");
		return;
	}
	flagTop--;
}
void push(int sign,int row) {
	flagStk[flagTop].row = row;//保证鲁棒性和多种case，需要规范push每次都输入row防止最后数据混乱
	flagStk[flagTop++].sign = sign;
}
int isEmpty() {
	return flagTop;
}