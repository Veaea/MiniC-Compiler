

#include"stdafx.h"
#include "globals.h"
#include "code.h"

/* 当前指令发射的TM位置号 */
static int emitLoc = 0;

/* 迄今为止发射的最高TM位置，可与emitSkip emitBackup和emitRestore一起使用 */
static int highEmitLoc = 0;

/*函数功能说明:emitComment在代码文件中打印带有注释c的注释行*/
/*输入接口信息:c:注释字符串指针  */
/*输出接口信息:返回void             */
/*调用注意事项:注意参数类型是否正确                            */
void emitComment(char * c)
{
	if (TraceCode) fprintf(code, "* %s\n", c);
}

/*函数功能说明:emitRO发出寄存器专用TM指令*/
/*输入接口信息:
op = 操作码
r = 目标寄存器
s = 第一个源寄存器
t = 第2源寄存器
c:如果TraceCode为真，则要打印的注释  */
/*输出接口信息:返回void             */
/*调用注意事项:注意参数类型是否正确          */
void emitRO(char *op, int r, int s, int t, char *c)
{
	fprintf(code, "%3d:  %5s  %d,%d,%d ", emitLoc++, op, r, s, t);
	if (TraceCode) fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc) highEmitLoc = emitLoc;
} /* emitRO */

  /*函数功能说明:emitRM发出一个寄存器到内存的TM指令*/
  /*输入接口信息:
  op = 操作码
  r = 目标寄存器
  d = 偏移量
  s = 基寄存器
  c:如果TraceCode为真，则要打印的注释  */
  /*输出接口信息:返回void             */
  /*调用注意事项:注意参数类型是否正确          */
void emitRM(char * op, int r, int d, int s, char *c)
{
	fprintf(code, "%3d:  %5s  %d,%d(%d) ", emitLoc++, op, r, d, s);
	if (TraceCode) fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc)  highEmitLoc = emitLoc;
} /* emitRM */

  /*函数功能说明:emitSkip为以后的回填跳过howMany代码位,它还返回当前代码位置*/
  /*输入接口信息:howMany:跳过的代码位  */
  /*输出接口信息:返回void             */
  /*调用注意事项:注意参数类型是否正确          */
int emitSkip(int howMany)
{
	int i = emitLoc;
	emitLoc += howMany;
	if (highEmitLoc < emitLoc)  highEmitLoc = emitLoc;
	return i;
} /* emitSkip */

  /*函数功能说明:emitBackup备份到之前跳过的位置loc*/
  /*输入接口信息:loc:之前跳过的位置  */
  /*输出接口信息:返回void             */
  /*调用注意事项:注意参数类型是否正确          */
void emitBackup(int loc)
{
	if (loc > highEmitLoc) emitComment("BUG in emitBackup");
	emitLoc = loc;
} /* emitBackup */

  /*函数功能说明:emitRestore将当前代码的位置恢复到先前未发出的最高位置*/
  /*输入接口信息:void  */
  /*输出接口信息:返回void             */
  /*调用注意事项:注意参数类型是否正确          */
void emitRestore(void)
{
	emitLoc = highEmitLoc;
}

/*函数功能说明:当发出寄存器到内存的TM指令时，emitRM_Abs将绝对引用转换为pc相对引用*/
/*输入接口信息:
op = 操作码
r = 目标寄存器
a = 内存中的绝对位置
c:如果TraceCode为真，则要打印的注释  */
/*输出接口信息:返回void             */
/*调用注意事项:注意参数类型是否正确          */
void emitRM_Abs(char *op, int r, int a, char * c)
{
	fprintf(code, "%3d:  %5s  %d,%d(%d) ",
		emitLoc, op, r, a - (emitLoc + 1), pc);
	++emitLoc;
	if (TraceCode) fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc) highEmitLoc = emitLoc;
} /* emitRM_Abs */
