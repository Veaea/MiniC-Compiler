

#include"stdafx.h"
#include "globals.h"
#include "symtab.h"
#include "analyseSemantic.h"

/* 可变的内存位置计数器 */
static int location = 0;

 /*函数功能说明:traverse是一个通用的递归语法树遍历例程:
 它在preorder中使用preProc，在postorder中使用postProc对t指向的树进行遍历*/
 /*输入接口信息:t:语法树节点     (*preProc)、(*postProc):函数名类型的参数,后续对语法树进行插入符号表以及类型检查的函数传递参数*/
 /*输出接口信息:返回void                       */
 /*调用注意事项:注意参数类型是否正确                            */
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

 /*函数功能说明:nullProc是一个不做任何事情的过程，用于从遍历中生成仅前序或仅后序的遍历*/
 /*输入接口信息:t:语法树节点     (*preProc)、(*postProc):函数名类型的参数,后续对语法树进行插入符号表以及类型检查的函数传递参数*/
 /*输出接口信息:返回void                       */
 /*调用注意事项:注意参数类型是否正确                            */
static void nullProc(TreeNode * t)
{
	if (t == NULL) return;
	else return;
}

 /*函数功能说明:insertNode将存储在t中的标识符插入符号表*/
 /*输入接口信息:t:需要被插入符号表的语法树节点*/
 /*输出接口信息:返回void                       */
 /*调用注意事项:注意参数类型是否正确                            */
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
				/*  表中尚未列出，请视为新定义 */
				st_insert(t->attr.name, t->lineno, location++);
			else
				/* 已经在表中，所以忽略位置，只添加使用的行号 */
				st_insert(t->attr.name, t->lineno, 0);
			break;
		case CallK://input和output是已经默认声明的函数，因此不在FunctionK中插入表，而在调用时插入表
			if (strcmp(t->attr.name, "input") == 0 || strcmp(t->attr.name, "output") == 0)
			{
				if (st_lookup(t->attr.name) == -1)
					/*  表中尚未列出，请视为新定义 */
					st_insert(t->attr.name, t->lineno, location++);
				else
					/* 已经在表中，所以忽略位置，只添加使用的行号 */
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
				/* 表中尚未列出，请视为新定义 */
				st_insert(t->attr.name, t->lineno, location++);
			else
				/* 已经在表中，所以忽略位置，只添加使用的行号 */
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

/*函数功能说明:buildSymtab通过对语法树的预排序遍历来构造符号表*/
/*输入接口信息:FILE:待分析语法树根节点   list:符号表保存文件路径  */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
void buildSymtab(TreeNode * syntaxTree, FILE *list)
{
	traverse(syntaxTree, insertNode, nullProc);
	if (TraceAnalyze)
	{
		fprintf(list, "\n符号表:\n\n");
		printSymTab();
	}
}

/*函数功能说明:typeError检查的语法树节点类型错误时将出错行号以及错误信息字符串打印到符号表文件中*/
/*输入接口信息:t:类型检查为错误的语法树节点   message:类型错误时打印进文件的错误信息字符串指针  */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
static void typeError(TreeNode * t, char * message)
{
	fprintf(list, "Type error at line %d: %s\n", t->lineno, message);
	Error = TRUE;
}

/*函数功能说明:checkNode在单个树节点上执行类型检查*/
/*输入接口信息:t:待类型检查的语法树节点 */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
static void checkNode(TreeNode * t)
{
	switch (t->nodekind)
	{
	case ExpK:
		switch (t->kind.exp)
		{
		case OpK://操作数两边的表达式类型如果不为整型或者数组类型，报错
			if ((t->child[0]->type != Integer && t->child[0]->type != ArrayUnit) ||
				(t->child[1]->type != Integer && t->child[1]->type != ArrayUnit))
				typeError(t, "Op applied to non-integer or non-array unit");
			switch (t->attr.op)
			{
				//+、-、*、/、=的表达式结果都为int类型
			case ASSIGN:
			case PLUS:
			case MINUS:
			case TIMES:
			case OVER:
				t->type = Integer;
				break;
				//>、<、>=、<=、!=、==的表达式结果都为bool类型
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
		case ConstK://常数为int类型
			t->type = Integer;
			break;
		case IdK://变量名类型可为数组类型或整型
			if (t->type == ArrayUnit)
				break;
			else
				t->type = Integer;
			if(t->type != Integer && t->type != ArrayUnit)//否则报错
				typeError(t, "id applied to non-integer or non-array unit");
			break;
		default:
			break;
		}
		break;
	case StmtK:
		switch (t->kind.stmt)
		{
		case IfK://if语句的判断表达式类型一定为bool类型，否则报错
			if (t->child[0]->type != Boolean)
				typeError(t->child[0], "if test is not Boolean");
			break;
		case WhileK://while语句的判断表达式类型一定为bool类型，否则报错
			if (t->child[0]->type != Boolean)
				typeError(t->child[0], "while of non-integer value");
			break;
		case ReturnK://return语句的表达式类型一定为int类型，否则报错
			if (t->child[0]->type != Integer)
				typeError(t->child[0], "return of non-integer value");
			break;
		case FunctionK://函数声明语句的参数列表
			if (t->child[0] == NULL)//函数参数列表为空
				break;
			else if (t->child[0]->child[0]->type == Void)//函数参数列表为Void
				break;
			else//多参数时，参数可为int或者数组指针类型
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
		case VarDeclarationK://变量声明语句的类型一定为int类型
			t->type = Integer;
			break;
		case CallK://函数调用语句的类型可为void或者int类型
			lineno = st_lookupLine(t->attr.name);//查找符号表锁定函数调用在代码第几行
			FILE *inFile;
			char lineBuf[256];
			char str[5];
			int bufsize;
			inFile = fopen(sourceFile, "r");
			for (int i = 0; i < lineno; ++i)//获取文件特定第几行的内容
				fgets(lineBuf, 256 - 1, inFile);
			bufsize = strlen(lineBuf);
			for (int i = 0; i < bufsize; i++)//根据函数声明规则，寻找函数的类型
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
			if (strcmp(str, "int") == 0)//类型赋值
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

/*函数功能说明:typeCheck通过后序遍历语法树执行类型检查*/
/*输入接口信息:syntaxTree:需要类型检查的语法树根节点 */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
void typeCheck(TreeNode * syntaxTree)
{
	traverse(syntaxTree, nullProc, checkNode);
}
