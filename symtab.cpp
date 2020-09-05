

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include"stdafx.h"
#include"globals.h"

/* SIZE是哈希表的大小*/
#define SIZE 211

/* SHIFT是hash函数中用作乘法器的2的幂  */
#define SHIFT 4

/* 散列函数 */
static int hash(char * key)
{
	int temp = 0;
	int i = 0;
	while (key[i] != '\0')
	{
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

/* 引用变量的源代码行号列表*/
typedef struct LineListRec
{
	int lineno;//源代码行号
	struct LineListRec * next;//链表指针
} *LineList;

/* BucketListRec中的每个变量的记录，包括名称、分配的内存位置和它在源代码中出现的行号列表*/
typedef struct BucketListRec
{
	char * name;//名称
	LineList lines;//在源代码中出现的行号结点
	int memloc; //分配的内存位置
	struct BucketListRec * next;//链表指针
} *BucketList;

/*哈希表*/
static BucketList hashTable[SIZE];

/*函数功能说明:
将行号和内存位置插入符号表,loc=仅在第一次插入内存位置,否则忽略*/
/*输入接口信息:name:待插入符号表的符号字符串指针   lineno:符号字符串在源代码中的行    loc:仅在第一次插入内存位置,否则忽略  */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
void st_insert(char * name, int lineno, int loc)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) /* 表中还没有变量，赋值名称、行号、分配的内存位置 */
	{
		l = (BucketList)malloc(sizeof(struct BucketListRec));
		l->name = name;
		l->lines = (LineList)malloc(sizeof(struct LineListRec));
		l->lines->lineno = lineno;
		l->memloc = loc;
		l->lines->next = NULL;
		l->next = hashTable[h];
		hashTable[h] = l;
	}
	else /* 在表中找到，所以只需添加行号 */
	{
		LineList t = l->lines;
		while (t->next != NULL) t = t->next;
		t->next = (LineList)malloc(sizeof(struct LineListRec));
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
} /* st_insert */

  /*函数功能说明:st_lookup返回变量的内存位置，如果找不到则返回-1*/
  /*输入接口信息:name:待从符号表中查找的符号字符串指针   */
  /*输出接口信息:返回int型的内存位置                       */
  /*调用注意事项:注意参数类型是否正确                            */
int st_lookup(char * name)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) return -1;
	else return l->memloc;
}

/*函数功能说明:st_lookupLine返回变量的源代码行号，如果找不到则返回-1*/
/*输入接口信息:name:待从符号表中查找的符号字符串指针   */
/*输出接口信息:返回int型的源代码行号                       */
/*调用注意事项:注意参数类型是否正确                            */
int st_lookupLine(char * name)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) return -1;
	else return l->lines->lineno;
}

/*函数功能说明:printSymTab将符号表内容的格式化列表打印到符号表文件*/
/*输入接口信息:void   */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
void printSymTab()
{
	int i;
	fprintf(list, "变量名      地址       行号\n");
	fprintf(list, "---------      ------       ------\n");
	for (i = 0; i < SIZE; ++i)
	{
		if (hashTable[i] != NULL)
		{
			BucketList l = hashTable[i];
			while (l != NULL)
			{
				LineList t = l->lines;
				fprintf(list, "%-14s ", l->name);
				fprintf(list, "%-8d  ", l->memloc);
				while (t != NULL)
				{
					fprintf(list, "%4d ", t->lineno);
					t = t->next;
				}
				fprintf(list, "\n");
				l = l->next;
			}
		}
	}
} /* printSymTab */
