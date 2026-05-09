#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define data_type int
/*
* 本文件用于编写一个伪泛型的单向链表(额外维护了tail，可以直接构建循环单项链表)
* 主要泛型实现是通过data_type，在任务中可以直接进行macro定义，可以将data_type定义为一个指针
* 
* 对于一个单向链表,我们这里采用设置dummy，用head指向
* 


注意事项：
1 首先创建结点时：记得置空NULL
2 插入结点时，记得看一下采取什么方式
	如果是真实头结点，则需要返回头节点
	dummy：直接插入即可
	（插入时，一般需要设置front前驱结点更改指向，dummy则dummy，真实则NULL
3 释放结点时，记得先缓存再释放，也可以不释放
4 

*/
typedef struct Node {
	data_type data;
	struct Node* next;
}Node;

typedef struct S_List {
	Node* head;//head实际指向dummy，方便初始化一个链表
	Node* tail;
	int len;//链表长度
}S_List;

void S_List_init(S_List* list) {
	//初始化一个链表，将头尾指针malloc
	list->head = list->tail = (Node*)malloc(sizeof(Node));
	list->head->next = list->tail;
	list->tail->next = NULL;
	list->len = 0;
}

Node* bulidNode(data_type data) {
	//将datatype对应的数据传入，然后返回创建的节点
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL) {
		printf("ERROR_MALLOC\n");
		return NULL;
	}
	
	//TODO
	newNode->data = data;
	
	newNode->next = NULL;
	return newNode;
}

void S_List_insert(S_List* list, Node* target, Node* newNode) {
	//在list某个target后加入newNode
	//target=tail即尾插法，target=head即头插法
	newNode->next = target->next;
	target->next = newNode;
	list->len++;
}

void S_List_erase() {
	//

}
