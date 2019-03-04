#include "ex_func.h"
#include "ex_var.h"
//#include <intrins.h>
#include <string.h>
#include <absacc.h>

void Init_Pad_Io(void)
{	
	GPIOA->DAT = 0x0000FFFF;
	GPIOB->DAT = 0x0000FFFB;
	GPIOC->DAT = 0x0000FFFF;
	GPIOD->DAT = 0x0000FFFF;
	GPIOE->DAT = 0x0000FFFF;
	GPIOF->DAT = 0x00000000;
}

void Init_Pad_Mdoe(void)
{
  GPIO_InitType GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUTPUT_CMOS;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
  GPIOBToF_Init(GPIOB, &GPIO_InitStruct);
}

void Init_Int(void)	//��֤INT0��INT3Ϊ�ߵ�ƽ
{
}

void INIT_F251WDT(void)
{	
    WDT_Enable();//ʹ��WDT
}

void Clear_Wdt(void)
{
    WDT_Clear();
}

void Clear_SYS_Wdt(void)
{
    WDT_Clear();
}

void Delay_Initial(unsigned char Num) //��ʱNum*5ms(���ʱ���С��800ms)
{
	unsigned char i;
	
	Clear_Wdt();
	for( i=0; i<Num; i++ )
	{
		I2c_Wait_5ms();
		if( FLAG_EA == 0 ) break;
	}
	Clear_Wdt();
}


void Clear_Nv_Ram(void)//��startup��δ����Ĳ���ram
{
//	memset( (uint08 near*)(0xF50), 0x00, 0x56 );
}



void Nvram_Judge(void)	//���粻���ram�Ϸ����ж�
{
	if( (Password_Err[0]>Defpassworderrcnt) || (Password_Err[1]>Defpassworderrcnt) || (Mac_Err_Times > 200) ) Clear_Nv_Ram();//��startup��δ����Ĳ���ram
	
}


