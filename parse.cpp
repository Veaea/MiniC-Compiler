
#include "stdafx.h"
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenTypeInProjece token; /* ��ǰ���еĴ� */

								 /* �ݹ���õĺ���ԭ�� */
static TreeNode * declaration_list(void);
static TreeNode * declaration(void);
static TreeNode * params(void);
static TreeNode * param_list(void);
static TreeNode * param(void);
static TreeNode * compound_stmt(void);
static TreeNode * local_declarations(void);
static TreeNode * statement_list(void);
static TreeNode * statement(void);
static TreeNode * expression_stmt(void);
static TreeNode * if_stmt(void);
static TreeNode * while_stmt(void);
static TreeNode * return_stmt(void);
static TreeNode * expression(void);
static TreeNode * var(void);
static TreeNode * simple_exp(void);
static TreeNode * additive_expression(void);
static TreeNode * term(void);
static TreeNode * factor(void);
static TreeNode * args(void);
static TreeNode * arg_list(void);

/*��������˵��:����syntaxErrorִ�дʷ�����ı��棬��ʾ�ʷ����������кʹ�*/
/*����ӿ���Ϣ:char *���͵�message����ʾ����Ĵ�  */
/*����ӿ���Ϣ:void                    */
/*����ע������:ע���������������                            */
static void syntaxError(char * message)
{
	fprintf(listing, "\n>>> ");
	fprintf(listing, "Syntax error at line %d: %s", lineno, message);
	Error = TRUE;
}

/*��������˵��:����match�ж϶�ȡ�Ĵ����Ƿ�ΪԤ�ƵĴ��أ�����token��ȡ��һ���أ�������ú���syntaxError����*/
/*����ӿ���Ϣ:TokenTypeInProjece���͵�expected����ʾԤ�ƶ�ȡ���ַ�����  */
/*����ӿ���Ϣ:void                    */
/*����ע������:ע���������������                   */
static void match(TokenTypeInProjece expected)
{
	if (token == expected) token = getToken();
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		fprintf(listing, "      ");
	}
}

/**************************************************/
/***********  �����﷨�����������﷨�� ************/
/**************************************************/


/*��������˵��:����declaration_listִ���﷨��declaration_list->declaration{@|declaration}  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * declaration_list(void)
{
	TreeNode * t = declaration();
	TreeNode * p = t;
	while ((token == Int) || (token == VoiD))
	{
		TreeNode *q = declaration();
		if (q != NULL)
		{
			if (t == NULL)
				t = p = q;
			else /* ����p����Ϊ�� */
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

