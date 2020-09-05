
#pragma once
#include <string>
using namespace std;

/*函数功能说明:analyse进行词法分析和语法分析，并将结果保存在对应文件路径*/
/*输入接口信息:argv:待分析代码文件路径   str:语法树保存文件路径  */
/*输出接口信息:返回0                       */
/*调用注意事项:注意路径格式是否正确                            */
extern int analyse(string argv, string str, string sts,string sem);