void Note_Reset_Number(void)	//��¼�ϵ縴λ��wdt��λ����,������ϵ縴λ��ram
{	
	unsigned char i;
	
	for( i=0; i<20; i++ ) I2c_Wait_5ms();	//��ʱ100ms
		
	if( I2C_Read_Eeprom44( ee_meterstatus1_page, Eeprom.Buffer, ee_meterstatus1_lenth)==0 )//�����״̬�ֵ�ram�ɹ�
	{
		Meter_Status_S.B08  = Eeprom.Buffer[0]&B0000_1000;//�ָ�ʣ�����Ƿ����(�״�)
		Meter_Status1_S.B08 = Eeprom.Buffer[1];
		Meter_Status2_S.B08 = Eeprom.Buffer[2];
	}
	
	if( I2C_Read_Eeprom44( ee_factorymode_page, Eeprom.Buffer, ee_factorymode_lenth ) == 0 )//���������ʣ�������������������
	{
		if( Eeprom.Buffer[0] == 0x55 ) progenable = 1;	//���ʹ��
		else progenable = 0;	//��̽�ֹ
	}
	
//	if( RSTFLAG & B0000_1000 )
//	{	
//		I2C_Read_Eeprom44( ee_wdt_page, Eeprom.Buffer,ee_wdt_lenth );//�ϵ縴λ������1
//		Eeprom.Buffer[1]++;
//		if( Eeprom.Buffer[1] == 0 ) Eeprom.Buffer[0]++;
//		I2C_Write_Eeprom44( ee_wdt_page, Eeprom.Buffer, ee_wdt_lenth );
//		RSTFLAG=0;
//		Clear_Nv_Ram();//��startup��δ����Ĳ���ram
//		
//		if( I2C_Read_Eeprom( ee_reportdata_page, 0, Eeprom.Buffer, ee_reportdata_lenth)==0 )//���¼��ϱ�����
//		{
//			if( Eeprom.Buffer[0] != 0xff )//��ֹ���ַǷ�����
//			{
//				System_Status = (Eeprom.Buffer[0]&0x3f);//�ָ���־
//				if( Eeprom.Buffer[0]&0x80 ) Cover.Status = 1;//�ָ�����״̬
//				memcpy( Report_Data, Eeprom.Buffer+1, ee_reportdata_lenth-1 );
//			}
//		}
//
//	}
	//33
//	if ( RSTFLAG & B0001_0000 )
//	{	
//		I2C_Read_Eeprom44( ee_wdt_page, Eeprom.Buffer, ee_wdt_lenth );//wdt��λ������1
//		Eeprom.Buffer[3]++;
//		if( Eeprom.Buffer[3] == 0 ) Eeprom.Buffer[2]++;
//		I2C_Write_Eeprom44( ee_wdt_page, Eeprom.Buffer, ee_wdt_lenth );
//		RSTFLAG=0;	
//	}
//		
//	else if ( RSTFLAG & B0000_0001 )
//	{	
//		I2C_Read_Eeprom44( ee_wdt_page, Eeprom.Buffer, ee_wdt_lenth );//ϵͳwdt��λ������1
//		Eeprom.Buffer[5]++;
//		if( Eeprom.Buffer[5] == 0 ) Eeprom.Buffer[4]++;
//		I2C_Write_Eeprom44( ee_wdt_page, Eeprom.Buffer, ee_wdt_lenth );
//		Clear_Nv_Ram();//��startup��δ����Ĳ���ram
//		RSTFLAG=0;	
//	}
//	
//	else if ( (LVDSTATUS & B0000_0001) || (RAMREG3 == 0xaa) )//�ϵ绽�ѻ���ͨ������������ʼ��
//	{
//		Nvram_Judge();//���粻���ram�Ϸ����ж�
//		System_Status = System_St_Bk; //�ָ���־
//		LVDSTATUS = 0;
//	}
//	
//	else//�쳣
//	{
//		I2C_Read_Eeprom44( ee_wdt_page, Eeprom.Buffer, ee_wdt_lenth );//�쳣��λ������1
//		Eeprom.Buffer[7]++;
//		if( Eeprom.Buffer[7] == 0 ) Eeprom.Buffer[6]++;
//		I2C_Write_Eeprom44( ee_wdt_page, Eeprom.Buffer, ee_wdt_lenth );
//		Clear_Nv_Ram();//��startup��δ����Ĳ���ram
//		
//		if( I2C_Read_Eeprom( ee_reportdata_page, 0, Eeprom.Buffer, ee_reportdata_lenth)==0 )//���¼��ϱ�����
//		{
//			if( Eeprom.Buffer[0] != 0xff )//��ֹ���ַǷ�����
//			{
//				System_Status = (Eeprom.Buffer[0]&0x3f);//�ָ���־
//				if( Eeprom.Buffer[0]&0x80 ) Cover.Status = 1;//�ָ�����״̬
//				memcpy( Report_Data, Eeprom.Buffer+1, ee_reportdata_lenth-1 );
//			}
//		}		
//	}
	
	RAMREG3 = 0;//����������־�ֽ�
	Nvram_Judge();//���粻���ram�Ϸ����ж�
}

unsigned char Clk_Adj_Scan(unsigned char trimA, unsigned char trimB)
{
//	unsigned char temp08A,temp08B,GoodResult,i,n;
//	union B16_B08 Temp16;
//	uint16 DiffValue,TempDiffValue;
//	
//	DiffValue = 0xffff;
//	GoodResult = (trimA+trimB)/2.0+0.5;
//	
////	ET2CTRL1 = B0010_0100;	//ET2~~ , ��׽ģʽ,���ڲ�׽,�������㣬������׽
////	ET2CTRL2 = B1100_0000;	//����ԴFcpu����׽Դ32768���ſ�
////	ET2CFG1  = B0000_0001;	//ʱ��1=32768, Group2�ź�=fcpu, Group1�ź�=32768
////	
////	ET2PRESCALE = 127;//128��Ƶ

////	ET2IE	 = 0;		//��ֹET2�ж�
////	ET2IF	 = 0;		//bit0����׽�жϣ�bit1������ж�
////	
//	if(trimA<trimB) 	//��С����
//	{
//		temp08A = trimA;
//		temp08B = trimB;
//	}
//	else
//	{
//		temp08A = trimB;
//		temp08B = trimA;	
//	}
//		
////	for( i=temp08A; i<=temp08B; i++ )//ɨ��trimֵ
////	{
////		RCTRIM = i;
////		Clear_Wdt();
////		
////		ET2CTRL1 |= B1000_0000;	//����ET2
////		for( n=0; n<6; n++ ) //�������ж���Ҫ3.906ms = 128/32768 s
////		{					  
////			for( Temp16.B16=0x00; Temp16.B16<1500; Temp16.B16++ )	//7.5ms@8M	  15ms@4M	
////			{
////				if( ET2IF & B0000_0001 ) break;
////			}		
////			ET2IF = 0x00;
////		}	
////		ET2CTRL1 &= B0111_1111; 
////		if( Temp16.B16 >= 1500 ) continue;
////        	
////		Temp16.B08[0] = ET2PRESETH;
////		Temp16.B08[1] = ET2PRESETL;
////		
////		if(Temp16.B16>(31250/clkmode))
////		{
////			TempDiffValue = Temp16.B16-(31250/clkmode);
////		}
////		else
////		{
////			TempDiffValue = (31250/clkmode)-Temp16.B16;
////		}
////		
////		if( TempDiffValue<DiffValue )//�жϽ���Ƿ��ø�����
////		{
////			DiffValue = TempDiffValue;
////			GoodResult = RCTRIM;
////		}
//	}
//	return GoodResult;
    return 0;
}

