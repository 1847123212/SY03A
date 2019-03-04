/**
* @file    v85xx_isr.c
* @author  weaktea
* @version V1.0
* @date    2016/07/05
* @brief   Main Interrupt Service Routines.
******************************************************************************/
#include "ex_var.h"
#include "v85xx_isr.h"
#include "main.h"
#include "ex_func.h"
//#include <intrins.h>
#include <string.h>
#include <stdio.h>

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
  while(1)
  {}
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  while(1)
  {}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}

/**
  * @brief  This function handles PMU interrupt request.
  * @param  None
  * @retval None
  */
void PMU_IRQHandler(void)
{
	
	if((((*PMU_IOAINT)&BIT10 )||((*PMU_IOAINT)&BIT11 ) )&&(FLAG_EA==0x5555AAAA))  
	{		
//		PLUSELED_ON;		
//		TMR1->RELOAD   = 0x0002AAAA ;	
//    TMR1->CTRL   = 0x00000009;
//		NVIC_EnableIRQ(TMR1_IRQn);
//    NVIC_SetPriority(TMR1_IRQn, 1);
//		*PMU_IOAINT =PMU_IOAINT_IOAINT;
//	  
//		if( dl_back_flag == 0 )
//		{
//					Decimal.Fw[0]++;		//��������β����1					
//					if ( Decimal.Fw[0] >= Meter.Const_H )
//					{
//						Decimal.Fw[0] = 0;
//						Decimal.Unit_Pluse++;
//					}
//		}
//		else
//		{
//					Decimal.Bk[0]++;		//��������β����1
//					if ( Decimal.Bk[0] >= Meter.Const_H )
//					{
//						Decimal.Bk[0] = 0;
//						Decimal.Unit_Bkpluse++;
//					}
//		}
//		
//		
//		Demand[0]++;//������������������(����ʱ����)��1
//		Tar_Demand[0]++;//��������������������(����ʱ����)��1
//      PowerRead_Flag = 0x5A;
		
	}
	else if ( FLAG_EA==0 )
	{
		 if ((*PMU_IOAINT)&BIT12 ) 
		 {
			 DisKeySamp1 = 0xff;
			 KEY_Status = 1; 
		 }
		 
	   if ((*PMU_IOAINT)&BIT13 )
		 {
			 DisKeySamp = 0xff;
			 KEY_Status = 1; 
		 }
//		 if ((*PMU_IOAINT)&BIT15 ) KEY_Status |= B0010_0000; 
//		Displaykey_Process();//��ʾ��������
//		if( Stop_Disptimer==0 ) 
//		{
//		  Display.Timer = Display_Para.Loop_Time;//��1��ѭ����ʾ,��2�ΰ���ͣ��30s
//			Display.Status &= B1111_1110;//�尴����ʾ��־
//			Stop_Disptimer = Get_Valid_Num(0)*Display.Timer;//ѭ����ʾһ��
//						
//			if( Stop_Disptimer==0 ) Stop_Disptimer = Display.Timer;//��ֹѭ����ʾ��Ŀ��Ϊ0
//		}
//		else
//		{
//			Stop_Disptimer = default_keydisplaytimer/2;//30s
//		}
//		Lcd_Display();			//ˢ����ʾ
//    Lcd_Disp_On();
	
	}
		
	
  PMU->IOAINT = 0x0000FFFF;
}