/*��������˵��:����declarationִ���﷨��declaration->var_declaration | fun_declaration  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * declaration(void)
{
	TreeNode * t = NULL;
	switch (token)
	{
	case VoiD:
	case Int:
		t = newStmtNode(DeclarationK);
		if (token == Int)
			t->type = Integer;
		else
			t->type = Void;
		match(token);
		switch (token)
		{
		case ID://token==ID�������var_declaration->type_specifier ID; | type_specifier ID[NUM]; | type_specifier ID(params)
			t->attr.name = copyString(tokenString);
			t->kind.stmt = VarDeclarationK;
			match(ID);
			switch (token)
			{
			case LBRACKET://token==LBRACKET�������var_declaration->type_specifier ID[NUM];
				t->kind.stmt = VarDeclarationK;
				t->type = IntArray;
				match(LBRACKET);
				match(NUM);
				match(RBRACKET);
				match(SEMI);
				break;
			case LPAREN://token==LPAREN�������var_declaration->type_specifier ID(params) compound_stmt
				t->kind.stmt = FunctionK;
				match(LPAREN);
				t->child[0] = params();
				match(RPAREN);
				t->child[1] = compound_stmt();
				break;
			default://����var_declaration->type_specifier ID;
				match(SEMI);
				break;
			}
			break;
		default:
			syntaxError("unexpected token -> ");
			printToken(token, tokenString);
			token = getToken();
			break;
		}
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

/*��������˵��:����paramsִ���﷨��params->param_list | void  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * params(void)
{
	TreeNode * t = NULL;
	if (token == VoiD)//token==VoiD������params->void
	{
		match(token);
		t = newStmtNode(ParamListK);
		t->child[0] = newStmtNode(ParamK);
		t->child[0]->type = Void;
	}
	else if (token == RPAREN)//token==RPAREN���˻�fun_declaration->type_specifier ID(params) compound_stmt
		t = NULL;
	else//����params->param_list
	{
		t = param_list();
	}
	return t;
}

/*��������˵��:����param_listִ���﷨��param_list->param{,param}  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * param_list(void)
{
	TreeNode * t = newStmtNode(ParamListK);
	int i = 1;
	t->child[0] = param();
	while (token != RPAREN)//token!=RPAREN����ѭ������param_list->param{,param}  �����˻�params->param_list | void
	{
		match(DOT);
		t->child[i] = param();
		i++;
	}
	return t;
}

/*��������˵��:����paramִ���﷨��param->type_specifier ID | type_specifier ID[]  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * param(void)
{
	TreeNode * t = NULL;
	match(Int);//���������������һ��ΪInt��������VoiD
	t = newStmtNode(ParamK);
	t->type = Integer;
	t->attr.name = copyString(tokenString);
	match(ID);
	if (token == LBRACKET)//token == LBRACKET������param->type_specifier ID[]  ����������param->type_specifier ID
	{
		t->type = IntArray;
		match(LBRACKET);
		match(RBRACKET);
	}
	return t;
}

/*��������˵��:����compound_stmtִ���﷨��compound_stmt->{local_declarations statement-list}  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * compound_stmt(void)
{
	TreeNode * t = newStmtNode(ComK);
	match(LBRACE);
	t->child[0] = local_declarations();
	t->child[1] = statement_list();

	match(RBRACE);
	return t;
}

/*��������˵��:����local_declarationsִ���﷨�� local_declarations-> { var_declaration }  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * local_declarations(void)
{
	TreeNode * t = newStmtNode(LocalDeclarationK);
	int i = 0;
	while (token == Int || token == VoiD)//token==Int��VoiD,����local_declarations->  var_declaration 
	{

		t->child[i] = declaration();
		i++;
	}
	return t;
}

/*��������˵��:����statement_listִ���﷨��statement_list-> { statement }  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * statement_list(void)
{
	TreeNode * t = newStmtNode(StmtListK);
	int i = 0;
	while (token != RBRACE)//token!=RBRACE������statement_list->  statement  �����˻�compound_stmt->{local_declarations statement-list}
	{
		t->child[i] = statement();
		i++;
	}
	return t;
}

/*��������˵��:����statementִ���﷨��statement-> expression_stmt | compound_stmt | selection_stmt | interation_stmt | return_stmt  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * statement(void)
{
	TreeNode * t = NULL;
	switch (token)//����tokenֵ���벻ͬ���
	{
	case IF:
		t = if_stmt(); break;
	case WHILE:
		t = while_stmt(); break;
	case ID:
	case SEMI:
		t = expression_stmt(); break;
	case RETURN:
		t = return_stmt(); break;
	case LBRACE:
		t = compound_stmt(); break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

/*��������˵��:����expression_stmtִ���﷨��expression_stmt-> expression; | ;  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * expression_stmt(void)
{
	TreeNode * t = newStmtNode(ExpStmtK);
	if (token == SEMI)//token==SEMI�����룬expression_stmt-> ;
		match(SEMI);
	else//������룬expression_stmt-> expression;
	{
		t = expression();
		match(SEMI);
	}
	return t;
}

/*��������˵��:����if_stmtִ���﷨��selection_stmt-> if (expression) statement {else statement}   */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * if_stmt(void)
{
	TreeNode * t = newStmtNode(IfK);
	if (t != NULL)
	{
		match(IF);
		match(LPAREN);
		t->child[0] = expression();
		match(RPAREN);
		t->child[1] = statement();
		if (token == ELSE)//token==ELSE������selection_stmt-> if (expression) statement else statement
		{
			match(ELSE);
			if (t != NULL)
				t->child[2] = newStmtNode(ElseK);
			t->child[2]->child[0] = statement();
		}
	}
	return t;
}