void Clk_Adj_Proc( void )
{
//	union B16_B08 Temp16;
//	unsigned char n,m,tempbyte,TrimBuf;	
//		
//	Adj_Clk_Timer_S = Adjclk_timer_delay_s;	//����������ʱ8Mʱ�ӵ�У����(����uartר��)
//	Adj_Clk_Timer = Adjclk_timer_delay;	//����������ʱ8Mʱ�ӵ�У����
	
//	ET2CTRL1 = B0010_0100;	//ET2~~ , ��׽ģʽ,���ڲ�׽,�������㣬������׽
//	ET2CTRL2 = B1100_0000;	//����ԴFcpu����׽Դ32768���ſ�
//	ET2CFG1  = B0000_0001;	//ʱ��1=32768, Group2�ź�=fcpu, Group1�ź�=32768
//	
//	ET2PRESCALE = 127;//128��Ƶ

//	ET2IE	 = 0;		//��ֹET2�ж�
//	ET2IF	 = 0;		//bit0����׽�жϣ�bit1������ж�
//	
//	for( m=0; m<6; m++ )
//	{
//		Clear_Wdt();
//		
//		ET2CTRL1 |= B1000_0000;	//����ET2
//		for( n=0; n<6; n++ ) //�������ж���Ҫ3.906ms = 128/32768 s
//		{					  
//			for( Temp16.B16=0x00; Temp16.B16<1500; Temp16.B16++ )	//7.5ms@8M	  15ms@4M	
//			{
//				if( ET2IF & B0000_0001 ) break;
//			}		
//			ET2IF = 0x00;
//		}	
//		ET2CTRL1 &= B0111_1111; 
//		if( Temp16.B16 >= 1500 ) continue;
//        	
//		Temp16.B08[0] = ET2PRESETH;
//		Temp16.B08[1] = ET2PRESETL;
//		
//		if( (Temp16.B16>(31250-120)/clkmode)&&(Temp16.B16<(31250+120)/clkmode) ) return; //����������ֱֵ�ӷ���
//        	
//		TrimBuf = RCTRIM;
//		if( Temp16.B16 > 31250/clkmode )	//31250 = 8M*128/32768; 156.25 = (15625*2/4*2M)*40k
//		{
//			Temp16.B16 -= 31250/clkmode;
//			tempbyte = Temp16.B16/(156.25/clkmode);
//        	
//			if( tempbyte > 0x20 ) tempbyte = 0x20;
//			
//			if( RCTRIM >= tempbyte )
//			{
//				RCTRIM -= tempbyte;
//			}
//			else
//			{
//				RCTRIM = 0x00;
//			}
//			
//		}
//		else
//		{
//			Temp16.B16 = 31250/clkmode-Temp16.B16;
//			tempbyte = Temp16.B16/(156.25/clkmode);
//        	
//			if( tempbyte > 0x20 ) tempbyte = 0x20;
//        	
//			if( (RCTRIM+tempbyte)<=0x7F)
//			{
//				RCTRIM += tempbyte;
//			}
//			else
//			{
//				RCTRIM = 0x7F;
//			}			
//		}
//			
//		if( m == 5 )				//δ�ҵ�����ֵ,�������2�ε�ֵȡƽ��
//		{
////			TrimBuf += RCTRIM;
////			RCTRIM = TrimBuf/2.0+0.5;				
//			RCTRIM = Clk_Adj_Scan(TrimBuf,RCTRIM);		
//		}
//	}	
}

