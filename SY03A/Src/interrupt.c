#include "ex_var.h"
#include "ex_func.h"
//#include <intrins.h>
#include <string.h>
#include <stdio.h>

//δʹ���ж�Ԥ����ڷ�ֹ�����ת(����������ֲ�88ҳ)
//void int0_int( void ) interrupt 0 using 1
//{
//	IE0_= 0;
//	EX0 = 0;
//}
//void int1_int( void ) interrupt 2 using 1
//{
//	IE1_= 0;
//	EX1 = 0;
//}
//void timer1_int( void ) interrupt 3 using 1
//{
//	TF1 = 0;
//	ET1 = 0;
//}
//void timer2_int( void ) interrupt 5 using 1
//{
//	{TF2 = 0;EXF2 = 0;}
//	ET2 = 0;
//}
//void pca_int( void ) interrupt 6 using 1
//{
//	{CF = 0;CCF0 = 0;CCF1 = 0;CCF2 = 0;CCF3 = 0;CCF4 = 0;}
//	EC = 0;
//}
//void Eint3_int (void) interrupt 11 using 1
//{
//	AIF3 = 0;
//	AIE3 = 0;
//}
//void Eint4_int (void) interrupt 12 using 1
//{
//	AIF4 = 0;
//	AIE4 = 0;
//}
//void Eint5_int (void) interrupt 13 using 1
//{
//	AIF5 = 0;
//	AIE5 = 0;
//}
//void Eint6_int (void) interrupt 14 using 1
//{
//	AIF6 = 0;
//	AIE6 = 0;
//}

//void Trap_int (void) interrupt 15 using 2
//{
//}

////time16bit��ʱ,��ʱʱ��4096us,
//void timer0_int( void ) interrupt 1 using 1
//{
//	unsigned char Ch;
//	uint16 i;

//	TF0 = 0;			//Ӳ�������Զ�����
//	TR0 = 0;
//	TL0 = init_tl0;
//	TH0 = init_th0;
//	TR0 = 1;
//	if( VectorTimer == 0 ) VectorTimer  = ms_timer_delay;
//	if( VectorTimer == 1 ) run_readrtc_flag = 1;	//�����ʱ��
//	if( (VectorTimer == 50) || (((VectorTimer == 20)||(VectorTimer == 80))&&display_rtc_flag) ) run_display_flag = 1;	//����ˢ����ʾ
//	if( VectorTimer == 100 )
//	{
//		run_relaychk_flag = 1;	//����̵������
//		RELAYCTRLON;		//�򿪿��Ƶ�ƽ
//		Clear_SYS_Wdt();	//��ϵͳ���Ź�
//	}
//	VectorTimer--;
//	
//	DisKeySamp<<=1;
//	if(DISKEY_N)  DisKeySamp++;	//��ʾ����ȥ���ж�

//	CardKeySamp<<=1;
//	if(KEY_N)  CardKeySamp++;	//�忨ȥ���ж�
//	
//	//ADC����
//	if((Adcctrl.Status & B0000_0011)&&(ADCDATAH & B1000_0000))
//	{
//		ADCResultArr.Buffer[ADCResultArr.ReadCount&0x0f] = ((ADCDATAH&B0000_0111)*0x100+ADCDATAL);
//		ADCResultArr.ReadCount++;

//		//ת��32��ȡ����16���Ա�֤ת������׼ȷ��
//		if(ADCResultArr.ReadCount >= 32)
//		{
//			if ( Adcctrl.Status == B0000_0001 ) 
//			{
//				Adcctrl.Status = B0001_0000;//���adc����
//			}
//			else
//			{
//				Adcctrl.Status = B0010_0000;//�¶�adc����
//			}
//		}
//	}
//	
//	if ( Shiduanctrl.Timer != 0 ) //ʱ��Ͷ���������
//	{
//		Shiduanctrl.Timer--;
//	}
//	if( Shiduanctrl.Timer == 0 ) SHIDUANCTRL_OFF;
//		
//	if( Relay_Mode != 0x55 )//���ü̵���
//	{
//		if ( Relayctrl.Timer != 0 ) 
//		{
//			Relayctrl.Timer--;
//		}
//		if( Relayctrl.Timer == 0 ) {RELAYOFF1; _nop_(); RELAYOFF2;}//�ű��̵ּ�����Ч
//	}
//	else
//	{
//		if(Mode.Feature1&0x01)//���巽ʽ
//		{
//			if ( Relayctrl.Timer != 0 ) 
//			{
//				Relayctrl.Timer--;
//			}
//			if( relay_status_flag && (Relayctrl.Timer == 0) ) {RELAYOFF3;}//
//		}
//	}
//	
//	if ( Resetworking_Timer != 0 ) 
//	{
//		Resetworking_Timer--;
//	}
//	if( Resetworking_Timer == 0 )
//	{
//		PLC_RST1;
//	}
//	
//	if ( Zb_Relayctrl_Timer != 0 ) 
//	{
//		Zb_Relayctrl_Timer--;
//	}	
//	
//	Beep_Ctrl();//����������
//		
//	for ( Ch=0; Ch<3; Ch++ )
//	{
//		if(Uart[Ch].Overtime_Timer != 0)
//		{
//			Uart[Ch].Overtime_Timer--;
//			if( Uart[Ch].Overtime_Timer == 0 ) 
//			{
//					if( Ch==0 ) 
//					{	
//						UARTIE &= B1111_1110;	//�ر�TXIE0
//						UARTIE |= B0000_0010;	//��RXIE0
////						RXSTA0 &= B1111_0001;
//						{RXSTA0 = 0;RXSTA0 = B0101_0000;}//��λ����FIFO
//					}
//					if( Ch==1 ) 
//					{	
//						UARTIE &= B1111_1011;	//�ر�TXIE1
//						UARTIE |= B0000_1000;	//��RXIE1
////						RXSTA1 &= B1111_0001;
//						{RXSTA1 = 0;RXSTA1 = B0101_0000;}//��λ����FIFO
//						R485CTRLR;		//�������						
//					}
//					if( Ch==2 ) 
//					{	
//						UARTIE &= B1110_1111;	//�ر�TXIE2
//						UARTIE |= B0010_0000;	//��RXIE2
////						RXSTA2 &= B1111_0001;
//						{RXSTA2 = 0;RXSTA2 = B0101_0000;}//��λ����FIFO
//					}
//					Uart[Ch].Step = 0x00;
//					Uart[Ch].Flag = 0x00;
//					Uart[Ch].Response_Timer = 0x00;
//			}
//		}
//				
//	}
//	for ( Ch=0; Ch<3; Ch++ )
//	{
//		if(Uart[Ch].Response_Timer != 0)
//		{
//			Uart[Ch].Response_Timer--;
//			if( Uart[Ch].Response_Timer == 0 ) 
//			{
//				if ( Uart[Ch].Flag & B0000_1000 )
//				{	
//					
//					if( Ch==0 ) 			//�����жϷ�������
//					{	
//						TXREG0  = 0x68;		//����0x68
//						UARTIE &= B1111_1101;	//�ر�RXIE0
//						UARTIE |= B0000_0001;	//��TXIE0
//					}
//					if( Ch==1 ) 
//					{	
//						R485CTRLS;		//������
//						TXREG1  = 0x68;		//����0x68
//						UARTIE &= B1111_0111;	//�ر�RXIE1
//						UARTIE |= B0000_0100;	//��TXIE1
//					}
//					if( Ch==2 ) 
//					{	
//						TXREG2  = 0x68;		//����0x68
//						UARTIE &= B1101_1111;	//�ر�RXIE2
//						UARTIE |= B0001_0000;	//��TXIE2
//					}
//					Uart[Ch].Len  = 5;//��ʼ��������
//					Uart[Ch].Total_Len = Uart[Ch].Buf[9]+16;
//					Uart[Ch].Overtime_Timer = txtimeout_max;
//				}
//				else
//				{
//					Uart[Ch].Response_Timer++;//�����ȴ�
//				}
//			}
//		}			
//	}
//	
//	if( Compile_Polar_485 == 0x55 )	//�����޼���485
//	{
//		if( (Uart[1].Response_Timer == 0)&&(Uart[1].Overtime_Timer == 0)&&((UARTIF&B0000_1000)==0x00) )
//		{
//			Uart1KeySamp<<=1;
//			if(GPIO0&B0100_0000)  Uart1KeySamp++;		//��֤���߿���ʱ�ж�,�����޼���485
//			
//			if( Uart1KeySamp == 0xff ) //�ߵ�ƽ ������
//			{
//				if( RTXCON1 != B0000_0000 )	RTXCON1 = B0000_0000;//������ �������ݲ�ȡ��,�������ݲ�ȡ��
//			}
//			else if( Uart1KeySamp == 0x00 )
//			{
//				if( RTXCON1 != B0000_0011 )	RTXCON1 = B0000_0011;//������ ��������ȡ��,��������ȡ��		
//			}	 
//		} 
//		else Uart1KeySamp = 0x55;
//	}
//	else
//	{
//		if( RTXCON1 != B0000_0000 )	RTXCON1 = B0000_0000;//������ �������ݲ�ȡ��,�������ݲ�ȡ��
//	}
//	
//	if( (LVDSTATUS & B0000_0010) && (Relayctrl.Timer == 0) ) 
//	{	
//		for(i=0; i<0x300; i++)		//Լ3.75ms
//		{
//			if( (LVDSTATUS & B0000_0010) == 0 ) break;
//		}	
//		if( i==0x300 ) //����
//		{
//			LCDLEDOFF;//�رձ���(ʡ��)
//			RELAYLEDOFF;//�رռ̵���ָʾ��(ʡ��)
//			BEEPALARMOFF;//�رշ�����(ʡ��)
//			
//			for(i=0; i<0x300; i++)		//Լ3.75ms
//			{
//				if( (LVDSTATUS & B0000_0010) == 0 ) break;
//			}	
//			if( i==0x300 ) //����
//			{
//				EA = 0;		//�ص�ȫ���ж�
//			}
//		}
//	}			
//}

