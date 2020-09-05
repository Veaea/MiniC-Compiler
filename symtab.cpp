

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include"stdafx.h"
#include"globals.h"

/* SIZE�ǹ�ϣ��Ĵ�С*/
#define SIZE 211

/* SHIFT��hash�����������˷�����2����  */
#define SHIFT 4

/* ɢ�к��� */
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

/* ���ñ�����Դ�����к��б�*/
typedef struct LineListRec
{
	int lineno;//Դ�����к�
	struct LineListRec * next;//����ָ��
} *LineList;

/* BucketListRec�е�ÿ�������ļ�¼���������ơ�������ڴ�λ�ú�����Դ�����г��ֵ��к��б�*/
typedef struct BucketListRec
{
	char * name;//����
	LineList lines;//��Դ�����г��ֵ��кŽ��
	int memloc; //������ڴ�λ��
	struct BucketListRec * next;//����ָ��
} *BucketList;

/*��ϣ��*/
static BucketList hashTable[SIZE];

/*��������˵��:
���кź��ڴ�λ�ò�����ű�,loc=���ڵ�һ�β����ڴ�λ��,�������*/
/*����ӿ���Ϣ:name:��������ű�ķ����ַ���ָ��   lineno:�����ַ�����Դ�����е���    loc:���ڵ�һ�β����ڴ�λ��,�������  */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
void st_insert(char * name, int lineno, int loc)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) /* ���л�û�б�������ֵ���ơ��кš�������ڴ�λ�� */
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
	else /* �ڱ����ҵ�������ֻ������к� */
	{
		LineList t = l->lines;
		while (t->next != NULL) t = t->next;
		t->next = (LineList)malloc(sizeof(struct LineListRec));
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
} /* st_insert */

  /*��������˵��:st_lookup���ر������ڴ�λ�ã�����Ҳ����򷵻�-1*/
  /*����ӿ���Ϣ:name:���ӷ��ű��в��ҵķ����ַ���ָ��   */
  /*����ӿ���Ϣ:����int�͵��ڴ�λ��                       */
  /*����ע������:ע����������Ƿ���ȷ                            */
int st_lookup(char * name)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) return -1;
	else return l->memloc;
}

/*��������˵��:st_lookupLine���ر�����Դ�����кţ�����Ҳ����򷵻�-1*/
/*����ӿ���Ϣ:name:���ӷ��ű��в��ҵķ����ַ���ָ��   */
/*����ӿ���Ϣ:����int�͵�Դ�����к�                       */
/*����ע������:ע����������Ƿ���ȷ                            */
int st_lookupLine(char * name)
{
	int h = hash(name);
	BucketList l = hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL) return -1;
	else return l->lines->lineno;
}

/*��������˵��:printSymTab�����ű����ݵĸ�ʽ���б��ӡ�����ű��ļ�*/
/*����ӿ���Ϣ:void   */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
void printSymTab()
{
	int i;
	fprintf(list, "������      ��ַ       �к�\n");
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
