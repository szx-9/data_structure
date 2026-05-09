#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long
/*

*/

char buf[205]; // 缓存加密字符串：末尾有换行符
char res[200]; // 缓存输出倒叙字符串

void reverseRes(char *buf)
{
    int len = strlen(buf);
    int l = 0, r = len - 1;
    while (l <= r)
    {
        char tmp = buf[l];
        buf[l] = buf[r];
        buf[r] = tmp;
        l++;
        r--;
    }
    return;
}

int isZhi(char ch)
{
    if (isalpha(ch) || isdigit(ch))
        return 1;
    return 0;
}

int main()
{
    gets(buf);

    int len = strlen(buf);
    for (int i = 0; i < len; ++i)
    {
        if (isalpha(buf[i]))
        {
            // 如果当前是字母，继续遍历，检查后续是否是字母
            // 遍历检查到空字符为止
            int resIndex = 0;
            res[resIndex++] = buf[i];
            int j = i + 1;

            while (j < len &&isalpha(buf[j]))
            {
                res[resIndex++] = buf[j++];
            }
            res[resIndex] = '\0';

            reverseRes(res);
            printf("%s", res);

            i = j - 1;
        }
        else if (isdigit(buf[i]))
        {
            int resIndex = 0;
            res[resIndex++] = buf[i];
            int j = i + 1;

            while (j < len && isdigit(buf[j]))
            {
                res[resIndex++] = buf[j++];
            }
            res[resIndex] = '\0';

            reverseRes(res);
            printf("%s", res);
            i = j - 1;
        }
        else
        {
            // 如果不是字母或者数字则原样输出
            printf("%c", buf[i]);
        }
    }

    return 0;
}