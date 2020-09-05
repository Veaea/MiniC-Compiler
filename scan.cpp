
#include "stdafx.h"
#include "globals.h"
#include "util.h"
#include "scan.h"
#include"MiniCDlg.h"
/*ɨ����DFA�е�״̬*/
typedef enum
{
	START, INEQUAL, INNEQ, INCOMMENT, INNUM, INID, DONE, INLT, INRT
}
StateType;

/* ��ʶ�������ֵĴ��� */
char tokenString[MAXTOKENLEN + 1];

/* BUFLEN=Դ�����е����뻺�������� */
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* �����ĵ�ǰԴ���������� */
int linepos = 0; /* ��ǰ��LineBuf�е�λ�� */
int bufsize = 0; /* �������ַ����ĵ�ǰ��С */
int EOF_flag = FALSE; /* �Ƿ�����EOF�ϵ�ungetNextChar��Ϊ��� */

/*��������˵��:����getNextChar��lineBuf��ȡ��һ���ǿ��ַ������lineBuf�ľ������ȡ����    */
/*����ӿ���Ϣ:void                                 */
/*����ӿ���Ϣ:��ȡ���ַ�ǿ��ת��Ϊint���͵�ASCII��            */
/*����ע������:ע�ⷵ��ֵ����������                 */
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

/*��������˵��:����ungetNextChar��lineBuf�л���һ���ַ�    */
/*����ӿ���Ϣ:void                                 */
/*����ӿ���Ϣ:void            */
/*����ע������:ע��ȫ�ֱ���linepos�ı仯                 */
static void ungetNextChar(void)
{
	if (!EOF_flag) linepos--;
}

/* �����ֲ��ұ� */
static struct
{
	char* str;
	TokenTypeInProjece tok;
} reservedWords[MAXRESERVED] = { { "if",IF },{ "else",ELSE },{ "int",Int },
{ "void",VoiD },{ "return",RETURN },{ "while",WHILE } };

/*��������˵��:����reservedLookupʹ�������������ұ�ʶ���Բ鿴���Ƿ��Ǳ�����    */
/*����ӿ���Ϣ:char *���͵��ַ���,��ʾ��Ҫ�����ı�ʶ��                         */
/*����ӿ���Ϣ:TokenTypeInProjece���͵�ö��ֵ            */
/*����ע������:ע������ͷ���ֵ������                 */
static TokenTypeInProjece reservedLookup(char * s)
{
	int i;
	for (i = 0; i < MAXRESERVED; i++)
		if (!strcmp(s, reservedWords[i].str))
			return reservedWords[i].tok;
	return ID;
}

/****************************************/
/*           ɨ���ǵ���Ҫ����           */
/****************************************/

/*��������˵��:����getToken����Դ�ļ��е���һ�����    */
/*����ӿ���Ϣ:void                                    */
/*����ӿ���Ϣ:TokenTypeInProjece���͵�ö��ֵ            */
/*����ע������:ע�ⷵ��ֵ������                   */
TokenTypeInProjece getToken(void)
{  
	/* ���ڴ洢��tokenString�е����� */
	int tokenStringIndex = 0;
	/* ����Ҫ���صĵ�ǰtoken */
	TokenTypeInProjece currentToken;
	/* ��ǰDFA״̬,ʼ�մӿ�ʼ����ʼ */
	StateType state = START;
	/* �ж��Ƿ񱣴�tokenString */
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
		default: /* ������ʾ��Ӧ�ò������ */
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