/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{	
  
	if((RTC->INTSTS&RTC_INTSTS_WKUSEC )&&(PowerLow==0))  //rtc
	{		
		RTC->INTSTS = RTC_INTSTS_WKUSEC;
		
		read_back_flag = 1;//ÿ���һ��7051��־
		
		if( Display.Timer != 0 ) Display.Timer--;
		
		if( Successled.Timer != 0 ) Successled.Timer--;//ͨѶ������ʱ��ʱ����1
		
		if( Keyclose_timer!= 0 ) Keyclose_timer--;//�����պϼ̵�����ʱ����1
		
		Adcctrl.Timer++;
		if( Adcctrl.Timer == 5 )//�������adc 
  	{
      ANA->REG1	= 0x10 ;
			ANA->INTSTS= 0x00000003;
			ANA->ADCCTRL = 0x00000002;
			
		  ANA->REG3 |= BIT0;
			ANA->ADCCTRL |=  BIT31;
			Adcctrl.Status= B0000_0001;
			
		}
		if( Adcctrl.Timer == 10 )//�����¶�adc 
		{
			Adcctrl.Timer = 0;
			ANA->REG1	= 0x00 ;
			ANA->INTSTS= 0x00000003;
			ANA->ADCCTRL = 0x0000000a;
			ANA->ADCCTRL |= BIT31;
		  ANA->REG3 |= BIT0;			
			Adcctrl.Status= B0000_0010;
		}	
		
//		if( Display_Space_Timer != 0 ) //����ң�ػ�����ʱ
//		{
//			Display_Space_Timer--;	
//			if( Display_Space_Timer==0 ) Lcdled.B16 = Hw_Lcdled_Timer;//�������ⶨʱ��
//		}
		
		if( Relay_Space_Timer != 0 ) //�ϵ���ʱ�����̵���
		{
			Relay_Space_Timer--;
			if( Relay_Space_Timer==0 ){ relay_direct_flag = 1; run_alarm_flag = 1; }
		}
		if( PLC_RST1_Timer != 0 )
		{			
			PLC_RST1_Timer--;			
		}
		
//		
//		PowerValue.ActPowerP += 1;
		
		PowerRead_Flag = 0x5A;
		
		
	}
	if ( PowerLow == 1 )
	{
		if (RTC->INTSTS&RTC_INTSTS_WKUSEC )    RTC_Status |= RTC_INTSTS_WKUSEC;
		if (RTC->INTSTS&RTC_INTSTS_WKUHOUR )   RTC_Status |= RTC_INTSTS_WKUHOUR;
		if (RTC->INTSTS&RTC_INTSTS_MIDNIGHT )  RTC_Status |= RTC_INTSTS_MIDNIGHT;
	}	
	RTC->INTSTS = 0x0000ffff;
}

/**
  * @brief  This function handles U32K0 interrupt request.
  * @param  None
  * @retval None
  */
void U32K0_IRQHandler(void)
{
}

/**
  * @brief  This function handles U32K1 interrupt request.
  * @param  None
  * @retval None
  */
void U32K1_IRQHandler(void)
{
}

/**
  * @brief  This function handles I2C interrupt request.
  * @param  None
  * @retval None
  */
void I2C_IRQHandler(void)
{
}

/**
  * @brief  This function handles SPI1 interrupt request.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{
}

/**
  * @brief  This function handles UART0 interrupt request.
  * @param  None
  * @retval None
  */
void UART0_IRQHandler(void)
{
     
}

/**
  * @brief  This function handles UART1 interrupt request.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
}

/**
  * @brief  This function handles UART2 interrupt request.
  * @param  None
  * @retval None
  */
void UART2_IRQHandler(void)
{
	   unsigned char  Temp;
	
		 if((UART2->INTSTS)&UART_INTSTS_RXIF) 
	   {
			   UART2->INTSTS = UART_INTSTS_RXIF; //�κζԼĴ����Ĳ������ᵼ��������, �����Ȼ���
	
		
			   Temp = UART2->DATA; //��UART2���ݼĴ�����ȡ��������

			   Judge_645_Frame( 0, Temp );
			   if ( Uart[0].Step == 0x08 )
		   	{
				 //UARTIE &= B0000_1111;	//�ر�RXIE2
				  //UART2->CTRL &= ~(BIT3|BIT1 );	//clr UART2.rxif
				 display_zaibo_flag = 1;
			  }
		 }	
		if( (UART2->INTSTS)&UART_INTSTS_TXDONE)
	  {
		    UART2->INTSTS = UART_INTSTS_TXDONE;
			  Tx_645_Frame( 0 );
//			UARTIF &= B1110_1111;	//clr UART2.txif
		}
		UART2->INTSTS = 0xffffffff;
}

/**
  * @brief  This function handles UART3 interrupt request.
  * @param  None
  * @retval None
  */
void UART3_IRQHandler(void)
{
}

/**
  * @brief  This function handles UART4 interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{
	unsigned char  Temp;
	
		 if((UART4->INTSTS)&UART_INTSTS_RXIF) 
	   {
			   UART4->INTSTS = UART_INTSTS_RXIF; //�κζԼĴ����Ĳ������ᵼ��������, �����Ȼ���
	
		
			   Temp = UART4->DATA; //��UART2���ݼĴ�����ȡ��������

			   Judge_645_Frame( 1, Temp );
			   if ( Uart[1].Step == 0x08 )
		   	{
				 display_zaibo_flag = 0;
					
			  }
		 }	
		if( (UART4->INTSTS)&UART_INTSTS_TXDONE)
	  {
		    UART4->INTSTS = UART_INTSTS_TXDONE;
			  Tx_645_Frame( 1 );

		}	
		UART4->INTSTS = 0xffffffff;
}

/**
  * @brief  This function handles UART5 interrupt request.
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{
     unsigned char  Temp;
	
		 if((UART5->INTSTS)&UART_INTSTS_RXIF) 
	   {
			   UART5->INTSTS = UART_INTSTS_RXIF; //�κζԼĴ����Ĳ������ᵼ��������, �����Ȼ���
	
		
			   Temp = UART5->DATA; //��UART2���ݼĴ�����ȡ��������

			   Judge_645_Frame( 2, Temp );
			   if ( Uart[2].Step == 0x08 )
		   	{
				 display_zaibo_flag = 0;//UARTIE &= B0000_1111;	//�ر�RXIE2
				  //UART2->CTRL &= ~(BIT3|BIT1 );	//clr UART2.rxif
				 
			  }
		 }	
		if( (UART5->INTSTS)&UART_INTSTS_TXDONE)
	  {
		    UART5->INTSTS = UART_INTSTS_TXDONE;
			  Tx_645_Frame( 2 );
//			UARTIF &= B1110_1111;	//clr UART2.txif
		}
		UART5->INTSTS = 0xffffffff;
}

/**
  * @brief  This function handles ISO78160 interrupt request.
  * @param  None
  * @retval None
  */
