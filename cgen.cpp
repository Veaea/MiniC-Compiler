

#include "stdafx.h"
#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

/* tmpOffset是temps的内存偏移量。每次存储一个temp时，它都会递减，再次加载时会递增*/
static int tmpOffset = 0;

/* 内部递归代码生成器原型 */
static void cGen(TreeNode * tree);

/*函数功能说明:genStmt在语句节点生成代码*/
/*输入接口信息:tree:需要生成中间代码的语法树根节点 */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
static void genStmt(TreeNode * tree)
{
	TreeNode * p1, *p2, *p3;
	int savedLoc1, savedLoc2, currentLoc;
	int loc;
	switch (tree->kind.stmt) {

	case IfK:
		if (TraceCode) emitComment("-> if");
		p1 = tree->child[0];
		p2 = tree->child[1];
		p3 = tree->child[2];
		/* 为判断语句表达式生成中间代码 */
		cGen(p1);
		savedLoc1 = emitSkip(1);
		emitComment("if: jump to else belongs here");
		/* if为True执行语句进行递归生成中间代码 */
		cGen(p2);
		savedLoc2 = emitSkip(1);
		emitComment("if: jump to end belongs here");
		currentLoc = emitSkip(0);
		emitBackup(savedLoc1);
		emitRM_Abs("JEQ", ac, currentLoc, "if: jmp to else");
		emitRestore();
		/* if为False执行语句进行递归生成中间代码 */
		cGen(p3);
		currentLoc = emitSkip(0);
		emitBackup(savedLoc2);
		emitRM_Abs("LDA", pc, currentLoc, "jmp to end");
		emitRestore();
		if (TraceCode)  emitComment("<- if");
		break;
	case ElseK:
		if (TraceCode) emitComment("-> else");
		p1 = tree->child[0];
		//else语句中处理其他语句，跳转到其他语句中生成中间代码，当然也可以处理if-else的嵌套
		cGen(p1);
		if (TraceCode)  emitComment("<- else");
		break;
	case LocalDeclarationK://函数体内局部变量的声明
		if (TraceCode) emitComment("-> local declaration");
		for (int i = 0; i < MAXCHILDREN; i++)
		{
			p1 = tree->child[i];
			if (p1 != NULL)//跳转到VarDeclarationK中执行递归生成局部变量声明中间代码
				cGen(p1);
		}
		if (TraceCode)  emitComment("<- local declaration");
		break;
	case VarDeclarationK://变量声明
		if (TraceCode) emitComment("-> var declaration");
		/* 保存值 */
		loc = st_lookup(tree->attr.name);//在符号表中查找声明变量的地址
		emitRM("ST", ac, loc, gp, "store value");//生成中间代码
		if (TraceCode)  emitComment("<- var declaration");
		break;
	case FunctionK://函数声明
		if (TraceCode) emitComment("-> function declaration");
		/* 保存值 */
		loc = st_lookup(tree->attr.name);//在符号表中查找声明函数的地址
		emitRM("ST", ac, loc, gp, "store function"); //生成中间代码
		p1 = tree->child[0];//函数的参数列表
		p2 = tree->child[1];//函数的函数体
		cGen(p1);//递归调用生成中间代码
		cGen(p2);
		if (TraceCode)  emitComment("<- function declaration");
		break;
	case ReturnK://return语句
		if (TraceCode) emitComment("-> return ");
		p1 = tree->child[0];//return语句的返回的表达式的值
		/* 为表达式生成中间代码 */
		cGen(p1);
		/* 保存值 */
		loc = st_lookup(p1->attr.name);//在符号表中查找变量的地址
		emitRM("ST", ac, loc, gp, "return: store value"); //生成中间代码
		if (TraceCode)  emitComment("<- return");
		break;
	case AssignK://赋值语句
		if (TraceCode) emitComment("-> assign");
		/* 为表达式生成中间代码 */
		cGen(tree->child[0]);
		/* 保存值 */
		loc = st_lookup(tree->attr.name);//在符号表中查找变量的地址
		emitRM("ST", ac, loc, gp, "assign: store value");//生成中间代码
		if (TraceCode)  emitComment("<- assign");
		break;
	case WhileK://while语句
		if (TraceCode) emitComment("-> while");
		p1 = tree->child[0];
		p2 = tree->child[1];
		/* 为判断语句表达式生成中间代码 */
		cGen(p1);
		savedLoc1 = emitSkip(1);
		emitComment("while: jump after body comes back here");
		/* while为True执行语句进行递归生成中间代码 */
		cGen(p2);
		emitRM_Abs("JEQ", ac, savedLoc1, "repeat: jmp back to body");
		if (TraceCode)  emitComment("<- while");
		break;
	case StmtListK://语句块中的执行语句
		if (TraceCode) emitComment("-> statement list");
		for (int i = 0; i < MAXCHILDREN; i++)
		{
			p1 = tree->child[i];//递归调用生成中间代码
			cGen(p1);
		}
		if (TraceCode)  emitComment("<- statement list");
		break;
	case ParamListK://函数声明的参数列表
		if (TraceCode) emitComment("-> param list");
		p1 = tree->child[0];//参数或为NULL，或为void，或为一个或多个的int|IntArray类型
		cGen(p1);
		for (int i = 1; i < MAXCHILDREN; i++)
		{
			p1 = tree->child[i];//递归调用生成中间代码
			cGen(p1);
		}
		if (TraceCode)  emitComment("<- param list");
		break;
	case CallK://函数调用语句
		if (TraceCode) emitComment("-> call");
		if (tree->attr.name == "input")//若为input函数调用
		{
			emitRO("IN", ac, 0, 0, "input integer value");//读入寄存器
			loc = st_lookup(tree->attr.name);
			emitRM("ST", ac, loc, gp, "input: store value");
		}
		else if (tree->attr.name == "output")//若为output函数调用
		{
			emitRO("OUT", ac, 0, 0, "output integer value");//从寄存器取出
			loc = st_lookup(tree->attr.name);
			emitRM("ST", ac, loc, gp, "output: store value");
		}
		else//若为其它函数调用
		{
			p1 = tree->child[0];//生成函数调用参数的中间代码
			cGen(p1);
			loc = st_lookup(tree->attr.name);//从符号表中查找调用函数名所在地址
			emitRM("ST", ac, loc, gp, "call: store value");//生成中间代码
		}
		if (TraceCode)  emitComment("<- call");
		break;
	case ArgListK://函数调用的参数列表
		if (TraceCode) emitComment("-> arglist");
		p1 = tree->child[0];
		cGen(p1);
		if (TraceCode)  emitComment("<- arglist");
		break;
	case ArgK://函数调用参数列表的单个参数
		if (TraceCode) emitComment("-> arg");
		p1 = tree->child[0];//生成中间代码
		cGen(p1);
		if (TraceCode)  emitComment("<- arg");
		break;
	case ParamK://函数声明参数列表的单个参数
		if (TraceCode) emitComment("-> param");
		if (tree->type == Void)//为void时不生成中间代码
			break;
		else//否则可能是int或者IntArray的数组指针类型，生成中间代码
		{
			loc = st_lookup(tree->attr.name);
			emitRM("ST", ac, loc, gp, "param: store the value of a parameter list");
		}
		if (TraceCode)  emitComment("<- param");
		break;
	case ComK://语句块语法
		if (TraceCode) emitComment("-> compound");
		p1 = tree->child[0];//局部变量声明
		p2 = tree->child[1];//执行语句
		cGen(p1);//生成中间代码
		cGen(p2);
		if (TraceCode)  emitComment("<- compound");
		break;
	default:
		break;
	}
} /* genStmt */

