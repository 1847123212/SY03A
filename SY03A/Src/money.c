#include "ex_func.h"
#include "ex_var.h"
//#include <intrins.h>
#include <string.h>
#include <absacc.h>


//Style=0x55ͨѶ�˷�,��������Ǯ���ۼ�
void Remainmoney_Process(unsigned char *BcdA, unsigned char Style )	//���ۼ�����
{		
	unsigned char Buf[24];
	
	Remainmoney_Verify();	//ʣ�������У��
	Decimal.Status |= B0000_0010;//���仯��־
	memcpy( Buf, Money.Remain, 12 );//����ʣ����
	
	memset( Buf+12, 0x00, 6 );
	if( Style == 0x55 )	//ͨѶ�˷�
	{
		memcpy( Buf+12, BcdA, 4 );//�����˷ѽ��
		Decimal.Status |= B0000_0001;//ʣ����д����ee
		UpdateMoneytimer = 0;	//Ǯ���ۼ�����
	}
	else
	{
		memcpy( Buf+12, BcdA, 4 );//���Ƶ���
	}
	
	if ( Cmp_Bcd_Inv( Buf, Buf+12, 6) !=2 )
	{
		if ( Cmp_Bcd_Inv( Buf, Buf+12, 6) ==3 ) Decimal.Status |= B0000_0001;//��ֹʣ����Ϊ����ַǷ�У��
		Sub_Bcd( Buf, Buf+12, 6);//ʣ�������ȥ��λ���
	}
	else
	{
		Sub_Bcd( Buf+12, Buf, 6);//��λ��ۼ�ȥʣ�����
		memset( Buf, 0x00, 6 );//ʣ���������
		Add_Bcd( Buf+6, Buf+12, 6);//�õ�����ʣ�����
	}
	memcpy( Money.Remain, Buf, 12 );//����ʣ����
	Money.Crc16.B16 = Crc_16Bits( Money.Remain, ee_remainmoney_lenth );	 //CRC����
	
	if(Decimal.Status & B0000_0001)
	{
		WriteE2WithBackup( ee_remainmoney_page, ee_remainmoney_inpage, Money.Remain, ee_remainmoney_lenth );//ʣ����д����ee	

		if( UpdateMoneytimer == 0 )
		{
			Moneybag_Process1();//Ǯ��ͬ�����´���
			UpdateMoneytimer = default_updatemoneytimer;	//������������ESAMǮ����ʱ��	
		}
	
		Decimal.Status &= (~B0000_0001);//��������ȱ�־
	}
	
	run_alarm_flag = 1;	//�õ���״̬�жϱ�־
}


void MonthDaymoney_Process(unsigned char *BcdA )	//���ۼ���
{
  unsigned char Buf[6];
	
	memset( Buf, 0x00, 6 );
	memcpy( Buf, BcdA, 4 );
	Add_Bcd( Money_UserINFO.Day_Moneyused, Buf, 6);
	Money_UserINFO.DayCrc16.B16=Crc_16Bits( Money_UserINFO.Day_Moneyused,6 );

	Add_Bcd( Money_UserINFO.Month_Moneyused, Buf, 6);  
	Money_UserINFO.MonthCrc16.B16=Crc_16Bits( Money_UserINFO.Month_Moneyused,6 );
	
}

void Calc_Pwr_Proc(void)	//����������ۼƴ���ͽ���
{
	unsigned char Buf[24];
	union B16_B08 Temp16;
	union B32_B08 TempB;
	uint16 Page;	
	unsigned char Inpage;
	unsigned char *Dl_Buf;
	unsigned char n;
	
	if( FLAG_EA == 0 ) return;//���粻����,����ֱ�ӷ���

	if ( Decimal.Unit_Pluse > 0x00 )
	{
//  	NVIC_DisableIRQ(PMU_IRQn );
		n = Decimal.Unit_Pluse;
		Decimal.Unit_Pluse = 0;
//    NVIC_EnableIRQ(PMU_IRQn );
		if( Fwdl_Verify() ) return;				//�������У��
		Dl_Buf = Fwdl.Fwdl_T.B08;
		Page = ee_fwdl_page;
		Inpage = ee_fwdl_inpage;
		Decimal.Status |= B0100_0000;		//����������ı��־
	}
	else if ( Decimal.Unit_Bkpluse> 0x00 )
	{
//    NVIC_DisableIRQ(PMU_IRQn );
		n = Decimal.Unit_Bkpluse;
		Decimal.Unit_Bkpluse = 0;
//    NVIC_EnableIRQ(PMU_IRQn );
		if( Bkdl_Verify() ) return;				//�������У��
		Dl_Buf = Bkdl.Bkdl_T.B08;
		Page = ee_bkdl_page;
		Inpage = ee_bkdl_inpage;
		Decimal.Status |= B1000_0000;		//�÷�������ı��־
	}
	else
	{
		return;//�쳣
	}
	
	if( n > maxtimes ) return; //���������쳣
	
	for( ; n>0; n-- ) 
	{
		TempB.B32 = 1;	//��������
		memcpy( Buf, Dl_Buf, ee_fwdl_lenth );
		Add_Bcd( Buf, TempB.B08, 4 );//��������
		Add_Bcd( Buf+((Tariff.Current&3)+1)*4, TempB.B08, 4 );//���ʵ�������
		Add_Bcd( Buf+20, TempB.B08, 4 );//�����õ�������
		memcpy( Dl_Buf, Buf, ee_fwdl_lenth );
		Temp16.B16 = Crc_16Bits( Dl_Buf, ee_fwdl_lenth ); //CRC����
		Dl_Buf[ee_fwdl_lenth] = Temp16.B08[0];
		Dl_Buf[ee_fwdl_lenth+1] = Temp16.B08[1];
		
		if( Dl_Buf[3] == 0x00 )//�ж��ܵ����Ƿ�����
		{
			WriteE2WithBackup( Page, Inpage, Dl_Buf, ee_fwdl_lenth );
			Decimal.Status |= B0000_0001;//�õ������ȱ�־
			if( Inpage == ee_fwdl_inpage ) Decimal.Status &= ~(B0100_0000);//����������ı��־
			else Decimal.Status &= ~(B1000_0000);//�巴������ı��־
		}
		
		if( BDMODE ) 	//����
		{
			Price_Judge();	//���ʽ��ݵ���ж�
			Remainmoney_Process(Price.Current_X.B08, 0);  //��ǰ���
			run_price_flag = 1;	//�÷��ʽ����жϱ�־
			MonthDaymoney_Process(Price.Current_X.B08);  //��ǰ���
		}
	}
}


