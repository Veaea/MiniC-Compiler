

#ifndef _ANALYSESEMANTIC_H_
#define _ANALYSESEMANTIC_H_
#include"globals.h"

/*函数功能说明:buildSymtab通过对语法树的预排序遍历来构造符号表*/
/*输入接口信息:FILE:待分析语法树根节点   list:符号表保存文件路径  */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
void buildSymtab(TreeNode *,FILE *list);

/*函数功能说明:typeCheck通过后序遍历语法树执行类型检查*/
/*输入接口信息:syntaxTree:需要类型检查的语法树根节点 */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
void typeCheck(TreeNode *syntaxTree);

#endif