////lvd,xstp_int
//void nmi_int (void) interrupt 7 using 2
//{
//	if( (FDET&B0000_0001) ) //��Ƶ����ͣ���ж�
//	{
//		FDETFLAG = 0;		//��XTLFͣ���־
//	}

////	if(  )	//��ѹ����ж�
////	{
////	}
//	
//	if( (LVDSTATUS&B0000_0001) && (LVDEFCFG!=Sleeplvdefcfg) ) //LVIF=1, ����Ƿѹ�����ж�
//	{
//		LVDSTATUS = 0;	//������ "��ѹ�仯�жϱ�־λ"
//	}
//}  

//TI RIδ��.uart1,etmr1,etmr2
//void serial_int (void) interrupt 4 using 1
//{
//	unsigned char Temp_IF, Temp;
//		
//	//uart1
//	{
//		Temp_IF = UARTIE&UARTIF; //�κζԼĴ����Ĳ������ᵼ��������, �����Ȼ���

//		if( (Temp_IF&B0000_1000) ) //Receive
//		{
//			Temp = RXREG1; //��UART2���ݼĴ�����ȡ��������		
//	
//			Judge_645_Frame( 1, Temp );			
//			if ( Uart[1].Step == 0x08 )
//			{
//				UARTIE &= B0011_0011;	//�ر�RXIE1
//				UARTIF &= B1111_0111;	//clr UART1.rxif
//				display_zaibo_flag = 0;
//			}			
//		}

//		if( (Temp_IF&B0000_0100) ) //Transmit
//		{
//			Tx_645_Frame( 1 );
//			UARTIF &= B1111_1011;	//clr UART1.txif
//		}
//	}
//	
//	//etmr1
//	{	
//		Temp_IF = ( ET1IE & ET1IF );
//		if( Temp_IF&B0000_1000 ) //�Ƚ�ƥ���ж�
////		if( Temp_IF )
//		{
////			if((Emu_Data.I1.B32) && (energy_rst_flag == 0))
//			{
//				if( dl_back_flag == 0 )
//				{
//					Decimal.Fw[0]++;		//��������β����1					
//					if ( Decimal.Fw[0] >= Meter.Const_H )
//					{
//						Decimal.Fw[0] = 0;
//						Decimal.Unit_Pluse++;
//					}
//				}
//				else
//				{
//					Decimal.Bk[0]++;		//��������β����1
//					if ( Decimal.Bk[0] >= Meter.Const_H )
//					{
//						Decimal.Bk[0] = 0;
//						Decimal.Unit_Bkpluse++;
//					}
//				}
//			}
//			
//			ET1IF = 0x00;
//		}
//	}
//	
////	if(  ) //etmr2
////	{
////		Temp_IF = ( ET2IE & ET2IF );
////		if( Temp_IF )
////		{	
////			ET2IF = 0x00;
////		}
////	}
//}

////AIF0,rstflag,disp,uart0,tmpov,i2c
//void Eint0_int (void) interrupt 8 using 1
//{
//	unsigned char  Temp_IF, Temp;
//	AIF0 = 0;
//	
//	//uart0
//	{
//		Temp_IF = UARTIE&UARTIF; //�κζԼĴ����Ĳ������ᵼ��������, �����Ȼ���
//		if( (Temp_IF&B0000_0010)  ) //Receive
//		{
//			Temp = RXREG0; //��UART���ݼĴ�����ȡ��������
//			
//			Judge_645_Frame( 0, Temp );
//			if ( Uart[0].Step == 0x08 )
//			{
//				UARTIE &= B0011_1100;	//�ر�RXIE0
//				UARTIF &= B1111_1101;	//clr UART0.rxif
//				display_zaibo_flag = 0;
//			}
//		}
//		
//		if( (Temp_IF&B0000_0001) ) //Transmit
//		{
//			Tx_645_Frame( 0 );
//			UARTIF &= B1111_1110;	//clr UART0.txif
//		}
//	}
//	
//	if( RSTFLAG & B0000_0100 )	//���ʹ���ж�
//	{
//		RSTFLAG &= B1111_1011;	//����ʹ���жϱ�־
//	}
//	
////	if(   ) //disp
////	{
////	}
////	if(   ) //i2c
////	{
////	}
////	if(   ) //TMP_OV
////	{
////	}
//}