//offset=0��������,offset=3���������
//offset=1�½���,offset=4���½���
//offset=2�����,offset=5�������,offset=6�����ʱ������
void Process_Usedl( unsigned char offset )
{
	unsigned char i, len;
	unsigned char  Buf[ee_yhistroyusedl_lenth+8];
	
	memcpy( Buf, Sclock.Clockbackup+2, 4 );//����ʱ��
	Cal_Comb( 0, 0, 0, Buf+4 );	//���㵱ǰ��ϵ���	
//	Inverse( Buf+4, 4 );
	
	if( offset<3 ) 
	{				
		I2C_Read_Eeprom( ee_pre1_histroyusedl_page+offset, 0x00, Buf+8, ee_yhistroyusedl_lenth );//����1~6�ܵ���������1�ܵ���
	}
	else if( offset<6 )
	{
		offset -= 3;
		memset( Buf, 0x00, ee_yhistroyusedl_lenth );		//����1~7�ܵ���������1~2�ܵ���
	}
	else
	{
		offset = 2;
		memset( Buf, 0x00, 4 );			//ʱ������
	
		for( i=1; i<7; i++ )
		{
			memcpy( Buf+8*i, Buf, 8 );	//��1~6����ת��
		}
	}
	
	if( offset<2 )	len= ee_histroyusedl_lenth;
	else	len= ee_yhistroyusedl_lenth;

	I2C_Write_Eeprom( ee_pre1_histroyusedl_page+offset, 0x00, Buf, len );//��1~4�ܵ���ת��	
}


//type=0�����,��ת��,���õ���
//type=1�����,��ת��,�����õ���
//type=2�½���,��ת��,���õ���
void Freeze_Usedl( unsigned char type )//���㴦��
{
	Clear_Wdt();
	
	if( type == 0 )
	{
		Process_Usedl( 2 );	//��1��������ܵ���ת��
	}
	else
	{
		Process_Usedl( 1 );	//��1�½������ܵ���ת��
	}
	
	if( type == 1 )	return;//�����,��ת��,�����õ���
	
	if( Fwdl_Verify() ) return;	//�������У��
	memset( Fwdl.Usedl.B08, 0x00, 4 );//�����õ�������
	Fwdl.Crc16.B16 = Crc_16Bits( Fwdl.Fwdl_T.B08, ee_fwdl_lenth ); //CRC����
	WriteE2WithBackup( ee_fwdl_page, ee_fwdl_inpage, Fwdl.Fwdl_T.B08, ee_fwdl_lenth );
	
	if( Bkdl_Verify() ) return;	//�������У��
	memset( Bkdl.Usedl.B08, 0x00, 4 );//�����õ�������
	Bkdl.Crc16.B16 = Crc_16Bits( Bkdl.Bkdl_T.B08, ee_bkdl_lenth ); //CRC����
	WriteE2WithBackup( ee_bkdl_page, ee_bkdl_inpage, Bkdl.Bkdl_T.B08, ee_bkdl_lenth );
}


void FreezeMonth_Money(void)
{
	  I2C_Read_Eeprom( ee_Pre12monthMon_page, ee_Pre12monthMon_inpage, &Ic_Comm_TxBuf[6], ee_Pre12monthMon_lenth );
    
	  //�������1������
	  memcpy(Ic_Comm_TxBuf,Money_UserINFO.Month_Moneyused, 6 );
	  memset(Money_UserINFO.Month_Moneyused,0, 6 ); 
    Money_UserINFO.MonthCrc16.B16=Crc_16Bits( Money_UserINFO.Day_Moneyused, 6 );	
    I2C_Write_Eeprom( ee_Pre12monthMon_page, ee_Pre12monthMon_inpage, &Ic_Comm_TxBuf[0], ee_Pre12monthMon_lenth );
	
}


