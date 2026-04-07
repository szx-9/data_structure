#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
考虑全局内设置dummy作为头节点（因为本题中没有头插法）
insert返回尾节点tail
insert函数接受当前链表的尾节点，创建节点插入，同时返回尾节点，
初始的时候dummy作为空节点


*/
typedef struct Node {
	int num;
	struct Node* next;
}Node;

int rec[105];//用于记录第一次遍历时的链表各个节点，并进行排序

Node* insert(Node* tail, int input, int rec[], int* recIndex);
int cmp(const void* a, const void* b);
void solve(Node* head, Node* tail, int rec[], int recLen);
int main()
{
	int n;
	scanf("%d", &n);
	
	Node dummy ;//随机初始化了一个Node变量，野数据
	Node* tail = &dummy;
	int input;
	int recIndex = 0;
	while (n--) {
		scanf("%d", &input);
		//先组装循环链表，
		tail=insert(tail, input,rec,&recIndex);
	}
	//此时tail就是链表尾部
	qsort(rec, recIndex, sizeof(int), cmp);
	//对rec进行排序
	tail->next = dummy.next;
	Node* head = dummy.next;//最初头节点
	//循环链表构建完成

	solve(head, tail, rec, recIndex);



	return 0;
}
int cmp(const void* a, const void* b) {
	int* a1 = (int*)a;
	int* b1 = (int*)b;
	if (*a1 > *b1)return 1;
	else if (*a1 < *b1)return -1;
	else return 0;
}

Node* insert(Node* tail, int input,int rec[], int* recIndex) {
	//输入dummy虚拟头节点的地址（在全局中生命周期全局）
	
	Node* cur = (Node*)malloc(sizeof(Node));
	cur->next = NULL;
	cur->num = input;
	tail->next = cur;
	rec[*recIndex] = cur->num;
	(*recIndex)++;//执行自增

	return cur;
}

void solve(Node* head, Node* tail, int rec[], int recLen) {
	//将链表传入，传入rec,从零开始索引元素，一共循环reclen遍
	int recIndex = 0;
	Node* res = NULL;
	while (recLen--) {
		while ( (head->num)!=rec[recIndex]) {
			tail = head;
			printf("%d ", head->num);
			head = head->next;
		}
		//此时head指向了需要释放的节点
		printf("%d\n", head->num);
		res = head;
		head = head->next;
		tail->next = head;
		free(res);
		
		recIndex++;

		//一共循环长度遍，每次删除一个节点
	}
}