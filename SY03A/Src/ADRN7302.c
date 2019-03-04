/************************************************************************
* ��Ȩ���� (C)2016, �����������ܿƼ��ɷ����޹�˾��
*
* �ļ����ƣ� // RN8302.c
* �ļ���ʶ�� // 
* ����ժҪ�� //����������������
* ����˵���� // 
* ��ǰ�汾�� // V01.00
* �� �ߣ�    // yangxing
* ������ڣ� // 2016��08��06��
*
* �޸ļ�¼1��//
* �޸����ڣ� //
* �� �� �ţ� // V01
* �� �� �ˣ� //
* �޸����ݣ� //
************************************************************************/
//#define  RN8302_H

#include "ex_func.h"
#include "ex_var.h"
#include <string.h>
//#include <absacc.h>
//#include <stdio.h>


//RN8302�Ĵ�������//
#define  UA       0x0007     //28 �з��� Phase A ��ѹ��Чֵ
#define  UB       0x0008     //28 �з��� Phase B ��ѹ��Чֵ
#define  UC       0x0009     //28 �з��� Phase C ��ѹ��Чֵ
#define  IA       0x000B     //28 �з��� Phase A ������Чֵ
#define  IB       0x000C     //28 �з��� Phase B ������Чֵ
#define  IC       0x000D     //28 �з��� Phase C ������Чֵ

#define  PA	  0x0014     //32 �з��� phase A �й�����˲ʱֵ
#define  PB	  0x0015     //32 �з��� phase B �й�����˲ʱֵ
#define  PC	  0x0016     //32 �з��� phase C �й�����˲ʱֵ
#define  PTP	  0x0017     //32 �з��� phase �����й�����˲ʱֵ

#define  QA	  0x0018     //32 �з��� phase A �޹�����˲ʱֵ
#define  QB	  0x0019     //32 �з��� phase B �޹�����˲ʱֵ
#define  QC	  0x001A     //32 �з��� phase C �޹�����˲ʱֵ
#define  QTQ	  0x001B     //32 �з��� phase �����޹�����˲ʱֵ

#define  SA	  0x001C     //32 �з��� phase A ���ڹ���˲ʱֵ
#define  SB	  0x001D     //32 �з��� phase B ���ڹ���˲ʱֵ
#define  SC	  0x001E     //32 �з��� phase C ���ڹ���˲ʱֵ
#define  STS	  0x004C     //32 �з��� phase �������ڹ���˲ʱֵ

#define  PfA	  0x0020     //24 �з��� phase A ��������
#define  PfB	  0x0021     //24 �з��� phase A ��������
#define  PfC	  0x0022     //24 �з��� phase A ��������
#define  PfTV	  0x004D     //24 �з��� phase ���๦������

#define  EPA      0x0030    //24 �޷���  Phase A �й������Ĵ���
#define  EPB      0x0031    //24 �޷���  Phase B �й������Ĵ���
#define  EPC      0x0032    //24 �޷���  Phase C �й������Ĵ���
#define  EPT      0x0033    //24 �޷���  Phase �����й������Ĵ���

#define  POSEPA   0x0034    //24 �޷���  Phase A �����й������Ĵ���
#define  POSEPB   0x0035    //24 �޷���  Phase B �����й������Ĵ���
#define  POSEPC   0x0036    //24 �޷���  Phase C �����й������Ĵ���
#define  POSEPT   0x0037    //24 �޷���  Phase ���������й������Ĵ���

#define  NEGEPA   0x0038    //24 �޷���  Phase A �����й������Ĵ���
#define  NEGEPB   0x0039    //24 �޷���  Phase B �����й������Ĵ���
#define  NEGEPC   0x003A    //24 �޷���  Phase C �����й������Ĵ���
#define  NEGEPT   0x003B    //24 �޷���  Phase ���෴���й������Ĵ���

#define  EQA      0x003C    //24 �޷��� Phase A �޹������Ĵ���
#define  EQB      0x003D    //24 �޷��� Phase B �޹������Ĵ���
#define  EQC      0x003E    //24 �޷��� Phase C �޹������Ĵ���
#define  EQT      0x003F    //24 �޷���  Phase �����޹������Ĵ���

#define  POSEQA   0x0040    //24 �޷��� Phase A �����޹������Ĵ���
#define  POSEQB   0x0041    //24 �޷��� Phase B �����޹������Ĵ���
#define  POSEQC   0x0042    //24 �޷��� Phase C �����޹������Ĵ���
#define  POSEQT   0x0043    //24 �޷���  Phase ���������޹������Ĵ���

#define  NEGEQA   0x0044    //24 �޷��� Phase A �����޹������Ĵ���
#define  NEGEQB   0x0045    //24 �޷��� Phase B �����޹������Ĵ���
#define  NEGEQC   0x0046    //24 �޷��� Phase C �����޹������Ĵ���
#define  NEGEQT   0x0047    //24 �޷���  Phase ���෴���޹������Ĵ���

#define  ESA      0x0048    //24 �޷��� Phase A ���������Ĵ���
#define  ESB      0x0049    //24 �޷��� Phase B ���������Ĵ���
#define  ESC      0x004A    //24 �޷��� Phase C ���������Ĵ���
#define  ESTV     0x004F    //24 �޷��� Phase �������������Ĵ���

#define  YUA	  0x0050    //24 �޷��� Phase UA ��ǼĴ���
#define  YUB	  0x0051    //24 �޷��� Phase UB ��ǼĴ���
#define  YUC	  0x0052    //24 �޷��� Phase UC ��ǼĴ���
#define  YIA	  0x0053    //24 �޷��� Phase IA ��ǼĴ���
#define  YIB	  0x0054    //24 �޷��� Phase IB ��ǼĴ���
#define  YIC	  0x0055    //24 �޷��� Phase IC ��ǼĴ���
#define  YIN	  0x0056    //24 �޷��� Phase IN ��ǼĴ���

#define  UFreq	  0x0057    //24 �޷��� ��ѹ��Ƶ��

//---------------���ú�״̬�Ĵ���----------------------//
//EMMУ��Ĵ���
#define	HFCONST1	0x0100  //16 �޷��� ��Ƶ��������Ĵ���1
#define	HFCONST2	0x0101  //16 �޷��� ��Ƶ��������Ĵ���2
#define	IStart_PS	0x0102  //16 �޷��� �й���������������ֵ�Ĵ���
#define	IStart_Q	0x0103  //16 �޷��� �޹�����������ֵ�Ĵ���
#define	LostVoltT	0x0104  //16 �޷��� ʧѹ��ֵ�Ĵ���
#define	ZXOT		0x0105  //16 �޷��� ���㷧ֵ�Ĵ���
#define	PRTH1L		0x0106  //16 �޷��� ��λ�ֶ�У��������ֵ1����
#define	PRTH1H		0x0107  //16 �޷��� ��λ�ֶ�У��������ֵ1����
#define	PRTH2L		0x0108  //16 �޷��� ��λ�ֶ�У��������ֵ2����
#define	PRTH2H		0x0109  //16 �޷��� ��λ�ֶ�У��������ֵ2����

#define	PHSUA		0x010C  //8 �޷��� ����ͨ��UA��λУ���Ĵ���
#define	PHSUB		0x010D  //8 �޷��� ����ͨ��UB��λУ���Ĵ���
#define	PHSUC		0x010E  //8 �޷��� ����ͨ��UC��λУ���Ĵ���
#define	PHSIA		0x010F  //24 �޷��� ����ͨ��IA��λУ���Ĵ���
#define	PHSIB		0x0110  //24 �޷��� ����ͨ��IV��λУ���Ĵ���
#define	PHSIC		0x0111  //24 �޷��� ����ͨ��IC��λУ���Ĵ���

