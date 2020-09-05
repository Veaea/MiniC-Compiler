

#include "globals.h"
#include "stdafx.h"
#include "util.h"
#include "parse.h"
#include "scan.h"
#include <string>
#include"analyseSemantic.h"
#include"cgen.h"
using namespace std;

/* ����ȫ�ֱ��� */
int lineno = 0;//�к�
FILE * source;//�����������ļ�
char sourceFile[120];
FILE * listing;//�﷨�������ļ�
FILE * result;//�ʷ�������������ļ�
FILE * code;
FILE *list;

/* ��������ø��ٵı�־ */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;

int Error = FALSE;

/*��������˵��:analyse���дʷ��������﷨������������������ڶ�Ӧ�ļ�·��*/
/*����ӿ���Ϣ:argv:�����������ļ�·��   str:�﷨�������ļ�·��  */
/*����ӿ���Ϣ:����0                       */
/*����ע������:ע��·����ʽ�Ƿ���ȷ                            */
int analyse(string argv, string str, string sts,string sem)
{
	lineno = 0;
	TreeNode * syntaxTree;
	char pgm[120]; /* source code file name */
	strcpy(pgm, argv.c_str());
	if (strchr(pgm, '.') == NULL)
		strcat(pgm, ".tny");
	strcpy(sourceFile, pgm);
	source = fopen(pgm, "r");
	if (source == NULL)
	{
		fprintf(stderr, "File %s not found\n", pgm);
		exit(1);
	}
	result = fopen(sts.c_str(), "w");
	listing = fopen(str.c_str(), "w");
	list = fopen(sem.c_str(), "w");
	//code = fopen(mc.c_str(), "w");
	fprintf(listing, "\nMiniC �﷨������\n");

	int linepos = 0;
	int bufsize = 0;
	EOF_flag = FALSE;
	syntaxTree = parse();

	fprintf(listing, "\nMiniC �﷨��:\n");
	printTree(syntaxTree);
	
	Error = FALSE;
	fprintf(list, "\n������ű�\n");
	buildSymtab(syntaxTree,list);
	fprintf(list, "\n���ͼ�飺\n");
	typeCheck(syntaxTree);
	if(Error==FALSE)
		fprintf(list, "\n���ͼ������\n");

	char * codefile;
	int fnlen = strcspn(pgm, ".");
	codefile = (char *)calloc(fnlen + 4, sizeof(char));
	strncpy(codefile, pgm, fnlen);
	strcat(codefile, ".tm");
	code = fopen(codefile, "w");
	if (code == NULL)
	{
		printf("Unable to open %s\n", codefile);
		exit(1);
	}
	codeGen(syntaxTree, codefile);
	fclose(code);

	fclose(list);
	fclose(listing);
	fclose(result);
	fclose(source);
	return 0;
}