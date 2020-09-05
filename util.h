

#pragma once
#pragma warning(disable:4996)


#ifndef _UTIL_H_
#define _UTIL_H_
#include "globals.h"

/*��������˵��:printToken����Ǽ������ʹ�ӡ���﷨���ļ�*/
/*����ӿ���Ϣ:TokenTypeInProjece���͵�token��const char*���͵�tokenString����*/
/*����ӿ���Ϣ:void */
/*����ע������:ע���������������*/
void printToken(TokenTypeInProjece, const char*);

/*��������˵��:����printResult���кš����͡��ַ�����ӡ���ʷ���������ļ�  */
/*����ӿ���Ϣ:lineno��ʾ�к�,token��ʾ����,tokenString��ʾ�ַ���  */
/*����ӿ���Ϣ:void  */
/*����ע������:ע���������������  */
void printResult(int lineno, TokenTypeInProjece token, const char* tokenString);

/*��������˵��:����newStmtNodeΪ�﷨������һ���µ����ڵ�  */
/*����ӿ���Ϣ:StmtKind���͵����ݣ���ʾ���ڵ���������  */
/*����ӿ���Ϣ:TreeNode���͵����ڵ��ַ  */
/*����ע������:ע������ͷ���ֵ����������  */
TreeNode * newStmtNode(StmtKind);

/*��������˵��:����newExpNode���������﷨��������±��ʽ�ڵ�  */
/*����ӿ���Ϣ:ExpKind���͵����ݣ���ʾ���ʽ�ڵ������  */
/*����ӿ���Ϣ:TreeNode���͵ı��ʽ�ڵ��ַ  */
/*����ע������:ע������ͷ���ֵ����������  */
TreeNode * newExpNode(ExpKind);

/*��������˵��:����copyString���䲢���������ַ������¸���   */
/*����ӿ���Ϣ:char *���͵����ݣ���ʾ��Ҫ�������tokenString  */
/*����ӿ���Ϣ:char *���͵����ݣ���ʾ�ʷ��������Ĵ�  */
/*����ע������:ע������ͷ���ֵ����������  */
char * copyString(char *);

/*��������˵��:����printreeʹ���������﷨����ӡ���б��ļ���ָʾ����   */
/*����ӿ���Ϣ:TreeNode *���͵����ݣ���ʾ��Ҫ��ӡ���﷨���ڵ�  */
/*����ӿ���Ϣ:void  */
/*����ע������:ע���������������  */
void printTree(TreeNode *);

#endif

