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
	ll totalWeght = 0;//记录当前银块总重量
	Node* head = NULL;//设置头节点为空节点，通过函数赋值
	int opNum;
	while (1) {
		if (scanf("%d", &opNum)==EOF) {
			break;
		}//读入操作数
		/*
		如果大于零，则进入插入逻辑
		小于零，进入卖出逻辑
		等于零，进入融合逻辑
		*/
		if (opNum == END_NUM)break;
		if (opNum > 0) {
			//插入逻辑
			head = add(head, opNum);
			totalWeght += opNum;
		}
		else if (opNum < 0) {
			head = minus(head, opNum, &totalWeght);
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
		if (head->weight > cur->weight) {
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
	if (head == NULL)return NULL;
	weight = -weight;//此时weight等于需要减去的数量
	if ((ll)weight == *total) {
		//需要释放节点
		Node* res = head;
		while (head) {
			res = head;
			head = head->next;
			free(res);
		}
		return NULL;
	}
	if ((ll)weight > *total) {
		return head;
	}
	Node* res = head;//用于实时释放节点
	while (weight > 0) {
		if (head->weight == weight) {
			//当前节点正好满足释放要求，释放这个节点后直接return其后继即可
			*total -= weight;
			Node* ans = head->next;
			free(head);
			return ans;
		}
		else if (head->weight > weight) {
			//如果当前节点发现多余了、
			head->weight -= weight;
			*total -= weight;
			return head;//当前节点减去需要的重量后继续成为头节点

		}
		else {
			//当前节点不够，需更新weight，total，并进行释放
			res = head;
			*total -= head->weight;
			weight -= head->weight;

			head = head->next;
			free(res);
		}
	}
	return NULL;
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
