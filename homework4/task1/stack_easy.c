#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

*/
int stk[105];
int main()
{
	int top = 0;//待插入位置

	int op,num;
	while (1) {
		scanf("%d", &op);
		switch (op) {
		case -1:
			//结束
			goto a;
		case 1:
			scanf("%d", &num);
			if (top >= 100) {
				printf("error ");
				break;
			}
			stk[top++] = num;
			break;
		case 0:
			if (top <= 0) {
				printf("error ");
				break;
			}
			printf("%d ", stk[--top]);
		}
	}
a:

	
	return 0;
}