////AIF1.ioc,7816,uart2,etmr3
//void Eint1_int (void) interrupt 9 using 1
//{
//	unsigned char Temp_IF, Temp;
//	AIF1 = 0;

////	if( (BkpIF&B0000_0010) ) //etmr3
////	{
////	}

////	if( (BkpIF&B0000_1000) ) //u7816
////	{
////	}

//	if( (GPIO0IE&GPIO0IF)&B0100_0000 ) //ioc
//	{
//		if( (PCON&B0000_1100)==0 ) EA = 0;	//�ص�ȫ���ж�(sleep����)
//		else GPIO0IF = 0;
//	}
//	
//	if( (GPIO4IE&GPIO4IF)&B0110_0000 ) //ioc
//	{
//		if( (PCON&B0000_1100)==0 ) EA = 0;	//�ص�ȫ���ж�(sleep����)
//		else GPIO4IF=0;
//	}
//	else	//uart2
//	{
//		Temp_IF = UARTIE&UARTIF; //�κζԼĴ����Ĳ������ᵼ��������, �����Ȼ���
//	
//		if( (Temp_IF&B0010_0000) ) //Receive
//		{
//			Temp = RXREG2; //��UART2���ݼĴ�����ȡ��������

//			Judge_645_Frame( 2, Temp );
//			if ( Uart[2].Step == 0x08 )
//			{
//				UARTIE &= B0000_1111;	//�ر�RXIE2
//				UARTIF &= B1101_1111;	//clr UART2.rxif
//				display_zaibo_flag = 1;
//			}	
//		}
//	
//		if( (Temp_IF&B0001_0000) ) //Transmit
//		{
//			Tx_645_Frame( 2 );
//			UARTIF &= B1110_1111;	//clr UART2.txif
//		}
//	}
//}
//	
////AIF2.rtc,spi,etmr4
//void Eint2_int (void) interrupt 10 using 1
//{
//	AIF2 = 0;

//	if( RTCIE&B0000_0100 ) //rtc
//	{
//		if( (PCON&B0000_1100)==0 ) EA = 0;	//�ص�ȫ���ж�(sleep����)
//		else {RTCIF &= B1111_1110; RTCTMRIF = 0;}	//��������жϱ�־
//	}
//	else if( RTCIF&RTCIE ) //rtc
//	{		
//		if( Display.Timer != 0 ) Display.Timer--;
//		
//		if( Successled.Timer != 0 ) Successled.Timer--;//ͨѶ������ʱ��ʱ����1
//		
//		if( Keyclose_timer!= 0 ) Keyclose_timer--;//�����պϼ̵�����ʱ����1
//		
//		Adcctrl.Timer++;
//		if( Adcctrl.Timer == 5 )//�������adc 
//		{
//			if( ADCTRIMH  != const_adc_TrimBH ) ADCTRIMH = const_adc_TrimBH;//LSB=2.34mV
//			if( ADCTRIML  != const_adc_TrimBL ) ADCTRIML = const_adc_TrimBL;//У׼��׼Դ	
//			ADCCTRL = B0001_1110;
//			Adcctrl.Status= B0000_0001;
//		}

//		if( Adcctrl.Timer == 10 )//�����¶�adc 
//		{
//			Adcctrl.Timer = 0;
//			if( Lasttesting_Valid != 0x55 )//�ǳɲ�����
//			{
//				if( ADCTRIMH  != 0x06 ) ADCTRIMH = 0x06;//LSB=1.5mV
//				if( ADCTRIML  != 0x40 ) ADCTRIML = 0x40;//У׼��׼Դ
//			}
//			else
//			{
//				if( ADCTRIMH  != const_adc_TrimH ) ADCTRIMH = const_adc_TrimH;//LSB=1.5mV
//				if( ADCTRIML  != const_adc_TrimL ) ADCTRIML = const_adc_TrimL;//У׼��׼Դ
//			}			
//			ADCCTRL = B0001_1111;
//			Adcctrl.Status= B0000_0010;
//		}	
//		
////		if( Display_Space_Timer != 0 ) //����ң�ػ�����ʱ
////		{
////			Display_Space_Timer--;	
////			if( Display_Space_Timer==0 ) Lcdled.B16 = Hw_Lcdled_Timer;//�������ⶨʱ��
////		}
//		
//		if( Relay_Space_Timer != 0 ) //�ϵ���ʱ�����̵���
//		{
//			Relay_Space_Timer--;
//			if( Relay_Space_Timer==0 ){ relay_direct_flag = 1; run_alarm_flag = 1; }
//		}
//				
//		Lcdled_Control();	//�������
//			
//		RTCIF &= B0000_0100;	//���Сʱ�仯����ı�־
//	}

////	if(  ) //spi
////	{
////	}

////	if(  ) //ETMR4
////	{
////	}
//}