void ISO78160_IRQHandler(void)
{
}

/**
  * @brief  This function handles ISO78161 interrupt request.
  * @param  None
  * @retval None
  */
void ISO78161_IRQHandler(void)
{
}

/**
  * @brief  This function handles TMR0 interrupt request.
  * @param  None
  * @retval None
  */
void TMR0_IRQHandler(void)
{
	  unsigned char Ch;
	  uint16 i;


     if(!(TMR0->INT&TMR_INT_INT_Msk)) return;
	   TMR0->INT = TMR_INT_INT_Msk; //�κζԼĴ����Ĳ������ᵼ��������, �����Ȼ���
		
	
	if( VectorTimer == 0 ) VectorTimer  = ms_timer_delay;
	if( VectorTimer == 1 )
		run_readrtc_flag = 1;	//?????
	if( (VectorTimer == 50) || (((VectorTimer == 20)||(VectorTimer == 70))&&display_rtc_flag) ) run_display_flag = 1;	//??????
	if( VectorTimer == 100 )
	{
		run_relaychk_flag = 1;	//???????
//		RELAYCTRLON;		//??????
//		Clear_SYS_Wdt();	//??????
	}
	VectorTimer--;
//	
	DisKeySamp<<=1;
	if(DISKEY1_N)  
	{		
		DisKeySamp++;	//????????		
	}
 
  DisKeySamp1<<=1;
	if(DISKEY_N) 
  {		 
		DisKeySamp1++;	//????????		
	}
//	CardKeySamp<<=1;
//	if(KEY_N)  CardKeySamp++;	//??????
	
	//ADC??
//	if((Adcctrl.Status & B0000_0011)&&(ADCDATAH & B1000_0000))
//	{
//		ADCResultArr.Buffer[ADCResultArr.ReadCount&0x0f] = ((ADCDATAH&B0000_0111)*0x100+ADCDATAL);
//		ADCResultArr.ReadCount++;

//		//??32????16???????????
//		if(ADCResultArr.ReadCount >= 32)
//		{
//			if ( Adcctrl.Status == B0000_0001 ) 
//			{
//				Adcctrl.Status = B0001_0000;//??adc??
//			}
//			else
//			{
//				Adcctrl.Status = B0010_0000;//??adc??
//			}
//		}
//	}
	
  if( Timer.PowerRead_Tmr < 60 )//
     Timer.PowerRead_Tmr++;
  
	if( Timer.PowerRead_Tmr >= 100 )
  {
     Timer.PowerRead_Tmr = 0;
      PowerRead_Flag = 0x5A;
  }

	if ( Shiduanctrl.Timer != 0 ) //????????
	{
		Shiduanctrl.Timer--;
	}
	if( Shiduanctrl.Timer == 0 ) SHIDUANCTRL_OFF;
		
	if( Relay_Mode != 0x55 )//?????
	{
		if ( Relayctrl.Timer != 0 ) 
		{
			Relayctrl.Timer--;
		}
		if( Relayctrl.Timer == 0 ) {RELAYOFF1; __NOP(); RELAYOFF2;}//????????
    
		if ( Powertimer.Timer != 0 ) 
		{
			Powertimer.Timer--;
		}
	}
	else
	{
		if(Mode.Feature1&0x01)//????
		{
			if ( Relayctrl.Timer != 0 ) 
			{
				Relayctrl.Timer--;
			}
			if( relay_status_flag && (Relayctrl.Timer == 0) ) {RELAYOFF3;}//
		}
	}
	
	if ( Resetworking_Timer != 0 ) 
	{
		Resetworking_Timer--;
	}
	if( Resetworking_Timer == 0 )
	{
	//	PLC_RST1;
	}
	
	if ( Zb_Relayctrl_Timer != 0 ) 
	{
		Zb_Relayctrl_Timer--;
	}	
	
	Beep_Ctrl();//?????
		
	for ( Ch=0; Ch<3; Ch++ )
	{
		if(Uart[Ch].Overtime_Timer != 0)
		{
			Uart[Ch].Overtime_Timer--;
			if( Uart[Ch].Overtime_Timer == 0 ) 
			{
					if( Ch==0 ) 
					{	
						if( UART2->CTRL  != 0x0000000a ) UART2->CTRL   = 0x0000000a;
//						UARTIE |= B0000_0010;	//??RXIE0
////						RXSTA0 &= B1111_0001;
//						{RXSTA0 = 0;RXSTA0 = B0101_0000;}//????FIFO    �ز�
					}
					if( Ch==1 ) 
					{	
						if( UART4->CTRL  != 0x0000000a ) UART4->CTRL   = 0x0000000a;
//						UARTIE |= B0000_1000;	//??RXIE1
////						RXSTA1 &= B1111_0001;
//						{RXSTA1 = 0;RXSTA1 = B0101_0000;}//????FIFO     ����
//						R485CTRLR;		//????						
					}
					if( Ch==2 ) 
					{	
						if( UART5->CTRL  != 0x0000000a ) UART5->CTRL   = 0x0000000a;
//						UARTIE &= B1110_1111;	//??TXIE2
//						UARTIE |= B0010_0000;	//??RXIE2
////						RXSTA2 &= B1111_0001;
//						{RXSTA2 = 0;RXSTA2 = B0101_0000;}//????FIFO  RS485
					}
					Uart[Ch].Step = 0x00;
					Uart[Ch].Flag = 0x00;
					Uart[Ch].Response_Timer = 0x00;
			}
		}
				
	}
	for ( Ch=0; Ch<3; Ch++ )
	{
		if(Uart[Ch].Response_Timer != 0)
		{
			Uart[Ch].Response_Timer--;
			if( Uart[Ch].Response_Timer == 0 ) 
			{
				if ( Uart[Ch].Flag & B0000_1000 )
				{	
					
					if( Ch==0 ) 			//????????
					{	
						UART2->CTRL &= ~(BIT3|BIT1 );	//clr UART2.rxif
				    UART2->CTRL |= (BIT0|BIT8 );	
						UART2->DATA = 0x68;	
						
					}
					if( Ch==1 ) 
					{	
//						R485CTRLS;		//????
//						TXREG1  = 0x68;		//??0x68
//						UARTIE &= B1111_0111;	//??RXIE1
//						UARTIE |= B0000_0100;	//??TXIE1
				    UART4->CTRL &= ~(BIT3|BIT1 );	//clr UART2.rxif
				    UART4->CTRL |= (BIT0|BIT8 );	
						UART4->DATA = 0x68;	
					}
					if( Ch==2 ) 
					{	
//						TXREG2  = 0x68;		//??0x68
//						UARTIE &= B1101_1111;	//??RXIE2
//						UARTIE |= B0001_0000;	//??TXIE2
						
						
						UART5->CTRL &= ~(BIT3|BIT1 );	//clr UART2.rxif
				    UART5->CTRL |= (BIT0|BIT8 );	
						UART5->DATA = 0x68;	
						
						//if( UART2->CTRL  != 0x00000100 ) UART2->CTRL   = 0x00000100;
						
					}
					Uart[Ch].Len  = 5;//??????
					Uart[Ch].Total_Len = Uart[Ch].Buf[9]+16;
					Uart[Ch].Overtime_Timer = txtimeout_max;
				}
				else
				{
					Uart[Ch].Response_Timer++;//????
				}
			}
		}			
	}
	
//	if( Compile_Polar_485 == 0x55 )	//?????485
//	{
//		if( (Uart[1].Response_Timer == 0)&&(Uart[1].Overtime_Timer == 0)&&((UARTIF&B0000_1000)==0x00) )
//		{
//			Uart1KeySamp<<=1;
//			if(GPIO0&B0100_0000)  Uart1KeySamp++;		//?????????,?????485
//			
//			if( Uart1KeySamp == 0xff ) //??? ???
//			{
//				if( RTXCON1 != B0000_0000 )	RTXCON1 = B0000_0000;//??? ???????,???????
//			}
//			else if( Uart1KeySamp == 0x00 )
//			{
//				if( RTXCON1 != B0000_0011 )	RTXCON1 = B0000_0011;//??? ??????,??????		
//			}	 
//		} 
//		else Uart1KeySamp = 0x55;
//	}
//	else
//	{
//		if( RTXCON1 != B0000_0000 )	RTXCON1 = B0000_0000;//??? ???????,???????
//	}
	
	if( (*ANA_CMPOUT & BIT8) && (Powertimer.Timer == 0) ) 
	{	
		for(i=0; i<0x300; i++)		//?3.75ms
		{
			if( (*ANA_CMPOUT & BIT8) == 0 ) break;
		}	
		if( i==0x300 ) //??
		{
			LCDLEDOFF;//
			RELAYLEDOFF;//
			BEEPALARMOFF;//
			
			for(i=0; i<0x300; i++)		//?3.75ms
			{
				if( (*ANA_CMPOUT & BIT8)== 0 ) 
				{
				    FLAG_EA = 0x5555AAAA;			
		        break;	
				}	
			}	
			if( i==0x300 ) 
			{
				FLAG_EA = 0;
        NVIC_DisableIRQ(RTC_IRQn);  		
			}
		}
	}
}

