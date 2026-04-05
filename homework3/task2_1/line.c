#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

1有可能出现，中间有一项插进来，使得正好链接了两端


*/

typedef struct point{
	int x;
	int y;
}point;

typedef struct node{
	point left;//左节点
	point right;//右节点
	struct node* next;
	int num;//当前节点有多少线段
}node;

node* resNode[105];//用于存储可能出现的所有链表头节点
int isSame(point, point);
void delete(node*);
int main()
{
	int lenHeadNode = 0;//标记数组长度，即有多少链表头节点
	int MaxX=0, MaxY=0;
	int MaxNum = 0;
	int n;
	scanf("%d", &n);
	node* cur = NULL;
	while (n--) {
		cur = (node*)malloc(sizeof(node));
		cur->num = 1;
		cur->next = NULL;


		scanf("%d%d%d%d", &cur->left.x, &cur->left.y, 
			&cur->right.x, &cur->right.y);
		if (!lenHeadNode) {
			resNode[lenHeadNode++] = cur;
			if (1 > MaxNum) {
				MaxX = resNode[0]->left.x;
				MaxY = resNode[0]->left.y;
				MaxNum = 1;
			}
		}
		else {
			//对于数组每一个指针,进入内层循环，
			// 先判断第一个节点能不能接上
			// ,找到最后一个节点，判断是否
			//满足条件，若满足，则将后继指向，否则跳出，进行外层循环
			int i = 0;
			for (; i < lenHeadNode; ++i) {
				node* tra = resNode[i];//将当前的头节点取出作为头节点
				
				if (isSame(tra->left, cur->right)) {
					//如果当前可以接在头部
					cur->next = tra;
					cur->num += tra->num;
					if (cur->num > MaxNum) {
						MaxX = cur->left.x;
						MaxY = cur->left.y;

						MaxNum = cur->num;
					}
					resNode[i] = cur;
					break;
				}
				
				for (int j = 1; j < resNode[i]->num; ++j) {
					tra = tra->next;
				}
				//此时tra为最后一个节点
				if (isSame(tra->right, cur->left)) {
					tra->next = cur;
					(resNode[i]->num)++;//这里只有一个元素，所以可以自增
					if (resNode[i]->num > MaxNum) {
						MaxX = resNode[i]->left.x;
						MaxY = resNode[i]->left.y;

						MaxNum = resNode[i]->num;
					}

					break;
				}
			}
			if (i == lenHeadNode) {
				//遍历所有节点发现都进不去
				resNode[lenHeadNode++] = cur;
			}

		}

	}
	for (int i = 0; i < lenHeadNode-1; ++i) {
		node* ifirst = resNode[i], *ilast=resNode[i];
		for (int k = 1; k < resNode[i]->num; ++k){
			ilast = ilast->next;
		}

		
		for (int j = i + 1; j < lenHeadNode; ++j) {
			//对于每一个节点进行再搜索，匹配相应的可
			//取出四个节点
			node* jfirst = resNode[j], *jlast = resNode[j];
			for (int l = 1; l < resNode[j]->num; ++l) {
				jlast = jlast->next;
			}

			if (isSame(ifirst->left, jlast->right)) {
				jlast->next = ifirst;
				jfirst->num += ifirst->num;
				if (jfirst->num > MaxNum) {
					MaxNum = jfirst->num;
					MaxX = jfirst->left.x;
					MaxY = jfirst->left.y;
				}
			}
			else if (isSame(ilast->right, jfirst->left)) {
				ilast->next = jfirst;
				ifirst->num += jfirst->num;
				resNode[j] = ifirst;//因为我们将信息存储到了i中，但是i后续不会被
				//遍历，所以要将这个i存储到j的为位置
				if (ifirst->num > MaxNum) {
					//因为本轮循环中无法实时更改ij，所以只能用i这里
					MaxNum = ifirst->num;
					MaxX = ifirst->left.x;
					MaxY = ifirst->left.y;

				}
			}
			//为避免后续重复free，此处应直接free掉不用的i节点
			
		}
	}

	printf("%d ", MaxNum);
	printf("%d %d",MaxX,MaxY);

	return 0;
}
void delete(node* list) {
	//释放list头节点的内存
	node* cur = list;
	while (cur) {
		node* res = cur;
		cur = cur->next;
		free(res);
	}

}


int isSame(point a, point b) {
	if (a.x != b.x)return 0;
	if (a.y != b.y)return 0;
	return 1;
}