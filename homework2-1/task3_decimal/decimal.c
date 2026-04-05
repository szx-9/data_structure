#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

*/
char buf[55];
char base[55];
char exp1[20];

int main()
{
	scanf("%s", buf);
	int bufIndex = 0;
	int len = strlen(buf);
	//首先输入所有有效数字
	int ansIndex = 0;
	while (bufIndex < len) {
		if (buf[bufIndex] == 'e' || buf[bufIndex] == 'E')break;

		if (buf[bufIndex] >= '0' && buf[bufIndex] <= '9') {
			base[ansIndex++] = buf[bufIndex];
		}
		
		bufIndex++;
	}//此时找到了e
	base[ansIndex] = '\0';
	bufIndex++;

	int expIndex = 0;
	while (bufIndex < len) {
		exp1[expIndex++] = buf[bufIndex];
		bufIndex++;
	}
	exp1[expIndex] = '\0';
	int sign = 0;
	int pos = atoi(exp1);
	sign = pos > 0 ? 1 : -1;
	if (!pos) {
		sign = 0;
	}
	//此时有效数字在base中，数字移位在pos中，正负号在sign中
	int baseLen = strlen(base);
	if (sign == 1) {
		printf("%c", base[0]);
		for (int i = 1; i < baseLen; ++i) {
			printf("%c", base[i]);
			if (i == pos && i!=baseLen-1)printf(".");
		}
		if (baseLen - 1 < pos) {
			int loop = pos - baseLen + 1;
			for (int i = 0; i < loop; ++i) {
				printf("0");
			}
		}
	}
	else if (sign==0) {
		printf("%c.", base[0]);
		for (int i = 1; i < baseLen; ++i) {
			printf("%c", base[i]);
		}
	}
	else {
		pos = -pos;
		printf("0.");
		int loop = pos - 1;
		for (int i = 0; i < loop; ++i) {
			printf("0");
		}
		for (int i = 0; i < baseLen; ++i) {
			printf("%c", base[i]);
		}
	
	}


	return 0;
}
