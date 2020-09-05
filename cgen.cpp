

#include "stdafx.h"
#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

/* tmpOffset��temps���ڴ�ƫ������ÿ�δ洢һ��tempʱ��������ݼ����ٴμ���ʱ�����*/
static int tmpOffset = 0;

/* �ڲ��ݹ����������ԭ�� */
static void cGen(TreeNode * tree);

/*��������˵��:genStmt�����ڵ����ɴ���*/
/*����ӿ���Ϣ:tree:��Ҫ�����м������﷨�����ڵ� */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
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
		/* Ϊ�ж������ʽ�����м���� */
		cGen(p1);
		savedLoc1 = emitSkip(1);
		emitComment("if: jump to else belongs here");
		/* ifΪTrueִ�������еݹ������м���� */
		cGen(p2);
		savedLoc2 = emitSkip(1);
		emitComment("if: jump to end belongs here");
		currentLoc = emitSkip(0);
		emitBackup(savedLoc1);
		emitRM_Abs("JEQ", ac, currentLoc, "if: jmp to else");
		emitRestore();
		/* ifΪFalseִ�������еݹ������м���� */
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
		//else����д���������䣬��ת����������������м���룬��ȻҲ���Դ���if-else��Ƕ��
		cGen(p1);
		if (TraceCode)  emitComment("<- else");
		break;
	case LocalDeclarationK://�������ھֲ�����������
		if (TraceCode) emitComment("-> local declaration");
		for (int i = 0; i < MAXCHILDREN; i++)
		{
			p1 = tree->child[i];
			if (p1 != NULL)//��ת��VarDeclarationK��ִ�еݹ����ɾֲ����������м����
				cGen(p1);
		}
		if (TraceCode)  emitComment("<- local declaration");
		break;
	case VarDeclarationK://��������
		if (TraceCode) emitComment("-> var declaration");
		/* ����ֵ */
		loc = st_lookup(tree->attr.name);//�ڷ��ű��в������������ĵ�ַ
		emitRM("ST", ac, loc, gp, "store value");//�����м����
		if (TraceCode)  emitComment("<- var declaration");
		break;
	case FunctionK://��������
		if (TraceCode) emitComment("-> function declaration");
		/* ����ֵ */
		loc = st_lookup(tree->attr.name);//�ڷ��ű��в������������ĵ�ַ
		emitRM("ST", ac, loc, gp, "store function"); //�����м����
		p1 = tree->child[0];//�����Ĳ����б�
		p2 = tree->child[1];//�����ĺ�����
		cGen(p1);//�ݹ���������м����
		cGen(p2);
		if (TraceCode)  emitComment("<- function declaration");
		break;
	case ReturnK://return���
		if (TraceCode) emitComment("-> return ");
		p1 = tree->child[0];//return���ķ��صı��ʽ��ֵ
		/* Ϊ���ʽ�����м���� */
		cGen(p1);
		/* ����ֵ */
		loc = st_lookup(p1->attr.name);//�ڷ��ű��в��ұ����ĵ�ַ
		emitRM("ST", ac, loc, gp, "return: store value"); //�����м����
		if (TraceCode)  emitComment("<- return");
		break;
	case AssignK://��ֵ���
		if (TraceCode) emitComment("-> assign");
		/* Ϊ���ʽ�����м���� */
		cGen(tree->child[0]);
		/* ����ֵ */
		loc = st_lookup(tree->attr.name);//�ڷ��ű��в��ұ����ĵ�ַ
		emitRM("ST", ac, loc, gp, "assign: store value");//�����м����
		if (TraceCode)  emitComment("<- assign");
		break;
	case WhileK://while���
		if (TraceCode) emitComment("-> while");
		p1 = tree->child[0];
		p2 = tree->child[1];
		/* Ϊ�ж������ʽ�����м���� */
		cGen(p1);
		savedLoc1 = emitSkip(1);
		emitComment("while: jump after body comes back here");
		/* whileΪTrueִ�������еݹ������м���� */
		cGen(p2);
		emitRM_Abs("JEQ", ac, savedLoc1, "repeat: jmp back to body");
		if (TraceCode)  emitComment("<- while");
		break;
	case StmtListK://�����е�ִ�����
		if (TraceCode) emitComment("-> statement list");
		for (int i = 0; i < MAXCHILDREN; i++)
		{
			p1 = tree->child[i];//�ݹ���������м����
			cGen(p1);
		}
		if (TraceCode)  emitComment("<- statement list");
		break;
	case ParamListK://���������Ĳ����б�
		if (TraceCode) emitComment("-> param list");
		p1 = tree->child[0];//������ΪNULL����Ϊvoid����Ϊһ��������int|IntArray����
		cGen(p1);
		for (int i = 1; i < MAXCHILDREN; i++)
		{
			p1 = tree->child[i];//�ݹ���������м����
			cGen(p1);
		}
		if (TraceCode)  emitComment("<- param list");
		break;
	case CallK://�����������
		if (TraceCode) emitComment("-> call");
		if (tree->attr.name == "input")//��Ϊinput��������
		{
			emitRO("IN", ac, 0, 0, "input integer value");//����Ĵ���
			loc = st_lookup(tree->attr.name);
			emitRM("ST", ac, loc, gp, "input: store value");
		}
		else if (tree->attr.name == "output")//��Ϊoutput��������
		{
			emitRO("OUT", ac, 0, 0, "output integer value");//�ӼĴ���ȡ��
			loc = st_lookup(tree->attr.name);
			emitRM("ST", ac, loc, gp, "output: store value");
		}
		else//��Ϊ������������
		{
			p1 = tree->child[0];//���ɺ������ò������м����
			cGen(p1);
			loc = st_lookup(tree->attr.name);//�ӷ��ű��в��ҵ��ú��������ڵ�ַ
			emitRM("ST", ac, loc, gp, "call: store value");//�����м����
		}
		if (TraceCode)  emitComment("<- call");
		break;
	case ArgListK://�������õĲ����б�
		if (TraceCode) emitComment("-> arglist");
		p1 = tree->child[0];
		cGen(p1);
		if (TraceCode)  emitComment("<- arglist");
		break;
	case ArgK://�������ò����б�ĵ�������
		if (TraceCode) emitComment("-> arg");
		p1 = tree->child[0];//�����м����
		cGen(p1);
		if (TraceCode)  emitComment("<- arg");
		break;
	case ParamK://�������������б�ĵ�������
		if (TraceCode) emitComment("-> param");
		if (tree->type == Void)//Ϊvoidʱ�������м����
			break;
		else//���������int����IntArray������ָ�����ͣ������м����
		{
			loc = st_lookup(tree->attr.name);
			emitRM("ST", ac, loc, gp, "param: store the value of a parameter list");
		}
		if (TraceCode)  emitComment("<- param");
		break;
	case ComK://�����﷨
		if (TraceCode) emitComment("-> compound");
		p1 = tree->child[0];//�ֲ���������
		p2 = tree->child[1];//ִ�����
		cGen(p1);//�����м����
		cGen(p2);
		if (TraceCode)  emitComment("<- compound");
		break;
	default:
		break;
	}
} /* genStmt */

