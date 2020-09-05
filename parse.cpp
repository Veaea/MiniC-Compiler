
#include "stdafx.h"
#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenTypeInProjece token; /* 当前持有的词 */

								 /* 递归调用的函数原型 */
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

/*函数功能说明:函数syntaxError执行词法错误的报告，显示词法错误所在行和词*/
/*输入接口信息:char *类型的message，表示错误的词  */
/*输出接口信息:void                    */
/*调用注意事项:注意参数的数据类型                            */
static void syntaxError(char * message)
{
	fprintf(listing, "\n>>> ");
	fprintf(listing, "Syntax error at line %d: %s", lineno, message);
	Error = TRUE;
}

/*函数功能说明:函数match判断读取的词素是否为预计的词素，是则token获取下一词素；否则调用函数syntaxError报错*/
/*输入接口信息:TokenTypeInProjece类型的expected，表示预计读取的字符类型  */
/*输出接口信息:void                    */
/*调用注意事项:注意参数的数据类型                   */
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
/***********  进行语法分析，构建语法树 ************/
/**************************************************/


/*函数功能说明:函数declaration_list执行语法，declaration_list->declaration{@|declaration}  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
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
			else /* 现在p不能为空 */
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

/*函数功能说明:函数declaration执行语法，declaration->var_declaration | fun_declaration  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
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
		case ID://token==ID，则进入var_declaration->type_specifier ID; | type_specifier ID[NUM]; | type_specifier ID(params)
			t->attr.name = copyString(tokenString);
			t->kind.stmt = VarDeclarationK;
			match(ID);
			switch (token)
			{
			case LBRACKET://token==LBRACKET，则进入var_declaration->type_specifier ID[NUM];
				t->kind.stmt = VarDeclarationK;
				t->type = IntArray;
				match(LBRACKET);
				match(NUM);
				match(RBRACKET);
				match(SEMI);
				break;
			case LPAREN://token==LPAREN，则进入var_declaration->type_specifier ID(params) compound_stmt
				t->kind.stmt = FunctionK;
				match(LPAREN);
				t->child[0] = params();
				match(RPAREN);
				t->child[1] = compound_stmt();
				break;
			default://进入var_declaration->type_specifier ID;
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

/*函数功能说明:函数params执行语法，params->param_list | void  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * params(void)
{
	TreeNode * t = NULL;
	if (token == VoiD)//token==VoiD，进入params->void
	{
		match(token);
		t = newStmtNode(ParamListK);
		t->child[0] = newStmtNode(ParamK);
		t->child[0]->type = Void;
	}
	else if (token == RPAREN)//token==RPAREN，退回fun_declaration->type_specifier ID(params) compound_stmt
		t = NULL;
	else//进入params->param_list
	{
		t = param_list();
	}
	return t;
}

/*函数功能说明:函数param_list执行语法，param_list->param{,param}  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * param_list(void)
{
	TreeNode * t = newStmtNode(ParamListK);
	int i = 1;
	t->child[0] = param();
	while (token != RPAREN)//token!=RPAREN，则循环进入param_list->param{,param}  否则退回params->param_list | void
	{
		match(DOT);
		t->child[i] = param();
		i++;
	}
	return t;
}

/*函数功能说明:函数param执行语法，param->type_specifier ID | type_specifier ID[]  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * param(void)
{
	TreeNode * t = NULL;
	match(Int);//变量和数组的类型一定为Int，不能是VoiD
	t = newStmtNode(ParamK);
	t->type = Integer;
	t->attr.name = copyString(tokenString);
	match(ID);
	if (token == LBRACKET)//token == LBRACKET，进入param->type_specifier ID[]  否则进入进入param->type_specifier ID
	{
		t->type = IntArray;
		match(LBRACKET);
		match(RBRACKET);
	}
	return t;
}

/*函数功能说明:函数compound_stmt执行语法，compound_stmt->{local_declarations statement-list}  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * compound_stmt(void)
{
	TreeNode * t = newStmtNode(ComK);
	match(LBRACE);
	t->child[0] = local_declarations();
	t->child[1] = statement_list();

	match(RBRACE);
	return t;
}

/*函数功能说明:函数local_declarations执行语法， local_declarations-> { var_declaration }  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * local_declarations(void)
{
	TreeNode * t = newStmtNode(LocalDeclarationK);
	int i = 0;
	while (token == Int || token == VoiD)//token==Int或VoiD,进入local_declarations->  var_declaration 
	{

		t->child[i] = declaration();
		i++;
	}
	return t;
}

/*函数功能说明:函数statement_list执行语法，statement_list-> { statement }  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * statement_list(void)
{
	TreeNode * t = newStmtNode(StmtListK);
	int i = 0;
	while (token != RBRACE)//token!=RBRACE，进入statement_list->  statement  否则退回compound_stmt->{local_declarations statement-list}
	{
		t->child[i] = statement();
		i++;
	}
	return t;
}

/*函数功能说明:函数statement执行语法，statement-> expression_stmt | compound_stmt | selection_stmt | interation_stmt | return_stmt  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * statement(void)
{
	TreeNode * t = NULL;
	switch (token)//根据token值进入不同语句
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

/*函数功能说明:函数expression_stmt执行语法，expression_stmt-> expression; | ;  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * expression_stmt(void)
{
	TreeNode * t = newStmtNode(ExpStmtK);
	if (token == SEMI)//token==SEMI，进入，expression_stmt-> ;
		match(SEMI);
	else//否则进入，expression_stmt-> expression;
	{
		t = expression();
		match(SEMI);
	}
	return t;
}

/*函数功能说明:函数if_stmt执行语法，selection_stmt-> if (expression) statement {else statement}   */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
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
		if (token == ELSE)//token==ELSE，进入selection_stmt-> if (expression) statement else statement
		{
			match(ELSE);
			if (t != NULL)
				t->child[2] = newStmtNode(ElseK);
			t->child[2]->child[0] = statement();
		}
	}
	return t;
}

