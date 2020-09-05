

#ifndef _CODE_H_
#define _CODE_H_

/* pc = ���������  */
#define  pc 7

/*mp = ��memory pointer��ָ���ڴ涥��(������ʱ�洢)*/
#define  mp 6

/*gp = "ȫ��ָ��"ָ���ڴ�ײ���(ȫ��)�����洢*/
#define gp 5

/* �ۼ��� */
#define  ac 0

/* �ڶ����ۼ��� */
#define  ac1 1

/* ���뷢��ʵ�ó��� */

/*��������˵��:emitComment�ڴ����ļ��д�ӡ����ע��c��ע����*/
/*����ӿ���Ϣ:c:ע���ַ���ָ��  */
/*����ӿ���Ϣ:����void             */
/*����ע������:ע����������Ƿ���ȷ                            */
void emitComment(char * c);

/*��������˵��:emitRO�����Ĵ���ר��TMָ��*/
/*����ӿ���Ϣ:
op = ������
r = Ŀ��Ĵ���
s = ��һ��Դ�Ĵ���
t = ��2Դ�Ĵ���
c:���TraceCodeΪ�棬��Ҫ��ӡ��ע��  */
/*����ӿ���Ϣ:����void             */
/*����ע������:ע����������Ƿ���ȷ          */
void emitRO(char *op, int r, int s, int t, char *c);

/*��������˵��:emitRM����һ���Ĵ������ڴ��TMָ��*/
/*����ӿ���Ϣ:
op = ������
r = Ŀ��Ĵ���
d = ƫ����
s = ���Ĵ���
c:���TraceCodeΪ�棬��Ҫ��ӡ��ע��  */
/*����ӿ���Ϣ:����void             */
/*����ע������:ע����������Ƿ���ȷ          */
void emitRM(char * op, int r, int d, int s, char *c);

/*��������˵��:emitSkipΪ�Ժ�Ļ�������howMany����λ,�������ص�ǰ����λ��*/
/*����ӿ���Ϣ:howMany:�����Ĵ���λ  */
/*����ӿ���Ϣ:����void             */
/*����ע������:ע����������Ƿ���ȷ          */
int emitSkip(int howMany);

/*��������˵��:emitBackup���ݵ�֮ǰ������λ��loc*/
/*����ӿ���Ϣ:loc:֮ǰ������λ��  */
/*����ӿ���Ϣ:����void             */
/*����ע������:ע����������Ƿ���ȷ          */
void emitBackup(int loc);

/*��������˵��:emitRestore����ǰ�����λ�ûָ�����ǰδ���������λ��*/
/*����ӿ���Ϣ:void  */
/*����ӿ���Ϣ:����void             */
/*����ע������:ע����������Ƿ���ȷ          */
void emitRestore(void);

/*��������˵��:�������Ĵ������ڴ��TMָ��ʱ��emitRM_Abs����������ת��Ϊpc�������*/
/*����ӿ���Ϣ:
op = ������
r = Ŀ��Ĵ���
a = �ڴ��еľ���λ��
c:���TraceCodeΪ�棬��Ҫ��ӡ��ע��  */
/*����ӿ���Ϣ:����void             */
/*����ע������:ע����������Ƿ���ȷ          */
void emitRM_Abs(char *op, int r, int a, char * c);

#endif