/*��������˵��:genExp�ڱ��ʽ�ڵ������ɴ���*/
/*����ӿ���Ϣ:tree:��Ҫ�����м������﷨�����ڵ� */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
static void genExp(TreeNode * tree)
{
	int loc;
	TreeNode * p1, *p2;
	switch (tree->kind.exp) {

	case ConstK:
		if (TraceCode) emitComment("-> Const");
		/* ʹ��LDC�������ͳ��������м���� */
		emitRM("LDC", ac, tree->attr.val, 0, "load const");
		if (TraceCode)  emitComment("<- Const");
		break; /* ConstK */

	case IdK:
		if (TraceCode) emitComment("-> Id");
		if (tree->type == ArrayUnit)
		{
			//ʹ��LD�����������������м����
			cGen(tree->child[0]);//����������±�ݹ������м����
			loc = st_lookup(tree->child[0]->attr.name);//���������±�����Ƿ��ڷ��ű���
			emitRM("LD", ac, loc, gp, "load array index value");
			loc = st_lookup(tree->attr.name);//������������м���룬�����������Ƿ��ڷ��ű���
			emitRM("LD", ac, loc, gp, "load array unit value");
		}
		else//ʹ��LD�����������������м����
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
		/* ��������ĵݹ���� */
		cGen(p1);
		/* �洢����������м���� */
		emitRM("ST", ac, tmpOffset--, mp, "op: push left");
		/* �Ҳ������ĵݹ���� */
		cGen(p2);
		/* ����������� */
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

/*��������˵��:cGenͨ���������ݹ�������м����*/
/*����ӿ���Ϣ:tree:�������м������﷨���ڵ� */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
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
/*��������������Ҫ����*/
/**********************************************/
/*��������˵��:codeGenͨ�������﷨��������ļ����ɴ���*/
/*����ӿ���Ϣ:syntaxTree:���������﷨�����ڵ�   codefile:�м���뱣���ļ�·��  */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
void codeGen(TreeNode * syntaxTree, char * codefile)
{
	char * s = (char *)malloc(strlen(codefile) + 7);
	strcpy(s, "File: ");
	strcat(s, codefile);
	emitComment("TINY Compilation to TM Code");
	emitComment(s);
	/* ���ɱ�׼��ǰ�� */
	emitComment("Standard prelude:");
	emitRM("LD", mp, 0, ac, "load maxaddress from location 0");
	emitRM("ST", ac, 0, ac, "clear location 0");
	emitComment("End of standard prelude.");
	/* Ϊ�������ɴ��� */
	cGen(syntaxTree);
	/* finish */
	emitComment("End of execution.");
	emitRO("HALT", 0, 0, 0, "");
}
