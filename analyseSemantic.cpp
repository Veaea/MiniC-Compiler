

#include"stdafx.h"
#include "globals.h"
#include "symtab.h"
#include "analyseSemantic.h"

/* �ɱ���ڴ�λ�ü����� */
static int location = 0;

 /*��������˵��:traverse��һ��ͨ�õĵݹ��﷨����������:
 ����preorder��ʹ��preProc����postorder��ʹ��postProc��tָ��������б���*/
 /*����ӿ���Ϣ:t:�﷨���ڵ�     (*preProc)��(*postProc):���������͵Ĳ���,�������﷨�����в�����ű��Լ����ͼ��ĺ������ݲ���*/
 /*����ӿ���Ϣ:����void                       */
 /*����ע������:ע����������Ƿ���ȷ                            */
static void traverse(TreeNode * t, void(*preProc) (TreeNode *), void(*postProc) (TreeNode *))
{
	if (t != NULL)
	{
		preProc(t);
		{ 
			int i;
			for (i = 0; i < MAXCHILDREN; i++)
				traverse(t->child[i], preProc, postProc);
		}
		postProc(t);
		traverse(t->sibling, preProc, postProc);
	}
}

 /*��������˵��:nullProc��һ�������κ�����Ĺ��̣����ڴӱ��������ɽ�ǰ��������ı���*/
 /*����ӿ���Ϣ:t:�﷨���ڵ�     (*preProc)��(*postProc):���������͵Ĳ���,�������﷨�����в�����ű��Լ����ͼ��ĺ������ݲ���*/
 /*����ӿ���Ϣ:����void                       */
 /*����ע������:ע����������Ƿ���ȷ                            */
static void nullProc(TreeNode * t)
{
	if (t == NULL) return;
	else return;
}

 /*��������˵��:insertNode���洢��t�еı�ʶ��������ű�*/
 /*����ӿ���Ϣ:t:��Ҫ��������ű���﷨���ڵ�*/
 /*����ӿ���Ϣ:����void                       */
 /*����ע������:ע����������Ƿ���ȷ                            */
