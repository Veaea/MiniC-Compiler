
#pragma once
#pragma warning(disable:4996)

#include "stdafx.h"
#include "globals.h"
#include "util.h"

/*��������˵��:printToken����Ǽ������ʹ�ӡ���﷨���ļ�*/
/*����ӿ���Ϣ:TokenTypeInProjece���͵�token��const char*���͵�tokenString����*/
/*����ӿ���Ϣ:void*/
/*����ע������:ע���������������*/
void printToken(TokenTypeInProjece token, const char* tokenString)
{
	switch (token)
	{
	case IF:
	case ELSE:
	case Int:
	case RETURN:
	case VoiD:
	case WHILE:
		fprintf(listing, "reserved word: %s\n", tokenString);
		break;
	case LT: fprintf(listing, "<\n"); break;
	case RT: fprintf(listing, ">\n"); break;
	case EQ: fprintf(listing, "==\n"); break;
	case LPAREN: fprintf(listing, "(\n"); break;
	case RPAREN: fprintf(listing, ")\n"); break;
	case SEMI: fprintf(listing, ";\n"); break;
	case PLUS: fprintf(listing, "+\n"); break;
	case MINUS: fprintf(listing, "-\n"); break;
	case TIMES: fprintf(listing, "*\n"); break;
	case OVER: fprintf(listing, "/\n"); break;
	case ENDFILE: fprintf(listing, "EOF\n"); break;
	case NEQ: fprintf(listing, "!=\n"); break;
	case ASSIGN:fprintf(listing, "=\n"); break;
	case DOT:fprintf(listing, ",\n"); break;
	case LBRACKET:fprintf(listing, "[\n"); break;
	case RBRACKET:fprintf(listing, "]\n"); break;
	case LBRACE:fprintf(listing, "{\n"); break;
	case RBRACE:fprintf(listing, "}\n"); break;
	case LNOTE:fprintf(listing, "/*\n"); break;
	case RNOTE:fprintf(listing, "*/\n"); break;
	case GE:fprintf(listing, ">=\n"); break;
	case LE:fprintf(listing, "<=\n"); break;
	case NUM:
		fprintf(listing, "NUM, val= %s\n", tokenString);
		break;
	case ID:
		fprintf(listing, "ID, name= %s\n", tokenString);
		break;
	case ERROR_INP:
		fprintf(listing, "ERROR: %s\n", tokenString);
		break;
	default: /*������ʾ��Ӧ�ò������*/
		fprintf(listing, "Unknown token: %d\n", token);
	}
}

/*��������˵��:����printResult���кš����͡��ַ�����ӡ���ʷ���������ļ�  */
/*����ӿ���Ϣ:lineno��ʾ�к�,token��ʾ����,tokenString��ʾ�ַ���  */
/*����ӿ���Ϣ:void  */
/*����ע������:ע���������������  */
void printResult(int lineno, TokenTypeInProjece token, const char* tokenString)
{
	switch (token)
	{
	case IF:
	case ELSE:
	case Int:
	case RETURN:
	case VoiD:
	case WHILE:
		fprintf(result, "�ؼ���: %s &%d\n", tokenString, lineno);
		break;
	case ID:
		fprintf(result, "��ʶ��: %s &%d\n", tokenString, lineno);
		break;
	case NUM:
		fprintf(result, "����: %s &%d\n", tokenString, lineno);
		break;
	case ASSIGN:
		fprintf(result, "��ֵ����:= &%d\n", lineno);
		break;
	case GE:
		fprintf(result, "���ڵ��ں�:>= &%d\n", lineno);
		break;
	case LE:
		fprintf(result, "С�ڵ��ں�:<= &%d\n", lineno);
		break;
	case EQ:
		fprintf(result, "�Ⱥ�:== &%d\n", lineno);
		break;
	case NEQ:
		fprintf(result, "���Ⱥ�:!= &%d\n", lineno);
		break;
	case LBRACKET:
		fprintf(result, "��������:[ &%d\n", lineno);
		break;
	case RBRACKET:
		fprintf(result, "��������:] &%d\n", lineno);
		break;
	case LBRACE:
		fprintf(result, "������:{ &%d\n", lineno);
		break;
	case RBRACE:
		fprintf(result, "�һ�����:} &%d\n", lineno);
		break;
	case LT:
		fprintf(result, "С�ں�:< &%d\n", lineno);
		break;
	case RT:
		fprintf(result, "���ں�:> &%d\n", lineno);
		break;
	case PLUS:
		fprintf(result, "�Ӻ�:+ &%d\n", lineno);
		break;
	case MINUS:
		fprintf(result, "����:- &%d\n", lineno);
		break;
	case TIMES:
		fprintf(result, "�˺�:* &%d\n", lineno);
		break;
	case OVER: fprintf(result, "����:/ &%d\n", lineno);
		break;
	case LPAREN:
		fprintf(result, "������:( &%d\n", lineno);
		break;
	case RPAREN:
		fprintf(result, "������:) &%d\n", lineno);
		break;
	case SEMI:
		fprintf(result, "�ֺ�:; &%d\n", lineno);
		break;
	case DOT:
		fprintf(result,"����: %s &%d\n", tokenString, lineno);
		break;
	case ENDFILE:
		break;
	case ERROR_INP:
		fprintf(result,"�������:%s &%d\n", tokenString, lineno);
		break;
	default:
		fprintf(result, "δ֪����: %d &%d\n", token, lineno);
	}
}