//�����洢
//mode =0;�ϵ�ģʽ��mode =1;�ϵ粹���һ���������ģʽ 
void FreezeMonth_Demand(unsigned  char mode)
{
	unsigned  char Result;
	
	Result = Write_Event_Point(ee_histroyDM_inpage);
	
	memset( Ic_Comm_TxBuf, 0x00, 42 );//���ݳ�ʼ��0xaa
	if( mode == 0)	I2C_Read_Eeprom( ee_p_demand_page, ee_p_demand_inpage, &Ic_Comm_TxBuf[0] , ee_pdemand_lenth );
	
	I2C_Write_Eeprom( ee_histroyp_demand_page+Result, ee_histroyp_demand_inpage, &Ic_Comm_TxBuf[0], ee_pdemand_lenth );//������������
	//������������
	memset( Ic_Comm_TxBuf, 0x00, 42 );//���ݳ�ʼ��0xaa
	if( mode == 0) I2C_Read_Eeprom( ee_n_demand_page, ee_n_demand_inpage, &Ic_Comm_TxBuf[0], ee_ndemand_lenth );
	I2C_Write_Eeprom( ee_histroyp_demand_page+Result, ee_histroypn_demand_inpage,&Ic_Comm_TxBuf[0], ee_ndemand_lenth );//������������
	
	Init_Demand();
	memset( Ic_Comm_TxBuf, 0x00, 42);
	I2C_Write_Eeprom( ee_p_demand_page, ee_p_demand_inpage, Ic_Comm_TxBuf, ee_pdemand_lenth );//��ǰ���������������
	I2C_Write_Eeprom( ee_n_demand_page, ee_n_demand_inpage, Ic_Comm_TxBuf, ee_ndemand_lenth );//��ǰ���������������
}

//�ն�����
void FreezeDay_Money(void)
{
	  I2C_Read_Eeprom( ee_Pre7DayMon_page, ee_Pre7DayMon_inpage, &Ic_Comm_TxBuf[6], ee_Pre7DayMon_lenth );
    
	  //�������1������
	  memcpy(Ic_Comm_TxBuf,Money_UserINFO.Day_Moneyused, 6 );
	  memset(Money_UserINFO.Day_Moneyused,0, 6 ) ;
    Money_UserINFO.DayCrc16.B16=Crc_16Bits( Money_UserINFO.Day_Moneyused, 6 );	
    I2C_Write_Eeprom( ee_Pre7DayMon_page, ee_Pre7DayMon_inpage, &Ic_Comm_TxBuf[0], ee_Pre7DayMon_lenth );
	
}


//����������ֽ���ǰ��ee,������������Ե��ֽ���ǰ������ee
void Freeze_Dl(unsigned short page, unsigned char offset)
{
	unsigned char Result, i;

	Clear_Wdt();

	memcpy( Eeprom.Buffer, Sclock.Clockbackup+1, 5 );
		
	if( page==ee_histroy_dl_page ) 		
	{
		memcpy( Eeprom.Buffer+5,  Fwdl.Fwdl_T.B08, 20 );
		memcpy( Eeprom.Buffer+25, Bkdl.Bkdl_T.B08, 20 );
		Eeprom.Buffer[47]=Mode.Comp_B;//�����й������
		Process_Usedl(0);//��1�������ܵ���ת��
	}
	else
	{
		for(i=0; i<5; i++)
		{
			memcpy( Eeprom.Buffer+5+4*i, Fwdl.Fwdl_T.B08+4*i, 4 );
		}
		
		for(i=0; i<5; i++)
		{
			memcpy( Eeprom.Buffer+25+4*i, Bkdl.Bkdl_T.B08+4*i, 4 );
		}
		
		memcpy( Eeprom.Buffer+45,  Emu_Data.P1.B08, 3 );
		if( dl_back_flag ) Eeprom.Buffer[47] |= 0x80;//������bit7��1
	}
	
	Result = Write_Event_Point(offset);
	
	if( page==ee_dayfreezedl_page )//�ն���ʣ����͹������
	{ 	
		Hex2Bcd( Money.Buy_Times.B08, Eeprom.Buffer+56, 4 );
		memcpy( Eeprom.Buffer+48, Eeprom.Buffer+58, 2 );//���Ṻ�����
		memcpy( Eeprom.Buffer+50, Money.Remain+2, 4 );//����ʣ����
		memcpy( Eeprom.Buffer+54, Money.Tick+2, 4 );//����͸֧���
		I2C_Write_Eeprom( ee_dayfreezemoney_page+Result, ee_dayfreezemoney_inpage, Eeprom.Buffer+48, ee_dayfreezemoney_lenth );
		
		Wr_Pretime_Dayfreeze();	//д��һ���ն���ʱ��	
	}

	I2C_Write_Eeprom( page+Result*2, 0x00, Eeprom.Buffer, ee_freezedl_lenth/2 );	//48�ֽڷ�2��д
	I2C_Write_Eeprom( page+Result*2+1, 0x00, Eeprom.Buffer+ee_freezedl_lenth/2, ee_freezedl_lenth/2 );
}


void Hour_Freeze_Dl(uint16 page,unsigned char offset)//���㶳����߰�㶳��
{
	unsigned char Result,inpage;
	
	memcpy( Eeprom.Buffer, Sclock.Clockbackup+1, 5 );
	DltoEepromBuf( 5 );
	Result = Write_Event_Point(offset);
	inpage = (Result%4)*13;
	I2C_Write_Eeprom( page+(Result>>2), inpage, Eeprom.Buffer, ee_hourfreezedl_lenth );
}