void Init_Dw80251(void)
{
//	IE   = B0001_0010;	//ʹ��TIME0�ж�,ʹ�ܴ���(������ռ��)�ж�
//	IPH  = B0000_0000;	//�����ȼ�
//	IPL  = B0000_0000;	//�����ȼ�
//	
//	AIE  = B0000_0111;	//ʹ�ܸ����ж�0/1/2����оƬ��������ռ�ã�
//	AIPH = B0000_0000;	//�����ȼ�
//	AIPL = B0000_0000;	//�����ȼ�
//	
//	TMOD = B0000_0001;	//16bit��ʱ,��ʱʱ��2731*1.5us=4096us
////	TL0  = 0x55;		//65536-2731=0XF555,@8M
////	TH0  = 0xF5;
//	TR0  = 1;		//����time0
}

void Clock_Init(void)
{
  CLK_InitTypeDef CLK_Struct;

  CLK_Struct.ClockType = CLK_TYPE_AHBSRC \
                        |CLK_TYPE_PLLL   \
                        |CLK_TYPE_HCLK   \
                        |CLK_TYPE_PCLK;
  CLK_Struct.AHBSource      = CLK_AHBSEL_LSPLL;
  CLK_Struct.PLLL.Frequency = CLK_PLLL_13_1072MHz;
  CLK_Struct.PLLL.Source    = CLK_PLLLSRC_XTALL;
  CLK_Struct.PLLL.State     = CLK_PLLL_ON;
  CLK_Struct.HCLK.Divider   = 1;
  CLK_Struct.PCLK.Divider   = 6;
  CLK_ClockConfig(&CLK_Struct);
}


