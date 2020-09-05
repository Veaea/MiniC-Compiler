

#pragma once
#pragma warning(disable:4996)


#ifndef _UTIL_H_
#define _UTIL_H_
#include "globals.h"

/*函数功能说明:printToken将标记及其类型打印到语法树文件*/
/*输入接口信息:TokenTypeInProjece类型的token，const char*类型的tokenString数组*/
/*输出接口信息:void */
/*调用注意事项:注意参数的数据类型*/
void printToken(TokenTypeInProjece, const char*);

/*函数功能说明:函数printResult将行号、类型、字符串打印到词法分析结果文件  */
/*输入接口信息:lineno表示行号,token表示类型,tokenString表示字符串  */
/*输出接口信息:void  */
/*调用注意事项:注意参数的数据类型  */
void printResult(int lineno, TokenTypeInProjece token, const char* tokenString);

/*函数功能说明:函数newStmtNode为语法树创建一个新的语句节点  */
/*输入接口信息:StmtKind类型的数据，表示语句节点的语句类型  */
/*输出接口信息:TreeNode类型的语句节点地址  */
/*调用注意事项:注意参数和返回值的数据类型  */
TreeNode * newStmtNode(StmtKind);

/*函数功能说明:函数newExpNode创建用于语法树构造的新表达式节点  */
/*输入接口信息:ExpKind类型的数据，表示表达式节点的类型  */
/*输出接口信息:TreeNode类型的表达式节点地址  */
/*调用注意事项:注意参数和返回值的数据类型  */
TreeNode * newExpNode(ExpKind);

/*函数功能说明:函数copyString分配并生成现有字符串的新副本   */
/*输入接口信息:char *类型的数据，表示需要被处理的tokenString  */
/*输出接口信息:char *类型的数据，表示词法分析出的词  */
/*调用注意事项:注意参数和返回值的数据类型  */
char * copyString(char *);

/*函数功能说明:函数printree使用缩进将语法树打印到列表文件以指示子树   */
/*输入接口信息:TreeNode *类型的数据，表示需要打印的语法树节点  */
/*输出接口信息:void  */
/*调用注意事项:注意参数的数据类型  */
void printTree(TreeNode *);

#endif