//status=0��ʾͣ�����,status=1��ʾ�ϵ����
//RAMREG5����������,RAMREG6������մ���,RAMREG7�������һ�����մ���,RAMREG8�½�����һ�����մ���,�ϵ粹��ʹ��
//Ic_Comm_RxBuf��Ϊ����ʱ����ʱ������
void Histroy_Freeze_Dl(unsigned char status)//����������ᴦ��
{
	unsigned char i, Result=0;
	unsigned char  Buf[10];
	
	if( BDMODE ) //����ģʽ
	{
		//������ж�
		if( status ) Result = Year_Ladder_Judge(); //����������жϺ����ݺϷ����ж�

		for( i=0; i<ee_curyearjs_len; i+=3 )
		{
			if( Cmp_Data( Price.Year_Ladder+i, Sclock.Clockbackup+2, 3 ) == 0x03 ) break;
		}
		
		if( (i<ee_curyearjs_len) && (Price.Ladder_Mode == 0x55) && (Sclock.Clockbackup[3] <= 0x28) ) //ʱ�������
		{
	  	if( status )
	  	{
	  		if( (Result!=0x55)&&(Result!=0x5a) ) Freeze_Usedl(0);//�����,���õ���ת�������
	  	}
	  	else
	  	{
	  		if( RAMREG5 < 7 ) RAMREG5++;	//֧����1~��6���õ���
	  		memcpy( Ic_Comm_RxBuf+60, Ic_Comm_RxBuf+60+4, 24 );//��λ��ʷͣ�������ʱ��
				memcpy( Ic_Comm_RxBuf+60+24, Sclock.Clockbackup+2, 4 );//���浱ǰͣ�������ʱ��(ʱ������)
	  	}
		}
	}
	else
		Price.Ladder_Mode = 0x00;	//��֧�������
	
	//�������ж�
	ReadE2WithBackup( ee_histroyparameter_page, ee_histroyparameter_inpage, Buf, ee_histroyparameter_lenth );
	if( ((Buf[0]== Sclock.Clockbackup[2]) && (Buf[1]== Sclock.Clockbackup[3])) ||
	    ((Buf[2]== Sclock.Clockbackup[2]) && (Buf[3]== Sclock.Clockbackup[3])) ||
	    ((Buf[4]== Sclock.Clockbackup[2]) && (Buf[5]== Sclock.Clockbackup[3])) ) 
	{
		//�������ж�
		if( status )
		{
			if( (Result!=0x5a)&&(Result!=0xaa) )	
			{
				Freeze_Dl(ee_histroy_dl_page,ee_histroyf_inpage);
				FreezeMonth_Money();
				FreezeMonth_Demand(0);
			}
		}
		else
		{
			if( RAMREG6 < 12 ) RAMREG6++;
			memcpy( Ic_Comm_RxBuf, Ic_Comm_RxBuf+4, 44 );//��λ��ʷͣ�綳��ʱ��
			memcpy( Ic_Comm_RxBuf+44, Sclock.Clockbackup+2, 4 );//���浱ǰͣ�綳��ʱ��(ʱ������)
		}
		
		//��һ�������ж�
		if((Buf[0]== Sclock.Clockbackup[2]) && (Buf[1]== Sclock.Clockbackup[3]))
		{
			//�������һ������
			if( Price.Ladder_Mode == 0x55 )
			{
				if( status )
				{
					if( Result!=0x5a) Freeze_Usedl(1);//�����,��ת��
				}
				else
				{
					if( RAMREG7 < 2 ) RAMREG7++;
				}
			}
			else//�½�����һ������
			{
				if( status )
				{
					if( Result!=0xaa) Freeze_Usedl(2);//�½���,���õ���ת�������
				}
				else
				{
					if( RAMREG8 < 2 ) RAMREG8++;
				}
			}
		}	
	}
}


void Freeze_Judge(void)	//����ͽ����ж�
{
	unsigned char i;
	unsigned char  Result,Buf[20];
	
	run_freeze_flag = 0;
	run_loadchk_flag = 1;
	
	I2C_Read_Eeprom44( ee_freezeparameter_page, Buf, ee_freezeparameter_lenth );
		
	if( FLAG_EA && ((Cmp_Clock( Buf, 5 ) != 0x01)) )//���㶳����ʼʱ��<=��ǰʱ��
	{
		Result = Sclock.Clockbackup[1];
		if( (Result == 0) || ((Result == 0x30)&&(Buf[5] == 0x30)) )
		{
			Hour_Freeze_Dl(ee_hourfreezedl_page,ee_hourf_inpage);//���㶳��
		}
	}
	
	if( FLAG_EA && (Cmp_Clock( Buf+6, 2 ) == 0x03) ) 
	{
		Freeze_Dl(ee_dayfreezedl_page,ee_dayf_inpage); //�ն���ʱ��=��ǰʱ��,�ն���
		FreezeDay_Money(); //�ն���������
	}	
        
	for(i=0; i<3; i++)
	{
		if(Buf[8+i]== 0x99) Buf[8+i] = Sclock.Clockbackup[4-i];
	}
	if( FLAG_EA && ((Cmp_Clock( Buf+8, 4 ) == 0x03)) ) 
	{
		Freeze_Dl(ee_timingfreeze_dl_page,ee_timingf_inpage); //��ʱ����
	}
		
	if( Sclock.Clockbackup[1]== 0x00 )//���ӵ���0
	{
		Histroy_Freeze_Dl(1);//����������ᴦ��
		RTC_Status &= ~RTC_INTSTS_WKUHOUR;	
//		RTCIF = 0;	//��Сʱ�仯��־
	}				
}


