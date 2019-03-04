#include "ex_var.h"
#include "ex_func.h"
#include <string.h>
#include <absacc.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cpu_card.h"

#define	MAXREGION	14		//��ʱ�������ֵ, ����
#define	MAXSIDUANTB	8		//ʱ�α������ֵ, ����
#define	MAXSIDUAN	max_shiduancount//ʱ�������ֵ, ����
#define	MAXHOLIDAY	max_maxholiday	//�������������ֵ, ����
#define	MAXFLPRICE	12		//�������������ʵ����
#define SCREENNUM	99		//���طѿر�
//��ַ��Χ:PROGRAMSTA1~PROGRAMEND1~PROGRAMSTA2~PROGRAMEND2
#define PROGRAMSTA1	0x800000	//����ռ�1��ʼ��ַ
#define PROGRAMEND1	0x80FFFF	//����ռ�1������ַ
#define PROGRAMSTA2	0xFF0000	//����ռ�2��ʼ��ַ
#define PROGRAMEND2	0xFFFFFF	//����ռ�2��ʼ��ַ
#define PROGRAMPROZ	0xFFF000	//���򱣻�����ʼ��ַ(keil���̶���?EC?FACTORY(0xFFF000))

#define	KEYTYPE1	0x97
#define	KEYTYPE2	0x96


extern unsigned char UpdataRefer_ToEe_WithMac(void);
extern unsigned char ReadBinWithMac( uint08 CH, uint08 P1, uint08 P2, uint08 Len );
extern unsigned char Authorization( unsigned char *Buf, unsigned char Len );
extern unsigned char Auth_In_Out( unsigned char INS, unsigned char CH, unsigned char P2, unsigned char *BufA );
extern unsigned char DecreaseWithMac( unsigned char *InBuf );
extern unsigned char CardTxRxFrame( unsigned char CH, uint08 Lc, uint08 Le );
extern void RxTxCommhead( uint08 inc, uint08 com, uint08 P1, uint08 P2, uint08 len);
extern uint16 CalcAbsDay(unsigned char * psTm);
extern uint16 CalcAbsMin(unsigned char * psTm);

void ClrSelfClose( void )
{
	self_close_flag = 0; 
	Self_Cnt = 0;
	Self_Timer3 = 0;
	Self_Timer24 = 0;

	Store_Meterstatus();
}

void LoopExchange( unsigned char *Drc, unsigned char *Src, unsigned char Times, unsigned char Spec )
{
	unsigned char i, k;
	
	for( i=0,k=0; i<Times; i++, k+=Spec ) Exchange( Drc+k, Src+k, Spec );
}

void LoopInverse( unsigned char *Inv, unsigned char Times, unsigned char Spec )
{
	unsigned char i;

	for( i=0; i<Times; i++ ) Inverse( Inv+i*Spec, Spec );
}