void Judge_645_Frame( uint08 CH, uint08 RcvData ) //using 1 //���մ���
{
	unsigned char i, n, m, status=0;
	
	Uart[CH].Overtime_Timer = txtimeout_max;//���ճ�ʱ
			
	switch( Uart[CH].Step )
	{
		case 0x00: //�ȴ�����0x68
				if( RcvData == 0x68 )
				{
					Uart[CH].Step = 0x01; //�ȴ�����id.
					Uart[CH].Len = 0;
				}
				else
				{
					status = 0xaa;	//��ʾ���մ���		
				}
				break;
			
		case 0x01: //�ȴ�����id.
				if ( (Uart[CH].Len) >= 6 )
				{
					Uart[CH].Step = 0x02; //�ȴ����յڶ���0x68
				}
				break;
	
		case 0x02: //�ȴ����յڶ���0x68
				if( RcvData == 0x68 )
				{
					if( CH == 0 )
					{
						Lcdled.B16 = Hw_Lcdled_Timer;//�������ⶨʱ��
						LCDLEDON;
					}
					
					Uart[CH].Flag = 0x00;//ͨѶ��־����
					
					for( i=0; i<6; i++ )
					{
						if( Uart[CH].Buf[ 1+i ] != 0x99 )  break;//��ʾ���ղ��ִ���
					}
					if( i == 6 ) 
					{
						Uart[CH].Flag = B0000_0001;//�㲥��ű�־
						Uart[CH].Step = 0x03; //�ȴ�����CMD
						break;
					}
					
					for( i=0; i<6; i++ )
					{
						if( Uart[CH].Buf[ 1+i ] != 0xaa )  break;//��ʾ���ղ��ִ���
					}	
					if( i == 6 ) 
					{
						Uart[CH].Flag = B0000_0010;//ȫaa��ű�־
						Uart[CH].Step = 0x03; //�ȴ�����CMD
						break;
					}
					
					for( i=0; i<6; i++ )
					{	
						if( (Uart[CH].Buf[ 1+i ] != Comm_Addr.Comm_Id[ 5-i ]) )
						{
							for( ; i<6; i++ )
							{
								if( Uart[CH].Buf[ 1+i ] != 0xaa ) break;//��ʾ���ղ��ִ���
							}
							if( i == 6 ) Uart[CH].Flag = B0000_0010;//����λ��ַ��־
							break;
						}
					}
					if( i == 6 )  
					{
						Uart[CH].Step = 0x03; //�ȴ�����CMD
						break;	
					}	
				}
				status = 0x55;
				break;
	
		case 0x03: //�ȴ�����cmd.
				Uart[CH].Step = 0x04; //�ȴ�����LEN
				break;
	
		case 0x04: //�ȴ�����len
				if( RcvData == 0x00 )
				{
					Uart[CH].Total_Len = RcvData;
					Uart[CH].Step = 0x06; //����cs
				}
				else 
				{
					if( RcvData <= (urx_buf_size-12) )
					{
						Uart[CH].Total_Len = RcvData;
						Uart[CH].Step = 0x05; //�ȴ�����DATA
					}
					else
					{
						status = 0xaa;	//��ʾ���մ���
					}
				}
				break;
	
		case 0x05: //�ȴ�����DATA
				if( Uart[CH].Len == ( Uart[CH].Total_Len + 0x09) ) //֡���������
				{
					Uart[CH].Step = 0x06; //�ȴ�����cs.								
				}
				
				if( Uart[CH].Len > ( Uart[CH].Total_Len + 0x09) ) //֡���������
				{
					status = 0xaa; //��ʾ���մ���									
				}
				
				break;
	
		case 0x06: //�ȴ�����cs.
				Uart[CH].Step = 0x07; //�ȴ�����0x16
				break;
	
		case 0x07: //�ȴ�����0x16
				if( RcvData == 0x16 )
				{
					Uart[CH].Step = 0x08;//�ֽڽ�����ȷ
					Uart[CH].Overtime_Timer = 0x00;//����ճ�ʱ
					Uart[CH].Response_Timer = framedelay_mx;//����Ӧ��ʱ
					Uart[CH].Flag |= B0000_0100;//����֡���ݽ�����ɱ�־
				}
				else
				{
					status = 0xaa;	//��ʾ���մ���
				}
				break;
	
		default: 
				status = 0xaa;	//��ʾ���մ���
				break;
		
	}

	if( status == 0x00 )	//�ֽڽ�����ȷ
	{
		Uart[CH].Buf[Uart[CH].Len] = RcvData;
		Uart[CH].Len++;
	}
	
	if( status == 0x55 )	//��Ŵ�����ߵڶ���68h����,��Ҫ������68h
	{
		status = 0xaa;//���մ���
		
//		if(CH)
		{
			Uart[CH].Buf[Uart[CH].Len] = RcvData;
			Uart[CH].Len++;
			for( n=1; n<Uart[CH].Len; n++ )
			{
				if( Uart[CH].Buf[n] == 0x68 )
				{
					
					i = n;
					for( m=0; m<(Uart[CH].Len-n); m++ )
					{
						Uart[CH].Buf[m] = Uart[CH].Buf[i];
						i++;
					}
					Uart[CH].Len -= n;
					Uart[CH].Step = 0x01;
					if ( Uart[CH].Len ==0x07 ) Uart[CH].Step = 0x02;
					status = 0x00;//���վ���ɹ�
					break;
					       
				}					
			}
		}
	}	
//	
//	if((*(&RXSTA0+CH))&B0000_1110)
//	{ 	   
//	   (*(&RXSTA0+CH))&=B1111_0001;
//	   status = 0xaa;
//	   if(CH==0)
//	   {
////		if ( Display_Space_Timer==0 ) 
////		{
////			for( i=0; i<20; i++ )//ȥ��150us
////			{
////				if( RXHW_P ) break;
////			}
////			if( i==20 ) Display_Space_Timer = 2;//����2sȥ��ʱ��	
////		}
//	   }
//	}
//		
	if( status == 0xaa )	//���մ���
	{
		Uart[CH].Step = 0x00;
		Uart[CH].Overtime_Timer = txtimeout_max/5;
	}
}



void Tx_645_Frame( uint08 CH )	//using 1	//���ݷ���
{
	if( Uart[CH].Len >= Uart[CH].Total_Len )
	{
		if( (CH>0) && (Uart[CH].Flag&B0001_0000) ) reg_veri_flag = 1; //ͨ���Ĵ���У�����bps�޸�
		Uart[CH].Overtime_Timer = 2;//ͨ����ʱ���Ƹ�λͨѶ
	}
	else
	{
//		if( Uart[CH].Len < 4 ) *(&TXREG0+CH) = 0xfe;
//		else *(&TXREG0+CH) = Uart[CH].Buf[Uart[CH].Len-4];
		Uart[CH].Len++;
		Uart[CH].Overtime_Timer = txtimeout_max;//���ͳ�ʱ
	}
}


void Beep_Ctrl()	//using 1	//����������
{
	if(!Beep_Timer) 
	{
		BEEPALARMOFF;
		return;
	}
	
	Beep_Timer--;
		
	if(!Ic_Err_Reg)
	{
		if( Beep_Timer == 155 )	
		{
			BEEPALARMON;		//��Ƭ�ɹ�,ֻ����һ��
		}
		
		if(!Beep_Timer)
		{
			BEEPALARMOFF;
		}
	}
	else
	{
		if( (Beep_Timer%31) ==0 ) 
		{
			if( (Beep_Timer/31)&0x01 )
			{	
				BEEPALARMON;		//5,3,1��
			}
			else
			{
				BEEPALARMOFF;		//4,2,0��
			}
		}
	}
}


void Lcdled_Control()	//using 1	//�������
{
	if( Lcdled.B16 != 0 ) Lcdled.B16--;
	
	if( Lcdled.B16 || Opendelaytimer.B32 || relay_delay_flag || relay_commu_alarm_flag || Bat_Delay_Timer || low_voltage_flag ||
		(dl_back_flag && ((Mode.Plc==0xa0)||(Mode.Plc==0xaa))) )
	{
		LCDLEDON;
	}
	else if( (BDMODE) && (remainder_alarm1_flag || remainder_alarm2_flag || remainder_zero_flag || charge_relay_open_flag) ) 
	{
		LCDLEDON;
	}
	else if( relay_status_flag && (relay_commu_disable_close_flag ==0) && relay_commu_open_flag )	//����ͨѶ��բ��������
	{
		LCDLEDON;
	}
	else if( (Relay_Mode!=0x55) && (relay_status_flag==0) && relay_error && (relay_commu_close2_flag==0) ) //������բʧ�ܵ�������(���ü̵���)(��������)
	{
		LCDLEDON;
	}
	else if( (Relay_Mode==0x55) && (relay_status_flag==0) && (real_relaystatus_flag == 1) && (relay_success_flag==0) )//������բ��ʱ���̵�������(���ü̵���)
	{
		LCDLEDON;
	}
	else
	{
		LCDLEDOFF;
	}
	
	
	if( Init_Led_Timer != 0 ) Init_Led_Timer--;	//ledָʾ�ƶ�ʱ����1
	
	if( Init_Led_Timer )
	{
		RELAYLEDON;
	}
	else if( relay_commu_disable_close_flag || (BDMODE&&(relay_commu_open_flag==0)&&(relay_status_flag==1)&&(first_eqzero_flag || first_alaram2_flag)) )      //1s��˸��բָʾ��
	{
		if( Sclock.Clockbackup[0] & B0000_0001 ) RELAYLEDON;
		else RELAYLEDOFF;	
	}
	else if( (Relay_Mode==0x55) && (relay_status_flag==0) && (real_relaystatus_flag == 1) && (relay_success_flag==0) )
	{
		if( Sclock.Clockbackup[0] & B0000_0001 ) RELAYLEDON;
		else RELAYLEDOFF;	
	}
	else if( real_relaystatus_flag )
	{
		RELAYLEDON;
	}
	else
	{
		RELAYLEDOFF;
	}		
}