static void insertNode(TreeNode * t)
{
	switch (t->nodekind)
	{
	case StmtK:
		switch (t->kind.stmt)
		{
		case FunctionK:
		case VarDeclarationK:
			if (st_lookup(t->attr.name) == -1)
				/*  ������δ�г�������Ϊ�¶��� */
				st_insert(t->attr.name, t->lineno, location++);
			else
				/* �Ѿ��ڱ��У����Ժ���λ�ã�ֻ���ʹ�õ��к� */
				st_insert(t->attr.name, t->lineno, 0);
			break;
		case CallK://input��output���Ѿ�Ĭ�������ĺ�������˲���FunctionK�в�������ڵ���ʱ�����
			if (strcmp(t->attr.name, "input") == 0 || strcmp(t->attr.name, "output") == 0)
			{
				if (st_lookup(t->attr.name) == -1)
					/*  ������δ�г�������Ϊ�¶��� */
					st_insert(t->attr.name, t->lineno, location++);
				else
					/* �Ѿ��ڱ��У����Ժ���λ�ã�ֻ���ʹ�õ��к� */
					st_insert(t->attr.name, t->lineno, 0);
			}
			break;
		default:
			break;
		}
		break;
	case ExpK:
		switch (t->kind.exp)
		{
		case IdK:
			if (st_lookup(t->attr.name) == -1)
				/* ������δ�г�������Ϊ�¶��� */
				st_insert(t->attr.name, t->lineno, location++);
			else
				/* �Ѿ��ڱ��У����Ժ���λ�ã�ֻ���ʹ�õ��к� */
				st_insert(t->attr.name, t->lineno, 0);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

/*��������˵��:buildSymtabͨ�����﷨����Ԥ���������������ű�*/
/*����ӿ���Ϣ:FILE:�������﷨�����ڵ�   list:���ű����ļ�·��  */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
void buildSymtab(TreeNode * syntaxTree, FILE *list)
{
	traverse(syntaxTree, insertNode, nullProc);
	if (TraceAnalyze)
	{
		fprintf(list, "\n���ű�:\n\n");
		printSymTab();
	}
}

/*��������˵��:typeError�����﷨���ڵ����ʹ���ʱ�������к��Լ�������Ϣ�ַ�����ӡ�����ű��ļ���*/
/*����ӿ���Ϣ:t:���ͼ��Ϊ������﷨���ڵ�   message:���ʹ���ʱ��ӡ���ļ��Ĵ�����Ϣ�ַ���ָ��  */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
static void typeError(TreeNode * t, char * message)
{
	fprintf(list, "Type error at line %d: %s\n", t->lineno, message);
	Error = TRUE;
}

/*��������˵��:checkNode�ڵ������ڵ���ִ�����ͼ��*/
/*����ӿ���Ϣ:t:�����ͼ����﷨���ڵ� */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
static void checkNode(TreeNode * t)
{
	switch (t->nodekind)
	{
	case ExpK:
		switch (t->kind.exp)
		{
		case OpK://���������ߵı��ʽ���������Ϊ���ͻ����������ͣ�����
			if ((t->child[0]->type != Integer && t->child[0]->type != ArrayUnit) ||
				(t->child[1]->type != Integer && t->child[1]->type != ArrayUnit))
				typeError(t, "Op applied to non-integer or non-array unit");
			switch (t->attr.op)
			{
				//+��-��*��/��=�ı��ʽ�����Ϊint����
			case ASSIGN:
			case PLUS:
			case MINUS:
			case TIMES:
			case OVER:
				t->type = Integer;
				break;
				//>��<��>=��<=��!=��==�ı��ʽ�����Ϊbool����
			case LE:
			case LT:
			case GE:
			case RT:
			case NEQ:
			case EQ:
				t->type = Boolean;
				break;
			default:
				break;
			}
			break;
		case ConstK://����Ϊint����
			t->type = Integer;
			break;
		case IdK://���������Ϳ�Ϊ�������ͻ�����
			if (t->type == ArrayUnit)
				break;
			else
				t->type = Integer;
			if(t->type != Integer && t->type != ArrayUnit)//���򱨴�
				typeError(t, "id applied to non-integer or non-array unit");
			break;
		default:
			break;
		}
		break;
	case StmtK:
		switch (t->kind.stmt)
		{
		case IfK://if�����жϱ��ʽ����һ��Ϊbool���ͣ����򱨴�
			if (t->child[0]->type != Boolean)
				typeError(t->child[0], "if test is not Boolean");
			break;
		case WhileK://while�����жϱ��ʽ����һ��Ϊbool���ͣ����򱨴�
			if (t->child[0]->type != Boolean)
				typeError(t->child[0], "while of non-integer value");
			break;
		case ReturnK://return���ı��ʽ����һ��Ϊint���ͣ����򱨴�
			if (t->child[0]->type != Integer)
				typeError(t->child[0], "return of non-integer value");
			break;
		case FunctionK://�����������Ĳ����б�
			if (t->child[0] == NULL)//���������б�Ϊ��
				break;
			else if (t->child[0]->child[0]->type == Void)//���������б�ΪVoid
				break;
			else//�����ʱ��������Ϊint��������ָ������
			{
				int i = 0;
				while (t->child[0]->child[i] != NULL)
				{
					if (t->child[0]->child[i]->type != Integer && t->child[0]->child[i]->type != IntArray)
						typeError(t->child[0]->child[i], "the parsing of a function argument list is not an integer or integer array");
					i++;
				}
			}
			break;
		case VarDeclarationK://����������������һ��Ϊint����
			t->type = Integer;
			break;
		case CallK://���������������Ϳ�Ϊvoid����int����
			lineno = st_lookupLine(t->attr.name);//���ҷ��ű��������������ڴ���ڼ���
			FILE *inFile;
			char lineBuf[256];
			char str[5];
			int bufsize;
			inFile = fopen(sourceFile, "r");
			for (int i = 0; i < lineno; ++i)//��ȡ�ļ��ض��ڼ��е�����
				fgets(lineBuf, 256 - 1, inFile);
			bufsize = strlen(lineBuf);
			for (int i = 0; i < bufsize; i++)//���ݺ�����������Ѱ�Һ���������
			{
				if (lineBuf[i] == 'i' && lineBuf[i + 1] == 'n' && lineBuf[i + 2] == 't')
				{
					str[0] = lineBuf[i];
					str[1] = lineBuf[i + 1];
					str[2] = lineBuf[i + 2];
					str[3] = '\0';
					break;
				}
				if (lineBuf[i] == 'v' && lineBuf[i + 1] == 'o' && lineBuf[i + 2] == 'i' && lineBuf[i + 3] == 'd')
				{
					str[0] = lineBuf[i];
					str[1] = lineBuf[i + 1];
					str[2] = lineBuf[i + 2];
					str[3] = lineBuf[i + 3];
					str[4] = '\0';
					break;
				}
			}
			fclose(inFile);
			if (strcmp(str, "int") == 0)//���͸�ֵ
				t->type = Integer;
			if (strcmp(str, "void") == 0)
				t->type = Void;
			break;
		default:
			break;
		}
		break;
	default:
		break;

	}
}

/*��������˵��:typeCheckͨ����������﷨��ִ�����ͼ��*/
/*����ӿ���Ϣ:syntaxTree:��Ҫ���ͼ����﷨�����ڵ� */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
void typeCheck(TreeNode * syntaxTree)
{
	traverse(syntaxTree, nullProc, checkNode);
}
