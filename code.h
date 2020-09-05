

#ifndef _CODE_H_
#define _CODE_H_

/* pc = 程序计数器  */
#define  pc 7

/*mp = “memory pointer”指向内存顶部(用于临时存储)*/
#define  mp 6

/*gp = "全局指针"指向内存底部的(全局)变量存储*/
#define gp 5

/* 累加器 */
#define  ac 0

/* 第二个累加器 */
#define  ac1 1

/* 代码发射实用程序 */

/*函数功能说明:emitComment在代码文件中打印带有注释c的注释行*/
/*输入接口信息:c:注释字符串指针  */
/*输出接口信息:返回void             */
/*调用注意事项:注意参数类型是否正确                            */
void emitComment(char * c);

/*函数功能说明:emitRO发出寄存器专用TM指令*/
/*输入接口信息:
op = 操作码
r = 目标寄存器
s = 第一个源寄存器
t = 第2源寄存器
c:如果TraceCode为真，则要打印的注释  */
/*输出接口信息:返回void             */
/*调用注意事项:注意参数类型是否正确          */
void emitRO(char *op, int r, int s, int t, char *c);

/*函数功能说明:emitRM发出一个寄存器到内存的TM指令*/
/*输入接口信息:
op = 操作码
r = 目标寄存器
d = 偏移量
s = 基寄存器
c:如果TraceCode为真，则要打印的注释  */
/*输出接口信息:返回void             */
/*调用注意事项:注意参数类型是否正确          */
void emitRM(char * op, int r, int d, int s, char *c);

/*函数功能说明:emitSkip为以后的回填跳过howMany代码位,它还返回当前代码位置*/
/*输入接口信息:howMany:跳过的代码位  */
/*输出接口信息:返回void             */
/*调用注意事项:注意参数类型是否正确          */
int emitSkip(int howMany);

/*函数功能说明:emitBackup备份到之前跳过的位置loc*/
/*输入接口信息:loc:之前跳过的位置  */
/*输出接口信息:返回void             */
/*调用注意事项:注意参数类型是否正确          */
void emitBackup(int loc);

/*函数功能说明:emitRestore将当前代码的位置恢复到先前未发出的最高位置*/
/*输入接口信息:void  */
/*输出接口信息:返回void             */
/*调用注意事项:注意参数类型是否正确          */
void emitRestore(void);

/*函数功能说明:当发出寄存器到内存的TM指令时，emitRM_Abs将绝对引用转换为pc相对引用*/
/*输入接口信息:
op = 操作码
r = 目标寄存器
a = 内存中的绝对位置
c:如果TraceCode为真，则要打印的注释  */
/*输出接口信息:返回void             */
/*调用注意事项:注意参数类型是否正确          */
void emitRM_Abs(char *op, int r, int a, char * c);

#endif

