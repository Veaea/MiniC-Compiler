
#include "stdafx.h"
#include "globals.h"
#include "util.h"
#include "scan.h"
#include"MiniCDlg.h"
/*扫描仪DFA中的状态*/
typedef enum
{
	START, INEQUAL, INNEQ, INCOMMENT, INNUM, INID, DONE, INLT, INRT
}
StateType;

/* 标识符或保留字的词素 */
char tokenString[MAXTOKENLEN + 1];

/* BUFLEN=源代码行的输入缓冲区长度 */
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* 保留的当前源代码行内容 */
int linepos = 0; /* 当前在LineBuf中的位置 */
int bufsize = 0; /* 缓冲区字符串的当前大小 */
int EOF_flag = FALSE; /* 是否修正EOF上的ungetNextChar行为标记 */

/*函数功能说明:函数getNextChar从lineBuf获取下一个非空字符，如果lineBuf耗尽，则读取新行    */
/*输入接口信息:void                                 */
/*输出接口信息:获取的字符强制转化为int类型的ASCII码            */
/*调用注意事项:注意返回值的数据类型                 */
static int getNextChar(void)
{
	if (!(linepos < bufsize))
	{
		lineno++;
		if (fgets(lineBuf, BUFLEN - 1, source))
		{
			if (EchoSource) fprintf(listing, "%4d: %s", lineno, lineBuf);
			bufsize = strlen(lineBuf);
			linepos = 0;
			return lineBuf[linepos++];
		}
		else
		{
			EOF_flag = TRUE;
			return EOF;
		}
	}
	else return lineBuf[linepos++];
}

/*函数功能说明:函数ungetNextChar在lineBuf中回溯一个字符    */
/*输入接口信息:void                                 */
/*输出接口信息:void            */
/*调用注意事项:注意全局变量linepos的变化                 */
static void ungetNextChar(void)
{
	if (!EOF_flag) linepos--;
}

/* 保留字查找表 */
static struct
{
	char* str;
	TokenTypeInProjece tok;
} reservedWords[MAXRESERVED] = { { "if",IF },{ "else",ELSE },{ "int",Int },
{ "void",VoiD },{ "return",RETURN },{ "while",WHILE } };

/*函数功能说明:函数reservedLookup使用线性搜索查找标识符以查看它是否是保留字    */
/*输入接口信息:char *类型的字符串,表示需要搜索的标识符                         */
/*输出接口信息:TokenTypeInProjece类型的枚举值            */
/*调用注意事项:注意参数和返回值的类型                 */
static TokenTypeInProjece reservedLookup(char * s)
{
	int i;
	for (i = 0; i < MAXRESERVED; i++)
		if (!strcmp(s, reservedWords[i].str))
			return reservedWords[i].tok;
	return ID;
}

/****************************************/
/*           扫描仪的主要功能           */
/****************************************/

/*函数功能说明:函数getToken返回源文件中的下一个标记    */
/*输入接口信息:void                                    */
/*输出接口信息:TokenTypeInProjece类型的枚举值            */
/*调用注意事项:注意返回值的类型                   */
TokenTypeInProjece getToken(void)
{  
	/* 用于存储到tokenString中的索引 */
	int tokenStringIndex = 0;
	/* 保留要返回的当前token */
	TokenTypeInProjece currentToken;
	/* 当前DFA状态,始终从开始处开始 */
	StateType state = START;
	/* 判断是否保存tokenString */
	int save;
	while (state != DONE)
	{
		int c = getNextChar();
		save = TRUE;
		switch (state)
		{
		case START:
			if (isdigit(c))
				state = INNUM;
			else if (isalpha(c))
				state = INID;
			else if (c == '=')
				state = INEQUAL;
			else if (c == '<')
				state = INLT;
			else if (c == '>')
				state = INRT;
			else if ((c == ' ') || (c == '\t') || (c == '\n'))
				save = FALSE;
			else if (c == '!')
				state = INNEQ;
			else if (c == '/')
			{
				if (getNextChar() != '*')
				{
					ungetNextChar();
					state = DONE;
					currentToken = OVER;
					break;
				}
				else
				{
					save = FALSE;
					state = INCOMMENT;
				}
			}
			else
			{
				state = DONE;
				switch (c)
				{
				case EOF:
					save = FALSE;
					currentToken = ENDFILE;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case '-':
					currentToken = MINUS;
					break;
				case '*':
					currentToken = TIMES;
					break;
				case '(':
					currentToken = LPAREN;
					break;
				case ')':
					currentToken = RPAREN;
					break;
				case ';':
					currentToken = SEMI;
					break;
				case '[':
					currentToken = LBRACKET;
					break;
				case ']':
					currentToken = RBRACKET;
					break;
				case '{':
					currentToken = LBRACE;
					break;
				case '}':
					currentToken = RBRACE;
					break;
				case ',':
					currentToken = DOT;
					break;
				default:
					currentToken = ERROR_INP;
					break;
				}
			}
			break;
		case INCOMMENT:
			save = FALSE;
			if (c == EOF)
			{
				state = DONE;
				currentToken = ERROR_INP;
			}
			else if (c == '*')
			{
				if (getNextChar() == '/')
					state = START;
				else
					ungetNextChar();
			}
			break;
		case INNEQ:
			state = DONE;
			if (c == '=')
				currentToken = NEQ;
			else
			{
				ungetNextChar();
				save = FALSE;
				currentToken = ERROR_INP;
			}
			break;
		case INEQUAL:
			state = DONE;
			if (c == '=')
				currentToken = EQ;
			else
			{
				ungetNextChar();
				currentToken = ASSIGN;
			}
			break;
		case INNUM:
			if (!isdigit(c))
			{
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = NUM;
			}
			break;
		case INID:
			if (!isalpha(c))
			{
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = ID;
			}
			break;
		case INLT:
			state = DONE;
			if (c == '=')
				currentToken = LE;
			else
			{
				ungetNextChar();
				currentToken = LT;
			}
			break;
		case INRT:
			state = DONE;
			if (c == '=')
				currentToken = GE;
			else
			{
				ungetNextChar();
				currentToken = RT;
			}
			break;
		case DONE:
		default: /* 错误提示，应该不会出现 */
			fprintf(listing, "Scanner Bug: state= %d\n", state);
			state = DONE;
			currentToken = ERROR_INP;
			break;
		}
		if ((save) && (tokenStringIndex <= MAXTOKENLEN))
			tokenString[tokenStringIndex++] = (char)c;
		if (state == DONE)
		{
			tokenString[tokenStringIndex] = '\0';
			if (currentToken == ID)
			{
				currentToken = reservedLookup(tokenString);
			}
		}
	}
	if (TraceScan)
	{

		fprintf(listing, "\t%d: ", lineno);
		printToken(currentToken, tokenString);
	}
	printResult(lineno, currentToken, tokenString);
	return currentToken;
}