/*��������˵��:����newStmtNodeΪ�﷨������һ���µ����ڵ�  */
/*����ӿ���Ϣ:StmtKind���͵����ݣ���ʾ���ڵ���������  */
/*����ӿ���Ϣ:TreeNode���͵����ڵ��ַ  */
/*����ע������:ע������ͷ���ֵ����������  */
TreeNode * newStmtNode(StmtKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++)
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
	}
	return t;
}

/*��������˵��:����newExpNode���������﷨��������±��ʽ�ڵ�  */
/*����ӿ���Ϣ:ExpKind���͵����ݣ���ʾ���ʽ�ڵ������  */
/*����ӿ���Ϣ:TreeNode���͵ı��ʽ�ڵ��ַ  */
/*����ע������:ע������ͷ���ֵ����������  */
TreeNode * newExpNode(ExpKind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++)
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
		t->type = Void;
	}
	return t;
}

/*��������˵��:����copyString���䲢���������ַ������¸���   */
/*����ӿ���Ϣ:char *���͵����ݣ���ʾ��Ҫ�������tokenString  */
/*����ӿ���Ϣ:char *���͵����ݣ���ʾ�ʷ��������Ĵ�  */
/*����ע������:ע������ͷ���ֵ����������  */
char * copyString(char * s)
{
	int n;
	char * t;
	if (s == NULL) return NULL;
	n = strlen(s) + 1;
	t = (char *)malloc(n);
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else strcpy(t, s);
	return t;
}

/*printreeʹ�ñ���indentno�洢��ǰҪ�����Ŀո���*/
static int indentno = 0;

/* ����/���������������ո��� */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/*��������˵��:����printSpaces����ӡ�ռ�����   */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:void  */
/*����ע������: ��  */
static void printSpaces(void)
{
	int i;
	for (i = 0; i < indentno; i++)
		fprintf(listing, " ");
}

/*��������˵��:����printreeʹ���������﷨����ӡ���б��ļ���ָʾ����   */
/*����ӿ���Ϣ:TreeNode *���͵����ݣ���ʾ��Ҫ��ӡ���﷨���ڵ�  */
/*����ӿ���Ϣ:void  */
/*����ע������:ע���������������  */
void printTree(TreeNode * tree)
{
	int i;
	INDENT;
	while (tree != NULL)
	{
		printSpaces();
		if (tree->nodekind == StmtK)
		{
			switch (tree->kind.stmt)
			{
			case IfK:
				fprintf(listing, "If\n");
				break;
			case WhileK:
				fprintf(listing, "While\n");
				break;
			case ElseK:
				fprintf(listing, "Else\n");
				break;
			case ExpStmtK:
				fprintf(listing, "Expression\n");
				break;
			case AssignK:
				switch (tree->type)
				{
				case ArrayUnit:
					fprintf(listing, "Assign to: %s[]\n", tree->attr.name); break;
				default:
					fprintf(listing, "Assign to: %s\n", tree->attr.name); break;
				}
				break;
			case ReturnK:
				fprintf(listing, "return\n");
				break;
			case FunctionK:
				fprintf(listing, "Function Declaration: ");
				switch (tree->type)
				{
				case Integer:
					fprintf(listing, "Return Type: %s Name: %s\n", "[int]", tree->attr.name);
					break;
				case Void:
					fprintf(listing, "Return Type %s Name: %s \n", "[void]", tree->attr.name); break;
				default:break;
				}
				break;
			case ParamK:
				switch (tree->type)
				{
				case Integer:
					fprintf(listing, "Type: %s Name: %s \n", "[int]", tree->attr.name); break;
				case IntArray:
					fprintf(listing, "Name: %s Type: %s \n", tree->attr.name,"[int*]"); break;
				case Void:
					fprintf(listing, "EMPTY\n"); break;
				}
				break;
			case VarDeclarationK:
				fprintf(listing, "Var Declaration: ");
				switch (tree->type)
				{
				case Integer:
					fprintf(listing, "Name: %s Type: %s \n", tree->attr.name, "[int]"); break;
				case IntArray:
					fprintf(listing, "Name: %s Type: %s \n", tree->attr.name, "[int*]"); break;
				case Void:
					fprintf(listing, "Type %s :\n", "[void]"); break;
				}
				break;
			case LocalDeclarationK:
				fprintf(listing, "Local Declarations:\n");
				break;
			case StmtListK:
				fprintf(listing, "StatementList:\n");
				break;
			case ComK:
				fprintf(listing, "Compound:\n");
				break;
			case CallK:
				fprintf(listing, "Call Function:%s()\n", tree->attr.name);
				break;
			case ArgK:
				fprintf(listing, "Args:\n");
				break;
			case ParamListK:
				fprintf(listing, "ParamList:\n");
				break;
			case ArgListK:
				fprintf(listing, "ArgList:\n");
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else if (tree->nodekind == ExpK)
		{
			switch (tree->kind.exp)
			{
			case OpK:
				fprintf(listing, "Op: ");
				printToken(tree->attr.op, "\0");
				break;
			case ConstK:
				fprintf(listing, "const: %d\n", tree->attr.val);
				break;
			case IdK:
				switch (tree->type)
				{
				case ArrayUnit:
					fprintf(listing, "Id: %s[]\n", tree->attr.name); break;
				default:
					fprintf(listing, "Id: %s\n", tree->attr.name); break;
				}
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else
			fprintf(listing, "Unknown node kind\n");
		for (i = 0; i < MAXCHILDREN; i++)
		{
			if (tree->child[i] != NULL)
				printTree(tree->child[i]);
		}
		tree = tree->sibling;
	}
	UNINDENT;
}