void Pd_Histroyfreeze_Judge(void)	//�ϵ�����ղ����жϴ���
{
	unsigned char  i;
	unsigned char  Buf[10];
	
	//ͣ�粹���ᴦ��
	if( ((RAMREG5 > 0)&&(RAMREG5 < 8)) || ((RAMREG7 > 0)&&(RAMREG7 < 3)) )
	{
		memcpy( Buf, Sclock.Clockbackup+2, 4 );//���浱ǰʱ��(ʱ������)
		for( i=0; i<RAMREG5; i++ )
		{	
			memcpy( Sclock.Clockbackup+2, Ic_Comm_RxBuf+60+28-(RAMREG5-i)*4, 4 );//����ʱ��
			Freeze_Usedl(0);//�����,���õ���ת�������
		}
		memcpy( Sclock.Clockbackup+2, Buf, 4 );//�ָ���ǰʱ��(ʱ������)		
	
		for( i=0; i<RAMREG7; i++ )
		{	
			Freeze_Usedl(1);//�����,��ת��
		}
	}
	else if( (RAMREG8 > 0) && (RAMREG8 < 3) )
	{
		for( i=0; i<RAMREG8; i++ )
		{	
			Freeze_Usedl(2);//�½���,���õ���ת�������
		}
	}
	
	if( (RAMREG6 > 0) && (RAMREG6 < 13) )
	{
		memcpy( Buf, Sclock.Clockbackup+2, 4 );//���浱ǰʱ��(ʱ������)
		for( i=0; i<RAMREG6; i++ )
		{	
			memcpy( Sclock.Clockbackup+2, Ic_Comm_RxBuf+48-(RAMREG6-i)*4, 4 );//����ʱ��			
			Freeze_Dl(ee_histroy_dl_page,ee_histroyf_inpage); //���㶳��
		}
		memcpy( Sclock.Clockbackup+2, Buf, 4 );//�ָ���ǰʱ��(ʱ������)
	}
	
	if( (RAMREG6 > 0) && (RAMREG6 < 13) )
	{			
		for( i=0; i<RAMREG6; i++ )
		{	
			if( i== 0) FreezeMonth_Demand(0);
			else  FreezeMonth_Demand(1);
		}		
	}
	
	
	RAMREG5 = 0;//����������
	RAMREG6 = 0;//������մ���
	RAMREG7 = 0;//�������һ�����մ���
	RAMREG8 = 0;//�½�����һ�����մ���
}

//Type=0��ʾ����sleepǰ����,1��ʾsleep�е���,2��ʾsleep��������
/*RAMREG9��־˵����
bit0����������ն����־
bit1���������㶳���־
bit2�������ն����־
bit3�����涨ʱ�����־

bit5������ʱ���ѿ����㶳����ʼʱ��
bit6���ն�����ʱ��־
bit7����ʱ������ʱ��־*/
void Pd_Freeze_Judge(unsigned char CH)
{
	unsigned char  i;
	unsigned char Buf[ee_freezeparameter_lenth];
	
	I2C_Read_Eeprom44( ee_freezeparameter_page, Eeprom.Buffer, ee_freezeparameter_lenth );//����������ʱ��
	
	memcpy(Buf, Eeprom.Buffer, ee_freezeparameter_lenth);
	
	//���㶳���ж�
	if( (Cmp_Clock( Eeprom.Buffer, 5 ) != 0x01) )//���㶳����ʼʱ��<=��ǰʱ��
	{
		if(CH == 0)
		{
			RAMREG9 |= B0010_0000;	//�����ѹ����㶳����ʼʱ���־
		}
		
		if(CH == 1)
		{
			RAMREG9 |= B0000_0010;	//�ϵ粹�����㶳���־
			RAMREG9 &= B1101_1111;	//����ʱ��־
		}
	}
	
	if(CH < 2)
	{
		//�ն����ж�
		if((RAMREG9 & B0100_0000) != 0)
		{
			RAMREG9 |= B0000_0100;	//�ϵ粹���ն������
			RAMREG9 &= B1011_1111;	//����ʱ��־
		}
		if((RAMREG9 & B0000_0100) == 0)
		{
			if(Eeprom.Buffer[6] == Sclock.Clockbackup[2]) //ʱ
			{
				if(CH == 1)
				{
					if(Eeprom.Buffer[7] == 0)	RAMREG9 |= B0000_0100;	//��, �ն����־
					else	RAMREG9 |= B0100_0000;	//�ն�����ʱ��־
				}
				else
				{
					if(Sclock.Clockbackup[1] < Eeprom.Buffer[7]) RAMREG9 |= B0100_0000;	//�ն�����ʱ��־
				}
			}
		}
		
		//��ʱ�����ж�
		if((RAMREG9 & B1000_0000) != 0)
		{
			RAMREG9 |= B0000_1000;	//�ϵ粹�涨ʱ�������
			RAMREG9 &= B0111_1111;	//����ʱ��־
		}
		if((RAMREG9 & B0000_1000) == 0)
		{
			for(i=0; i<3; i++)
			{
				if(Eeprom.Buffer[8+i]== 0x99) Eeprom.Buffer[8+i] = Sclock.Clockbackup[4-i];
			}
			if( (Eeprom.Buffer[ 8] == Sclock.Clockbackup[4]) &&
			    (Eeprom.Buffer[ 9] == Sclock.Clockbackup[3]) &&
				(Eeprom.Buffer[10] == Sclock.Clockbackup[2]) )
			{
				if(CH == 1)
				{
					if(Eeprom.Buffer[11] == 0)	
					{
						RAMREG9 |= B0000_1000;	//�ϵ粹�涨ʱ�������
						RAMREG9 &= B0111_1111;	//����ʱ��־
					}
					else	RAMREG9 |= B1000_0000;	//��ʱ������ʱ��־
				}
				else
				{
					if(Sclock.Clockbackup[1] < Eeprom.Buffer[11]) RAMREG9 |= B1000_0000;	//��ʱ������ʱ��־
				}				
			}
		}
	}
	else//�ϵ粹���ж�
	{
		if( Eeprom.Buffer[5] == 0x30 )	//���㶳����ʱ����30min
		{    			
			if( (RAMREG9 & B0010_0000) != 0 )//����ʱ�ѹ�����ʱ��
			{
				if( (RAMREG1 < 30) && (Sclock.Clockbackup[1] >= 0x30) )	//ͬʱ�������С��30min,�ϵ���Ӵ���30min
				{
					RAMREG9 |= B0000_0010;//���㶳��
				}
			}
			else if( (Cmp_Clock( Eeprom.Buffer, 5 ) != 0x01) ) //�ϵ�������ʱ��
			{
				if( (Eeprom.Buffer[4] <= 0x30) && (Sclock.Clockbackup[1] >= 0x30) )	//����ʱ�����С��30min,�ϵ���Ӵ���30min
				{
					RAMREG9 |= B0000_0010;//���㶳��
				}
			}
		}
		
		if((RAMREG9 & B0000_0010) != 0)
		{
			Hour_Freeze_Dl(ee_hourfreezedl_page,ee_hourf_inpage);//���㶳��
		}
			
		if(((((RAMREG9 & B0100_0000) != 0) && (Sclock.Clockbackup[1] >= Buf[7])))	|| ((RAMREG9 & B0000_0100) != 0))
		{
			Freeze_Dl(ee_dayfreezedl_page,ee_dayf_inpage); //�ն���
		}
		
		if(((((RAMREG9 & B1000_0000) != 0) && (Sclock.Clockbackup[1] >= Buf[11]))) || ((RAMREG9 & B0000_1000) != 0))
		{
			Freeze_Dl(ee_timingfreeze_dl_page,ee_timingf_inpage);//��ʱ����
		}
		
		RAMREG9 = 0;
	}
}


