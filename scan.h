

#pragma once
#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXTOKENLEN��token������С */
#define MAXTOKENLEN 40

/* tokenString����洢ÿ��token�Ĵ��� */
extern char tokenString[MAXTOKENLEN + 1];

/*��������˵��:����getToken����Դ�ļ��е���һ����*/
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:����Դ�ļ��е���һ���ʣ�������ת��ΪTokenTypeInProjeceö������      */
/*����ע������:ע�ⷵ��ֵ����������                            */
TokenTypeInProjece getToken(void);

extern  int linepos; /* ��ǰ��LineBuf�е�λ�� */
extern  int bufsize; /* �������ַ����ĵ�ǰ��С */
extern  int EOF_flag;/* �Ƿ�����EOF�ϵ�ungetNextChar��Ϊ�ı�ǣ�1��������0���� */

#endif