#define	GSUA		0x0113  //16 �з��� ����ͨ��UA����
#define	GSUB		0x0114  //16 �з��� ����ͨ��UB����
#define	GSUC		0x0115  //16 �з��� ����ͨ��UC����
#define	GSIA		0x0116  //16 �з��� ����ͨ��IA����
#define	GSIB		0x0117  //16 �з��� ����ͨ��IB����
#define	GSIC		0x0118  //16 �з��� ����ͨ��IC����
#define	GSIN		0x0119  //16 �з��� ����ͨ��IN����

#define	DCOS_UA		0x011A  //16 �з��� ����ͨ��UAֱ��OffsetУ��
#define	DCOS_UB		0x011B  //16 �з��� ����ͨ��UBֱ��OffsetУ��
#define	DCOS_UC		0x011C  //16 �з��� ����ͨ��UCֱ��OffsetУ��
#define	DCOS_IA		0x011D  //16 �з��� ����ͨ��IAֱ��OffsetУ��
#define	DCOS_IB		0x011E  //16 �з��� ����ͨ��IBֱ��OffsetУ��
#define	DCOS_IC		0x011F  //16 �з��� ����ͨ��ICֱ��OffsetУ��

#define	UA_OS		0x0121  //16 �з��� A���ѹ��ЧֵOffset
#define	UB_OS		0x0122  //16 �з��� B���ѹ��ЧֵOffset
#define	UC_OS		0x0123  //16 �з��� C���ѹ��ЧֵOffset
#define	IA_OS		0x0124  //16 �з��� A�������ЧֵOffset
#define	IB_OS		0x0125  //16 �з��� B�������ЧֵOffset
#define	IC_OS		0x0126  //16 �з��� C�������ЧֵOffset

#define	GPA		0x0128  //16 �з��� A���й���������
#define	GPB		0x0129  //16 �з��� B���й���������
#define GPC		0x012A  //16 �з��� C���й���������
#define	GQA		0x012B  //16 �з��� A���޹���������
#define	GQB		0x012C  //16 �з��� B���޹���������
#define	GQC		0x012D  //16 �з��� C���޹���������
#define	GSA		0x012E  //16 �з��� A���ӹ���������
#define	GSB		0x012F  //16 �з��� B���ӹ���������
#define	GSC		0x0130  //16 �з��� C���ӹ���������

#define	PA_PHSL		0x0131  //16 �з��� A���й��ֶ���λУ���Ĵ���-�Ͷ�
#define	PB_PHSL		0x0132  //16 �з��� B���й��ֶ���λУ���Ĵ���-�Ͷ�
#define	PC_PHSL		0x0133  //16 �з��� C���й��ֶ���λУ���Ĵ���-�Ͷ�
#define	PA_PHSM		0x01B0  //16 �з��� A���й��ֶ���λУ���Ĵ���-�ж�
#define	PB_PHSM		0x01B2  //16 �з��� B���й��ֶ���λУ���Ĵ���-�ж�
#define	PC_PHSM		0x01B4  //16 �з��� C���й��ֶ���λУ���Ĵ���-�ж�
#define	PA_PHSH		0x01B1  //16 �з��� A���й��ֶ���λУ���Ĵ���-�߶�
#define	PB_PHSH		0x01B3  //16 �з��� B���й��ֶ���λУ���Ĵ���-�߶�
#define	PC_PHSH		0x01B5  //16 �з��� C���й��ֶ���λУ���Ĵ���-�߶�

#define	QA_PHSL		0x0134  //16 �з��� A���޹��ֶ���λУ���Ĵ���-�Ͷ�
#define	QB_PHSL		0x0135  //16 �з��� B���޹��ֶ���λУ���Ĵ���-�Ͷ�
#define	QC_PHSL		0x0136  //16 �з��� C���޹��ֶ���λУ���Ĵ���-�Ͷ�
#define	QA_PHSM		0x01B6  //16 �з��� A���޹��ֶ���λУ���Ĵ���-�ж�
#define	QB_PHSM		0x01B8  //16 �з��� B���޹��ֶ���λУ���Ĵ���-�ж�
#define	QC_PHSM		0x01BA  //16 �з��� C���޹��ֶ���λУ���Ĵ���-�ж�
#define	QA_PHSH		0x01B7  //16 �з��� A���޹��ֶ���λУ���Ĵ���-�߶�
#define	QB_PHSH		0x01B9  //16 �з��� B���޹��ֶ���λУ���Ĵ���-�߶�
#define	QC_PHSH		0x01BB  //16 �з��� C���޹��ֶ���λУ���Ĵ���-�߶�

#define	PA_OS		0x0137  //16 �з��� A���й�����Offset
#define	PB_OS		0x0138  //16 �з��� B���й�����Offset
#define	PC_OS		0x0139  //16 �з��� C���й�����Offset
#define	QA_OS		0x013A  //16 �з��� A���޹�����Offset
#define	QB_OS		0x013B  //16 �з��� B���޹�����Offset
#define	QC_OS		0x013C  //16 �з��� C���޹�����Offset
//EMM���ú�״̬�Ĵ���
#define	CFCFG		0x0160  //24 CF�������üĴ���
#define EMUCFG          0x0161  //24 EMU���üĴ���
#define EMUCON          0x0162  //24 EMU���ƼĴ���
#define	PQSign		0x0166  //16 �й��޹����ʷ���Ĵ���
#define	PHASES		0x0169  //16 ���ѹ����״̬�Ĵ���
#define	CheckSum1	0x016A  //24 EMMУ������üĴ���У���
//ϵͳ���üĴ���
#define	WREN		0x0180  //8  дʹ�ܼĴ���
#define	WMSW		0x0181  //8  ����ģʽ�л��Ĵ���
#define	SOFTRST		0x0182  //8  �����λ�Ĵ���
#define	ADCCFG		0x0183  //16 ADC���üĴ���
#define	MODSEL		0x0186  //8  ��������/����ģʽѡ��Ĵ���
//ϵͳ״̬�Ĵ���
#define	RData		0x018C  //32 �ϴ�SPI����������
#define	WData		0x018D  //32 �ϴ�SPIд�������
#define	DeviceID	0x018f  //24 RN8302 Device ID

#define	ZXOTU		0x0190  //16 �޷��� ���㷧ֵ��ѹ�͵����ֿ��Ĵ���
#define	ZXOTCFG		0x0192  //16 �޷��� ���㷧ֵѡ����ƼĴ���






//unsigned char  RN8302Status;
//unsigned char  RN8302Errcum;
//unsigned char  Second_8302;
//unsigned char  Second_Power;
//unsigned char  PowerRead_Flag;
//unsigned char  PowerReact1_Flag;
//unsigned char  PowerReact2_Flag;
//unsigned char  Quadrant[4];
//unsigned char  Angle_I;
//unsigned char  U_InvOrder_Flag;
//unsigned char  I_InvOrder_Flag;
//unsigned char  FactoryCFDEN;
//unsigned long  RN8302Checksum;


/**************************************************************************/
unsigned char BYTE_BCD( unsigned char HEX )
{
    if( HEX > 99 )
    {
        return 0x00;
    }

    return( (HEX/10)*0x10 + HEX%10 );
}

/**************************************************************************/
unsigned char BCD_BYTE( unsigned char BCD )
{
    return( ((BCD>>4) & 0x0F)*10 + (BCD & 0x0F) );
}

