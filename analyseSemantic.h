

#ifndef _ANALYSESEMANTIC_H_
#define _ANALYSESEMANTIC_H_
#include"globals.h"

/*��������˵��:buildSymtabͨ�����﷨����Ԥ���������������ű�*/
/*����ӿ���Ϣ:FILE:�������﷨�����ڵ�   list:���ű����ļ�·��  */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
void buildSymtab(TreeNode *,FILE *list);

/*��������˵��:typeCheckͨ����������﷨��ִ�����ͼ��*/
/*����ӿ���Ϣ:syntaxTree:��Ҫ���ͼ����﷨�����ڵ� */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
void typeCheck(TreeNode *syntaxTree);

#endif