void Init_Dayfreeze_Judge(void)	//�ն��Ჹ���ж�
{
	unsigned char i, n, k,result,result1,result2;
	unsigned char Buf[50],BufA[6],BufB[6],BufC[6];
	const unsigned char code days[] = { 0, 0x31, 0x28, 0x31, 0x30, 0x31, 0x30, 0x31, 0x31, 0x30, 0x31, 0x30, 0x31 };
	
	if(clock_error) return;	//��ǰʱ��Ƿ�
	
	I2C_Read_Eeprom44( ee_pre_dayfreeze_page, BufB, ee_predayfreeze_lenth );//����һ���ն���ʱ��(��ʱ������)
	if( JudgeClockNoWeek( 1, BufB, 5 ) ) return;	//ʱ��Ƿ�
											
	memcpy( BufA, Sclock.Clockbackup+1, 5 );//��ǰʱ��(��ʱ������)
	
	result = Cmp_Bcd_Inv( BufA, BufB, 5 );//�ȽϷ�ʱ������5�ֽ�
	if( result != 1 ) return;//A=B����A<B,����
	
	I2C_Read_Eeprom44( ee_freezeparameter_page, Buf, ee_freezeparameter_lenth );//���ն���ʱ��

	//���ÿ���ն���ʱ��
	memcpy( BufC, BufA, 5 );//��ǰʱ��
	BufC[0] = Buf[7];//�ն����
	BufC[1] = Buf[6];//�ն���ʱ
	
	n = 0;
	for( i=0; i<8; i++ )
	{
		result1 = Cmp_Bcd_Inv( BufB, BufC, 5 );
		result2 = Cmp_Bcd_Inv( BufA, BufC, 5 );
		
		//BufB<BufC<=BufA��
		if((result1==2)&&(result2!=2))
		{
			n++;//���ն��������1
			memcpy( Buf+n*5, BufC, 5 );//����ʱ��
		}
		
		//����ʱ���1��
		if( BufC[2] == 0x01 )//�Ƚϵ�ǰ���Ƿ�Ϊ1
		{
			if( BufC[3] == 0x01 )
			{
				BufC[4] = Hex_To_Bcd(Bcd_To_Hex(BufC[4])-1);//���1
				BufC[3] = 0x12;//12��
				BufC[2] = 0x31;//31��
			}
			else
			{
				BufC[3] = Hex_To_Bcd(Bcd_To_Hex(BufC[3])-1);//�¼�1
				//�����ж�
				k = days[Bcd_To_Hex(BufC[3])];
				if( (BufC[3] == 0x02) && ((Bcd_To_Hex(BufC[4])%4)==0 ) ) k++;	//����
				BufC[2] = k;//�����
			}	
		}
		else
		{
			BufC[2] = Hex_To_Bcd(Bcd_To_Hex(BufC[2])-1);//�ռ�1
		}
	}
	
//	if( n > 7 ) n = 7;
//	
//	if( n )
//	{
//		memcpy( Buf, Sclock.Clockbackup+1, 5 );//���浱ǰʱ��(��ʱ������)
//		
//		for(i=0; i<n; i++)
//		{
//			memcpy( Sclock.Clockbackup+1, Buf+(n-i)*5, 5 );//����ʱ��
//			Freeze_Dl(ee_dayfreezedl_page,ee_dayf_inpage); //�ն��Ჹ��
//		}
//	
//		memcpy( Sclock.Clockbackup+1, Buf, 5 );//�ָ���ǰʱ��(��ʱ������)
//	}
	
	if( n ) Freeze_Dl(ee_dayfreezedl_page,ee_dayf_inpage); //�ն��Ჹ��(����Ҫ���ϵ粹һ��)
		
}