/************************************************************************
* ������ ��//WORD_BCD2
* ���� ��//��˫�ֽ�HEX��ת��Ϊ˫�ֽ�BCD��
* ������� ��//BCD��˫�ֽ�BCD�����ݵ�ַ,WORD��˫�ֽ�HEX������
* ������� ��//2�ֽ�BCD��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void WORD_BCD2( unsigned char *BCD, unsigned short WORD )
{
    if( WORD > 9999 )
    {
        *BCD = 0x00;
        *(BCD+1) = 0x00;
        return;
    }
    
    *(BCD+1) = BYTE_BCD( (unsigned char)(WORD/100) );
    *BCD = BYTE_BCD( (unsigned char)(WORD%100) );
}

/************************************************************************
* ������ ��//BCD2_WORD
* ���� ��//��˫�ֽ�BCD��ת��Ϊ˫�ֽ�HEX��
* ������� ��//BCD��˫�ֽ�BCD�����ݵ�ַ
* ������� ��//˫�ֽ�HEX������
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
unsigned int BCD2_WORD( unsigned char *BCD )
{
    return ( BCD_BYTE( *(BCD+1) )*100 + BCD_BYTE( *BCD ) );
}

/************************************************************************
* ������ ��//HEX4_BCD4
* ���� ��//��4�ֽ�HEX��ת��Ϊ4�ֽ�BCD��
* ������� ��//BCD��4�ֽ�BCD�����ݵ�ַ��HEX��4�ֽ�HEX������
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void HEX4_BCD4( unsigned char *BCD, unsigned long HEX )
{
    unsigned char i;

    if( HEX > 99999999 )
    {
        *BCD = 0x00;
        *(BCD+1) = 0x00;
        *(BCD+2) = 0x00;
        *(BCD+3) = 0x00;
    }
    else
    {
        for( i = 0; i < 4; i++ )
        {
            *BCD = HEX%10;
            HEX = HEX/10;
            *BCD += ((HEX%10)<<4) & 0xF0;
            HEX = HEX/10;
            BCD++;
        }
    }
}

/************************************************************************
* ������ ��//BCD4_HEX4
* ���� ��//��4�ֽ�BCD��ת��Ϊ4�ֽ�HEX��
* ������� ��//BCD��4�ֽ�BCD�����ݵ�ַ
* ������� ��//4�ֽ�HEX������
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
unsigned long BCD4_HEX4( unsigned char *BCD )
{
    unsigned char i,Tmp;
    unsigned long HEX=0;

    BCD = BCD + 3;
    for( i=0;i<3;i++ )
    {
        Tmp = BCD_BYTE( *BCD );
        HEX = (HEX + Tmp) * 100;
        BCD--;
    }
    Tmp = BCD_BYTE( *BCD );
    HEX = HEX + Tmp;
    return HEX;
}
/**************************************************************************/
void Delay_Xms(unsigned long mSec)
{   
	while (mSec--)	
	{           
      I2c_Wait_1ms();
	}
}
/************************************************************************
* ������ ��//RN8302_Reset
* ���� ��//RN8302��λ
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/03/06 V1.0 yangxing XXXX
**************************************************************************/
void RN8302_Reset(void)
{
    unsigned char Temp=0;
    Clear_Wdt();//�忴�Ź�
    
    Temp = 0xE5;//дʹ��
    RN8302_Write( WREN, (unsigned char*)&Temp, 1 );
    Temp = 0xA2;//�л�����ģʽ��EMM
    RN8302_Write( WMSW, (unsigned char*)&Temp, 1 ); 
    Temp = 0xFA;//�����λ
    RN8302_Write( SOFTRST, (unsigned char*)&Temp, 1 );
    Delay_Xms(50);
    Temp = 0xDC;//дʹ��
    RN8302_Write( WREN, (unsigned char*)&Temp, 1 );
    Clear_Wdt();//�忴�Ź�
}

/************************************************************************
* ������ ��//Enable_SPI
* ���� ��//RN8302����SPIͨѶ
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/03/06 V1.0 yangxing XXXX
**************************************************************************/
void Enable_SPI(void)
{
    unsigned char i;

    for( i = 0; i < 3; i++ )
    {
        P_RN8302_CS_H;// CS=1
        P_RN8302_CS_PORT;
        Delay_Xms(1);
        P_RN8302_CS_L;// CS=0
        Delay_Xms(1);
         Clear_Wdt();//�忴�Ź�
    }
    Delay_Xms(1);
    P_RN8302_CS_H;// CS=1
    Delay_Xms(10);
    Clear_Wdt();//�忴�Ź�
}

/************************************************************************
* ������ ��//RN8302_BasicPara
* ���� ��//������������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/03/06 V1.0 yangxing XXXX
**************************************************************************/
void RN8302_BasicPara(void)
{
    unsigned long Temp=0;
    
    RN8302Checksum = 0;
    Temp = 0xE5;//дʹ��
    RN8302_Write( WREN, (unsigned char*)&Temp, 1 );
          
    RN8302_Read( (unsigned char*)&Temp, EMUCON, 3 );
    Temp = 0x000777;//ʹ�ܷ����й����޹������ڵ��ܼ����ҷ�����������ܼ���
    RN8302_Write( EMUCON, (unsigned char*)&Temp, 3 );
//	  Temp = 0x000000;
//	  RN8302_Read( (unsigned char*)&Temp, EMUCON, 3 );
    RN8302Checksum +=Temp;

    Temp = MeterParameter.MeterHFConst1;//��Ƶ��������Ĵ���1,Ĭ��ֵ��0x1000
    RN8302_Write( HFCONST1, (unsigned char*)&Temp, 2 );
    RN8302Checksum +=Temp;
    
    Temp = MeterParameter.MeterHFConst2;//��Ƶ��������Ĵ���2,Ĭ��ֵ��0x1000
    RN8302_Write( HFCONST2, (unsigned char*)&Temp, 2 );
    RN8302Checksum +=Temp;
    
    Temp = 0x0000;//��������1������ѹ����1��
    RN8302_Write( ADCCFG, (unsigned char*)&Temp, 2 ); 
    
    if( MeterParameter.Meter_Type == 0x34 )//��������ģʽ
        Temp = 0x00;
    else//��������ģʽ
        Temp = 0x33;
    RN8302_Write( MODSEL, (unsigned char*)&Temp, 1 ); 
    
    Temp = Factor.IStart_P;//���ڡ��й�������������ֵ�Ĵ���,Ĭ��ֵ��0x0250
    RN8302_Write( IStart_PS, (unsigned char*)&Temp, 2 );   
    RN8302Checksum +=Temp;
    
    //Temp = Factor.IStart_Q;//�޹�������������ֵ�Ĵ���
    RN8302_Write( IStart_Q, (unsigned char*)&Temp, 2 ); //Ĭ��ֵ��0x0250
    RN8302Checksum +=Temp;
    
    Temp = (unsigned long)(MeterParameter.RN8302Irms*0.34);//��λ�ֶ�У��������ֵ1����
    Temp = (unsigned long)(Temp >> 12);//ȡ��16λ
    RN8302_Write( PRTH1L, (unsigned char*)&Temp, 2 );
    RN8302Checksum +=Temp;
    
    Temp = (unsigned long)(MeterParameter.RN8302Irms*0.35);//��λ�ֶ�У��������ֵ1����
    Temp = (unsigned long)(Temp >> 12);//ȡ��16λ
    RN8302_Write( PRTH1H, (unsigned char*)&Temp, 2 );
    RN8302Checksum +=Temp;
    if(MeterParameter.CT == 0x03 )//ֱ��ʽ
        Temp = (unsigned long)(MeterParameter.RN8302Irms*5.4);//��λ�ֶ�У��������ֵ2����
    else
        Temp = (unsigned long)(MeterParameter.RN8302Irms*2.4);//��λ�ֶ�У��������ֵ2����
    Temp = (unsigned long)(Temp >> 12);//ȡ��16λ
    RN8302_Write( PRTH2L, (unsigned char*)&Temp, 2 );
    RN8302Checksum +=Temp;
    
    if(MeterParameter.CT == 0x03 )
        Temp = (unsigned long)(MeterParameter.RN8302Irms*5.5);//��λ�ֶ�У��������ֵ2����
    else
        Temp = (unsigned long)(MeterParameter.RN8302Irms*2.5);//��λ�ֶ�У��������ֵ2����
    Temp = (unsigned long)(Temp >> 12);//ȡ��16λ
    RN8302_Write( PRTH2H, (unsigned char*)&Temp, 2 );
    RN8302Checksum +=Temp;  

    Temp = (unsigned long)(MeterParameter.RN8302Irms*0.001/4096);//���㷧ֵ�Ĵ���Ĭ��ֵ��0x0073
    RN8302_Write( ZXOT, (unsigned char*)&Temp, 2 );
    RN8302Checksum +=Temp;
    
    Temp = (unsigned long)(MeterParameter.RN8302Urms*0.05/4096);//���㷧ֵ�Ĵ���Ĭ��ֵ��0x0000
    RN8302_Write( ZXOTU, (unsigned char*)&Temp, 2 );
    RN8302Checksum +=Temp;

    Temp = 0x0010;//��ѹ�нǺ�Ƶ�ʲ���ZXOTU��Ϊ�жϷ�ֵ��0x0010
    RN8302_Write( ZXOTCFG, (unsigned char*)&Temp, 2 );
    
    Temp = 0x047798;//CF1ʹ������й�������CF2ʹ������޹�������CF3��CF4��ֹ���Ĭ��ֵ��0x043210
    RN8302_Write( CFCFG, (unsigned char*)&Temp, 3 );
    RN8302Checksum +=Temp;
    
    Temp = 0x400000;//�����С��޹������ۼӼĴ������ô������ۼ�ģʽ���������㡢���߲�������㡢ȫ�����������жϡ���ͨ�˲�ʹ��
    RN8302_Write( EMUCFG, (unsigned char*)&Temp, 3 );//Ĭ��ֵ��0x400000
    RN8302Checksum +=Temp;

    Temp = 0xDC;//д����
    RN8302_Write( WREN, (unsigned char*)&Temp, 1 );   

}

