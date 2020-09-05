

#pragma once
#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXTOKENLEN是token的最大大小 */
#define MAXTOKENLEN 40

/* tokenString数组存储每个token的词素 */
extern char tokenString[MAXTOKENLEN + 1];

/*函数功能说明:函数getToken返回源文件中的下一个词*/
/*输入接口信息:void  */
/*输出接口信息:返回源文件中的下一个词，并将词转化为TokenTypeInProjece枚举类型      */
/*调用注意事项:注意返回值的数据类型                            */
TokenTypeInProjece getToken(void);

extern  int linepos; /* 当前在LineBuf中的位置 */
extern  int bufsize; /* 缓冲区字符串的当前大小 */
extern  int EOF_flag;/* 是否修正EOF上的ungetNextChar行为的标记，1不修正，0修正 */

#endif
