#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

*/

typedef struct Node {
	int num;
	int cnt;
	struct Node* next;
}Node;
Node* insert(Node* head, int input, int* cmpCnt);

void PrintAndDelete(Node* head);

int main()
{
	int n;
	scanf("%d", &n);
	Node* head = NULL;
	int cmpCnt = 0;

	int input;
	while (n--) {
		scanf("%d", &input);
		head=insert(head,input,&cmpCnt);
	}
	printf("%d\n", cmpCnt);
	PrintAndDelete(head);



	return 0;
}

void PrintAndDelete(Node* head) {
	//打印前五个节点
	if (head == NULL) {
		return;
	}
	int cnt = 0;
	//cnt为5说明已经打印了五个节点
	Node* res = head;
	while (cnt < 5&&head) {
		res = head;
		head = head->next;
		printf("%d %d\n", res->num, res->cnt);
		free(res);

		cnt++;
	}


}


Node* insert(Node* head,int input,int* cmpCnt) {
	Node* cur = (Node*)malloc(sizeof(Node));
	cur->next = NULL;;
	cur->num = input;
	cur->cnt = 1;
	if (head == NULL) {
		//如果是空链表
		return cur;
	}
	Node* move = head;
	//游标指针
	Node* front = NULL;//move的前驱
	while (move) {
		if (move->num == cur->num) {
			(move->cnt)++;
			(*cmpCnt)++;

			//此时需要考虑，front
			if (front) {
				//只有front不是NULL时，才需要将节点移动，front等于零的时候说明已经在头部
				free(cur);
				front->next = move->next;
				move->next = head;
				return move;//此时move为头节点
			}
			free(cur);
			return head;//如果已经是头节点，直接返回
		}
		front = move;
		move = move->next;
		(*cmpCnt)++;
	}
	//到达这里说明move遍历完成后没有看到相同的
	front->next = cur;//此时front指向链表末尾
	return head;
}