/************************************************************************
* ������ ��//ADE7858_AdjPara
* ���� ��//ADE7858У���������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------

**************************************************************************/
void RN8302_AdjPara(void)
{
    unsigned char i;
    unsigned long Temp=0;
    
    Meter_RN7302Para_Verify();
    
    Temp = 0xE5;//дʹ��
    RN8302_Write( WREN, (unsigned char*)&Temp, 1 );
    
    for( i = 0; i < 3; i++ )//ע�⣬У���������Ǹ���������ֽڲ�0F
    {   //��ѹ��������
        RN8302_Write( (GSIA+i), (unsigned char*)&Adj_Block[i].Adj.Igain, 2 );//��������
        RN8302Checksum +=Adj_Block[i].Adj.Igain;
        RN8302_Write( (GSUA+i), (unsigned char*)&Adj_Block[i].Adj.Ugain, 2 );//��ѹ����
        RN8302Checksum +=Adj_Block[i].Adj.Ugain;
        //��ѹ����ƫ��
        RN8302_Write( (IA_OS+i), (unsigned char*)&Adj_Block[i].Adj.Iofst, 2 );//����ƫ��
        RN8302Checksum +=Adj_Block[i].Adj.Iofst;
        RN8302_Write( (UA_OS+i), (unsigned char*)&Adj_Block[i].Adj.Uofst, 2 );//��ѹƫ��
        RN8302Checksum +=Adj_Block[i].Adj.Uofst;
        //��������
        RN8302_Write( (GSA+i), (unsigned char*)&Adj_Block[i].Adj.Pgain, 2 );//���ڹ�������
        RN8302Checksum +=Adj_Block[i].Adj.Pgain;
        RN8302_Write( (GPA+i), (unsigned char*)&Adj_Block[i].Adj.Pgain, 2 );//�й�������
        RN8302Checksum +=Adj_Block[i].Adj.Pgain;
        
        Adj_Block2[i].Adj2.Iofst2=0x0000;
        RN8302_Write( (DCOS_IA+i), (unsigned char*)&Adj_Block2[i].Adj2.Iofst2, 2 );//ֱ��OFFSET
        
        if( Meterspecial.AdjCheck_Mode == 0x5A )//�޹�У��
        {
            RN8302_Write( (GQA+i), (unsigned char*)&Adj_Block[i].Adj.Qgain, 2 );//�޹�������
            RN8302Checksum +=Adj_Block[i].Adj.Qgain;
        }
        else
        {
            RN8302_Write( (GQA+i), (unsigned char*)&Adj_Block[i].Adj.Pgain, 2 );//�޹�������
            RN8302Checksum +=Adj_Block[i].Adj.Pgain;
        }

        //����ƫ��
        RN8302_Write( (PA_OS+i), (unsigned char*)&Adj_Block[i].Adj.Pofst, 2 );//�й���ƫ��
        RN8302Checksum +=Adj_Block[i].Adj.Pofst;
        RN8302_Write( (QA_OS+i), (unsigned char*)&Adj_Block[i].Adj.Pofst, 2 );//�޹�����ƫ��
        RN8302Checksum +=Adj_Block[i].Adj.Pofst;
        //ͨ����λ
        RN8302_Write( (PHSUA+i), (unsigned char*)&Adj_Block[i].Adj.UTofst, 1 );//����ͨ��U��λƫ��
        RN8302Checksum +=Adj_Block[i].Adj.UTofst;
        RN8302_Write( (PHSIA+i), (unsigned char*)&Adj_Block[i].Adj.ITofst, 3 );//����ͨ��I��λƫ�� 
        memcpy( (unsigned char*)&Temp, (unsigned char*)&Adj_Block[i].Adj.ITofst, 3 );
        RN8302Checksum += Temp;
        //������λ
        RN8302_Write( (PA_PHSL+i), (unsigned char*)&Adj_Block[i].Adj.PTofst[1], 2 );//�Ͷ��й���λƫ��
        RN8302Checksum +=Adj_Block[i].Adj.PTofst[1];
        
        Temp=0x00;
        memcpy( (unsigned char*)&Temp, (unsigned char*)&Adj_Block[i].Adj.PTofst[0], 2 );
        RN8302_Write( (PA_PHSM+i*2), (unsigned char*)&Temp, 3 );//�ж��й���λƫ�ƣ��Ĵ���Ϊ3���ֽڣ�ֻ�Ǹ��ֽ�δ�ã�
        RN8302Checksum +=Temp;
        
        Temp=0x00;
        memcpy( (unsigned char*)&Temp, (unsigned char*)&Adj_Block[i].Adj.PTofst[2], 2 );
        RN8302_Write( (PA_PHSH+i*2), (unsigned char*)&Temp, 3 );//�߶��й���λƫ��
        RN8302Checksum +=Temp;
        
        if( Meterspecial.AdjCheck_Mode == 0x5A )//�޹�У��
        {
            RN8302_Write( (QA_PHSL+i), (unsigned char*)&Adj_Block[i].Adj.QTofst[1], 2 );//�Ͷ��޹���λƫ��
            RN8302Checksum +=Adj_Block[i].Adj.QTofst[1];
        }
        else
        {
            RN8302_Write( (QA_PHSL+i), (unsigned char*)&Adj_Block[i].Adj.PTofst[1], 2 );//�Ͷ��޹���λƫ��
            RN8302Checksum +=Adj_Block[i].Adj.PTofst[1];
        }

        Temp=0x00;
        if( Meterspecial.AdjCheck_Mode == 0x5A )//�޹�У��
        {
            memcpy( (unsigned char*)&Temp, (unsigned char*)&Adj_Block[i].Adj.QTofst[0], 2 );
        }
        else
            memcpy( (unsigned char*)&Temp, (unsigned char*)&Adj_Block[i].Adj.PTofst[0], 2 );
        RN8302_Write( (QA_PHSM+i*2), (unsigned char*)&Temp, 3 );//�ж��޹���λƫ��
        RN8302Checksum +=Temp;
        
        Temp=0x00;
        if( Meterspecial.AdjCheck_Mode == 0x5A )//�޹�У��
        {
            memcpy( (unsigned char*)&Temp, (unsigned char*)&Adj_Block[i].Adj.QTofst[2], 2 );
        }
        else
            memcpy( (unsigned char*)&Temp, (unsigned char*)&Adj_Block[i].Adj.PTofst[2], 2 );
        RN8302_Write( (QA_PHSH+i*2), (unsigned char*)&Temp, 3 );//�߶��޹���λƫ��
        RN8302Checksum +=Temp;
        
        memcpy( (unsigned char*)&Temp, (unsigned char*)&Adj_Block[i].Adj.Igain, 2 );
        if(Temp<0x8000)
            Nvm1Ki.Ki[i]= (float)((float)Factor.Ki*16*(float)(Temp/32768+1))/0.87909;
        else
        {
            Temp=65536-Temp;
            Nvm1Ki.Ki[i]= (float)((float)Factor.Ki*16*(float)(1-(float)Temp/32768))/0.87909;
        }
    }
    RN8302Checksum += 0x480;//ʧѹ��ֵ�Ĵ���0x400+����ͨ��IN��λ�Ĵ���0x80
    RN8302Checksum = ((~RN8302Checksum) & 0x00FFFFFF);//У���
    RN8302_Read( (unsigned char*)&Temp, CheckSum1, 3 );//��У���
    if(Temp == RN8302Checksum)
        WriteE2WithBackup( ee_Chk_RN8302_page, ee_Chk_RN8302_inpage, (unsigned char*)&RN8302Checksum, ee_Chk_RN8302_lenth);

    
    Temp = 0xDC;//д����
    RN8302_Write( WREN, (unsigned char*)&Temp, 1 );
}