void Wr_Pretime_Dayfreeze(void)//�ն���д/����д/Уʱд/�����ն���ʱ��д
{
	I2C_Write_Eeprom44( ee_pre_dayfreeze_page, Sclock.Clockbackup+1, ee_predayfreeze_lenth );//д��һ���ն���ʱ��
}


void Judge_Relay_Delay(void)	//�������ʱ��բ�ж�
{
//	if( Relay_Mode != 0x55 )//���ü̵���
	{
		if( over_xxx_flag && CurrentTimer.B32 )//��բʱ����������ֵ��ʱ��բ
		{
			if( relay_commu_open_flag && (Opendelaytimer.B32 == 0x00) ) relay_delay_flag = 1;//�ô������ʱ��բ��־
			else if( BDMODE && (!relay_commu_close_flag) && (charge_relay_open_flag || relay_dl_status_flag) ) relay_delay_flag = 1;	//�ô������ʱ��բ��־
			else
			{
				CurrentTimer.B32 = Calculate_Timer(1);
				relay_delay_flag = 0;	//��������ʱ��բ��־
			}
		}
		else relay_delay_flag = 0;	//��������ʱ��բ��־
	}
//	else	relay_delay_flag = 0;	//��������ʱ��բ��־
}				


void Closerelay1(void)               //�պϼ̵���
{
	if( Relay_Mode != 0x55 )//���ü̵���
	{
		Relayctrl.Timer = Relaytimer_Delay_N;
		Powertimer.Timer = Powertimer_Delay_N;
		RELAYOFF1;
		__NOP();
		__NOP();
		RELAYON2;
	}
	else
	{
		if(Mode.Feature1&0x01)//���巽ʽ
		{
			Relayctrl.Timer = 0;
			RELAYOFF3;
		}
		else
		{
			RELAYOFF3;
		}
	}
}

void Openrelay1(void)
{
	if( Relay_Mode != 0x55 )//���ü̵���
	{
		Relayctrl.Timer = Relaytimer_Delay_N;
		RELAYOFF2;
		__NOP();
		__NOP();
		RELAYON1;
	}
	else
	{
		if(Mode.Feature1&0x01)//���巽ʽ
		{
			Relayctrl.Timer = Relaytimer_Delay_W;
			RELAYON3;	
		}
		else
		{
			RELAYON3;
		}
	}
}

void Init_Metererrstatus(void)	//����̵�������״̬
{
	relay_error = 0;
	Relayerror_Count  = 0;
	Relayerror_Count1 = Defrelayerrorcnt_s;//��ֵ��ʼ����
	Relayerror_Count2 = Defrelayerrorcnt_s;//��ֵ��ʼ����
	relay_failed_flag = 0;	//��̵��������־(�ϱ�)
	Store_Meterstatus();
}


void Closerelay(void)               //�պϼ̵���(��д״̬��)
{
	if( (relay_status_flag == 0) && (relay_direct_flag == 0) ) return;
	
	Closerelay1();
	if( relay_status_flag == 1 )
	{
		if( relay_error ) Note_Run_Event(ee_relayerrn_inpage|0x80);	//���ɿ����쳣������¼
		relay_status_flag = 0;
		relay_success_flag = 0;	//��̵����ɹ���־
		Relay_modechange_flag = 0;
		Init_Metererrstatus();	//����̵�������״̬
		Note_Relay_Event();//����բ��¼
	}
	Relay_Space_Timer = 0;	
}



void Openrelay(void)                //�Ͽ��̵���(��д״̬��)	
{
	if( relay_status_flag && (relay_direct_flag ==0 ) )  return;
	
	Judge_Relay_Delay();	//�������ʱ��բ�ж�
	if( relay_delay_flag ) return;	//�������ʱ��բ
	
	Openrelay1();
	if( relay_status_flag == 0 ) 
	{
		if( relay_commu_open_flag )
		{
			Opendelaytimer.B32 = 0x00;	//�Ѿ���բ������բ��ʱ
		}
		else if( (BDMODE) && (charge_relay_open_flag || relay_dl_status_flag) )
		{
			memset( Eeprom.Buffer, 0xff, 4 );	//���ò����ߴ���Ϊ0xff
			I2C_Write_Eeprom( ee_operatorcode_page, ee_operatorcode_inpage, Eeprom.Buffer, 4 );
		}
		
		if( relay_error ) Note_Run_Event(ee_relayerrn_inpage|0x80);	//���ɿ����쳣������¼
		relay_status_flag = 1;
		relay_success_flag = 0;	//��̵����ɹ���־
		Relay_modechange_flag = 0;
		if( Relay_Mode == 0x55 ) relay_cleardl_flag = 0; //�����ñ�������־
		Init_Metererrstatus();	//����̵�������״̬
		Note_Relay_Event();//����բ��¼
	}
	Relay_Space_Timer = 0;
}


