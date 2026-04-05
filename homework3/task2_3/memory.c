#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

1题目背景
对于循环链表（节点）：
2插入节点：当前位置开始遍历，最小块（节省空间），此时移除链表节点
并指向下一个

如果有盈余：将大小合适的给用户，剩余部分留下，当前位置仍空闲

极端情况
1 只有一个节点，，指向自己


*/
typedef struct node {
	int pos;
	int size;
	struct node* next;
}node;//定义链表节点


node* InputLoopNode(int*,node**);
void solve(node* head, node* tail, int n);

int main()
{
	int n;
	node* tail = NULL;
	node* head = InputLoopNode(&n, &tail);
	//此时n存储长度，head是头节点
	solve(head, tail, n);

	
	return 0;
}
node* InputLoopNode(int* n, node** tail1){
	//用于接受数据，同时生成循环链表，返返回链表头，以及长度n
	//返回链表尾部的指向，需要改变指针值，故用二级指针

	int m;
	scanf("%d", &m);
	*n = m;

	int nodePos, nodeSize;
	node* dummy = (node*)malloc(sizeof(node));
	dummy->next = NULL;
	node* tail = dummy;//尾部节点，用于指向下一个节点

	node* cur = NULL;
	for (int i = 0; i < m; ++i) {
		scanf("%d%d", &nodePos, &nodeSize);
		
		cur = (node*)malloc(sizeof(node));
		cur->pos = nodePos;
		cur->size = nodeSize;
		cur->next = NULL;

		tail->next = cur;//dummy充当虚拟头节点，可以不用判空
		tail = cur;
	}
	//差首位相接,此时cur指向最后一个节点
	*tail1 = cur;//*tail是真实的tail指向地址，直接更改为cur的值
	//即最后一个节点的值
	cur->next = dummy->next;
	return dummy->next;
}

void solve(node* head, node* tail,int n) {
	//传入head和n，即直接得到整个链表,tail为指向head的尾节点
	int space;

	node* cur = head;//设置游标节点
	node* front = tail;
	while (scanf("%d", &space)) {
		if (space == -1)break;
		node* minPos = NULL,*minPosFront=NULL;//最小size的地址存储，
		int flag = 1;//0标记已经通过方案一结束循环
		for (int i = 0; i < n; ++i) {
			if (cur->size == space) {
				front->next = cur->next;
				node* res = cur;
				cur = cur->next;
				free(res);
				flag = 0;
				n--;//链表长度实时更新
				break;//结束对于这个size的寻找，直接搜索下一个size
			}
			else if (cur->size > space 
				&&(minPos==NULL||cur->size<minPos->size) ) {
				//有盈余
				//!!!!注意此处要处理第一个的问题
				minPos = cur;
				minPosFront = front;
			}
			front = cur;
			cur = cur->next;
		}
		if (minPos&&flag) {
			//找到了min，没找到不管，因为循环n次后正好回到起点
			cur = minPos;
			front = minPosFront;
			cur->size -= space;//此处不用n--因为没有进行链表长度改变!!!
		}
	}
	if (!n) {
		return;
	}
	//此时所有请求遍历完毕，开始执行打印
	node* rec = cur;
	while (1) {
		printf("%d %d\n", cur->pos, cur->size);
		cur = cur->next;
		if (cur == rec)break;//如果说cur是rec才结束
	}

	//此时cur指向tail
	node* end = cur;
	cur = cur->next;
	end->next = NULL;
	//此时cur指向头部
	while (cur) {
		node* res = cur;
		cur = cur->next;
		free(res);
	}
}
//首先对于这个申请至多只用遍历n次即可，
//遍历过程中，1
// 1如果遇到相等空间，则利用front释放；
/*
2 遍历完成后，如果记录的minPos为NULL，说明没有，直接此时cur指向最后要给节点，
直接再次指向后继（回到起始位置）
如果发现了最小值：则利用记录的两个minfront和minPos进行移动，此时静安


*/