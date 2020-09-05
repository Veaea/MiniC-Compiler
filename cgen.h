
#pragma once

#ifndef _CGEN_H_
#define _CGEN_H_

/*函数功能说明:codeGen通过遍历语法树向代码文件生成代码*/
/*输入接口信息:syntaxTree:待操作的语法树根节点   codefile:中间代码保存文件路径  */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
void codeGen(TreeNode * syntaxTree, char * codefile);

#endif