unsigned char Judge_Io_Status()	//����Դ�쳣io��״̬
{	
	uint16 i;
	
	for( i=0; i<260/clkmode; i++ )	//2ms��ʱ�ж�
	{
		if( RELAYTEST_P ) break;
	}
	
//	if( LVDSTATUS & B0000_0010 )
	{	
		if( i == 260/clkmode )	return 0x55;	//�͵�ƽ����2ms����Ϊ��Դ�쳣(����Ƿѹ)
	}
	
	return 0;
}


void PowerError_Check()		//��Դ�쳣���
{
	uint16 i;
	unsigned char j;
	
	for(j=0;j<10;j++)
	{
		Clear_Wdt();
		
		RAMREG11 = 0x00;
		
		if( RELAYTEST_N )	
		{
			RAMREG11 = Judge_Io_Status();	
		}
			
		if( (RELAYTEST_P) && (RAMREG11 == 0x00) )
		{
			for( i=0; i<3600/clkmode; i++ )	//25ms��ʱ�ж�
			{
				if( RELAYTEST_N ) break;
			}
				
			if( i != 3600/clkmode )
			{					
				RAMREG11 = Judge_Io_Status();
			}
		}
		
		if( RAMREG11 == 0 ) break;
	}
}


//mode=1д����β�����¼���־,mode=0����β�����¼���־����
//ÿСʱ/����ʱ��/����/����ʱд����ee
void Wr_Powerdown_Data( unsigned char mode )	//д����������ݵ�����ee
{
	unsigned char  Buf[ee_powerdown_lenth+2];
	
	memset( Buf, 0x00, ee_powerdown_lenth );
	
	memcpy( Buf, Sclock.Clockbackup, 7 );//ʱ��

	if( mode )
	{
		memcpy( Buf+7, Decimal.Fw, 4 );//��������/��������β������������
		
		if( below_60u_flag==0 ) Buf[18] |= 0x80;//�����¼���ʼ��¼
		if(relay_error ) Buf[18] |= 0x40;//���ɿ����쳣������¼
		if( load_over_flag )
		{
			Buf[18] |= 0x20;//��¼���������¼�(����)
		}
	}
	
	memcpy( Buf+23, Load.Validpoint[0].B08, ee_loadpoint_lenth+2 );//���渺�ɼ�¼ָ��(��crc)
	
	I2C_Write_Eeprom( ee_power_down_page, 0x00, Buf, ee_powerdown_lenth);//д�������ݵ�����ee
}

//���ۼ�ʱ�䴦��
//���缰����ʱ���������ۼ�ʱ�䲢���µ�EE�������RAM�������㣬�����ۼ�
//Store=0��ֻ����Store=1���洢
void Save_Keep_Timer( unsigned char Store )
{
	unsigned char i;
	
	if( Store && (Crc_Check( Keeptime.Times[0].B08, ee_illegalcardnum_len )==0) ) return;
	
	I2C_Read_Eeprom( ee_illegalcardnum_page, ee_illegalcardnum_inpage, Eeprom.Buffer, ee_illegalcardnum_len );
	
	
	for( i=0; i<ee_illegalcardnum_len/4; i++ ) 
//		(unsigned long)(*(Eeprom.Buffer+i*4)) += Keeptime.Times[i].B32;
	*(unsigned long *)(Eeprom.Buffer+i*4) += Keeptime.Times[i].B32;
	
	if( Store)
	{
		I2C_Write_Eeprom( ee_illegalcardnum_page, ee_illegalcardnum_inpage, Eeprom.Buffer, ee_illegalcardnum_len );
		memset( Keeptime.Times[0].B08, 0x00, ee_illegalcardnum_len );
	}
}

//ͨѶ��������ۼ�ʱ��
void Get_Keep_Timer(unsigned char Ch, unsigned char *AckBuf)
{
	Save_Keep_Timer(0);
	
	Hex2Bcd( Eeprom.Buffer+Ch*4, AckBuf, 4 );
	Inverse(AckBuf+1, 3);
	AckBuf[0] = 3;
}

void Store_Data()
{
//	GPIO27CFG = 0;//�ر�����(ʡ��)
	LCDLEDOFF;//�رձ���(ʡ��)
	RELAYLEDOFF;//�رռ̵���ָʾ��(ʡ��)
	BEEPALARMOFF;//�رշ�����(ʡ��)
	if( BDMODE ) //����ģʽ
	{
		if( Decimal.Status&B1100_0000 ) I2C_Write_Eeprom( ee_remainmoney_page, ee_remainmoney_inpage, Money.Remain, ee_remainmoney_lenth+2 );
	}

	if( Decimal.Status&B0100_0000 ) I2C_Write_Eeprom( ee_fwdl_page, ee_fwdl_inpage, Fwdl.Fwdl_T.B08, ee_fwdl_lenth+2 );
	if( Decimal.Status&B1000_0000 ) I2C_Write_Eeprom( ee_bkdl_page, ee_bkdl_inpage, Bkdl.Bkdl_T.B08, ee_bkdl_lenth+2 );
	
	Read_Sysclock();//��ʱ��
	Wr_Powerdown_Data(1);//д����������ݵ�����ee
	
	real_relaystatus_flag = (relay_status_flag^relay_error);
	System_St_Bk = (System_Status&0x3f);//����״̬��
	if( Cover.Status ) System_St_Bk |= 0x80;//���濪��״̬
	I2C_Write_Eeprom( ee_reportdata_page, 0, &System_St_Bk, ee_reportdata_lenth);//�����¼��ϱ����ݷ�ֹ����
	
	Save_Keep_Timer(1);//��������ۼ�ʱ��
	
	RAMREG0 = 0x00;//��ʾδСʱ����
	RAMREG1 = Bcd_To_Hex(Sclock.Clockbackup[1]);//ͣ��ʱ�ķ�����
	RAMREG2 = 1;//����1������
	RAMREG3 = 0;//�建��������־�ֽ�
	RAMREG4 = 0;//��������־
	
	RAMREG5 = 0;//����������
	RAMREG6 = 0;//������մ���
	RAMREG7 = 0;//�������һ�����մ���
	RAMREG8 = 0;//�½�����һ�����մ���
	
	RAMREG9 = 0;//����״̬��
	Pd_Freeze_Judge(0);//ͣ�綳�ᴦ��
	
	RAMREG11 = 0;//��Դ�쳣״̬
	RAMREG12 = Sleepdispdays;//ͣ��7���й�����ܵ���
	
	memset( Emu_Data.U.B08, 0x00, 28 );//��ѹ�������ʹ�����������
	Emu_Data.Cosa.B32 = 0x1000;
	System_Status1 = 0;//����״̬��1
	
	Cal_Comb( 0x00, 0xFF, 0x01, Ic_Comm_TxBuf );//������1��������ϵ���(������������Ҫ��)
	Cal_Comb( 0x00, 0xFF, 0x02, Ic_Comm_TxBuf+20 );//������2��������ϵ���(������������Ҫ��)
}

