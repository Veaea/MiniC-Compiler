

#include "globals.h"
#include "stdafx.h"
#include "util.h"
#include "parse.h"
#include "scan.h"
#include <string>
#include"analyseSemantic.h"
#include"cgen.h"
using namespace std;

/* 分配全局变量 */
int lineno = 0;//行号
FILE * source;//待分析代码文件
char sourceFile[120];
FILE * listing;//语法树保存文件
FILE * result;//词法分析结果保存文件
FILE * code;
FILE *list;

/* 分配和设置跟踪的标志 */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode = TRUE;

int Error = FALSE;

/*函数功能说明:analyse进行词法分析和语法分析，并将结果保存在对应文件路径*/
/*输入接口信息:argv:待分析代码文件路径   str:语法树保存文件路径  */
/*输出接口信息:返回0                       */
/*调用注意事项:注意路径格式是否正确                            */
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
	fprintf(listing, "\nMiniC 语法分析：\n");

	int linepos = 0;
	int bufsize = 0;
	EOF_flag = FALSE;
	syntaxTree = parse();

	fprintf(listing, "\nMiniC 语法树:\n");
	printTree(syntaxTree);
	
	Error = FALSE;
	fprintf(list, "\n构造符号表：\n");
	buildSymtab(syntaxTree,list);
	fprintf(list, "\n类型检查：\n");
	typeCheck(syntaxTree);
	if(Error==FALSE)
		fprintf(list, "\n类型检查无误！\n");

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