void Powerd_Data_Process(void)	//ʱ�Ӵ����õ���ʱ��ָ���β���ָ�
{
	unsigned char  Buf[ee_powerdown_lenth+2];
	
	if( I2C_Read_Eeprom( ee_power_down_page, 0x00, Buf, ee_powerdown_lenth ) == 0 )
	{
		memcpy( Decimal.Fw, Buf+7, 4 );//�ָ�����/��������β������������
		
		if( clock_error == 0 )
		{
			if( Cmp_Bcd_Inv(Sclock.Clockbackup, Buf, 6)==2 ) clock_error = 1;//�ϵ�ʱ��С�ڵ���ʱ��
			else
			{
				Buf[7] = Buf[5];//���������
				Buf[5] = Hex_To_Bcd(Bcd_To_Hex(Buf[5])+10);	//����ʱ���n��(n=10)
				if( Cmp_Bcd_Inv(Sclock.Clockbackup, Buf, 6)==1 ) clock_error = 1;//�ϵ�ʱ����ڵ���ʱ���n��	
				Buf[5] = Buf[7];//������ָ�
			}
			
			if( clock_error ) Set_Reportdata(0*8+7);//��ʱ�Ӵ����ϱ���־
		}
		
		if( clock_error )
		{
			memcpy( Buf+7, Buf, 3 );//���ʱ
			memcpy( Buf+11, Buf+3, 3 );//������
			Buf[10] = Buf[6];//��
			Set_Rtc( 0, Buf+7, 7 );//�����м�
			Read_Sysclock();	//��ʱ��
			
			//������ϵ�ʱ�Ӽ���ʱ����ʼʱ��
			I2C_Write_Eeprom( ee_newclock_page, ee_newclock_inpage, Sclock.Clockbackup, ee_newclock_lenth );
		}
		
		if( Buf[18] != 0xff )//��ֹ���ַǷ�����
		{
			memcpy( Sclock.Clockbackup, Buf, 7 );//ʹ�õ���ʱ������¼���¼
			if( Buf[18]&0x80 ) Note_Run_Event( ee_powerdownn_inpage ); //�����¼���ʼ��¼
			if( Buf[18]&0x40 ) relay_error = 1;	//���ɿ����쳣������¼
			if( Buf[18]&0x20 )
			{ 
				Note_Run_Event(ee_maxi_inpage|0x80);	//��¼���������¼�(����)
			}
			Syn_Clock();	//ͬ��ʱ��
		}
		
		Wr_Powerdown_Data(0);//д����������ݵ�����ee(��ҪĿ��������β������ر�־)
	}
		
	run_loadchk_flag = 0;	//��ֹ�ϵ��󴥷����ɼ�¼
	if( Crc_Check( Load.Validpoint[0].B08, ee_loadpoint_lenth+2 ) )	//ram���ɼ�¼ָ�����
	{
		memcpy( Load.Validpoint[0].B08, Buf+23, ee_loadpoint_lenth+2 );//�����ɼ�¼ָ�뵽ram
	}
}
void MeterParaRead( void )
{
    unsigned long Tmp1;

    Tmp1 = (unsigned short)(1.5*MeterParameter.MeterUn);
    WORD_BCD2( (unsigned char*)&MeasPara.Un150, Tmp1 );
    
    Tmp1 = (unsigned short)(0.9*MeterParameter.MeterUn);
    WORD_BCD2( (unsigned char*)&MeasPara.Un90, Tmp1 );

    Tmp1 = (unsigned short)(0.75*MeterParameter.MeterUn);
    WORD_BCD2( (unsigned char*)&MeasPara.Un75, Tmp1 );

    Tmp1 = (unsigned short)(0.68*MeterParameter.MeterUn);
    WORD_BCD2( (unsigned char*)&MeasPara.Un68, Tmp1 );

    Tmp1 = (unsigned short)(0.60*MeterParameter.MeterUn);
    WORD_BCD2( (unsigned char*)&MeasPara.Un60, Tmp1 );

    Tmp1 = (unsigned short)(0.40*MeterParameter.MeterUn);
    WORD_BCD2( (unsigned char*)&MeasPara.Un40, Tmp1 );

    Tmp1 = (unsigned short)(MeterParameter.MeterIb*MeterParameter.CT/10);
    HEX4_BCD4( (unsigned char*)&MeasPara.StartI, Tmp1 );//??????BCD XXX.XXX
    Tmp1 = (unsigned short)(MeterParameter.MeterIb*MeterParameter.CT);
    HEX4_BCD4( (unsigned char*)&MeasPara.StartI2, Tmp1 );//??????BCD XX.XXXX
    
    Tmp1 = (MeterParameter.MeterUn*MeterParameter.MeterIb*MeterParameter.CT)/10000;//??????
    HEX4_BCD4( (unsigned char*)&MeasPara.StartP, Tmp1 );//BCD XX.XXXX
    
    Tmp1 = (MeterParameter.MeterUn*MeterParameter.MeterIb*MeterParameter.CT)/1000;//??????
    HEX4_BCD4( (unsigned char*)&MeasPara.StartP2, Tmp1 );//BCD XX.XXXXX

    Tmp1 = MeterParameter.MeterIb*5;
    HEX4_BCD4( (unsigned char*)&MeasPara.Ib05, Tmp1 );//?????????0.05Ib
    
    Tmp1 = MeterParameter.MeterIb*30;
    HEX4_BCD4( (unsigned char*)&MeasPara.Ib30, Tmp1 );//?????????0.3Ib
    
    Tmp1 = MeterParameter.MeterIb*35*10;
    HEX4_BCD4( (unsigned char*)&MeasPara.Ib350, Tmp1 );//?????????3.5Ib
    
    Tmp1 = MeterParameter.MeterImax*12*10;
    HEX4_BCD4( (unsigned char*)&MeasPara.Imax120, Tmp1 );//?????1.2Imax
    
    MeasPara.DmdCp = MeterParameter.MeterCP;//????????
    
//    //Ir_Power_Cnt =0;//????????
//    Ir_Power_Time = 0;//?????
//    memcpy((unsigned char*)&Timer.KeyAble_Tmr,(unsigned char *)&SystermInitTab,3);//????????
}


