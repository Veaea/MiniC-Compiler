

#include"stdafx.h"
#include "globals.h"
#include "code.h"

/* ��ǰָ����TMλ�ú� */
static int emitLoc = 0;

/* ����Ϊֹ��������TMλ�ã�����emitSkip emitBackup��emitRestoreһ��ʹ�� */
static int highEmitLoc = 0;

/*��������˵��:emitComment�ڴ����ļ��д�ӡ����ע��c��ע����*/
/*����ӿ���Ϣ:c:ע���ַ���ָ��  */
/*����ӿ���Ϣ:����void             */
/*����ע������:ע����������Ƿ���ȷ                            */
void emitComment(char * c)
{
	if (TraceCode) fprintf(code, "* %s\n", c);
}

/*��������˵��:emitRO�����Ĵ���ר��TMָ��*/
/*����ӿ���Ϣ:
op = ������
r = Ŀ��Ĵ���
s = ��һ��Դ�Ĵ���
t = ��2Դ�Ĵ���
c:���TraceCodeΪ�棬��Ҫ��ӡ��ע��  */
/*����ӿ���Ϣ:����void             */
/*����ע������:ע����������Ƿ���ȷ          */
void emitRO(char *op, int r, int s, int t, char *c)
{
	fprintf(code, "%3d:  %5s  %d,%d,%d ", emitLoc++, op, r, s, t);
	if (TraceCode) fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc) highEmitLoc = emitLoc;
} /* emitRO */

  /*��������˵��:emitRM����һ���Ĵ������ڴ��TMָ��*/
  /*����ӿ���Ϣ:
  op = ������
  r = Ŀ��Ĵ���
  d = ƫ����
  s = ���Ĵ���
  c:���TraceCodeΪ�棬��Ҫ��ӡ��ע��  */
  /*����ӿ���Ϣ:����void             */
  /*����ע������:ע����������Ƿ���ȷ          */
void emitRM(char * op, int r, int d, int s, char *c)
{
	fprintf(code, "%3d:  %5s  %d,%d(%d) ", emitLoc++, op, r, d, s);
	if (TraceCode) fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc)  highEmitLoc = emitLoc;
} /* emitRM */

  /*��������˵��:emitSkipΪ�Ժ�Ļ�������howMany����λ,�������ص�ǰ����λ��*/
  /*����ӿ���Ϣ:howMany:�����Ĵ���λ  */
  /*����ӿ���Ϣ:����void             */
  /*����ע������:ע����������Ƿ���ȷ          */
int emitSkip(int howMany)
{
	int i = emitLoc;
	emitLoc += howMany;
	if (highEmitLoc < emitLoc)  highEmitLoc = emitLoc;
	return i;
} /* emitSkip */

  /*��������˵��:emitBackup���ݵ�֮ǰ������λ��loc*/
  /*����ӿ���Ϣ:loc:֮ǰ������λ��  */
  /*����ӿ���Ϣ:����void             */
  /*����ע������:ע����������Ƿ���ȷ          */
void emitBackup(int loc)
{
	if (loc > highEmitLoc) emitComment("BUG in emitBackup");
	emitLoc = loc;
} /* emitBackup */

  /*��������˵��:emitRestore����ǰ�����λ�ûָ�����ǰδ���������λ��*/
  /*����ӿ���Ϣ:void  */
  /*����ӿ���Ϣ:����void             */
  /*����ע������:ע����������Ƿ���ȷ          */
void emitRestore(void)
{
	emitLoc = highEmitLoc;
}

/*��������˵��:�������Ĵ������ڴ��TMָ��ʱ��emitRM_Abs����������ת��Ϊpc�������*/
/*����ӿ���Ϣ:
op = ������
r = Ŀ��Ĵ���
a = �ڴ��еľ���λ��
c:���TraceCodeΪ�棬��Ҫ��ӡ��ע��  */
/*����ӿ���Ϣ:����void             */
/*����ע������:ע����������Ƿ���ȷ          */
void emitRM_Abs(char *op, int r, int a, char * c)
{
	fprintf(code, "%3d:  %5s  %d,%d(%d) ",
		emitLoc, op, r, a - (emitLoc + 1), pc);
	++emitLoc;
	if (TraceCode) fprintf(code, "\t%s", c);
	fprintf(code, "\n");
	if (highEmitLoc < emitLoc) highEmitLoc = emitLoc;
} /* emitRM_Abs */