/**
  * @brief  This function handles TMR1 interrupt request.
  * @param  None
  * @retval None
  */
void TMR1_IRQHandler(void)
{
	if(!(TMR1->INT&TMR_INT_INT_Msk)) return;
	TMR1->INT = TMR_INT_INT_Msk; //�κζԼĴ����Ĳ������ᵼ��������, �����Ȼ���
	PLUSELED_OFF;
	NVIC_DisableIRQ(TMR1_IRQn);
	TMR1->CTRL   = 0;
}

/**
  * @brief  This function handles TMR2 interrupt request.
  * @param  None
  * @retval None
  */
void TMR2_IRQHandler(void)
{
}

/**
  * @brief  This function handles TMR3 interrupt request.
  * @param  None
  * @retval None
  */
void TMR3_IRQHandler(void)
{
}

/**
  * @brief  This function handles PWM0 interrupt request.
  * @param  None
  * @retval None
  */
void PWM0_IRQHandler(void)
{
}

/**
  * @brief  This function handles PWM1 interrupt request.
  * @param  None
  * @retval None
  */
void PWM1_IRQHandler(void)
{
}

/**
  * @brief  This function handles PWM2 interrupt request.
  * @param  None
  * @retval None
  */
void PWM2_IRQHandler(void)
{
}

