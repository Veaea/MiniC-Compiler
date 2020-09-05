

#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include<string>

/*函数功能说明:st_insert将行号和内存位置插入符号表,loc=仅在第一次插入内存位置,否则忽略*/
/*输入接口信息:name:待插入符号表的符号字符串指针   lineno:符号字符串在源代码中的行    loc:仅在第一次插入内存位置,否则忽略  */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
void st_insert(char * name, int lineno, int loc);

/*函数功能说明:st_lookup返回变量的内存位置，如果找不到则返回-1*/
/*输入接口信息:name:待从符号表中查找的符号字符串指针   */
/*输出接口信息:返回int型的内存位置                       */
/*调用注意事项:注意参数类型是否正确                            */
int st_lookup(char * name);

/*函数功能说明:st_lookupLine返回变量的源代码行号，如果找不到则返回-1*/
/*输入接口信息:name:待从符号表中查找的符号字符串指针   */
/*输出接口信息:返回int型的源代码行号                       */
/*调用注意事项:注意参数类型是否正确                            */
int st_lookupLine(char * name);

/*函数功能说明:printSymTab将符号表内容的格式化列表打印到符号表文件*/
/*输入接口信息:void   */
/*输出接口信息:返回void                       */
/*调用注意事项:注意参数类型是否正确                            */
void printSymTab();

#endif

