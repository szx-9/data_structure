#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
对于一个位置
如果空，跳过
否则根据所处位置进行方向搜索
搜索过程中，如果遇到了
*/

typedef enum {
	empty = 0,
	black = 2,
	white = 1
}Piece;

int chess[19][19];
int judge(int chess[][19], int i, int j, int type);
int main()
{
	int cntWhite = 0, cntBlack = 0;
	int lastRow, lastcol;
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			scanf("%d", &chess[i][j]);
			if (chess[i][j] == white) {
				lastRow = i;
				lastcol = j;
				cntWhite++;
			}
			else if (chess[i][j] == black) {
				lastRow = i;
				lastcol = j;
				cntBlack++;
			}
		}
	}
	if (cntWhite < 4 && cntBlack < 4) {
		printf("No");
		return 0;
	}
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			if (chess[i][j] == empty)continue;
			if (chess[i][j] == white) {
				int ans = judge(chess, i, j, white);
				if (ans == 0) {
					printf("\n");
					return 0;
				}
			}
			else {
				int ans = judge(chess, i, j, black);
				if (ans == 0) {
					printf("\n");
					return 0;
				}
			}
		}
	}
	printf("No\n");

	return 0;
}
int judge(int chess[][19], int i, int j, int type) {
	{//纵向搜索
		int cntCur = 1;
		int obs = 0;
		int posRow = i, posCol = j;
		for (int k = 1; i - k >= 0; ++k) {//向上搜索
			int cur = chess[i - k][j];
			if (cur != type) {
				if (cur == empty) {
					obs = 1;
				}
				break;
			}
			if (cur == type) {
				posRow = i - k;
				cntCur++;
			}
		}
		for (int k = 1; i + k < 19; ++k) {//向上搜索
			int cur = chess[i + k][j];
			if (cur != type) {
				if (cur == empty) {
					obs = 1;
				}
				break;
			}
			if (cur == type)cntCur++;
		}
		if (cntCur >= 4 && obs == 1) {
			printf("%d:%d,%d", type, posRow + 1, posCol + 1);
			return 0;
		}
	}
	{//水平搜索
		int cntCur = 1;
		int obs = 0;
		int posRow = i, posCol = j;
		for (int k = 1; j - k >= 0; ++k) {//向上搜索
			int cur = chess[i][j - k];
			if (cur != type) {
				if (cur == empty) {
					obs = 1;
				}
				break;
			}
			if (cur == type) {
				posCol = j - k;
				cntCur++;
			}
		}
		for (int k = 1; j + k < 19; ++k) {//向上搜索
			int cur = chess[i][j + k];
			if (cur != type) {
				if (cur == empty) {
					obs = 1;
				}
				break;
			}
			if (cur == type)cntCur++;
		}
		if (cntCur >= 4 && obs == 1) {
			printf("%d:%d,%d", type, posRow + 1, posCol + 1);
			return 0;
		}
	}
	{//左斜向搜索
		int cntCur = 1;
		int obs = 0;
		int posRow = i, posCol = j;
		for (int k = 1; i - k >= 0 && j - k >= 0; ++k) {//向上搜索
			int cur = chess[i - k][j - k];
			if (cur != type) {
				if (cur == empty) {
					obs = 1;
				}
				break;
			}
			if (cur == type) {
				posRow = i - k;
				posCol = j - k;
				cntCur++;
			}
		}
		for (int k = 1; i + k < 19 && j + k < 19; ++k) {//向上搜索
			int cur = chess[i + k][j + k];
			if (cur != type) {
				if (cur == empty) {
					obs = 1;
				}
				break;
			}
			if (cur == type)cntCur++;
		}
		if (cntCur >= 4 && obs == 1) {
			printf("%d:%d,%d", type, posRow + 1, posCol + 1);
			return 0;
		}
	}
	{//右斜向搜索
		int cntCur = 1;
		int obs = 0;
		int posRow = i, posCol = j;
		for (int k = 1; i - k >= 0 && j + k < 19; ++k) {//向上搜索
			int cur = chess[i - k][j + k];
			if (cur != type) {
				if (cur == empty) {
					obs = 1;
				}
				break;
			}
			if (cur == type) {
				posRow = i - k;
				posCol = j + k;
				cntCur++;
			}
		}
		for (int k = 1; i + k < 19 && j - k >= 0; ++k) {//向上搜索
			int cur = chess[i + k][j - k];
			if (cur != type) {
				if (cur == empty) {
					obs = 1;
				}
				break;
			}
			if (cur == type)cntCur++;
		}
		if (cntCur >= 4 && obs == 1) {
			printf("%d:%d,%d", type, posRow + 1, posCol + 1);
			return 0;
		}
	}
	return 1;
}
