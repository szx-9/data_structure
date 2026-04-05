#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
核心用一个char数组存储所有字符，用一个index索引某个位置的索引
然后这个位置置1，单独设置一个hash


*/
int hash[130];
char oldStr[25];
char newStr[25];
char compare[25];
char buf[200];

int isSame(char buf, char* compare, int* hash, int i, int scanIndex);
int isChar(char);
int isSameChar(char, char);
int main()
{
	int index;
	scanf("%s", oldStr);
	getchar();
	scanf("%s", newStr);
	getchar();


	int lenOld = strlen(oldStr);
	int comIndex = 0;
	int scanIndex=-1;//注意可能没有扫描集，此时用-1表示
	int lenRealOld = 0;
	for (int i = 0; i < lenOld; ++i) {
		if ( isChar(oldStr[i])) {
			//当前是字母
			compare[comIndex++] = oldStr[i];
			lenRealOld++;
		}
		else {
			//遇到扫描集
			scanIndex = comIndex;//表明当前特殊的扫描集在i处
			compare[comIndex++] = 1;
			lenRealOld++;
			++i;//进入字母部分
			while (isChar(oldStr[i])) {
				//如果当前遍历位置仍然是字母
				char ch = oldStr[i];
				if (ch >= 'a' && ch <= 'z') {
					hash[ch ] = 1;
				}
				else if (ch >= '0' && ch <= '9') {
					hash[ch] = 1;
				}
				else {
					hash[ch] = 1;
				}
				i++;
			}
			//此时i指向]
		}
	}
	//此时comp和hash定义完毕
	//接下来读入一行，输出一行
	while (fgets(buf, sizeof(buf), stdin)) {
		//按照一行输入
		int bufLen = strlen(buf);
		int bufIndex = 0;
		while (bufIndex<bufLen) {
			
			//只有当 当前字母等于对比集合中第一个字母时才进行下一步
			if (isSame(buf[bufIndex], compare, hash, 0, scanIndex)) {
				//只有当当前字符与标准集中第一个匹配了再进行搜索
				int i = 1;
				int flag = 1;
				for (; i < lenRealOld; ++i) {
					if (!isSame(buf[bufIndex+i], compare, hash, i, scanIndex)) {
						//如果遍历过程在没有遇到空格前，且发现每一次均匹配，则flag为1
						flag = 0;
						break;
					}
					if (bufIndex+i>=bufLen) {//如果匹配过程发现遍历到了行末
						flag = 0;
						break;
					}
				}
				if (flag == 1) {
					//如果匹配成功
					printf("%s", newStr);
					bufIndex = bufIndex + i;
				}
				else {
					//没有匹配成功，
					printf("%c", buf[bufIndex++]);
				}
			}
			else {
				printf("%c", buf[bufIndex++]);//为了处理第一个字符没有匹配的情况
			}
		}
		printf("\n");

	}




	return 0;
}
int isSameChar(char a, char b) {
	int ans = abs(a - b);
	int st = abs('a' - 'A');
	if (ans == st||ans==0)return 1;
	return 0;
}


int isSame(char buf, char* compare, int* hash,int i,int scanIndex) {
	//对于当前遍历字符buf，在compare中对比，是否是第i个字符
	if (i != scanIndex) {
		if (  isSameChar(buf,compare[i]))return 1;
		return 0;
	}
	else {
		//如果buf是数字，则直接判断
		//如果是字母，需要区分大小写
		if (hash[buf] == 1)return 1;
		if (buf >= 'a' && buf <= 'z') {
			if (hash[buf - 'a' + 'A'] == 1)return 1;
		}
		if (buf >= 'A' && buf <= 'Z') {
			if (hash[buf - 'A' + 'a'])return 1;
		}
		return 0;
	}
}

int isChar(char c) {
	if (c >= 'a' && c <= 'z')return 1;
	if (c >= 'A' && c <= 'Z')return 1;
	if (c >= '0' && c <= '9')return 1;

	return 0;
}