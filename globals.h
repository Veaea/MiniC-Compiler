

#pragma once
#ifndef _GLOBALS_H_
#define _GLOBALS_H_


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*宏定义0为FALSE，方便使用和代码阅读*/
#ifndef FALSE
#define FALSE 0
#endif

/*宏定义1为TRUE，方便使用和代码阅读*/
#ifndef TRUE
#define TRUE 1
#endif

/*宏定义MAXRESERVED等于保留关键字个数*/
#define MAXRESERVED 6

typedef enum
/* 记录token类型 */
{
	ENDFILE, ERROR_INP,
	/* 保留关键字 */
	IF, ELSE, Int, RETURN, VoiD, WHILE,
	/* 其他标记 */
	ID, NUM,
	/* 专用符号，ASSIGN:'=',   GE:'>=',   LE:'<=',   EQ:'==',   NEQ:'!=',   LBRACKET:'[',   RBRACKET:']',   LBRACE:'{',   RBRACE:'}'             */
	/* LT:'<',   RT:'>',   PLUS:'+',   MINUS:'-',   TIMES:'*',   OVER:'/',   LPAREN:'(',   RPAREN:')'，   SEMI:';'，   DOT:',',   LNOTE:'左注释' */
	/*RNOTE:'右注释'                                                                                                                            */
	ASSIGN, GE, LE, EQ, NEQ, LBRACKET, RBRACKET, LBRACE, RBRACE, LT, RT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI, DOT, LNOTE, RNOTE
} TokenTypeInProjece;

extern FILE* source; /* 待分析代码文件 */
extern char sourceFile[120];
extern FILE* listing; /* 语法树保存文件 */
extern FILE* result; /*词法分析保存文件*/
extern FILE* code; /* code text file for TM simulator */
extern FILE* list;

extern int lineno; /*待分析代码的行号*/

/**************************************************/
/*********** 用于分析的语法树数据结构  ************/
/**************************************************/

/*stmtK:语句类型的标记,ExpK:表达式类型的标记*/
typedef enum { StmtK, ExpK } NodeKind;

/*IfK:if语法标记,   LocalDeclarationK:LocalDeclaration语法标记,   ReturnK:return语法标记,   ExpStmtK:ExpressionStatement语法标记,   AssignK:赋值语法标记,   ElseK:else语法标记,   WhileK:while语法标记   */
/*FunctionK:函数语法标记,   VarDeclarationK:VarDeclaration语法标记,   DeclarationK:Declaration语法标记,   StmtListK:StatementList语法标记,   ParamListK:ParamList语法标记,   CallK:Call语法标记        */
/*ArgListK:ArgList语法标记,   ParamK:Param语法标记,   ComK:Compound语法标记,   ArgK:Arg语法标记                                                                                                    */
typedef enum { IfK, LocalDeclarationK, ReturnK, ExpStmtK, AssignK, ElseK, WhileK, FunctionK, VarDeclarationK, DeclarationK, StmtListK, ParamListK, CallK, ArgListK, ParamK, ComK, ArgK } StmtKind;

/*ExpK表达式数据类型，Op:操作数,   Const:常数,   Id:标识符*/
typedef enum { OpK, ConstK, IdK } ExpKind;

/*ExpType用于类型检查,Void:void类型, Integer:int类型, Boolean:bool类型, IntArray:数组类型, ArrayUnit:数组单元*/
typedef enum { Void, Integer, Boolean, IntArray, ArrayUnit } ExpType;

/*MAXCHILDREN:语法树最大子孩子树*/
#define MAXCHILDREN 20

typedef struct treeNode
{
	struct treeNode * child[MAXCHILDREN];
	struct treeNode * sibling;
	int lineno;
	NodeKind nodekind;
	union { StmtKind stmt; ExpKind exp; } kind;
	union {
		TokenTypeInProjece op;
		int val;
		char * name;
	} attr;
	ExpType type; /* 用于表达式的类型检查 */
} TreeNode;

/**************************************************/
/***********       用于跟踪的标志      ************/
/**************************************************/

/* EchoSource=TRUE时，解析期间用行号将待分析代码回显到语法树输出文件*/
extern int EchoSource;

/* TraceScan=TRUE时，在扫描仪识别每个token时，将token信息打印到语法树输出文件*/
extern int TraceScan;

/* TraceParse=TRUE时，将语法树以线性形式打印到语法树输出文件（对子级使用缩进）*/
extern int TraceParse;

/* TraceAnalyze=TRUE时，将导致向语法树输出文件报告符号表插入和查找*/
extern int TraceAnalyze;

/* TraceCode=TRUE时，在生成代码时将注释写入TM代码文件*/
extern int TraceCode;

/* Error=TRUE时，在发生错误时阻止进一步传递 */
extern int Error;
#endif