/**
  * @brief  This function handles PWM3 interrupt request.
  * @param  None
  * @retval None
  */
void PWM3_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA_IRQHandler(void)
{
}

/**
  * @brief  This function handles FLASH interrupt request.
  * @param  None
  * @retval None
  */
void FLASH_IRQHandler(void)
{
}

/**
  * @brief  This function handles ANA interrupt request.
  * @param  None
  * @retval None
  */
void ANA_IRQHandler(void)
{
}

/**
  * @brief  This function handles SPI2 interrupt request.
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void)
{
}

/******************* (C) COPYRIGHT Vango Technologies, Inc *****END OF FILE****/



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
					if( CH == 1 )
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
		if( Uart[CH].Len < 4 ) 
		{
			if( CH==0 ) 	UART2->DATA = 0xfe; //�ز�
		  else if( CH==1 ) 	UART4->DATA = 0xfe;//����
		  else  	UART5->DATA = 0xfe;//RS485
		}
		else 
		{	
			if( CH==0 ) 	UART2->DATA = Uart[CH].Buf[Uart[CH].Len-4]; //�ز�
		  else if( CH==1 ) UART4->DATA = Uart[CH].Buf[Uart[CH].Len-4];//����
		  else  	UART5->DATA = Uart[CH].Buf[Uart[CH].Len-4];//RS485
		}
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
		if( Sclock.Clockbackup[0] & B0000_0001 ) {RELAYLEDON;}
		else {RELAYLEDOFF;}	
	}
	else if( (Relay_Mode==0x55) && (relay_status_flag==0) && (real_relaystatus_flag == 1) && (relay_success_flag==0) )
	{
		if( Sclock.Clockbackup[0] & B0000_0001 ){ RELAYLEDON;}
		else {RELAYLEDOFF;}	
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
//	BEEPALARMOFF;//�رշ�����(ʡ��)
	if( BDMODE ) //����ģʽ
	{
		if( Decimal.Status&B1100_0000 ) 
		{
			I2C_Write_Eeprom( ee_remainmoney_page, ee_remainmoney_inpage, Money.Remain, ee_remainmoney_lenth+2 );
			I2C_Write_Eeprom( ee_DayUserINFO_page, ee_UserINFO_inpage, Money_UserINFO.Day_Moneyused, 16 );
		}
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
	
	memset( Emu_Data.U.B08, 0x00, 32 );//��ѹ�������ʹ�����������
	
	
	Emu_Data.Cosa.B32 = 0x1000;
	
	
	Real.U[0] =0;Real.U[1] =0;
	Real.U[2] =0;
	Real.I[0] =0;Real.I[1] =0;
	Real.I[2] =0;
	
	memset( &Real.U[0], 0x00, 74 );
	
	Real.PF[0] =0x1000;Real.PF[1] =0x1000;Real.PF[2] =0x1000;
	Real.PF[3] =0x1000;
	System_Status1 = 0;//����״̬��1
	
	Cal_Comb( 0x00, 0xFF, 0x01, Ic_Comm_TxBuf );//������1��������ϵ���(������������Ҫ��)
	Cal_Comb( 0x00, 0xFF, 0x02, Ic_Comm_TxBuf+20 );//������2��������ϵ���(������������Ҫ��)
}

void Lcd_Disp_Off(void)//����lcd������ʾ
{			 	
    LCD->CTRL = 0;
	  LCD->SEGCTRL0 = 0x00000000;	
	
	  LCD->SEGCTRL1 = 0x00000000;	
	  LCD->SEGCTRL2 = 0x00000000;	
	  
}

void Lcd_Disp_On(void)//����lcdͣ����ʾ
{
  if( LCD->CTRL !=(BIT4|BIT7|BIT1 )) LCD->CTRL = BIT4|BIT7|BIT1;		//4com or 8com �C 0x10; 6com �C 0x15
  if( LCD->CTRL2 !=(BIT0)) LCD->CTRL2 = BIT0;		//A���� 4com = 0x00; 6com = 0x01; 8com =0x02
	if( LCD->SEGCTRL0 !=0xfff00ffc) LCD->SEGCTRL0 = 0xfff00ffc;	//seg0~seg31
		if( LCD->SEGCTRL1 !=0xb8160407) LCD->SEGCTRL1 = 0xb8160407;	//seg32~seg63
		if( LCD->SEGCTRL2 !=0x00001c0c) LCD->SEGCTRL2 = 0x00001c0c; //seg64~seg79
	if(*ANA_REG6  != B0000_0011 )	*ANA_REG6  = B0000_0011;	//��ֹ����
  if( LCD->CTRL !=(BIT4|BIT7|BIT1 )) LCD->CTRL = BIT4|BIT7|BIT1;	
}


void Configure_Register_Sleep(void)//sleepǰ���üĴ���
{
  
	GPIOA->DAT = 0x000030C0;//ȷ�������½��ش�������
	GPIOB->DAT = 0x00006000;
	GPIOC->DAT = 0x00000000;
	GPIOD->DAT = 0x00000000;
	GPIOE->DAT = 0x00000000;
	GPIOF->DAT = 0x00000000;
	
	GPIOA->OEN = 0x0000FFFF;
	GPIOA->IE  = 0x000030C0;	
	GPIOA->WKUEN = 0x05000000 ;//PE2ǿ����ʹ��	
	GPIOA->SEL= 0x00000000;	
	GPIOA->IOANODEG	=0x00000000 ;	//ģ���Һ����AD����ѡ	
	*PMU_CONTROL =0x00000001;
	*PMU_IOAINT = 0x0000FFFF;
	NVIC_ClearPendingIRQ(PMU_IRQn );
 	NVIC_EnableIRQ(PMU_IRQn );
	
	GPIOB->OEN=0x00009FFF;
	GPIOB->IE= 0x00000000;
	
	GPIOC->OEN	=0x0000FFFF ;
	GPIOC->IE	= 0x00000000 ;
	
	
	GPIOD->OEN  = 0x0000FFFF;
  GPIOD->IE   = 0x00000000;
	
	GPIOE->OEN	= 0x0000FFFF;
	GPIOE->IE   = 0x00000020;
	
	GPIOF->OEN	= 0x0000FFFF;
	GPIOF->IE   = 0x00000000;
  
	TMR0->CTRL  = 0x00000000;
	NVIC_DisableIRQ(TMR0_IRQn);
	TMR1->CTRL   = 0x00000000;
	NVIC_DisableIRQ(TMR1_IRQn);
	
	ANA->ADCCTRL = 0x00000000;
	ANA->REG3 = 0x00000020;//��ʱ�Ӳ��ܹرգ�BGP������ǰ�ر�		
  ANA->REG1	= 0x00 ;
	
	UART0->CTRL   = 0x00000000;
  NVIC_DisableIRQ(UART0_IRQn);	
	
	UART2->CTRL   = 0x00000000;
  NVIC_DisableIRQ(UART2_IRQn);	
	
	UART0->CTRL   = 0x00000000;
  NVIC_DisableIRQ(UART4_IRQn);  
 
	UART5->CTRL   = 0x00000000;
  NVIC_DisableIRQ(UART5_IRQn);	
	*MISC_IREN = 0x00000000;
	
	

  ANA->INTEN |= BIT8|BIT11; 
  ANA->REGA  |= (BIT1|BIT3);//�л���BAT1���硢���Ľ���3
  *ANA_REG9  |= BIT7;	//��ֹ����
	NVIC_DisableIRQ(ANA_IRQn);
  ANA->INTSTS = 0xFFFFFFFF;
	

	NVIC_DisableIRQ(RTC_IRQn); 
	
	RTC_AutoCalCmd(DISABLE); 
  RTC_WriteProtection(DISABLE);
	if( RTC->ACCTRL	!= BIT2 ) RTC->ACCTRL 	= BIT2;	//ʹ��RTC��� 
  RTC->CTL 	= 0;	//ʹ��RTC��� 			
	RTC->ACCTRL = 0;
	if( RTC->INTEN	!= (BIT2|BIT4|BIT5) ) RTC->INTEN = (BIT2|BIT4|BIT5);
  RTC_WriteProtection(ENABLE);
  NVIC_DisableIRQ(RTC_IRQn); 
	
	DisKeySamp =0;
	DisKeySamp1 =0;
	Stop_Disptimer = 0;//ͣ����ʾʱ������
	Display.Number = 1;
	Display.Status = 0;//�尴����ʾ��ر�־
  memset( DispBuf, 0x00, 160 );	//����ʾ������
	memcpy( (unsigned char xdata *)(0x40002000), DispBuf, 160 );	
	Lcd_Disp_Off();//����lcdͣ����ʾ
	Ic_Type_Reg = 0;//��忨��ʾ��ر�־  
  CoverDetCnt = 0;
	
	if( (PowerDetCnt1 < 4)&&(PowerDetCnt2 < 8) )//������е�Դ�쳣���
	{
		PowerDetCnt0 = 3;
	}
	else
	{
		PowerDetCnt0 = 0;
	}

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
	 
	if( Status < 2 )
	{
		if(RTC_Status&RTC_INTSTS_WKUHOUR ) //rtc
	  {		
		  RTC_Status &= ~RTC_INTSTS_WKUHOUR;
			if( Status == 1 )
			{
				if( RAMREG0 == 0x55 )
				{
					Calculate_Min(60);//��Сʱ����
				}
				else
				{
					Calculate_Min((60-RAMREG1));//��һ��Сʱ����
					RAMREG0 = 0x55;
				}
				
//				//ͣ���¶Ȳ���
//				ADCResultArr.ReadCount = 0;
				
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
				
				//ת��18��ȡ����16���Ա�֤ת������׼ȷ��
//				for( i=0; i<18; i++ )
//				{
//					I2c_Wait_5ms();
//					ADCResultArr.Buffer[ADCResultArr.ReadCount&0x0f] = ((ADCDATAH&B0000_0111)*0x100+ADCDATAL);
//					ADCResultArr.ReadCount++;
//				}
//				Adcctrl.Status = B0010_0000;//�¶�adc����
//				Adc_Proc();
//				ADCCTRL   = 0; //��ʹ��ADC
			}					
			
			Read_Sysclock();//��ʱ��
			RAMREG4 = 0x55;	//�ÿ�������־	
			Histroy_Freeze_Dl(0);//����������ᴦ��
			Pd_Freeze_Judge(1);//ͣ�綳�ᴦ��
			
			if(RAMREG12) RAMREG12--;//ͣ����ʾʱ���1
			if(RAMREG12 == 0)
			{
				if(Stop_Disptimer == 0) 
				Lcd_Disp_Off();	//�ر�lcd��ʾ
			}
		}
		
		if ( RTC->INTSTS&RTC_INTSTS_MIDNIGHT )//1�컽��
		{
			Clr_Zero_Data();//�������������
		  RTC_Status &= ~RTC_INTSTS_MIDNIGHT;
		}
//		 RTC_Status &= ~RTC_INTSTS_MIDNIGHT;
//		 (RTC->INTSTS) |= RTC_INTSTS_MIDNIGHT;	//���Сʱ�仯����ı�־
	}
	else
	{
		if(RTC->INTSTS&RTC_INTSTS_WKUHOUR ) //rtc
	  {		
		  RTC_Status &= ~RTC_INTSTS_WKUHOUR;	
			Freeze_Judge();		//����ͽ����ж�
		}
	}
}

	
void Reset()	//���Ѹ�λ
{	
	
//	Clock_Init();
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
// 
	(*( void (*)( ) )0x00000321) ();	//��λ,��ͷ��ʼ����
}


void Sleep( void )
{
	uint16 i;
//  PMU_LowPWRTypeDef LowPower_InitStruct;
	//�ڽ���sleepǰ�ٴν���ȥ��
	for(i=0; i<0x300; i++)		//Լ3.75ms
	{
		if( (*ANA_CMPOUT & BIT8) == 0 ) { reg_veri_flag = 1; FLAG_EA = 0x5555AAAA; NVIC_EnableIRQ(RTC_IRQn); return; }	//����������
	}
	IICPowerON = 0;	
	
	Store_Data();
	Configure_Register_Sleep();
	Powerd_Hourwakeup(0);		//Сʱ�����¼��ж� 
	NVIC_EnableIRQ(RTC_IRQn); 
	for( ; ; )
	{		
		
		Clear_Wdt();	
		
		if( (*ANA_CMPOUT & BIT8)  == 0  )	//sleepǰ�ٴ�ȥ��
 		{ 			
	 		for(i=0; i<300; i++)
			{
				if( (*ANA_CMPOUT & BIT8)  != 0) break;
			}	
			if( i==300 ) 
			{				
				NVIC_DisableIRQ(RTC_IRQn); 
				Reset();
			}
 		}
		PowerLow = 1;
		Clear_Wdt(); 
		WDT_Disable();	
		while ((*PMU_IOASTS&0X00003000)!=0X00003000)
		{
		  Clear_Wdt();
		}
		ANA->REG3 |= 0x00000008;
		SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; 
		 __WFI();
		Clear_Wdt();
		
	  if ( KEY_Status )
	  {
		  KEY_Status = 0;
//			
		  Displaykey_Process();//��ʾ��������
		  if( Stop_Disptimer==0 ) 
		  {
		    Display.Timer = Display_Para.Loop_Time;//��1��ѭ����ʾ,��2�ΰ���ͣ��30s
			  Display.Status &= B1111_1110;//�尴����ʾ��־
			  Stop_Disptimer = Get_Valid_Num(0)*Display.Timer;//ѭ����ʾһ��
						
			  if( Stop_Disptimer==0 ) Stop_Disptimer = Display.Timer;//��ֹѭ����ʾ��Ŀ��Ϊ0
				
		  }
		  else
		  {
			  Stop_Disptimer = default_keydisplaytimer/2;//30s
		  }
		  Lcd_Display();			//ˢ����ʾ
      Lcd_Disp_On();
      DisKeySamp =0;
			DisKeySamp1 =0;
	  }	
		
		
		if ((RTC_Status&RTC_INTSTS_WKUSEC ))
	  {
			
			
			
			RTC_Status &= ~RTC_INTSTS_WKUSEC;
			if( PowerDetCnt0 >= 3 )
		{
			PowerDetCnt0++;
			if( PowerDetCnt0 >= 5 )
			{									
				PowerError_Check();//��Դ�쳣���(��բ���ж�)
				if( RAMREG11 != 0x55 ) PowerDetCnt0 = 0;
				else
//						if( PowerDetCnt0 >= 8 )
				{
					PowerDetCnt0 = 0;
					RAMREG11 = 0xaa;//ֻ�м�¼����ʱ�����ϵ��������ʱ��
					memcpy( Ic_Comm_RxBuf+100, Sclock.Clockbackup, 6 );//�����Դ�쳣����ʱ��(���ʱ������)
				}
						
			}
			else
			{
				Read_Sysclock();//��ʱ��
			}
		}
    if( RAMREG2 )
		{
			if( Cover_Space_Timer )
			{
				Cover_Space_Timer--;
				if( Cover_Space_Timer == 0 ) Read_Sysclock();//��ʱ��
			}
			Cover_Check();
		}		
//		if( CoverDetCnt )
//		{
//			
//                       				 
//			Cover_Check();//���Ǽ��
//			if( Cover_Space_Timer == Bcd_To_Hex(Threshold[3]) )  //δ��⵽���ǻ�������¼��� 
//			{
//				CoverDetCnt = 0;
//			}			
////					if( RAMREG2 == 0 ) GPIO4IE &= B1011_1111;
//		}
				
		if( Stop_Disptimer ) 
		{
			Stop_Disptimer--;
			if( Stop_Disptimer ) 
			{
				if( Display.Timer ) Display.Timer--;
					
				if( Display.Timer==0 ) display_rtc_flag = 1;
				if( display_rtc_flag )
				{
							
					Lcd_Display();	//ˢ����ʾ
        	Lcd_Disp_On();
							
				}
			}	
			if( Stop_Disptimer == 0 ) 
		  {
		       Display.Number = 1 ;	//��ͷ��ʼ��ʾ
					
		      Lcd_Disp_Off();//����lcd������ʾ			
					
//					if(( PowerDetCnt0 < 3 ) && (CoverDetCnt == 0) &&(!(GPIO0&B0100_0000))) RTCIE &= B1111_1110;//�ر�ÿ�뻽��
		
	  }		
		}
				
		
		Powerd_Hourwakeup(1);	//Сʱ�����¼��ж�	   
	}	
//	KEY_Status = 0;
//	RTC_Status = 0;
//	
 
		
	}
}
	
	
	