/************************************************************************
* ������ ��//ADE7858_Run
* ���� ��//ADE7858��������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/03/06 V1.0 yangxing XXXX
**************************************************************************/
void RN8302_Run(void)
{
    unsigned int Temp=0;

    Temp = 0xE5;//дʹ��
    RN8302_Write( WREN, (unsigned char*)&Temp, 1 );
    
    Temp = 0xA2;//�л�����ģʽ��EMM
    RN8302_Write( WMSW, (unsigned char*)&Temp, 1 );    
    
    Temp = 0xDC;//д����
    RN8302_Write( WREN, (unsigned char*)&Temp, 1 );
}

/************************************************************************
* ������ ��//ADE7858Init
* ���� ��//ADE7858��ʼ��
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/03/06 V1.0 
**************************************************************************/
void RN8302Init(void)
{
    Enable_SPI();//ѡ��SPIͨѶģʽ
    RN8302_SPI_Init();
	  RN8302_Reset();//��λ
    RN8302_BasicPara();//������������
    RN8302_AdjPara();//У���������
    Timer.PowerRead_Tmr = 0;
    Flag.PhOfset0 = 0;
    Flag.PhOfset1 = 0;
    RN8302Errcum = 0;
}

/************************************************************************
* ������ ��//UIMeas
* ���� ��//��ѹ��������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/03/06 V1.0 yangxing XXXX
**************************************************************************/
void UIMeas(void)
{
    unsigned char i;
    unsigned long Volt,Curnt;

    for( i = 0; i < 3; i++ )//��ȡ��ѹ��������Чֵ
    {
        if( ( i == 1 )&&(MeterParameter.Meter_Type == 0x33) )
            continue;
        RN8302_Read( (unsigned char*)&ChipReg.Urms[i], (UA+i), 4 );//��ѹ��Чֵ
        if(( ChipReg.Urms[i] & 0XF8000000 ) != 0)
        {
            ChipReg.Urms[i] = 0;
            RN8302Errcum++;
        }
        RN8302_Read( (unsigned char*)&ChipReg.Irms[i], (IA+i), 4 );//������Чֵ
        if(( ChipReg.Irms[i] & 0XF8000000 ) != 0)
        {
            ChipReg.Irms[i] = 0;
            RN8302Errcum++; 
        }
        Volt = (unsigned long)(((float)(ChipReg.Urms[i]*Factor.Ku))*1000);//XXX.XXX
        Curnt = (unsigned long)(((float)(ChipReg.Irms[i]*Factor.Ki))*10000);//XXX.XXXX

        if( (Volt > Factor.DispUnDown) && (Volt < Factor.DispUnUp) )
        {
            HEX4_BCD4((unsigned char*)&RealAdj.UAdj[i],Volt);//У���ѹ
            Real.U[i] = (unsigned short)(RealAdj.UAdj[i] >> 8);//��ʾ��ѹ
            if( Real.U[i] < MeasPara.Un75 )//Ϊ���л����ѹ���ڵ���Ƶ�ʼ��
                Flag.LineV |= ( F_LineA << i );
            else
                Flag.LineV &= ~( F_LineA << i );
        }
        else
        {
            RealAdj.UAdj[i] = 0;
            Real.U[i] = 0;
            Flag.LineV |= ( F_LineA << i );
        }

        if( Curnt < Factor.ImaxUp )
        {
            HEX4_BCD4((unsigned char*)&RealAdj.IAdj[i],Curnt);//У�����XX.XXXX
            Real.I[i] = (RealAdj.IAdj[i]>>4);
            
            if( MeterParameter.CT == 3 )
            {
                if(( (Real.I[i]&0x007FFFFF) < MeasPara.StartI )||(Timer.PowerUpTim_Tmr !=0 ))//��ʽXXX.XXX
                {
                    RealAdj.IAdj[i] = 0;
                    Real.I[i] = 0;
                }
            }
            else
            {
                if(( (RealAdj.IAdj[i]&0x007FFFFF) <= MeasPara.StartI2 )||(Timer.PowerUpTim_Tmr !=0 ))//��ʽXX.XXXX
                {
                    RealAdj.IAdj[i] = 0;
                    Real.I[i] = 0;
                }
            }
        }
    }
}
/************************************************************************
* ������ ��//PowerMeas
* ���� ��//���ʲ���
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/03/06 V1.0 yangxing XXXX
**************************************************************************/
void PowerMeas(void)
{
    signed long Power[16];//���������ʱ�й�\�޹�\���ڹ���\��������
    unsigned long ActPower,RActPower,ApparentPower;
    unsigned char i,Num;
    unsigned short Angle;
    
    memset( (unsigned char*)&Power[0], 0x00, 64 );

    for( i = 0; i < 3; i++ )//����
    {
        if( ( i == 1 )&&(MeterParameter.Meter_Type == 0x33) )
            continue;
        RN8302_Read( (unsigned char*)&Power[i], (PA+i), 4 );//��ȡ�����й�����
        RN8302_Read( (unsigned char*)&Power[i+4], (QA+i), 4 );//��ȡ�����޹�����
        RN8302_Read( (unsigned char*)&Power[i+8], (SA+i), 4 );//��ȡ�������ڹ���
        RN8302_Read( (unsigned char*)&Power[i+12], (PfA+i), 3 );//��ȡ���๦������
        Power[i+12] = (0x007FFFFF&Power[i+12]);

        if( Power[i] < 0 )//�й������ж�
        {
            Power[i] = ~Power[i]+1;
            Power[i+12] = (8388608-Power[i+12]);
            Flag.ECDir |= ( F_ActRevA << i );
        }
        else
            Flag.ECDir &= ~( F_ActRevA << i );

        if( Power[i+4] < 0 )//�޹������ж�
        {
            Power[i+4] = ~Power[i+4]+1;
            Flag.ECDir |= ( F_RActRevA << i );
        }
        else
            Flag.ECDir &= ~( F_RActRevA << i );

        ActPower = (unsigned long)(((float)(Power[i]*Factor.Kp))*10);//�й�����
        RActPower = (unsigned long)(((float)(Power[i+4]*Factor.Kp))*10);//�޹�����
        ApparentPower = (unsigned long)(((float)(Power[i+8]*Factor.Kp)));//���ڹ���

        HEX4_BCD4((unsigned char*)&RealAdj.PAdj[i+1],ActPower);//У����XX.XXXXX
        Real.PKw[i+1] = (RealAdj.PAdj[i+1] >> 4);//��ʾ�й�����XX.XXXX
        HEX4_BCD4( (unsigned char*)&RealAdj.RAdj[i+1],RActPower );//��ʾ�޹�����XX.XXXX
        Real.PKvar[i+1] = (RealAdj.RAdj[i+1] >> 4);//��ʾ�޹�����XX.XXXX
        HEX4_BCD4( (unsigned char*)&Real.PKva[i+1],ApparentPower );//��ʾ���ڹ���XX.XXXX
        
        if((( (RealAdj.PAdj[i+1]&0x07FFFFFF) <= MeasPara.StartP2 )&&( (Real.I[i]&0x007FFFFF) < MeasPara.Ib05 ))||( Timer.PowerUpTim_Tmr !=0 ))
        {
            RealAdj.PAdj[i+1]=0;
            Real.PKw[i+1] = 0;
            Power[i] = 0;
            Flag.ECDir &= ~( F_ActRevA << i );
        }

        if((( (RealAdj.RAdj[i+1]&0x07FFFFFF) <= MeasPara.StartP2 )&&( (Real.I[i]&0x007FFFFF) < MeasPara.Ib05 ))||( Timer.PowerUpTim_Tmr !=0 ))
        {
            Real.PKvar[i+1] = 0;
            Power[i+4] = 0;
            Flag.ECDir &= ~( F_RActRevA << i );
        }

        if(( (Real.PKva[i+1]&0x007FFFFF) <= MeasPara.StartP )||( Timer.PowerUpTim_Tmr !=0 ))
        {
            Real.PKva[i+1] = 0;
            Power[i+8] = 0;
        }
        
        if( ((Real.I[i]&0x007FFFFF) < MeasPara.StartI )||(Real.PKva[i+1] == 0) ) //����������������:0.1Ib 
            Real.PF[i+1] = 0x1000;  
        else
        {
            Angle = (unsigned short)((float)(Power[i+12]/(float)8388608)*1000);//��������
            if( Angle > 1000 )
                Angle = 1000;
            WORD_BCD2( (unsigned char*)&Real.PF[i+1], Angle );//��ʾ��������X.XXX
        }

        if( Flag.ECDir & ( F_ActRevA << i ) )//�й�����
        {
            if( Real.I[i]!= 0 )
                Real.I[i] |= 0x800000;
            if( Real.PKw[i+1]!= 0 )
                Real.PKw[i+1] |= 0x800000;
            if( Real.PF[i+1]!= 0 )
                Real.PF[i+1] |= 0x8000;
        }
        else
        {
            Real.I[i] &= ~0x800000;
            Real.PKw[i+1] &= ~0x800000;
            Real.PF[i+1] &= ~0x8000;
        }

        if( Flag.ECDir & ( F_RActRevA << i ) )//�޹�����
        {
            if( Real.PKvar[i+1]!= 0 )
                Real.PKvar[i+1] |= 0x800000;
        }
        else
            Real.PKvar[i+1] &= ~0x800000;
             
    }
    
    RN8302_Read( (unsigned char*)&Power[3], PTP, 4 );//��ȡ�����й�����(ע�⣺���಻���ۼ�ģʽ��Ӱ�죬ֻ�д�����ģʽ)
    RN8302_Read( (unsigned char*)&Power[7], QTQ, 4 );//��ȡ�����޹�����
    RN8302_Read( (unsigned char*)&Power[11], STS, 4 );//��ȡ�������ڹ���
    RN8302_Read( (unsigned char*)&Power[15], PfTV, 3 );//��ȡ���๦������
    Power[15] = (0x007FFFFF&Power[15]);
    
    if(  Power[3] < 0 )//�������й�����
    {
        Power[3] = ~Power[3]+1;
        Power[15] = (8388608-Power[15]);
        Flag.ECDir |= F_ActRevZ;
    }
    else
        Flag.ECDir &= ~F_ActRevZ;
    
    if( Power[7] < 0 )//�������޹�����
    {
        Power[7] = ~Power[7]+1;
        Flag.ECDir |= F_RActRevZ;
    }
    else
        Flag.ECDir &= ~F_RActRevZ;
//����֤����    
    ActPower = (unsigned long)(((float)(Power[3]*Factor.Kp))*10);//�й�����
    RActPower = (unsigned long)(((float)(Power[7]*Factor.Kp))*10);//�޹�����
    ApparentPower = (unsigned long)((float)(Power[11]*Factor.Kp));//���ڹ���

    HEX4_BCD4( (unsigned char*)&RealAdj.PAdj[0],ActPower );//���й�����
    Real.PKw[0] = (RealAdj.PAdj[0] >> 4);//��ʾ���й�����XX.XXXX
    ActPower = (ActPower/10);
    HEX4_BCD4( (unsigned char*)&RealAdj.RAdj[0],RActPower );//���޹�����
    Real.PKvar[0] = (RealAdj.RAdj[0] >> 4);//��ʾ���޹�����XX.XXXX
    RActPower = (RActPower/10);
    HEX4_BCD4( (unsigned char*)&Real.PKva[0],ApparentPower );//�����ڹ���

    Angle = (unsigned short)((float)(Power[15]/(float)8388608)*1000);//��������
    if(( Angle > 1000 )||( Real.PKva[0]==0 ))
        Angle = 1000;
    WORD_BCD2( (unsigned char*)&Real.PF[0], Angle );//��ʾ��������X.XXX

    if( MeterParameter.Meter_Type == 0x34 )
    {
        if((Timer.PowerUpTim_Tmr !=0)||(( (RealAdj.PAdj[0]&0x07FFFFFF) <= MeasPara.StartP2 )&&(Real.PKw[1]== 0)&&(Real.PKw[2]== 0)&&(Real.PKw[3]== 0)))//����й����ʹ�С
        {
           Real.PKw[0] = 0;
           ActPower = 0;
           Flag.ECDir &= ~F_ActRevZ;
        }
        if((Timer.PowerUpTim_Tmr !=0)||(( (RealAdj.RAdj[0]&0x07FFFFFF) <= MeasPara.StartP2 )&&(Real.PKvar[1]== 0)&&(Real.PKvar[2]== 0)&&(Real.PKvar[3]== 0)))//����޹����ʹ�С
        {
           Real.PKvar[0] = 0;
           RActPower = 0;
           Flag.ECDir &= ~F_RActRevZ;
        }
        if((Timer.PowerUpTim_Tmr !=0)||(( (Real.PKva[0]&0x007FFFFF) <= MeasPara.StartP )&&(Real.PKva[1]== 0)&&(Real.PKva[2]== 0)&&(Real.PKva[3]== 0)))//������ڹ��ʹ�С
        {
           Real.PKva[0] = 0;
           Real.PF[0] = 0x1000;
           ApparentPower = 0;
        }
    }
    else
    {
        if((Timer.PowerUpTim_Tmr !=0)||(( (RealAdj.PAdj[0]&0x07FFFFFF) <= MeasPara.StartP2 )&&(Real.PKw[1]== 0)&&(Real.PKw[3]== 0)))//����й����ʹ�С
        {
           Real.PKw[0] = 0;
           ActPower = 0;
           Flag.ECDir &= ~F_ActRevZ;
        }
        if((Timer.PowerUpTim_Tmr !=0)||(( (RealAdj.RAdj[0]&0x07FFFFFF) <= MeasPara.StartP2 )&&(Real.PKvar[1]== 0)&&(Real.PKvar[3]== 0)))//����޹����ʹ�С
        {
           Real.PKvar[0] = 0;
           RActPower = 0;
           Flag.ECDir &= ~F_RActRevZ;
        }
        if((Timer.PowerUpTim_Tmr !=0)||(( (Real.PKva[0]&0x007FFFFF) <= MeasPara.StartP )&&(Real.PKva[1]== 0)&&(Real.PKva[3]== 0)))//������ڹ��ʹ�С
        {
           Real.PKva[0] = 0;
           Real.PF[0] = 0x1000;
           ApparentPower = 0;
        }
    }
    
    if( Flag.Run & F_Fist )
    {
        Flag.Run &= ~F_Fist;
        if( Flag.ECDir & F_ActRevZ )
            Flag.Run |= F_RevZP0;//�ϵ��һ�μ�鵽���ʵķ���ͬ��
        else 
            Flag.Run &=~ F_RevZP0;
    }
    
    if(( Flag.ECDir & F_ActRevZ )&&(Real.PKw[0] != 0))
    {
        Real.PKw[0] |= 0x800000;
        Real.PF[0] |= 0x8000;
    }
    else
    {
        Real.PKw[0] &= ~0x800000;
        Real.PF[0] &= ~0x8000;
    }

    if(( Flag.ECDir & F_RActRevZ )&&(Real.PKvar[0] != 0))
        Real.PKvar[0] |= 0x800000;
    else
        Real.PKvar[0] &= ~0x800000;

    
    //�����ж�
    for( i = 0; i < 4; i++ )
    {
        Num = 0;

        if( (Flag.ECDir & (F_ActRevZ<<i)) == 0 )//�й�����
        {
            Num = 10;
        }
        else
        {
            if( i == 0 )//���������֤�����л����й����ʷ���ͬ��
            {
                if( Timer.Rev_Cnt[3] < 2 )
                    Num = 10;
            }
            else
            {
                if( Timer.Rev_Cnt[i-1] < 2 )
                    Num = 10;
            }
        }

        if( (Flag.ECDir & (F_RActRevZ<<i)) == 0 )//�޹�����
            Num += 1;

        switch( Num )
        {
            case 0:  //��3����
                Quadrant[i] = 3;
                break;
            case 1:  //��2����
                Quadrant[i] = 2;
                break;
            case 10: //��4����
                Quadrant[i] = 4;
                break;
            default: //��1����
                Quadrant[i] = 1;
                break;
        }
    }

}

