#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
结构体：
	调用函数名称（char数组：全局不用堆空间，只进行一次复制，将nameBUf复制到funcInfo中，然后stk中用索引作为下标
	下标数组（有可能已经出现，
	或没出现：创建一个近邻下标）
	下标数组长度

数组：模拟stk  存储答案的ans

progress：
1遇到输入函数：
	检查是否ans空？或者招不到当前name重名函数的索引->创建堆空间函数名，获取索引：
	找到索引的话，直接将索引入栈

	然后入栈
		
		查看栈顶是否为空
		如果空：创建节点已经完成，不需要操作
		如果栈顶非空：将索引下标传入“栈顶”函数

2遇到弹出: 释放堆空间 ,更新栈顶索引即可，可以不用将指针置空，只要不解引用即可

每一轮循环过后检查是否栈空（维护一个栈的size）

最后根据ans检查每一个元素的索引数组是否为零，如果不是按照数组顺序输出


*/
typedef struct funcInfo{
	int len;//invo长度，便于最后索引
	int invo[11];//调用函数索引，最多10个
	char name[25];//函数名：最多20
}funcInfo;


int stk[205];//模拟栈，存储的是ans中的索引
funcInfo ans[105];//存储最多100个函数名字
char nameBuf[25];//缓存函数名称


int findFunc(char nameBuf[25], funcInfo ans[105], int ansTop);
int findIndex(int curIndex, int* invo, int len);
int main()
{
	int stkTop = 0;//待插入位置
	int ansTop = 0;//同上
	int opType;
	do {
		scanf("%d", &opType);
		if (opType == 8) {
			//进行输入操作
			scanf(" %s", nameBuf);
			int curIndex=findFunc(nameBuf,ans,ansTop);//标记当前函数在ans中索引
			if (ansTop == 0 || curIndex==-1  ) {
				//说明需要新建节点，并插入ans末尾，更新ans索引和当前获取索引,如果是-1的话，将索引改成最大值，否则是0
				strcpy(ans[ansTop].name,   nameBuf);
				ans[ansTop++].len = 0;
				if (curIndex == -1) {
					curIndex = ansTop - 1;
				}
				else {
					curIndex = 0;
				}
			}
			//检查栈顶是否为空，便于更新栈顶的的invo
			if (stkTop > 0) {
				int topPosInAns = stk[stkTop - 1];//当前栈顶元素在ans中的索引
				int insertPos = ans[topPosInAns].len;
				if (findIndex(curIndex, ans[topPosInAns].invo, insertPos)==0 ) {
					ans[topPosInAns].invo[insertPos] = curIndex;
					(ans[topPosInAns].len)++;
					
				}

			}
			stk[stkTop++] = curIndex;
		}
		else {
			stkTop--;
		}

	} while (stkTop>0);//Top同时也代表size，所以等于0也不行
	for (int i = 0; i < ansTop; ++i) {
		int invoLen = ans[i].len;
		if (!invoLen)continue;//如果没有调用函数，跳过
		printf("%s:", ans[i].name);
		for (int j = 0; j < invoLen; ++j) {
			int corrIndex = ans[i].invo[j] ;//当前第i个元素中invo索引的第j个是什么索引？
			printf("%s", ans[corrIndex].name);
			if (j < invoLen - 1) {
				printf(",");
			}
		}
		printf("\n");
	}


	return 0;
}
int findIndex(int curIndex, int* invo ,int len ) {
	//检查invo数组中有没有curIndex
	for (int i = 0; i < len; ++i) {
		if (invo[i] == curIndex)return 1;
	}
	return 0;
}

int findFunc(char nameBuf[25],funcInfo ans[105],int ansTop) {
	//返回ans数组中是否出现过nameBUf中存储的函数名：如果有返回索引，否则返回-1
	for (int i = 0; i < ansTop; ++i) {
		if (!strcmp(ans[i].name, nameBuf)) {
			return i;
		}
	}
	return -1;
}