/*函数功能说明:genExp在表达式节点上生成代码*/
/*输入接口信息:tree:需要生成中间代码的语法树根节点 */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
static void genExp(TreeNode * tree)
{
	int loc;
	TreeNode * p1, *p2;
	switch (tree->kind.exp) {

	case ConstK:
		if (TraceCode) emitComment("-> Const");
		/* 使用LDC加载整型常量生成中间代码 */
		emitRM("LDC", ac, tree->attr.val, 0, "load const");
		if (TraceCode)  emitComment("<- Const");
		break; /* ConstK */

	case IdK:
		if (TraceCode) emitComment("-> Id");
		if (tree->type == ArrayUnit)
		{
			//使用LD加载数组类型生成中间代码
			cGen(tree->child[0]);//数组的索引下标递归生成中间代码
			loc = st_lookup(tree->child[0]->attr.name);//查找索引下标变量是否在符号表中
			emitRM("LD", ac, loc, gp, "load array index value");
			loc = st_lookup(tree->attr.name);//数组变量生成中间代码，查找数组名是否在符号表中
			emitRM("LD", ac, loc, gp, "load array unit value");
		}
		else//使用LD加载数组类型生成中间代码
		{
			loc = st_lookup(tree->attr.name);
			emitRM("LD", ac, loc, gp, "load id value");
		}
		if (TraceCode)  emitComment("<- Id");
		break; /* IdK */

	case OpK:
		if (TraceCode) emitComment("-> Op");
		p1 = tree->child[0];
		p2 = tree->child[1];
		/* 左操作数的递归调用 */
		cGen(p1);
		/* 存储左操作数的中间代码 */
		emitRM("ST", ac, tmpOffset--, mp, "op: push left");
		/* 右操作数的递归调用 */
		cGen(p2);
		/* 载入左操作数 */
		emitRM("LD", ac1, ++tmpOffset, mp, "op: load left");
		switch (tree->attr.op) {
		case PLUS:
			emitRO("ADD", ac, ac1, ac, "op +");
			break;
		case MINUS:
			emitRO("SUB", ac, ac1, ac, "op -");
			break;
		case TIMES:
			emitRO("MUL", ac, ac1, ac, "op *");
			break;
		case OVER:
			emitRO("DIV", ac, ac1, ac, "op /");
			break;
		case LT:
			emitRO("SUB", ac, ac1, ac, "op <");
			emitRM("JLT", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case LE:
			emitRO("SUB", ac, ac1, ac, "op <=");
			emitRM("JLE", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case RT:
			emitRO("SUB", ac, ac1, ac, "op >");
			emitRM("JGT", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case GE:
			emitRO("SUB", ac, ac1, ac, "op >=");
			emitRM("JGE", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case EQ:
			emitRO("SUB", ac, ac1, ac, "op ==");
			emitRM("JEQ", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		case NEQ:
			emitRO("SUB", ac, ac1, ac, "op !=");
			emitRM("JNE", ac, 2, pc, "br if true");
			emitRM("LDC", ac, 0, ac, "false case");
			emitRM("LDA", pc, 1, pc, "unconditional jmp");
			emitRM("LDC", ac, 1, ac, "true case");
			break;
		default:
			emitComment("BUG: Unknown operator");
			break;
		} /* case op */
		if (TraceCode)  emitComment("<- Op");
		break; /* OpK */

	default:
		break;
	}
} /* genExp */

/*函数功能说明:cGen通过树遍历递归地生成中间代码*/
/*输入接口信息:tree:待生成中间代码的语法树节点 */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
static void cGen(TreeNode * tree)
{
	if (tree != NULL)
	{
		switch (tree->nodekind) {
		case StmtK:
			genStmt(tree);
			break;
		case ExpK:
			genExp(tree);
			break;
		default:
			break;
		}
		cGen(tree->sibling);
	}
}

/**********************************************/
/*代码生成器的主要功能*/
/**********************************************/
/*函数功能说明:codeGen通过遍历语法树向代码文件生成代码*/
/*输入接口信息:syntaxTree:待操作的语法树根节点   codefile:中间代码保存文件路径  */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
void codeGen(TreeNode * syntaxTree, char * codefile)
{
	char * s = (char *)malloc(strlen(codefile) + 7);
	strcpy(s, "File: ");
	strcat(s, codefile);
	emitComment("TINY Compilation to TM Code");
	emitComment(s);
	/* 生成标准的前奏 */
	emitComment("Standard prelude:");
	emitRM("LD", mp, 0, ac, "load maxaddress from location 0");
	emitRM("ST", ac, 0, ac, "clear location 0");
	emitComment("End of standard prelude.");
	/* 为程序生成代码 */
	cGen(syntaxTree);
	/* finish */
	emitComment("End of execution.");
	emitRO("HALT", 0, 0, 0, "");
}
