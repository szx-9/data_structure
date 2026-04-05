#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
定义结构体info：存储一个人的名字和电话号码

接受循环次数，并根据循环次数进行读取处理info，加入数组（设置函数）

处理过程中，从后向前进行处理，如果遇到了完全相等直接清空当前的info数组位置
如果遇到了名字相同的，则也加入数组，但次数加1



*/
typedef enum {
	same = 1,//完全重复
	fam = 2,//名字重复
	new_info = 3//没有重复
};
typedef struct {
	char* name;
	char* number;
	int loop;//该名字是同类型中第几个
}info;

info res[50];//默认指针是null

void setEmpty(info*);
int judge(info*, info*);
int cmp(const void*, const void*);

int main()
{
	int n;
	scanf("%d", &n);
	int lenInfo = 0;
	while (n--) {//循环n次
		res[lenInfo].name = (info*)malloc(21);
		res[lenInfo].number = (info*)malloc(21);

		scanf(" %s", res[lenInfo].name);
		scanf(" %s", res[lenInfo].number);
		res[lenInfo].loop = 0;//默认是零
		info* input = res + lenInfo;
		for (int i = lenInfo - 1; i >= 0; --i) {
			//从后向前遍历，
			int ans = judge(res + i, input);
			if (ans == same) {
				setEmpty(input);
				lenInfo--;
				break;
			}
			else if (ans == fam) {
				input->loop = 1 + res[i].loop;
				break;
			}
		}
		lenInfo++;
	}
	qsort(res, lenInfo, sizeof(info), cmp);
	for (int i = 0; i < lenInfo; ++i) {
		printf("%s", res[i].name);
		if (res[i].loop) {
			printf("_%d", res[i].loop);
		}
		printf(" ");
		printf("%s\n", res[i].number);
	}
	for (int i = 0; i < lenInfo; ++i) {
		free(res[i].name);
		free(res[i].number);
	}

	return 0;
}
void setEmpty(info* a) {
	free(a->name);
	a->name = NULL;
	free(a->number);
	a->number = NULL;
	a->loop = 0;
}
int judge(info* a, info* b) {//判断是什莫类型
	int seqName = strcmp(a->name, b->name);
	int seqNum = strcmp(a->number, b->number);
	if (seqName == 0 && seqNum == 0) {//完全相等
		return same;
	}
	else if (seqName == 0) {
		return fam;
	}
	else return new_info;

}
int cmp(const void* a, const void* b) {
	info* c = (info*)a;
	info* d = (info*)b;
	int ans = strcmp(c->name, d->name);
	if (ans < 0)return -1;
	if (ans > 0)return 1;
	if (!ans) {
		return c->loop - d->loop;
	}
}