void Lcd_Disp_Off()//����lcd������ʾ
{			 	
//	//δѡ��LCDģʽʱ��LCD��·�ضϲ����ף��ж���2.4uA����
//	//��ѡ�л�ض�LCDʱ������MD����Ϊ00�⣬ENMODEҲ��Ҫ����Ϊ00
//	LCDSET 	 = Sleeplcdset;//��ʾ��ʽ
//	LCDTEST   = B0000_0000;
//	DISPFREQ  = 0x3F;//���ˢ��Ƶ��
//	LCDBIAS  = 0x00;//���µ�ѹƫ��
//	ENMODE 	  = 0x0F;//�����ʾģʽ
////	DISPCTRL = B0000_0000;	//�ر�lcd��ʾ
// 	memset( (unsigned char xdata *)(0x2093), 0x00, 42 );	//ˢ����ʾ
// 	LCDTranslate64();
 }

void Lcd_Disp_On()//����lcdͣ����ʾ
{
//	LCDSET 	 = Sleeplcdset;//��ʾ��ʽ
//	DISPFREQ  = Sleeplcdfreq;//ˢ��Ƶ��
//	LCDBIAS  = Sleeplcdbias;
//	ENMODE 	  = Sleepenmode;
//	DISPCTRL = B0100_0000;		//��lcd��ʾ
}


void Configure_Register_Sleep()//sleepǰ���üĴ���
{
//	if(sleeptmr0ie == 0x20)//0x20��tmr0ͣ�绽��,0x00�ر�tmr0ͣ�绽��
//	GPIO06CFG = B0100_0000;//RX485, RXD1
//	else
//	GPIO06CFG = B0010_0000;//RX485, RXD1
//	GPIO07CFG = 0;//TX485, TXD1
//	GPIO12CFG = 0;//METERIC_RX RXD3
//	GPIO13CFG = 0;//METERIC_TX TXD3
//	GPIO14CFG = B0100_0000;//RXHW, RXD0
//	GPIO15CFG = 0;//TXHW, TXD0
//	GPIO16CFG = B0100_0010;//SCL
//	GPIO17CFG = B0110_0010;//SDA
////	GPIO17CFG = B0000_0010;//SDA
//	GPIO26CFG = 0;//ACTIVEP
//	GPIO27CFG = 0;//�������EXOUT1  �в�ϣ�����ֵ�����
//	GPIO32CFG = B0100_0000;//PLCA
//	GPIO33CFG = B0100_0000;//PLCRST	
//	GPIO34CFG = B0100_0000;//MADE_CHK	
//	GPIO35CFG = B0100_0000;//PLCEVT
//	GPIO36CFG = B0100_0000;//RX3,	RXD2
//	GPIO37CFG = B0100_0000;//TX3,	TXD2
//	GPIO40CFG = 0;//MCUCLK,U7816��ʱ�����
//	GPIO41CFG = B0100_0000;//SAMIO,	ESAM��IO��
//	GPIO42CFG = 0;//BEEP(����������)
//	GPIO45CFG = B0010_0000;//DISPKEY(Cmp1)
//	GPIO46CFG = B0010_0000;//TAIL
//	GPIO55CFG = 0;//MRST
//	GPIO57CFG = 0;//BEEP
//	GPIO60CFG = 0;//RelayCtrl
//	GPIO61CFG = 0;//RelayTest
//	GPIO62CFG = 0;//485Ctrl
//	GPIO63CFG = 0;//LEDRelay	
//	GPIO65CFG = 0;//TM���
//	GPIO70CFG = 0;//EsamVcc
//	GPIO74CFG = 0;//RFMO
//	GPIO75CFG = 0;//RFMI
//	GPIO76CFG = 0;//RFCLK
//	GPIO77CFG = 0;//RFCS
//	GPIO80CFG = 0;//RelayOff,�̵������ƽ� 
//	GPIO81CFG = 0;//RelayOn, �̵������ƽ�
//	GPIO82CFG = 0;//LCDLED 
//	GPIO83CFG = 0;//SAMRST
//	GPIO84CFG = 0;//RFKEY,������
//	GPIO85CFG = 0;//RFRST  

//	EXIN1DFCTL = 0;
//	EXIN2DFCTL = 0;
//	
//	GPIO0 &= B1011_1111;
//	GPIO1 &= B1110_1111;
//	GPIO2 &= B1111_1111;
//	GPIO3 &= B0000_0011;
//	GPIO4 &= B1111_1001;
//	GPIO8 &= B1111_1111;
	
//	ADCCTRL   = 0; //��ʹ��ADC
//	PLLEN	  = 0; //�ر����໷

//	RXSTA0 = 0;
//	RXSTA1 = 0;
//	RXSTA2 = 0;
//	TXSTA0 = 0;
//	TXSTA1 = 0;
//	TXSTA2 = 0;
//	TMLSREG = 0;
//	U7816CTRL0 = 0;
//	U7816CTRL1 = 0;	

	Lcd_Disp_On();//����lcdͣ����ʾ
	Stop_Disptimer = 0;//ͣ����ʾʱ������
	Display.Number = 0;
	Display.Status = 0;//�尴����ʾ��ر�־
	Ic_Type_Reg = 0;//��忨��ʾ��ر�־  
	
	Lcd_Display();

//	if( (PowerDetCnt1 < 4)&&(PowerDetCnt2 < 8) )//������е�Դ�쳣���
//	{
//		PowerDetCnt0 = 3;
//		RTCIE = B0000_0101+sleeptmr0ie;	//Сʱ���Ѻ��뻽��,tmr0����(����)
//	}
//	else
//	{
//		PowerDetCnt0 = 0;
//		RTCIE = B0000_0100+sleeptmr0ie;	//Сʱ����,tmr0����(����)
//	}

//	GPIO4IE = B0110_0000;		//gpio4.5,4.6�ж�ʹ��	�������ѻ���
//	GPIO4IF = 0;			//���־λ
//	GPIO0IE = B0100_0000;		//gpio06 = rx485
//	GPIO0IF = 0;
//	
//	UARTIE 	= B0000_0000;		//�ر�uart�ж�
//	IE  	= B0000_0000;
//	AIE  	= B0000_0110;		//ʹ��1/2����оƬ��������ռ�ã�
}


