/************************************************************************
* ��Ȩ���� (C)2016, �����������ܿƼ��ɷ����޹�˾��
*
* �ļ����ƣ� // MonthBalance.H
* �ļ���ʶ�� //�½����ļ�ͷ
* ����ժҪ�� // 
* ����˵���� //
* ��ǰ�汾�� // V01.00
* �� �ߣ� // yangxing
* ������ڣ� // 2016��08��06��
*
* �޸ļ�¼1��//
* �޸����ڣ�
* �� �� �ţ�
* �� �� �ˣ�
* �޸����ݣ�
************************************************************************/
#ifdef	MonthBalance_H
#define	MonthBalance_EXT
#else
#define	MonthBalance_EXT extern
#endif

MonthBalance_EXT unsigned char  Hour_Balance;

extern unsigned char PowerOffTimeCheck(void);
extern void EngryBalanceCheck(void);
extern void BalanceCheck(void);
extern void DmdBalanceCheck(void);
//extern unsigned char PowOnBalance(void);
extern void MonthBalanceProc(void);
MonthBalance_EXT void YearBalanceProc(void);
MonthBalance_EXT unsigned char BalanceMode_Judge(void);
MonthBalance_EXT void ClearYearBalance(void);