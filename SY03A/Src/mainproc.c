#include "main.h"
#include "variable4.h"
#include "ex_func.h"
#include "ex_var.h"
#include <string.h>
#include <stdio.h>

#ifndef  ASSERT_NDEBUG
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_errhandler error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_errhandler error line source number
  * @retval None
  */
void assert_errhandler(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


//;================================================================
//;����
//;================================================================
void Comm_Event_Proc( void )
{
	unsigned char i;
	
	PowerMeasure();		//����������ۼƴ���ͽ���
	
	for( i=0; i<3; i++ )
	{
		if(Uart[i].Flag&B0000_0100)
		{
			Uart_Proc(i);	//ͨѶ����
			Clear_Wdt();
			break;
		}
	}
		
	if( FLAG_EA == 0 )			//����Ƿѹ(��ȥ������)
	{	
		Sleep();		//����sleep
	}
}


void Vector_Event_Proc( void )
{
	if( run_alarm_flag && (Zb_Relayctrl_Timer==0) )
	{
		Mt_Alarmstatus_Judge();	//�����״̬�ж�
	}	
	
	if( run_price_flag && (run_freeze_flag==0) )
  {
		Year_Ladder_Judge();	//����������жϺ����ݺϷ����ж�
		Price_Judge();	//���ʽ��ݵ���ж�
	}	
	
	if( run_timezone_flag )
	{
		Timezone_Judge();	//ʱ��ʱ�η����ж�
	}
	
	if( run_loadchk_flag )
	{
		Load_Judge();		//���ɼ�¼�ж�
	}
	
	if( run_freeze_flag )
	{
		Freeze_Judge();		//����ͽ����ж�
	}		
	
	if( Dm_process_flag )	
	{
	  Demand_Process();	//��������
	}
	
	if( run_readrtc_flag )
	{
		Read_Sysclock();	//��ʱ��
	}
	
	if( run_display_flag )
	{			
		Lcd_Display();		//ˢ����ʾ
	}
	
	if( run_relaychk_flag )
	{
		Relaystatus_Check();	//�̵������
	}	
	
	Cover_Check();		//���Ǽ��
	Displaykey_Check();	//��ʾ�������		
}

void Sclock_Event_Proc( void )
{

	if( fwdl_veri_flag )
	{
		Fwdl_Verify();		//�������У��
	}	
	
	if( bkdl_veri_flag )
	{
		Bkdl_Verify();		//�������У��
	}
	
	if( mode_veri_flag )
	{
		Mode_Verify();		//����ģʽ��У��
	}
				
	if( reg_veri_flag )
	{
		Veri_Sys_Reg();		//�Ĵ���У��
	}
	
	if( addr_veri_flag )
	{
		Commaddr_Verify();	//ͨѶ��ַУ��	
	}
	
	if( Dm_veri_flag )
	{
		Demand_Para_Verify();		//��������У��
	}
	
	if( disppm_veri_flag )
	{
		Display_Para_Verify();	//��ʾ����У��
	}
	
	if( dispitem_veri_flag )
	{
		Displayitem_Verify();	//��ʾ��ĿУ��
	}
	if( temperpara_veri_flag )
	{
		Temperpara_Verify();	//�¶�ϵ��У��	
	}
}


void Energy_Event_Proc( void )
{
	if( check_7051_flag1 )
	{
//		ATT7051_Check();	//У��һ��7051���̼Ĵ���
	}
	if( read_back_flag )
	{
		
//		Emu_Data.U.B32 = 0x2200;
    ParaMeasure();	//��˲ʱUIPֵ
//		Judge_Uip_Data();	//UIP�����ж�
	}
}
	
int main( void )
{
	Init_System();				    //ϵͳ��ʼ��
	
	for( ; ; )
	{
		Clear_Wdt();
		Init_Dw80251();
//		FLAG_EA = 1;		
		PowerLow = 0;
		
		Init_Int();			        //��֤INTΪ�ߵ�ƽ(Ԥ��)
	
		Adc_Proc();				        //��غ��¶�adc����
		
		Vector_Event_Proc();		//time0ʱ�괦��
		Comm_Event_Proc();		  //ͨѶ�����͵�Դ����
		
		Sclock_Event_Proc();		//rtcʱ�괦��
		Comm_Event_Proc();		  //ͨѶ�����͵�Դ����
		
		Energy_Event_Proc();		//��������
		Comm_Event_Proc();		  //ͨѶ�����͵�Դ����
		
		Card_Proc();			      //IC������
		Comm_Event_Proc();		  //ͨѶ�����͵�Դ����
	}
}


