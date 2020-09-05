

#pragma once
#ifndef _GLOBALS_H_
#define _GLOBALS_H_


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*�궨��0ΪFALSE������ʹ�úʹ����Ķ�*/
#ifndef FALSE
#define FALSE 0
#endif

/*�궨��1ΪTRUE������ʹ�úʹ����Ķ�*/
#ifndef TRUE
#define TRUE 1
#endif

/*�궨��MAXRESERVED���ڱ����ؼ��ָ���*/
#define MAXRESERVED 6

typedef enum
/* ��¼token���� */
{
	ENDFILE, ERROR_INP,
	/* �����ؼ��� */
	IF, ELSE, Int, RETURN, VoiD, WHILE,
	/* ������� */
	ID, NUM,
	/* ר�÷��ţ�ASSIGN:'=',   GE:'>=',   LE:'<=',   EQ:'==',   NEQ:'!=',   LBRACKET:'[',   RBRACKET:']',   LBRACE:'{',   RBRACE:'}'             */
	/* LT:'<',   RT:'>',   PLUS:'+',   MINUS:'-',   TIMES:'*',   OVER:'/',   LPAREN:'(',   RPAREN:')'��   SEMI:';'��   DOT:',',   LNOTE:'��ע��' */
	/*RNOTE:'��ע��'                                                                                                                            */
	ASSIGN, GE, LE, EQ, NEQ, LBRACKET, RBRACKET, LBRACE, RBRACE, LT, RT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI, DOT, LNOTE, RNOTE
} TokenTypeInProjece;

extern FILE* source; /* �����������ļ� */
extern char sourceFile[120];
extern FILE* listing; /* �﷨�������ļ� */
extern FILE* result; /*�ʷ����������ļ�*/
extern FILE* code; /* code text file for TM simulator */
extern FILE* list;

extern int lineno; /*������������к�*/

/**************************************************/
/*********** ���ڷ������﷨�����ݽṹ  ************/
/**************************************************/

/*stmtK:������͵ı��,ExpK:���ʽ���͵ı��*/
typedef enum { StmtK, ExpK } NodeKind;

/*IfK:if�﷨���,   LocalDeclarationK:LocalDeclaration�﷨���,   ReturnK:return�﷨���,   ExpStmtK:ExpressionStatement�﷨���,   AssignK:��ֵ�﷨���,   ElseK:else�﷨���,   WhileK:while�﷨���   */
/*FunctionK:�����﷨���,   VarDeclarationK:VarDeclaration�﷨���,   DeclarationK:Declaration�﷨���,   StmtListK:StatementList�﷨���,   ParamListK:ParamList�﷨���,   CallK:Call�﷨���        */
/*ArgListK:ArgList�﷨���,   ParamK:Param�﷨���,   ComK:Compound�﷨���,   ArgK:Arg�﷨���                                                                                                    */
typedef enum { IfK, LocalDeclarationK, ReturnK, ExpStmtK, AssignK, ElseK, WhileK, FunctionK, VarDeclarationK, DeclarationK, StmtListK, ParamListK, CallK, ArgListK, ParamK, ComK, ArgK } StmtKind;

/*ExpK���ʽ�������ͣ�Op:������,   Const:����,   Id:��ʶ��*/
typedef enum { OpK, ConstK, IdK } ExpKind;

/*ExpType�������ͼ��,Void:void����, Integer:int����, Boolean:bool����, IntArray:��������, ArrayUnit:���鵥Ԫ*/
typedef enum { Void, Integer, Boolean, IntArray, ArrayUnit } ExpType;

/*MAXCHILDREN:�﷨������Ӻ�����*/
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
	ExpType type; /* ���ڱ��ʽ�����ͼ�� */
} TreeNode;

/**************************************************/
/***********       ���ڸ��ٵı�־      ************/
/**************************************************/

/* EchoSource=TRUEʱ�������ڼ����кŽ�������������Ե��﷨������ļ�*/
extern int EchoSource;

/* TraceScan=TRUEʱ����ɨ����ʶ��ÿ��tokenʱ����token��Ϣ��ӡ���﷨������ļ�*/
extern int TraceScan;

/* TraceParse=TRUEʱ�����﷨����������ʽ��ӡ���﷨������ļ������Ӽ�ʹ��������*/
extern int TraceParse;

/* TraceAnalyze=TRUEʱ�����������﷨������ļ�������ű����Ͳ���*/
extern int TraceAnalyze;

/* TraceCode=TRUEʱ�������ɴ���ʱ��ע��д��TM�����ļ�*/
extern int TraceCode;

/* Error=TRUEʱ���ڷ�������ʱ��ֹ��һ������ */
extern int Error;
#endif

