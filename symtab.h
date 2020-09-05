

#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include<string>

/*��������˵��:st_insert���кź��ڴ�λ�ò�����ű�,loc=���ڵ�һ�β����ڴ�λ��,�������*/
/*����ӿ���Ϣ:name:��������ű��ķ����ַ���ָ��   lineno:�����ַ�����Դ�����е���    loc:���ڵ�һ�β����ڴ�λ��,�������  */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
void st_insert(char * name, int lineno, int loc);

/*��������˵��:st_lookup���ر������ڴ�λ�ã�����Ҳ����򷵻�-1*/
/*����ӿ���Ϣ:name:���ӷ��ű��в��ҵķ����ַ���ָ��   */
/*����ӿ���Ϣ:����int�͵��ڴ�λ��                       */
/*����ע������:ע����������Ƿ���ȷ                            */
int st_lookup(char * name);

/*��������˵��:st_lookupLine���ر�����Դ�����кţ�����Ҳ����򷵻�-1*/
/*����ӿ���Ϣ:name:���ӷ��ű��в��ҵķ����ַ���ָ��   */
/*����ӿ���Ϣ:����int�͵�Դ�����к�                       */
/*����ע������:ע����������Ƿ���ȷ                            */
int st_lookupLine(char * name);

/*��������˵��:printSymTab�����ű����ݵĸ�ʽ���б���ӡ�����ű��ļ�*/
/*����ӿ���Ϣ:void   */
/*����ӿ���Ϣ:����void                       */
/*����ע������:ע����������Ƿ���ȷ                            */
void printSymTab();

#endif
