# sign introduction
---
## word grammar  
**bold**  
*lean*  
***same***  
==highlight==  
~~delete~~  

---
## paragraph  
***no ordered list***  
- list1 
- list2
	- list3
- list4
	- list5
- list6

***ordered list***  
1. list1
2. list2
1. list3
1. list4

> 引用
>> 二级引用

--- 
## code  
* a way to introduce a code is to use`` `using namespace std;`
```c
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ll long long 
/*

*/

int main()
{

	return 0;
}
```
--- 
# ***data structure note***
## lesson 1  
### 1.FILE knowledge  
1. function  
    1. fopen,fclose-需要指定打开模式：r只读，w值写（会默认覆盖），r+w+只是在各自基础上加入读写函数的使用权，
    输入绝对路径时可以通过../访问受上一级目录，然后再进行搜索
    1. 读写函数介绍
        
		```c
		int fgetc(FILE* );//读取一个字符，并使指针自增
		int fputc(int,FILE*);//写入一个字符
		char* fgets(char* str,int n,FILE* stream);
		//读入n-1个有效字符，自动读取换行符！！
		//停止条件：遇到换行符，达到n-1个字符，读到文件末尾EOF均是停止条件
		//fgets读取后文件指针会移动到最后一个成功读取的字符的下一个位置（file指向结构体，所以file不是真正的文件读取指针)

		int fputs(const char* str,FILE* stream);//写入字符串
		int fprintf(FILE* ,const char*);//格式化写入，输出到文件流，控制台用stdin
		int fscanf(FILE *stream, const char *format, ...);
        //扫描集：%[^sign]读取到扫描集合之前的所有符号，且将符号本身留在缓冲区，此时返回值是成功读取的扫描集符号
		//会直接操作指针指向缓冲区的位置指示器，即操作读取位置到下一个应该读取的字符

		size_t fread(void* ptr,size_t size,size_t cnt,FILE*);
		//读取cnt个块，每个块size个字节，以数据本身的二进制类型读取
		//ptr指针必须提前分配好空间，大小装下size*cnt字节，返回值（成功读取的数据块数量），
		//完全按照机器码进行读取，对于文本类通常不好用，就是严格按照sizeof的字节空间进行读取拷贝
		//通常只能读取格式完美或者fwrite写入内容，好处是非常快，不用对格式字符串进行转化

		```
	2. 位置操作函数
    	```c
		void rewind(FILE* stream);//跳回文件开头，注意这里是stream指向文件的内置指针（属于FILE结构体）
    	long ftell(FILE* stream);//返回偏移量
		int fseek(FILE* stream, long offset,int origin);//对指针进行对于偏移基准点的偏移
		//offset：有正负——SEEK_SET,SEEK_CUR,SEEK_END
    	```

2. 具体结构体
    ```c
	// 简化版 FILE 结构体（仅示意）
	typedef struct {
    char* buf;          // 指向底层缓冲区的起始地址
    int buf_size;       // 底层缓冲区的大小（如 4096 字节）
    int pos;            // 缓冲区的当前读取位置（偏移量）
    int remain;         // 缓冲区剩余未读取的字符数
    long file_offset;   // 物理文件的偏移（对应磁盘文件的位置）
    // 其他：文件状态、错误标记、读写模式等
	} FILE;
	```

### 2.struct knowledge
1. attention（注意事项）
    1. 结构体成员可以使用当前类型的指针，但不能定义自己（***申请字节的时候必须要知道==多少字节==***），申请指针的时候最好加上struct
    1. 内存对齐：
        1. 首先以所有成员中内存最大的对齐单元（数组以其类型）
        1. 其中按照书写顺序，填入成员时
            1. 检查==起始地址==是否偏移自己内存的整数倍
            1. 末尾填充自动补齐
    1. 可以放入函数指针void（*P）();（其中p为指针变量名，同时也是成员变量名，同时必须长得跟==函数声明==差不多），同时可以利用typedef对函数指针进行简化（起别名）
	
	```c
	typedef void (*update) (int);
	typedef struct {
    	int date;
		update update;//相当于给某个函数指针（这是一个变量类型，类比int指针），起了一个类型别名
	}Mtstruct;

	void add(int x){
    	return x+1;
	}

	Mystruct s;
	s.update=add;//给成员中的函数指针赋值，告诉这个指向指向哪个函数？
	s.update(5);//调用成员指针，或者函数
	```
### 3.enum(枚举类型）
1. def：一组有别名的，本质是int类型的数据类型`enum color{red,blue}; enum color co=red;`
1. 主要功能是匿名，定义某些特定数字和含义的==映射==，便于编码

### 4.string knowledge
1. func
    ```c
	char* strtok(char* str,const char* delim);
	//str表示被分割的字符串，delim是分割字符的集合eg " \n",后续调用的时候会记住当前分割的位子hi
	//传入NULL即可，从上次结束的地方进行分割
	//str必须是可修改的数组，而非字符串常量
	
	int atoi(const char* nptr);
	//跳过开头空白，识别正负号，连续读取，种植转换（非数字字符）
	```
### 5.指针pointer和地址
1. 指针 (注意事项） 
    1. 指针长度根据计算机硬件去决定，是固定值
    1. void* 没有步长设置，所以执行自增或者与指针类型有关的操作时会进行报错
    1. 数组名字和名字取地址值一样，数组名取地址执行自增时步长为整个数组，即二级指针
2. 地址  
    1. 地址：是一种表示编号的符号，常见可以通过二进制，十六进制进行地址构建，16进制更常见
    （节省长度，便于与二进制配合）
	2. 十六进制通常以32位为主，即地址占8byte，==1位十六进制数表示4位的二进制数==（即半个byte）
	3. 小端序：内存高低地址：从左到右从低到高，而数据高低位则是指的是一个数据的二进制位权重从小到大
	小段存储指的是，低地址存储小数据，所以数据顺序是反过来的，==先切分字节==

### 6. 

### .attention(注意事项，易错点）
1. 赋值语句（不带分号）的返回值是赋值后左侧变量的值，getchar同理
1. while循环中记得自增，特别容易忘
1. 需要先声明名称再在内部放入指针，内部指针必须加typedef,不能只有typedef
1. 及时return在函数中、
1. 注意题目中有多余的换行，可以先将样例复制到控制台进行查看
1. 大数相乘等模拟算法时，如果输出乱码可以考虑是否时数据类型导致的溢出

### 7