/************************************************************************
* ������ ��//ParaMeasure
* ���� ��//˲ʱ������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/03/06 V1.0 yangxing XXXX
**************************************************************************/
void ParaMeasure(void)
{
    unsigned char i;
    unsigned short Freq_Hz = 0;
    unsigned long Angle = 0;
    unsigned long Temp=0;
    unsigned short AngBuf[11];//Ua Ub Uc Ia Ib Ic Iab Ibc Iac Uab Ucb
    unsigned short UFlag = 0;

    if(  Second_8302 != Sclock.Clockbackup[0] )
    {
        Second_8302 = Sclock.Clockbackup[0];
        //PowerRead_Flag = 0;//��ֹ�ж϶������Ĵ���

        UIMeas();//��ѹ��������Чֵ����
          
        PowerMeas();//�й����޹������ڹ���,������������

        //Ƶ�ʲ���
        Temp=0;
        RN8302_Read( (unsigned char*)&Temp, UFreq, 3 );//����Ƶ��
        if( Temp != 0 )
            Freq_Hz = (unsigned short)(6553600000/Temp);//Freq_Hz=8192000*8/Temp*100 XX.XX
        else
            Freq_Hz = 5000;
        WORD_BCD2( (unsigned char*)&Real.Freq, (unsigned short)Freq_Hz );
         
        //�ǶȲ���
        for( i = 0; i < 6; i++ )
        {
            if( ( i == 1 )&&(MeterParameter.Meter_Type == 0x33) )
            continue;
            Angle = 0;
            RN8302_Read( (unsigned char*)&Angle, (YUA+i), 3 );//�Ƕ�
            AngBuf[i] = (unsigned short)(((float)Angle/16777216)*3600);//�ֱ���Ϊ0.07����Ǹ�ʽΪXXX.X���Զ����10����ʽ�ο��ֲ�           
        }
        
        //������ѹ�͵����ļн�
        for( i = 0; i < 3; i++ )
        {
            if( AngBuf[i] >= AngBuf[i+3] )
                Angle = (3600-(AngBuf[i] - AngBuf[i+3]));
            else
                 Angle = (AngBuf[i+3] - AngBuf[i]);
            
            if(( (Real.I[i]&0x7FFFFF) < MeasPara.StartI )||( Real.PKva[i+1] == 0 ))//�����
                Angle = 0;

            if( Angle > 3600 )
                Angle = 3600;
            WORD_BCD2( (unsigned char*)&Real.PhaseAngle[i], (unsigned short)Angle );//A��B��C��ѹ�������
        }
        
        if( MeterParameter.Meter_Type == 0x34 )//�������ߵ����������ⷽ��
        {
            if((Real.U[0]>=MeasPara.Un60)&&(Real.U[1]>=MeasPara.Un60)&&(Real.U[2]>=MeasPara.Un60))
            {
              if( AngBuf[4] >= AngBuf[3] ) 
                  AngBuf[6] = (AngBuf[4]-AngBuf[3]);//Iab
              else
                  AngBuf[6] = (3600-(AngBuf[3]-AngBuf[4]));//Iab
              if( AngBuf[5] >= AngBuf[4] ) 
                  AngBuf[7] = (AngBuf[5]-AngBuf[4]);//Ibc
              else
                  AngBuf[7] = (3600-(AngBuf[4]-AngBuf[5]));//Ibc
              if( AngBuf[5] >= AngBuf[3] ) 
                  AngBuf[8] = (AngBuf[5]-AngBuf[3]);//Iac
              else
                  AngBuf[8] = (3600-(AngBuf[3]-AngBuf[5]));//Iac
              AngBuf[7] += AngBuf[6];//Iab��Ibc�н�֮��
              AngBuf[8] += 2400;//Iac�н�+240
              if(((Real.I[0]&0x7FFFFF)>=MeasPara.Ib05)&&((Real.I[1]&0x7FFFFF)>=MeasPara.Ib05)&&((Real.I[2]&0x7FFFFF)>=MeasPara.Ib05))
              {
                  if( AngBuf[8] > AngBuf[7] )//�������жϣ��������AC�нǴ���AB��BC�н�֮��
                      I_InvOrder_Flag = 0;
                  else
                      I_InvOrder_Flag = 1;
              }
              else
                  I_InvOrder_Flag = 0;
            }
            else
                I_InvOrder_Flag = 0;
        }
        
        //��ѹ��������
        RN8302_Read( (unsigned char*)&UFlag, PHASES, 2 );//��ȡ��ѹ������״̬λ
        if( MeterParameter.Meter_Type == 0x34 )//�������ߵ�ѹ�������ⷽ��
        {
            if((Real.U[0]>=MeasPara.Un60)&&(Real.U[1]>=MeasPara.Un60)&&(Real.U[2]>=MeasPara.Un60))
            {
                if( UFlag & 0x0008 )
                    U_InvOrder_Flag = 1;
                else
                    U_InvOrder_Flag = 0;
            }
            else
                U_InvOrder_Flag = 0;
        }
        else//�������ߵ�ѹ�������ⷽ��
        {
            if( (Real.U[0]>=MeasPara.Un60)&&(Real.U[2]>=MeasPara.Un60) )
            {
              if( AngBuf[2] >= AngBuf[0] ) 
                  AngBuf[10] = (AngBuf[2]-AngBuf[0]);//Uca
              else
                  AngBuf[10] = (3600-(AngBuf[0]-AngBuf[2]));//Uca
              //if( UFlag & 0x0008 )
              if( (AngBuf[10] >= 300) && (AngBuf[10] <= 1800) )//������ʱ�н�Ϊ300�ȣ��н�Ϊ30-180��ʱΪ������ȱ�಻�ж�
                  U_InvOrder_Flag = 1;
              else
                  U_InvOrder_Flag = 0;
            }
            else
                U_InvOrder_Flag = 0;
        }
        UFlag = 0xFFFF;
        RN8302_Write( PHASES, (unsigned char*)&UFlag, 2 );//��״̬λ
				
				if( (Real.I[0]<MeasPara.StartI)&&(Real.I[1]<MeasPara.StartI)&&(Real.I[2]<MeasPara.StartI) )
				 power_zero_flag = 1;
    }
}