uint08 IsBCD( uint08 Value )
{
	if( ((Value&0xF0)<=0x90) && ((Value&0x0F)<=0x09) )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

unsigned char IsMultiBcd( unsigned char *Buf, unsigned char Len )
//return: 0: BCD; 1: no BCD
{
	unsigned char i;
	
	for( i=0; i<Len; i++ )
	{
		if( IsBCD(Buf[i]) != 0x00 )		return 1;
	}
	
	return 0;
}


void Xor_Data( unsigned char *BufA, unsigned char *BufB, unsigned char Num )
{
	unsigned char i;

	for( i=0; i<Num; i++ )
	{
		BufA[i] ^= BufB[i];
	}
}


void Calculate_Program_Cs( unsigned char *Buf )//��������ۼӺ�	NNNNNNNN	4
{
//	unsigned long int ecode *Code_Flash_Char;
	union B32_B08 Temp32;
		
	Temp32.B32 = 0;
	
	if( progenable )	//����ģʽ�Ż����
	{
//		Clear_Wdt();
//		for( Code_Flash_Char=0x800000; Code_Flash_Char<=0x80ffff; Code_Flash_Char++ )	//4M��ִ��ʱ��Լ280ms
//		{
//			Temp32.B32 += *Code_Flash_Char;
//			if( Code_Flash_Char == 0x80fffc ) break;
//		}
//		
//		for( Code_Flash_Char=0xff0000; Code_Flash_Char<=0xffffff; Code_Flash_Char++ )	//4M��ִ��ʱ��Լ280ms
//		{
//			Temp32.B32 += *Code_Flash_Char;
//			if( Code_Flash_Char == 0xfffffc ) break;
//		}
		Clear_Wdt();
	}
	
	Hex2Bcd( Temp32.B08, Buf, 4 );	//ת����bcd��,������ʾ
}


//Offset��0~3,�ֱ�Ϊ��բ��ʱ,�������բ��ʱ,������֤ʱ��,�ϱ��Զ���λʱ��
/*BCD��ķ��Ӷ�ʱ��ת����hex���붨ʱ����
offset��
		0��ͨѶ��բ��ʱ			2�ֽڣ�min
		1���������բ��ʱ		2�ֽڣ�min�������ޣ�
		2��������֤ʱ��			1�ֽڣ�min
		3���ϱ��Զ���λʱ��	1�ֽڣ�min
 0xff��Զ�������֤ʱЧ 2�ֽڣ�min
*/
unsigned long int Calculate_Timer(unsigned char Offset)
{
	unsigned char len;
	union B32_B08 Temp32;
	
	Temp32.B08[0] = 0;
	Temp32.B08[1] = 0;
	Temp32.B08[2] = 0;
	Temp32.B08[3] = 0;
	
	if( Offset == 0xff )
	{
		Bcd2Hex( Esam_Para.Identity_Delay.B08, Temp32.B08+2, 2 );	//���������֤ʱЧ
	}
	else
	{
		if( Offset < 2)
		{
			Offset = Offset*2;	//0,2
			len = 2;
		}
		else
		{
			Offset = Offset+2;	//4,5
			len = 1;
		}	
		Bcd2Hex( Mode.Exp_Timer+Offset, Temp32.B08, len );
	}
	
	Temp32.B32 *= 60;	//��ʱʱ�����ת������
	return( Temp32.B32 );
}

	
void Clr_Commu_Relaydata()
{
	Opendelaytimer.B32 = 0;
	relay_commu_open_flag = 0;	//��ͨѶ��բ��־
	relay_commu_disable_close_flag = 0;
	relay_commu_close2_flag = 0;	//�屣������־
}


void Clr_Reportnum( unsigned char i ) //�����ϱ���������
{
	unsigned char Temp08;
	
	for( Temp08=0; Temp08<8; Temp08++ )//�����ϱ�״̬�������ϱ���������
	{
		if( (Report_Data[i]&(1<<Temp08))==0 ) Report_Num[i*8+Temp08] = 0;
	}
}

void Clr_Reportdata( unsigned char i ) //���ϱ�״̬�ֺ���������(���¼���¼��)
{
	unsigned char Temp08;
	
	Temp08 = EventTable[i-ee_clearn_inpage].offset2;
	if( Temp08 != 0xff )
	{	
		Report_Data[Temp08/8] &= ~(1<<(Temp08%8));	//���ϱ�״̬��
		Report_Num[Temp08] = 0;	//����������
	}
}

//��λ�ϱ�״̬�ֺ�����������1
void Set_Reportdata( unsigned char Offset ) 
{
	unsigned char byt,bt,i,j;
	
	if( Offset == 0xff ) return;
	
	byt = Offset/8;
	bt = Offset%8;
	
	//�����ϱ�ģʽ��
	if( (byt==2)&&(bt==7) )
	{
		i = 4; j = 5; //������ʱ���ô�A�����
	}
	else
	{
		if( byt<3 )   i = byt;	//����ģʽ��
		else  i = byt+3;
		j = bt;
	}
	
	if( Mode.Report[i]&(1<<j) )	//�ж�ģʽ���Ƿ���λ
	{
		Report_Data[byt] |= (1<<bt);	//��λ�ϱ�״̬��
		i = byt*8+bt;
		if( ((i>=1)&&(i<=9)) || (i==22) ) Report_Num[i] = 0xff;	//�޷����������ݱ�ʶ���¼���FF
		else if( Report_Num[i] < 0xff ) Report_Num[i]++;	//����������1
	}
}

//׼�������ϱ�״̬�ֺ���������
unsigned char Ready_Reportdata( unsigned char *Buf )
{
	unsigned char i,n;
	
	Reset_Rep_Timer = Calculate_Timer(3);		//������λ�ϱ�״̬�ֶ�ʱ��
	memcpy( Report_Data2, Report_Data, 5 );		//�����ϱ�״̬��
	
	Buf[0] = 0x00;
	memcpy( Buf+1, Report_Data, 3 );
	memset( Buf+4, 0x00, 7 );
	Buf[11] = Report_Data[3];
	Buf[12] = Report_Data[4];
	Buf[13] = 0xAA;
	
	//����Ӧ����ϱ�״̬��
	if( Buf[3]&B1000_0000 ) {Buf[3]&=B0111_1111; Buf[9]|=B0010_0000;}//������ʱ���ô�A����� 
		
	for( i=0,n=0; i<40; i++ )
	{
		Buf[14+n] = Report_Num[i];
		if( Report_Num[i] ) n++;
	}
	Buf[14+n] = 0xAA;
	return(15+n);
}


//�������������
void Clr_Zero_Data()
{
	already_setclock_flag = 0;//��㲥Уʱ��־
	hangup_comm_flag = 0;//������־
	Mac_Err_Times = 0;//mac�����������
	Password_Err[0] = 0;//������������
	Password_Err[1] = 0;
}


//type=0��Ƿ��忨����,type=1��������ʱ��
void Clr_Otherdata( unsigned char Type )
{
//	�Ƿ��忨����(4)+��������ʱ��(4)+���Ƿѹ���ۼ�ʱ��(4)+�������ۼ�ʱ��(4)+��ع���ʱ��(4) 
	if( Type > 4 ) return;
	
	Save_Keep_Timer(0);
	
	Keeptime.Times[Type].B32 = 0x00; //��������ʱ��(4)
	memset(Eeprom.Buffer+Type*4, 0x00, 4);
	
	I2C_Write_Eeprom( ee_illegalcardnum_page, ee_illegalcardnum_inpage, Eeprom.Buffer, ee_illegalcardnum_len );
}


unsigned char ClockAddOne( unsigned char *Clock, unsigned char Max, unsigned char Def )
{
	unsigned char Temp08;
	
	Temp08 = Bcd_To_Hex( *Clock ); //ʱ
	if( Temp08 < Max )
	{
		*Clock = Hex_To_Bcd( Temp08+1 ); //
		return 0;
	}
	else
	{
		*Clock = Def;
		return 1;
	}
}

unsigned char Add_Min_Clock( unsigned char *Clock, unsigned char Min )
//Clock: ssmmhhDDMMYY
{
	unsigned char Temp08, MaxDay;
	uint16  Year;
	unsigned char code Days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	Temp08 = Bcd_To_Hex(Clock[1]); //mm
	if( (Temp08+Min) < 60 )
	{
		Clock[1] = Hex_To_Bcd( Temp08+Min );
	}
	else
	{
		Clock[1] = Hex_To_Bcd( (Temp08+Min)-60 );
		Temp08 = ClockAddOne( Clock+2, 23, 0 );	//hour
		if( Temp08 )
		{
			Temp08 = Bcd_To_Hex(Clock[4]);
			MaxDay = Days[ Temp08-1 ];
			if( Temp08 == 2 )
			{
				Year = Bcd_To_Hex( Clock[5] );
				if( (Year&0x03) == 0 )	MaxDay++;	//����
			}

			Temp08 = ClockAddOne( Clock+3, MaxDay, 1 );	//day
			if( Temp08 )
			{
				Temp08 = ClockAddOne( Clock+4, 12, 1 );	//month
				if( Temp08 ) ClockAddOne( Clock+5, 99, 0 );	//year
			}
		}
	}
	
	return 0;
}

unsigned char Get_RTCTop_Proc( short Temp16 )	//�����Уֵ����
{
	unsigned char i;
	float Rtcadj;
		
	Rtcadj = Temp16/131.76 + Rtcadj_Offset;		//0.0864*1.525*1000
	
	if( Rtcadj >= 0 )
	{
		i = Rtcadj+0.5;
		return i;
	}
	else
	{
		i = (Rtcadj-0.5)*(-1.0);
		return (i|0x80);
	}
}

//�����ж�
unsigned char Veri_Psw( uint08 *Buf )
//Buf[0]:Ȩ��[1]~[3]:��������
//����ֵ: 0:��ȷ; 1/2:����
{
	unsigned char i, I2C_Buf[6];

	i = Buf[0];
	
	if( (i != 3) && (i != 4) ) return (1);
	
	i -= 3; //0,1
	
	if( Password_Err[i] >= Defpassworderrcnt ) return (1);
	
	ReadE2WithBackup( ee_password_page, ee_password_inpage+i*6, I2C_Buf, ee_password_lenth );

	if( Cmp_Bcd_Inv( I2C_Buf, Buf, 4 ) == 0 )	//0-A=B; 1:A>B; 2:A<B
	{
		Password_Err[i] = 0;
		return (0);
	}
	else
	{
		Password_Err[i]++;
		return (1);
	}
}


//��ͬ�Ķ����Ӧ�̶���ģʽ�֣�����ģʽ���ǿ����õģ��û��ڶ���ģʽ����δѡ����������ͣ�
//���೭��ʱӦ�����쳣Ӧ�𣬴�����Ϣ��Bit1 λ�á�1�������������󡱡�
//���ݿ鳭��������ʱ������������AAH �Թ���
void ReadFreezedRec( uint08 Type, uint08 Sn, uint08 No, uint08 *AckBuf )
//Type: ��¼����; No: �ϼ���; Sn:������
//return: 0:��ȡ�ɹ�; 1:��ȡʧ��
{
	unsigned char k, n, ptrmax, offset, len, style, fill=0xff;
	unsigned char ptr, num, mode;
	uint16 page;
	unsigned char Buf[64];
	
	AckBuf[0] = 0x00;
	
	if( (!BDMODE) && ((Type == 5) || (Type == 7) || (Type == 8) || (Type == 0x0a)) ) return; //Զ��ģʽ

	switch( Type )
	{
		case 0x00:	//��ʱ����, 60��
			ptr = ee_timingf_inpage;
			page = ee_timingfreeze_dl_page;
			num = max_timingfreeze_p;
			mode = Mode.Freeze[1];
			break;

		case 0x01:	//˲ʱ����, 3��
			ptr = ee_immedf_inpage;
			page = ee_realtimefreeze1_page;
			num = 3;
			mode = Mode.Freeze[2];
			break;
		
		case 0x02:	//Լ������, ʱ�����л�, 2��
			ptr = ee_agreesqf_inpage;
			page = ee_appointfreeze1_page;
			num = 2;
			mode = Mode.Freeze[3];
			break;
		
		case 0x03:	//Լ������, ��ʱ�α��л�, 2��
			ptr = ee_agreesdf_inpage;
			page = ee_appointfreeze2_page;
			num = 2;
			mode = Mode.Freeze[3];
			break;

		case 0x04:	//���㶳��: ����ʱ��+�����й��ܵ���+�����й��ܵ���, 254��, ���㶳��ʱ����Ĭ��Ϊ60���ӡ�
			ptr = ee_hourf_inpage;
			page = ee_hourfreezedl_page;
			num = max_hourfreeze_p;
			mode = Mode.Freeze[4];
			break;
			
		case 0x06:	//�ն���, 62��
			ptr = ee_dayf_inpage;
			page = ee_dayfreezedl_page;
			num = max_dayfreeze_p;
			mode = Mode.Freeze[5];
			break;	

		case 0x05:	//Լ������, ���ʵ���л�, 2��
			ptr = ee_agreeflf_inpage;
			page = ee_appointfreeze3_page;
			num = 2;
			mode = Mode.Freeze[3];
			break;
		
		case 0x07:	//Լ������, �����л�, 2��
			ptr = ee_agreejtf_inpage;
			page = ee_appointfreeze4_page;
			num = 2;
			mode = Mode.Freeze[3];
			break;

		case 0x08:	//�ն����ܹ������+ʣ����+͸֧���
			ptr = ee_dayf_inpage;
			num = max_dayfreeze_p;
			break;	

		case 0x09:	//Լ������, ����ģʽ�л�, 2��
			ptr = ee_modeswith_inpage;
			page = ee_modeswith_note_page;
			num = 2;
			mode = Mode.Freeze[3];
			break;
			
		case 0x0A:	//Լ������, ����ʱ���л�, 2��
			ptr = ee_jtsqswith_inpage;
			page = ee_jtsqswith_note_page;
			num = 2;
			mode = Mode.Freeze[3];
			break;
			
		default:
			return;
	}

	if( (No==0) || (No > num) ) return;	//��������
	
	ptrmax = RdRecTimes( ptr );//������Чָ��
	ptr = Read_Event_Point( ptr );
	if( ptr < (No-1) ) ptr += num;
	ptr -= (No-1);

	if( Type == 0x08 )	//�ն����ܹ������+ʣ����+͸֧���
	{
		if( Sn != 0x02 ) return;
		
		if( ptrmax < No )	memset( AckBuf+1, 0x00, 10 );
		else I2C_Read_Eeprom( ee_dayfreezemoney_page+ptr, ee_dayfreezemoney_inpage, AckBuf+1, 10 );

		AckBuf[0] = 10;	//���÷������ݵĳ���
		return;
	}
	
	if( Type == 0x04 ) //���㶳��
	{
		page += (ptr>>2);
		offset = (ptr%4)*13;
		len = ee_hourfreezedl_lenth;
		style = 4;	//ƫ�Ƶ�ַ�ͳ���
	}
	else
	{
		page += ptr*2;
		offset = 0x00;
		len = ee_freezedl_lenth/2;
		style = 20;	//ƫ�Ƶ�ַ�ͳ���
	}

	if( ptrmax < No )	memset( Buf, 0x00, len*2 );
	else 
	{	
		I2C_Read_Eeprom( page, offset, Buf, len );
		if( Type != 0x04 )	I2C_Read_Eeprom( page+1, offset, Buf+len, len );
	}
		
	k = 0x01;
	switch( Sn )
	{
		case 0xFF:
		case 0x00: //����ʱ��:YYMMDDhhmm
			memcpy( AckBuf+k, Buf, 5 );
			k += 5;
			if( Sn == 0x00 ) break;
			AckBuf[k++] = 0xAA;
			
		default: 
			for( n=0; n<2; n++ )
			{
				if( (Sn == (n+1) || (Sn == 0xFF)) )	//�����й��ܵ���, �����й��ܵ���
				{
					if( mode & (n+1) )
					{
						memcpy( AckBuf+k, Buf+5+n*style, style );
						k += style;
					}

					if( Sn == (n+1) ) break;
					AckBuf[k++] = 0xAA;
				}
			}
			
			if( Type == 0x04 ) break;	//���㶳��
			
			//����޹�1~2��������, ��һ~�������޹���������, �����й��������, �����й��������
			if( Sn == 0xFF )
			{
				memset( AckBuf+k, 0xAA, 8 );
				k += 8;
			}
			
			//��������: ���й�����
			if( (Sn == 0x10) || (Sn == 0xFF) )
			{
				if( mode & 0x80 )
				{
					memcpy( AckBuf+k+0, Buf+45, 3 );	//���й�����
					memcpy( AckBuf+k+3, Buf+45, 3 );	//A ���й�����
					if( ptrmax < No ) fill = 0x00;
					memset( AckBuf+k+6, fill, 18 );		//��������
					k += 24;
				}

				if( Sn == 0x10 ) break;
				AckBuf[k++] = 0xAA;
			}
			break;
	}

	AckBuf[0] = k-1;
	return;
}


uint08 Find_EventTable( unsigned char *DI )
{
	unsigned char  i;
	union B16_B08 Temp16;
	
	Temp16.B08[0] = DI[0];
	Temp16.B08[1] = DI[1];
	
	for( i=0; i<24; i++ )
	{
		
		if( Temp16.B16 == EventTable[i].DI21 )	break;
	}

	if( (!BDMODE) && (i > ee_batterlow_inpage-ee_clearn_inpage) ) return 24;	//Զ��ģʽ
	if( i >= 24 ) return 24;
	
	return i;
}


unsigned char BitValue( unsigned char *InBuf, unsigned char Pos )
{
	return ( (InBuf[Pos/8]>>(Pos%8))&1 );
}

unsigned char ReadMaxDemand( unsigned char *DIx, unsigned char *ReturnBuf )
{
	unsigned char result,ptrmax;
	unsigned char xdata Buf[64];
	unsigned int  Page;
	unsigned char Inpage;
	ReturnBuf[0] = 0x00;
	
	if( DIx[0] == 0x00 )	//��ǰ
	{
		Page = ee_p_demand_page;
		Inpage = (DIx[2]-0x01)*42;
	}
	else if( DIx[0] <= 12 )	//1~12������
	{
		ptrmax = RdRecTimes(ee_histroyDM_inpage);//������Чָ��
		result = Read_Event_Point(ee_histroyDM_inpage);
		
		if( result < (DIx[0]-1) ) result += max_histroydm_p;
		result -= (DIx[0]-1);
		
		Page = ee_histroyp_demand_page+result;
		Inpage = (DIx[2]-0x01)*42;
	}
	
	I2C_Read_Eeprom( Page, Inpage, Buf, ee_pdemand_lenth );

	if( DIx[1] <= 4 )	//��, 1~4����
	{
		ReturnBuf[0] = 8;
		memcpy( ReturnBuf+1, Buf+8*DIx[1], 8 );
		if( ptrmax < DIx[0] )	memset( ReturnBuf+1, 0x00, 8 );
	}
	else
	{
		ReturnBuf[0] = 40;
		memcpy( ReturnBuf+1, Buf, 40 );
		if( ptrmax < DIx[0] )	memset( ReturnBuf+1, 0x00, 40 );
	}

	return 0;
}


void Rd_Relay_Buy_Rec( unsigned char *DI, unsigned char *AckBuf )
{
	unsigned char i, k, n, result, inpage, offset=0x00, ptr, len=0, total_len, rec_len, ptrmax, Temp08;
	uint16  page;
	union B16_B08 Temp16;
	unsigned char Buf[64];
	unsigned char code Tab_Buy[] = {5,2,4,4,4,4};
	unsigned char code Tab_Reley[]={6,4,4,4};
	unsigned char code Tab_Maxi[]= {6,4,4,4,4,4,4,4,4,2,3,3,3,2,4,4,4,4,2,3,3,3,2,4,4,4,4,2,3,3,3,2, 
				       				6,4,4,4,4,4,4};
	unsigned char code Tab_PwrDn[]={6,4,4,4,4,4,4,4,4,          4,4,4,4,          4,4,4,4,
									6,4,4,4,4,4,4};
	const uint08 code MaxiOffset1[8] = { 0,  6, 22, 38, 49, 109, 109+6, 109+22 };
	const uint08 code MaxiOffset2[8] = { 0, 12, 12, 20, 28, 6, 30, 30 };
	const uint08 code MaxiLenth[8] = { 6,  8,  0,  8,  2, 6,  8,  0 };
	
	unsigned char code PwrDnOffset1[6] = { 0, 6,22,70,70+6,70+22 };
	unsigned char code PwrDnOffset2[6] = { 0,12,12, 6, 20, 20 };
	unsigned char code PwrDnLen[6]     = { 6, 8, 0, 6, 8,  0 };
	unsigned char code MaxiSup [7] = { B0000_0111, B0010_1110, B0000_0000, B0000_0000, B0000_0111, B0000_0000, B0000_0000 };
	unsigned char code PwrDnSup[5] = { B0000_0111, B0000_0000, B0000_1110, B0000_0000, B0000_0000 };

	AckBuf[0] = 0x00;	//ȱʡ�������ݳ�����0x00

	if( (DI[1] == 0xFF) && (DI[0] == 0xFF) ) return;

	i = Find_EventTable(DI+2);		//�ж��Ƿ��ҵ��Ϸ����ݱ�ʶ
	if( i == 23 )	return;

	page = EventTable[i].page;
	inpage = EventTable[i].offset;
	rec_len = EventTable[i].len;
	ptr = i+ee_clearn_inpage;
	
	switch( ptr )
	{
		case ee_powerdownn_inpage: //�����¼
			total_len = 140;
			len = 140;

			if( DI[1] != 0xFF )
			{
				if( DI[1] == 0 )
				{
					if( DI[0] == 0x0c )//���������ۼ�ʱ��
					{
						Get_Keep_Timer(3, AckBuf);
						return;
					}
				}
				else
				{
					if( (DI[1] > 0x22) || (BitValue(PwrDnSup, DI[1]-1) == 0) ) return;
	
					len = Tab_PwrDn[DI[1]-1];
					offset += Cal_ChkSum( Tab_PwrDn, (DI[1]-1) );
				}
			}
			break;
			
		case ee_maxi_inpage:	//����
			total_len = 179;
			len = 179;

			if( DI[1] != 0xFF )
			{
				if( DI[1] == 0 )
				{
					if( DI[0] == 2 )//�������ۼ�ʱ��
					{
						Get_Keep_Timer(1, AckBuf);
						return;
					}
				}
				else
				{
					if( (DI[1] > 0x31) || (BitValue(MaxiSup, DI[1]-1) == 0) ) return;
	
					len = Tab_Maxi[DI[1]-1];
					offset += Cal_ChkSum( Tab_Maxi, (DI[1]-1) );
				}
			}
			break;

		case ee_batterlow_inpage:	//���Ƿѹ��¼
			if( DI[1] == 0 )
			{
				if( DI[0] == 0 ) //���Ƿѹ���ۼ�ʱ��
				{
					Get_Keep_Timer(2, AckBuf);
					return;
				}
				else if( DI[0] == 1 ) //���Ƿѹ��¼
				{
					total_len = 70;
					len = 70;
				}
			}
			break;

		case ee_buymoneyn_inpage: //�����¼
			total_len = 23;
			len = 23;

			if( DI[1] != 0xFF )
			{
				if( (DI[1]==0x00) || (DI[1]>6) ) return;
				
				if( DI[1] )
				{
					len = Tab_Buy[DI[1]-1];
					offset += Cal_ChkSum( Tab_Buy, (DI[1]-1) );
				}
			}
			break;

		case ee_openrelayn_inpage:	//��բ����
		case ee_closerelayn_inpage:	//��բ����
			total_len = 34;
			len = 34;

			if( DI[1] != 0xFF )
			{
				if( DI[1] > 4 ) return;

				if( DI[1] )
				{
					len = Tab_Reley[DI[1]-1];
					offset += Cal_ChkSum( Tab_Reley, (DI[1]-1) );
				}
			}
			break;

		default:
			return;
	}

	if( len == 0 ) return;

	if( (ptr != ee_batterlow_inpage) && (DI[1] == 0) )
	{
		if( ((ptr == ee_powerdownn_inpage) && (DI[0] == 0)) || //�����¼
			((ptr != ee_powerdownn_inpage) && (DI[0] == 1)))	 //������¼
		{
			Read_Event_Number( AckBuf+1, ptr );
			AckBuf[0] = 3;
		}

		return;		
	}
					
	if( (DI[0]) && (DI[0]<=max_eventrecord_p) )	//��1~10��
	{
		Temp16.B08[0] = DI[0]-1;
		Temp16.B08[1] = DI[0];
	}
	else if( DI[0] == 0xFF )
	{
		Temp16.B08[0] = 0;
		Temp16.B08[1] = max_eventrecord_p;
	}
	else
	{
		return;
	}

	ptrmax = RdRecTimes(ptr);//������Чָ��
	Temp08 = Read_Event_Point( ptr );
	
	k = 0x01;
	for( n=Temp16.B08[0]; n<Temp16.B08[1]; n++ )
	{
		result = Temp08;
		if( result < n ) result += max_eventrecord_p;
		result -= n;

		if( ptrmax < n+1 ) memset( Ic_Comm_TxBuf, 0x00, total_len );
		else
		{
			memset( Ic_Comm_TxBuf, 0xFF, total_len );
			I2C_Read_Eeprom( page+result, inpage, Buf, rec_len );//���洢������ee�����Ч����
			if( ptr == ee_maxi_inpage )	//������������֡����
			{
				for( i=0; i<8; i++ ) memcpy( Ic_Comm_TxBuf+MaxiOffset1[i], Buf+MaxiOffset2[i], MaxiLenth[i] );//����ʱ��
			}
			else if( ptr == ee_powerdownn_inpage )	//������������֡����
			{
				for( i=0; i<6; i++ ) memcpy( Ic_Comm_TxBuf+PwrDnOffset1[i], Buf+PwrDnOffset2[i], PwrDnLen[i] );//����ʱ��
			}
			else if( ptr == ee_batterlow_inpage )	//���Ƿѹ
			{
				memcpy( Ic_Comm_TxBuf+ 0, Buf+0, 14 );//����ʱ��+���������
				memcpy( Ic_Comm_TxBuf+22, Buf+6,  8 );//���������
			}
			else 
			{
				memcpy( Ic_Comm_TxBuf, Buf, rec_len );
			}
		}
		
		memcpy( AckBuf+k, Ic_Comm_TxBuf+offset, len );
		k += len;
	}
	
	AckBuf[0] = k-1;
}


uint08 RdHisEvent( unsigned char *DI, unsigned char SN, uint08 *AckBuf )
//return 0: success; other: failure.
{
	unsigned char  i, k, n, TmpPtr, ptrmax, MaxEvent = max_eventrecord_p;
	unsigned char offset, ptr, len;
	uint16  page, TmpP1, TmpP2;
	unsigned char Buf[64], TmpBuf[5], ReadPtr[16];
	union B32_B08 Temp32;

	AckBuf[0] = 0x00;

	i = Find_EventTable(DI+1);		//�ж��Ƿ��ҵ��Ϸ����ݱ�ʶ
	if( i == 24 )	return 1;

	page = EventTable[i].page;
	offset = EventTable[i].offset;
	len = EventTable[i].len;
	ptr = i+ee_clearn_inpage;

	if( ptr == ee_powerdownn_inpage || 
		ptr == ee_maxi_inpage		||
		ptr == ee_openrelayn_inpage ||
		ptr == ee_closerelayn_inpage||
		ptr == ee_batterlow_inpage	||
		ptr == ee_buymoneyn_inpage ) return 1;	//�Ƿ����ݱ�ʶ
	
	if( len > 0x00 )
	{	
		if( DI[0] == 0x00 )	//�ܴ���
		{
			if( ptr == ee_dmoneyn_inpage )  return 2;	//�˷Ѽ�¼���ܴ���
			
			Read_Event_Number( AckBuf+1, ptr );
			AckBuf[0] = 3;
		}
		else	//�¼���¼
		{
			if( ptr == ee_keysetn_inpage ) MaxEvent = 2;	//��Կ���¼�¼ֻ��2��
			
			if( DI[0] > MaxEvent ) return 4;	//��������
			
			ptrmax = RdRecTimes(ptr);//������Чָ��
			ptr = Read_Event_Point( ptr );
			
			TmpPtr = ptr;	//������һ���¼���¼ָ��
			
			if( ptr < (DI[0]-1) )
			{
				ptr += MaxEvent;
			}
			
			ptr -= (DI[0]-1);
	
			I2C_Read_Eeprom( page+ptr, offset, Buf, len );
			memcpy( AckBuf+1, Buf, len );

			switch( DI[1] )
			{
				case 0x01:	//�������
					AckBuf[0] = 106;	//���÷������ݵĳ���
					memset( AckBuf+19, 0xFF, 88 );
					break;
				case 0x02:	//���������¼
					switch( SN )
					{
						case 0x00:	//����ʱ��, �����ߴ���, ������, ��һ~������
							AckBuf[0] = 58;	//���÷������ݵĳ���
							
							memset( AckBuf+27, 0xFF,  32 );
							break;
						
						case 0x01:	//A,B,C����: ����, ��һ~������
							AckBuf[0] = 145;	//���÷������ݵĳ���
							memset( AckBuf+1, 0xFF, 144 );
							AckBuf[145] = SN;
							break;
						
						default:
							break;
					}
					break;
					
				case 0x05:	//ʱ�α���
				case 0x06:	//ʱ������
					//��(Buf�еڼ������ֽڵ�ֵ)Ϊ"ָ����ָ���ReadPtr�Ķ�Ӧ�ֽ�"��д�뼸��
					I2C_Read_Eeprom( page+TmpPtr, 0, TmpBuf, 5 );	//��ȡ��1����ʱ�α��̼�¼ҳ����ʷ��¼
					
					memset( ReadPtr, 0xFF, 16 );
	
					for( n=0; n<DI[0]; n++ )
					{
						if( TmpPtr%2 )	ReadPtr[ (TmpBuf[TmpPtr/2]&0xF0)>>4 ] = TmpPtr;
						else			ReadPtr[ (TmpBuf[TmpPtr/2]&0x0F)    ] = TmpPtr;
	
						if( TmpPtr ) TmpPtr--;
						else	TmpPtr = MaxEvent-1;
					}

					if( DI[1] == 0x05 )
					{
						TmpP1 = ee_shiduantable1_1_page;
						TmpP2 = ee_shiduanrecord_page;
					}
					else
					{
						TmpP1 = ee_shiqu1_page;
						TmpP2 = ee_shiqurecord_page;
						
						if( SN ) return 0x00;
					}
					
					switch( SN )
					{
						case 0x00:
							if( DI[1] == 0x05 )
							{
								Temp32.B08[0] = 196;//��E2�г����ֽ�����
								Temp32.B08[1] = 0;	//��ʼҳ
								Temp32.B08[2] = 4;	//����ҳ
								Temp32.B08[3] = 0;	//ƫ����
							}
							else
							{
								Temp32.B08[0] = 94;	//��E2�г����ֽ�����
								Temp32.B08[1] = 0;	//��ʼҳ
								Temp32.B08[2] = 1;	//����ҳ
								Temp32.B08[3] = 0;	//ƫ����
							}

							memcpy( AckBuf+1, Buf+5, 10 );	//����ʱ��+�����ߴ���
							k = 10;	//���ػ�������ַָ��
							break;
							
						case 0x01:
							Temp32.B08[0] = 195;//��E2�г����ֽ�����
							Temp32.B08[1] = 4;	//��ʼҳ
							Temp32.B08[2] = 9;	//����ҳ
							Temp32.B08[3] = 18;	//ƫ����
							k = 0;	//���ػ�������ַָ��
							break;

						case 0x02:
							Temp32.B08[0] = 195;//��E2�г����ֽ�����
							Temp32.B08[1] = 9;	//��ʼҳ
							Temp32.B08[2] = 13;	//����ҳ
							Temp32.B08[3] = 3;	//ƫ����
							k = 0;	//���ػ�������ַָ��
							break;
							
						case 0x03:
							Temp32.B08[0] = 96;//��E2�г����ֽ�����
							Temp32.B08[1] = 13;	//��ʼҳ
							Temp32.B08[2] = 15;	//����ҳ
							Temp32.B08[3] = 30;	//ƫ����
							k = 0;	//���ػ�������ַָ��
							break;
							
						default:
							return 0x00;
					}

					AckBuf[0] = Temp32.B08[0];	//���÷������ݵĳ���

				for( n=Temp32.B08[1]; n<=Temp32.B08[2]; n++, k+=len )	//
				{
					if( ReadPtr[n] == 0xFF )
					{
						page = TmpP1+n;
						offset = 0;
					}
					else
					{
						page = TmpP2+ReadPtr[n];
						offset = 15;
					}

					len = 42;	//Ĭ�϶�ȡ�ĳ���
					if( n == Temp32.B08[1] ) //��4ҳ���ֳ����ζ�ȡ����һ֡��ȡ��18�ֽ�, ���ζ�ȡ18�ֽ�֮��
					{
						len    -= Temp32.B08[3];
						offset += Temp32.B08[3];
					}
					
					if( k+len > Temp32.B08[0] ) len = Temp32.B08[0]-k;

					I2C_Read_Eeprom( page, offset, AckBuf+1+k, len );	//��ǰʱ�α�
				}
				break;

				case 0x0D:	//�����
					AckBuf[0] = 60;	//���÷������ݵĳ���
					memset( AckBuf+21, 0xFF, 40 );
					memcpy( AckBuf+37, Buf+20, 8 );
					break;
					
				case 0x10:	//���ݱ��
				switch( SN )
				{
					case 0x00:
						Temp32.B08[0] = 196;//��E2�г����ֽ�����
						Temp32.B08[1] = 0;	//��ʼҳ
						Temp32.B08[2] = 2;	//����ҳ
						Temp32.B08[3] = 0;	//ƫ����
						k = 10;	//���ػ�������ַָ��
						break;
							
					case 0x01:
						Temp32.B08[0] = 195;//��E2�г����ֽ�����
						Temp32.B08[1] = 2;	//��ʼҳ
						Temp32.B08[2] = 5;	//����ҳ
						Temp32.B08[3] = 46;	//ƫ����
						k = 0;	//���ػ�������ַָ��
						break;

					case 0x02:
						Temp32.B08[0] = 179;//��E2�г����ֽ�����
						Temp32.B08[1] = 5;	//��ʼҳ
						Temp32.B08[2] = 7;	//����ҳ
						Temp32.B08[3] = 31;	//ƫ����
						k = 0;	//���ػ�������ַָ��
						break;
							
					default:
						return 0x00;
				}
					
				for( n=Temp32.B08[1]; n<=Temp32.B08[2]; n++ )
				{
					I2C_Read_Eeprom( ee_jtpricerecord2_page+ptr*8+n, Temp32.B08[3], AckBuf+1+k, ee_jtpricerecord2_len-Temp32.B08[3] );
					k += (ee_jtpricerecord2_len-Temp32.B08[3]);
					Temp32.B08[3] = 0x00;
				}

				AckBuf[0] = Temp32.B08[0];	//���÷������ݵĳ���
				break;
					
				default:
					AckBuf[0] = len;	//���÷������ݵĳ���
					break;
			}
			
			if( ptrmax < DI[0] )	memset( AckBuf+1, 0x00, AckBuf[0] );
			if( ((DI[1] == 0x05) || (DI[1] == 0x10)) && SN  )	AckBuf[ ++AckBuf[0] ] = SN;
		}
	}
	
	return 0x00;	//��ȡ�ɹ�
}

//��ϵ������㺯��, ÿ�μ���1��
void CalZhuhe( const unsigned char *ZhuBuf, unsigned char *RtBuf, unsigned char Mode, unsigned char Offset )
//ZhuBufҪ������������ͷ�����ܼ��ƽ�ȵ�����
//RtBuf�Ƿ�����ϵ�����4�ֽڡ�
{
	signed long int Zhen;
	union B32_B08 Temp32;
	
	Zhen = 0x00;

	switch( Mode&B0000_0011 )
	{
		case 0x01:
		case 0x02:
			Bcd2Hex( (uint08*)ZhuBuf, Temp32.B08, 4 );
			if( (Mode&B0000_0011) == 0x01 )
			{
				Zhen += Temp32.B32;
			}
			else
			{
				Zhen -= Temp32.B32;
			}
			break;
			
		default:
			break;
	}

	switch( Mode&B0000_1100 )
	{
		case 0x04:
		case 0x08:
			if( Offset == 0 )
			{
				Bcd2Hex( (uint08*)ZhuBuf+20, Temp32.B08, 4 );
			}
			else
			{
				Bcd2Hex( (uint08*)ZhuBuf+4, Temp32.B08, 4 );
			}
			if( (Mode&B0000_1100) == 0x04 )
			{
				Zhen += Temp32.B32;
			}
			else
			{
				Zhen -= Temp32.B32;
			}
			break;
			
		default:
			break;
	}

	Temp32.B32 = labs(Zhen);
	if( Temp32.B32 > 79999999 )	//���: ����й�������޹�����ֵ��Χ��Ϊ��0.00��799999.99
	{
		Temp32.B32 -= 80000000;	//�Դˣ�Ҫ���ڵ��Ｋ��ֵʱ���й��㴦��
		if( Temp32.B32 > 79999999 )	Temp32.B32 -= 80000000;
	}

	Hex2Bcd( Temp32.B08, RtBuf, 4 );

	if( Zhen < 0x00 )	RtBuf[0] |= 0x80;	//��ϵ���<0�����λ��1��
}


unsigned char Cal_Comb( unsigned char DI2, unsigned char DI1, unsigned char DI0, unsigned char *ReturnBuf )
//DI2: 0:���; 1:����; 2:����; 3:����ʱ��
//DI1: 0:��(����ʱ��); 1~4:1~4����; FF:����
//DI0: 0:��ǰ; 1~12:1~12������; FF:����(��13��)
//ReturnBuf: ���ݾ�Ϊ����
//��ǰ����,�õ���,����������Ը��ֽ���ǰ������ee
{
	unsigned char i, k, n, result, Temp08, ptrmax;
	unsigned char Buf[64];
	union B32_B08 Temp32;

	if( DI2 > 0x03 ) return 0x00;
	
	if( (DI1 == 0xFF) && (DI0 == 0xFF) ) return 0x00;	//���߲���ͬʱΪFF��
	
	if( (DI1 > 0x04) && (DI1 != 0xFF) ) return 0x00;
	
	if( DI0 == 0xFF ) 
	{
		Temp32.B08[0] = 0;
		Temp32.B08[1] = 13;
	}
	else
	{
		Temp32.B08[0] = DI0;
		Temp32.B08[1] = DI0+1;
	}
	
	if( DI1 == 0xFF ) 
	{
		Temp32.B08[2] = 0;
		Temp32.B08[3] = 5;
	}
	else
	{
		Temp32.B08[2] = DI1;
		Temp32.B08[3] = DI1+1;
	}
		
	if( DI0 ) //�ǵ�ǰ����
	{
		ptrmax = RdRecTimes(ee_histroyf_inpage);//������Чָ��
		Temp08 = Read_Event_Point(ee_histroyf_inpage);
	}

	k = 0x00;
	for( n=Temp32.B08[0]; n<Temp32.B08[1]; n++ )	//DI0: 0~12,FF
	{
		if( n == 0x00 )	//��ǰ
		{
			if( DI2 != 0x03 )
			{
				memcpy( Buf, Fwdl.Fwdl_T.B08, 20 );
				memcpy( Buf+20, Bkdl.Bkdl_T.B08, 20 );
			}
		}
		else if( n <= 12 )	//1~12������
		{
			result = Temp08;
			if( result < (n-1) ) result += max_histroy_p;
			result -= (n-1);

			if( DI2 != 0x03 )
			{
				I2C_Read_Eeprom( ee_histroy_dl_page+result*2, 0x05, Buf, ee_histroydl_lenth/2-5 );
				I2C_Read_Eeprom( ee_histroy_dl_page+result*2+1, 0x00, Buf+ee_histroydl_lenth/2-5, ee_histroydl_lenth/2 );
				if( ptrmax < n )	memset( Buf, 0x00, ee_histroydl_lenth-5 );
			}
			else	//����ʱ��
			{
				I2C_Read_Eeprom( ee_histroy_dl_page+result*2, 0x01, ReturnBuf+k, 4 );
				if( ptrmax < n )	memset( ReturnBuf+k, 0x00, 4 );
				
				k += 4;
			}
		}
		else return 0x00;

		if( DI2 != 0x03 )
		{
			for( i=Temp32.B08[2]; i<Temp32.B08[3]; i++, k+= 4 )
			{
				if( DI2 == 0x00 )	//���
				{
					if( n == 0x00 )	//��ǰ
						result = Mode.Comp_B;
					else
						result = Buf[42];
					
					CalZhuhe( Buf+i*4, ReturnBuf+k, result, 0 );
					
//					Inverse( ReturnBuf+k, 4 );
				}
				else	//������� or �������
				{
					memcpy( ReturnBuf+k, Buf+(DI2-1)*20+i*4, 4 );
				}
			}
		}
	}

	return (k);
}



void Cal_Comb_Usedl( unsigned char DI2, unsigned char DI0, unsigned char *ReturnBuf )
//DI2: 0:���������õ���;1:���õ���;2:���õ���
//DI0: 0:��ǰ; 1:��1
//ReturnBuf: ���ݾ�Ϊ����
//��ǰ����,�õ���,����������Ը��ֽ���ǰ������ee
{
	unsigned char result;
	unsigned char Buf[ee_yhistroyusedl_lenth+8];
	
	//׼������
	memcpy( Buf, Sclock.Clockbackup+2, 4 );//����ʱ��
	Cal_Comb( 0, 0, 0, Buf+4 );	//���㵱ǰ��ϵ���	
	Inverse( Buf+4, 4 );

	I2C_Read_Eeprom( ee_pre1_histroyusedl_page+DI2, 0, Buf+8, ee_yhistroyusedl_lenth );

	memcpy( ReturnBuf+4, Buf+8*DI0+4, 4);
	memcpy( ReturnBuf+8, Buf+8*DI0+4+8, 4);
	
	//�����õ���
	if( ReturnBuf[4]&0x80 )	result = 0x02;
	else result = 0x01;
	ReturnBuf[4] &= (~0x80);

	if( ReturnBuf[8]&0x80 )	result |= 0x04;
	else result |= 0x08;
	ReturnBuf[8] &= (~0x80);
	
	CalZhuhe( ReturnBuf+4, ReturnBuf+4, result, 0x01 );
	
	//Ӧ������
	Inverse( ReturnBuf+4, 4 );		//ת���ɵ��ֽ���ǰ
	memcpy( ReturnBuf, Buf+8*DI0, 4 );	//����ʱ��

	if( DI2 == 0x02 ) //���õ���
	{
		if( (Price.Ladder_Mode == 0x00) && (DI0 == 0x00) )
		{
			memset( ReturnBuf, 0x00, 8 );//�½���,��ǰ��������Ϊ0	
		}
	}
}


//��̼�¼��˵�����Ӱ��±�̿��ص������̿��أ��ҶԵ��ܱ���б�̲�����
//��¼Ϊһ�α�̼�¼����¼��̵����10 �����ݱ�ʶ����̽���ʱ�̡�
//��̼�¼����ʱ�̣�1.�����˳����ģʽ; 2.�����Чʱ�����; 3.�����֤ʧЧ; 4.�����֤��Чʱ�����
//����4���������Ҫ����New_Program_Rec = 0;
void ProgramRecord( unsigned char *Dix, unsigned char *Opt )
{
	if( first_prog_flag == 0 )	//��Ҫ��������һ����̼�¼
	{
		first_prog_flag = 1;	//�������α�̼�¼
		memcpy( Eeprom.Buffer+6, Opt, 4 );			//�����ߴ���(4)
		memcpy( Eeprom.Buffer+10, Dix, 4 );			//���ݱ�ʶ(4)
		memset( Eeprom.Buffer+14, 0xFF, 36 );			//����Ĳ�0xFF(4*9)
		Event_Record(ee_programn_inpage);
	}
	else
	{
		memcpy( Eeprom.Buffer+50, Dix, 4 );			//���ݱ�ʶ(4)
		Event_Record(ee_programn_inpage|0x80);
	}
}


unsigned char UpdateSysPara( unsigned char Start, unsigned char *Para, unsigned char Len )
{
	unsigned char result, Temp08, Inpage;
	uint16 Page;
	unsigned char Buf[64];
	unsigned char  *ptr;
	
	memset( Buf,0, 64 );	//����RAM\
	
	switch( Start&0xE0 )
	{
		case 0x00:
			Page = ee_displaypara_page;
			Inpage = ee_displaypara_inpage;
			Temp08 = ee_displaypara_lenth;
			ptr =  &Display_Para.S_Number;
			disppm_veri_flag = 1;
			break;
			
		case 0x40:
			Page = ee_esampara_page;
			Inpage = ee_esampara_inpage;
			Temp08 = ee_esampara_lenth;
			ptr = Esam_Para.Alarm_Limit1.B08;
			break;

		case 0x60:
			Page = ee_weekholiday_page;
			Inpage = ee_weekholiday_inpage;
			Temp08 = ee_weekholiday_lenth;
			ptr = &Week_Holiday.Status;
			run_timezone_flag = 1;
			Tariff.Crc16.B16 += 1;
			break;
			
		case 0x80:
			Page = ee_featureword_page;
			Inpage = ee_featureword_inpage;
			Temp08 = ee_featureword_lenth;
			ptr =  Mode.Bps;
			mode_veri_flag = 1;
			break;
			
		case 0xA0:
			Page = ee_eventthreshold_page;
			Inpage = ee_eventthreshold_inpage;
			Temp08 = ee_eventthreshold_lenth;
			ptr =  Threshold;
			mode_veri_flag = 1;
			break;
			
		case 0xC0:
			Page = ee_starttimer_page;
			Inpage = ee_starttimer_inpage;
			Temp08 = ee_starttimer_lenth;
			ptr = Starttime.Shiqu;
			run_timezone_flag = 1;
			break;
			
		default:
			return 1;
	}
	
	//����RAM��E2�еĿͻ����
	ReadE2WithBackup( Page, Inpage, Buf, Temp08 );
	Exchange( Buf+(Start&0x1F), Para, Len );	//���ݵ���
	result = WriteE2WithBackup( Page, Inpage, Buf, Temp08 );
	if( result ) return 1;
	//ReadE2WithBackup( Page, Inpage, Buf, Temp08 );
	memcpy( ptr, Buf, Temp08+2 );	//����RAM
	return 0;
}


unsigned char JudgeSwitch( unsigned char *Buf, unsigned char Inv )
{
	unsigned char i, result;

	if( ChkBufZero(Buf,5)==0 ) return 2;//�Ϸ�����ʱ��(ȫ0x00)
	
	for( i=0; i<5; i++ )
	{
		if( Buf[i] != 0x99 ) break;
	}
	
	if( i < 5 )
	{
		if( Inv ==1 )	Inverse( Buf, 5 );
		result = JudgeClockNoWeek( 1, Buf, 5 );
		if( Inv ==1 )	Inverse( Buf, 5 );
		return (result); //����
	}
	else
	{
		return 3;		//�Ϸ�����ʱ��(ȫ0x99)
	}
}

void JudgeSwitch2( unsigned char offset )//�ж�����ʱ���Ƿ�Ϊȫ99,�������Ĭ�ϵ�һ��
{
	I2C_Read_Eeprom44( ee_changecontrol_page, Eeprom.Buffer, ee_changecontrol_lenth);//�����������ֽ�
	if( Eeprom.Buffer[ offset ] != 0xFF )
	{
		Eeprom.Buffer[ offset ] = 0xFF;
		I2C_Write_Eeprom44( ee_changecontrol_page, Eeprom.Buffer, ee_changecontrol_lenth ); //д���������ֽ�
		Tariff.Crc16.B16 += 1;
		
		//����Լ������
		Freeze_Dl(ee_appointfreeze1_page+4*offset,5+offset);
	}
}

//ʱ��/ʱ�α��̼�¼
uint16 ShiquRecord( unsigned char *InDi, unsigned char *InOpt )
{
	unsigned char i, n, Temp08, Ptr;
	union B16_B08 AddrA, AddrB;
	unsigned char Buf[64];
	
	if( InDi[0] == 0x00 )	//ʱ����
	{
		Ptr = ee_shiqun_inpage;
		AddrA.B16 = ee_shiqurecord_page;
		AddrB.B16 = ee_shiqu1_page;
	}
	else	//��ʱ�α�
	{
		Ptr = ee_shiduann_inpage;
		AddrA.B16 = ee_shiduanrecord_page;
		AddrB.B16 = ee_shiduantable1_1_page;
	}
		
	//��ʷ��¼(5,ÿ�ֽڴ���2��)+����ʱ�̣�6��+�����ߴ��루4��+ ʱ������ǰ�ڼ��׵�1~14 ʱ�������ݣ�3*14��
	//��ʷ��¼(5,ÿ�ֽڴ���2��)+����ʱ�̣�6��+�����ߴ��루4��+ ʱ�α���ǰ�ڼ��׵ڼ� ��ʱ�α����ݣ�3*14��
	Number_Add1( Ptr );	//ʱ��/ʱ���޸��ܴ���+1(3)
	
	Temp08 = Read_Event_Point( Ptr );
	I2C_Read_Eeprom( AddrA.B16+Temp08, 0x00, Buf, 5 );	//��ʷ��¼(5,ÿ�ֽڴ���2��)
	
	//i��Χʱ��0��1,ʱ��0��15
	if( InDi[2] == 1 )	i = 0;
	else
	{
		if( InDi[0] == 0 )	i = 1;	//ʱ����
		else i = 8;	//��ʱ�α�
	}
	if( InDi[0] ) i += (InDi[0]-1);			//����õ���i�����޸�

	Temp08 = Write_Event_Point( Ptr );	//����ָ��+1

	n = Temp08/2;
	if( Temp08%2 )	//��ʷ��¼(5,ÿ�ֽڴ���2��)
		Buf[n] = (Buf[n]&0x0F)+(i<<4);		//��¼��Ӧ�ֽڵĸ�4λ
	else
		Buf[n] = (Buf[n]&0xF0)+i;			//��¼��Ӧ�ֽڵĵ�4λ
	
	memcpy( Buf+5, Sclock.Clockbackup, 6 );	//����ʱ��(6)
	memcpy( Buf+11, InOpt, 4 );				//�����ߴ���(4)
	I2C_Read_Eeprom( AddrB.B16+i, 0x00, Buf+15, 42 );	//���ǰ ʱ����orʱ�ε�i��(42)
	
	I2C_Write_Eeprom( AddrA.B16+Temp08, 0x00, Buf, ee_shiduanrecord_lenth );	//ʱ����
	
	return (AddrB.B16+i);
}


void FLRecord( unsigned char *InOpt )
{
	//--- ���ʱ�̼�¼ ---
	memcpy( Eeprom.Buffer+6, InOpt, 4 );			//�����ߴ���(4)
	I2C_Read_Eeprom( ee_tariffprice1_page, 0x00, Eeprom.Buffer+10, ee_tariffprice_lenth );
	I2C_Read_Eeprom( ee_tariffprice2_page, 0x00, Eeprom.Buffer+26, ee_tariffprice_lenth );
	LoopInverse( Eeprom.Buffer+10, 8, 4 );
	Event_Record( ee_flpricen_inpage );
	//--- ���ʱ�̼�¼ end---
}


void JTRecord( unsigned char *InOpt )
{
	unsigned char i, Temp08;
	unsigned char Buf[ee_ladder_lenth+2];
	
	//--- ���ݱ�̼�¼ ---
	memcpy( Eeprom.Buffer+6, InOpt, 4 );			//�����ߴ���(4)
	Temp08 = Event_Record( ee_jtpricen_inpage );

	for( i=0; i<8; i++ )
	{
		I2C_Read_Eeprom( ee_ladder1_page+i, 0, Buf, ee_ladder_lenth);
		LoopInverse( Buf, 13, 4 );
		I2C_Write_Eeprom( ee_jtpricerecord2_page+8*Temp08+i, 0x00, Buf, ee_jtpricerecord2_len );
	}
	//--- ���ݱ�̼�¼ end ---
}


unsigned char Set_Comm_Rate( unsigned char Ch, unsigned char Rate )
{
	switch( Ch )
	{
		case 0x01:	//�����ͺ����ڲ�����������	NN  1
			if( Rate == 0x04 )	//1200bps
			{
				return 0;
			}
			break;
						
//		case 0x02:	//�Ӵ�ʽ�����ڲ�����������    NN  1
//			Temp08 = 0x00;
//			break;
			
		case 0x03:	//ͨ�ſ�1 ������������          NN  1
			if( (Rate == 0x04) || (Rate == 0x08) || (Rate == 0x10) || (Rate == 0x20) )	//1200, 2400, 4800, 9600
			{
				Uart[2].Flag |= B0001_0000;	//�����ʸı��־(��Լ������λ)
				return(UpdateSysPara( 0x80+0, &Rate, 1 ));
			}
			break;
			
//		case 0x04:	//ͨ�ſ�2 ������������          NN  1
//			Temp08 = 0x00;
//			break;											

		case 0x05:	//ͨ�ſ�3 ������������          NN  1
			if( (Rate == 0x04) || (Rate == 0x08) || (Rate == 0x10) || (Rate == 0x20) )	//1200, 2400, 4800, 9600
			{
				Uart[0].Flag |= B0001_0000;	//�����ʸı��־(��Լ������λ)
				return(UpdateSysPara( 0x80+29, &Rate, 1 ));
			}
			break;									

		default:
			break;
	}
	
	return 1;
}


//ͨ����ʾ�����ݱ�ʶ�����ȷ����Ӧ��������
unsigned char GetLcdNo( unsigned char *LcdSn )
{
	unsigned char n, Temp08, result, Sn;
	union B32_B08 Temp32;
	
	memcpy( Temp32.B08, LcdSn, 4 );
	Sn = LcdSn[4];

	//------ ��� (��ǰ����1~��12������),��Ŵ�1��ʼ ------
	//------ ���� (��ǰ����1~��12������),��Ŵ�66��ʼ ------
	//------ ���� (��ǰ����1~��12������),��Ŵ�131��ʼ ------
	if( (Temp32.B08[3] == 0x00) 
	 && (Temp32.B08[2] <= 0x02) 
	 && (Temp32.B08[1] <= 0x04) 
	 && (Temp32.B08[0] <= 0x0C) )
	{
		Temp08 = 0;		//��������0
		result = (Temp32.B08[2]*65+1)+Temp32.B08[1]+Temp32.B08[0]*5;
	}	
	//------ ������ʾ��Ŀ����Ŵ�200��ʼ--------
	else
	{
		Exchange( Temp32.B08, LcdSn, 4 );
		for( n=0; n<disp_num; n++ )
		{
			if( Temp32.B32 == disp_table[n] ) break;
		}
		
		if( n >= disp_num ) return 0x00;	//��ʾ��Ŀ����
			
		if( (!BDMODE) && ((n>=disp_num-6)||(n==15)) && (!progenable) ) return 0x00;	//Զ��ģʽ,��֧�ֱ�����ʾ��Ŀ(����ģʽ֧��),����Զ�̲�֧�ֿͻ������ʾ

		switch( n )	//ȷ��������
		{
			case 15:	//����
			case 17:	//���
			case 19:	//��ַ
			case 23:	//������ں�ʱ��
			case 25:	//�������ں�ʱ��
				Temp08 = 1;		//��������1
				break;
				
			case 0:	//ȫ��
				if( Sn != 0xFF ) 	return 0x00;	//��ʾ��Ŀ����

				Sn = 0x00;
				Temp08 = 0;		//��������0
				break;
				
			default:
				Temp08 = 0;		//��������0
				break;
		}
		
		result = 200+n+Sn;		//��ʾ���ΪHEX
	}

	if( Sn > Temp08 )		
		return 0x00;	//��ʾ��Ŵ�
	else
		return (result);
}

unsigned char JudgeEndClock()	//��ֹʱ��Ϸ��Ժ���Ч���ж�
{
	if( JudgeClockNoWeek( 0, Ic_Comm_RxBuf+2, 6 ) || (Cmp_Bcd_Inv( Ic_Comm_RxBuf+2, Sclock.Clockbackup, 6 )==2) )return 0x04; //����
	else return 0x00; //�ɹ�
}

unsigned char Decrypt( unsigned char Ch, unsigned char File, unsigned char Len )
{
	unsigned char result, TmpLen;

	if( Ch < 3 )
	{
		if( Len != 0x02 )	
		{
			TmpLen = 16;
			memcpy( Uart[Ch].Buf+22+16, Uart[Ch].Buf+18,  4 );
		}
		else TmpLen = Uart[Ch].Buf[9]-12-4;
	
		result = Update_ESAM_Bin_Mac( File, 0x00, Uart[Ch].Buf+22, TmpLen );
	}
	else
	{
		result = WriteBinWithMac( ESAM, File, 0x00, 16+4 );
	}
	if( result ) return IC_MAC_ERR;	//

	result = ReadBinFile( ESAM, File, 0x00, Len );
	if( result ) return IC_ESAM_RDWR_ERR;
	
	if( Len == 0x02 )
	{
		result = ReadBinFile( ESAM, File, 0x01, Ic_Comm_RxBuf[1]+1 );
		if( result ) return IC_ESAM_RDWR_ERR;
	}
	else
	{
		//1A00 091104100301 9000
		Inverse( Ic_Comm_RxBuf+2, 6 );
		
		result = JudgeEndClock();	//�жϽ���ʱ��
		if( result ) return IC_FORMAT_ERR; //����
	}

	return 0x00;
}


void CommSetRtc_Adj()
{	
	unsigned char Buf[8];
	
	memcpy( Buf, Sclock.Clockbackup, 6 );	//����Уʱǰʱ��
	
	Read_Rtc( Sclock.Clockmain );//��ʱ��
	Syn_Clock();	//ͬ��ʱ��
	
	if( Cmp_Bcd_Inv( Buf, Sclock.Clockbackup, 6 ) == 1 )
	{
		ClearPloadRec( Sclock.Clockbackup+1 );
	}
	
	run_timezone_flag = 1;
	Tariff.Crc16.B16 += 1;
	RefreshPrice();
	run_readrtc_flag = 1;
	
	Wr_Powerdown_Data(0);//д����������ݵ�����ee
	Wr_Pretime_Dayfreeze();	//д��һ���ն���ʱ��
}


unsigned char CommSetRtc( unsigned char Ch, unsigned char Start, unsigned char Len )
{
	unsigned char result;
	unsigned char Buf[8];
	
	result = Judge_Clock( Start, &Uart[Ch].Buf[22], Len ); //�ж������Ƿ�Ϸ�
	if( result != 0 ) return 1; //����ֵ����
	
	if( clock_error )
	{
		Buf[0] = 0x00;	//ss
		Buf[1] = 0x00;	//mm
		Buf[2] = 0x00;	//hh
		Buf[3] = 0x05;	//WW
		Buf[4] = 0x01;	//DD
		Buf[5] = 0x01;	//MM
		Buf[6] = 0x10;	//YY
	}
	else
	{
		Read_Rtc( Buf );//��ʱ��
	}
	
	memcpy( Buf+Start, Uart[Ch].Buf+22, Len );
	
	Buf[3] = (CalcAbsDay(Buf+2)+6)%7;	//������,��ʽΪ��+��+ʱ+��+��+��+��,��ʼ��ַ��,���տ�ʼ����

	result = Set_Rtc( 0, Buf, 7 ); //
	if( result != 0 ) return 2; //����RTCʧ��

	memcpy( Eeprom.Buffer, Uart[Ch].Buf+18, 4 );		//�����ߴ���(4)
	memcpy( Eeprom.Buffer+4, Sclock.Clockbackup, 6 );	//Уʱǰʱ��(6)
	
	CommSetRtc_Adj();	//����ʱ�ӵ����������
	
	memcpy( Eeprom.Buffer+10, Sclock.Clockbackup, 6 );	//Уʱ��ʱ��(6)
	Event_Record( ee_setclockn_inpage );
	
	if( Cmp_Data( Eeprom.Buffer+7, Sclock.Clockbackup+3, 3 ) != 3 )	//���ڷ����仯
	{
		Clr_Zero_Data();	//���������������
//		Find_Card = 0x00;	//����Ѱ��ģʽ
	}
	
	return 0;
}



unsigned char Judge_Min_Space( unsigned char *bufA )
{				
	unsigned char result,Temp08;
	unsigned char Buf[16];
	
	memcpy( Buf+0, bufA, 6 ); //��Ҫ�жϵ�ʱ��
	memcpy( Buf+6, Sclock.Clockbackup, 6 ); //��ǰʱ��
	
	Temp08 = Cmp_Bcd_Inv( Buf+0, Buf+6, 6 ); 
	switch( Temp08 )
	{
		case 0x01: //ʱ�� > ��ǰʱ��
			Add_Min_Clock( Buf+6, 5 ); //�Ե�ǰʱ������5���Ӵ���
			break;
			
		case 0x02: //ʱ�� < ��ǰʱ��
			Add_Min_Clock( Buf+0, 5 ); //��ʱ������5���Ӵ���
			break;
			
		default:
			break;
	}
	
	if( Temp08 )
	{
		result = Cmp_Bcd_Inv( Buf+0, Buf+6, 6 );
		if( result == Temp08 ) //����5min����
		{
			return 1;	//����5min����
		}
	}
	
	return 0;
}


unsigned char  Cal_ChkSum(unsigned char *Buf,unsigned char Len)
{
	unsigned char i,csdata=0;
	
	for(i=0;i<Len;i++)
	{
		csdata+= Buf[i];
	}
	return(csdata);
}

//ģʽ�л�ʱ��ˢ����ʾ�б�
//void ModeChangeDisp(unsigned char NewMode)
//{
//	unsigned char i;
//	unsigned char Buf[64];
//	
//	Clear_Wdt();
//	WriteE2WithBackup( ee_displaypara_page, ee_displaypara_inpage, Def_DisplayPara_table+8*NewMode, ee_displaypara_lenth );
//	memcpy( &Display_Para.S_Number, Def_DisplayPara_table+8*NewMode, ee_displaypara_lenth );
//
//	memset( Buf, 0x01, ee_displayitem_lenth );
//	I2C_Write_Eeprom( ee_loop_displayitem_page+1, 0x00, Buf, ee_displayitem_lenth );
//	I2C_Write_Eeprom( ee_key_displayitem_page+1, 0x00, Buf, ee_displayitem_lenth );
//
//	memcpy( Buf, Def_Autodisp_table+16*NewMode, sizeof(Def_Autodisp_table)/2 );
//	I2C_Write_Eeprom( ee_loop_displayitem_page, 0x00, Buf, ee_displayitem_lenth );
//	memset( Buf, 0x01, ee_displayitem_lenth );
//	memcpy( Buf, Def_Keydisp_table+32*NewMode, sizeof(Def_Keydisp_table)/2 );
//	I2C_Write_Eeprom( ee_key_displayitem_page, 0x00, Buf, ee_displayitem_lenth );
//
//	for( i=0; i<4; i++ ) Item[i].Crc16.B16 += 1;
//	dispitem_veri_flag = 1;
//}

unsigned char ModeChange( unsigned char NewMode )
{
	unsigned char Result,Buf[ee_curyearjs_len];
	
	run_timezone_flag = 1;	//ˢ��ʱ����ʱ�α�, ����ģʽ��λˢ�µ�ۻ�ˢ�¼̵���.
	
	if( UpdateSysPara( 6, &NewMode, 1 ) ) return 1;

	remainder_tick_flag = 0;//����Ƿ��־
	Meter_Status = 0x00;
	Meter_Status1 &= B1000_0001;
	Opendelaytimer.B32 = 0;
	ClrSelfClose();

	if( NewMode == 0 ) //����Ϊ����ģʽ
	{
		RefreshPrice();
		
		Result = Year_Ladder_Judge();
		if( Result == 0x00 )
		{
			Freeze_Usedl(2);//�½���,���õ���ת�������
			Freeze_Dl(ee_histroy_dl_page,ee_histroyf_inpage);
		}

		Clr_Reportdata(ee_clearn_inpage);	//�嵥���ϱ�״̬�ֺ���������(���¼���¼��)
	}
	else if( NewMode == 1 ) //����ΪԶ��ģʽ
	{
		//�ѿص��ܱ��л�ΪԶ��ģʽ��ֹͣʣ����ۼ����л�ΪԶ��ģʽ��ʾ�б�
		//������ر�������բ�ȷѿ�״̬����Ϊ��բ״̬������Զ������բ�������
		Report_Data[1] &= B1111_1101;
		Report_Num[9] = 0x00;  //͸֧״̬
		Clr_Reportdata(ee_flpricen_inpage);	//�嵥���ϱ�״̬�ֺ���������(���¼���¼��)
		Clr_Reportdata(ee_jtpricen_inpage);	//�嵥���ϱ�״̬�ֺ���������(���¼���¼��)
		
		//�嵱ǰ��۲���
		memset(Price.Current_L.B08, 0x00, 16);
		
		//Զ�̱�Ӧ��ʼ�մ����½���
		if( Price.Ladder_Mode == 0x55 )
		{
			//����㵽�½���
			Freeze_Usedl(0);//���õ���ת�������
			Price.Ladder_Mode = 0x00;//�½���ģʽ
			Process_Usedl(1);//���õ�����0��ʼ
			Freeze_Dl(ee_histroy_dl_page,ee_histroyf_inpage);
			
			//��ǰ��1~6���������Ч
			memset( Buf, 0x99, ee_curyearjs_len);	
			WriteE2WithBackup( ee_curyearjs_page, ee_curyearjs_inpage, Buf, ee_curyearjs_len );
		}
		else
		{
			Freeze_Usedl(2);//�½���,���õ���ת�������
			Freeze_Dl(ee_histroy_dl_page,ee_histroyf_inpage);
		}
	}
	else return 1;
	
//	ModeChangeDisp(NewMode);
	
	RdMeterMode();
	
	//�ѿ�ģʽ�л������������բ���ϱ�
//	Result = relay_status_flag;
	Alarmstatus_Judge();		//�����״̬�ж�
	Enable_Closerelay();		//ֱ�ӱպϼ̵���
	
	if( Relay_Mode == 0x55 )//���ü̵���
	{
		if( (!BDMODE) || ((remainder_maxtick_flag == 0) || relay_commu_close_flag) )//����Ԥ�ÿ����ں�բ״̬���ü̵����ɹ���־
		{
			relay_cleardl_flag = 1;	//�����ñ������־
		}
	}
	
	Mt_Alarmstatus_Judge();
//	if( ((Result == 1) && (relay_status_flag == 0))||((Result == 0) && (relay_status_flag == 1)) ) Relay_modechange_flag = 1;//�̵������������ϱ�
//	else Relay_modechange_flag = 0;
	Relay_modechange_flag = 1;
	Store_Meterstatus();
	
	return 0;
}

unsigned char Yc_Auth(unsigned char Ch, unsigned char *OutBuf)
{
	if( Clear_Rand_Gene() ) return 1;	//Esam Reset

	LoopInverse( Uart[Ch].Buf+18, 3, 8 ); //����1+�����1+��ɢ����

	if( Uart[Ch].Buf[34] || Uart[Ch].Buf[35] ) return 1;
	if( Cmp_Meter_Id(Uart[Ch].Buf+36) ) return 1;

	if( Authorization( Uart[Ch].Buf+18, 4 ) ) return 8; //�����֤ʧ��

	memcpy( PcRand,  Uart[Ch].Buf+26, 8 );	//�����, 8bytes 
	memcpy( PcRst8B, Uart[Ch].Buf+34, 8 );	//��ɢ����, 8bytes

	first_prog_flag = 0;	//���״α�̱�־

	//������Ϣ: �����2+ESAM���к�
	Exchange( OutBuf+1, EsamRand, 4 );	//�����2, 4bytes
	Exchange( OutBuf+5, EsamRst8B, 8 );	//ESAM���к�, 8bytes
	
	CardType = 0;
	UpdataFlag = 0;
	UserCardFlag = 0xff;
	return 0;
}

//��֤ͨ���������Ķ����ָ��ִ����Ӧ����
/*Ch:
	0x14���û���Զ��ģʽ�µĺ�բ����޲����ߴ��룬�����
	0x24~0x26��Զ���ն˵ĺ�բ����в����ߴ��룬�����
	0x20~0x22��96��Զ�̿��Ƽ̵�������в����ߴ��룬�����
	0x00~0x02��03��Զ�̿��Ƽ̵�������в����ߴ��룬�޽���
*/
unsigned char Relay_Ctrl(unsigned char Ch)
{
	unsigned char Temp08;
	
	if(Ch >= 0x10) //��03�����룬��������Ҫ����
	{
		Temp08 = Decrypt( Ch&0x0f, 0x87, 0x08 );
		if( Temp08 ) return(Temp08);
	}
	
	//��֤ͨ���������Ķ����ָ��ִ����Ӧ����
	Temp08 = 0x00;
	switch( Ic_Comm_RxBuf[0] )	//N1
	{
		case 0x1A:	//������բ
			if(relay_commu_close_flag == 1) {Temp08 = 0x04;break;}
			relay_commu_open_flag = 1;
			
			if( relay_status_flag || progenable )
			{
				Opendelaytimer.B32 = 0;	//�Ѿ���բ�˾Ͳ�Ҫ�ٵ���ʱ
			}
			else Opendelaytimer.B32 = Calculate_Timer(0);
			
			if( over_xxx_flag && (CurrentTimer.B32 || (relay_status_flag==0)) ) CurrentTimer.B32 = Calculate_Timer(1); //���ڴ������ʱ��բ����������
			
			relay_commu_disable_close_flag = 0;
			relay_commu_close2_flag = 0;	//�屣������־
					
			break;

		case 0x1B:	//�����բ����
			if(relay_commu_close_flag == 1) break;
			if(relay_commu_open_flag == 0)	break;

			Clr_Commu_Relaydata();
			if(Relay_Mode != 0x55)
			{
				if( relay_status_flag == 1 )
				{
					if( (!BDMODE) || ((remainder_maxtick_flag == 0) && (charge_relay_open_flag == 0)) )
					{
						relay_commu_disable_close_flag = 1;
					}
				}
			}
			break;
			
		case 0x1C:	//ֱ�Ӻ�բ��
			Clr_Commu_Relaydata();
			Enable_Closerelay();
			break;

		case 0x2A:	//��������
			relay_commu_alarm_flag = 1;
			Temp08 = 0xff;
			break;

		case 0x2B:	//�����������
			relay_commu_alarm_flag = 0;
			Temp08 = 0xff;
			break;

		case 0x3A:	//�����磬
			relay_commu_close_flag = 1;
			Clr_Commu_Relaydata();
			if(Relay_Mode != 0x55)//���ü̵���
			{
				if( relay_status_flag == 1 )
				{
					relay_commu_disable_close_flag = 1;//�忨���ܱպϼ̵���
				}
			}
			break;

		case 0x3B:	//����������
			if( self_close_flag && (!BDMODE) ) {Temp08 = 0x04;break;}
			if( (Relay_Mode != 0x55) && relay_commu_close_flag && (BDMODE&&( charge_relay_open_flag || remainder_maxtick_flag)) )//���ñ�
			{
				relay_commu_disable_close_flag = 0;//������,��������������͸֧����բ
			}
			
			if( relay_commu_close_flag ) relay_commu_close2_flag = 1;//�ñ�������־
			relay_commu_close_flag = 0;
			Temp08 = 0xff;
			break;

		default:
			Temp08 = 0x04;
			break;
	}

	if( Temp08 == 0x04 ) return(IC_FORMAT_ERR);
	else
	{
		if( (Temp08 != 0xff) && (Ch != 0x14) ) I2C_Write_Eeprom(ee_operatorcode_page, ee_operatorcode_inpage, &Uart[Ch&0x03].Buf[14], 4);//�����ߴ���д����ee
		
		if( (Temp08 != 0xff) && ((Ch&0x0f) == 0x00) ) Zb_Relayctrl_Timer = Relaytimer_Delay_ZB;//�ز�����բ��ʱ
		
		if( (!BDMODE) && (SELFCLOSE == 0x55) )	//Զ����բ��ֱ�Ӻ�բ������ʱ��ȡ���Ա�״̬
		{
			if( Temp08 == 0x00 ) ClrSelfClose();
		}
		
		Store_Meterstatus();	//������״̬�ֽ�
		
		return 0;
	}
}

unsigned char Yc_Money_In( unsigned char Type )
{
	unsigned char result;
					
	result = OpenAddMoney( Type|0x80 );
	if( result ) return (result);
	
	Alarmstatus_Judge();			//�����״̬�ж�
	if(Relay_Mode != 0x55)//���ü̵���
	{
		if( (relay_status_flag == 0) || relay_commu_open_flag )
		{
			Enable_Closerelay();		//�忨�պϼ̵���
		}
		else if( Type&B0001_0000 )//�����ն�ֱ�Ӻ�բ,Զ�̳�ֵ�ֶ���բ
		{
			Enable_Closerelay();		//�忨�պϼ̵���
		}
		else if( (remainder_maxtick_flag == 0)&&(charge_relay_open_flag == 0) )
		{
			relay_commu_disable_close_flag = 1;//Զ�̳�ֵ����Ȼ��բ
			Store_Meterstatus();	//������״̬�ֽ�
		}
	}
	else	Enable_Closerelay();		//ֱ�ӱպϼ̵���
	
	return 0;
}


unsigned char Yc_Preset_Pro(void)
{
	unsigned char result;

	LoopInverse( Buy_Money_Mac[0].B08, 2, 4 ); //��ȡǮ���������+MAC
	LoopInverse( Buy_Times_Mac[0].B08, 2, 4 ); //��ȡǮ���������+MAC
	
	if( Buy_Times_Mac[0].B32 ) return 0x20; //������������� 0����Ϊ�������
	
	result = Init_Money();
	if( result != 0x00 ) 
	{
		Mac_Err_Proc();	//mac���������1
		return 0x04;
	}
	else
	{
		Alarmstatus_Judge();		//�����״̬�ж�
		Enable_Closerelay();		//�忨�պϼ̵���
		return 0;
	}
}


//��ȡǮ���ļ���
//Type = 1������¼�ļ�
//Type = 2����MAC��ȡǮ���ļ�
unsigned char Get_Money_File(unsigned char Type, unsigned char *OutBuf)
{
	unsigned char i, K;
	
	Moneybag_Process2();//ͨѶ����Ǯ��
		
	for(i=0; i<2; i++)
	{
		K = i*2+1;
		if(Type == 1) { if( ReadRecFile(K) ) return 1; }
		else { if( ReadBinWithMac( ESAM+0x10, K, 0x14, 4+4) ) return 1; }
		LoopExchange( OutBuf+i*(Type*4), Ic_Comm_RxBuf, Type, 4 );
	}

	return 0;
}

//������Ϣ: ��д�ļ�״̬(1)+��д��Ϣ�ļ�(50)+MAC3(4)
unsigned char Terminal_Moneyin_Judge(unsigned char *OutBuf)
{
	unsigned char Temp08=0;
	
	if( (UpdataFlag & B0011_0000) != 0 )
	{
		OutBuf[1] = 0;	//�޷�д��Ϣ
		return 2;
	}
	
	if( CardType == UserCardCommand )//�û���
	{
//		result = Yc_Money_In( UserCardFlag+0x10 );	//��������ͬ��Ƭ
//		if( result == 0 )
		{
			//����Eeprom����
			if( UserCardFlag != 0x01 ) UpdataFlag &= B0111_1010;	//���翨�Ͳ�����ֻ������±�����; 
			UpdataRefer_ToEe_WithMac();	//����Eeprom����

			UpdataRunFile();
				
			ReadBinWithMac( ESAM, 0x86, 0, 50+4 );
		
			OutBuf[1] = 1;	//�з�д��Ϣ
			MacExchange( OutBuf+2, Ic_Comm_RxBuf, 50 );
			Temp08 = 56;
		}
	}
	else
	{
		Yc_Preset_Pro();

		UpdataRefer_ToEe_WithMac();	//����Eeprom����
		
		OutBuf[1] = 0;	//�޷�д��Ϣ
		Temp08 = 1;
	}
	
	terminal_auth_flag = 0;	//���ݲ��������������֤ʧЧ
	Identitytimer.B32 = 0;
	
	return (Temp08);
}

//Զ�������֤/�����ն������֤��Ч�ж�
unsigned char Comm_Auth_Judge( void )
{
	if(identity_prog_flag || terminal_auth_flag) return 1;
	else return 0;
}

void MacExchange( unsigned char *Drc, unsigned char *Src, unsigned char Len )
{
	Exchange( Drc, Src, Len );
	Exchange( Drc+Len, Src+Len, 4 );
}

void Dmoney_Record(unsigned char *Buf)
{
	Hex2Bcd( Buf, Buy_Money_Bcd, 4 );//�˷ѽ��
	memcpy( Remain_back+16, Money.Remain, 4 );	//�����˷�ǰ ʣ����͹������
	Remainmoney_Process(Buf, 0x55);//ֱ���˷�
	Buy_Record( ee_dmoneyn_inpage );
}

void Uart_Proc( unsigned char Ch )
{
	unsigned char i, k, n=0, Temp08=0, result, Err_DI=0, Auth=0, Remain = 0x00;
	uint16  MeterClear = 0;
	unsigned char OutBuf[256];
	unsigned char Buf[64],ChangeBuf[32];
	unsigned char ecode *Code_Flash_Char;
	unsigned char *Ram_Ptr;
	unsigned char *Ptr;
	union B16_B08 Temp16,TempCap16;
	union B32_B08 Temp32,DI07;
	union B32_B08 OptID;
	unsigned long Temp;
	
	if( FLAG_EA == 0 ) return;//���粻����,����ֱ�ӷ���
	
	Uart[Ch].Flag &= ~(B0000_0100);
	
	Adj_Clk_Timer = Adjclk_timer_delay;	//����������ʱ8Mʱ�ӵ�У����
	
	Ram_Ptr = Uart[Ch].Buf;
	if( Uart[Ch].Buf[8]&B1000_0000 )	//ͨѶbit7=1���������������Ӧ
	{
		Uart[Ch].Overtime_Timer = 2;
		return;
	}

	result=Cal_ChkSum(Uart[Ch].Buf,Uart[Ch].Buf[9]+10);
		
	for( i=0; i<Uart[Ch].Buf[9]; i++ )
	{
		Uart[Ch].Buf[ 10+i ] -= 0x33;
	} 	
	
	if( Uart[Ch].Buf[10+i] != result )
	{
		Uart[Ch].Overtime_Timer = 2;
		return;
	}
	
	memset( Buf, 0xaa, 6 );
	if( (Cmp_Data( Buf, Uart[Ch].Buf+1, 6 ) ==3) &&			//ȫaa���֧��д
	    ((Uart[Ch].Buf[8]==0x14)||(Uart[Ch].Buf[8]==0x16)||(Uart[Ch].Buf[8]==0x1a)||(Uart[Ch].Buf[8]==0x1b)||(Uart[Ch].Buf[8]==0x1c)||(Uart[Ch].Buf[8]==0x1d)) )
	{
		Uart[Ch].Overtime_Timer = 2;
		return;
	}
	
	memcpy( DI07.B08, Uart[Ch].Buf+10 ,4 );
	
	memcpy( OptID.B08, Uart[Ch].Buf+14 ,4 );
	
	if( (Ch == 0x01) && (progenable==0) )	//����ͨ�ſ�
	{
		//������֤��ͨ���������֤ʧЧ��
		//���ܱ�Ӧ��֧�ֶ���š�ͨ�ŵ�ַ�������š���ǰ���ڡ���ǰʱ�䡢
		//��ǰ���ܡ���ǰʣ���
		//������֤��ѯ���������Ϣ�����������������Ϣ������������
		if( IR_EnTimer == 0 )	//������֤ʧЧ
		{
			Temp08 = 0xFF;	//Ĭ�ϲ�������
			
			if( (Uart[Ch].Buf[8] == 0x03) && 
				 ((DI07.B32 == 0xFF038007) || (DI07.B32 == 0xFF030007) || (DI07.B32 == 0xFF028007) || (DI07.B32 == 0xFF028107)) )	//������֤���󡢺�����֤�����ݶԱȡ�״̬��ѯ����
			{	
				Temp08 = 0x00;
			}
			else if( (Uart[Ch].Buf[8] == 0x09) || (Uart[Ch].Buf[8] >= 0x11 && Uart[Ch].Buf[8] <= 0x13) || (Uart[Ch].Buf[8] == 0x1f) )
			{
				Temp08 = 0x00;
			}
			else if( Uart[Ch].Buf[8] == 0x1c )
			{
				Temp08 = 0x00;
			}
			
			if( Temp08 )	//��������
			{
				if( Uart[Ch].Buf[8] == 0x03 ) DI07.B08[3] = 0xff;//����2�ֽڴ���
				else Uart[Ch].Buf[8] |= 0x40; //����1�ֽ�04����
			}
		}
	}
	
	Successled.Timer = 5+1;	//���5s

	k = 0x00;
	OutBuf[0] = 0x00;
	switch( Uart[Ch].Buf[8] ) //cmd
	{
		case 0x03:	//��ȫ��֤
			if( (Uart[Ch].Flag&B0000_0001) ||
			((Uart[Ch].Flag&B0000_0010)&&(DI07.B32!=0xFF018007)&&(DI07.B32!=0xFF028007)&&(DI07.B32!=0xFF028107)&&(DI07.B32!=0xFF038007)) )
			{
				OutBuf[ k++ ] = 0xFF;	//Operation result.
				OutBuf[ k++ ] = 0x01;
				OutBuf[ k++ ] = 0x00;
				break;
			}
			
			//ע3������Ȩ��04����ͨ�����ĵķ�ʽ�������ݴ��䣬��Ҫ����������֤��ͬʱҪ�б�̼���ϡ�
			//ע4������Ȩ��99����ͨ������+MAC�ķ�ʽ�������ݴ��䣬����Ҫ����������֤��Ҳ����Ҫ��̼����ʹ�á�
			//ע5������Ȩ��98����ͨ������+MAC�ķ�ʽ�������ݴ��䣬����Ҫ����������֤��Ҳ����Ҫ��̼����ʹ�á�

			//--------------------------- ��ȫ��֤������Ϣ�� SERR ---------------------------------------------
			//Bit7 | Bit6       | Bit5         | Bit4           | Bit3         | Bit2          | Bit1     | Bit0     |
			//���� | ���糬�ڻ� | ��ֵ�������� | �ͻ���Ų�ƥ�� | �����֤ʧ�� | ESAM ��֤ʧ�� | �ظ���ֵ | �������� |
			//Bit15 Bit14 Bit13 Bit12 Bit11 Bit10 Bit9 Bit8
			//���� ���� ���� ���� ����������������	
			//ע 1��0 ��������Ӧ��������1 ������Ӧ��������
			//ע2���������ѳ�ֵ�����ȳ�ֵָ���г�ֵ����С1 ʱ��Ϊ������ֵ����ֵ��ɺ�����ѳ�ֵ������1����
			//�����ѳ�ֵ�����ͳ�ֵָ���г�ֵ�������ʱ���ó�ֵ����ɣ���ֵ����Ϊ�ظ���ֵ��������������
			//���⣬Ϊ��ֵ��������
			//ע3�������֤ʧ�ܺ������֤��ʱͳ��Ϊ�����֤ʧ�ܣ���Ҫ���½��������֤��

			//68H A0 �� A5 68H 03H L DIO �� DI3 C0 �� C3 N1 �� Nm CS 16H
			switch( DI07.B08[3] )
			{
				case 0x07:	//
					switch( DI07.B08[2] )
					{
						case 0x00:	//�����ָ֤��
							switch( DI07.B08[1] )
							{
								case 0x00:	//�����֤
									switch( DI07.B08[0] )
									{
//										case 0x01:	//����1, 8bytes	[18]~[25]
//										case 0x02:  //�����1, 8bytes	[26]~[33]
//										case 0x03:  //��ɢ����, 8bytes	[34]~[41]
										case 0xFF:  //�����ָ֤��: ����1+�����1+��ɢ����
											//68+���+68+03+20+FF000007+�����ߴ���+����1(8)+�����1(8)+��ɢ����(8)+CS+16
											if( terminal_auth_flag )
											{
												result = 0x01;	//�����ն������֤����
											}
											else result = Yc_Auth(Ch, OutBuf);
											if( result )
											{
												k = 0x00;
												OutBuf[ k++ ] = 0xFF;
												OutBuf[ k++ ] = result;	//�����֤ʧ��
												OutBuf[ k++ ] = 0x00;
												break;
											}
											
											Identitytimer.B32 = Calculate_Timer(0xff);//���������֤ʱЧ
											identity_prog_flag = 1;	//��λ ��ȫ��֤��̱�־
																			
											k = 13;
											break;

										default:
											break;
									}
									break;
									
								case 0x01:	//�����֤ʱЧ����
									switch( DI07.B08[0] )
									{
//										case 0x01:	//�����֤��Чʱ��(����), 2bytes, BCD
//										case 0x02:	//MAC, 4bytes
										case 0xFF:
											//68+���+68+03+0E+FF010007+�����ߴ���+2�ֽ���֤ʱЧʱ��+MAC+CS+16
											//1.��û�����ð�ȫ��֤ʱЧ����ǰ��Ĭ��Ϊ5���ӡ�
										
											//��Կ��Ϣ�ļ�1 0005 ���� �����֤+MAC
											//�����޸����04d68200+LC+��ɢ����+����+MAC��LC Ϊ��ɢ���ӣ�������MAC ���ܳ��ȣ�����1�ֽڵ�ʮ����������
											if( hangup_comm_flag )
											{
												k = 0x00;  
												OutBuf[ k++ ] = 0xFF;	//Operation result.
												OutBuf[ k++ ] = 0x00;
												OutBuf[ k++ ] = 0x01;	//Զ�̰�ȫ��֤��ס״̬λ
												break;
											}
											
											if( !identity_prog_flag )
											{		
												k = 0x00;
												OutBuf[ k++ ] = 0xFF;
												OutBuf[ k++ ] = 0x08;	//�����֤ʧ��
												OutBuf[ k++ ] = 0x00;
												break;
											}
											
											if( (Uart[Ch].Buf[18]==0x00) && (Uart[Ch].Buf[19]== 0x00) )	//��ȫ��֤ʱЧ����д��0����д��0�򷵻�ʧ�ܡ�
											{
												break;
											}
											
											if( IsMultiBcd( Uart[Ch].Buf+18, 2 ) ) break;

											result = Update_ESAM_Bin_Mac( 0x81, 0x2B, &Uart[Ch].Buf[18], 2 );
											if( result != 0x00 ) 
											{
												k = 0x00;
												OutBuf[ k++ ] = 0xFF;
												OutBuf[ k++ ] = 0x04;	//ESAM��֤ʧ��
												OutBuf[ k++ ] = 0x00;
												break;
											}
											
											result = UpdateSysPara( 0x40+27, Uart[Ch].Buf+18, 2 ); //����XRAM, E2PROM
											if( result ) break;
											
											Identitytimer.B32 = Calculate_Timer(0xff);//���������֤ʱЧ

											ProgramRecord( DI07.B08, OptID.B08 );
											
											//��ȷ��68+���+68+83+04+FF010007+CS+16
											k = 1;
											break;
											
										default:
											break;
									}
									break;
									
								case 0x02:	//�����֤ʧЧ
									switch( DI07.B08[0] )
									{
										case 0xFF:
											if( !identity_prog_flag )
											{		
												k = 0x00;
												OutBuf[ k++ ] = 0xFF;
												OutBuf[ k++ ] = 0x08;	//�����֤ʧ��
												OutBuf[ k++ ] = 0x00;
												break;
											}
											
											//�ͻ����
											result = ReadBinFile( ESAM, 0x81, 36, 6 );
											if( result != 0x00 ) break;

											Exchange( OutBuf+1, Ic_Comm_RxBuf, 6 );
											
											result = Get_Money_File(1, OutBuf+7);	//��ȡǮ���ļ�
											if( result != 0x00 ) break;
						
											//��Կ��Ϣ
											GetKey( OutBuf+15 );
										
											//�����֤ʧЧ
											Clear_Rand_Gene();
											
											k = 19;
											break;
											
										default:
											break;
									}
									break;
									
								case 0x03:	//������֤����, ������֤�ɹ���30��������Ч, �����������֤���
									switch( DI07.B08[0] )
									{
										case 0xFF:
											if( Ch &&(progenable==0) ) break; //ֻ�������ͨ�ſ�
											if( identity_ir_flag == 0 ) break; //û�к�������
											
											//�����2����
											//������֤�ɹ���30��������Ч��
											Inverse( Uart[Ch].Buf+18, 8 );	//����2

											result = Auth_In_Out(0x82, ESAM, 0x10, Uart[Ch].Buf+18 );//��Ƭ���ⲿ��֤
											if( result )
											{
												{IR_EnTimer = 0;identity_ir_flag = 0;}
												k = 0x00;
												OutBuf[ k++ ] = 0xFF;
												OutBuf[ k++ ] = 0x04;	//ESAM��֤ʧ��
												OutBuf[ k++ ] = 0x00;
												break;
											}				
											
											IR_EnTimer = Calculate_Timer(2);
											
											k = 1;
											break;
											
										default:
											break;
									}
									break;
									
								case 0x04:	//����д��Ϣ�ļ�
								case 0x05:	//����д��Ϣ�ļ�
									//68+���+68+03+L+FF050007+�����ߴ���+�ļ����+ƫ�Ƶ�ַ+����(����)+MAC+CS+16
									if(DI07.B08[0] != 0xff) break;
									if( Uart[Ch].Buf[9] <= 15 ) break;//���ݳ��ȱ������15
										
									if( hangup_comm_flag )
									{
										k = 0x00;  
										OutBuf[ k++ ] = 0xFF;	//Operation result.
										OutBuf[ k++ ] = 0x00;
										OutBuf[ k++ ] = 0x01;	//Զ�̰�ȫ��֤��ס״̬λ
										break;
									}
									
									if( !identity_prog_flag )
									{		
										k = 0x00;
										OutBuf[ k++ ] = 0xFF;
										OutBuf[ k++ ] = 0x08;	//�����֤ʧ��
										OutBuf[ k++ ] = 0x00;
										break;
									}
																		
									result = SelectFile( ESAM, 0x00, Uart[Ch].Buf[18] );//ѡ���ļ�(���ģ�18��1A,1B,1C; ����:0B,15)
									if(result) break;
										
									result = Update_ESAM_Bin_Mac(Uart[Ch].Buf[20], Uart[Ch].Buf[19], Uart[Ch].Buf+21, Uart[Ch].Buf[9]-15 );
									if(result)
									{
										k = 0x00;
										OutBuf[ k++ ] = 0xFF;
										OutBuf[ k++ ] = 0x04;	//ESAM��֤ʧ��
										OutBuf[ k++ ] = 0x00;
										break;
									}
									
									ProgramRecord( DI07.B08, OptID.B08 );
									
									k = 1;
									break;	
									
								default:
									break;
							}
							break;
						
						case 0x01:	//����/��ֵ/Ǯ����ʼ��
							if( !BDMODE ) break; //Զ��ģʽ
							
							//������, 4bytes.	[18~21]
							//�������, 4bytes.	[22~25]
							//MAC1, 4bytes.		[26~29]
							//�ͻ����, 6bytes.	[30~35]
							//MAC2, 4bytes.		[36~39]
							//��ͻ���ţ�04D682240A+����+MAC
							//���ܱ��ֵ��8432010C0C+������+����+MAC

							//68+���+68+03+1E+FF010107+�����ߴ���+���+����+MAC+����+MAC+CS+16	//����
							//����ʱ������Ҫ��֤�ͻ���ţ�ֱ�ӽ��ͻ����д��ESAM �Ķ�Ӧ�ļ��У��ٽ��г�ֵ������

							//68+���+68+03+1E+FF020107+�����ߴ���+���+����+MAC+����+MAC+CS+16	//��ֵ
							//��ֵʱ���ȱȶԿͻ�����Ƿ���ͬ����ͬ�ٽ��ͻ����д��ESAM ����Ӧ��������MAC У�飬�����֤ͨ���ٽ��г�ֵ������
							if( DI07.B08[0] != 0xFF )
							{
								break;
							}
							
//							if( hangup_comm_flag )
//							{
//								k = 0x00;  
//								OutBuf[ k++ ] = 0xFF;	//Operation result.
//								OutBuf[ k++ ] = 0x00;
//								OutBuf[ k++ ] = 0x01;	//Զ�̰�ȫ��֤��ס״̬λ
//								break;
//							}  
////							
//							if( !identity_prog_flag )
//							{		
//								k = 0x00;
//								OutBuf[ k++ ] = 0xFF;
//								OutBuf[ k++ ] = 0x08;	//�����֤ʧ��
//								OutBuf[ k++ ] = 0x00;
//								break;
//							}
							if( DI07.B08[1] == 0x01) 
							{
								Exchange(&KeyH_Token[0], Uart[Ch].Buf+38, 20);//key token1
		            Exchange(&KeyL_Token[0], Uart[Ch].Buf+58, 20);//key token2
							  						
							}
							if( (DI07.B08[1] == 0x01) || (DI07.B08[1] == 0x02) )
							{
//								if( Key_Status != 1 ) break; //��Կ��������/��ֵ
								Exchange(Charge_Token, Uart[Ch].Buf+18, 20);	
								if( DI07.B08[1] == 0x01) n =78;
							  else  n =38;
								Exchange( &Refers_File_Mac[BACKADDR[1]], Uart[Ch].Buf+n, 6 ); //���ݿͻ����+MAC
								result = Yc_Money_In( DI07.B08[1] );
								if( result )
								{
									k = 0x00;
									OutBuf[ k++ ] = 0xFF;
									OutBuf[ k++ ] = result;	//��������
									OutBuf[ k++ ] = 0x00;
									break;
								}
								
								k = 1;
							}
							else if( DI07.B08[1] == 0x03 )	//Ǯ����ʼ������Ч�ڲ���������Ԥ�ÿ�
							{
								Exchange(Charge_Token, Uart[Ch].Buf+18, 20);
								result=api_Handle_key_sts_code(&Charge_Token[0]);
	              if( result)
								{
									k = 0x00;
									OutBuf[ k++ ] = 0xFF;
									OutBuf[ k++ ] = result;	//ESAMУ��ʧ��
									OutBuf[ k++ ] = 0x00;
								} 
								result = Yc_Preset_Pro();
								if( result )
								{
									k = 0x00;
									OutBuf[ k++ ] = 0xFF;
									OutBuf[ k++ ] = result;	//ESAMУ��ʧ��
									OutBuf[ k++ ] = 0x00;
								}
								else
								{
									//�������,���������¼
									MeterClear = 0x55aa;		//Ǯ����ʼ������
									
									k = 1;
								}
							}
							break;
							
						case 0x03:	//��Կ��������
							if( (DI07.B08[1] != 0x01) || (DI07.B08[0] != 0xFF) )
							{
								break;
							}

							if( !identity_prog_flag )
							{		
								k = 0x00;
								OutBuf[ k++ ] = 0xFF;
								OutBuf[ k++ ] = 0x08;	//�����֤ʧ��
								OutBuf[ k++ ] = 0x00;
								break;
							}
							
							result = UpdateEepromKey( Ch );
							if( result ) //���Ӧ����Ӧ����վӦ������Կ��װ����
							{
//								FailKey();	//������Կʧ�ܴ���
								k = 0x00;
								OutBuf[ k++ ] = 0xFF;
								OutBuf[ k++ ] = result;
								OutBuf[ k++ ] = 0x00;
							}
							else
							{
								MeterClear = 0xaa55;
								
								k = 1;
							}
							break;
							
						case 0x80:	//���ݻس�
							switch( DI07.B08[1] )
							{
								case 0x01:	//
									switch( DI07.B08[0] )
									{
										case 0xFF:	//���ݻس���ʶ, 8�ֽ�,
											//���ص�����: 8+Le+4�ֽ�, [7:0]���ݻس���ʶ; [7+Le:8]���ص�����; [8+Le+3:8+Le]MAC
						
											//���ݻس������ȡ�ļ����ķ�ʽ��
											//81,��ȡʣ����͹��������
											//�س�ʣ���04b2010C11+4 �ֽ������+04d681000A+8 �ֽڷ�ɢ���ӣ�
											//�س����������04b2030C11+4 �ֽ������+04d681040A+8 �ֽڷ�ɢ���ӡ�
											//82��85����ȡ��ӦESAM �������ļ����ݣ�
											//04b082��83��84��85��+ p2(ƫ�Ƶ�ַ)��11+4 �ֽ������1+04d683+00+LC+8 �ֽڷ�ɢ���ӡ�
											//LC ����Ҫ��ȡ���������ݣ�MAC+��ɢ���ӵ��ܳ��ȣ�����1 �ֽڵ�ʮ����������

											if( (!identity_prog_flag)&&(!progenable) )
											{		
												k = 0x00;
												OutBuf[ k++ ] = 0xFF;
												OutBuf[ k++ ] = 0x08;	//�����֤ʧ��
												OutBuf[ k++ ] = 0x00;
												break;
											}
											
											//����Ŀ¼ѡ��ESAM
											if( (Uart[Ch].Buf[25] != 0xDF) || (Uart[Ch].Buf[24] != 0x01) ) break;

//											result = SelectFile( ESAM, 0xDF, 0x01 );
//											if( result != 0x00 ) break;	//

											if( Uart[Ch].Buf[22] != 0x02 )	//��Ǯ���ļ�����������ļ�
											{
												if( Uart[Ch].Buf[18] > 200 ) break;	//645�涨��ȡ�ַ�������200��
												
												//���ͣ�04b082(83,84,86)+p2+11+S4+04D686+P2+LC+8�ֽڷ�ɢ����
												//���أ�61 XX
												//���ͣ�00C0000008
												//���أ�Data+MAC
												
												if( Uart[Ch].Buf[22] == 0x06 ) UpdataRunFile();//���������ļ�
												
												result = ReadBinWithMac( ESAM+0x10, (Uart[Ch].Buf[22]|0x80), Uart[Ch].Buf[20], Uart[Ch].Buf[18]+4 );
												if( result != 0x00 ) break;
												
												MacExchange( OutBuf+9, Ic_Comm_RxBuf, Uart[Ch].Buf[18] );
												
												k = Uart[Ch].Buf[18]+13;
												memcpy( OutBuf+1, Uart[Ch].Buf+18, 8 );
												
											}
											
											else	//��ȡʣ����͹������
											{
												//ʣ����(ESAM ��), 4bytes.
												//���������ESAM �ڣ�, 4bytes.
												result = Get_Money_File(2, OutBuf+9);
												if( result != 0x00 ) break;
	
												//����״̬��Ϣ:
												k = 25;
												memcpy( OutBuf+1, Uart[Ch].Buf+18, 8 );
											}
											break;
											
										default:
											break;
									}
									break;
									
								case 0x02:	//���ݱȶ�
									switch( DI07.B08[0] )
									{
										case 0xFF:
											if( !identity_prog_flag )
											{		
												k = 0x00;
												OutBuf[ k++ ] = 0xFF;
												OutBuf[ k++ ] = 0x08;	//�����֤ʧ��
												OutBuf[ k++ ] = 0x00;
												break;
											}
											
											//�ж�cpu���
											if( Uart[Ch].Buf[18] != 0x00 ) break;
											
											//�жϵ�ַ�Ƿ��쳣��׼������
											for( i=0; i<2; i++ )
											{
												if( i== 0 ) {Temp16.B16 = 16; Ptr = ChangeBuf;}	//�ȶ�����
												else {Temp16.B16 = 256; Ptr = OutBuf;}	//�ȶ�����
												
												Temp32.B32 = 0;
												Exchange( Temp32.B08, Uart[Ch].Buf+20+4*i, 4);	//�ȶ�������ʼ��ַ
												Code_Flash_Char = (unsigned char ecode *)Temp32.B32;
												if( (Temp32.B32 < PROGRAMSTA1) || (Temp32.B32 > PROGRAMEND2) 
													|| ((Temp32.B32 > PROGRAMEND1)&&(Temp32.B32 < PROGRAMSTA2)) 
													|| Uart[Ch].Buf[23] || Uart[Ch].Buf[27] )
												{
													break;		//��ַ�쳣
												}
												else if( (Temp32.B32 < (PROGRAMEND1+1)) && (Temp32.B32+Temp16.B16 > (PROGRAMEND1+1)) )//�Ͽ���ַ����
												{
//												   	memcpy( Ptr, Code_Flash_Char, PROGRAMEND1+1-Temp32.B32 );	//�Ͽ���ַ����
//												  	memcpy( Ptr+(PROGRAMEND1+1-Temp32.B32), (unsigned char ecode *)0xff0000, Temp16.B16-(PROGRAMEND1+1-Temp32.B32) );
												  	memset( Ptr, 0x00, Temp16.B16);	
												  	memcpy( Ptr, Code_Flash_Char, PROGRAMEND1+1-Temp32.B32 );	//����������ַ��0x80
												   	Ptr[PROGRAMEND1+1-Temp32.B32] = 0x80;
												}
												else if( (Temp32.B32+Temp16.B16 > (PROGRAMEND2+1)) )	//������ַ��0x80
												{
												  	memset( Ptr, 0x00, Temp16.B16);
												  	memcpy( Ptr, Code_Flash_Char, PROGRAMEND2+1-Temp32.B32 );
												   	Ptr[PROGRAMEND2+1-Temp32.B32] = 0x80;
												}
												else if( (Temp32.B32 < (PROGRAMPROZ)) && (Temp32.B32+Temp16.B16 > (PROGRAMPROZ)) )	//����������ַ��0x80
												{
												   	memset( Ptr, 0x00, Temp16.B16);
												  	memcpy( Ptr, Code_Flash_Char, PROGRAMPROZ-Temp32.B32 );
												   	Ptr[PROGRAMPROZ-Temp32.B32] = 0x80;
												}
												else
												{
													memcpy( Ptr, Code_Flash_Char, Temp16.B16 );
												}
											}
											
											if( i != 2 )
											{
												k = 0x00;
												OutBuf[ k++ ] = 0xFF;
												OutBuf[ k++ ] = 0x80;	//��ַ�쳣
												OutBuf[ k++ ] = 0x00;
												break;
											}
                									
											//��ROM����,��������
											Xor_Data( ChangeBuf, ChangeBuf+8, 8 );//�õ��ȶ�����
											result = Auth_In_Out(0xfa, ESAM, Uart[Ch].Buf[19], ChangeBuf );//�����ȶ���Կ
											if( result ) break;
											
											for( i=1; i<4; i++ )
											{
												Xor_Data( OutBuf, OutBuf+64*i, 64 );//�õ�data1
											}
											result = Cal_Mac_Pro(0x00, OutBuf, 64); //ESAM����data1(������Ҫ���볤��)
											if( result ) break;
											
											Exchange(OutBuf+1, Ic_Comm_RxBuf, 64);	//����ȡ��,���ؼ�������
											
											k = 65;
											OutBuf[0] = 0x00;
											break;
											
										default:
											break;
									}
									break;

								case 0x03:	//������֤����
									switch( DI07.B08[0] )
									{
										case 0xFF:	//�����1, 8�ֽ�,
											if( Ch &&(progenable==0) ) break; //ֻ�������ͨ�ſ�
										
											//������Ϣ: ���(8)+ESAM���к�(8)+�����1����(8)+�����2(8)
											result = Clear_Rand_Gene(); //Esam Reset
											if( result != 0x00 ) break;	

											Buf[16] = 0x00;
											Buf[17] = 0x00;
											memcpy( Buf+18, Esam_Para.Meter_Id, 6 );
											Exchange( Buf+8, Uart[Ch].Buf+18, 8 );	//�����1
											result = Authorization( Buf, 8 );
											if( result ) break;
											
											//������Ϣ: ��� ESAM���к� �����1���� �����2
											Exchange( OutBuf+1, Esam_Para.Meter_Id, 6 );//���, 6bytes
											Exchange( OutBuf+7, EsamRst8B, 8 );	//ESAM���к�, 8bytes
											Exchange( OutBuf+15, Buf, 8 );	//�����1����, 8bytes
											Exchange( OutBuf+23, EsamRand, 8 );	//�����2, 8bytes
											
											identity_ir_flag = 1;	//�ú��������־

											k = 31;
											break;
											
										default:
											break;
									}
									break;									
									
								default:
									break;
							}
							break;
							
						case 0x81:	//״̬��ѯ
							switch( DI07.B08[1] )
							{
								case 0x02:
									switch( DI07.B08[0] )
									{
										case 0xFF:
											if( !identity_prog_flag )
											{		
												k = 0x00;
												OutBuf[ k++ ] = 0xFF;
												OutBuf[ k++ ] = 0x08;	//�����֤ʧ��
												OutBuf[ k++ ] = 0x00;
												break;
											}
											
											//������Ϣ: ʣ���ESAM �ڣ�+ MAC + ���������ESAM �ڣ�+ MAC + �ͻ���� + ��Կ��Ϣ
											result = Get_Money_File(2, OutBuf+1);
											if( result != 0x00 ) break;
						
											//�ͻ����,	6bytes
											result = ReadBinFile( ESAM, 0x81, 36, 6 );
											if( result != 0x00 ) break;

											Exchange( OutBuf+17, Ic_Comm_RxBuf, 6 );
						
											//��Կ��Ϣ, 4bytes.							
											GetKey( OutBuf+23 );
											
											//����״̬��Ϣ:
											k = 27;
											break;
										
										default:
											break;
									}
									break;

								default:
									break;
							}
							break;
							
						case 0xA0:	//�����ն�
							if( !terminal_auth_flag && (DI07.B08[1] != 0x01) && (DI07.B08[1] != 0x06) )
							{
								k = 3;
								OutBuf[0] = 0xFF;
								OutBuf[1] = 0x08;	//�����֤ʧ��
								OutBuf[2] = 0x00;
								break;
							}

//							DIVSEL	= 0x00;		//ϵͳƵ����ߵ�8M
//							
							switch( DI07.B08[1] )
							{
								case 0x01: //�����ն˺ͱ������֤
									result = Yc_Auth(Ch, OutBuf);//������Ϣ: �����2+ESAM���к�+�ѿ�ģʽ״̬��
									if(result)
									{
										k = 0x00;
										OutBuf[ k++ ] = 0xFF;
										OutBuf[ k++ ] = result;	//�����֤ʧ��
										OutBuf[ k++ ] = 0x00;
										break;
									}
									
									Identitytimer.B32 = Terminal_delay;//�����ն������֤ʱЧ1����
									terminal_auth_flag = 1;	//�����ն������֤��־
									identity_prog_flag = 0;
									
									if((Decimal.Status & B0000_0010) != 0) UpdateMoneytimer = 0;	//���仯����Ǯ��,����ʵʱ����
									
									result = Get_Money_File(1, OutBuf+14);	//��ȡǮ���ļ�
									if( result ) break;

									OutBuf[13] = Display_Para.MeterCtrl;	//�ѿ�ģʽ״̬��, 1byte
									
									k = 22;
									break;
									
								case 0x02: //�����ն��·������ļ�
//								case 0x01:	//��Ƭ���к�, 8bytes	[18]~[25]
//								case 0x02:  //�����1, 4bytes	[26]~[29], �����㷵д��Ϣ�ļ���MAC
//								case 0x03:	//�翨���ͣ�1byte[30]
//								case 0x03:  //������, 4bytes	[31]~[34]
//								case 0x03��	//�������	4bytes	[35]~[38]
//								case 0x04:	//MAC1,MAC2, 8bytes [39]~[46]���û�����MAC2��4�ֽ�00��
//Ԥ�ÿ�
//								case 0x05:	//������Ϣ�ļ�, 32bytes,[47]~[78] ����ȫ��������Ϣ�ļ�����ʼ�뵽�������ȫ������
//								case 0x06:	//MAC3, 4bytes, [79]~[82]���ݲ�����Ϣ�ļ������׷�ʱ�����л�ʱ�䵽��ѹ������������ݽ��м���
//								case 0X07:	//MAC4, 4bytes, [83]~[86]4�ֽ�00
//�û���
//								case 0x05:	//������Ϣ�ļ�, 48bytes,[47]~[91] ����ȫ��������Ϣ�ļ�����ʼ�뵽�������ȫ������
//								case 0x06:	//MAC3, 4bytes, [92]~[95]���ݲ�����Ϣ�ļ������׷�ʱ�����л�ʱ�䵽��ѹ������������ݽ��м���
//								case 0X07:	//MAC4, 4bytes, [96]~[99]���ݲ�����Ϣ�ļ��еĿͻ�������ݽ��м���
									//68+���+68+03+20+FF02A007+�����ߴ���+��Ƭ���к�+�����1(4)+....+MAC3(4)+CS+16
									//���ز�������д�ļ�״̬(1)+��д�ļ�(50)+MAC(4)
									//���������2 ��բ�󣬵��ܱ��յ������ն˷����������ն��·������ļ������ݱ�ʶ��07A002FF��������ʱ�����ܱ��բ��
									Alarmstatus_Judge();			//�����״̬�ж�
									Enable_Closerelay();			//�忨�պϼ̵���
									
									Exchange( PcRand,  Ram_Ptr+26, 4 );	//�����, 4bytes 
									Exchange( PcRst8B, Ram_Ptr+18, 8 );	//��Ƭ���к�, 8bytes
									
									CardType = Ram_Ptr[30];
									if(CardType == UserCardCommand)//�û���
									{
										if( Key_Status != 1 ) break;	//ֻ����˽Կ
										
										Exchange(Buf, Ram_Ptr+56, 6);
										if( Cmp_Meter_Id(Buf) ) break;//��Ų����
										
										UserCardFlag = Ram_Ptr[49];	//�ݴ��û�������
										
										if( BDMODE ) //���ر�
										{
											if( UserCardFlag && (UserCardFlag <4) )//�û�������Ϊ1��2��3
											{
												UpdataFlag = Ram_Ptr[86];	//�������±�־��

												Exchange( CardRst8B, Ram_Ptr+18, 8 );	//��Ƭ���к�
			
												memcpy( Refers_File_Mac+BACKADDR[13], Ram_Ptr+50, 6 ); //���ݿͻ����
												memcpy( Refers_File_Mac+BACKADDR[13]+6, Ram_Ptr+96, 4 ); //MAC4
												memcpy( Buy_Money_Mac, Ram_Ptr+31, 8+4 ); //���ݹ����������MAC1
												
												result = Yc_Money_In(UserCardFlag+0xd0);	//�жϹ�������Ͷڻ����												
												if(result == 0)
												{
													if( (BuyTimesFlag == 2) || (BuyTimesFlag == 0xff) )
													{
														UpdataRunFile();
					
														ReadBinWithMac( ESAM, 0x86, 0, 50+4 );
													
														if(BuyTimesFlag == 2)	//���������>1����дSERR+0xAA+��д��Ϣ
														{
															OutBuf[0] = 0xFF;
															OutBuf[1] = 0x20;	//��ֵ������
															OutBuf[2] = 0x00;
															OutBuf[3] = 0xAA;
															MacExchange( OutBuf+4, Ic_Comm_RxBuf, 50 );
															k = 58;
														}
														else
														{
															OutBuf[1] = 1;	//�з�д��Ϣ
															MacExchange( OutBuf+2, Ic_Comm_RxBuf, 50 );
															k = 56;
														}
														terminal_auth_flag = 0;	//���ݲ��������������֤ʧЧ
														Identitytimer.B32 = 0;
														
														break;
													}
												}
												else
												{
													terminal_auth_flag = 0;	//���ݲ��������������֤ʧЧ
													Identitytimer.B32 = 0;
													
													k = 3;
													OutBuf[0] = 0xFF;
													OutBuf[1] = result;	//��������
													OutBuf[2] = 0x00;
													break;
												}
												
												if(UpdataFlag & B0000_0010)	//�û��������л�ʱ���޸�MAC��֤
												{
													memcpy( Ram_Ptr+77+5, Ram_Ptr+43, 4);//MAC2
													result = Update_ESAM_Bin_Mac( 0x81, 0x0a, Ram_Ptr+77, 5 );//��MAC���²���1
													if(result)
													{
														k = 0x00;
														OutBuf[ k++ ] = 0xFF;
														OutBuf[ k++ ] = 0x04;	//ESAM��֤ʧ��
														OutBuf[ k++ ] = 0x00;
														break;
													}
													//��������
													Exchange( Refers_File_Mac+BACKADDR[4], Ram_Ptr+77, 5 );	//2�׷�ʱ�����л�ʱ��
												}

												memcpy(Ram_Ptr+82-6, Ram_Ptr+92, 4);//MAC3
												Temp08 = 62;	//����ƫ�Ƶ�ַ
											}
											else break;	//�û������ʹ�
										}
										else	//Զ�̱�
										{
											if( UserCardFlag == 0 )//ֻ���պ�բ��������
											{
												k = 2;
												OutBuf[1] = 0;
												break;
											}
											else break;
										}
									}
									else if(CardType == PresetCardCommand)//Ԥ�ÿ�
									{
										if( (Key_Status != 0) || (!BDMODE) ) break;	//ֻ֧�ֹ�Կ������ģʽ(����Ҫ��)
										
										memcpy( Buy_Money_Mac[0].B08, Ram_Ptr+31, 4 ); 	//���ݹ�����
										memcpy( Buy_Money_Mac[1].B08, Ram_Ptr+39, 4 ); 	//����MAC1
										memcpy( Buy_Times_Mac[0].B08, Ram_Ptr+35, 4 ); 	//���ݹ������
										memcpy( Buy_Times_Mac[1].B08, Ram_Ptr+43, 4 ); 	//����MAC2
									
										Exchange( Refers_File_Mac+BACKADDR[4], Ram_Ptr+64, 5 );	//2�׷�ʱ�����л�ʱ��
										
										UpdataFlag = Ram_Ptr[73];
										memcpy(Ram_Ptr+69-6, Ram_Ptr+79, 4);//MAC3
										Temp08 = 49;	//����ƫ�Ƶ�ַ
									}
									else break;	//��Ƭ���ʹ�
															
									if( (UpdataFlag & B1000_0000) && ((CardType == PresetCardCommand) || (UserCardFlag == 0x01)) )	//�����޸�MAC��֤
									{
										result = Update_ESAM_Bin_Mac( 0x81, 0x10, Ram_Ptr+Temp08, 14 );//��MAC���²���1
										if(result)
										{
											k = 0x00;
											OutBuf[ k++ ] = 0xFF;
											OutBuf[ k++ ] = 0x04;	//ESAM��֤ʧ��
											OutBuf[ k++ ] = 0x00;
											break;
										}
										
										//��������
										Refers_File_Mac[BACKADDR[1]] = 0;
										Exchange( Refers_File_Mac+BACKADDR[1]+1, Ram_Ptr+Temp08, 14 );//����1����
										
										ProgramRecord( DI07.B08, OptID.B08 );
									}
									
									//������Ϣ: ��д�ļ�״̬(1)+��д��Ϣ�ļ�(50)+MAC3(4)									
									UpdataFlag &= B1100_1111;
									if( UpdataFlag & B0000_0101 ) UpdataFlag |= B0001_0000;//��λbit4
									if( UpdataFlag & B0000_1010 ) UpdataFlag |= B0010_0000;//��λbit5
									
									k = Terminal_Moneyin_Judge(OutBuf);	//�з�д��Ϣ
									if( k == 1 ) 
									{
										MeterClear = 0x55aa;	//Ԥ�ÿ�����
										k = 2;
									}
									
									break;

								case 0x03: //�����ն��·���ǰ�׵���ļ�
								case 0x04: //�����ն��·������׵���ļ�
									//��ǰ/�����׵���ļ�, 199bytes	[18]~[216]
									//MAC1, 4bytes	[217]~[220], �����㷵д��Ϣ�ļ���MAC
									//68+���+68+03+20+FF03A007+�����ߴ���+��ǰ�׵���ļ�+MAC1(4)+CS+16
									if( ((DI07.B08[1] == 0x03) && ((UpdataFlag & B0000_0101) == 0x00)) ||
									    ((DI07.B08[1] == 0x04) && ((UpdataFlag & B0000_1010) == 0x00)) )
									{
										k = 2;
										OutBuf[1] = 0;
										break;		//������²��������سɹ�
									}
									
									if( (DI07.B08[1] == 0x04) || (UserCardFlag == 0x01) || (CardType == PresetCardCommand) ) //���翨�Ͳ����������µ�ǰ�ף�ֱ�ӷ��سɹ�
									{
										Temp08 = DI07.B08[1]+0x80;
										result = Update_ESAM_Bin_Mac( Temp08, 0, Ram_Ptr+18, 199 );//��MAC���±����׵���ļ�
										if( result ) 
										{
											k = 0x00;
											OutBuf[ k++ ] = 0xFF;
											OutBuf[ k++ ] = 0x04;	//ESAM��֤ʧ��
											OutBuf[ k++ ] = 0x00;
											break;
										}
										Inverse(Ram_Ptr+18, 199);
										
										//������ز���
										Temp08 = DI07.B08[1]-3;
										memcpy(&Refers_File_Mac[BACKADDR[2+Temp08]], Ram_Ptr+18, 52);//���ʵ��
										memcpy(&Refers_File_Mac[BACKADDR[5+Temp08*4]], Ram_Ptr+18+52, 52);//��1���ݵ��
										memcpy(&Refers_File_Mac[BACKADDR[6+Temp08*4]], Ram_Ptr+18+52+52, 18);//��1�������
										memcpy(&Refers_File_Mac[BACKADDR[7+Temp08*4]], Ram_Ptr+18+52+52+18, 52);//��2���ݵ��
										memcpy(&Refers_File_Mac[BACKADDR[8+Temp08*4]], Ram_Ptr+18+52+52+18+52, 18+5);//��2�������(���������л�ʱ��5�ֽ�)
										
										ProgramRecord( DI07.B08, OptID.B08 );
									}
									if(DI07.B08[1] == 0x03) UpdataFlag &= B1110_1111;
									else UpdataFlag &= B1101_1111;
																		
									k = Terminal_Moneyin_Judge(OutBuf);	//�з�д��Ϣ
									if( k == 1 ) 
									{
										MeterClear = 0x55aa;	//Ԥ�ÿ�����
										k = 2;
									}
									
									break;
											
								case 0x05: //��բ����
									//68+���+68+03+20+FF05A007+�����ߴ���+��բ��������+CS+16	
									if( BDMODE ) break;	//ֻ��Զ�̱��֧�ֺ�բ��������
									
									MacExchange( Ic_Comm_RxBuf, Ram_Ptr+18, 16 );
									result = Relay_Ctrl(Ch+0x24);
									if( result )
									{
										k = 0x00;
										OutBuf[ k++ ] = 0xFF;
										OutBuf[ k++ ] = 0x00;
										OutBuf[ k++ ] = 0x02;	//��բ����ʧ��
										break;
									}
									
									terminal_auth_flag = 0;	//���ݲ��������������֤ʧЧ
									Identitytimer.B32 = 0;
									
									k = 2;
									OutBuf[1] = 0;
									break;
											
								case 0x06: //�ѿ�ģʽ�л�����
//								case 0x01:	//���, 8bytes	[18]~[25]
//								case 0x02:  //�ѿ�ģʽ״̬��, 1byte[26]
//								case 0x03:	//MAC1, 4bytes[27]~[30]
//								case 0x03:  //Ǯ���ļ�, 8bytes[31]~[38]
//								case 0x04:	//MAC2, 4bytes[39]~[42]
									if( hangup_comm_flag )
									{
										k = 0x00;  
										OutBuf[ k++ ] = 0xFF;	//Operation result.
										OutBuf[ k++ ] = 0x00;
										OutBuf[ k++ ] = 0x01;	//Զ�̰�ȫ��֤��ס״̬λ
										break;
									}
											
									if( !identity_prog_flag )
									{
										k = 0x00;
										OutBuf[ k++ ] = 0xFF;
										OutBuf[ k++ ] = 0x08;	//�����֤ʧ��
										OutBuf[ k++ ] = 0x00;
										break;
									}
									
									if( (Ram_Ptr[26] != 0x00) && (Ram_Ptr[26] != 0x01) ) break;	//ģʽ�ִ���
									if(Ram_Ptr[26] == Display_Para.MeterCtrl) break;	//ģʽ��ͬ

									Exchange( Buf, Ram_Ptr+18, 8 );	//���
									if( Buf[0] || Buf[1] || Cmp_Meter_Id(Buf+2) ) break;
									
									if((!BDMODE) && (Key_Status != 0)) break;	//˽Կ������Զ���б���
									
									if( Update_ESAM_Bin_Mac( 0x81, 0x2d, Ram_Ptr+26, 1 ) ) //���·ѿ�ģʽ��
									{
										k = 0x00;
										OutBuf[ k++ ] = 0xFF;
										OutBuf[ k++ ] = 0x04;	//ESAM��֤ʧ��
										OutBuf[ k++ ] = 0x00;
										break;
									}
									
									if(BDMODE)//���ء���Զ�̣���Ǯ���ļ�
									{
										Moneybag_Process(); //Ǯ��ͬ�����´���
										
										if( ChkBufZero( Money.Remain, 4 ) )//ʣ�������0
										{
											Bcd2Hex(&Money.Remain[0], Ic_Comm_TxBuf+5, 4);//�۳�����ʣ����
                                                                                	
											RxTxCommhead( 0x80, 0x30, 0x00, 0x14, 0x04 );
											result = CardTxRxFrame( ESAM, 0x04, 0x08 );
											if( result )
											{
												k = 0x00;
												OutBuf[ k++ ] = 0xFF;
												OutBuf[ k++ ] = 0x04;	//ESAM��֤ʧ��
												OutBuf[ k++ ] = 0x00;
												break;
											}
											
											Dmoney_Record(Ic_Comm_TxBuf+5);	//�˷Ѽ��˷Ѽ�¼
										}
										else//ʣ�������0
										{
											memset( Money.Remain, 0x00, 12 );//ʣ�����͸֧�������
											Money.Crc16.B16 = Crc_16Bits( Money.Remain, ee_remainmoney_lenth );	 //CRC����
											WriteE2WithBackup( ee_remainmoney_page, ee_remainmoney_inpage, Money.Remain, ee_remainmoney_lenth );//ʣ����д����ee(������ͬ��)
										}
									}
									
									result = ModeChange( Ram_Ptr[26] ); //�ѿ�ģʽ�л�
									if( result ) break;
									
									MeterClear = 0x1122;			//����Ӧ��ʱ��					
									
									k = 2;
									OutBuf[1] = Display_Para.MeterCtrl;	//�ѿ�ģʽ״̬��, 1byte
									break;

								default:
									break;
							}
							
							if( ((k == 0) || (k == 3)) && (DI07.B08[1] != 6)) //���������ն������֤ʧЧ
							{
								if( terminal_auth_flag ) Identitytimer.B32 = 0;
								terminal_auth_flag = 0;	
							}

							break;
							
						default:
							break;
					}
				
				default:
				break;
			}
			
			if( k == 0x00 )
			{
				//����68+���+68+C3+02+SERR+CS+16
				OutBuf[ k++ ] = 0xFF;	//Operation result.
				OutBuf[ k++ ] = 0x01;
				OutBuf[ k++ ] = 0x00;
			}
			break;

		//====================================================================================
		case 0x14: //д�α���
			if( ((Uart[Ch].Flag & B0000_0011)==0)&&(DI07.B32 == 0x03150004)&&(Reset_Rep_Timer) )//��λ�����ϱ�״̬��
 			{
 				if( Uart[Ch].Buf[9] != 24 ) break;
 				
 				//�����ϱ�״̬��
				if( (Uart[Ch].Buf[22+2]&B1000_0000)==0 ) Uart[Ch].Buf[22+2] |= B1000_0000;//������ʱ���ô�A�����
				if( (Uart[Ch].Buf[22+8]&B0010_0000)==0 ) Uart[Ch].Buf[22+2] &= B0111_1111;
 				
 				for( i=0,n=0; i<5; i++,n++ )
				{
					Temp08 = (Report_Data[i]^Report_Data2[i]);
					if( Temp08 && ((Temp08&Uart[Ch].Buf[22+n]) != Temp08) )  break;//����30min���µ��ϱ�,����ͨѶ��λ�����ϱ�	
					if( i==2 ) n = 9;
				}
				if( i < 5 )
				{
					k = 2;
 					OutBuf[0] = 0xFF;	//Operation result.
 					OutBuf[1] = 0x04;	//Error: bit2�������/δ��Ȩ
					break;//����30min���µ��ϱ�,����ͨѶ��λ�����ϱ�
				}
 				
 				for( i=0,n=0; i<5; i++,n++ )
				{
					Report_Data[i] &= Uart[Ch].Buf[22+n];//��λ�ϱ�״̬��
					if( i==2 ) n = 9;
					
					Clr_Reportnum(i);	//�����ϱ���������
					Reset_Rep_Timer = 0;
				}
 				k = 1;
				break;
 			}
 			
 			if(  (Uart[Ch].Flag & B0000_0011) || //��֧��ͨ�ŵ�ַΪ�㲥��|| (!identity_prog_flag)ȫAA
 				(( DI07.B08[3] != 0x04 )&&( DI07.B08[3] != 0xee )) ||	//�û��Բ�֧��д��������������
 				( Uart[Ch].Buf[14] == 0x04 ) ||	//��֧��04������
 				((Uart[Ch].Buf[14] == 0x03 ) && (!progenable)&&(Veri_Psw( Uart[Ch].Buf+14) ) ) || //03������̽�ֹ
 				((Uart[Ch].Buf[14] != 0x03 ) && (hangup_comm_flag )) )	//98��99�������ù��ܹ���, ��ȫ��֤ʧЧ
 			{
 				k = 2;
 				OutBuf[0] = 0xFF;	//Operation result.
 				OutBuf[1] = 0x04;	//Error: bit2�������/δ��Ȩ
 				break;
 			}
			
			if( Uart[Ch].Buf[9] <= 12 ) break;//���ݳ��ȱ������12

			memcpy( OptID.B08, Uart[Ch].Buf+18 ,4 );

			Auth = Uart[Ch].Buf[14];
			switch( Auth ) //����Ȩ��
			{
				case KEYTYPE2:
					if( DI07.B32 == 0x06100004 ) 
					{
						if( !BDMODE ) { Err_DI = 1;	break; }
						
						Moneybag_Process1();//Ǯ��ͬ�����´���
						
						result = DecreaseWithMac( Uart[Ch].Buf+22 );
						if( result )                                           
						{
							k = 0x00; 
							OutBuf[ k++ ] = 0xFF;
							OutBuf[ k++ ] = result;
							break;
						}
						
						Dmoney_Record(Ram_Ptr+22);   //�˷Ѽ�¼ 
						
						k = 1;
						break;
					}
				
					//ͨ������+MAC�ķ�ʽ�������ݴ��䣬����Ҫ����������֤��Ҳ����Ҫ��̼����ʹ�á�
					//68+���+68+14+L+DI0-DI3+99(98)000000+�����ߴ���+��������+MAC����MAC�Ĳ������ģ�+CS+16
					//��MACдESAM:		04D682(83,84)+P2+LC+����+MAC
					//���뵽ESAM�Ĳ��������ļ������ܣ��ļ���=DI07.B08[2]%5;
					//������д��ESAM: 	04D689(90,91,92)+00+14+����(��MAC)
					if( Uart[Ch].Buf[9] <= 16 ) break;//���ݳ��ȱ������16

					Temp08 = Hex_To_Bcd(88+(DI07.B08[2]%5));
					result = Decrypt( Ch, Temp08, 0x02 );
					if( result )
					{
						k = 0x00; 
						OutBuf[ k++ ] = 0xFF;
						OutBuf[ k++ ] = 0x04;
						break;
					}
					
					//Ic_Comm_RxBuf[0]:���ݳ���; [1]~[4]:���ݱ�ʶ; [5]~[n]:
					//�ȶ����������ݱ�ʶ
					for( i=0; i<4; i++ )
					{
						if( Ic_Comm_RxBuf[4-i] != DI07.B08[i] ) break;
					}
					
					if( i < 4 ) break;
				
					//�޳����ݳ��Ⱥ�DI������UART.Buf��.
					Uart[Ch].Buf[9] = 8+Ic_Comm_RxBuf[0];	//
					
					Exchange( Uart[Ch].Buf+22, Ic_Comm_RxBuf+5, Ic_Comm_RxBuf[0]-4 );
					
				case 0x03:	//������չ����
					if(DI07.B08[3] == 0xee)
					{
						switch( DI07.B32 )
					  {
							case 0xEE000500://У��������ݿ�
              case 0xEE000501:
              case 0xEE000502:
                  Flag.PhOfset0 = 0;
                  Flag.PhOfset1 = 0;
                  Ch = (unsigned char)( DI07.B32 - 0xEE000500 );
                  if(Uart[Ch].Buf[22+14] == 0x00)
                  {
                    Uart[Ch].Buf[22+14]= 0x80;
                    Uart[Ch].Buf[22+15] = 0x80;
                    Uart[Ch].Buf[22+16] = 0x80;
                    Uart[Ch].Buf[22+17] = 0x80;
                   }            
            memcpy( Adj_Block[Ch].AdjBuff, &Uart[Ch].Buf[22], 30 );
						WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;
        case 0xEE000503://��������
        case 0xEE000504:
        case 0xEE000505:
            Ch = (unsigned char)( DI07.B32 - 0xEE000503 );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.Igain, &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;
        case 0xEE000506://��ѹ����
        case 0xEE000507:
        case 0xEE000508:
            Ch = (unsigned char)( DI07.B32 - 0xEE000506 );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.Ugain, &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;
        case 0xEE000509://����ƫ��
        case 0xEE00050A:
        case 0xEE00050B:
            Ch = (unsigned char)( DI07.B32  - 0xEE000509 );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.Pofst, &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;
        case 0xEE00050C://Ib������λ
        case 0xEE00050D:
        case 0xEE00050E:
            Ch = (unsigned char)(  DI07.B32 - 0xEE00050C );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.PTofst[0], &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;            
        case 0xEE00050F://С������λ
        case 0xEE000510:
        case 0xEE000511:
            Ch = (unsigned char)(  DI07.B32 - 0xEE00050F );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.PTofst[1], &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;            
            RN8302Init();
            break;            
        case 0xEE000512://�������λ
        case 0xEE000513:
        case 0xEE000514:
            Ch = (unsigned char)(  DI07.B32 - 0xEE000512 );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.PTofst[2], &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;
        case 0xEE00052C://�޹�Ib������λ
        case 0xEE00052D:
        case 0xEE00052E:
            Ch = (unsigned char)(  DI07.B32 - 0xEE00052C );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.QTofst[0], &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;            
        case 0xEE00052F://�޹�С������λ
        case 0xEE000530:
        case 0xEE000531:
            Ch = (unsigned char)(  DI07.B32 - 0xEE00052F );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.QTofst[1], &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;            
        case 0xEE000532://�޹��������λ
        case 0xEE000533:
        case 0xEE000534:
            Ch = (unsigned char)(  DI07.B32 - 0xEE000532 );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.QTofst[2], &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;            
        case 0xEE000515://����ƫ��
        case 0xEE000516:
        case 0xEE000517:
            Ch = (unsigned char)( DI07.B32 - 0xEE000515 );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.Iofst, &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;
        case 0xEE00051C://��ѹƫ��
        case 0xEE00051D:
        case 0xEE00051E:
            Ch = (unsigned char)( DI07.B32 - 0xEE00051C );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.Uofst, &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;            
        case 0xEE000518://�й���������
        case 0xEE000519:
        case 0xEE00051A:
            Ch = (unsigned char)( DI07.B32 - 0xEE000518 );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.Pgain, &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;    
        case 0xEE000526://�޹���������
        case 0xEE000527:
        case 0xEE000528:
            Ch = (unsigned char)( DI07.B32 - 0xEE000526 );
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.Qgain, &Uart[Ch].Buf[22], 2 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;            
        case 0xEE000520://����ͨ��U��λƫ��
        case 0xEE000521:
        case 0xEE000522:
            Ch = (unsigned char)( DI07.B32 - 0xEE000520 );
            if( Uart[Ch].Buf[22] == 0x00 )
               Uart[Ch].Buf[22] = 0x80; 
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.UTofst, &Uart[Ch].Buf[22], 1 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;    
        case 0xEE000523://����ͨ��I��λƫ��
        case 0xEE000524:
        case 0xEE000525:
            Ch = (unsigned char)( DI07.B32 - 0xEE000523 );
            if( Uart[Ch].Buf[22] == 0x00 )
            {
                Uart[Ch].Buf[22] = 0x80; 
                Uart[Ch].Buf[23] = 0x80;
                Uart[Ch].Buf[24] = 0x80;
            }
            memcpy( (unsigned char*)&Adj_Block[Ch].Adj.ITofst, &Uart[Ch].Buf[22], 3 );
            WriteE2WithBackup( ee_para1_RN8302_page, ee_para1_RN8302_inpage+30*Ch,(unsigned char*)&Adj_Block[Ch].AdjBuff[0], ee_para1_RN8302_lenth);			 
            Adj_Block[Ch].Adj.AdjChk = 0;
            RN8302Init();
            break;  
//        case 0xEE000535://NVM1ģʽ�µ����Զ� offset У�� 
//            for( Ch = 0; Ch < 3; Ch++ )
//            {
//                RN8302_Read( (unsigned char*)&Adj_Block2[Ch].Adj2.Iofst2, (DCOS_IA+Ch), 2 );//��ֱ��ͨ��
//                MEM_Write( ADJ_DCOS_IA+2*Ch, (unsigned char*)&Adj_Block2[Ch].Adj2.Iofst2, 2, Mem_E2PROMA );
//            }
//            break;   
        case 0xEE000536://�رո�ͨ 
            Temp = 0xE5;//дʹ��
            RN8302_Write( 0x0180, (unsigned char*)&Temp, 1 );
            Temp = 0x02;//�رո�ͨ
            RN8302_Write( 0x0170, (unsigned char*)&Temp, 1 );
            Temp = 0x0095;//ֱ�� OFFSET �Զ�У��ʹ�ܼĴ�����0x0010(������У��,���´�checksum�����)
            RN8302_Write( 0x0191, (unsigned char*)&Temp, 2 );
            Temp = 0xDC;//д����
            RN8302_Write( 0x0180, (unsigned char*)&Temp, 1 );
            break;				
			  default:
						Err_DI = 1;	//���ݱ�ʶ��
						break;
					}
								
					}
				  else
					{
						switch( DI07.B08[2] )
					  {
						case 0x00:
							switch( DI07.B08[1] )
							{
								case 0x01:
									switch( DI07.B08[0] )
									{
										case 0x01:	//���ڼ�����(����0����������)	YYMMDDWW  	4	����������	
										case 0x02:	//ʱ��                          hhmmss    	3	ʱ����      
										case 0x0C:	//���ڡ�����(����0����������)��ʱ��	YYMMDDWWhhmmss	7 ����������ʱ����
											if( DI07.B08[0] == 0x01 )
											{
												Temp16.B08[0] = 3;
												Temp16.B08[1] = 4;
											}
											else if( DI07.B08[0] == 0x02 )
											{
												Temp16.B08[0] = 0;
												Temp16.B08[1] = 3;
											}
											else
											{
												Temp16.B08[0] = 0;
												Temp16.B08[1] = 7;
											}
																									
											result = CommSetRtc( Ch, Temp16.B08[0], Temp16.B08[1] );
											if( result ) break;
                      
											Festival_holiday_Judge(); 
											run_alarm_flag = 1;
											
											Init_Demand();	//��ǰ��������
											
											k = 1;
											OutBuf[0] = 0x00;
											k = 1;
											break;
											
										case 0x03:	//�����������                  NN        	1	��
											
											result = IsBCD( Uart[Ch].Buf[22] );
											if( result != 0x00 ) 
											{												
												break; //error.
											}
											
											//����Ĭ��ֵ����������15min������ʱ��1min��
											switch( Uart[Ch].Buf[22] )
											{
												case 0x05:
												case 0x10:
												case 0x15:
												case 0x30:
												case 0x60:
													ReadE2WithBackup( ee_Demand_Period_page, ee_Demand_Period_inpage,Buf, ee_Demand_Period_len );				
													
													Temp08 = Bcd_To_Hex(Uart[Ch].Buf[22]);
													if( Temp08 == Buf[0])
										      {													
														k = 1;
													  OutBuf[0] = 0x00;
														break;
													}				
													else
													  Buf[0] = Temp08;
													
													result = WriteE2WithBackup( ee_Demand_Period_page, ee_Demand_Period_inpage,Buf, ee_Demand_Period_len );
													if( result != 0x00 )
													{													
														break;
													}

													Demand_Para.Max_Demand_Period = Temp08;													
				                            	
													Init_Demand();	//��ǰ��������
                          
													Dm_veri_flag = 1;
													k = 1;
													OutBuf[0] = 0x00;
													break;
													
												default:
													break;
											}
											break;

										case 0x04:	//����ʱ��                      NN        	1	��          
											 
										  result = IsBCD( Uart[Ch].Buf[22] );
											if( result != 0x00 ) 
											{												
												break; //error.
											}  
											switch( Uart[Ch].Buf[22] )
											{
												case 0x01:
												case 0x02:
												case 0x03:
												
												  ReadE2WithBackup( ee_Demand_Period_page, ee_Demand_Period_inpage,Buf, ee_Demand_Period_len );				
													
													Temp08 = Bcd_To_Hex(Uart[Ch].Buf[22]);
													if( Temp08 == Buf[1])
										      {													
														k = 1;
													  OutBuf[0] = 0x00;
														break;
													}				
													else
													  Buf[1] = Temp08;
													
													result = WriteE2WithBackup( ee_Demand_Period_page, ee_Demand_Period_inpage,Buf, ee_Demand_Period_len );
													if( result != 0x00 )
													{													
														break;
													}

													Demand_Para.Sliding_Time = Temp08;													
				                            	
													Init_Demand();	//��ǰ��������
                          Dm_veri_flag = 1;
													k = 1;
													OutBuf[0] = 0x00;
													break;												
													
												default:
													break;
											}
											break;
										
	
//										case 0x05:	//У��������                  XXXX      	2	����        
//											break;

										case 0x0A:	//���׽���ʱ���л�ʱ��	YYMMDDhhmm 5
											if( !BDMODE ) { Err_DI = 1;	break; }
										case 0x06:	//����ʱ�����л�ʱ��            YYMMDDhhmm	5	������ʱ��
										case 0x07:	//������ʱ���л�ʱ��            YYMMDDhhmm	5	������ʱ��
//										case 0x0B:	//���׷ѿ�ģʽ�л�ʱ��	YYMMDDhhmm 5
											//����ʱ�����л�ʱ�䣨5����������ʱ�α��л�ʱ�䣨5�������׷��ʵ���л�ʱ�䣨5�������׽����л�ʱ�䣨5����crc��2��
											Temp08 = JudgeSwitch( Uart[Ch].Buf+22, 0 );
											if( Temp08==1 ) break;
												
											result = UpdateSysPara( 0xC0+5*(DI07.B08[0]-6), Uart[Ch].Buf+22, 5 );
											if( result ) break;
											
											if( (Temp08==3)&&(DI07.B08[0]<=0x07) ) JudgeSwitch2(DI07.B08[0]-6);//��ת�����ֽ�,������ʱ��ͽ���Լ������
											
											k = 1;
											break;
											
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									break;
									
								case 0x02:
									//��ʱ������1������ʱ�α�����1������ʱ���� ��1������������1����������������1��+г������������1��+��������1������Կ����1����crc��2��
									Temp32.B08[0] = 0x00;	//��Сֵ
									Temp32.B08[1] = 0x00;	//���ֵ
									Temp32.B08[2] = 0x00;	//����������
									switch( DI07.B08[0] )
									{
										case 0x01:	//��ʱ����p��14            	NN      1	��
											Temp32.B08[0] = 1;			//��Сֵ
											Temp32.B08[1] = MAXREGION;	//���ֵ
											Temp32.B08[2] = 0x10;		//����������: ��ʱ������(Bit4)
											break;

										case 0x02:	//��ʱ�α���q��8           	NN      1	��	
											Temp32.B08[0] = 1;			//��Сֵ
											Temp32.B08[1] = MAXSIDUANTB;//���ֵ
											break;

										case 0x03:	//��ʱ����(ÿ���л���)m��14	NN      1	��
											Temp32.B08[0] = 1;			//��Сֵ
											Temp32.B08[1] = MAXSIDUAN;	//���ֵ
											Temp32.B08[2] = 0x20;		//����������: ��ʱ������(Bit5)
											break;

										case 0x04:	//������k��63              	NN      1	��	
											Temp32.B08[1] = 4;			//���ֵ
											Temp32.B08[2] = 0x40;		//����������: ��������(Bit6)
											break;

										case 0x05:	//����������n��254         	NNNN	2	��
											if( Uart[Ch].Buf[23] ) break;
											Temp32.B08[1] = MAXHOLIDAY;	//���ֵ
											break;
//
//										case 0x06:	//г����������             	NN  	1	��	
//											break;

										case 0x07:	//������			NN	1	��
											if( !BDMODE ) { Err_DI = 1;	break; }
											
											Temp32.B08[1] = 6;			//���ֵ
											RefreshPrice();
											break;
											
										case 0x08:	//��Կ������			HH 	1	��
											Temp32.B08[1] = MAX_PSW_NUM;	//���ֵ
											break;
										
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;	
									}
									
									if( Temp32.B08[1] == 0x00 ) break;	//����ʧ��

									if( DI07.B08[0] != 0x08 )
									{
										if( IsBCD(Uart[Ch].Buf[22]) ) break;
										Temp08 = Bcd_To_Hex( Uart[Ch].Buf[22] );
									}
									else Temp08 = Uart[Ch].Buf[22];		//��Կ��������Ϊhex��

									if( (Temp08>=Temp32.B08[0]) && (Temp08 <= Temp32.B08[1]) )
									{
										ReadE2WithBackup( ee_feik_number_page, ee_feiknumber_inpage, Buf, ee_feiknumber_lenth );
										Buf[DI07.B08[0]-1] = Uart[Ch].Buf[22];
										result = WriteE2WithBackup( ee_feik_number_page, ee_feiknumber_inpage, Buf, ee_feiknumber_lenth );
										if( result != 0x00 ) break; //error
										
										run_timezone_flag = 1;
										Tariff.Crc16.B16 += 1;
										Feik.Crc16.B16 += 1;
									
										if( DI07.B08[0] == 8 )
										{
											Feik.Num[7] = Uart[Ch].Buf[22];
											GetKeyStatus();
										}

										k = 1;
									}
									else if( Temp32.B08[2] && (Temp08 > Temp32.B08[1]) )
									{
										k = 2;
										OutBuf[0] = 0xFF;
										OutBuf[1] = Temp32.B08[2]; 	//������Ϣ��ERR
									}
									break;
									
								case 0x03:	  
									//ѭ������(1)��ѭ��ʱ��(1)������С��λ��(1)������λ��(1)����������(1)��������(1)+ ��բ��ʱʱ��(2)��crc(2)
									Temp32.B08[0] = 0x00;	//��Сֵ
									Temp32.B08[1] = 0x00;	//���ֵ
									switch( DI07.B08[0] )
									{
										case 0x01:	//�Զ�ѭ����ʾ����            	NN      1   ��
										case 0x05:	//����ѭ����ʾ����            	NN      1   ��  	
											Temp32.B08[0] = 1;	//��Сֵ
											Temp32.B08[1] = SCREENNUM;	//���ֵ
											break;
											
										case 0x02:	//ÿ����ʾʱ��                	NN      1   ��
											Temp32.B08[0] = 5;	//��Сֵ
											Temp32.B08[1] = 20;	//���ֵ
											break;
                                	
										case 0x03:	//��ʾ����С��λ��            	NN      1   λ  
											Temp32.B08[1] = 2;	//���ֵ
											break;
                                	
										case 0x04:	//��ʾ���ʣ����������С��λ��	NN      1   λ  
											Temp32.B08[1] = 4;	//���ֵ
											break;
										
										case 0x08:	//�ϵ�ȫ��ʱ��	NN	1	λ
											Temp32.B08[0] = 5;	//��Сֵ
											Temp32.B08[1] = 30;	//���ֵ
											break;
											
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									
									if( Temp32.B08[1] == 0x00 ) break;

									if( IsBCD(Uart[Ch].Buf[22]) ) break;
									Temp08 = Bcd_To_Hex( Uart[Ch].Buf[22] );
									
									if( (Temp08 >= Temp32.B08[0]) && (Temp08 <= Temp32.B08[1]) )
									{
										result = UpdateSysPara( DI07.B08[0]-1, &Temp08, 1 );
										if( result != 0x00 ) break;
	                                		                                	
										k = 1;
									}
									break;

								case 0x04:	  
									switch( DI07.B08[0] )
									{
										case 0x01:	//ͨ�ŵ�ַ                     	NNNNNNNNNNNN	6
											memset( Buf, 0x99, 6 );
											if( Cmp_Data( Buf, Uart[Ch].Buf+22, 6 ) ==3 ) break;	//��ֹ����ȫ9
											if( IsMultiBcd( Uart[Ch].Buf+22, 6 ) )	break;
											
											Exchange( Buf, Uart[Ch].Buf+22, 6 );
																						
											result = WriteE2WithBackup( ee_commaddr_page, ee_commaddr_inpage, Buf, ee_commaddr_lenth );
											if( result != 0x00 ) break;
												
											memcpy( Comm_Addr.Comm_Id, Buf, 8 );
	
											k = 1;
											break;
                    case 0x02:	//ͨ�ŵ�ַ                     	NNNNNNNNNNNN	6
											memset( Buf, 0x99, 6 );
											if( Cmp_Data( Buf, Uart[Ch].Buf+22, 6 ) ==3 ) break;	//��ֹ����ȫ9
											if( IsMultiBcd( Uart[Ch].Buf+22, 6 ) )	break;
											
											Exchange( Buf, Uart[Ch].Buf+22, 6 );
											memcpy( Esam_Para.Meter_Id, Buf, 6 );//����esam��ŵ�ram
		                  
										  result = WriteE2WithBackup(ee_esampara_page, ee_esampara_inpage, Esam_Para.Alarm_Limit1.B08, ee_esampara_lenth);
												
											if( result != 0x00 ) break;											
	
											k = 1;
											break;
										case 0x05:	//�����/����������ASCII �룩XXXXXXXXXXXX	6
										case 0x06:	//��������ASCII �룩         	XXXXXXXXXXXX	6
										case 0x07:  	//���ȵȼ���ASCII �룩         XXXXXXXX   	4
										case 0x0B:  	//����ͺţ�ASCII �룩         XX��XX      	10 
										case 0x0C:	//�������ڣ�ASCII �룩          XX��XX      	10 
										case 0x0D:	//Э��汾�ţ�ASCII �룩        XX��XX      	16 	
											if( Auth != 3 )
											{
												k = 2;
												OutBuf[0] = 0xFF; //���
												OutBuf[1] = 0x04; //Error: bit4�������/δ��Ȩ
												break;
											}
										case 0x03:	//�ʲ�������루ASCII �룩     	NN��NN      	32
										case 0x0F:	//���ܱ�λ����Ϣ				11
											if(DI07.B08[0] == 0x05)
											{
												Temp16.B08[0] = ee_ratedcurr_page;//�����
												Temp16.B08[1] = ee_ratedcurr_inpage;
												Temp08 = 6;//6�ֽ�
											}
											else if(DI07.B08[0] == 0x06)
											{
												Temp16.B08[0]=ee_maxcurr_page;//������
												Temp16.B08[1] = ee_maxcurr_inpage;
												Temp08 = 6;//6�ֽ�
											}
											else if(DI07.B08[0] == 0x07)
											{
												Temp16.B08[0] = ee_activeclass_page; //�й����ȵȼ�
												Temp16.B08[1] = ee_activeclass_inpage;
												Temp08 = 4;
											}
											else if(DI07.B08[0] == 0x0B)
											{
												Temp16.B08[0] = ee_matertype_page; //����ͺ�
												Temp16.B08[1] = ee_matertype_inpage;
												Temp08 = 10;
											}
											else if(DI07.B08[0] == 0x0c) 
											{
												Temp16.B08[0] = ee_meterdate_page;//��������
												Temp16.B08[1] = ee_meterdate_inpage;
												Temp08 = 10;
											}
											else if(DI07.B08[0] == 0x0D)
											{
												Temp16.B08[0] = ee_protocolver_page;//Э��汾��
												Temp16.B08[1] = ee_protocolver_inpage;
												Temp08 = 16;
											}
											else if(DI07.B08[0] == 0x03) 
											{
												Temp16.B08[0] = ee_zichannum_page;//�ʲ�������
												Temp16.B08[1] = ee_zichannum_inpage;
												Temp08 = 32;
											}
											else
											{
												Temp16.B08[0] = ee_meterpos_page;//���ܱ�λ����Ϣ
												Temp16.B08[1] = ee_meterpos_inpage;
												Temp08 = 11;
												
												if( Auth == KEYTYPE2 )
												{
													Inverse( Uart[Ch].Buf+22, 11 );
													Inverse( Uart[Ch].Buf+22+0, 4 );	//XXXX.XXXX	4	����
													Inverse( Uart[Ch].Buf+22+4, 4 );	//XXXX.XXXX	4	γ��
													Inverse( Uart[Ch].Buf+22+8, 3 );	//XXXX.XX	3	�߶�
												}
											}
											if( Uart[Ch].Buf[9] != (Temp08+12) ) break;//���ݳ��ȴ���
											I2C_Write_Eeprom( Temp16.B08[0], Temp16.B08[1], Uart[Ch].Buf+22, Temp08 );
											k = 1;
											break;
										
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;                                      		
									}
									break;

								case 0x06:	  
									switch( DI07.B08[0] )
									{
										case 0x01:  //�й���Ϸ�ʽ������  	NN	1
											//�����������֣�5�����й���Ϸ�ʽ������1(1)����������ģʽ��(6)���������������1(1) +�ϱ�ģʽ��(6)+crc��2��
											if( Uart[Ch].Buf[22]&B1111_0000 ) break;	//�Ƿ���ʽ
											
											i = Mode.Comp_B;	//�ݴ���ǰ����
											
											result = UpdateSysPara( 0x80+1, Uart[Ch].Buf+22, 1 );
											if( result != 0x00 ) break;
												
											//--------- �й���Ϸ�ʽ��� ------------
											memcpy( Eeprom.Buffer+6, OptID.B08, 4 );	//�����ߴ���(4)
											Eeprom.Buffer[10] = i;		//���ǰ���õ�����(1)
											Event_Record( ee_combn_inpage );
											
											k = 1;
											break;
											
//										case 0x02:  //�޹���Ϸ�ʽ1 ������  NN	1	
//											break;
//											
//										case 0x03:	//�޹���Ϸ�ʽ2 ������  NN	1	
//											break;
											
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									break;
									
								case 0x07:	  
									switch( DI07.B08[0] )
									{
										case 0x01:	//�����ͺ����ڲ�����������	NN  1
//										case 0x02:	//�Ӵ�ʽ�����ڲ�����������    NN  1
										case 0x03:	//ͨ�ſ�1 ������������          NN  1
//										case 0x04:	//ͨ�ſ�2 ������������          NN  1
										case 0x05:	//ͨ�ſ�3 ������������          NN  1
											Temp08 = Set_Comm_Rate( DI07.B08[0], Uart[Ch].Buf[22] );
											if( Temp08 )
											{
												k = 2;
												OutBuf[0] = 0xff;
												OutBuf[1] = 0x08;
											}
											else
											{
												k = 1;
											}
											break;
	
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									break;
																		
								case 0x08:
									switch( DI07.B08[0] )
									{
										case 0x02:	//�����ղ��õ���ʱ�α��    NN	1
											if( (Uart[Ch].Buf[22] == 0x00) || (Uart[Ch].Buf[22] > MAXSIDUANTB) )	break;	//ֻ����1~8����ʱ�α��
											i = Week_Holiday.Shiduan_N;	//�ݴ���ǰ����
											
										case 0x01:	//������������		NN		1										
											result = UpdateSysPara( 0x60+DI07.B08[0]-1, Uart[Ch].Buf+22, 1 );
											if( result != 0x00 ) break;

											if( DI07.B08[0] == 0x02 )
											{
												//--------- �����ձ�� ------------
												memcpy( Eeprom.Buffer+6, OptID.B08, 4 );	//�����ߴ���(4)
												Eeprom.Buffer[10] = i;		//���ǰ���õ�����(1)
												Event_Record( ee_weekdayn_inpage );
											}
											
											k = 1;
											break;

										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									break;

								case 0x09:	
									switch( DI07.B08[0] )
									{
										case 0x01:	//���ɼ�¼ģʽ��		NN	1													
										case 0x02:	//��������ģʽ��		NN	1	��ʱ��������ģʽ��
										case 0x03:	//˲ʱ��������ģʽ��	NN	1
										case 0x04:	//Լ����������ģʽ��    NN	1
										case 0x05:	//���㶳������ģʽ��    NN	1
										case 0x06:	//�ն�������ģʽ��      NN	1
											//�����������֣�5�����й���Ϸ�ʽ������1(1)����������ģʽ��(6)���������������1(1) +�ϱ�ģʽ��(6)+crc��2��
											if( DI07.B08[0] == 0x05 )
											{
												if( Uart[Ch].Buf[22]&B1111_1100 ) break;
											}
											if( DI07.B08[0] == 0x01 )
											{
												if( Uart[Ch].Buf[22]&B1111_0000 ) break;
											}
											else
											{
												if( Uart[Ch].Buf[22]&B0111_1100 ) break;
											}
											
											result = UpdateSysPara( 0x80+2+DI07.B08[0]-1, Uart[Ch].Buf+22, 1 );
											if( result != 0x00 ) break;
	
											k = 1;
											break;
	
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									break;
									
								case 0x0A:
									switch( DI07.B08[0] )
									{
										case 0x01:	//���ɼ�¼��ʼʱ��		MMDDhhmm	4
											result = JudgeClockNoWeek( 1, Uart[Ch].Buf+22, 4 );
											if( result != 0 ) break; //����
											
											result = UpdateSysPara( 0x60+3, Uart[Ch].Buf+22, 4 );
											if( result != 0x00 ) break;
											
											k = 1;
											break;
												
										case 0x02:	//��1 �ฺ�ɼ�¼���ʱ��    NNNN	2
										case 0x03:	//��2 �ฺ�ɼ�¼���ʱ��    NNNN	2
										case 0x04:	//��3 �ฺ�ɼ�¼���ʱ��    NNNN	2
										case 0x05:	//��4 �ฺ�ɼ�¼���ʱ��    NNNN	2
											if( IsMultiBcd( Uart[Ch].Buf+22, 2 ) )	break;
											if( ((Uart[Ch].Buf[22] == 0) || (Uart[Ch].Buf[22]>0x60)) || Uart[Ch].Buf[23] ) break;
											
											result = UpdateSysPara( 0x60+7+2*(DI07.B08[0]-2), Uart[Ch].Buf+22, 2 );
											if( result != 0x00 ) break;
										
											k = 1;
											break;

										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									break;
																	
								case 0x0B:
									switch( DI07.B08[0] )
									{
										case 0x01:	//ÿ�µ�1 ������	DDhh	2	��ʱ
										case 0x02:	//ÿ�µ�2 ������    DDhh	2	��ʱ
										case 0x03:	//ÿ�µ�3 ������    DDhh	2	��ʱ
											if( (Uart[Ch].Buf[22] != 0x99) || (Uart[Ch].Buf[23] != 0x99) )
											{
												result = JudgeClockNoWeek( 2, Uart[Ch].Buf+22, 2 );
												if( result != 0 ) break; //����
	
												if( Uart[Ch].Buf[23] > 0x28 )	break;	//ֻ����1~28�յ�����ʱ��
											}
											
											ReadE2WithBackup( ee_histroyparameter_page, ee_histroyparameter_inpage, Buf, ee_histroyparameter_lenth );
											
											memcpy( ChangeBuf, Buf, ee_histroyparameter_lenth );

											Buf[2*(DI07.B08[0]-1)+0] = Uart[Ch].Buf[22];
											Buf[2*(DI07.B08[0]-1)+1] = Uart[Ch].Buf[23];

											result = WriteE2WithBackup( ee_histroyparameter_page, ee_histroyparameter_inpage, Buf, ee_histroyparameter_lenth );
											if( result != 0x00 ) break;
											
											if( (DI07.B08[0] == 0x01)	//���õ�һ������
											 && ((ChangeBuf[0]!=Uart[Ch].Buf[22]) || (ChangeBuf[1]!=Uart[Ch].Buf[23])) )//��һ�����շ����ı�
											{
												if( Price.Ladder_Mode == 0x55 )
													Freeze_Usedl(1);//�����,��ת��,�����õ���
												else
													Freeze_Usedl(2);//�½���,���õ���ת�������
												Freeze_Dl(ee_histroy_dl_page,ee_histroyf_inpage);
												
												run_price_flag = 1;
											}

											//---- �����ձ�̼�¼ ----
											memcpy( Eeprom.Buffer+6, OptID.B08, 4 );	//�����ߴ���(4)
											memcpy( Eeprom.Buffer+10, ChangeBuf, 6 );	//�����ձ��ǰÿ�µ�1~3����������(6)
											
											Event_Record( ee_jieshuann_inpage );
											
											k = 1;
											break;

										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									break;
								
								case 0x0C:	//0����Կ~9����Կ��03�����룬04������		
									if( ((DI07.B08[0] == 0x04) || (DI07.B08[0] == 0x05)) && ((DI07.B08[0]-1) == Uart[Ch].Buf[22]) )
									{
										WriteE2WithBackup( ee_password_page, ee_password_inpage+(DI07.B08[0]-4)*6, Uart[Ch].Buf+22, ee_password_lenth );
										k = 1;
									}
									else
									{
										k = 2;
										OutBuf[0] = 0xFF; //���
										OutBuf[1] = 0x04; //Error: bit2�������/δ��Ȩ
									}		
									break;
									
								case 0x10:
									if( !BDMODE ) { Err_DI = 1;	break; }
										
									switch( DI07.B08[0] )
									{
										case 0x01:  //�������1 ��ֵ	XXXXXX.XX	4	Ԫ											
										case 0x02:  //�������2 ��ֵ	XXXXXX.XX	4	Ԫ
										  Temp32.B08[2] = 0x40+(DI07.B08[0]-1)*4;	//E2��ҳ��ƫ����	
										  Temp32.B08[0] = 4;
										  if( IsMultiBcd( Uart[Ch].Buf+22, 4 ) )	break;
	 
										  result = UpdateSysPara( Temp32.B08[2], Uart[Ch].Buf+22, Temp32.B08[0] ); //����XRAM, E2PROM
										  if( result ) break;	
										
										  run_price_flag = 1;
										  k = 1;									
									    break;
										
										case 0x03:	//͸֧�����ֵ	XXXXXX.XX	4	Ԫ
											if( remainder_maxtick_flag )	//��ֹʣ����Ϊ0,͸֧������0��ɷ�0�Զ��պϼ̵��������
											{
												card_closerelay_flag   = 0;
												card_closerelay_flag1  = 0;
												Store_Meterstatus();
											}
										case 0x04:	//�ڻ������ֵ    NNNNNN.NN	4	Ԫ
										case 0x05:	//��բ��������ֵ  NNNNNN.NN   4	Ԫ
											//͸֧�����ֵ(4)���ڻ������ֵ(4)����բ��������ֵ(4)��Crc(2)
											Temp08 = (DI07.B08[0]-0x03)*4;
											
											if( IsMultiBcd( Uart[Ch].Buf+22, 4 ) )	break;

											ReadE2WithBackup( ee_limitmenory_page, ee_limitmenory_inpage, Buf, ee_limitmenory_lenth );

											Exchange( Buf+Temp08, Uart[Ch].Buf+22, 4 );

											result = WriteE2WithBackup( ee_limitmenory_page, ee_limitmenory_inpage, Buf, ee_limitmenory_lenth );

											if( result != 0x00 ) break;
																								
											memcpy( Money_Para.Tick.B08, Buf, ee_limitmenory_lenth+2 );

											run_alarm_flag = 1;

											k = 1;
											break;
											
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									break;
								
								case 0x11:
									switch( DI07.B08[0] )
									{
										case 0x01:	//�������������1	NN	1
											//�����������֣�5�����й���Ϸ�ʽ������1(1)����������ģʽ��(6)���������������1(1) +�ϱ�ģʽ��(6)+crc��2��
											result = UpdateSysPara( 0x80+8, Uart[Ch].Buf+22, 1 );
											if( result != 0x00 ) break;
	
											k = 1;
											break;
											
										case 0x04:	//�����ϱ�ģʽ��	NNNNNNNNNNNNNNNN	8
											//�����������֣�5�����й���Ϸ�ʽ������1(1)����������ģʽ��(5)���������������1(1)���ϱ�ģʽ��(6) + crc��2��
											if( Uart[Ch].Buf[9] != 20 ) break;//���ݳ��ȴ���
											Inverse( Uart[Ch].Buf+22, 8 );	//���ݵ���(���ֽ���ǰ��ɸ��ֽ���ǰ,��������ֱ�ɵ��ֽ���ǰ)
											result = UpdateSysPara( 0x80+9, Uart[Ch].Buf+22, 8 );
											if( result != 0x00 ) break;
	
											k = 1;
											break;
											
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									break;
																
								case 0x12:
									I2C_Read_Eeprom44( ee_freezeparameter_page, Buf, ee_freezeparameter_lenth );
									
									Temp08 = 0x00;	//Ĭ��ʧ��
									switch( DI07.B08[0] )
									{
										case 0x01:	//���㶳����ʼʱ��	YYMMDDhhmm	5	������ʱ�֣�Ĭ����ֵΪȫ�㣬����ֵӦ������ֵ����ֵ��
//											if( (Uart[Ch].Buf[22]!=0x00) && (Uart[Ch].Buf[22]!=0x30) )
//											{
//												break;	//����
//											}

											result = JudgeClockNoWeek( 1, &Uart[Ch].Buf[22], 5 );
											if( result != 0 ) break; //����
																						
											Exchange( Buf, Uart[Ch].Buf+22, 5 );
											Temp08 = 0x01;
											break;
											
										case 0x02:  //���㶳��ʱ����	NN          1	����, ����Ϊ30��60���ӣ�Ĭ��Ϊ60���ӡ�
											if( (Uart[Ch].Buf[22]!=0x30)&&(Uart[Ch].Buf[22]!=0x60) ) break;
											
											Buf[5] = Uart[Ch].Buf[22];
											Temp08 = 0x01;
											break;
											
										case 0x03:  //�ն���ʱ��      	hhmm        2	ʱ��      
											result = JudgeClockNoWeek( 1, &Uart[Ch].Buf[22], 2 );
											if( result != 0 ) break; //����
											
											Wr_Pretime_Dayfreeze();	//д��һ���ն���ʱ��
											
											Buf[6] = Uart[Ch].Buf[23];
											Buf[7] = Uart[Ch].Buf[22];
											Temp08 = 0x01;
											break;
											
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}

									if( Temp08 )
									{
										result = I2C_Write_Eeprom44( ee_freezeparameter_page, Buf, ee_freezeparameter_lenth );
										if( result != 0x00 ) break;
										
										k = 1;
									}
									break;
									
//								case 0x13:	
//									switch( DI07.B08[0] )
//									{
//										case 0x01:	//����ͨ�����߼��ź�ǿ��ָʾ	NN	1	
//											break;
//											
//										default:
//											break;
//									}
//									break;
									
								case 0x14:
									switch( DI07.B08[0] )
									{
										case 0x01:	//��բ��ʱʱ�䣨NNNN Ϊ��բǰ�澯ʱ�䣩	NNNN	2
										case 0x02:	//�̵�����բ���Ƶ�������ֵ	XXX.XXX	3
										case 0x03:	//�̵�����ʱ��բʱ��	XXXX	2
										case 0x04:	//������֤ʱЧ	XXXX	1
										case 0x05:	//�ϱ���λʱ��	XXXX	1
											if( DI07.B08[0] == 0x02 )
											{
												Temp16.B08[0] = 0;	//ƫ�Ƶ�ַ0
												Temp16.B08[1] = 3;	//�ֽ���
											}
											else if( DI07.B08[0] >= 0x04 )
											{
												Temp16.B08[0] = DI07.B08[0]+0x03;//ƫ�Ƶ�ַ7,8
												Temp16.B08[1] = 1;
												if( Uart[Ch].Buf[22] == 0 ) break;
											}
											else
											{
												Temp16.B08[0] = DI07.B08[0]+2;//ƫ�Ƶ�ַ3,5	
												Temp16.B08[1] = 2;	
//												if( (DI07.B08[0] == 0x03) && (Uart[Ch].Buf[22] == 0) && (Uart[Ch].Buf[23] == 0) ) break;
											}
											
											if( IsMultiBcd( Uart[Ch].Buf+22, Temp16.B08[1] ) ) break;
											
											result = UpdateSysPara( 0x80+17+Temp16.B08[0], Uart[Ch].Buf+22, Temp16.B08[1] );
											if( result != 0x00 ) break;						
											
											if( Opendelaytimer.B32 && (DI07.B08[0]==0x01) ) Opendelaytimer.B32 = Calculate_Timer(0)+2;
//											if( Relay_Mode != 0x55 )//���ü̵���
											{
												if( CurrentTimer.B32 && (DI07.B08[0]==0x03) ) CurrentTimer.B32 = Calculate_Timer(1);
											}
											if( IR_EnTimer && (DI07.B08[0]==0x04) ) IR_EnTimer = Calculate_Timer(2);
											if( Reset_Rep_Timer && (DI07.B08[0]==0x05) ) Reset_Rep_Timer = Calculate_Timer(3);
											
											k = 1;
											break;											
											
										case 0xee:	//�̵�����բ����ʽ	XX	1
											result = UpdateSysPara( 0x80+26, Uart[Ch].Buf+22, 1 );
											if( result != 0x00 ) break;
											
										case 0xf0:	//PLCEVTOUT��״̬  1	BCD	
											if( (Uart[Ch].Buf[22]!=0x55) && (Uart[Ch].Buf[22]!=0xaa) ) break;
											
											F_Plcevtout_Sta = Uart[Ch].Buf[22];//0x55�øߣ������©��,0xaa�õͣ�����ͣ�
											
											k = 1;
											break;
										
										case 0xF1:	//ͣ���¼���ѹ������ֵ	1	BCD
										case 0xF2:	//ͣ���¼�����������ֵ	1	BCD
										case 0xF3:	//ͣ���¼��ж���ʱʱ�䶨ֵ	1	BCD
										case 0xF4:	//������¼��ж���ʱʱ�䶨ֵ	1	BCD
										case 0xF5:	//ʱ�ӵ��Ƿѹ�¼��������޶�ֵ	1	BCD
										case 0xF6:	//ʱ�ӵ��Ƿѹ�¼��ж���ʱʱ�䶨ֵ	1	BCD
											if( IsBCD(Uart[Ch].Buf[22]) || (Uart[Ch].Buf[22]==0x00) ) break;
											
											result = UpdateSysPara( 0xA0+(DI07.B08[0]-0xF1), Uart[Ch].Buf+22, 1 );
											if( result != 0x00 ) break;				
											
											k = 1;
											break;
										
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									break;

								default:
									Err_DI = 1;	//���ݱ�ʶ��
									break;
							}
							break;
							
						case 0x07:	//��ǰ�׽���ʱ���������׽���ʱ����
							if( !BDMODE ) { Err_DI = 1;	break; }
						case 0x01:	//��һ��ʱ�������ʱ�α�����
						case 0x02:	//�ڶ���ʱ�������ʱ�α�����
							//ʱ������ʱ����1~14�������裬ʱ��������14��ʱ�������һ�����㡣
							//��ʱ�α�ĸ�����1~8�������裬û�в��㹦�ܡ�
							//��ʱ�α���ʱ����1~14���������裬
							//�·�����ʱ�����ж��趨��ʱ������ʱ������ֻ�����а����˶��ٸ���
							//��ʱ������1������ʱ�α�����1������ʱ���� ��1������������1����������������1��+г������������1��+��������1������Կ����1����crc��2��
							if( (DI07.B08[1]!=0x00) || (DI07.B08[0]>MAXSIDUANTB) || 
							   ((DI07.B08[2]==0x07) && (DI07.B08[0]>0x01)) )//����ʱ����ǰ�ͱ����׾���������
							{
								Err_DI = 1;	//���ݱ�ʶ��
								break;
							}

							if( DI07.B08[2] == 0x07 ) DI07.B08[0] += 0x09;  //�������������������
							if( DI07.B08[0] == 0x00 )	//��һ��ʱ��������, ��1~14ʱ����ʼ���ڼ���ʱ�α��(14*3), MMDDNN
							{
								Temp32.B08[0] = 3;	//ʱ����ʼλ��
								Temp32.B08[1] = MAXSIDUANTB;	//��ʱ�α�����ֵ
								Temp32.B08[2] = MAXREGION;	//���ʱ����Ŀ
								Temp32.B08[3] = ee_shiqu_lenth;
							}
							else if( DI07.B08[0] <= 0x08 )	//��������8����ʱ�α�, ÿ����ʱ�α�̶�Ϊ14��ʱ��
							{
								Temp32.B08[0] = 1;	//ʱ����ʼλ��
								Temp32.B08[1] = 4;	//���ʺ���ֵ
								Temp32.B08[2] = MAXSIDUAN;	//�����ʱ����Ŀ
								Temp32.B08[3] = ee_shiduantable_lenth;
							}
							else
							{
								Temp32.B08[0] = 3;	//ʱ����ʼλ��
								Temp32.B08[1] = 4;	//���ݱ�������ֵ
								Temp32.B08[2] = 4;	//������ʱ����Ŀ
								Temp32.B08[3] = ee_jtshiqu_len;
							}
						
							//ʱ��ʱ��˳���ж�: Ҫ����ʱ����������
							Temp08 = (Uart[Ch].Buf[9]-12)/3;	//
							if( ((Uart[Ch].Buf[9]-12)%3) || (Temp08==0) ) break; //���ݳ��ȴ���, �˳� 
								
							if( Temp08 > Temp32.B08[2] )
							{
								k = 2;
								OutBuf[0] = 0xFF;
								if( DI07.B08[0]==0 ) OutBuf[1] = 0x10;//ʱ������(Bit4)
								else if( DI07.B08[0] <= 0x08 ) OutBuf[1] = 0x20;//ʱ������(Bit5)
								else OutBuf[1] = 0x01;
								break;
							}							
							
							if( Auth == KEYTYPE2 )
							{
								Inverse( Uart[Ch].Buf+22, (Uart[Ch].Buf[9]-12) );
								
								for( i=0; i<Temp08; i++ )
								{
									Inverse( Uart[Ch].Buf+22+3*i, 3 );
								}
							}

							memcpy( Buf, Uart[Ch].Buf+22, Uart[Ch].Buf[9]-12 );
							Sequence( Buf, Temp08 );

							//ʱ��˳���ж�: Ҫ����ʱ����������
							n = Temp08*3;	//Ϊ���ж���ʱ��15���Ӽ������, ��"��1��+�̶�ֵ"����"���1��"�ĺ��档
							Buf[n+0] = Buf[0];	//��ʱ�α�� or ���ʺ�
							Buf[n+1] = Buf[1];	//�� or ��
							Buf[n+2] = Hex_To_Bcd(Bcd_To_Hex(Buf[2])+24);	//�� or ʱ
							
							for( i=0, n=0; i<Temp08; i++, n+=3 ) //ʱ�����������һֱ��ͬ
							{
								if( (Buf[0+n]==0) || (Buf[0+n]>Temp32.B08[1]) )	//��ʱ�α��or���ʺ��Ƿ�Ϸ�
								{
									if( DI07.B08[0] && (DI07.B08[0]<=0x08) )
									{
										k = 2;
										OutBuf[0] = 0xFF;
										OutBuf[1] = 0x40;//��������(Bit6)
									}
									break;
								}
							
								result = JudgeClockNoWeek( Temp32.B08[0], Buf+1+n, 2 );	//minute+hour
								if( result != 0 ) break; //����
									
								result = Cmp_Bcd_Inv( Buf+1+n, Buf+4+n, 2 );
								if( result == 0x00 )	//A == B
								{
									if( Buf[3+n] != Buf[0+n] ) break;
								}
								else if( result == 0x02 ) //A < B
								{
									if( DI07.B08[0] && (DI07.B08[0]<=0x08))	//��ʱ�α�����Сʱ�μ����ӦС��15����
									{
										if( (CalcAbsMin(Buf+4+n)-CalcAbsMin(Buf+1+n)) < 15 ) break;
									}
								}
								else //A > B
								{
									break;	//��������
								}
							}
							
							if( i < Temp08 )	break;	//��������

							for( i=(Temp08); i<Temp32.B08[2]; i++ )	//���ݲ���, �����һ������
							{
								memcpy( Uart[Ch].Buf+22+3*i, Uart[Ch].Buf+22+3*(Temp08-1), 3 );
							}

							if( DI07.B08[0] <= 0x08 ) Temp16.B16 = ShiquRecord( DI07.B08, OptID.B08 );	//�¼���¼
							else Temp16.B16 = ee_jtshiqu1_page+DI07.B08[0]-0x09;
			
							I2C_Write_Eeprom( Temp16.B16, 0x00, Uart[Ch].Buf+22, Temp32.B08[3] );	//��̺� ʱ����orʱ�ε�i��(42)
			
							if( DI07.B08[2] == 0x07 ) DI07.B08[0] -= 0x09;  //�������������������
							
							run_timezone_flag = 1;
							Tariff.Crc16.B16 += 1;
							RefreshPrice();

							k = 1;												
							break;

						case 0x03:	//��n �����������ڼ���ʱ�α��
							switch( DI07.B08[1] ) 
							{
								case 0x00:
									if( (DI07.B08[0]>=1) && (DI07.B08[0]<=MAXHOLIDAY) )	//�������գ�1~14, YYMMDDNN, 4
									{
										if( (Uart[Ch].Buf[22]==0) || (Uart[Ch].Buf[22]>MAXSIDUANTB) ) break;	//��ʱ�α��: 1~8

										result = Judge_Clock( 4, &Uart[Ch].Buf[23], 3 ); //�ж������Ƿ�Ϸ�
										if( result != 0 ) break;
										
										Temp16.B08[0] = (DI07.B08[0]-1)/15;
										Temp16.B08[1] = ((DI07.B08[0]-1)%15)*4;
										
										result = I2C_Write_Eeprom( ee_holiday_page+Temp16.B08[0], Temp16.B08[1], Uart[Ch].Buf+22, 4 );
										if( result != 0x00 ) break;
										
										run_timezone_flag = 1;
										Tariff.Crc16.B16 += 1;
										
										k = 1;
									}
									else
									{
										Err_DI = 1;	//���ݱ�ʶ��
									}
									break;
									
								default:
									Err_DI = 1;	//���ݱ�ʶ��
									break;
							}
							break;

						case 0x04:
							switch( DI07.B08[1] )
							{
								case 0x01:	//�Զ�ѭ��
								case 0x02:	//����ѭ��
									//ѭ������(1)��ѭ��ʱ��(1)������С��λ��(1)������λ��(1)����������(1)��������(1)+ ��բ��ʱʱ��(2)��crc(2)
									if( Auth == KEYTYPE2 )
									{
										Temp08 = Uart[Ch].Buf[22];
										memcpy( Uart[Ch].Buf+22, Uart[Ch].Buf+23, 4);
										Uart[Ch].Buf[26] = Temp08;
									}
														
									if( (DI07.B08[0]) && (DI07.B08[0]<=SCREENNUM) )
									{
										result = GetLcdNo( Uart[Ch].Buf+22 );
										if( result == 0x00 ) break;	//��ʾ��Ŀ���� or ��ʾ��Ŵ�
																				
										n = 2*(DI07.B08[1]-1)+((DI07.B08[0]-1)/62);
										Temp08 = ((DI07.B08[0]-1)%62);
										
										Item[n].Display[Temp08] = result;
										Item[n].Crc16.B16 = Crc_16Bits( Item[n].Display, ee_displayitem_lenth );
										result = I2C_Write_Eeprom( ee_loop_displayitem_page+n, 0x00, Item[n].Display, ee_displayitem_lenth );
										if( result != 0x00 )
										{
											break;
										}
										
										k = 1;
									}
									else
									{
										Err_DI = 1;	//���ݱ�ʶ��
									}
									break;
								
								default:
									Err_DI = 1;	//���ݱ�ʶ��
									break;
							}
							break;
							
						case 0x09:
							switch( DI07.B08[1] )
							{
								case 0x08:
									if( DI07.B08[0]==1 )//�����¼�������������	NNN.N	2	A
									{		
										if( IsMultiBcd( Uart[Ch].Buf+22, 2) )	break;
										result = UpdateSysPara( 0x60+15, Uart[Ch].Buf+22, 2 );
										
										
										
									}
									else if( DI07.B08[0]==2 )//�����¼��ж���ʱʱ��	NN	1	S
									{
											
										if( IsMultiBcd( Uart[Ch].Buf+22, 1) || (Uart[Ch].Buf[22] < 10) ) break;
										result = UpdateSysPara( 0x60+17, Uart[Ch].Buf+22, 1 );
									}
									else
									{
										Err_DI = 1;	//���ݱ�ʶ��
										break;
									}
										
									if( result != 0x00 ) break;
											
									k = 1;
									break;

								default:
									Err_DI = 1;	//���ݱ�ʶ��
									break;
							}
							break;	
							
						case 0x80:    // :p		
							switch( DI07.B08[1] )
							{
								case 0x00:
									switch( DI07.B08[0] )
									{
										case 0x01:	//��������汾��(ASCII ��)	NN��NN	32
										case 0x02:	//����Ӳ���汾��(ASCII ��)	NN��NN	32
										case 0x03:	//���ұ���(ASCII ��)		NN��NN	32
										case 0x04:	//���������			NN��NN	8
											if( Auth != 3 )
											{
												k = 2;
												OutBuf[0] = 0xFF; //���
												OutBuf[1] = 0x04; //Error: bit4�������/δ��Ȩ
												break;
											}
											Temp08 = 32;//32�ֽ�
											if(DI07.B08[0] == 0x01)
											{
												Temp16.B08[0] = ee_softver_page;	//���������
												Temp16.B08[1] = ee_softver_inpage;
											}
											else if(DI07.B08[0] == 0x02)
											{
												Temp16.B08[0] = ee_hardver_page;	//Ӳ��������
												Temp16.B08[1] = ee_hardver_inpage;
											}
											else if(DI07.B08[0] == 0x03)
											{
												Temp16.B08[0] = ee_factorynum_page;	//���ұ��
												Temp16.B08[1] = ee_factorynum_inpage;
											}
											else
											{
												Temp16.B08[0] = ee_softbk_page; 	//���������
												Temp16.B08[1] = ee_softbk_inpage;
												Temp08 = 8;//8�ֽ�
											}
											if( Uart[Ch].Buf[9] != (Temp08+12) ) break;//���ݳ��ȴ���
											I2C_Write_Eeprom( Temp16.B08[0], Temp16.B08[1], Uart[Ch].Buf+22, Temp08 );
											k = 1;
											break;
																						
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
									break;
									
								default:
									Err_DI = 1;	//���ݱ�ʶ��
									break;
							}
							break;		
							
						default:
							Err_DI = 1;	//���ݱ�ʶ��
							break;
					}
					break;
				}

				case KEYTYPE1:	//ͨ������+MAC�ķ�ʽ�������ݴ��䣬����Ҫ����������֤��Ҳ����Ҫ��̼����ʹ�á�
					if( (DI07.B32 != 0x02040004) && 
					   (((BDMODE)&&(DI07.B32 == 0x0E040004)) || 
					    ((!BDMODE)&&(DI07.B32 != 0x0E040004))) )//�κ�ģʽ��֧�ֱ������, �ͻ��������ģʽ
					{
						Err_DI = 1;	//Error: bit2�������/δ��Ȩ
						break;
					}

					switch( DI07.B08[3] )
					{
						case 0x04:
							switch( DI07.B08[2] )
							{
								case 0x00:
									Temp32.B08[0] = 0x00;
									Temp32.B08[3] = 0x81;	//д��ESAM���ļ���
									
									switch( DI07.B08[1])
									{
										case 0x01:
											switch( DI07.B08[0] )
											{
												case 0x08:	//���׷��ʵ���л�ʱ��			YYMMDDhhmm	5	������ʱ��
													//����ʱ�����л�ʱ�䣨5����������ʱ�α��л�ʱ�䣨5�������׷��ʵ���л�ʱ�䣨5�������׽����л�ʱ�䣨5����crc��2��
													result = JudgeSwitch( Uart[Ch].Buf+22, 0 );
													if( result==1 ) break;
													
													Temp32.B08[0] = 5;	//���ݳ���
													Temp32.B08[1] = 10;	//82�ļ���ƫ����
													Temp32.B08[2] = 0xC0+10;	//E2��ҳ��ƫ����
													break;

												case 0x09:	//���׽����л�ʱ��		YYMMDDhhmm	5	������ʱ��
													//����ʱ�����л�ʱ�䣨5����������ʱ�α��л�ʱ�䣨5�������׷��ʵ���л�ʱ�䣨5�������׽����л�ʱ�䣨5����crc��2��
													result = JudgeSwitch( Uart[Ch].Buf+22, 0 );
													if( result==1 ) break;

													Temp32.B08[0] = 5;	//���ݳ���
													Temp32.B08[1] = 192;//84�ļ���ƫ����
													Temp32.B08[2] = 0xC0+15;	//E2��ҳ��ƫ����
													Temp32.B08[3] = 0x84;	//д��ESAM���ļ���
													break;
												
												default:
													Err_DI = 1;	//���ݱ�ʶ��
													break;
											}
											break;

										//�������1��ֵ(4)���������2��ֵ(4)+���������(6)�����(6)+�ͻ����(6)+�翨����(1)+�����֤ʱЧ(2)
										case 0x03:
											switch( DI07.B08[0] )
											{
												case 0x06:	//�������������              	NNNNNN	3       break;
													Temp32.B08[0] = 3;	//���ݳ���
													Temp32.B08[1] = 24;	//82�ļ���ƫ����
													Temp32.B08[2] = 0x40+8;	//E2��ҳ��ƫ����
													break;
																										
												case 0x07:	//��ѹ���������              	NNNNNN	3	    break;
													Temp32.B08[0] = 3;	//���ݳ���
													Temp32.B08[1] = 27;	//82�ļ���ƫ����
													Temp32.B08[2] = 0x40+11;	//E2��ҳ��ƫ����
													break;
												
												default:
													Err_DI = 1;	//���ݱ�ʶ��
													break;
											}
											break;
		
										case 0x04:
											switch( DI07.B08[0] )
											{
												case 0x02:	//���		NNNNNNNNNNNN	6
													if( Key_Status!=0 ){Err_DI = 1;break;}//��Կ��������

													Temp32.B08[0] = 6;	//���ݳ���
													Temp32.B08[1] = 30;	//82�ļ���ƫ����
													Temp32.B08[2] = 0x40+14;	//E2��ҳ��ƫ����
													break;
																										
												case 0x0E:	//�ͻ����	NNNNNNNNNNNN	6
													if( Key_Status!=0 ){Err_DI = 1;break;}//��Կ��������
													
													Temp32.B08[0] = 6;	//���ݳ���
													Temp32.B08[1] = 36;	//82�ļ���ƫ����
													Temp32.B08[2] = 0x40+20;	//E2��ҳ��ƫ����
													break;
																							
												default:
													Err_DI = 1;	//���ݱ�ʶ��
													break;
											}
											break;
											
										case 0x10:
											switch( DI07.B08[0] )
											{
												case 0x01:  //�������1 ��ֵ	XXXXXX.XX	4	Ԫ
													Temp32.B08[0] = 4;	//���ݳ���
													Temp32.B08[1] = 16;	//82�ļ���ƫ����
													Temp32.B08[2] = 0x40+0;	//E2��ҳ��ƫ����
													break;
													
												case 0x02:  //�������2 ��ֵ	XXXXXX.XX	4	Ԫ
													Temp32.B08[0] = 4;	//���ݳ���
													Temp32.B08[1] = 20;	//82�ļ���ƫ����
													Temp32.B08[2] = 0x40+4;	//E2��ҳ��ƫ����
													break;
		
												default:
													Err_DI = 1;	//���ݱ�ʶ��
													break;
											}
											break;
											
										default:
											Err_DI = 1;	//���ݱ�ʶ��
											break;
									}
					
									if( Temp32.B08[0] )
									{
										if( IsMultiBcd( Uart[Ch].Buf+22, Temp32.B08[0] ) )	break;
	
//										result = Update_ESAM_Bin_Mac( Temp32.B08[3], Temp32.B08[1], Uart[Ch].Buf+22, Temp32.B08[0] );
//										if( result != 0x00 )	//�����������MAC У����ǲ��ô�����Ϣ��ERR ��bit2�������/δ��Ȩ����λ���������쳣Ӧ��֡��Ӧ��
//										{
//											k = 0x00; 
//											OutBuf[ k++ ] = 0xFF;
//											OutBuf[ k++ ] = 0x04;	//�����/δ��Ȩ
//											break;
//										}
										
										result = UpdateSysPara( Temp32.B08[2], Uart[Ch].Buf+22, Temp32.B08[0] ); //����XRAM, E2PROM
										if( result ) break;
										
										run_price_flag = 1;

										k = 1;
									}
									break;
									
								case 0x05:	//���ñ����׷��ʵ��
									if( (DI07.B08[1] != 0x02) || (DI07.B08[0] != 0xFF) ) { Err_DI = 1;	break; } //���ݱ�ʶ��

									Temp16.B08[0] = (Uart[Ch].Buf[9]-16);
									Temp16.B08[1] = Temp16.B08[0]/4;
									if( (Temp16.B08[0]%4) || (Temp16.B08[1]==0) || (Temp16.B08[1]>MAXFLPRICE) ) break; //���ݳ��ȴ���, �˳� 
									if( IsMultiBcd( Uart[Ch].Buf+22, Temp16.B08[0] ) )	break;

									for( i=0; i<Temp16.B08[1]; i++ ) Inverse( Uart[Ch].Buf+22+4*i, 4 );
							
									memcpy( Buf, Uart[Ch].Buf+22, ee_tariffprice_lenth );	//�洢���������

//									Inverse( Uart[Ch].Buf+22, Temp16.B08[0] );	//Ϊ�˷���MACУ�麯���ĸ�ʽ
//									result = Update_ESAM_Bin_Mac( 0x84, 0x04, Uart[Ch].Buf+22, Temp16.B08[0] );
//									if( result != 0x00 )	//�����������MAC У����ǲ��ô�����Ϣ��ERR ��bit2�������/δ��Ȩ����λ���������쳣Ӧ��֡��Ӧ��
//									{
//										k = 0x00; 
//										OutBuf[ k++ ] = 0xFF;
//										OutBuf[ k++ ] = 0x04;	//�����/δ��Ȩ
//										break;
//									}
									
									FLRecord( OptID.B08 );	//���ʱ�̼�¼

									//E2PROM��ֻ����ǰ4����ʵ��
									for( i=Temp16.B08[1]; i<ee_tariffprice_lenth/4; i++ )	//���ݲ���, �����һ������
									{
										memcpy( Buf+4*i, Buf+4*(Temp16.B08[1]-1), 4 );
									}

									result = I2C_Write_Eeprom( ee_tariffprice2_page, 0x00, Buf, ee_tariffprice_lenth );	//�����ʽ
									if( result != 0x00 ) break;
							   
//									I2C_Read_Eeprom( ee_tariffprice2_page, 0x00, Buf, ee_tariffprice_lenth );
									run_price_flag = 1;
									
									k = 1;
									break;

								case 0x06:	//���ñ����׽���ֵ+���ݵ��+�������
									if( (DI07.B08[1] < 0x0a) || (DI07.B08[1] > 0x0b) || (DI07.B08[0] != 0xFF) ) { Err_DI = 1;	break; } //���ݱ�ʶ��

									if( Uart[Ch].Buf[9] != 70+12 ) break;	//6*4+7*4+4*3=64
									if( IsMultiBcd( Uart[Ch].Buf+22, 70 ) )	break;

									for( i=0; i<13; i++ ) Inverse( Uart[Ch].Buf+22+4*i, 4 );
									for( i=0; i< 6; i++ ) Inverse( Uart[Ch].Buf+74+3*i, 3 );
									
									for( i=1; i< 6; i++ ) //���ݵ��������ж�(������������Ҫ��)
									{
										if( Cmp_Data( Uart[Ch].Buf+22+4*(i-1), Uart[Ch].Buf+22+4*i, 4 ) == 1 ) break;  //1,A>B
									}
									if( i != 6 ) break;

									memcpy( Buf, Uart[Ch].Buf+22, 70 );	//�洢���������
									
////									if( DI07.B08[1] < 0x0c )
////									{
////										Inverse( Uart[Ch].Buf+22, 70 );	//Ϊ�˷���MACУ�麯���ĸ�ʽ
////										
//////										result = Update_ESAM_Bin_Mac( 0x84, 0x04+12*4+(DI07.B08[1]-0x0a)*70, Uart[Ch].Buf+22, 70 );
//////										if( result != 0x00 )	//�����������MAC У����ǲ��ô�����Ϣ��ERR ��bit2�������/δ��Ȩ����λ���������쳣Ӧ��֡��Ӧ��
//////										{
//////											k = 0x00; 
//////											OutBuf[ k++ ] = 0xFF;
//////											OutBuf[ k++ ] = 0x04;	//�����/δ��Ȩ
//////											break;
////									}

									JTRecord( OptID.B08 );	//���ݱ�̼�¼
							
									for( i=0; i< 6; i++ ) Inverse( Buf+52+3*i, 3 );

									WriteE2WithJieti( DI07.B08[1]-0x06, Buf );
									
									RefreshPrice();
									
									k = 1;
									break;	
									
								default:
									Err_DI = 1;	//���ݱ�ʶ��
									break;
							}
							break;
							
						default:
							Err_DI = 1;	//���ݱ�ʶ��
							break;
					}
					break;
				
				default:
					Err_DI = 1;	//Error: bit2�������/δ��Ȩ
					break;
			}

			if( Err_DI )	//�û��Բ�֧��д�������������̣������쳣Ӧ��֡����������Ϣ��Bit2��1�����������/δ��Ȩ����
			{
				k = 2;
				OutBuf[0] = 0xFF;	//Operation result.
				OutBuf[1] = 0x04;	//Error: bit2�������/δ��Ȩ
				break;
			}
						
			if( k && (OutBuf[0] == 0x00) )
			{
				ProgramRecord( DI07.B08, OptID.B08 );
			}
			break;

		//=====================================================================================
		case 0x11:	//��ȡ
			//����������̼�¼����
			
			if( (Uart[Ch].Buf[9] == 0x09) && (DI07.B32 == 0x00030404) )//Һ���鿴
			{
				if(progenable&&(Uart[Ch].Buf[14]==0xbb)&&(Uart[Ch].Buf[15]==0xbb)&&(Uart[Ch].Buf[16]==0xbb)&&(Uart[Ch].Buf[17]==0xbb)) result = 234;
				else if(progenable&&(Uart[Ch].Buf[14]==0xee)&&(Uart[Ch].Buf[15]==0xee)&&(Uart[Ch].Buf[16]==0xee)&&(Uart[Ch].Buf[17]==0xee)) result = 235;
				else if(progenable&&(Uart[Ch].Buf[14]==0xdd)&&(Uart[Ch].Buf[15]==0xdd)&&(Uart[Ch].Buf[16]==0xdd)&&(Uart[Ch].Buf[17]==0xdd)) result = 236;
				else
				{
					result = GetLcdNo( Uart[Ch].Buf+14 );
					if( result == 0x00 )	break;//��ʾ��Ŀ���� or ��ʾ��Ŵ�
				}
				
				Ext_Disp_Cnt = result;
				Display.Timer = 10;
				Display.Status &= B1111_1110;	//����ѭ��״̬
				Ic_Type_Reg = 0;//��忨��ʾ��ر�־
				
				Display.Number  = 0xfc;//Һ���鿴��ʾ
				Lcdled.B16 = 10;//��������
				if( (Ext_Disp_Cnt == 234) || (Ext_Disp_Cnt == 235) || (Ext_Disp_Cnt == 236) )
				{
//					GPIO27CFG = B0100_0000;//���
//					GPIO2 &= B0111_1111;//����͵�ƽ���������
				}
				if( Ext_Disp_Cnt == 200 )Init_Led_Timer = 10;//ȫ����ʾ����led��
				
				k = 6;
				memcpy( OutBuf+1, Uart[Ch].Buf+14, 5 );
				break;
			}
			
			if( Uart[Ch].Flag&B0000_0001 ) break; //ͨѶ��ַȫ99   ��֧��
			
			if( ((Uart[Ch].Buf[9] == 0x0A) || (Uart[Ch].Buf[9] == 0x05)) && (DI07.B08[3] == 0x06) )
			{
				if( (DI07.B08[2] == 0x10) && (DI07.B08[1]) && (DI07.B08[1] <= 0x08) )
				{
					if( Uart[Ch].Buf[9] != 0x0A ) break;
					if( IsMultiBcd( Uart[Ch].Buf+14, Uart[Ch].Buf[9]-0x04 ) ) break;
					if( JudgeClockNoWeek( 1, Uart[Ch].Buf+15, 5 ) )break;
				
					k = ReadLoadRec( Ch, Uart[Ch].Buf+8, OutBuf );
					if( k == 0 ) {RdLoadPara[Ch][4] = 0; k++;}
					else if(RdLoadPara[Ch][4]) Remain = 1;
					break;
				}
			}
			else if( (Uart[Ch].Buf[9] == 0x11)||(Uart[Ch].Buf[9] == 0x07) )//У�����
			{
				if(progenable)
				{
					if( DI07.B08[0] != 0x01)
					{							
						Inverse( Uart[Ch].Buf+12, 3 );
						Inverse( Uart[Ch].Buf+16, 3 );
						Inverse( Uart[Ch].Buf+19, 4 );
						Uart[Ch].Buf[11] = 0;
						Uart[Ch].Buf[15] = 0;
					}
					Uart[Ch].Buf[10] = 0xff;
				  Comm_Write_Reg(&Uart[Ch].Buf[10]);
			
					Uart[Ch].Overtime_Timer = 2;	//����Ӧ���ֽڷ���
					return;
				}
			}
			else if( Uart[Ch].Buf[9] != 0x04 ) break;	//���ȴ���
			else if( DI07.B32 == 0x5e000002 )
			{
				k = 2;
				OutBuf[0] = 0x00;
				if( progenable ) OutBuf[1] = 0x55;	//0x55��ʾ����״̬,0x00��ʾ����״̬
				else OutBuf[1] = 0;
				break;
			}
					
			switch( DI07.B08[3] )
			{
				case 0x00:	//��ǰ�ͽ����� ��������
					switch( DI07.B08[2] )
					{
						case 0x00:	//���
						case 0x01:	//����
						case 0x02:	//����
							//�������: ��ʱ����ʱ��(5)+�����й��ܵ���(5*4)+�����й��ܵ���(5*4)+��ʱ�����������(3)
							result = Cal_Comb( DI07.B08[2], DI07.B08[1], DI07.B08[0], OutBuf+1 );
							if( (result == 0x00) || (result > 13*4) ) break;	//��������ݱ�ʶ

							k = 0x01+result;
							break;
						
						case 0x90:
							switch( DI07.B08[1] )
							{
								case 0x02:
									switch( DI07.B08[0] )
									{
										case 0x00:	//����ǰ��ʣ����
										case 0x01:	//����ǰ��͸֧���
											if( !BDMODE ) break;
												
											if( DI07.B08[0] == 0x00 )
											{
												memcpy( OutBuf+1, Money.Remain+2, 4);
											}
											else
											{
												memcpy( OutBuf+1, Money.Tick+2, 4);
											}
											
											k = 5;
											break;
										
										default:
											break;
									}								
									break;
									
								default:
									break;
							}
							break;
						
						case 0x0B:	//��ǰ/��1���������õ���
						case 0x0C:	//��ǰ/��1�¶��õ���
							if( DI07.B08[0] > 0x01 ) break;
						case 0x0D:	//��ǰ/��1~6����õ���
							if( (DI07.B08[1] != 0x00) || (DI07.B08[0] > 0x06) ) break;
							if( (!BDMODE) && (DI07.B08[2] == 0x0D) ) break;

							Cal_Comb_Usedl( DI07.B08[2]-0x0B, DI07.B08[0], OutBuf+1 );
							
							if( DI07.B08[2] == 0x0D )
							{
								k = 9;
							}
							else
							{
								k = 5;
								memcpy( OutBuf+1, OutBuf+5, 4 );//���践�ؽ���ʱ��
							}
							break;
							
						case 0xFE:	//����ʱ��
							if( (DI07.B08[1] != 0x00) || (DI07.B08[0] == 0) || ((DI07.B08[0]>0x0C)&&(DI07.B08[0]!=0xFF)) ) break;
							
							result = Cal_Comb( 0x03, 0x00, DI07.B08[0], OutBuf+1 );	//���ؽ���ʱ��
							
							k = 0x01+result;
							break;
						
						default:
							break;
					}
					break;
				
				case 0x01:	//��ǰ�ͽ����� �������������ʱ��
					switch( DI07.B08[2] )
					{
						case 0x01:	//����
						case 0x02:	//����
							if( (DI07.B08[1]>4) && (DI07.B08[1]!=0xFF) ) break;
								
							if( DI07.B08[0] <= 12 )
							{
								Temp16.B08[0] = DI07.B08[0];
								Temp16.B08[1] = DI07.B08[0]+1;
							}
							else if( DI07.B08[0] == 0xFF )
							{
								if( DI07.B08[1] == 0xFF ) break;
						
								Temp16.B08[0] = 0;
								Temp16.B08[1] = 13;
							}
						
							k = 0x01;
							OutBuf[0] = 0x00;
						
							Temp08 = DI07.B08[0];
							for( n=Temp16.B08[0]; n<Temp16.B08[1]; n++ )
							{
								DI07.B08[0] = n;
						
								ReadMaxDemand( DI07.B08, Buf );
								
								memcpy( OutBuf+k, Buf+1, Buf[0] );
								k += Buf[0];
							}
							DI07.B08[0] = Temp08;

							break;

						default:
							break;
					}
					break;
				
				case 0x02:	//�������ݱ�ʶ
					if( DI07.B08[2] == 0x80 )
					{
						if( DI07.B08[1] != 0x00 ) break;

						if( (!BDMODE) && ( (DI07.B08[0] == 0x0b) || (DI07.B08[0]==0x20) || (DI07.B08[0]==0x21))) break;	//Զ��ģʽ��֧�ֳ������ز���
							
						switch( DI07.B08[0] )
						{
							case 0x01:	//���ߵ���	XXX.XXX	3	A
								Temp08 = 3;
								Temp32.B32 = Emu_Data.I2.B32;
								if((dl_back_flag) && (Emu_Data.I2.B32)) Temp32.B08[1] |= 0x80;
								break;
							
							case 0x02:	//����Ƶ��	XX.XX	2	Hz
								Temp08 = 2;
								Temp32.B32 = Emu_Data.Freq.B32;
								break;
								
							case 0x03:	//һ�����й���ƽ������	XX.XXXX	3	kW
								Temp08 = 3;
								Temp32.B32 = Emu_Data.Pav.B32;
								break;
					    
							case 0x04:	//��ǰ�й�����	XX.XXXX 3	kW
								Temp08 = 3;
								Temp32.B32 = Current_Demand.B32;
								if( dl_back_flag )	Temp32.B08[2] |= 0x80;
								
								break;
					
							case 0x07:	//�����¶�	XXX.X	2	��
								Temp08 = 2;
								Temp32.B32 = Fun_Para.Temper_M.B16;
								break;
								
							case 0x08:	//ʱ�ӵ�ص�ѹ(�ڲ�)	XX.XX	2	V
							case 0x09:
								Temp08 = 2;
								Temp32.B32 = Fun_Para.Voltage_B.B16;
								break;
								
//							case 0x09:	//ͣ�糭���ص�ѹ (�ⲿ)	XX.XX	2	V
//								break;
								
							case 0x0A:	//�ڲ���ع���ʱ��	XXXXXXXX	4	��
								Temp08 = 4;
								Save_Keep_Timer(0);
								Hex2Bcd( Eeprom.Buffer+16, Temp32.B08, 4 );
								break;
							
							case 0x0B:	//��ǰ���ݵ��	XXXX.XXXX	4	Ԫ/kWh
								Temp08 = 4;
								Temp32.B32 = Price.Current_L.B32;
							  Exchange( Temp32.B08,Price.Current_L.B08, 4 );
								break;
							
						
							case 0x0E:	//���ߵ���		XXX.XXX 3 A
								Temp08 = 3;
								Temp32.B32 = Emu_Data.I1.B32;
								if( dl_back_flag )	Temp32.B08[1] |= 0x80;
								break;
								
							case 0x0F:	//�����й�����	XX.XXXX 3 kW
								Temp08 = 3;
								Temp32.B32 = Emu_Data.P1.B32;
								if( dl_back_flag )	Temp32.B08[1] |= 0x80;
								break;
								
							case 0x10:	//���߹�������	X.XXX 2
								Temp08 = 2;
								Temp32.B32 = Emu_Data.Cosa.B32;
								if( dl_back_flag )	Temp32.B08[2] |= 0x80;
								break;
							
							case 0x20:	//��ǰ���
								Temp08 = 4;
								Temp32.B32 = Price.Current.B32;								
								break;
								
							case 0x21:	//��ǰ���ʵ��
								Temp08 = 4;
								Temp32.B32 = Price.Current_F.B32;
							  Exchange( Temp32.B08,Price.Current_F.B08, 4 );
								break;
								
							case 0x22:	//�����֤ʱЧʣ��ʱ��	XXXX 	2	����
							case 0x23:	//������֤ʱЧʣ��ʱ��	XXXX 	1  	����
								if( DI07.B08[0] == 0x22 )
								{
									Temp32.B32 = Identitytimer.B32;
									Temp08 = 2;
								}
								else
								{
									Temp32.B32 = IR_EnTimer;
									Temp08 = 1;
								}
								
								Temp32.B32 = (Temp32.B32+59)/60;
								Hex2Bcd( Temp32.B08, Temp32.B08, 4 );
								break;
								
							default:
								Temp08 = 0x00;
								break;
						}
						
						if( Temp08 )
						{	
							memcpy( OutBuf+1, Temp32.B08, 4 );
							k = 1+Temp08;
						}
					}
					else
					{
						if( DI07.B08[0] != 0x00 ) break;
						
						Temp08 = 0x00;
						switch( DI07.B08[2] )
						{
							case 0x01:	//A���ѹ	XXX.X	2
							case 0x02:	//A�����	XXX.XXX	3
								if( (DI07.B08[1] == 0x01) || (DI07.B08[1] == 0xFF) )
								{
									if( DI07.B08[2] == 0x01 ) Temp08 = 2;
									else Temp08 = 3;
								}
								break;

							case 0x03:	//˲ʱ���й�����	XX.XXXX	3
							case 0x05:	//˲ʱ�����ڹ���	XX.XXXX	3
							case 0x06:	//�ܹ�������		X.XXX	2
								if( (DI07.B08[1] <= 0x01) || (DI07.B08[1] == 0xFF) )
								{
									if( DI07.B08[2] == 0x06 ) Temp08 = 2;
									else Temp08 = 3;
								}
								break;
								
							default:
								break;
						}
						
						if( Temp08 )
						{
							memcpy( OutBuf+1, Emu_Data.U.B08+(DI07.B08[2]-1)*4, 4 );
							
							if( dl_back_flag && (
							(DI07.B08[2]==0x02) ||
							(DI07.B08[2]==0x03) ||
							(DI07.B08[2]==0x06) ) )	OutBuf[Temp08] |= 0x80;
							
							k = 1+(Temp08);
							if( DI07.B08[1] == 0xFF )
							{
								if(DI07.B08[2] >= 0x03 )
								{
									memcpy( OutBuf+k, OutBuf+1, (Temp08) );
									k += Temp08;
								}

								memset( OutBuf+k, 0xFF, 2*(Temp08) );
								k += 2*(Temp08);
							}
						}
					}
					break;
					
				case 0x03:	//�¼���¼���ݱ�ʶ				
					RdHisEvent( DI07.B08, 0, OutBuf );
					if( OutBuf[0] )
					{			
						if( (DI07.B08[1] == 0x02) && DI07.B08[0] ) Remain = 1;
						if( (DI07.B08[1] == 0x05) && DI07.B08[0] ) Remain = 1;
						if( (DI07.B08[1] == 0x10) && DI07.B08[0] ) Remain = 1;
						
						k = OutBuf[0]+1;
						OutBuf[0] = 0x00;
						break;
					}
					else if( DI07.B32 == 0x00143003 )//�Ƿ��忨����
					{		
						k = 4;
						Get_Keep_Timer( 0, OutBuf );
						OutBuf[0] = 0x00;
						break;
					}
					else if( (DI07.B08[2] != 0x11) && (DI07.B08[2] != 0x33) ) break;
					
				case 0x19:	//������¼
				case 0x1D:	//��բ��¼����բ��¼
				case 0x1E:	//��բ��¼
				case 0x22:	//���Ƿѹ��¼
					Rd_Relay_Buy_Rec( DI07.B08, OutBuf );
					if( OutBuf[0] == 0x00 ) break;
					
					k = OutBuf[0]+1;
					OutBuf[0] = 0x00;	//���
					break;

				case 0x04:	//�α������ݱ�ʶ
					switch( DI07.B08[2] )
					{
						case 0x00:	
							switch( DI07.B08[1] ) 
							{
								case 0x01:	
									switch( DI07.B08[0] )
									{
										case 0x01:	//���ڼ�����(����0����������)	YYMMDDWW  	4	����������	
											k = 5;
											OutBuf[1] = Sclock.Clockmain[3];
											OutBuf[2] = Sclock.Clockmain[4];
											OutBuf[3] = Sclock.Clockmain[5];
											OutBuf[4] = Sclock.Clockmain[6];
											break;
											
										case 0x02:	//ʱ��                          hhmmss    	3	ʱ����      
											k = 4;
											OutBuf[1] = Sclock.Clockmain[0];
											OutBuf[2] = Sclock.Clockmain[1];
											OutBuf[3] = Sclock.Clockmain[2];
											break;
										
										case 0x03:	//�����������                  NN        	1	��
											k = 2;
											OutBuf[0] = 0x00;
											OutBuf[1] = Hex_To_Bcd(Demand_Para.Max_Demand_Period);
											break;
											
										case 0x04:	//����ʱ��                      NN        	1	��
											k = 2;
											OutBuf[0] = 0x00;
											OutBuf[1] = Demand_Para.Sliding_Time;
											break;										
											
										case 0x05:	//У��������                  XXXX      	2	����        
											k = 3;
											OutBuf[1] = Hex_To_Bcd(Def_Plusevalidtimer*4);
											OutBuf[2] = 0x00;
											break;
											
										case 0x08:	//���׷��ʵ���л�ʱ��			YYMMDDhhmm	5	������ʱ��, ����+MAC
										case 0x09:	//���׽����л�ʱ��			YYMMDDhhmm	5	������ʱ��, ���ģ�MAC
										case 0x0A:	//���׽���ʱ���л�ʱ��	YYMMDDhhmm 5
											if( !BDMODE ) break;
										case 0x06:	//����ʱ�����л�ʱ��            YYMMDDhhmm	5	������ʱ��, ���ģ�MAC
										case 0x07:	//������ʱ���л�ʱ��            YYMMDDhhmm	5	������ʱ��, ���ģ�MAC
//										case 0x0B:	//���׷ѿ�ģʽ�л�ʱ��	YYMMDDhhmm 5
											k = 6;
											Exchange( OutBuf+1, Starttime.Shiqu+5*(DI07.B08[0]-0x06), 5 );
											break;
											
										case 0x0C:	//���ڡ�����(����0����������)��ʱ��	YYMMDDWWhhmmss	7 ����������ʱ����
											k = 8;
											memcpy( OutBuf+1, Sclock.Clockmain, 7 );
											break;
											
										default:
											break;
									}										
									break;
									
								case 0x02:	  
									//��ʱ������1������ʱ�α�����1������ʱ���� ��1������������1����������������1��+г������������1��+��������1������Կ����1����crc��2��
									if( (DI07.B08[0]) && (DI07.B08[0]<=ee_feiknumber_lenth) )
									{
										if( (!BDMODE) && (DI07.B08[0] == 0x07) ) break;
											
										k = 2;
										OutBuf[1] = Feik.Num[DI07.B08[0]-1];
										if( DI07.B08[0] == 5 )	//����������
										{
											OutBuf[2] = 0x00;
											k += 1;
										}
									}
									break;
									
								case 0x03:
									switch( DI07.B08[0] )
									{
										case 0x01:	//�Զ�ѭ����ʾ����            	NN      1   ��	
										case 0x02:	//ÿ����ʾʱ��					NN		1	��	
										case 0x03:	//��ʾ����С��λ��				NN		1	λ	
										case 0x04:	//��ʾ���ʣ����������С��λ��	NN      1   λ  
										case 0x05:	//����ѭ����ʾ����            	NN      1   ��  
										case 0x08:	//�ϵ�ȫ��ʱ��	NN	1	λ
//											if( DI07.B08[0] == 0x08 ) Temp32.B08[2] = 0x05;
//											else Temp32.B08[2] = DI07.B08[0]-1;
											
											k = 2;
											OutBuf[1] = Hex_To_Bcd(*(&Display_Para.S_Number+DI07.B08[0]-1));
											break;
											
										case 0x06:	//�������������              	NNNNNN	3       break;
										case 0x07:	//��ѹ���������              	NNNNNN	3	    break;
											//�������1��ֵ(4)���������2��ֵ(4)+���������(6)�����(6)+�ͻ����(6)+�翨����(1)+�����֤ʱЧ(2)
											k = 4;
											Exchange( OutBuf+1, Esam_Para.Current_Ratio+3*(DI07.B08[0]-6), 3 );
											break;

										default:	
											break;
									}
									break;
									
								case 0x04:	  
									switch( DI07.B08[0] )
									{
										case 0x01:	//ͨ�ŵ�ַ                     	NNNNNNNNNNNN	6
											k = 7;
											Exchange( OutBuf+1, Comm_Addr.Comm_Id, 6 );
											break;

										case 0x02:	//���                         	NNNNNNNNNNNN	6
											k = 7;
											Exchange( OutBuf+1, Esam_Para.Meter_Id, 6 );
											break;
											
										case 0x04:	//���ѹ��ASCII �룩         	XXXXXXXXXXXX	6          
											k = 7;
											Exchange( OutBuf+1, (uint08*)RatedVolt, 6 );
											break;	
											
										case 0x05:	//�����/����������ASCII �룩XXXXXXXXXXXX	6
										case 0x06:	//��������ASCII �룩         XXXXXXXXXXXX	6
										case 0x07:	//���ȵȼ���ASCII �룩         XXXXXXXX   	4
										case 0x0B:	//����ͺţ�ASCII �룩         XX��XX      	10 
										case 0x0C:	//�������ڣ�ASCII �룩         XX��XX      	10
										case 0x0D:	//Э��汾�ţ�ASCII �룩        XX��XX      	16
										case 0x03:	//�ʲ�������루ASCII �룩     NN��NN      	32	
										case 0x0F:	//���ܱ�λ����Ϣ				11
											if(DI07.B08[0] == 0x05)
											{
												Code_Flash_Char = (uint08*)RatedCurr;
												Temp16.B08[0] = ee_ratedcurr_page;//�����
												Temp16.B08[1] = ee_ratedcurr_inpage;
												Temp08 = 6;//6�ֽ�
											}
											else if(DI07.B08[0] == 0x06)
											{
												Code_Flash_Char = (uint08*)MaxCurr;
												Temp16.B08[0]=ee_maxcurr_page;//������
												Temp16.B08[1] = ee_maxcurr_inpage;
												Temp08 = 6;//6�ֽ�
											}
											else if(DI07.B08[0] == 0x07)
											{
												Code_Flash_Char = (uint08*)ActiveClass;
												Temp16.B08[0] = ee_activeclass_page; //�й����ȵȼ�
												Temp16.B08[1] = ee_activeclass_inpage;
												Temp08 = 4;
											}
											else if(DI07.B08[0] == 0x0B)
											{
												Code_Flash_Char = (uint08*)MaterType;
												Temp16.B08[0] = ee_matertype_page; //����ͺ�
												Temp16.B08[1] = ee_matertype_inpage;
												Temp08 = 10;
											}
											else if(DI07.B08[0] == 0x0c)
											{
												Code_Flash_Char = (uint08*)Facdate;
												Temp16.B08[0] = ee_meterdate_page;//��������
												Temp16.B08[1] = ee_meterdate_inpage;
												Temp08 = 10;
											}
											else if(DI07.B08[0] == 0x0D)
											{
												Code_Flash_Char = (uint08*)ProtocolVer;
												Temp16.B08[0] = ee_protocolver_page;//Э��汾��
												Temp16.B08[1] = ee_protocolver_inpage;
												Temp08 = 16;
											}
											else if(DI07.B08[0] == 0x03) 
											{
												Temp16.B08[0] = ee_zichannum_page;//�ʲ�������
												Temp16.B08[1] = ee_zichannum_inpage;
												Temp08 = 32;
											}
											else
											{
												Temp16.B08[0] = ee_meterpos_page;//���ܱ�λ����Ϣ
												Temp16.B08[1] = ee_meterpos_inpage;
												Temp08 = 11;
											}
											result = I2C_Read_Eeprom( Temp16.B08[0], Temp16.B08[1], OutBuf+1, Temp08 );
																						
											for( i=0; i<Temp08; i++ )	//���ȫ��Ϊ0xff�����Ĭ�ϵ�ֵ
											{
												if( OutBuf[1+i] != 0xff ) break;
											}
											
											if( (result || (i==Temp08)) && (DI07.B08[0] != 0x03) && (DI07.B08[0] != 0x0f) )
											{
												Exchange( OutBuf+1, Code_Flash_Char, Temp08 );  
											}
											k = Temp08+1;
											break; 
										
//										case 0x08:	//�޹�׼ȷ�ȵȼ���ASCII �룩    XXXXXXXX    	4	            
//											break;    											          	

										case 0x09:	//����й�����                  XXXXXX      	3	imp/kWh
											k = 4;
											memcpy( OutBuf+1, Meter.Const.B08, 3 );//ת���ɵ��ֽ���ǰ
											break;
											
//										case 0x0A:	//����޹�����                  XXXXXX      	3   imp/kvarh   
//											k = 4;
//											OutBuf[1] = 0x00;
//											OutBuf[2] = 0x32;
//											OutBuf[3] = 0x00;
//											break;
											
										case 0x0E:	//�ͻ����						NNNNNNNNNN		6
											k = 7;
											Exchange( OutBuf+1, Esam_Para.User_Id, 6 );
											break;

										default:	break;                                      		
									}
									break;
									
								case 0x05:	  
									switch( DI07.B08[0] )
									{
										case 0xFF:	//�������״̬�ֿ�	XXXX	14	
										case 0x01:	//�������״̬��1   XXXX	2
											//[0].B16	�������״̬��1					
											//bit15:	ʱ�ӹ���
											//bit14:	͸֧״̬
											//bit13:	�洢������
											//bit12:	�ڲ��������Ԥ����
											//bit09:	ESAM��
											//bit08:	���ƻ�·����
											//bit07:	reserved.
											//bit06:	�ѿ�ģʽ״̬  (0���أ�1Զ��)
											//bit05:	�޹����ʷ���	(0 ����1 ����)
											//bit04:	�й����ʷ���	(0 ����1 ����)
											//bit03:	ͣ�糭����	(0 ������1 Ƿѹ)
											//bit02:	ʱ�ӵ��		(0 ������1 Ƿѹ)    
											//bit01:	�������㷽ʽ	(0 ���1 ����)
											//bit00:	reserved.
											Temp16.B16 = 0x00;
											
											if( clock_error )	//bit15:	ʱ�ӹ���
											{
												Temp16.B08[0] |= B1000_0000;
											}
											
											if( remainder_tick_flag )	//bit14:	͸֧״̬
											{
												Temp16.B08[0] |= B0100_0000;
											}

											if( eeprom_error_flag )	//bit13:	�洢������
											{
												Temp16.B08[0] |= B0010_0000;
											}
											
											if( esam_error )	//bit09:	ESAM ״̬		(0 ������1 ����)
											{
												Temp16.B08[0] |= B0000_0010;
											}
											
											if( relay_error )	//bit08:	���ƻ�·����
											{
												Temp16.B08[0] |= B0000_0001;
											}
											
											if( !BDMODE )	//bit06:	�ѿ�ģʽ״̬
											{
												Temp16.B08[1] |= B0100_0000;
											}
																						
											if( dl_back_flag )			//�й����ʷ���(0 ����1 ����)
											{
												Temp16.B08[1] |= B0001_0000;
											}

											if( low_voltage_flag || Bat_Delay_Timer )		//ͣ�糭����(0 ������1 Ƿѹ)+ʱ�ӵ��(0 ������1 Ƿѹ)
											{
												Temp16.B08[1] |= B0000_1100;
											}

											k = 0x00;
											OutBuf[ k++ ] = 0x00;
											OutBuf[ k++ ] = Temp16.B08[1];
											OutBuf[ k++ ] = Temp16.B08[0];

											if( DI07.B08[0] == 0x01 )
											{
												break;
											}
											
										case 0x02:	//�������״̬��2	XXXX	2	
											Temp16.B16 = 0x00;
											if( dl_back_flag )	//bit0��A ���й����ʷ���( 0 ����1 ���� )
											{
												Temp16.B08[1] |= B0000_0001;
											}
											
											if( DI07.B08[0] == 0xFF )
											{
												OutBuf[ k++ ] = Temp16.B08[1];
												OutBuf[ k++ ] = Temp16.B08[0];
											}
											else
											{
												k = 3;
												OutBuf[1] = Temp16.B08[1];
												OutBuf[2] = Temp16.B08[0];
												break;
											}
											
										case 0x03:	//�������״̬��3	XXXX	2	
											//[2].B16	�������״̬��3(������)
											//bit15:	Զ�̿���(0����,1δ����)
											//bit14:	���ؿ���(0����,1δ����)
											//bit13:	�����֤״̬(0ʧЧ��1��Ч)
											//bit12:	����״̬	(0�Ǳ���,1����)
											//bit11:	��ǰ����	(0��ǰ�׽��ݣ�1�����׽���)
											//bit10:	��ǰ���ݱ�(0��1�Ž��ݱ�1��2�Ž��ݱ�)
											//bit9~8:	���ܱ�����, 		(00 ��Ԥ���ѱ�,01 ������Ԥ���ѱ�,10 �����Ԥ���ѱ�)	
											//bit7:		Ԥ��բ����״̬		(0 �ޣ�1 ��)
											//bit6:		�̵�������״̬		(0 ͨ��1 ��)
											//bit5:		��ǰ����ʱ��		(0 ��һ�ף�1 �ڶ���)
											//bit4:		�̵���״̬			(0 ͨ��1 ��)
											//bit3:		�������			(0 ��ֹ��1 ���)
											//bit2~1:	���緽ʽ			(00 ����Դ��01 ������Դ��10 ��ع���)
											//bit0:		��ǰ����ʱ��		(0 ��һ�ף�1 �ڶ���)
											Temp16.B16 = 0x00;
											
											Get_Usercard_Serial();//�������к�,ͬʱ�ж��Ƿ񿪻�
											if( comm_open_flag == 0 )	//bit15: Զ�̿���(0����,1δ����)
											{
												Temp16.B08[0] |= B1000_0000;
											}

											if( card_open_flag == 0 )	//bit14: ���ؿ���(0����,1δ����)
											{
												Temp16.B08[0] |= B0100_0000;
											}
											
											if( Comm_Auth_Judge() )	//bit13:	�����֤״̬
											{
												Temp16.B08[0] |= B0010_0000;
											}
											
											if( relay_commu_close_flag || self_close_flag )	//bit12:	����״̬
											{
												Temp16.B08[0] |= B0001_0000;
											}
											
											if( BDMODE )
											{
												if( Tariff.CurLad_N )	
												{
													Temp16.B08[0] |= B0000_0100;	//bit10: ��ǰ���ݱ� (0��1�Ž��ݱ�,1��2�Ž��ݱ�)
												}
												
												Temp16.B08[0] |= B0000_0010;	//bit9~8�������Ԥ���ѱ�
											}

											if( remainder_alarm1_flag||remainder_alarm2_flag||remainder_zero_flag||relay_commu_alarm_flag)	//bit7��Ԥ��բ����״̬
											{
												Temp16.B08[1] |= B1000_0000;
											}

											if( relay_commu_open_flag )		//bit6���̵�������״̬
											{
												Temp16.B08[1] |= B0100_0000;
											}

											if( Tariff.Status&B0000_1000 )		//bit5����ǰ����ʱ��
											{
												Temp16.B08[1] |= B0010_0000;
											}

											if( comm_relaystatus_flag )		//bit4���̵���״̬
											{
												Temp16.B08[1] |= B0001_0000;
											}

											if( IR_EnTimer )			//bit3��������֤/�������
											{
												Temp16.B08[1] |= B0000_1000;
											}

											if( Tariff.Status&B0000_0100 )		//bit0����ǰ����ʱ��
											{
												Temp16.B08[1] |= B0000_0001;
											}

											if( DI07.B08[0] == 0xFF )
											{
												OutBuf[ k++ ] = Temp16.B08[1];
												OutBuf[ k++ ] = Temp16.B08[0];
											}
											else
											{
												k = 3;
												OutBuf[1] = Temp16.B08[1];
												OutBuf[2] = Temp16.B08[0];
												break;
											}

										case 0x04:	//�������״̬��4   XXXX	2	
											Temp16.B16 = 0x00;
											if( load_over_flag )	//bit4��A �����
											{
												Temp16.B08[1] |= B0001_0000;
											}
											
											if( DI07.B08[0] == 0xFF )
											{
												OutBuf[ k++ ] = Temp16.B08[1];
												OutBuf[ k++ ] = Temp16.B08[0];
											}
											else
											{
												k = 3;
												OutBuf[1] = Temp16.B08[1];
												OutBuf[2] = Temp16.B08[0];
												break;
											}			
											
										case 0x05:	//�������״̬��5   XXXX	2	
										case 0x06:	//�������״̬��6   XXXX	2	
											Temp16.B16 = 0x00;
											if( DI07.B08[0] == 0xFF )
											{
												for( i=0; i<2; i++ )
												{
													OutBuf[ k++ ] = 0x00;
													OutBuf[ k++ ] = 0x00;
												}
											}
											else
											{
												k = 3;
												OutBuf[1] = Temp16.B08[1];
												OutBuf[2] = Temp16.B08[0];
												break;
											}
											
										case 0x07:	//�������״̬��7   XXXX	2	���������״̬��
											//bit10:	�����
											Temp16.B16 = 0x00;
											if( Cover.Status )	//bit9:	�����
											{
												Temp16.B08[0] |= B0000_0010;
											}
											if( below_60u_flag )	//bit5:	����
											{
												Temp16.B08[1] |= B0010_0000;
											}
											
											if( DI07.B08[0] == 0xFF )
											{
												OutBuf[ k++ ] = Temp16.B08[1];
												OutBuf[ k++ ] = Temp16.B08[0];
											}
											else
											{
												k = 3;
												OutBuf[1] = Temp16.B08[1];
												OutBuf[2] = Temp16.B08[0];
												break;
											}
											
										case 0x08:	//��Կ״̬��	XXXXXXX		4
											if( DI07.B08[0] == 0xFF )
											{
												GetKey( OutBuf+k );
												k += 4;
											}
											else
											{
												GetKey( OutBuf+1 );
												k = 5;
											}
											break;
											
										default:	
											break;
									}
									break;
									
								case 0x06:	  
									switch( DI07.B08[0] )
									{
										case 0x01:  //�й���Ϸ�ʽ������  	NN	1	
											//�����������֣�5�����й���Ϸ�ʽ������1(1)����������ģʽ��(6)���������������1(1) +�ϱ�ģʽ��(6)+crc��2��
											k = 2;
											OutBuf[1] = Mode.Comp_B;
											break;
											
//										case 0x02:  //�޹���Ϸ�ʽ1 ������  NN	1	
//											break;
//											
//										case 0x03:	//�޹���Ϸ�ʽ2 ������  NN	1	
//											break;
//											
										default:	
											break;
									}
									break;
									
								case 0x07:	  
									switch( DI07.B08[0] )
									{
										case 0x01:	//�����ͺ����ڲ�����������	NN  1
//										case 0x02:	//�Ӵ�ʽ�����ڲ�����������    NN  1											
										case 0x03:	//ͨ�ſ�1 ������������          NN  1
//										case 0x04:	//ͨ�ſ�2 ������������          NN  1
										case 0x05:	//ͨ�ſ�3 ������������          NN  1
											//�����������֣�5�����й���Ϸ�ʽ������1(1)����������ģʽ��(6)���������������1(1) +�ϱ�ģʽ��(6)+crc��2��
											k = 2;
											if( DI07.B08[0] == 3 ) OutBuf[1] = Mode.Bps[0];
											else if( DI07.B08[0] == 5 ) OutBuf[1] = Mode.Zbps[0];
											else OutBuf[1] = DI07.B08[0]+0x03;
											break;

										default:
											break;
									}
									break;
									
								case 0x08:	
									switch( DI07.B08[0] )
									{
										case 0x01:	//������������          	NN	1
											k = 2;
											OutBuf[1] = Week_Holiday.Status;
											break;
											
										case 0x02:	//��������õ���ʱ�α��	NN	1
											k = 2;
											OutBuf[1] = Week_Holiday.Shiduan_N;
											break;

										default:
											break;
									}
									break;

								case 0x0A:
									switch( DI07.B08[0] )
									{
										case 0x01:	//���ɼ�¼��ʼʱ��		MMDDhhmm	4
											k = 5;
											Exchange( OutBuf+1, Week_Holiday.Lstarttimer+1, 4 );
											break;
												
										case 0x02:	//��1 �ฺ�ɼ�¼���ʱ��    NNNN	2
										case 0x03:	//��2 �ฺ�ɼ�¼���ʱ��    NNNN	2
										case 0x04:	//��3 �ฺ�ɼ�¼���ʱ��    NNNN	2
										case 0x05:	//��4 �ฺ�ɼ�¼���ʱ��    NNNN	2
											k = 3;
											Temp16.B16 = Week_Holiday.Lspacetimer[DI07.B08[0]-2];
											OutBuf[1] = Temp16.B08[1];
											OutBuf[2] = Temp16.B08[0];
											break;

										default:
											break;
									}
									break;
								case 0x09:	
									//�����������֣�5�����й���Ϸ�ʽ������1(1)����������ģʽ��(6)���������������1(1) +�ϱ�ģʽ��(6)+crc��2��
									switch( DI07.B08[0] )
									{
										case 0x01:	//���ɼ�¼ģʽ��		NN	1								
										case 0x02:	//��������ģʽ��		NN	1	��ʱ��������ģʽ��
										case 0x03:	//˲ʱ��������ģʽ��	NN	1
										case 0x04:	//Լ����������ģʽ��    NN	1
										case 0x05:	//���㶳������ģʽ��    NN	1
										case 0x06:	//�ն�������ģʽ��      NN	1
											k = 2;
											OutBuf[1] = Mode.Freeze[DI07.B08[0]-1];
											break;
											
										default:
											break;
									}
									break;
									
								case 0x0B:	
									switch( DI07.B08[0] )
									{
										case 0x01:	//ÿ�µ�1 ������	DDhh	2	��ʱ
										case 0x02:	//ÿ�µ�2 ������    DDhh	2	��ʱ
										case 0x03:	//ÿ�µ�3 ������    DDhh	2	��ʱ
											ReadE2WithBackup( ee_histroyparameter_page, ee_histroyparameter_inpage, Buf, ee_histroyparameter_lenth );

											k = 3;
											OutBuf[1] = Buf[2*(DI07.B08[0]-1)+0];
											OutBuf[2] = Buf[2*(DI07.B08[0]-1)+1];
											break;

										default:
											break;
									}
									break;
								
								case 0x0C:	//0����Կ~9����Կ��02�����룬04������
//									if( DI07.B08[0] && (DI07.B08[0]<=0x0A) )
//									{
										k = 2;
										OutBuf[0] = 0xFF; //���
										OutBuf[1] = 0x04; //Error: bit2�������/δ��Ȩ
//									}
									break;

								case 0x10:
									if(!BDMODE) break;
										
									switch( DI07.B08[0] )
									{
										case 0x01:	//�������1 ��ֵ	XXXXXX.XX	4	Ԫ
										case 0x02:  //�������2 ��ֵ	XXXXXX.XX	4	Ԫ
											k = 5;
											if( DI07.B08[0] == 0x01 )
											{
												Temp08 = 0;
											}
											else
											{
												Temp08 = 4;
											}

											Exchange( OutBuf+1, Esam_Para.Alarm_Limit1.B08+Temp08, 4 );
											break;
										
										case 0x03:	//͸֧�����ֵ    	NNNNNN.NN	4	Ԫ
										case 0x04:	//�ڻ������ֵ    	NNNNNN.NN	4	Ԫ
										case 0x05:	//��բ��������ֵ  NNNNNN.NN   4	Ԫ
											//͸֧�����ֵ(4)���ڻ������ֵ(4)����բ��������ֵ(4)��Crc(2)
											k = 5;
											Exchange( OutBuf+1, Money_Para.Tick.B08+4*(DI07.B08[0]-0x03), 4 );
											break;	
											
										default:
											break;
									}
									break;
								
								case 0x11:
									switch( DI07.B08[0] )
									{
										case 0x01:	//�������������1	NN	1
											//�����������֣�5�����й���Ϸ�ʽ������1(1)����������ģʽ��(6)���������������1(1) +�ϱ�ģʽ��(6)+crc��2��
											k = 2;
											OutBuf[1] = Mode.Feature1;
											break;
											
										case 0x04:	//�����ϱ�ģʽ��	NNNNNNNNNNNNNNNN	8
											//�����������֣�5�����й���Ϸ�ʽ������1(1)����������ģʽ��(5)���������������1(1)���ϱ�ģʽ��(6) + crc��2��
											k = 9;
											memcpy( OutBuf+1, Mode.Report, 8 );
											break;
											
										default:
											break;
									}
									break;
								
								case 0x12:
									switch( DI07.B08[0] )
									{
										case 0x01:	//���㶳����ʼʱ��	YYMMDDhhmm	5	������ʱ��
											Temp16.B08[0] = 5;	//����
											Temp16.B08[1] = 0;	//ƫ����
											break;
											
										case 0x02:  //���㶳��ʱ����	NN          1	����      
											Temp16.B08[0] = 1;	//����
											Temp16.B08[1] = 5;	//ƫ����
											break;
											
										case 0x03:  //�ն���ʱ��      	hhmm        2	ʱ��      
											Temp16.B08[0] = 2;	//����
											Temp16.B08[1] = 6;	//ƫ����
											break;
											
										case 0x04:	//��ʱ����ʱ��	4	MMDDhhmm
											Temp16.B08[0] = 4;	//����
											Temp16.B08[1] = 8;	//ƫ����
											break;
																						
										default:
											Temp16.B08[0] = 0;	//����
											break;
									}
									
									if( Temp16.B08[0] )
									{
										I2C_Read_Eeprom44( ee_freezeparameter_page, Buf, ee_freezeparameter_lenth );
										
										k = Temp16.B08[0]+1;
										Exchange( OutBuf+1, Buf+Temp16.B08[1], Temp16.B08[0] );
									}
									break;

//								case 0x13:	
//									switch( DI07.B08[0] )
//									{
//										case 0x01:	//����ͨ�����߼��ź�ǿ��ָʾ	NN	1	
//											break;
//											
//										default:
//											break;
//									}
//									break;
									
								case 0x14:
									switch( DI07.B08[0] )
									{
										case 0x01:	//��բ��ʱʱ�䣨NNNN Ϊ��բǰ�澯ʱ�䣩	NNNN	2
										case 0x02:	//�̵�����բ���Ƶ�������ֵ	XXX.XXX	3
										case 0x03:	//�̵�����ʱ��բʱ��	XXXX	2
										case 0x04:	//������֤ʱЧ	XXXX	1
										case 0x05:	//�ϱ���λʱ��	XXXX	1
											if( DI07.B08[0] == 0x02 )
											{
												Temp16.B08[0] = 0;	//ƫ�Ƶ�ַ0
												Temp16.B08[1] = 3;	//�ֽ���
											}
											else if( DI07.B08[0] >= 0x04 )
											{
												Temp16.B08[0] = DI07.B08[0]+0x03;//ƫ�Ƶ�ַ7,8
												Temp16.B08[1] = 1;
											}
											else
											{
												Temp16.B08[0] = DI07.B08[0]+2;//ƫ�Ƶ�ַ3,5	
												Temp16.B08[1] = 2;	
											}

											k = Temp16.B08[1]+1;
											Exchange( OutBuf+1, Mode.Limit_I+Temp16.B08[0], Temp16.B08[1] );	
											break;	
											
										case 0xee:	//�̵�����բ����ʽ	XX	1
											k = 2;
											OutBuf[1] = Mode.Wz_Clocerelay;
											break;
										
										case 0xf0:	//PLCEVTOUT��״̬  1	BCD	
											k = 2;
											OutBuf[1] = F_Plcevtout_Sta;
											break;		
											
										case 0xF1:	//ͣ���¼���ѹ������ֵ	1	BCD
										case 0xF2:	//ͣ���¼�����������ֵ	1	BCD
										case 0xF3:	//ͣ���¼��ж���ʱʱ�䶨ֵ	1	BCD
										case 0xF4:	//������¼��ж���ʱʱ�䶨ֵ	1	BCD
										case 0xF5:	//ʱ�ӵ��Ƿѹ�¼��������޶�ֵ	1	BCD
										case 0xF6:	//ʱ�ӵ��Ƿѹ�¼��ж���ʱʱ�䶨ֵ	1	BCD	
											k = 2;
											OutBuf[1] = Threshold[DI07.B08[0]-0xF1];
											break;	
											
										default:
											break;
									}
									break;
									
								case 0x15:
									switch( DI07.B08[0] )
									{
										case 0x01:	 //�����ϱ�״̬��	NNNNNNNNNNNNNNNNNNNNNNNN	12+N
											k = Ready_Reportdata( OutBuf );
											break;
											
										case 0x02:	 //�忨״̬��	NNNN	2	
											k = 3;
											OutBuf[1] = InsertCardSta;
											OutBuf[2] = 0x00;
											InsertCardSta = 0;	//��忨״̬��Ϊδ֪
											break;
											
										default:
											break;
									}
									break;

								default:
									break;
							}
							break;

						case 0x01:	//��һ��ʱ�������ʱ�α�����
						case 0x02:	//�ڶ���ʱ�������ʱ�α�����
							//��ʱ������1������ʱ�α�����1������ʱ���� ��1������������1����������������1��+г������������1��+��������1������Կ����1����crc��2��
							if( DI07.B08[2] == 0x01 )
							{
								Temp16.B08[0] = ee_shiqu1_page;
								Temp16.B08[1] = ee_shiduantable1_1_page;
							}
							else
							{
								Temp16.B08[0] = ee_shiqu2_page;
								Temp16.B08[1] = ee_shiduantable2_1_page;
							}

							switch( DI07.B08[1] )
							{
								case 0x00:
									switch( DI07.B08[0] )
									{
										case 0x00:	//��һ��ʱ��������, ��1~14ʱ����ʼ���ڼ���ʱ�α��(14*3), MMDDNN
											Temp32.B08[0] = Bcd_To_Hex(Feik.Num[0]);	//�趨��"��ʱ����"
											if( Temp32.B08[0] > MAXREGION )
											{
												Temp32.B08[0] = MAXREGION;
											}

											I2C_Read_Eeprom( Temp16.B08[0], 0x00, Buf, ee_shiqu_lenth );
											
											k = Temp32.B08[0]*3+1;
											memcpy( OutBuf+1, Buf, k-1 );
											break;
											
										default:
//											Temp32.B08[0] = Bcd_To_Hex(Feik.Num[1]);	//�趨��"��ʱ�α���"
//											if( Temp32.B08[0] > MAXSIDUANTB )
//											{
//												Temp32.B08[0] = MAXSIDUANTB;
//											}
				
											Temp32.B08[1] = Bcd_To_Hex(Feik.Num[2]);	//�趨��"��ʱ����"
											if( Temp32.B08[1] > MAXSIDUAN )
											{
												Temp32.B08[1] = MAXSIDUAN;
											}
				
											if( DI07.B08[0] <= MAXSIDUANTB )	//��ȡ����ʱ�α�Ų��ܳ����趨��"��ʱ�α���"
											{
												I2C_Read_Eeprom( Temp16.B08[1]+(DI07.B08[0]-1), 0x00, Buf, ee_shiduantable_lenth );
	
												k = Temp32.B08[1]*3+1;
												memcpy( OutBuf+1, Buf, k-1 );
											}
											break;
									}
									break;
									
								default:
									break;
							}
							break;

						case 0x07:	//��ǰ�׽���ʱ���������׽���ʱ����
							if( !BDMODE ) break; //Զ��ģʽ
								
							if( (DI07.B08[1]!=0x00) || (DI07.B08[0]>0x01) ) break; //����ʱ�����ܳ���2��

							I2C_Read_Eeprom( ee_jtshiqu1_page+DI07.B08[0], ee_jtshiqu_inpage, OutBuf+1, ee_jtshiqu_len );
							
							k = 12+1;
							break;
							
						case 0x03:	//��n �����������ڼ���ʱ�α��
//							ReadE2WithBackup( ee_feik_number_page, ee_feiknumber_inpage, Buf, ee_feiknumber_lenth );

							switch( DI07.B08[1] ) 
							{
								case 0x00:
//									Temp32.B08[0] = Bcd_To_Hex(Buf[4]);	//�趨��"��ʱ����"
//									if( Temp32.B08[0] > 14 )	//�������գ�1~14, YYMMDDNN, 4
//									{
//										Temp32.B08[0] = 14;
//									}
//		
//									if( (DI07.B08[0]) && (DI07.B08[0] <= Temp32.B08[0]) )	//��ȡ�Ĺ������ղ��ܳ����趨��"����������"
									if( (DI07.B08[0]) && (DI07.B08[0] <= MAXHOLIDAY) )		//��ȡ�Ĺ������ղ��ܳ����趨��"����������"
									{
										Temp16.B08[0] = (DI07.B08[0]-1)/15;
										Temp16.B08[1] = ((DI07.B08[0]-1)%15)*4;
										
										result = I2C_Read_Eeprom( ee_holiday_page+Temp16.B08[0], Temp16.B08[1], OutBuf+1, 4 );
										if( result != 0x00 ) break;
											
										k = 5;
									}
									break;
									
								default:
									break;
							}
							break;							
							
						case 0x04:
							switch( DI07.B08[1] )
							{
								case 0x01:	//�Զ�ѭ��
								case 0x02:	//����ѭ��
									//ѭ������(1)��ѭ��ʱ��(1)������С��λ��(1)������λ��(1)����������(1)��������(1)+ ��բ��ʱʱ��(2)��crc(2)
									if( (DI07.B08[0]) && (DI07.B08[0]<=SCREENNUM) )
									{
										n = 2*(DI07.B08[1]-1)+((DI07.B08[0]-1)/62);
										Temp08 = ((DI07.B08[0]-1)%62);
										
										result = Item[n].Display[Temp08];
										
										//------ ��� ���� ���� (��ǰ����1~��12������),��Ŵ�1��ʼ ------
										if( result && (result <= 195) )
										{
											k = 6;
											OutBuf[3] = (result-1)/65;	//���,����,����
											OutBuf[1] = (result-1-65*OutBuf[3])/5;	//0:��ǰ; 1~12����
											OutBuf[2] = (result-1)%5;
											OutBuf[4] = 0x00;
											OutBuf[5] = 0x00;
											
										}
										//------ ������ʾ��Ŀ����Ŵ�200��ʼ--------
										else if( result >= 200 )
										{
											result -= 200;
											
											Temp08 = disp_num;	//disp_table�����ܼ���ʾ��Ŀ��

											if( result < Temp08 )
											{
												Temp32.B32 = disp_table[result];
												for( n=0; n<=result; n++ )
												{
													if( Temp32.B32 != disp_table[result-n] ) break;
												}
												
												if( result == 0 )
													result = 0xff;		//ȫ����ʾ
												else
													result = n-1;
												Inverse( Temp32.B08, 4 );
											}
//											else if( result < Temp08+14 )		//ʱ��
//											{
//												Temp32.B32 = disp_table[Temp08-1];
//												result -= (Temp08-1);
//											}
											else
											{
												Temp32.B32 = 0;
												result = 0;
											}

											k = 6;
											memcpy( OutBuf+1, Temp32.B08, 4 );
											OutBuf[5] = result;
										}
									}
									break;
								
								default:
									break;
							}
							break;

						case 0x05:	//���ʵ��
						case 0x06:	//��ǰ�׵�1�Ž��ݱ�~�����׵�1�Ž��ݱ�
							if( !BDMODE ) break; //Զ��ģʽ
								
							if( DI07.B08[2] == 0x05 )
							{
								if( (DI07.B08[1]!=0x01) && (DI07.B08[1]!=0x02) ) break;

								if( DI07.B08[0] && (DI07.B08[0]<=ee_tariffprice_lenth/4) )
								{
									Temp32.B08[1] = 1;
									Temp32.B08[2] = DI07.B08[0]-1;
								}
								else if( DI07.B08[0] == 0xFF )
								{
									Temp32.B08[1] = ee_tariffprice_lenth/4;
									Temp32.B08[2] = 0;
								}
								else break;

								I2C_Read_Eeprom( ee_tariffprice1_page+DI07.B08[1]-1, 0, Buf, ee_tariffprice_lenth );
	
								k = 1;
								for( i=0; i<Temp32.B08[1]; i++ )
								{
									Exchange( OutBuf+k, Buf+4*(Temp32.B08[2]+i), 4 );
									k += 4;
								}
							}
							else //��һ�׵�1~n ����ֵ		NNNNNN.NN	4	kWh
							{
								if( ((DI07.B08[1] != 0x06)&&(DI07.B08[1] != 0x07)&&(DI07.B08[1] != 0x0a)&&(DI07.B08[1] != 0x0b)) || (DI07.B08[0]!=0xFF) ) break;

								k = 71;
								ReadE2WithJieti( DI07.B08[1]-0x06, OutBuf+1 );//����ǰ���߱����׽��ݵ���/���/�����ʱ��
								for( i=0; i<13; i++ ) Inverse( OutBuf+1+4*i, 4 );
//								for( i=0; i< 6; i++ ) Inverse( OutBuf+1+52+3*i, 3 );
								break;
							}
							break;

						case 0x09:
							switch( DI07.B08[1] )
							{
								case 0x08:
									switch( DI07.B08[0] )
									{
										case 0x01:	//�����¼�������������	NNN.N	2	A
											k = 3;
											OutBuf[1] = Week_Holiday.Max_I.B08[1];
											OutBuf[2] = Week_Holiday.Max_I.B08[0];						
											break;
											
										case 0x02:	//�����¼��ж���ʱʱ��	NN	1	S
											k = 2;
											OutBuf[1] = Week_Holiday.Max_I_Period;
											break;
											
										default:
											break;
									}
									break;
									
								default:
									break;
							}
							break;		

						case 0x80:
							switch( DI07.B08[1] )
							{
								case 0x00:
									switch( DI07.B08[0] )
									{
										case 0x01:	//��������汾��(ASCII ��)	NN��NN	32
										case 0x02:	//����Ӳ���汾��(ASCII ��)	NN��NN	32
										case 0x03:	//���ұ��(ASCII ��)		NN��NN	32
										case 0x04:	//���������			NN��NN	8	
											Temp08 = 32;//32�ֽ�
											if(DI07.B08[0] == 0x01)
											{
												Temp16.B08[0] = ee_softver_page;	//���������
												Temp16.B08[1] = ee_softver_inpage;
											}
											else if(DI07.B08[0] == 0x02)
											{
												Temp16.B08[0] = ee_hardver_page;	//Ӳ��������
												Temp16.B08[1] = ee_hardver_inpage;
											}
											else if(DI07.B08[0] == 0x03)
											{
												Temp16.B08[0] = ee_factorynum_page;	//���ұ��
												Temp16.B08[1] = ee_factorynum_inpage;
											}
											else
											{
												Temp16.B08[0] = ee_softbk_page; 	//���������
												Temp16.B08[1] = ee_softbk_inpage;
												Temp08 = 8;//8�ֽ�
											}
											result = I2C_Read_Eeprom( Temp16.B08[0], Temp16.B08[1], OutBuf+1, Temp08 );
																						
											for( i=0; i<Temp08; i++ )	//���ȫ��Ϊ0xff�����Ĭ�ϵ�ֵ
											{
												if( OutBuf[1+i] != 0xff ) break;
											}
											
											if( result || (i==Temp08) )
											{
												if( DI07.B08[0] == 0x04 ) Exchange( OutBuf+1, (uint08*)Softbk, 8 );	
												else  memset( OutBuf+1, 0x00, Temp08 );	//����Ĭ�Ͽ�
											}		  
											k = Temp08+1;
											break; 
												
										default:
											break;
									}
									break;
								
								default:
									break;
							}
							break;
						
						default: break;
					}
					break;
				
				case 0x05:	//��������
//					if( DI07.B08[2] > 7 ) break;
						
					ReadFreezedRec( DI07.B08[2], DI07.B08[1], DI07.B08[0], OutBuf );
					if( OutBuf[0] == 0x00 ) break;

					k = OutBuf[0]+1;
					OutBuf[0] = 0x00;	//���
					break;
			
				default: break;
			}

			if( k == 0x00 )
			{
				
				OutBuf[ k++ ] = 0xFF;
				OutBuf[ k++ ] = 0x02;	//������Ϣ��ERR������������
			}
			break;
				
		case 0x12:	//��ȡ����֡
			if( Uart[Ch].Flag&B0000_0001 ) break; //ͨѶ��ַȫ99   ��֧��
			if( Uart[Ch].Buf[9] != 0x05 ) break;	//Len error
			
			switch( DI07.B08[3] )
			{
				case 0x03:
					switch( DI07.B08[2] )
					{
						case 0x30:
							switch( DI07.B08[1] )
							{
								case 0x02:
								case 0x05:	//��ʱ�α� ��̼�¼									
//								case 0x08:	//�ڼ��ձ��, �������ձ��
								case 0x10:	//���ݱ��̼�¼
									if( (DI07.B08[0]) && (DI07.B08[0] <= 0x0A) && Uart[Ch].Buf[14] )
									{
										RdHisEvent( DI07.B08, Uart[Ch].Buf[14], OutBuf );
										if( OutBuf[0] == 0x00 ) break;

										if( (DI07.B08[1] == 0x05) && (Uart[Ch].Buf[14] != 3) )	Remain = 1;
//										if( (DI07.B08[1] == 0x08) && (Uart[Ch].Buf[14] != 5) )	Remain = 1;
										if( (DI07.B08[1] == 0x10) && (Uart[Ch].Buf[14] != 2) )	Remain = 1;

										k = OutBuf[0]+1;
										OutBuf[0] = 0x00;
									}
									break;
									
								default:
									break;
							}
							break;
							
						default:
							break;
					}
					break;
					
				case 0x06:	//���ɼ�¼
					if( (DI07.B08[2] == 0x10) && (DI07.B08[1]) && (DI07.B08[1] <= 0x06) )
					{
						k = ReadLoadRec( Ch, Uart[Ch].Buf+8, OutBuf );
						if( k == 0 ) RdLoadPara[Ch][4] = 0;
						else if(RdLoadPara[Ch][4]) Remain = 1;
					}
					break;
					
				default:
					break;
			}
			
			if( (k ==0) && (Mode.Feature1&0x04) && ChkBufZero( Report_Data, 5 ) )
			{
				DI07.B32 = 0x01150004;
				
				//�����ϱ�״̬��	NNNNNNNNNNNNNNNNNNNNNNNN	12+N
				k = Ready_Reportdata( OutBuf );
				OutBuf[k++] = Uart[Ch].Buf[14];//����֡���
				memcpy( Uart[Ch].Buf+10, DI07.B08, 4 );//�����µı�ʶ
			}
			
			if( k == 0x00 )
			{
				OutBuf[ k++ ] = 0xFF;
				OutBuf[ k++ ] = 0x02;	//������Ϣ��ERR������������
			}
			break;

		case 0x13:	//��ͨ�ŵ�ַ, ��ַ�������ȫAAH
			//����������̼�¼����
			memset( Buf, 0xaa, 6 );
			if( (Cmp_Data( Buf, Uart[Ch].Buf+1, 6 ) !=3)	//��ȫaa	
				|| (Uart[Ch].Buf[9] != 0x00) )
			{
				Uart[Ch].Overtime_Timer = 2;
				return;
			}

			k = 7;
			Exchange( OutBuf+1, Comm_Addr.Comm_Id, 6 );
			break;

		case 0x15:	//дͨ�ŵ�ַ, ��ַ�������ȫAAH
			//����������̼�¼����
			if( ((Uart[Ch].Flag&B0000_0010) != B0000_0010)	//��ַ�������ȫAAH
				|| (!progenable) 							//��̰�����Ч,24hour��̽�ֹ��Ч
				|| (Uart[Ch].Buf[9] != 0x06) )				//���ݳ��Ȳ��Ϸ�
			{
				Uart[Ch].Overtime_Timer = 2;
				return;
			}
			
			Exchange( Buf, Uart[Ch].Buf+10, 6 );

			result = WriteE2WithBackup( ee_commaddr_page, ee_commaddr_inpage, Buf, ee_commaddr_lenth );	//д��E2PROM
			if( result )
			{
				Uart[Ch].Overtime_Timer = 2;
				return;
			}

			memcpy( Comm_Addr.Comm_Id, Buf, 8 );

			k = 1;
			break;

		case 0x08:	//�㲥Уʱ
			//����������̼�¼����
			if( ((Uart[Ch].Flag&B0000_0010) == B0000_0010) || (Uart[Ch].Buf[9] != 0x06) ) //��֧����λ
			{
				Uart[Ch].Overtime_Timer = 2;
				return;
			}

			result = JudgeClockNoWeek( 0, &Uart[Ch].Buf[10], 6 );
			if( result != 0 ) //
			{
				break; //����
			}
			
			if( already_setclock_flag == 1 )
			{
				k = 2; 
				OutBuf[0] = 0xFF;
				OutBuf[1] = 0x04;	//Error: bit2�������/δ��Ȩ
				break; 			//������Уʱ, ��������Уʱ
			}

			if( !clock_error )	//ϵͳʱ����ȷ
			{
				//----- ��������00:00:00�ġ�5min�ڹ㲥Уʱ------
				Buf[0] = 0x00;
				Buf[1] = 0x05;
				Buf[2] = 0x00;
				Temp08 = Cmp_Bcd_Inv( Buf, Sclock.Clockbackup, 3 );
				if( Temp08 == 1 )
				{
					k = 2; 
					OutBuf[0] = 0xFF;
					OutBuf[1] = 0x04;	//Error: bit2�������/δ��Ȩ
					break;	//��ǰʱ��С��00:05:00
				}
	
				Buf[0] = 0x00;
				Buf[1] = 0x55;
				Buf[2] = 0x23;
				Temp08 = Cmp_Bcd_Inv( Buf, Sclock.Clockbackup, 3 );
				if( Temp08 == 2 )
				{
					k = 2; 
					OutBuf[0] = 0xFF;
					OutBuf[1] = 0x04;	//Error: bit2�������/δ��Ȩ
					break;	//��ǰʱ�����23:55:00
				}
				
				//----- ������㲥Уʱ����5min����------
				result = Judge_Min_Space( Uart[Ch].Buf+10 );
				if( result  == 1 )
				{
					k = 2; 
					OutBuf[0] = 0xFF;
					OutBuf[1] = 0x04;	//Error: bit2�������/δ��Ȩ
					break;	//������㲥Уʱ����5min
				}
				
				//------ �������ڽ����յġ�5min�ڹ㲥Уʱ -------
				ReadE2WithBackup( ee_histroyparameter_page, ee_histroyparameter_inpage, Buf, ee_histroyparameter_lenth );	//������ʱ��: ��ʱ
				for( i=0; i<9; i++ )
				{
					Buf[6] = 0x00; //����ʱ��
					Buf[7] = 0x00;
					if( i < 3 )
					{
						memcpy( Buf+8, Buf+2*i, 2 ); //Hour+Day
						Buf[10] = Sclock.Clockbackup[4];
					}
					else
					{
						if( !BDMODE ) continue; //Զ��ģʽ
						memcpy( Buf+8, Price.Year_Ladder+3*(i-3), 3 );//Hour+Day+Month
					}
					Buf[11] = Sclock.Clockbackup[5];
					
					result = Judge_Min_Space( Buf+6 );
					if( result == 0 ) break;	//�������ڽ����յġ�5min�ڹ㲥Уʱ
				}

				if( result == 0 )
				{
					k = 2; 
					OutBuf[0] = 0xFF;
					OutBuf[1] = 0x04;	//Error: bit2�������/δ��Ȩ
					break;
				}
			}

			memcpy( Buf+0, Uart[Ch].Buf+10, 3 );

			if( clock_error ) { Buf[3] = 0x00; }
			else { Buf[3] = Sclock.Clockbackup[6]; }

			memcpy( Buf+4, Uart[Ch].Buf+13, 3 );

			result = Set_Rtc( 0, Buf, 7 );
			if( result != 0x00 ) break; //����ʱ��ʧ��

			already_setclock_flag = 1; //

			CommSetRtc_Adj();	//����ʱ�ӵ����������
			
			I2C_Read_Eeprom44( ee_wdt_page, Eeprom.Buffer,ee_wdt_lenth );//�㲥Уʱ������1
			Eeprom.Buffer[1]++;
			if( Eeprom.Buffer[1] == 0 ) Eeprom.Buffer[0]++;
			I2C_Write_Eeprom44( ee_wdt_page, Eeprom.Buffer, ee_wdt_lenth );
			
			Init_Demand();	//��ǰ��������
		
			k = 1;
			break;

		case 0x16:	//��������
			//68+���+68+16+04+mmhhDDMM+CS+16
			if( Uart[Ch].Buf[9] != 0x04 )	//�����򳤶ȴ���
			{
				Uart[Ch].Overtime_Timer = 2;
				return;
			}

			Temp08 = 0x00; //����ʱ����99H�ĸ���
			for( n=0; n<4; n++ )
			{
				if( Uart[Ch].Buf[10+n] == 0x99 )
				{
					Temp08++;
				}
				else
				{
					if( Temp08 || JudgeClockNoWeek(1+n, &Uart[Ch].Buf[10+n], 1) ) //ǰһ�ֽ�Ϊ99H���ֽ�����, �򶳽�ʱ�����
					{
						break;
					}
				}
			}
			
			if( n < 4 )
			{
				break;
			}

			switch( Temp08 )
			{
				case 0x00:	//mmhhDDMM, ��ͨ����
				case 0x01:	//99DDhhmm��ʾ����Ϊ���ڶ�ʱ����
					if( Uart[Ch].Buf[12] > 0x28 )  break;		//ֻ����1~28�ն���
				case 0x02:	//9999hhmm��ʾ����Ϊ���ڶ�ʱ����
				case 0x03:	//999999mm��ʾ��СʱΪ���ڶ�ʱ����
					//���㶳����ʼʱ��(5)�����㶳��ʱ����(1)���ն���ʱ��(2)+ ��ʱ����ʱ��(4)+ crc��2��
					I2C_Read_Eeprom44( ee_freezeparameter_page, Buf, ee_freezeparameter_lenth );

					Exchange( Buf+8, Uart[Ch].Buf+10, 4 );

					result = I2C_Write_Eeprom44( ee_freezeparameter_page, Buf, ee_freezeparameter_lenth );
					if( result != 0x00 ) break;

					k = 1;
					break;
					
				case 0x04:	//99999999Ϊ˲ʱ����
					//˲ʱ����ʱ��(5)+˲ʱ���������й���������(5*4)+˲ʱ���ᷴ���й���������(5*4)+˲ʱ�����������(3)
					Freeze_Dl( ee_realtimefreeze1_page, ee_immedf_inpage );

					k = 1;
					break;
					
				default:
					break;
			}
			break;
			
		case 0x17:	//����ͨ������, ������
			//����������̼�¼����

			//Bit7 Bit6     Bit5    Bit4    Bit3    Bit2    Bit1   Bit0
			//���� 19200bps 9600bps 4800bps 2400bps 1200bps 600bps ����
			
			if( Uart[Ch].Flag&B0000_0011 )
			{
				k = 2; 
				OutBuf[0] = 0xFF;
				OutBuf[1] = 0x04;	//Error: bit2�������/δ��Ȩ
				break;
			}
			
			if( Uart[Ch].Buf[9] != 0x01 ) //�����򳤶Ȳ�Ϊ1, ����
			{
				Uart[Ch].Overtime_Timer = 2;
				return;
			}
			
			//0x01:�����ͺ����ڲ�����������//0x03:ͨ�ſ�1 ������������//0x05:ͨ�ſ�3 ������������			
			Temp08 = Set_Comm_Rate( Ch*2+1, Uart[Ch].Buf[10] );
			if( Temp08 )
			{
				k = 2;
				OutBuf[0] = 0xFF;
				OutBuf[1] = 0x08;	//������Ϣ��ERR��ͨ�����ʲ��ܸ���(Bit3)
			}
			else
			{
				k = 2;
				OutBuf[1] = Uart[Ch].Buf[10];	//���ĺ�����
			}
			break;

			
		case 0x19:	//�����������
			//������������̼����ʹ��
			if( Uart[Ch].Buf[9] != 0x08 ) //�����򳤶ȴ���
			{
				Uart[Ch].Overtime_Timer = 2;
				return;
			}

			if( ((progenable)&&(Uart[Ch].Buf[10] != 0x03))	//����04��Ȩ��
			|| ((!progenable)&&(Veri_Psw( Uart[Ch].Buf+10 ) != 0x00) ))	//�������
			{
				k = 2;
				OutBuf[0] = 0xFF; //���
				OutBuf[1] = 0x04; //Error: bit2�������/δ��Ȩ
				break;
			}

			//1. �������� ��¼
			//�����ܴ���(3)+����ʱ��(6)+�����ߴ���(4)+����ʱ�����й�����������ͷ���ʱ��(8)+����ʱ�����й�����������ͷ���ʱ��(8)
//			Temp08 = Read_Event_Point( ee_cleardemand_inpage );
//			I2C_Read_Eeprom( ee_clear_demand_page+Temp08, ee_clear_demand_inpage, Buf, ee_clear_demand_lenth );

			//�洢��������ʱ���򣬵��ֽ���ǰ�����ֽ��ں�
			Number_Add1( ee_cleardemand_inpage);		//�ܴ���+1(3)
			memcpy( Buf, Sclock.Clockbackup, 6 );	//����ʱ��(6)
			memcpy( Buf+6, Uart[Ch].Buf+14, 4 );	//�����ߴ���(4)
			I2C_Read_Eeprom( ee_p_demand_page, ee_p_demand_inpage, Buf+10, ee_pdemand_lenth );	//��ǰ�����й�����������ͷ���ʱ��(8)
			I2C_Read_Eeprom( ee_n_demand_page, ee_n_demand_inpage, Buf+18, ee_ndemand_lenth );	//��ǰ�����й�����������ͷ���ʱ��(8)
			
			Temp08 = Write_Event_Point( ee_cleardemand_inpage );
			I2C_Write_Eeprom( ee_clear_demand_page+Temp08, ee_clear_demand_inpage, Buf, ee_clear_demand_lenth );

			//2. ��ǰ�������������ʱ����������
			Init_Demand();
			memset( Buf, 0x00, 64 );
			I2C_Write_Eeprom( ee_p_demand_page, ee_p_demand_inpage, Buf, ee_pdemand_lenth );
			I2C_Write_Eeprom( ee_n_demand_page, ee_n_demand_inpage, Buf, ee_ndemand_lenth );
			
			k = 1;
			OutBuf[0] = 0x00;
			break;
		
			
		case 0x1A:	//�������, ��յ��ܱ��ڵ����������������¼���¼��������, �����������¼����ʱ�̺�����ǰ�ĵ�����
//			if( BDMODE ) //����ģʽ, ����ģʽ
//			{
//				k = 2;
//				OutBuf[0] = 0xFF;	//Operation result.
//				OutBuf[1] = 0x04; //Error: bit2�������/δ��Ȩ
//				break;
//			}
		case 0x1B:	//�¼�����, ��յ��ܱ��ڴ洢��ȫ���¼���¼����
			//������������̼����ʹ�á�����ִ��ʱ���ܱ�Ӧ��֤��������¼���¼������������Զ�������¼�����Ӧ����
			//����������̼�¼����
			if( !(Uart[Ch].Flag & B0000_0011) && progenable && (Uart[Ch].Buf[10] == 0x03) )//����ģʽ����ֱ������
			{
				Ic_Comm_RxBuf[0] = Uart[Ch].Buf[8];
				Exchange( Ic_Comm_RxBuf+8, Uart[Ch].Buf+18, 4 );	//�¼������ʶ
			}
			else
			{
				if( (Uart[Ch].Flag & B0000_0011) || (Uart[Ch].Buf[10] != KEYTYPE2) || (identity_prog_flag == 0) || hangup_comm_flag )	//��ȫ��֤��̱�־ ʧЧ
				{
					k = 2;
					OutBuf[0] = 0xFF; //���
					OutBuf[1] = 0x04; //Error: bit2�������/δ��Ȩ
					break;	//�������ܱ�ֻ֧��98��Զ�̿���
				}
				
				if( Uart[Ch].Buf[8] == 0x1A ) //�������
					result = Decrypt( Ch, 0x93, 0x08 );
				else
					result = Decrypt( Ch, 0x93, 0x0c );
				if( result )
				{
					k = 0x00; 
					OutBuf[ k++ ] = 0xFF;
					OutBuf[ k++ ] = 0x04;
					break;
				}
				
				if( Uart[Ch].Buf[8] != Ic_Comm_RxBuf[0] ) break;
			}

			k = 2;
			OutBuf[0] = 0xFF; //���
			OutBuf[1] = 0x04; //Error: bit2�������/δ��Ȩ
			
			//��֤ͨ���������Ķ����ָ��ִ����Ӧ����
			if( Ic_Comm_RxBuf[0] == 0x1A )
			{
				MeterClear = 0x55aa;	//�������
			}
			if( Ic_Comm_RxBuf[0] == 0x1B )
			{
				Inverse( Ic_Comm_RxBuf+8, 4 );	//�¼������ʶ����
				memcpy( DI07.B08, Ic_Comm_RxBuf+8, 4 );	//�¼������ʶ

				if( DI07.B08[0] != 0xFF ) break;
				
				if( DI07.B32 == 0xFFFFFFFF ) ClrRecTimes( 0, 0x55 );//�嶳����¼���Чָ��(���������)
				else if( DI07.B32 == 0xFF143003 )//��Ƿ��忨����ҳ
				{
					Clr_Otherdata(0);//��Ƿ��忨����
				}
				else
				{
					if( (DI07.B08[3] == 0x19) ||(DI07.B08[3] == 0x1D)||(DI07.B08[3] == 0x1E)||(DI07.B08[3] == 0x22)||
					   ((DI07.B08[3] == 0x03)&&((DI07.B08[2] == 0x11)||(DI07.B08[2] == 0x33))))	//����բ���⴦��
					{
						if( DI07.B08[3] == 0x22 )
						{
							if( DI07.B08[1] != 0x00 ) break;
						}
						else
						{
							if( DI07.B08[1] != 0xFF ) break;
						}
						
						i = Find_EventTable(DI07.B08+2);		//�ж��Ƿ��ҵ��Ϸ����ݱ�ʶ
					}
					else if( DI07.B08[3] == 0x03 )
					{
						i = Find_EventTable(DI07.B08+1);		//�ж��Ƿ��ҵ��Ϸ����ݱ�ʶ
					}
					else break;

					if( i != 24 )
					{				
						i += ee_clearn_inpage;
						if( i < ee_powerdownn_inpage ) break; 
						
						ClrRecTimes( i, 0x01 );
					}
					else
					{
						break;
					}
				}
					
				//�¼������¼
				//�洢��������ʱ���򣬵��ֽ���ǰ�����ֽ��ں�
				memcpy( Eeprom.Buffer+6, Uart[Ch].Buf+14, 4 );	//�����ߴ���(4)
				memcpy( Eeprom.Buffer+10, Ic_Comm_RxBuf+8, 4 ); 	//�¼��������ݱ�ʶ��(4)
				Event_Record( ee_cleareventn_inpage );
			}
			k = 1;
			OutBuf[0] = 0x00;
			break;

		case 0x1C:	//����բ������������	
			if( Uart[Ch].Buf[10] != KEYTYPE2 )	//�ж��Ƿ�Ϊ96������
			{
				if( (Uart[Ch].Flag & B0000_0011) || (Uart[Ch].Buf[9] != 16) || (Uart[Ch].Buf[10] != 0x03) ||
				    (!progenable && ( Veri_Psw( Uart[Ch].Buf+10 ))) )//����ģʽ����ֱ������բ�������ĺ�բ����/ֱ�Ӻ�բ
				{
					k = 2;
					OutBuf[0] = 0xFF; //���
					OutBuf[1] = 0x04; //Error: bit2�������/δ��Ȩ
					break;
				}
				else
				{
					memcpy( Ic_Comm_RxBuf,  Uart[Ch].Buf+18, 8 );//����2�ֽ����ݺ�6�ֽ���Ч��ֹʱ��
					result = JudgeEndClock();
					if(result)
					{
						k = 2;
						OutBuf[0] = 0xFF; //���
						OutBuf[1] = 0x04; //Error: bit2�������/δ��Ȩ
						break;
					}
					Temp08 = Ch;
				}
			}
			else
			{
				if( (Uart[Ch].Flag & B0000_0011) || ((Ch == 0x01) && (progenable==0)&&(IR_EnTimer == 0)) || (identity_prog_flag == 0) || hangup_comm_flag )	//��ȫ��֤��̱�־ ʧЧ
				{
					k = 2;
					OutBuf[0] = 0xFF; //���
					OutBuf[1] = 0x04; //Error: bit2�������/δ��Ȩ
					break;	//�������ܱ�ֻ֧��98��Զ�̿���
				}
                        	
				//P0��P2��Ч, ��Ч��֤
				//��N1~Nmд��ESAM�Ŀ��������ļ����н���, ���ܳɹ����Խ��ܺ��Ӧ�����������������ִ����Ӧ�Ŀ��Ʋ�����
				//Զ�̿������80fb060010+���ģ�
				//��ȡԶ�̿�����������ģ�00C00000��LC��Ҫ��ȡ���������ݳ��ȣ�Ϊ1 �ֽڵ�ʮ������������
				//68+���+68+1C+1C+98000000+00000000+����+cs+16
				//������д��ESAM 08�ļ�����: 04D6880014+����
				//N1Ϊ�����������ͣ�1AH������բ��1BH�����բ����2AH��������2BH�����������3AH�����磬3BH����������
				//N2������
				//N3��N8����������Ч��ֹʱ�䣬���ݸ�ʽΪssmmhhDDMMYY�������ǰʱ���Ѿ��������ʱ��ʱ�����������Ϊ��Ч������ִ��
				
				Temp08 = Ch+0x20;
			}
			
			//��֤ͨ���������Ķ����ָ��ִ����Ӧ����
			result = Relay_Ctrl(Temp08);
			if(result)
			{
				k = 2;
				OutBuf[0] = 0xFF; //���
				OutBuf[1] = 0x04; //Error: bit2�������/δ��Ȩ
			}
			else k = 1;
			
			break;
			
		case 0x1D:	//�๦�ܶ��������������, �����̼���Ϻ�������֤, ���ܱ���ʹ�ù����У������������ϵ����ʱ�����ܶ��ӵ�ǰ��������źž��ָ������ź������
			if( Uart[Ch].Buf[9] != 0x01 )	//�����򳤶ȴ���
			{
				Uart[Ch].Overtime_Timer = 2;
				return;
			}
			
			switch( Uart[Ch].Buf[10] )
			{
				case 0x00:	//ʱ��������
				case 0x01:	//��������
				case 0x02:	//ʱ��Ͷ��
					Outputtype = Uart[Ch].Buf[10];
					reg_veri_flag = 1;
					k = 2;
					OutBuf[1] = Uart[Ch].Buf[10];
					break;
					
				default:
					break;
			}
			break;
			
		case 0x09:	//Ѱ������
			if( (identity_prog_flag == 0) || (Keyswitch_Valid == 0xaa) )	//��ȫ��֤��̱�־ ʧЧ
			{
				k = 2;
				OutBuf[0] = 0xFF; //���
				OutBuf[1] = 0x04; //Error: bit2�������/δ��Ȩ
				break;	//�������ܱ�ֻ֧��98��Զ�̿���
			}
			
			if( Ram_Ptr[ 9 ] != 12 ) break;

			result = JudgeClockNoWeek( 0, Uart[Ch].Buf+16, 6 );
			if( result != 0x00 ) break; //����
	
			result = Cmp_Bcd_Inv( Uart[Ch].Buf+16, Sclock.Clockbackup, 6 );
			if( result == 0x02 ) break; //����
			
			if(Uart[Ch].Buf[ 14 ] == 0x1a)
			{
				Find_Card = 0x55;	//����Ѱ��ģʽ
//				rf_card_flag = 1;
				CardCheck1 = 0;	//����Ѱ��һ��
			}
			else if(Uart[Ch].Buf[ 14 ] == 0x1b)
			{
				Find_Card = 0xaa;	//����Ѱ��ģʽ
//				rf_card_flag = 0;
			}
			else break;
			
			k = 1;
			break;
			
		//--------------------- ��չ������ -------------------------------
		case 0x1E://��չ����			
			if( (!BDMODE) && (SELFCLOSE == 0x55) )	//�Ա��繦��		
			{
				if((Uart[Ch].Buf[ 9 ] == 1) && ((Uart[Ch].Flag & B0000_0011) != 0))	//����֡,�㲥
				{
					Uart[Ch].Flag |= B0000_0001; 	//�㲥�����������
					
					Self_Timer3 = 0;			//����֮֡����ʱ����
					if(Self_Cnt < 3) Self_Cnt++;
					
					if(self_close_flag && (Self_Timer24 == 0) && (Self_Cnt >= 3))//�Ա���ʱ�����������յ�3������֡
					{
						ClrSelfClose();
					}
					
					Temp08 = Uart[Ch].Buf[10];
					if((Temp08 < 0x06) || (Temp08 > 0x90) || IsBCD(Temp08)) break;	//����ʱ����6~90����
					
					ReadE2WithBackup( ee_hearttime_page, ee_hearttime_inpage, Buf, ee_hearttime_lenth );
					
					Temp08 = Bcd_To_Hex( Temp08 );
					if(Temp08 != Buf[0])
					{
						WriteE2WithBackup( ee_hearttime_page, ee_hearttime_inpage, &Temp08, ee_hearttime_lenth );
					}
					break;
				}
			}
			
			if( Uart[Ch].Buf[ 10 ] == 5 )//��չ��������ee
			{
				Temp16.B08[1] = Uart[Ch].Buf[ 11 ];		//Di1
				Temp16.B08[0] = Uart[Ch].Buf[ 12 ];		//Di2		

				Temp08 = 68;
				if( (Temp16.B16>=0x2000) || (Uart[Ch].Buf[ 13 ] == 0x81) )
				{					
					if(  Temp16.B16==0xeeee )
					{
						memset(Uart[Ch].Buf+14, 0x00, 14);
						if( BDMODE && (Keyswitch_Valid!=0xAA) )  memcpy( Uart[Ch].Buf+14, "BD-CPU", 6 );
						else if( !BDMODE && (Keyswitch_Valid!=0xAA) ) memcpy( Uart[Ch].Buf+14, "YC-CPU", 6 );
						else if( BDMODE && (Keyswitch_Valid==0xAA) )  memcpy( Uart[Ch].Buf+14, "BDNCPU", 6 );
						else if( !BDMODE && (Keyswitch_Valid==0xAA) ) memcpy( Uart[Ch].Buf+14, "YCNCPU", 6 );					
						if( Relay_Mode != 0x55 ) memcpy( Uart[Ch].Buf+20, "-NZ-", 4);	
						else memcpy( Uart[Ch].Buf+20, "-WZ-", 4);		
						memcpy( Uart[Ch].Buf+14+14, Verdate, 18 );//���ذ汾��Ϣ
						Uart[Ch].Buf[14+32] = 0xaa;
						Uart[Ch].Buf[14+33] = clkmode;		//1��ƵΪ8M��ʱ��,2��ƵΪ4M��ʱ��
						Uart[Ch].Buf[14+34] = sleeptmr0ie;	//0x20��tmr0ͣ�绽��,0x00�ر�tmr0ͣ�绽�ѽ�֧��1h����
						Uart[Ch].Buf[14+35] = Correction_Mode;	//0x55-������ѹ��������,����-�ر�(Ŀǰ��rn8209c֧��)
						Uart[Ch].Buf[14+36] = SELFCLOSE;	//�Ա��繦�ܣ�00�رգ�55����
						Uart[Ch].Buf[14+37] = 0xaa;
					}
					else
					{
						Temp32.B08[3] = Uart[Ch].Buf[ 11 ];		//Di1
						Temp32.B08[2] = Uart[Ch].Buf[ 12 ];		//Di2
						Temp32.B08[1] = Uart[Ch].Buf[ 13 ];
						Temp32.B08[0] = 0;
	
						if( Uart[Ch].Buf[ 13 ] == 0x01 )//��ʾ��չ���Ĵ���(0x012000~0x0123ff)
						{
							memcpy( Uart[Ch].Buf+14, (unsigned char *)Temp32.B32, 64 );
						}
						else if( Uart[Ch].Buf[ 13 ] == 0x81 )//��ʾ��չ��flash
						{
							memcpy( Uart[Ch].Buf+14, (unsigned char ecode *)Temp32.B32, 64 );
						}
						else	//��ʾ��չ��ram(0x000000~0x000fff)
						{
							memcpy( Uart[Ch].Buf+14, (unsigned char near *)(Temp16.B16-0x2000), 64 );
						}
					}
				}
				else
				{
					I2C_Read_Eeprom( Temp16.B16, 0, Uart[Ch].Buf+14, Uart[Ch].Buf[13]);
//					Inverse(Uart[Ch].Buf+14, Uart[Ch].Buf[13]);
					if( Temp16.B16 >= 0x300 ) Temp08 = 76;
				}
				
				Uart[Ch].Buf[ 9 ] = Temp08;
				Uart[Ch].Buf[ Temp08+11 ] = 0x16;
							
				Uart[Ch].Flag |= B0000_1000;//��Լ�������׼���������ݱ�־
				return;
			}
			break;
			
		case 0x1F://��չ����	
//			if((Uart[Ch].Buf[ 10 ]!=0xAF) ) break;	//�������֧���ز�ͨ��
				
			switch( Uart[Ch].Buf[ 10 ] )
			{
				case 0x13:	//��ʼ ((Cover.Status == 0)&&((Fwdl.Fwdl_T.B32>=0x10000)||(Bkdl.Bkdl_T.B32>=0x10000)))�ָ�?
					if(  (!progenable)  )
					{
						k = 2;
						OutBuf[0] = 0xFF; //���
						OutBuf[1] = 0x04; //Error: bit4�������/δ��Ȩ
						break;
					}
					
					if( (Uart[Ch].Buf[11] == 0x14)&&(Uart[Ch].Buf[12] == 0x54)&&(Uart[Ch].Buf[13] == 0x80) )
					{
						MeterClear = 0x88BB;
						
//						Clear_Meter( Ch, Uart[Ch].Buf+10 );
//						Factory_Init();
						
						k = 1;
					}
					break;
					
				case 0x0f:	//�����̿��ؿ���ָ��
					if( Uart[Ch].Buf[9] != 0x03 ) break;//���ݳ��ȴ���
					
					if( Uart[Ch].Buf[11] == 0xaa )
					{
						Buf[0] = 0x00;	//�����̽�ֹ
						if( I2C_Write_Eeprom44( ee_factorymode_page, Buf, ee_factorymode_lenth ) ) break;
						progenable = 0;	//��̽�ֹ
					}
					else if(  Uart[Ch].Buf[11] == 0x55 )
					{
					//	if( (Key_Status==1) && (esam_error==0) ) break;	//����esam�������˽Կ����������
					//	else
						{
							Buf[0] = 0x55;	//������ʹ��
							if( I2C_Write_Eeprom44( ee_factorymode_page, Buf, ee_factorymode_lenth ) ) break;
							progenable = 1;	//���ʹ��
						}
					}
					else
					{
						break;		//�Ƿ�����
					}
					
					k = 1;
					break;	
					
				case 0x03:
					//�����¶����ߵĲ���˵����
					//0 �����¶�б��΢�� //1 �����¶�б��΢��
					//2 ���¾�������΢���������޵����κ���ϵ����x*0.0001
					//3 ���¾�������΢���������޵����κ���ϵ����x*0.0001
					//4 ���¶����Уֵ
					if( Ch == 0 ) break;//��ֹ�������
					if( Uart[Ch].Buf[9] != 0x0a ) break;//���ݳ��ȴ���
					if( Wr_Ext_Number == 50 ) break;
					else if ( Wr_Ext_Number > 50 ) Wr_Ext_Number = 25;
					else Wr_Ext_Number++;
					
					Temperpara_Verify();	//�¶�ϵ��У��
					memcpy( Buf, Tpara.Offset, ee_temperature_lenth );//�����¶�ϵ��
					
					if( Uart[Ch].Buf[19] == 0x00 )//ȫ������
					{
						memcpy( Buf+2, Uart[Ch].Buf+16, 3 );
					}
					else if( Uart[Ch].Buf[19] == 0x55 )//���¸����¶�ϵ��
					{
						Buf[2]=Uart[Ch].Buf[16];
					}
					else if( Uart[Ch].Buf[19] == 0x5a )//���µ����¶�ϵ��
					{
						Buf[3]=Uart[Ch].Buf[17];
					}
					else if( Uart[Ch].Buf[19] == 0xaa )//���µ�Уֵ
					{
						Buf[4]=Uart[Ch].Buf[18];
					}
					else	break;
					if( (Buf[4]&0x7f) > 3 ) break;	//��Уֵ������ΧΪ0~3
					result = WriteE2WithBackup( ee_temperature_page, ee_temperature_inpage, Buf, ee_temperature_lenth );
					if( result ) break;
					memcpy( Tpara.Offset, Buf, ee_temperature_lenth );//����ram�¶�ϵ��
					Tpara.Crc16.B16 = Crc_16Bits(Tpara.Offset, ee_temperature_lenth ); //CRC����
					
					Adcctrl.Timer=8;//���������¶�adc
					k = 1;
					break;
					
				case 0x04://�����帺�ص��ݵ�����
					k = 7;
					OutBuf[1] = const_tx_flag;	//0xc3��ʾ�¶ȵ�У��
					OutBuf[2] = Tpara.RtcTMode;	//��RTC��������ģʽ��0x55:����ģʽ��0xAA���²���ֵģʽ����������ͨģʽ
//					OutBuf[3] = XLCAPCTRLL;		//�����帺�ص���
//					OutBuf[4] = XLCAPCTRLH;
					OutBuf[5] = Temperature_Adc.B08[1];	//���¶�ADC����
					OutBuf[6] = Temperature_Adc.B08[0];
					break;

				case 0x05://д���帺�ص���	NNNN	2
				case 0x07://����RTC��������ģʽ��0x55:����ģʽ��0xAA���²���ֵģʽ����������ͨģʽ
				case 0x0a://д���¶����Уֵ��ͨ�ŵ�ʱ����
				case 0xa9://�ر��¶Ȳ�����ͨ�ŵ�ʱ����׼������ʱ����
				case 0xaa://д���¶����Уֵ��ͨ�ŵ�ʱ����(�·�2�ֽ��ռ�ʱ���,-15��~15��,����)
				case 0x0b://�����¶�ADC΢��
					//5 ���¶����Уֵ��ͨ�ŵ�ʱ����
					//6 �����¶�ADC΢��
					//7 Ԥ��
					//8 RTC��������ģʽ��0x55:����ģʽ��0xAA���²���ֵģʽ����������ͨģʽ
					//9~10	����ƥ�����(��ͨģʽ��Ч)
					if( (Ch == 0)||(!progenable)  ) break;//��ֹ�������,��̽�ֹ

					Temperpara_Verify();	//�¶�ϵ��У��
					memcpy( Buf, Tpara.Offset, ee_temperature_lenth );//�����¶�ϵ��
					if( Uart[Ch].Buf[ 10 ] == 0x05 )
					{
						if( (Uart[Ch].Buf[9] != 0x03) || (Tpara.RtcTMode == 0x55) || (Tpara.RtcTMode == 0xaa) ) break;	
						Buf[9] = Uart[Ch].Buf[12];
						Buf[10] = Uart[Ch].Buf[11];
					}
					else if( Uart[Ch].Buf[ 10 ] == 0x07 )
					{
						if( (Uart[Ch].Buf[9] != 0x02)) break;//���ݳ��ȴ���,��̽�ֹ
						Buf[8] = Uart[Ch].Buf[11];
					}
					else if( Uart[Ch].Buf[ 10 ] == 0x0a )
					{
						if( (Uart[Ch].Buf[9] != 0x02)||(const_tx_flag == 0xc3) ) break;	//�ձ�̵�У��
						Buf[5] = Uart[Ch].Buf[11];	
					}
					else if( Uart[Ch].Buf[ 10 ] == 0xa9 )//�ر��¶Ȳ�����ͨ�ŵ�ʱ����׼������ʱ����
					{
						if( (Uart[Ch].Buf[9] != 0x01)||(const_tx_flag == 0xc3) ) break;	//�ձ�̵�У��
						RtcStopProcFlag = 0x55;//�ر�RTC�¶Ȳ�������
						Adcctrl.Timer=8;//���������¶�adc
						k = 1;
						break;
					}
					else if( Uart[Ch].Buf[ 10 ] == 0xaa )//(�·�2�ֽ��ռ�ʱ���,-15��~15��,����)
					{
						if( (Uart[Ch].Buf[9] != 0x03)||(const_tx_flag == 0xc3)||(RtcStopProcFlag!=0x55)) break;	//�ձ�̵�У��
						
						Temp16.B08[0] = Uart[Ch].Buf[ 12 ];
						Temp16.B08[1] = Uart[Ch].Buf[ 11 ];
						if( ((short)Temp16.B16 > 15000) || ((short)Temp16.B16 < -15000) ) break;	//����Ƿ�	
						Buf[5] = Get_RTCTop_Proc( Temp16.B16 );	//���㶥���Уֵ
						RtcStopProcFlag = 0x00;//��RTC�¶Ȳ�������
					}
					else
					{
						if( (Uart[Ch].Buf[9] != 0x02)||((Uart[Ch].Buf[11]&0x7F)>50) ) break;//�������������Χ
						Buf[6] = Uart[Ch].Buf[11];	
					}
					result = WriteE2WithBackup( ee_temperature_page, ee_temperature_inpage, Buf, ee_temperature_lenth );
					if( result ) break;						
					
					if( (Uart[Ch].Buf[ 10 ]&0x0f) == 0x0a )//д���¶����Уֵʱʱ���¼
					{
						I2C_Read_Eeprom( ee_rtcadj_time_page, ee_rtcadjtime_inpage, Buf, ee_rtcadjtime_lenth );
						Buf[0]++;//д���¶����Уֵ������1
						memcpy( Buf+1, Sclock.Clockbackup, 7 );//д���¶����Уֵʱʱ��
						memcpy( Buf+8, Fun_Para.Temper_M.B08, 2 );//д���¶����Уֵʱ�¶�
						Buf[7] = Uart[Ch].Buf[ 10 ];//ͨѶ��ʶ
						I2C_Write_Eeprom( ee_rtcadj_time_page, ee_rtcadjtime_inpage, Buf, ee_rtcadjtime_lenth );		
					}
					
					Tpara.Crc16.B16 += 1;
					temperpara_veri_flag = 1;
					Adcctrl.Timer=8;//���������¶�adc
					k = 1;
					break;
					
				case 0x06://�޸ĸ��ص��ݣ�19.968��ƫ��������4.096pf��	NNNN	2	
					if( Ch == 0 ) break;//��ֹ�������
 					if( (Uart[Ch].Buf[9] != 0x03)||(!progenable) ) break;//���ݳ��ȴ���,��̽�ֹ
 					if( Tpara.RtcTMode!=0x55 ) break;//���ڲ���ģʽ����Ч
					
					Temp16.B08[0] = Uart[Ch].Buf[12];
					Temp16.B08[1] = Uart[Ch].Buf[11];
					if( (((int16)Temp16.B16)<-1024)||(((int16)Temp16.B16))>1024 ) break;//��19.968��ƫ��������4.096pf=1024*0.004��
					if( const_tx_flag != 0xc3 )//ͨ��У׼ʱ�����
					{
						TempCap16.B08[0] = constF_XcapH;
						TempCap16.B08[1] = constF_XcapL;
					}
					else
					{
						TempCap16.B08[0] = const_XcapH;
						TempCap16.B08[1] = const_XcapL;
					}

					TempCap16.B16 = (int16)TempCap16.B16 +	(int16)Temp16.B16;
//					XLCAPCTRLH  = TempCap16.B08[0];
//					XLCAPCTRLL  = TempCap16.B08[1];
//					CAPREFRESH  = 0x55; //���µ���ֵ		
					Adcctrl.Timer=8;//���������¶�adc
					k = 1;
					break;
					
				case 0x08://���ò���ֵ����
					if( Ch == 0 ) break;//��ֹ�������
 					if( (Uart[Ch].Buf[9] != 85)||(!progenable) ) break;//���ݳ��ȴ���,��̽�ֹ
					result = I2C_Write_Eeprom( ee_ptatvalue_page, ee_ptatvalue_inpage, Uart[Ch].Buf+11, ee_ptatvalue_lenth );
					result |= I2C_Write_Eeprom( ee_capvalue_page, ee_capvalue_inpage, Uart[Ch].Buf+11+42, ee_capvalue_lenth );
					if( result != 0x00 ) break;
					I2C_Read_Eeprom( ee_ptatvalue_page, ee_ptatvalue_inpage, RtcTadcBuf.PTAT_Value[0].B08, ee_ptatvalue_lenth );
					I2C_Read_Eeprom( ee_capvalue_page, ee_capvalue_inpage, RtcTcapBuf.CAP_Value[0].B08, ee_capvalue_lenth );
					k = 1;
					break;
					
				case 0x09://�����¶����Уֵ��ͨ�ŵ�ʱ����
					k = 4;
					OutBuf[1] = const_tx_flag;	//0xc3��ʾ�¶ȵ�У��
					OutBuf[2] = Tpara.Offset[5];	//���¶����Уֵ
					OutBuf[3] = Tpara.Offset[6];	//�¶�΢��ֵ
					break;

				case 0x15://�����ۼӺ�	NNNNNNNN	4
					k = 5;
					Calculate_Program_Cs( OutBuf+1 );
					Inverse( OutBuf+1, 4 );
					break;	
					
				case 0x16://�ز���λģʽ��(��չ)	NN	1
					k = 2;
					OutBuf[1] = Mode.Plc;
					break;
				
				case 0x17://�ز���λģʽ��(��չ)	NN	1
					if( Uart[Ch].Buf[9] != 0x02 ) break;//���ݳ��ȴ���
					if( Wr_Ext_Number == 50 ) break;
					else if ( Wr_Ext_Number > 50 ) Wr_Ext_Number = 25;
					else Wr_Ext_Number++;
					
					if( Uart[Ch].Buf[11] != Mode.Plc )//�����ı��д����ee
					{
						result = UpdateSysPara( 0x80+27, Uart[Ch].Buf+11, 1 );
						if( result != 0x00 ) break;
					}

					k = 1;
					break;	
					
				case 0xDF:	//�̵���״̬��������
					if( (Uart[Ch].Buf[9] != 0x01) || (!progenable) ) break;//���ݳ��ȴ���,��̽�ֹ

					//���̵�������·��ʵ��ͨ�����
					for( Temp16.B16=0; Temp16.B16<3600/clkmode; Temp16.B16++ )	//30msȥ���ж�
					{
						if( RELAYTEST_N ) break;
					}
					
					if( Temp16.B16 == 3600/clkmode ) OutBuf[1] = 0xaa; //��
					else OutBuf[1] = 0x55;	//ͨ
					
					k = 2;
					break;			
				
				case 0xAF:	//�����ز��������ַ�������¼���Ϣ
					if( (Uart[Ch].Flag&B0000_0001) && (Uart[Ch].Buf[9] == 0x02) && (Uart[Ch].Buf[11] == 0x02) )	//�㲥, Len=2
					{
						Uart[Ch].Flag &= B1111_1110;
						
						k = 9;
						OutBuf[1] = 0xAF;
						OutBuf[2] = 0x02;
						Exchange( OutBuf+3, Comm_Addr.Comm_Id, 6 );
					}
					break;
				
				default:
					break;
			}
			break;			
		
		case 0x60:	//������оƬ�Ĵ���	
			if(Uart[Ch].Buf[10] < 0xf0)//��ȡ����оƬ�Ĵ���ֵ
			{
				memset(&Uart[Ch].Buf[11], 0x00, 4);
				result = Comm_Read_Reg(&Uart[Ch].Buf[10]);
				if(result) break;
			}
			else if(Uart[Ch].Buf[10] < 0xf5)//��ȡU��I1��I2��P��Fϵ��
			{
				result = Uart[Ch].Buf[10] - 0xf0;
				if( result == 2 ) result = 3;				//Ϊ�˼���֮ǰ��У�����
				else if(result == 3 ) result = 2;
				Exchange( Uart[Ch].Buf+11, Emu_Para1.rate[result].B08, 4 );
			}
			else break;	//�Ƿ�����
			
			k = 6;
			memcpy(&OutBuf[1], &Uart[Ch].Buf[10], 5 );
			break;
			
		case 0x61:	//д����оƬ�Ĵ���			
			if(!progenable)
			{
				k = 2;
				OutBuf[0] = 0xFF; //���
				OutBuf[1] = 0x04; //Error: bit4�������/δ��Ȩ
				break;
			}
			
			if(Uart[Ch].Buf[10] < 0xf0)//д�Ĵ���
			{
				result = Comm_Write_Reg(&Uart[Ch].Buf[10]);
				if(result) break;
			}
			else if(Uart[Ch].Buf[10] < 0xf5)//дU��I1��I2��P��Fϵ��
			{
				result = Uart[Ch].Buf[10] - 0xf0;
				if( result == 2 ) result = 3;
				else if(result == 3 ) result = 2;
//				memcpy( Emu_Para.rate[result].B08, Uart[Ch].Buf+11, 4 );
//				WriteE2WithBackup( ee_emupara_page, ee_emupara_inpage, Emu_Para.rate[0].B08, ee_emupara_lenth );
				memset( Emu_Para1.rate[result].B08, 0, 4 );
				
				if(Uart[Ch].Buf[11] != 0 )
				  Exchange( Emu_Para1.rate[result].B08, Uart[Ch].Buf+11, 4 );
				else if (Uart[Ch].Buf[12] != 0  )
				  Exchange( Emu_Para1.rate[result].B08, Uart[Ch].Buf+12, 3 );
				else if (Uart[Ch].Buf[13] != 0  )
				  Exchange( Emu_Para1.rate[result].B08, Uart[Ch].Buf+13, 2 );
				else
					Exchange( Emu_Para1.rate[result].B08, Uart[Ch].Buf+14, 1 );
				
				WriteE2WithBackup( ee_7051_para_page, ee_7051_para_inpage, Emu_Para1.rate[0].B08, ee_7051_para_lenth );
			       
				Emu_Para1.Crc16.B16+=1;
			}
			else if(Uart[Ch].Buf[10] == 0xff)//У���ʼ������
			{
				for(i=0; i<4; i++)
				{
					if(Uart[Ch].Buf[11+i] != 0xff) break;
				}
				if(i != 4) break;
				
				Comm_Write_Reg(&Uart[Ch].Buf[10]);
			}
			else break;	//�Ƿ����� 
			
			check_7051_flag1=1;
			k = 1;
			break;

		default:
			k = 2;
			OutBuf[0] = 0xFF;	//Operation result.
			if( Uart[Ch].Buf[8]&0x40 )	//����ͨ�ſ�
				OutBuf[1] = 0x04; 	//Error: bit4�������/δ��Ȩ
			else
				OutBuf[1] = 0x02;	//���ݱ�ʶ��, ����������
			break;
	}

	if( Uart[Ch].Flag&B0000_0001 )	//��ַΪ�㲥��ַ, ��Ӧ��
	{
		Uart[Ch].Overtime_Timer = 2;
		return;
	}
	
	if( (DI07.B08[3]==0x07) && (DI07.B08[2]==0xA0) )//�����ն�����
	{
//		if( DIVSEL != rundivsel )	DIVSEL	= rundivsel;		//ϵͳƵ�ʻָ���4M
	}

	Temp08 = 0;
	if( k == 0x00 ) //������Ӧ��֡
	{
		k=2;
		OutBuf[1]=1;	   //������Ϣ��ERR����������(Bit0)
		Uart[Ch].Buf[8]|=0x40;		//����֡
	}
	else
	{
		if(OutBuf[0] == 0x00)	 //ȷ��֡
		{ 	   
		   if((Uart[Ch].Buf[8]==0x03)||(Uart[Ch].Buf[8]==0x11)||(Uart[Ch].Buf[8]==0x12))
		   	Temp08 += 4;
		   	   
		   if((Remain)||
		      (((Uart[Ch].Buf[8] == 0x11) || (Uart[Ch].Buf[8] == 0x12)) && (DI07.B32 != 0x01150004) && (Mode.Feature1&0x04)
  			   && ChkBufZero( Report_Data, 5 ))
		     )
		   	Uart[Ch].Buf[8]|=0x20;		 //����֡
        	
		}
		else
		{
//		  	if( Uart[Ch].Buf[8] == 0x03 )	//��ȫ��֤
//		  	{
//				if(hangup_comm_flag)	OutBuf[2] |= 0x01;	//Զ�̰�ȫ��֤��ס״̬λ
//		  	}
		  	Uart[Ch].Buf[8]|=0x40;		//����֡
		}
	}
	
	Uart[Ch].Buf[8]|=0x80;		   //Ӧ��֡
	Uart[Ch].Buf[9]=(k-1+Temp08); //len
	memcpy( Uart[Ch].Buf+10+Temp08, OutBuf+1, k );	//��������

//	Uart[Ch].Buf[7] = 0x68;
//	Uart[Ch].Buf[0] = 0x68;
	Exchange( Uart[Ch].Buf+1, Comm_Addr.Comm_Id, 6 );
	for( i=0; i<Uart[Ch].Buf[9]; i++ )
	{
		Uart[Ch].Buf[10+i] += 0x33;
	}	
	Uart[Ch].Buf[i+10]=Cal_ChkSum(Uart[Ch].Buf,Uart[Ch].Buf[9]+10);
	Uart[Ch].Buf[i+11]=0x16;

	Uart[Ch].Flag |= B0000_1000;//��Լ�������׼���������ݱ�־
	
	if( MeterClear == 0x55aa )	//����
	{
		Clear_Meter( Ch, OptID.B08 );	//0x1a�����������Ǯ����ʼ������
	}
//	else if( MeterClear == 0xaaaa )	//����
//	{
//		Clear_Meter( Ch, OptID.B08 );	//0x1a�����������Ǯ����ʼ������
//	}
	else if( MeterClear == 0x88bb )	//����ʼ��
	{
	  Clear_Meter( Ch, OptID.B08 );
		Factory_Init();
	}
	else if( MeterClear == 0xaa55 )	//��Կ����
	{
		UpdateEsamKey();
	}
	else if( MeterClear == 0x1122 )	//ģʽ�л�
	{
		Freeze_Dl(ee_modeswith_note_page, ee_modeswith_inpage);//Լ�����᣺ģʽ�л���¼
		
		UpdataRunFile();	//����ESAM������Ϣ�ļ�
		
		ProgramRecord( DI07.B08, OptID.B08 );
	}
}