void Init_Relay( void )
{	
	if( Relay_Mode != 0x55 )//���ü̵���
	{
		{RELAYOFF1; __NOP(); RELAYOFF2;}//�ű��̵ּ�����Ч
	}
	else
	{
		if(Mode.Feature1&0x01)//���巽ʽ
		{
			RELAYOFF3;
		}
		else
		{
			if(relay_status_flag==0)RELAYOFF3;//�̵����պ�
			else RELAYON3;//�̵����Ͽ�
		}
	}
}

void IntRTC( void )
{
	NVR_RTCINFO RTCData;
  RTC_AutCalType RTC_AutoCalInitStruct;
  uint32_t retval;
	
	retval = NVR_GetInfo_LoadRTCData(&RTCData);
  if (retval)
  {
    return;
  }
  
	RTC_AutoCalInitStruct.ADCSource = RTC_ADCS_DATA;
  RTC_AutoCalInitStruct.ATClockSource = RTC_ATCS_SEC;
  RTC_AutoCalInitStruct.ATDelay = RTC_ATDELAY_15MS;
  RTC_AutoCalInitStruct.Period = 30 - 1;
  RTC_AutoCalInit(&RTC_AutoCalInitStruct);
	
//	RTC_AutoCalCmd(ENABLE);
  
  while (RTC_GetINTStatus(RTC_INTSTS_ACDONE) == 0);
  RTC_ClearINTStatus(RTC_INTSTS_ACDONE);
	
}