/************************************************************************
* ������ ��//InstantPowerMeas
* ���� ��//�й����޹������ڵ���˲ʱֵ����
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/03/06 V1.0 yangxing XXXX
**************************************************************************/
void InstantPowerMeas(void)
{
    unsigned char i;

    memset( (unsigned char*)&ChipReg.EnergyBuf[0], 0x00, 36 );
    ChipReg.ActWatthrP = 0;
    ChipReg.ActWatthrN = 0;
    ChipReg.RactVarhr = 0;
    ChipReg.Vahr = 0;
    
    for( i = 0; i < 3; i++ )//A��B��C���й����ܼĴ���ֵ
    {
        if( ( i == 1 )&&(MeterParameter.Meter_Type == 0x33) )
            continue;
        RN8302_Read( (unsigned char*)&ChipReg.EnergyBuf[i], (POSEPA+i), 3 );//���������й�����
        RN8302_Read( (unsigned char*)&ChipReg.EnergyBuf[3+i], (NEGEPA+i), 3 );//���෴���й�����
        RN8302_Read( (unsigned char*)&ChipReg.EnergyBuf[6+i], (EQA+i), 3 );//�����޹�����

        if( ChipReg.EnergyBuf[i] > Factor.EnergyUp )//�й������������
            ChipReg.EnergyBuf[i] = 0;

        if( ChipReg.EnergyBuf[3+i] > Factor.EnergyUp )//�й������������
            ChipReg.EnergyBuf[3+i] = 0;
        
        if( ChipReg.EnergyBuf[6+i] > Factor.EnergyUp )//�޹���������
            ChipReg.EnergyBuf[6+i] = 0;
        
        PowerValue.ActPowerPp[i] += ChipReg.EnergyBuf[i];//���������й�����
        PowerValue.ActPowerPn[i] += ChipReg.EnergyBuf[3+i];//���෴���й�����
        PowerValue.RactPowerPp[i] += ChipReg.EnergyBuf[6+i];//�����޹�����
    }
    RN8302_Read( (unsigned char*)&ChipReg.ActWatthrP, POSEPT, 3 );//�����������й�����
    RN8302_Read( (unsigned char*)&ChipReg.ActWatthrN, NEGEPT, 3 );//���෴�����й�����
    RN8302_Read( (unsigned char*)&ChipReg.RactVarhr, EQT, 3 );//�������޹��������޷��ţ������������Ϊ��ʱ���ڲ��Զ�����˲���ת����
    RN8302_Read( (unsigned char*)&ChipReg.Vahr, ESTV, 3 );//������������
    
    if( ChipReg.ActWatthrP > Factor.EnergyUp )
        ChipReg.ActWatthrP = 0;
    if( ChipReg.ActWatthrN > Factor.EnergyUp )
        ChipReg.ActWatthrN = 0;
    if( ChipReg.RactVarhr > Factor.EnergyUp )
        ChipReg.RactVarhr = 0;
    if( ChipReg.Vahr > Factor.EnergyUp )
        ChipReg.Vahr = 0;
    
    PowerValue.ActPowerP += ChipReg.ActWatthrP;//�����������й�����
    PowerValue.ActPowerN += ChipReg.ActWatthrN;//���㷴�����й�����
    PowerValue.RactPower += ChipReg.RactVarhr;//�������޹�����
    
    if( Flag.ECDir & F_ActRevZ )
        PowerValue.VahrPower[1] += ChipReg.Vahr;//�������ڵ���
    else
        PowerValue.VahrPower[0] += ChipReg.Vahr;//�������ڵ���
}