/*��������˵��:����while_stmtִ���﷨��iteration_stmt-> while (expression) statement  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * while_stmt(void)
{
	TreeNode * t = newStmtNode(WhileK);
	match(WHILE);
	match(LPAREN);
	if (t != NULL)
		t->child[0] = expression();
	match(RPAREN);
	if (t != NULL)
		t->child[1] = statement();
	return t;
}

/*��������˵��:����return_stmtִ���﷨��return_stmt-> return {; | expression ;}  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * return_stmt(void)
{
	TreeNode * t = newStmtNode(ReturnK);
	if (token == RETURN)
		match(RETURN);
	if (token == SEMI)//token==SEMI������return_stmt-> return ;
		match(SEMI);
	else//�������return_stmt-> return expression ;
	{
		t->child[0] = expression();
		match(SEMI);
	}
	return t;
}

/*��������˵��:����expressionִ���﷨��expression-> var = expression | simple_expression  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * expression(void)
{
	TreeNode * t = simple_exp();
	return t;
}

/*��������˵��:����varִ���﷨��var-> ID { @ | [expression] }  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode* var(void)
{
	TreeNode* t = newExpNode(IdK);
	if ((t != NULL) && (token == ID))//token!=NULL && token==ID������var-> ID { @ | [expression]
		t->attr.name = copyString(tokenString);
	match(ID);
	if (token == LBRACKET)//token==LBRACKET������var-> ID [expression]
	{
		match(token);
		t->type = ArrayUnit;
		t->child[0] = expression();
		match(RBRACKET);
	}
	return t;
}

/*��������˵��:����simple_expִ���﷨��simple_exp-> additive_expression { relop additive_expresison }  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * simple_exp(void)
{
	TreeNode * t = additive_expression();
	if (t != NULL)
	{
		if (token == LT || token == LE || token == RT || token == GE || token == EQ || token == NEQ)
		{//����simple_exp-> additive_expression  relop additive_expresison 
			TreeNode * p = newExpNode(OpK);
			if (p != NULL)
			{
				p->attr.op = token;
				p->child[0] = t;
				match(token);
				p->child[1] = additive_expression();
				t = p;
			}
		}
	}
	return t;
}

/*��������˵��:����additive_expressionִ���﷨��additive_expression-> term { addop term }  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode* additive_expression(void)
{
	TreeNode * t = term();
	while (token == PLUS || token == MINUS)//����additive_expression-> term  addop term 
	{
		TreeNode * p = newExpNode(OpK);
		p->attr.op = token;
		p->child[0] = t;
		match(token);
		p->child[1] = term();
		t = p;
	}
	return t;
}

/*��������˵��:����termִ���﷨��term-> factor { mulop factor }  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * term(void)
{
	TreeNode * t = factor();
	while ((token == TIMES) || (token == OVER))//����term-> factor  mulop factor 
	{
		TreeNode * p = newExpNode(OpK);
		if (p != NULL)
		{
			p->child[0] = t;
			p->attr.op = token;
			match(token);
			p->child[1] = factor();
			t = p;
		}
	}
	return t;
}

/*��������˵��:����factorִ���﷨�� factor-> (expression) | var | call | NUM  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * factor(void)
{
	TreeNode * t = NULL;
	switch (token)
	{
	case NUM://����factor-> NUM
		t = newExpNode(ConstK);
		if ((t != NULL) && (token == NUM))
			t->attr.val = atoi(tokenString);//�Ѳ���tokenString��ָ����ַ���ת��Ϊһ������
		match(NUM);
		break;
	case ID://����factor-> var 
		t = var();
		if (token == ASSIGN)//token==ASSIGN������expression-> var = expression
		{
			TreeNode* p = newStmtNode(AssignK);
			p->attr.name = t->attr.name;
			match(token);
			p->child[0] = expression();
			t = p;
		}
		if (token == LPAREN)//token==LPAREN������factor-> call     call-> ID(args)
		{
			TreeNode * p = newStmtNode(CallK);
			p->attr.name = t->attr.name;
			t = p;
			match(token);
			p->child[0] = args();
			match(RPAREN);
		}
		break;
	case LPAREN://token==LPAREN������factor-> (expression)
		match(LPAREN);
		t = expression();
		match(RPAREN);
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

/*��������˵��:����argsִ���﷨��args-> { arg_list }  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * args(void)
{
	TreeNode * t = newStmtNode(ArgListK);
	if (token != RPAREN)//token==RPAREN������args-> arg_list 
	{
		t->child[0] = arg_list();
		return t;
	}
	else
		return NULL;
}

/*��������˵��:����arg_listִ���﷨��arg_list-> expression { ,expression }  */
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�                   */
/*����ע������:ע�ⷵ��ֵ����������                   */
TreeNode * arg_list(void)
{
	TreeNode * t = newStmtNode(ArgK);
	int i = 1;
	if (token != RPAREN)//token!=RPAREN������arg_list-> expression
		t->child[0] = expression();
	while (token != RPAREN)//token!=RPAREN������arg_list-> expression { ,expression }
	{
		match(DOT);
		t->child[i] = expression();
		i++;
	}
	return t;
}

/****************************************/
/*           ����������Ҫ����           */
/****************************************/
/*��������˵��:����parse�����µĹ���������﷨��    */
/*����ӿ���Ϣ:void                                 */
/*����ӿ���Ϣ:TreeNode*���͵��﷨���ڵ�            */
/*����ע������:ע�ⷵ��ֵ����������                 */
TreeNode * parse(void)
{
	TreeNode * t;
	token = getToken();
	t = declaration_list();
	if (token != ENDFILE)
		syntaxError("Code ends before file\n");
	return t;
}
