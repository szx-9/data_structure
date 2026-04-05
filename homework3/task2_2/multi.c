#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
1创建结构体：一个单项式：包含系数 指数
2进行数据输入：两个两个输入，直到某一次输入了零指数
利用回车进行切换输入，说明要用字符格式进行读取转化
利用fgets读取一行，设置索引，读到字符进入内层循环，直到读到space添加\0，
然后atoi，申请链表


生成新节点cur后进行插入
1如果head为空，此时将head设置为cur
2普通情况：直接将tail的后继指向cur，然后tail赋值cur


*/
typedef struct mono {
	int coef;
	int index;
}mono;

typedef struct node {
	mono item;
	struct node* next;
}node;

char buf[100];

int main()
{
	node* head1 = NULL, * head2 = NULL, *tail=NULL;
	
	int cnt = 1;//记录现在是第几个输入数字
	
	fgets(buf, sizeof(buf), stdin);
	char * num=strtok(buf, " \n");
	head1 = (node*)malloc(sizeof(node));
	head1->item.coef = atoi(num);
	head1->item.index = 0;

	cnt++;//表示偶数，应该开始指数读入
	tail = head1;
	//创建了head1同时读入coef，等待读入index中

	while (num = strtok(NULL, " \n")) {
		if ( (cnt++) & 1) {
			//奇数
			node* cur = (node*)malloc(sizeof(node));
			cur->next = NULL;
			cur->item.coef = atoi(num);
			cur->item.index = 0;

			tail->next = cur;
			tail = cur;
		}
		else {
			//偶数，应该输入应该处理index并将其加入tail中
			tail->item.index += atoi(num);
		}

	}
	//读到buf\0
	cnt = 1;
	fgets(buf, sizeof(buf), stdin);
	num = strtok(buf, " \n");
	
	head2 = (node*)malloc(sizeof(node));
	head2->item.coef = atoi(num);
	head2->item.index = 0;

	cnt++;//表示偶数，应该开始指数读入
	tail = head2;
	//创建了head1同时读入coef，等待读入index中

	while (num = strtok(NULL, " \n")) {
		if ((cnt++) & 1) {
			//奇数
			node* cur = (node*)malloc(sizeof(node));
			cur->next = NULL;
			cur->item.coef = atoi(num);
			cur->item.index = 0;

			tail->next = cur;
			tail = cur;
		}
		else {
			//偶数，应该输入应该处理index并将其加入tail中
			tail->item.index += atoi(num);
		}

	}
	//此时head1，2储存了两个多项式，
	//此时应该新建一个链表节点ans，然后双层遍历

	node* ans = NULL;
	tail = NULL;

	node * res2 = head2;
	while (head1) {

		while(head2){
			//生成当前结果项
			tail = ans;//tail为游标，需要每次重置

			node* cur = (node*)malloc(sizeof(node));
			cur->next = NULL;
			cur->item.index = head1->item.index + head2->item.index;
			cur->item.coef = head1->item.coef * head2->item.coef;
			
			//检索插入到ans中
			if (ans == NULL) {
				//空链表，tail为空
				ans = cur;
				tail = ans;
			}
			else {
				//非空，此时tail指向ans
				//遇到第一个当前大的，插入
				if (cur->item.index > tail->item.index) {
					cur->next = tail;
					ans = cur;
					break;
				}
				node* front = NULL;//在tail节点的前面
				while (tail) {
					if (cur->item.index == tail->item.index) {
						tail->item.coef += cur->item.coef;
						free(cur);
						break;
					}
					if (cur->item.index>tail->item.index) {
						cur->next = tail;
						front->next = cur;
						break;
					}
					front = tail;
					tail = tail->next;
				}
				if (tail == NULL) {
					front->next = cur;
				}
			}
			head2 = head2->next;
		}
		//此时head2已经遍历完一遍了，需要重置
		head2 = res2;
		head1 = head1->next;
	}
	//此时ans中存储答案
	int first = 1;//表示当前是第一个输出
	node* res = NULL;
	while (ans) {
		res = ans;
		ans = ans->next;
		if (res->item.coef) {
			//系数不为零
			printf("%d %d ", res->item.coef,res->item.index);
		}
		free(res);
	}



	return 0;
}