/************************************************************************
* ������ ��//UMesure_Init
* ���� ��//��ѹ������ʼ��
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//0��ѹ������1��ѹ�쳣
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/03/09 V1.0 
**************************************************************************/
unsigned char UMesure_Init(void)
{
    unsigned char Temp = 0;
    unsigned char i;
    unsigned short Adj[3];
    unsigned long Volt = 0;
    
    for( i = 0; i < 3; i++ )
    {
        Adj[i] = 0;
    }
    Clear_Wdt();//�忴�Ź�
    Enable_SPI();//ѡ��SPIͨѶģʽ
    RN8302_Reset();//��λ
    RN8302_BasicPara();//������������
    Temp = 0xE5;//дʹ��
    RN8302_Write( WREN, (unsigned char*)&Temp, 1 );
    
//33    MEM_Read( (unsigned char*)&Adj[0], ADJ_AVgain, 2, Mem_E2PROMA );
//33    MEM_Read( (unsigned char*)&Adj[1], ADJ_BVgain, 2, Mem_E2PROMA );
//33    MEM_Read( (unsigned char*)&Adj[2], ADJ_CVgain, 2, Mem_E2PROMA );

    for(i=0;i<3;i++)
    {
        RN8302_Write( (GSUA+i), (unsigned char*)& Adj[i], 2 );//��ѹ����
    }
    //RN8302_Run();//��������
    Temp = 0xDC;//д����
    RN8302_Write( WREN, (unsigned char*)&Temp, 1 );
    for( i = 0; i < 5; i++ )
    {
        Clear_Wdt();//�忴�Ź�
        Delay_Xms(100);
    }

    Clear_Wdt();//�忴�Ź�
    for( i = 0; i < 3; i++ )//��ȡ��ѹ��Чֵ
    {
        if( ( i == 1 )&&(MeterParameter.Meter_Type == 0x33) )
            continue;
        RN8302_Read( (unsigned char*)&ChipReg.Urms[i], (UA+i), 4 );
        Volt = (unsigned long)(((float)(ChipReg.Urms[i]*Factor.Ku))*1000);
        if( (Volt > Factor.DispUnDown) && (Volt < Factor.DispUnUp) )
        {
            HEX4_BCD4((unsigned char*)&RealAdj.UAdj[i],Volt);//У���ѹ
            Real.U[i] = (unsigned short)(RealAdj.UAdj[i] >> 8);//��ʾ��ѹ
        }
        else
        {
            Real.U[i] = 0;
        }
    }
   if( FLAG_EA == 1 )
       return 0;
    else
       return 1;
//		 return 0;
}