void Calculate_Min(unsigned char Result)//ͣ����Ӽ��㴦��
{
	unsigned char i;
	if( Result == 0 ) return;//�Ƿ�����
	if( Result > 60 ) return;//�Ƿ�����
	i = Result;//i��Χ0~60min
//	if( Progkey.Timer >= i )//�����ʱ��60
//	{
//		Progkey.Timer -= i;
//	}
//	else	
//	{
//		Progkey.Timer = 0;
//	}

	if(low_voltage_flag) 
	Keeptime.Times[2].B32 += i;//�ۼƵ��Ƿѹʱ��
	Keeptime.Times[3].B32 += i;//�ۼƵ����ۼ�ʱ��
	Keeptime.Times[4].B32 += i;	//�ۼƵ�ع���ʱ��	

	if( (!BDMODE) && (SELFCLOSE == 0x55) )
	{
		if( Self_Timer24 >= i )//�Ա�����ʱ
		{
			Self_Timer24 -= i;
		}
		else	
		{
			Self_Timer24 = 0;
		}
	}
}


//Status==0ͣ�����;Status==1���ѵ���,Status==2��ʼ������
void Powerd_Hourwakeup( unsigned char Status )	//Сʱ�����¼��ж�
{
//	unsigned char i;
//	 
//	if( Status < 2 )
//	{
//		if ( RTCIF & B0000_0100 )//1Сʱ����	
//		{			
//			RTCIF &= B1111_1011;	//��Сʱ�仯��־
//			
//			if( Status == 1 )
//			{
//				if( RAMREG0 == 0x55 )
//				{
//					Calculate_Min(60);//��Сʱ����
//				}
//				else
//				{
//					Calculate_Min((60-RAMREG1));//��һ��Сʱ����
//					RAMREG0 = 0x55;
//				}
//				
//				//ͣ���¶Ȳ���
//				ADCResultArr.ReadCount = 0;
//				
//				if( Lasttesting_Valid != 0x55 )//�ǳɲ�����
//				{
//					if( ADCTRIMH  != 0x06 ) ADCTRIMH = 0x06;//LSB=1.5mV
//					if( ADCTRIML  != 0x40 ) ADCTRIML = 0x40;//У׼��׼Դ	
//				}
//				else
//				{
//					if( ADCTRIMH  != const_adc_TrimH ) ADCTRIMH = const_adc_TrimH;//LSB=1.5mV
//					if( ADCTRIML  != const_adc_TrimL ) ADCTRIML = const_adc_TrimL;//У׼��׼Դ		
//				}
//				ADCCTRL = B0001_1111;//���¶�adc
//				I2c_Wait_5ms();
//				
//				//ת��18��ȡ����16���Ա�֤ת������׼ȷ��
//				for( i=0; i<18; i++ )
//				{
//					I2c_Wait_5ms();
//					ADCResultArr.Buffer[ADCResultArr.ReadCount&0x0f] = ((ADCDATAH&B0000_0111)*0x100+ADCDATAL);
//					ADCResultArr.ReadCount++;
//				}
//				Adcctrl.Status = B0010_0000;//�¶�adc����
//				Adc_Proc();
//				ADCCTRL   = 0; //��ʹ��ADC
//			}					
//			
//			Read_Sysclock();//��ʱ��
//			RAMREG4 = 0x55;	//�ÿ�������־	
//			Histroy_Freeze_Dl(0);//����������ᴦ��
//			Pd_Freeze_Judge(1);//ͣ�綳�ᴦ��
//			
//			if(RAMREG12) RAMREG12--;//ͣ����ʾʱ���1
//			if(RAMREG12 == 0)
//			{
//				if(Stop_Disptimer == 0) 
//				Lcd_Disp_Off();	//�ر�lcd��ʾ
//			}
//		}
//		
//		if ( RTCIF & B0000_1000 )//1�컽��
//		{
//			Clr_Zero_Data();//�������������
//		}
//		
//		RTCIF &= B0000_0100;	//���Сʱ�仯����ı�־
//	}
//	else
//	{
//		if ( RTCIF & B0000_0100 )//1Сʱ����	
//		{
//			Freeze_Judge();		//����ͽ����ж�
//		}
//	}
}

	
void Reset()	//���Ѹ�λ
{	
	Read_Sysclock();//��ʱ��
	
	if( RAMREG0 == 0x55 )
	{
		Calculate_Min(Bcd_To_Hex(Sclock.Clockbackup[1]));//��Сʱ����
	}
	else
	{
		Calculate_Min((Bcd_To_Hex(Sclock.Clockbackup[1])-RAMREG1));//δСʱ����
	}
	
	if( RAMREG4 == 0x55 ) Pd_Histroyfreeze_Judge();	//ͣ������ղ����ж�

	Save_Keep_Timer(1);
	
	Pd_Freeze_Judge(2);//ͣ���ն��ᴦ��
	
	if( RAMREG11 == 0xaa )//���ǵ�Դ�쳣��¼
	{
		memcpy( Ic_Comm_RxBuf+106, Sclock.Clockbackup, 6 );//���浱ǰʱ��
		memcpy( Sclock.Clockbackup, Ic_Comm_RxBuf+100, 6 );//��Դ�쳣����ʱ��			
		Note_Run_Event(ee_powererrn_inpage);		//��Դ�쳣��ʼ��¼
		memcpy( Sclock.Clockbackup, Ic_Comm_RxBuf+106, 6 );//�ָ���ǰʱ��
		Note_Run_Event(ee_powererrn_inpage|0x80);	//��Դ�쳣������¼
	}
	
	System_St_Bk = System_Status; //����ϵͳ״̬��	
	RAMREG3 = 0xaa;

	(*( void (*)( ) )0xFF0000) ();	//��λ,��ͷ��ʼ����
}