void Init_System( void )
{	
	__disable_irq();
	FLAG_EA = 0;                         //�ر�ȫ�����ж�	
	BDMODE = 1;                     //���ر��
	IICPowerON = 0; 
	INIT_F251WDT();
	Clear_Wdt();
	
	Clock_Init();

	Init_Pad_Io();	
	Veri_Pad();
	Release_Rf();
	
	Veri_Sys_Reg();
	
//************������λ����**************//
	UART4->CTRL &= ~(BIT0|BIT1 );  
	GPIOB->OEN &= ~BIT0; GPIOB->IE &= ~BIT0;//���ö˿�Ϊ���ģʽ
	METER_RST0;                    //����оƬ��λ��
	NVIC_DisableIRQ(PMU_IRQn );
//************PLC��λ����******************//
	PLC_RST0;		                    //��λ�ز�(�����)
//************�ϵ�LCD��ʾ����**************//
	
	
	Lcd_Disp_Off();
	Display_Para_Verify();	        //��ʾ����У��
	Displayitem_Verify();	          //��ʾ��ĿУ��
	Display.Number=0xff;
	Display.Timer = Display_Para.A_Time;//�ϵ�ȫ��
	Init_Led_Timer = Display_Para.A_Time;//��������	
	Lcdled.B16 = Display_Para.A_Time;//��������
	Lcd_Display();		              //�ϵ�ȫ��
	Lcd_Disp_On();	                //��lcd������ʾ
	
	//	if( !(LVDSTATUS & B0000_0010) )//δ����Ƿѹ
//	{
		RELAYLEDON;		//��բ�ƺͱ�������һ��
		LCDLEDON;
//	}


  Note_Reset_Number();	          //��¼�ϵ縴λ��wdt��λ����
	Resetworking_Timer = resetworking_timer_delay;//������λ����	
	Mode_Verify();		              //����ģʽ��У��
	Init_Relay();		                //�ϵ�̵�������	
	Veri_Uart_All();	              //ͨѶ������У��
	Commaddr_Verify();	            //ͨѶ��ַУ��
	Demand_Para_Verify();		        //��������У��
	
	Read_Sysclock();	              //��ʱ��
	Fwdl_Verify();		              //�������У��
	Bkdl_Verify();		              //�������У��
	Powerd_Data_Process();	        //�������ݴ���
	Mt_Alarmstatus_Judge();	        //�����״̬�ж�
	Powerd_Hourwakeup(2);         	//Сʱ�����¼��ж�
	RTCIF = 0;
	Temperpara_Verify();          	//�¶�ϵ��У��	
	
	Clear_Wdt();
	Timezone_Judge();	              //ʱ��ʱ�η����ж�
	
	GetKeyStatus();		              //����ǰ��Կ״̬
	
	if( BDMODE )                    //����ģʽ
	{
		Year_Ladder_Judge();	        //����������жϺ����ݺϷ����ж�
		Price_Judge();		            //���ʽ��ݵ���ж�
	}
	
	Festival_holiday_Judge();                 //�ϵ����������ڼ����ж�
	 MeterParaRead(  );
////	if( !(LVDSTATUS & B0000_0010) )//δ����Ƿѹ
////	{
//		RELAYLEDON;		//��բ�ƺͱ�������һ��
//		LCDLEDON;
////	}
	Adcctrl.Timer=8;	              //��֤�ܿ�������ص�ѹ���¶�adc
	below_60u_flag = 1;
	Voltage_Low_Count = Deflowvoltagecount;
	
	*PMU_CONTROL =0x00000000;       //����оƬ��ʼ��ǰ�رն˿��ж�
	METER_RST1;		                  //����оƬʹ��(��֤�㹻��ʱ��Ŵ������оƬ)	  
	GPIOB->OEN |= BIT0; GPIOB->IE |= BIT0;//�ָ��˿�Ϊ����ģʽ
	
	PowerDetCnt2 = Def_Powerdetcnt2;//�ϵ�3s�ڵ��磬������Դ�쳣���
	
	//IntRTC();
	
  FLAG_EA = 0x5555AAAA;	
	__enable_irq(); 
	
	PowerLow = 0;                   //�ϵ�RTC�����־
	
	PLC_RST1_Timer=2;               //�ز���λ��ʱ2s
	
	Delay_Initial(150);	            //��ʱ750ms
	if( FLAG_EA == 0 ) Sleep();         	//����sleep

//	Clk_Adj_Proc();		//�����ⲿ��32K�����������ڲ�8M��ƫ��
//	Clk_Adj_Proc();		//�����ⲿ��32K�����������ڲ�8M��ƫ��
//	Clk_Adj_Proc();		//�����ⲿ��32K�����������ڲ�8M��ƫ��
	
	Init_Dayfreeze_Judge();	        //�ϵ��ն��Ჹ��
	Calc_Pwr_Proc();              	//����������ۼƴ���ͽ���
	
	
	Delay_Initial(20); 
	
	Meter_RN7302Para_Verify();
  RN8302Init();
	
	
	
	
	IICPowerON = 1;                 //���µ�EEPROM��������־��
//	*PMU_CONTROL =0x00000001; 	    //�򿪼���
//	NVIC_EnableIRQ(PMU_IRQn );
	
	Pav_Temp = 0;	
	Pav_Cnt= 0;
	Init_Demand();		//���������ʼ��
	
	Esam_Timer = 4;		              //�ϵ�4���λESAM(�ȴ���Դ�����ȶ�)
	Relay_Space_Timer = 2;	        //����2���ϵ���ʱ���Ƽ̵�����ʱ��
	PowerDetCnt0 = Def_PowerDetCnt0;//�ϵ�̵�������Դȥ������
	Read_Sysclock();	              //�ٴζ�ʱ��
	comm_relaystatus_flag = real_relaystatus_flag;//ˢ�¼̵���ʵ��״̬(ͨѶbit4ר��)
	Adcctrl.Timer = 3;              //��֤��������ADC��ص�ѹ���	 
  RFTESTCNT=0;
	KeySTS_OK_flag = 0; //��Կ���±�־����
	KEY_Status = 0;
	RTC_Status = 0;
}


float GetADCResult(void) 
{
	uint16 i,j,temp;
	for(i=0;i<15;i++) //BubbleADC
	{
		for(j=i+1;j<16;j++) 
		if(ADCResultArr.Buffer[i]>ADCResultArr.Buffer[j]) 
		{
			temp=ADCResultArr.Buffer[i];
			ADCResultArr.Buffer[i]=ADCResultArr.Buffer[j];
			ADCResultArr.Buffer[j]=temp;
		} 
	}

	temp = 0;
	for(i=1;i<15;i++)
	{
		temp +=	ADCResultArr.Buffer[i];
	}

	return (temp/14.0);
}

