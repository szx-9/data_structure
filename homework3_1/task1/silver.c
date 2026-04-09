#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
#define END_NUM 999999
/*
1插入操作，只需要插入排序即可，需要特判一下
2卖出：先比对总重量，
然后新建头节点（栈上即可），进行遍历释放，同时更新总重量
3融合，进行遍历


*/

typedef struct Node {
	int weight;
	struct Node* next;
}Node;

void printAndDelete(Node* head);
Node* add(Node* head, int weight);

Node* minus(Node* head, int weight, ll* total);

Node* merge(Node* head);


int main()
{
	ll totalWeight = 0;//记录当前银块总重量
	Node* head = NULL;//设置头节点为空节点，通过函数赋值
	int opNum;
	while (scanf("%d" ,&opNum)==1) {
		//读入操作数
		/*
		如果大于零，则进入插入逻辑
		小于零，进入卖出逻辑
		等于零，进入融合逻辑
		*/
		if (opNum == END_NUM)break;
		if (opNum > 0) {
			//插入逻辑
			head = add(head, opNum);
			totalWeight += opNum;
		}
		else if (opNum < 0) {
			head = minus(head, opNum, &totalWeight);
			//total在函数内自动更新
		}
		else {
			head = merge(head);
		}

	}

	printAndDelete(head);

	return 0;
}
void printAndDelete(Node* head) {
	if (head == NULL)return;
	Node* res = head;
	while (head) {
		res = head;
		head = head->next;
		printf("%d ", res->weight);
		free(res);
	}
	return;
}

Node* add(Node* head, int weight) {
	//返回头节点，
	Node* cur = (Node*)malloc(sizeof(Node));
	cur->weight = weight;
	cur->next = NULL;
	if (head == NULL) {
		//如果当前是空链表，则直接返回cur
		return cur;
	}
	Node* front = NULL;//head的前驱
	Node* copy = head;
	while (head) {
		if (head->weight >= cur->weight) {
			if (front) {
				//front非空，此时不是头节点
				front->next = cur;
				cur->next = head;
				return copy;
			}
			//front是空
			cur->next = copy;
			return cur;
		}
		front = head;
		head = head->next;
	}
	front->next = cur;
	return copy;

}

Node* minus(Node* head, int weight, ll* total) {
	if (head == NULL) return NULL;
	weight = -weight;

	if ((ll)weight > *total) return head;

	if ((ll)weight == *total) {
		Node* p;
		while (head) {
			p = head;
			head = head->next;
			free(p);
		}
		*total = 0;
		return NULL;
	}

	while (head && weight > 0) {
		if (head->weight <= weight) {
			Node* del = head;
			weight -= head->weight;
			*total -= del->weight;
			head = head->next;
			free(del);
		}
		else {
			int remain = head->weight - weight;
			*total -= weight;
			Node* del = head;
			head = head->next;
			free(del);
			head = add(head, remain);
			return head;
		}
	}
	return head;
}

Node* merge(Node* head) {
	//将所有小于10的节点删除，并进行插入
	//不需要保留head，直接用head进行遍历，
	if (head == NULL)return NULL;
	if (head->weight >= 10)return head;//没有可以融合的操作，直接return

	Node* res = NULL;
	Node* cur = (Node*)malloc(sizeof(Node));
	//用于存储融合后的节点
	cur->next = NULL;
	cur->weight = 0;
	while (head) {
		if (head->weight < 10) {
			res = head;
			head = head->next;
			cur->weight += res->weight;
			free(res);
			continue;
		}
		//如果进入这里，说明head大于等于10，此时对head进行遍历
		head = add(head, cur->weight);
		free(cur);
		return head;
	}
	//如果进入这里，说明head遍历完毕，同时所有节点都小于10
	return cur;//此时cur直接就是head节点

}