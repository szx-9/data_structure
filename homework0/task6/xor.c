#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

*/
char buf[5];
int ans[40];
int cmp(const void* a, const void* b) {
	int c = *(int*)a;
	int d = *(int*)b;
	if (c > d)return -1;
	if (c == d)return 0;
	return 1;
}
int main()
{
	int i = 0;
	int ansIndex = 0;
	int cnt = 0;
	while (cnt<2) {
		char c = getchar();
		if (c == ' ' || c == '\n') {
			if (c == '\n')cnt++;
			buf[i++] = '\0';
			ans[ansIndex++] = atoi(buf);
			i = 0;
		}
		else {
			buf[i++] = c;
		}
	}
	qsort(ans, ansIndex, sizeof(int), cmp);
	for (int j = 0; j < ansIndex; ++j) {
		if ((j < ansIndex - 1 && ans[j] == ans[j + 1]) || (j > 0 && ans[j] == ans[j - 1]))continue;
		else printf("%d ", ans[j]);
	}
	return 0;
}