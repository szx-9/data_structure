#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
#define MaxAuthor 30
#define MaxYear 5
#define MaxPro 200
#define MaxTitle 200

/*



*/
typedef struct nameNode{
	char* name;//名字
	int num;
	struct nameNode* next;//指向下一个的指针，将所有名字串联
}nameNode;

typedef struct infoNode{
	nameNode* author;
	char* year;
	char* title;
	char* proceeding;
	struct infoNode* next;
	int seq;//插入顺序，不是最终序号，只是为了比较
	//最终序号由记录的总长度决定
}infoNode;


void insert(infoNode* dummy, int lenList, char* buf);
int isDiv(char*);
int isChar(char);
int order(infoNode*, infoNode*);
int cal(char*);
void Print(infoNode*);
void delete(infoNode*);
void deleteName(nameNode*);

char buf[505];
int main()
{
	int lenList = 0;//总节点个数
	infoNode* dummy=(infoNode*)malloc(sizeof(infoNode));
	//在全局进行内存分配，后续进行地址传递
	dummy->next = NULL;
	while (fgets(buf, sizeof(buf), stdin) ) {//可以读取EOF
		if (buf[0] == '\n')continue;//////////注意多余换行，坑死人
		insert(dummy, lenList, buf);
		lenList++;
	}
	Print(dummy);
	delete(dummy->next);
	free(dummy);
	return 0;
}
void deleteName(nameNode* dummy) {
	while (dummy) {
		nameNode* res = dummy;
		dummy = dummy->next;
		free(res->name);
		free(res);
	}
}


void delete(infoNode* dummy) {
	while (dummy) {
		infoNode* res = dummy;
		dummy = dummy->next;
		deleteName(res->author->next);
		free(res->author);
		free(res->year);
		free(res->title);
		free(res->proceeding);
		free(res);
	}
}


void Print(infoNode* dummy) {
	dummy = dummy->next;
	//第一个节点
	int i = 1;
	while (dummy) {
		printf("[%d]", i++);//序号和输入序无关
		
		int NumOfAuthor = dummy->author->num;
		nameNode* curs = dummy->author;
		curs = curs->next;
		int i = 0;
		while(i<NumOfAuthor) {//cursor不为零
			char* curName = curs->name;
			
			while (*curName) {
				if (*curName >= 'A' && *curName <= 'Z') {
					if (curName > curs->name && *(curName - 1) == '-') {
						goto a;//如果这个大写字母不是首字母且前一个是-，则不用输出space
					}
					printf(" ");
				}
			a:	printf("%c", *curName);
				curName++;
			}
			curs = curs->next;
			//判断
			if (i == NumOfAuthor - 1) {
				printf(".");
				break;
			}
			else if (i == NumOfAuthor - 2) {
				printf(" and");
			}
			else {
				printf(",");
			}
			++i;
		}

		printf(" ");
		//输出年份
		printf("%s.", dummy->year);
		
		printf(" ");
		printf("%s", dummy->title);

		printf(" ");
		printf("%s", dummy->proceeding);
		

		printf("\n");
		dummy = dummy->next;
	}

}


int cal(char* a) {
	int ans = 0;
	while (*a) {//最后一个是\0
		ans = ans * 10 + (*a) - '0';
		a++;
	}
	return ans;
}

int order(infoNode* a, infoNode* b) {
	//判断两个节点的先后顺序
	int numA = cal(a->year);
	int numB = cal(b->year);
	if (numA > numB) {
		return -1;
	}
	else if (numA < numB) {
		return 1;
	}
	else {
		int ans = strcmp(a->author->next->name, b->author->next->name);
		if (ans)return ans;
		else {
			return (a->seq) - (b->seq);
		}
	}
}

int isChar(char c) {
	//判断是否是字母？
	if (c >= 'a' && c <= 'z')return 1;
	if (c >= 'A' && c <= 'Z')return 1;
	return 0;
}
int isDiv(char* cur) {
	//传入指针指向位置，判断是否应该进行下一个名字读取
	//1表示， 2表示and,3表示。
	//没有遇到分隔符return0
	if (*cur == ',')return  1;
	if (*cur == '.')return 3;
	if (!isChar(*cur))return 0;
	if ( (*cur) != 'a')return 0;
	cur++;
	if ( (*cur) != 'n')return 0;
	cur++;
	if ( (*cur) != 'd')return 0;
	cur++;
	if( (*cur) != ' ')return 0;///////
	return 2;
}


