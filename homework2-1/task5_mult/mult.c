#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

大数乘法：
首先将两个数存入数组

*/
char num1[85];
char num2[85];

int ans[165];//!!!!!!!!大数相乘用char可能会溢出



int main()
{
	int first = 0;//表示第一个数字是否已经出现
	char ch;
	int num1Len = 0;
	while ( (ch = getchar() ) ) {
		if (ch == '\n')break;
		if (ch == '0' && first == 0)continue;//如果还没出现第一个字符且遇到了零
		num1[num1Len++] = ch;
		first = 1;
	}
	
	int first_ = 0;
	int num2Len = 0;
	char c;
	while ((c = getchar())) {
		if (c == '\n')break;
		if (c == '0' && first_ == 0)continue;
		num2[num2Len++] = c;
		first_ = 1;
	}
	
	if (num1Len == 0 || num2Len == 0) {
		printf("0");
		return 0;
	}


	for (int i = num1Len-1; i>=0; --i) {
		for (int j =num2Len-1; j>=0; --j) {
			int ri = num1Len - 1 - i;
			int rj = num2Len - 1 - j;
			int num = (num1[i] - '0') * (num2[j] - '0');
			//计算某两位的总乘积
			ans[ri + rj] += num % 10;
			ans[ri + rj + 1] += num / 10;

		}
	}
	for (int i = 0; i < 165; ++i) {
		//开始遍历ans
		if (ans == 164)break;
		if (ans[i] >= 10) {
			ans[i + 1] += ans[i] / 10;
			ans[i] =ans[i] % 10;
		}
		
	}
	int l = 0, r = 164;
	while (l < r) {
		char tmp = ans[l];
		ans[l] = ans[r];
		ans[r] = tmp;
		l++;
		r--;
	}
	int firstValid = 0;
	for (int i = 0; i < 165; ++i) {
		if (ans[i] == 0 && firstValid == 0)continue;
		if (ans[i]!=0||firstValid!=0) {
			firstValid = 1;
			printf("%d", ans [i]);
		}
	}

	return 0;
}