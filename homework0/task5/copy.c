#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

*/
char buf[1000];
int isSpace(char);
int main()
{
	FILE* fin = fopen("fcopy.in", "r");
	FILE* fout = fopen("fcopy.out", "w");
	fgets(buf, sizeof(buf), fin);
	char* cur = buf;
	while (*cur) {
		if ( !isSpace(*cur) || isSpace(*cur) && !isSpace(*(cur + 1)) ) {
			if (*cur == '\t') {
				*cur = ' ';
			}
			fputc(*cur, fout);
			cur++;
		}
		else {
			cur++;
		}
	}


	fclose(fin);
	fclose(fout);

	return 0;
}
int isSpace(char c) {
	if (c == ' ' || c == '\t')return 1;
	return 0;
}