void insert(infoNode* dummy, int lenList, char* buf) {//len在外侧自增
	//传入某一个节点所有信息，并进行节点生成，并在dummy中找到适合位置进行插入
	
	//节点内存申请
	infoNode* cur = (infoNode*)malloc(sizeof(infoNode));
	cur->author = (nameNode*)malloc(sizeof(nameNode));
	cur->author->num = 0;
	int* numAuthor = &cur->author->num;
	cur->author->next = NULL;
	//此处为namedummy，

	cur->year = (char*)malloc(sizeof(char)*MaxYear);
	cur->title = (char*)malloc(sizeof(char)*MaxTitle);
	cur->proceeding = (char*)malloc(sizeof(char) * MaxPro);
	cur->next=NULL;
	cur->seq = lenList;//序号从零开始，当前长度应该就是当前序号

	//输入信息
	char* trans = NULL;//用于转移各个指针具体位置
	
	{//处理作者姓名，直到遇到数字停止
		nameNode* now = cur->author;//当前指向的名字节点

		char* move = buf;
		//每一个作者之间是由, and进行区分的
		
		while (1) {//后续根据ans代表的分隔符情况及进行break
			int ans;
			int nameIndex = 0;
			
			nameNode* curAuthor =(nameNode*) malloc(sizeof(nameNode));
			curAuthor->name = (char*)malloc(sizeof(char) * MaxAuthor);
			curAuthor->next = NULL;

			while ( !( ans=isDiv(move) ) ) {//遍历到分隔符,赋值语句特性
			//如果没有遇到分隔符return0，则进入循环
				if (isChar(*move)||*move=='-') {//如果move是字母，进一步操作
					if (move == buf || *(move - 1) == ' ') {//首字母
						if (*move >= 'a' && *move <= 'z')
							*move = *move - 'a' + 'A';
					}
					curAuthor->name[nameIndex++] = *move;
				}
				move++;
			}
			//当前作者名字已经遍历完毕
			(*numAuthor)++;//对作者总数自增
			curAuthor->name[nameIndex] = '\0';
			now->next = curAuthor;
			now = curAuthor;
			
			if (ans == 1)move++;
			else if (ans == 2)move += 3;
			else {
				move++;
				break;
			}
		}
		trans = move;
	}
	
	{//处理年份
		int lenYear = 0;
		char* move = trans;
		while (*move != '.') {
			if (*move >= '0' && *move <= '9') {
				cur->year[lenYear++] = *move;
			}
			move++;
		}
		cur->year[lenYear] = '\0';
		move++;
		trans = move;
	}
	
	{//处理title,标题首字母大写
		int lenTitle = 0;
		char* move = trans;
		while (1) {
			if (isChar(*move)) {
				break;
			}
			move++;
		}
		//此时move指向第一个大写字母，之后只需要直接while
		while (1) {
			cur->title[lenTitle++] = *move;
			if (*move == '.')break;
			move++;
		}
		cur->title[lenTitle] = '\0';
		trans = move;
	}

	{//处理pro
		int lenPro = 0;
		char* move = trans;
		while (1) {
			if (isChar(*move)) {
				break;
			}
			move++;
		}
		while (1) {
			cur->proceeding[lenPro++] = *move;
			if (*move == '.')break;
			move++;
		}
		cur->proceeding[lenPro] = '\0';
	}

	//开始插入节点
	if (lenList==0) {
		dummy->next = cur;
		return;//及时return
	}
	infoNode* front=dummy;
	dummy = dummy->next;
	while (lenList--) {//事实证明lenlist和dummy判空都可以，应该是andler的bug导致之前的读取错误？
		if (order(dummy, cur)>0 ) {
			front->next = cur;
			cur->next = dummy;
			return;
		}
		front = dummy;
		dummy = dummy->next;
	}
	//直到最后一个，说明应该插在队尾
	front->next = cur;
	
}
