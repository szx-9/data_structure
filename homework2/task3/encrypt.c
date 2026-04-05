#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*
1创建存储密钥的对应字母表key，构建一个hash表，记录26个英文字母是否存入
然后先输入单词串（输入一个单词则将其字母在hash中的位置进行标记，
直接进行输入如果当前输入字母存入则跳过，输入完成单词串后，进行各个字母的遍历

得到key后，
获取fin的长度，进行长度次循环，每一轮取出fin的元素，然后将这个元素的对应值
输入到fout中

*/
char hash[26];//判断字母是否输入过
char key[26];//字母对应表
char word[55];//输入的字母
int isChar(int);
int main()
{
	FILE* fin = fopen("encrypt.txt", "r");
	FILE* fout = fopen("output.txt", "w");
	fseek(fin, 0, SEEK_END);//移动fin指针到文件末尾
	ll filesize = ftell(fin);//file的文件长度
	fseek(fin, 0, SEEK_SET);

	scanf("%s", word);
	int indexWord = 0;//word的索引
	int lenWord = strlen(word);//word长度，用于判断是否已经对word输入完成
	int figure = 25;//25代表z,0代表a
	for (int i = 0; i < 26; ++i) {//对key进行输入
		if (indexWord < lenWord) {//仍然处于word输入阶段
			while (indexWord < lenWord &&
				hash[word[indexWord] - 'a'] == 1) {//如果当前字母出现过
				indexWord++;
			}
			if (indexWord >= lenWord) {
				--i;
				continue;
			}
			key[i] = word[indexWord];
			hash[word[indexWord] - 'a'] = 1;
			indexWord++;
		}
		else {
			if (figure < 0)break;

			while (hash[figure] == 1) {
				figure--;
			}
			if (figure < 0)break;
			key[i] = figure + 'a';
			hash[figure] = 1;
		}
	}

	int cnt = 0;
	while (cnt < filesize) {
		int target = fgetc(fin);
		if (isChar(target)) {
			fputc(key[target - 'a'], fout);
		}
		else {
			fputc(target, fout);
		}

		cnt++;
	}

	fclose(fin);
	fclose(fout);
	return 0;
}
int isChar(int a) {
	if (a >= 'a' && a <= 'z')return 1;
	return 0;
}