void Alarmstatus_Judge(void)	//�����״̬�ж�
{
	unsigned char Result1=0;
	
	Remainmoney_Verify();	//ʣ�������У��
	Money_Para_Verify();	//��Ƿ���,�ڻ����Ȳ���У��

//	RTCIE = 0;//��ֹҺ������˸һ��
	Meter_Status  &=   B1000_1000;
	if( BDMODE ) //����ģʽ
	{
		if( ChkBufZero( &Money.Remain[2], 4 ) )//ʣ�������0
		{
			remainder_tick_flag = 0;//����Ƿ��־
			
			Result1 = Cmp_DataLH( &Money.Remain[2],  Esam_Para.Alarm_Limit1.B08,4 );
			if( Result1 == 0x01 ) 
			{
				relay_dl_status_flag = 0;//�պϼ̵���
			}
			else
			{	
				Result1 = Cmp_DataLH( &Money.Remain[2], Esam_Para.Alarm_Limit2.B08, 4 );
				if( Result1 == 0x01 )
				{
					remainder_alarm1_flag = 1;
					relay_dl_status_flag = 0;//�պϼ̵���
				}
				else
				{
					remainder_alarm2_flag = 1;
					if ( card_closerelay_flag ) //�ⲿ�øñ�־,��Ҫ���浽����ee
					{
						first_alaram2_flag =0;
						relay_dl_status_flag = 0;//�պϼ̵���
					}
					else
					{
						first_alaram2_flag =1;
						relay_dl_status_flag = 1;//�Ͽ��̵���
					}
				}
			}
				
		}
		else //ʣ����С�ڵ���0
		{
			remainder_zero_flag =1;
			if( ChkBufZero( Money.Tick, 4 ) )
			{
				if( remainder_tick_flag == 0 )
				{
					Set_Reportdata(1*8+1);//��͸֧״̬�ϱ���־
				}
				remainder_tick_flag = 1;//����Ƿ��־
			}
			else remainder_tick_flag = 0;//����Ƿ��־
			
			Result1 = Cmp_DataLH( &Money.Tick[2], Money_Para.Tick.B08, 4 );
			
			if( Result1 != 0x02 )
			{
				if(remainder_maxtick_flag == 0)
				{
					remainder_maxtick_flag = 1;//ʣ�����Ƿ����(�״�)
					Store_Meterstatus();
				}				
				relay_dl_status_flag = 1;//�Ͽ��̵���,������Ƿ���޻�����Ƿ���޵���0
			}
			else
			{			
				if ( card_closerelay_flag1 ) //�ⲿ�øñ�־,��Ҫ���浽����ee
				{
					first_eqzero_flag =0;
					relay_dl_status_flag = 0;//�պϼ̵���
				}
				else
				{
					first_eqzero_flag =1;
					relay_dl_status_flag = 1;//�Ͽ��̵���
				}
				
//				if( remainder_maxtick_flag )//������ʣ�����Ƿ���ޣ�������ֵ����բ��������ܺ�բ
//				{
//					relay_dl_status_flag = 1;//�Ͽ��̵���,������Ƿ���޻�����Ƿ���޵���0
//				}		
			}
			
//		if( remainder_maxtick_flag )//������ʣ�����Ƿ���ޣ�������ֵ����բ��������ܺ�բ
//		{
//			first_alaram2_flag =0;
//			first_eqzero_flag =0;
//		}		
		}
		
//		if( charge_relay_open_flag )//ʣ����С�ڵ��ں�բ������
//		{
//			first_alaram2_flag =0;
//			first_eqzero_flag =0;
	//	}		
	}
//	RTCIE = B0000_0001;
}


void Mt_Alarmstatus_Judge(void)	//�����״̬�ж�
{
	Esam_Para_Verify();	//�������1,2�Ȳ���(ESAMһ�ಿ�ֲ���)У��
	Alarmstatus_Judge();	//�����״̬�ж�

	if( FLAG_EA && ( below_70u_flag == 0 ) )
	{
		//�̵�������
		if ( relay_commu_disable_close_flag )//������պϼ̵���(ֻ�в忨�ſ�������)
		{
			Openrelay();
		}
		else if ( relay_commu_close_flag )//�������ȼ����
		{
			Closerelay();
		}
		else if( (relay_commu_open_flag && (Opendelaytimer.B32 == 0x00)) && (self_close_flag == 0) )//ͨѶ�Ͽ��̵������ȼ���֮
		{
			Openrelay();
		}
		else if( (BDMODE) && (charge_relay_open_flag || relay_dl_status_flag)&& (Festival_holiday_flag == 0) )//���粻�պϼ̵�����־, ���������̵����Ͽ�
		{
			Openrelay();
		}
		else//���������̵����պ�
		{
			Closerelay();
		}
		
		relay_direct_flag = 0;//��̵���ֱ�Ӷ�����־
	}
	
	run_alarm_flag = 0;		
}