/*函数功能说明:函数while_stmt执行语法，iteration_stmt-> while (expression) statement  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
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

/*函数功能说明:函数return_stmt执行语法，return_stmt-> return {; | expression ;}  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * return_stmt(void)
{
	TreeNode * t = newStmtNode(ReturnK);
	if (token == RETURN)
		match(RETURN);
	if (token == SEMI)//token==SEMI，进入return_stmt-> return ;
		match(SEMI);
	else//否则进入return_stmt-> return expression ;
	{
		t->child[0] = expression();
		match(SEMI);
	}
	return t;
}

/*函数功能说明:函数expression执行语法，expression-> var = expression | simple_expression  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * expression(void)
{
	TreeNode * t = simple_exp();
	return t;
}

/*函数功能说明:函数var执行语法，var-> ID { @ | [expression] }  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode* var(void)
{
	TreeNode* t = newExpNode(IdK);
	if ((t != NULL) && (token == ID))//token!=NULL && token==ID，进入var-> ID { @ | [expression]
		t->attr.name = copyString(tokenString);
	match(ID);
	if (token == LBRACKET)//token==LBRACKET，进入var-> ID [expression]
	{
		match(token);
		t->type = ArrayUnit;
		t->child[0] = expression();
		match(RBRACKET);
	}
	return t;
}

/*函数功能说明:函数simple_exp执行语法，simple_exp-> additive_expression { relop additive_expresison }  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * simple_exp(void)
{
	TreeNode * t = additive_expression();
	if (t != NULL)
	{
		if (token == LT || token == LE || token == RT || token == GE || token == EQ || token == NEQ)
		{//进入simple_exp-> additive_expression  relop additive_expresison 
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

/*函数功能说明:函数additive_expression执行语法，additive_expression-> term { addop term }  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode* additive_expression(void)
{
	TreeNode * t = term();
	while (token == PLUS || token == MINUS)//进入additive_expression-> term  addop term 
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

/*函数功能说明:函数term执行语法，term-> factor { mulop factor }  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * term(void)
{
	TreeNode * t = factor();
	while ((token == TIMES) || (token == OVER))//进入term-> factor  mulop factor 
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

/*函数功能说明:函数factor执行语法， factor-> (expression) | var | call | NUM  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * factor(void)
{
	TreeNode * t = NULL;
	switch (token)
	{
	case NUM://进入factor-> NUM
		t = newExpNode(ConstK);
		if ((t != NULL) && (token == NUM))
			t->attr.val = atoi(tokenString);//把参数tokenString所指向的字符串转换为一个整数
		match(NUM);
		break;
	case ID://进入factor-> var 
		t = var();
		if (token == ASSIGN)//token==ASSIGN，进入expression-> var = expression
		{
			TreeNode* p = newStmtNode(AssignK);
			p->attr.name = t->attr.name;
			match(token);
			p->child[0] = expression();
			t = p;
		}
		if (token == LPAREN)//token==LPAREN，进入factor-> call     call-> ID(args)
		{
			TreeNode * p = newStmtNode(CallK);
			p->attr.name = t->attr.name;
			t = p;
			match(token);
			p->child[0] = args();
			match(RPAREN);
		}
		break;
	case LPAREN://token==LPAREN，进入factor-> (expression)
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

/*函数功能说明:函数args执行语法，args-> { arg_list }  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * args(void)
{
	TreeNode * t = newStmtNode(ArgListK);
	if (token != RPAREN)//token==RPAREN，进入args-> arg_list 
	{
		t->child[0] = arg_list();
		return t;
	}
	else
		return NULL;
}

/*函数功能说明:函数arg_list执行语法，arg_list-> expression { ,expression }  */
/*输入接口信息:void  */
/*输出接口信息:TreeNode*类型的语法树节点                   */
/*调用注意事项:注意返回值的数据类型                   */
TreeNode * arg_list(void)
{
	TreeNode * t = newStmtNode(ArgK);
	int i = 1;
	if (token != RPAREN)//token!=RPAREN，进入arg_list-> expression
		t->child[0] = expression();
	while (token != RPAREN)//token!=RPAREN，进入arg_list-> expression { ,expression }
	{
		match(DOT);
		t->child[i] = expression();
		i++;
	}
	return t;
}

/****************************************/
/*           解析器的主要功能           */
/****************************************/
/*函数功能说明:函数parse返回新的构造的整个语法树    */
/*输入接口信息:void                                 */
/*输出接口信息:TreeNode*类型的语法树节点            */
/*调用注意事项:注意返回值的数据类型                 */
TreeNode * parse(void)
{
	TreeNode * t;
	token = getToken();
	t = declaration_list();
	if (token != ENDFILE)
		syntaxError("Code ends before file\n");
	return t;
}
