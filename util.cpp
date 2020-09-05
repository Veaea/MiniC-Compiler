
#pragma once
#pragma warning(disable:4996)

#include "stdafx.h"
#include "globals.h"
#include "util.h"

/*函数功能说明:printToken将标记及其类型打印到语法树文件*/
/*输入接口信息:TokenTypeInProjece类型的token，const char*类型的tokenString数组*/
/*输出接口信息:void*/
/*调用注意事项:注意参数的数据类型*/
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
	default: /*错误提示，应该不会出现*/
		fprintf(listing, "Unknown token: %d\n", token);
	}
}

/*函数功能说明:函数printResult将行号、类型、字符串打印到词法分析结果文件  */
/*输入接口信息:lineno表示行号,token表示类型,tokenString表示字符串  */
/*输出接口信息:void  */
/*调用注意事项:注意参数的数据类型  */
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
		fprintf(result, "关键字: %s &%d\n", tokenString, lineno);
		break;
	case ID:
		fprintf(result, "标识符: %s &%d\n", tokenString, lineno);
		break;
	case NUM:
		fprintf(result, "数字: %s &%d\n", tokenString, lineno);
		break;
	case ASSIGN:
		fprintf(result, "赋值符号:= &%d\n", lineno);
		break;
	case GE:
		fprintf(result, "大于等于号:>= &%d\n", lineno);
		break;
	case LE:
		fprintf(result, "小于等于号:<= &%d\n", lineno);
		break;
	case EQ:
		fprintf(result, "等号:== &%d\n", lineno);
		break;
	case NEQ:
		fprintf(result, "不等号:!= &%d\n", lineno);
		break;
	case LBRACKET:
		fprintf(result, "左中括号:[ &%d\n", lineno);
		break;
	case RBRACKET:
		fprintf(result, "右中括号:] &%d\n", lineno);
		break;
	case LBRACE:
		fprintf(result, "左花括号:{ &%d\n", lineno);
		break;
	case RBRACE:
		fprintf(result, "右花括号:} &%d\n", lineno);
		break;
	case LT:
		fprintf(result, "小于号:< &%d\n", lineno);
		break;
	case RT:
		fprintf(result, "大于号:> &%d\n", lineno);
		break;
	case PLUS:
		fprintf(result, "加号:+ &%d\n", lineno);
		break;
	case MINUS:
		fprintf(result, "减号:- &%d\n", lineno);
		break;
	case TIMES:
		fprintf(result, "乘号:* &%d\n", lineno);
		break;
	case OVER: fprintf(result, "除号:/ &%d\n", lineno);
		break;
	case LPAREN:
		fprintf(result, "左括号:( &%d\n", lineno);
		break;
	case RPAREN:
		fprintf(result, "右括号:) &%d\n", lineno);
		break;
	case SEMI:
		fprintf(result, "分号:; &%d\n", lineno);
		break;
	case DOT:
		fprintf(result,"逗号: %s &%d\n", tokenString, lineno);
		break;
	case ENDFILE:
		break;
	case ERROR_INP:
		fprintf(result,"错误符号:%s &%d\n", tokenString, lineno);
		break;
	default:
		fprintf(result, "未知符号: %d &%d\n", token, lineno);
	}
}

/*函数功能说明:函数newStmtNode为语法树创建一个新的语句节点  */
/*输入接口信息:StmtKind类型的数据，表示语句节点的语句类型  */
/*输出接口信息:TreeNode类型的语句节点地址  */
/*调用注意事项:注意参数和返回值的数据类型  */
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

/*函数功能说明:函数newExpNode创建用于语法树构造的新表达式节点  */
/*输入接口信息:ExpKind类型的数据，表示表达式节点的类型  */
/*输出接口信息:TreeNode类型的表达式节点地址  */
/*调用注意事项:注意参数和返回值的数据类型  */
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

/*函数功能说明:函数copyString分配并生成现有字符串的新副本   */
/*输入接口信息:char *类型的数据，表示需要被处理的tokenString  */
/*输出接口信息:char *类型的数据，表示词法分析出的词  */
/*调用注意事项:注意参数和返回值的数据类型  */
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

/*printree使用变量indentno存储当前要缩进的空格数*/
static int indentno = 0;

/* 增加/减少缩进的缩进空格数 */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/*函数功能说明:函数printSpaces按打印空间缩进   */
/*输入接口信息:void  */
/*输出接口信息:void  */
/*调用注意事项: 无  */
static void printSpaces(void)
{
	int i;
	for (i = 0; i < indentno; i++)
		fprintf(listing, " ");
}

/*函数功能说明:函数printree使用缩进将语法树打印到列表文件以指示子树   */
/*输入接口信息:TreeNode *类型的数据，表示需要打印的语法树节点  */
/*输出接口信息:void  */
/*调用注意事项:注意参数的数据类型  */
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