void Adc_Proc(void)		//��غ��¶�adc����
{
	unsigned short i;
//	signed char j; short Temp16;
// 	float T25ADC,AverADCf;
// 	float Fa,T,T25,Rtcadj,TopF_Rtc_Adj;
	float Fa,T;
 	union B16_B08 Temp;
// 	unsigned char Result;
//	signed char BufC[ee_temperature_lenth+2];
//	unsigned char tempCapH,tempCapL;
//  ADCInitType ADC_InitStruct;
  NVR_RTCINFO NVR_PxStruct;
	int32_t P2;
  int16_t P1, P0;
  int16_t adc_data;
  uint32_t retval;
	
	if ( Adcctrl.Status == B0000_0010 )//�¶�adc���ݴ���,�����¶Ȳ���
	{		
		Adcctrl.Status = 0;
	
		for(i=0; i<200; i++)//
		{
			if(ANA->INTSTS & BIT0)
			{
				break;
			}
		}
		if ( i == 200 ) return ;
		
		adc_data = ANA->ADCDATAA ;
		ANA->INTSTS = BIT0;		
		
	  retval = NVR_GetInfo_LoadRTCData(&NVR_PxStruct);
  
	  if (retval & 0x1U)
    {
      return ;
    }
    else
    {
      P0 = NVR_PxStruct.RTCTempP0;
      P1 = NVR_PxStruct.RTCTempP1;
      P2 = NVR_PxStruct.RTCTempP2;
    }	
		
	  T = (float)((((P0 * ((adc_data*adc_data)>>16)) + P1*adc_data + P2) >> 8) );
		Temp.B16 = T;		
		if( Temp.B16& 0x8000 )//���㵱ǰ�¶�
		{
			Temp.B16 = ~Temp.B16+1;
			Temp.B16 = Temp.B16*10/256;
			Hex2Bcd( Temp.B08, Fun_Para.Temper_M.B08, 2 );//ת����2�ֽ�bcd,��������idata����
 			Fun_Para.Temper_M.B08[1] |= B1000_0000;//���λ��1,��ʾ�����¶�			
		}
		else 
		{			
			Temp.B16 = Temp.B16*10/256;
			Hex2Bcd( Temp.B08, Fun_Para.Temper_M.B08, 2 );//ת����2�ֽ�bcd,��������idata����
		}
    ANA->ADCCTRL &= ~BIT31;
	  ANA->REG3 &=  ~BIT0;	
	}

	if ( Adcctrl.Status == B0000_0001 )//���adc����
	{	
		Adcctrl.Status = 0;
	
		for(i=0; i<1000; i++)//
		{
			if(ANA->INTSTS & BIT0)
			{
				break;
			}
		}
		if ( i == 200 ) return ;
		
		Fa = ANA->ADCDATA2 ;
		if( Fa >60000 ) Fa = 0 ;
		ANA->INTSTS = BIT0;
		Fun_Para.Voltage_B.B16 = Fa*0.015392+ 6.667986;

		Hex2Bcd( Fun_Para.Voltage_B.B08, Fun_Para.Voltage_B.B08, 2 );
		
		Temp.B16 = Threshold[4]<<4;	//ȡ���Ƿѹ�¼��������޶�ֵ
		
		if( Fun_Para.Voltage_B.B16 < Temp.B16 )	//��ص��ڷ�ֵ������ʱ60s�������Ƿѹ��¼
		{
			if( low_voltage_flag == 0 )
			{
				if( Bat_Delay_Timer < Bcd_To_Hex(Threshold[5]) ) //ȡ���Ƿѹ�¼��ж���ʱʱ�䶨ֵ
					Bat_Delay_Timer += 10;
				else
				{
					low_voltage_flag = 1; //��ص�ѹ��־
					Set_Reportdata(0*8+3);//��ʱ�ӵ��Ƿѹ�ϱ���־
					DltoEepromBuf( 6 );
					Event_Record( ee_batterlow_inpage );
				}
			}
		}
		else 
		{
			low_voltage_flag = 0;
			Bat_Delay_Timer = 0x00;
		}
		
		if( Fun_Para.Voltage_B.B16 < 0x80 ) Fun_Para.Voltage_B.B16 = 0; //��ѹ����0.8Vǿ�ƹ���
	 
		ANA->ADCCTRL &= ~BIT31;
	  ANA->REG3 &=  ~BIT0;	
	}
}
