#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
container：存储总结果的字符数组，
buf：缓存输入的所有命令,只有开始给定已经发生的操作时用，为了风格统一也可以不用，直接利用scanf和space进行匹配
res:存储所有命令（内容是操作类型，位置，str） -> 链表（头节点为栈顶，维护一个栈的size）

core：
对于每一个操作的读取，只需要记录存储其逆操作即可，不需要对索引重新定位，
因为撤销栈严格先进后出：所以必须先进行后面的操作，
才能执行前面的操作，所以后面的操作完成后container自动还原到前面操作索引的对应状态

progress
1输入container
	读取n->用buf读取命令，识别命令，链表栈存储逆操作（）

	识别空行（fgets一行读取，最后的\n处理）
	进行操作读取进行操作
		1插入：在container找到索引后，先将后面部分strcat接到缓存区，重新设置末尾，
		然后将插入字符strcat上去，最后将缓存区的string再次strcat
		2删除：定位pos位置和n作用下有效的起始位置，缓存起来，然后重新设置末尾，再strcat
		3撤销：执行链表栈顶的操作（已经提前逆了），然后重新设置头节点，同时free
*/
typedef struct Node {
	int op;
	int pos;
	int len;//存储str的长度
	struct Node* next;
	char str[100];
}Node;

char container[515];//存储总结果
char buf[500];//读取命令，fscanf
char str[500];//读取str，为了给链表申请空间的时赋值
char res[500];//在字符插入，删除时用于缓存的数组

void alter(int op, int pos, int len, char* str);
Node* record(Node* head, int op, int pos, char* str);
int main()
{
	fgets(container, sizeof(container), stdin);
	container[(int)strcspn(container, "\n")] = '\0';//将container中换行符进行替换
	//注意：单引号表示字符部分场景下退化成指针，而双引号会默认为constchar*


	int n;
	scanf(" %d", &n);
	getchar();//吸收换行

	Node dummy;
	dummy.op = 0;//将dummy作为链表的末尾节点
	Node* top = &dummy;//指向链表的栈顶
	int loop = 0;
	while (loop<n) {
		//进行原始操作输入
		int inOp, inPos;
		fgets(buf, sizeof(buf), stdin);
		if (buf[0] == '\n') {//！！！！注意fgets中时常会有未吸收换行符的情况，此时需要特判buf，此时不宜用n--，可能会将这次作为有效循环
			continue;
		}
		buf[strcspn(buf, "\n")] = '\0';
		sscanf(buf," %d %d %s", &inOp, &inPos, str);//用str存储插入或者删除字符串
		
		//生成节点 存储的是逆操作，所以需要进行判断  并入栈
		top = record(top, inOp, inPos, str);
		loop++;
	}
	getchar();//吸收多余的一行
	//原始操作读取完毕：现在进行实际操作
	
	
	int inOp, inPos;
	while (scanf(" %d", &inOp)) {
		if (inOp == -1)break;
		if (inOp == 1) {
			//进行插入操作，先记录操作，再进行修改
			scanf(" %d %s", &inPos,str);//输入插入位置和插入字符串
			
			//用栈记录操作
			top = record(top, inOp, inPos, str);
			//对container进行修改：
			alter(inOp, inPos, 0, str);

		}
		else if (inOp == 2) {
			//进行删除操作，输入为位置和字符数量
			int len;
			scanf(" %d %d", &inPos, &len);

			//用栈记录操作,此处需要具体用str求出pos位置后len的具体字符串
			strncpy(str, container + inPos, len);//将container中需要删除的字符串提取放入str中，删除的逆反操作是insert需要记录str
			
			//！！！！strncpy需要手动加入结尾
			str[len] = '\0';
			

			top = record(top, inOp, inPos, str);//删除操作中尽管有位置超出问题，但是记录
			//反操作时我们只会记录str，所以不用管
			//对container进行修改
			alter(inOp, inPos, len, str);
		}
		else {
			//进行撤销操作：直接执行栈顶的操作,然后进行相关free操作
			alter(top->op, top->pos, top->len, top->str);
			if (top->op == 0) {
				continue;//！！！！遍历top到dummy了已经，不进行后续释放
			}
			Node* tmp = top;
			top = top->next;
			
			free(tmp);
		}
	}
	printf("%s", container);

	while (top->op) {
		//对可能剩余的操作进行free，因为我们用的是链表进行记录
		Node* tmp = top;
		top = top->next;
		free(tmp);
	}



	return 0;
}
Node* record(Node* head, int op, int pos, char* str) {
	//识别操作类型，并进行逆反操作的储存，最后返回栈顶节点
	Node* cur = (Node*)malloc(sizeof(Node));
	cur->op = op == 1 ? 2 : 1;
	cur->pos = pos;
	if (op == 1) {
		//原始操作是insert，这里就要存储删除,且不用管cur->str
		cur->len = strlen(str);
	}
	else {
		//原始是删除的话这里就要将strcopy进入，且不用管len，
		strcpy(cur->str, str);
		cur->len = 0;
	}
	cur->next = head;
	return cur;
}

void alter(int op, int pos,int len, char* str) {
	//container和res在全局不需要传参，
	/*
	1定位container中pos位置，将pos之后内容全部strcat缓存到res
	2原container重新定义末尾
	3将container与str strcat
	4再将新的container与res strcat
	*/
	res[0] = '\0';//！！！！对每次缓存区进行清空
	if (op == 0)return;//遍历到dummy，则不进行操作
	if (op == 1) {
		//！！！！进行插入操作，只能用str的固有长度，因为此时len是脏数据
		strcat(res, container + pos);
		container[pos] = '\0';
		strcat(container, str);
		strcat(container, res);
	}
	else {
		//进行删除操作：有可能删除时会将后面的脏数据带出
		// ！！！！需要特判一次是否删除的末位置超过了数组长度
		int lenContainer = strlen(container);
		if (pos+len>lenContainer) {
			//如果删除末位置大于containersize，
			container[pos] = '\0';
			return;
		}
		strcat(res, container + pos + len);
		container[pos] = '\0';
		strcat(container, res);
	}
}