void Sleep( void )
{
//	unsigned char CoverDetCnt = 0;
//	uint16 i;

	//�ڽ���sleepǰ�ٴν���ȥ��
//	for(i=0; i<0x300; i++)		//Լ3.75ms
//	{
//		if( (LVDSTATUS & B0000_0010) == 0 ) { reg_veri_flag = 1; EA = 1; return; }	//����������
//	}	

	Store_Data();
	Configure_Register_Sleep();
	Powerd_Hourwakeup(0);		//Сʱ�����¼��ж�
//	
//	for( ; ; )
//	{		
////		Clear_Wdt();
////		Clear_SYS_Wdt();
////		LVDSTATUS = 0;
////		
////		if( (LVDSTATUS & B0000_0010) == 0 )	//sleepǰ�ٴ�ȥ��
//// 		{ 			
////	 		for(i=0; i<200; i++)
////			{
////				if( (LVDSTATUS & B0000_0010) != 0 ) break;
////			}	
//			if( i==200 ) 
//			{
//				Reset();
//			}
// 		}
//		
////		PDRCFG=0x00;	//�µ縴λ��Ҫ��~~
////		
////		CLKSEL= B0010_0011;	//�л���arclk(v03��оƬ)
////		{_nop_();_nop_();}
////		OSCEN = sleeposcen;	//�ر�rchf��rclf(v03��оƬ)
////		
////		Clear_Wdt();
////				
////		if( sleeptmr0ie == 0x20 )//0x20��tmr0ͣ�绽��,0x00�ر�tmr0ͣ�绽��
////		{
////			TMRCTL = B0000_0010;	//ֹͣrtctmr0,��ͬ��
////			TMR0PREDIV = 0;		//rtctmr0Ԥ��Ƶ�Ĵ�������Ϊ0��ʾ1��Ƶ
////			TMR0PRESET = sleepwakeuptimer;	//����Ԥ�üĴ���(�����),16-6,32-12,65-25
////			RTCTMR0OVC = 0;		//�������ֵ�Ĵ���
////			RTCTMRLOAD = B0000_0001;//load rtctmr0
////			TMRCTL = B0010_0010;	//����rtctmr0,��ͬ��
////			RTCTMRIF = 0;		//��rtctmr0
////		}
//		
////		PCON |= B0000_1100;	//GF0,GF1��λ
////		GPIO4IF = 0;
////		GPIO0IF = 0;
////		RTCIF &= B1111_1110;	//��������жϱ�־

////		{LVDEFCFG = 0; LVDEFCFG = Sleeplvdefcfg;}//��ѹ����·��Ъʹ��
////		
////		if( LVDSTATUS & B0000_0010 )//����Ƿѹ
////		{
////			EA = 1;			//��ȫ���ж�
////			PCON = B0000_0001;	//����sleepģʽ
////			EA = 0;			//�ص�ȫ���ж�
////		}
////		else  PCON &= B1111_0011;	//GF0,GF1����
////		
////		Clear_Wdt();
////		{LVDEFCFG = 0; LVDEFCFG = 0x80;}//��ѹ����·ʹ��
////		
////		OSCEN = runoscen; 		//��rchf
////		{_nop_();_nop_();}
////		CLKSEL= B0010_0000;		//�л���rchf		
////		Clear_Wdt();
////		Clear_SYS_Wdt();
////		
////		if( (sleeptmr0ie == 0x20) && RTCTMRIF ) GPIO06CFG = B0010_0000;//RX485, RXD1(��չ��ƽ��������)
////		if( GPIO0 & B0100_0000 ) RTCIE |= B0000_0001; 	//GPIO06�ߵ�ƽ���������뻽�Ѽ���ѹ

//		if( GPIO4IF & B0110_0000 )//��������
//		{
//			if( GPIO4IF&B0100_0000 )//����
//			{
////				if( RAMREG2 )//���Ǵ�������
//				{
//					CoverDetCnt = 1;
//					Cover_Space_Timer = Bcd_To_Hex(Threshold[3]); //ȡ������¼��ж���ʱʱ�䶨ֵ
//					RTCIE |= B0000_0001;  //�����ж� 
//				}						
//			}
//			if( GPIO4IF&B0010_0000 )//��ʾ����
//			{
//				for(i=0; i<0xff; i++)
//				{
//					if( GPIO4&B0010_0000 ) break;//��������Ϊ�͵�ƽ
//				}
//				if( i==0xff ) //����ȥ��(0.3ms)
//				{
//					Displaykey_Process();//��ʾ��������
//					if( Stop_Disptimer==0 ) 
//					{
//						Display.Timer = Display_Para.Loop_Time;//��1��ѭ����ʾ,��2�ΰ���ͣ��30s
//						Display.Status &= B1111_1110;//�尴����ʾ��־
//						Stop_Disptimer = Get_Valid_Num(0)*Display.Timer;//ѭ����ʾһ��
//						
//						if( Stop_Disptimer==0 ) Stop_Disptimer = Display.Timer;//��ֹѭ����ʾ��Ŀ��Ϊ0
//					}
//					else
//					{
//						Stop_Disptimer = default_keydisplaytimer/2;//30s
//					}
//					Lcd_Display();			//ˢ����ʾ
//					Lcd_Disp_On();
////					RTCIE   |= B0000_0001;		//��ÿ�뻽��һ��
//				}
//			}
//		}
//		
////		if( RTCIF & B0000_0011 )		//��ʱ����
////		{
////			if ( (RTCIE & RTCIF) & B0000_0001 )//1s����
////			{					
////				if( PowerDetCnt0 >= 3 )
////				{
////					PowerDetCnt0++;
////					if( PowerDetCnt0 >= 5 )
////					{
////						GPIO60CFG = B0100_0000;//RelayCtrl
////						GPIO61CFG = B0010_0000;//RelayTest
////						I2c_Wait_5ms();
////						PowerError_Check();//��Դ�쳣���(��բ���ж�)
////						if( RAMREG11 != 0x55 ) PowerDetCnt0 = 0;
////						else
//////						if( PowerDetCnt0 >= 8 )
////						{
////							PowerDetCnt0 = 0;
////							RAMREG11 = 0xaa;//ֻ�м�¼����ʱ�����ϵ��������ʱ��
////							memcpy( Ic_Comm_RxBuf+100, Sclock.Clockbackup, 6 );//�����Դ�쳣����ʱ��(���ʱ������)
////						}
////						
////						if(PowerDetCnt0 == 0)
////						{
////							GPIO60CFG = 0;
////							GPIO61CFG = 0;
////						}
////					}
////					else
////					{
////						Read_Sysclock();//��ʱ��
////					}
////				}
////				
//				if( CoverDetCnt )
//				{
//					if( Cover_Space_Timer )
//					{
//						Cover_Space_Timer--;
//						if( Cover_Space_Timer == 0 ) Read_Sysclock();//��ʱ��
//					}
//                       				 
//					Cover_Check();//���Ǽ��
//					if( Cover_Space_Timer == Bcd_To_Hex(Threshold[3]) )  //δ��⵽���ǻ�������¼��� 
//					{
//						CoverDetCnt = 0;
//					}			
////					if( RAMREG2 == 0 ) GPIO4IE &= B1011_1111;
//				}
//				
//				if( Stop_Disptimer ) 
//				{
//					Stop_Disptimer--;
//					if( Stop_Disptimer ) 
//					{
//						if( Display.Timer ) Display.Timer--;
//					
//						if( Display.Timer==0 ) display_rtc_flag = 1;
//						if( display_rtc_flag )
//						{
//							Lcd_Display();	//ˢ����ʾ
//						}
//					}	
//				}
//				
//				if( Stop_Disptimer == 0 ) 
//				{
//					Display.Number = 0 ;	//��ͷ��ʼ��ʾ
//					
//					if( RAMREG12==0 ) Lcd_Disp_Off();//����lcd������ʾ
//					else 
//					{
//						Display.Status &= B1111_1110;//�尴����ʾ��־
//						Lcd_Display();//ͣ�糤��7�����
//					}
//					
////					if(( PowerDetCnt0 < 3 ) && (CoverDetCnt == 0) &&(!(GPIO0&B0100_0000))) RTCIE &= B1111_1110;//�ر�ÿ�뻽��
//				}
//			}
//		}
//		
//		Powerd_Hourwakeup(1);	//Сʱ�����¼��ж�	   
//	}
}
