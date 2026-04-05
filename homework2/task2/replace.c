#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
ftell 获取当前指针相对文件开头的字节偏移量（对于字符数组相当于偏移量—）

int fseek(FILE *stream, long offset, int whence);
将文件位置指针移动到指定位置（offset偏移量）

1新建一个fileout文件，用于最后直接赋值
2通过取file指针，读取长度 ，然后遍历fread，将filein存入res
3对res进行遍历，同时利用file指针对fileout进行遍历
	a:如果不是strOld的首字符，将这个字符fputc，然后continue，
	b:如果是，继续遍历（换参数，保证原始i的起始位置没变）
	如果发现完全匹配，则从i开始输入strNew，同时更新原始遍历位置为i+j，
	如果发现匹配失败，则从i的下一个继续遍历

*/
char strOld[50];
char strNew[50];
int isSame(char, char);
int main()
{
	FILE* fin = fopen("filein.txt", "r");
	FILE* fout = fopen("fileout.txt", "w");//如果没有的话系统新建，如果有则
	//从头清空写入
	fseek(fin, 0, SEEK_END);//移动fin指针到文件末尾
	ll filesize = ftell(fin);//file的文件长度
	fseek(fin, 0, SEEK_SET);

	//分配内存
	char* res = (char*)malloc(filesize + 1);//动态分配内存
	fread(res, 1, filesize, fin);//利用fread快速读入filein的自读

	//输入两个str
	scanf("%s", strOld);
	scanf("%s", strNew);
	int lenOld = strlen(strOld);
	int lenNew = strlen(strNew);
	FILE* fres = res;//fres用于遍历res数组
	for (int i = 0; i < filesize; ++i) {
		if (!isSame(res[i], strOld[0])) {//若当前不是调换字符，则之前存入fileout
			fputc(res[i], fout);//
			continue;
		}
		else {//继续对当前进行遍历
			int flag = 1;//匹配成功为1
			for (int k = 1; i + k < filesize && k < lenOld; ++k) {//遍历lenOLd-1遍
				if (!isSame(res[i + k], strOld[k])) {
					flag = 0;
					break;
				}
			}
			if (flag == 1) {//如果匹配成功，i应该跳到strOLd的结尾，
				//下一轮自增的时候自动完成
				i += lenOld - 1;
				for (int j = 0; j < lenNew; ++j) {//如果匹配成功
					fputc(strNew[j], fout);
				}
			}
			else {//如果匹配失败，则i还在原来位置，需要输入，然后继续即可
				fputc(res[i], fout);
				continue;
			}
		}
	}

	fclose(fin);
	fclose(fout);
	return 0;
}
int isSame(char a, char b) {
	if (a == b || abs(a - b) == abs('a' - 'A')) {
		return 1;
	}
	return 0;
}
