#include "ex_func.h"
#include "ex_var.h"
//#include <intrins.h>
#include <string.h>
#include <absacc.h>


void Enable_Closerelay(void)//�����պϼ̵������߲忨�պϼ̵���
{	
	if( relay_commu_disable_close_flag )
	{
		relay_commu_disable_close_flag = 0;
		Store_Meterstatus();
	}
	
	if( !BDMODE ) return; //Զ��ģʽ

	if((!card_closerelay_flag) && (first_alaram2_flag))
	{
		first_alaram2_flag = 0;
		card_closerelay_flag = 1;
		Store_Meterstatus();
	}
	if((!card_closerelay_flag1) && (first_eqzero_flag))
	{
		first_eqzero_flag = 0;
		card_closerelay_flag1 = 1;
		Store_Meterstatus();
	}
}


void Store_Meterstatus(void)//������״̬�ֽ�
{
	run_alarm_flag = 1;
	I2C_Write_Eeprom44( ee_meterstatus1_page, &Meter_Status_S.B08, ee_meterstatus1_lenth);
}


void Displaykey_Process(void)
{
	Lcdled.B16 = lcdled_timer_delay;//�������ⶨʱ��
	LCDLEDON;
	Init_Led_Timer = 0;
	Display.Timer = default_keydisplaytimer;
	if (Display.Status & B0000_0001 ) 
	{
		if (DisKeySamp1 == 0xff )
		{ 	
			
			Display.Number--;
			if (Display.Number < 1 )
				Display.Number = Display_Para.K_Number;
			
		}
	 if (DisKeySamp == 0xff )
		{  
			Display.Number++;
		  if (Display.Number > Display_Para.K_Number) Display.Number = 1;
		}
		
	}
	else
	{
		Display.Number  = 1;
		Display.Status |= B0000_0001;//�ð�����ʾ��־
		Ic_Type_Reg = 0;//��忨��ʾ��ر�־
	}		
	Display.Status |= B1000_0000;
	run_display_flag = 1;	//��ˢ����ʾ��־
}


void Displaykey_Check(void)
{

	if( (!(Display.Status & B1000_0000)) && ((DisKeySamp==0xff)||(DisKeySamp1==0xff))) 
	{
		Displaykey_Process();
		Keyclose_timer = 3;//����3s�����պϼ̵�����ʱ��
		DisKeySamp=0x0f;
	}
	else if( (Display.Status & B1000_0000 ) && ((DisKeySamp==0)&&(DisKeySamp1==0))) 
	{
		Display.Status &= B0111_1111;
		Keyclose_timer = 0;
		DisKeySamp1=0;
	}
	else if( (Display.Status & B1000_0000 ) && (DisKeySamp==0xff) )
	{
		if( Keyclose_timer == 0 )
		{
			Enable_Closerelay();	//�����պϼ̵���
		}
	}
}


void Cover_Check( void )
{
	uint16 i;

	if( (Cover.Status == 0) && (COVKEY_P) ) //
	{
		for( i=0; i<650/clkmode; i++ )			//5ms��ʱ�ж�
		{
			if( COVKEY_N ) break;
		}
		
		if( i == 650/clkmode ) //����
		{
			if( Cover_Space_Timer )	return;		//������ʱδ��
			
			Cover.Status = 1;
			Note_Run_Event(ee_covern_inpage);	//��¼�����¼���¼
		}
	}
	else if( (Cover.Status) && (COVKEY_N) )
	{
		for( i=0; i<650/clkmode; i++ )
		{
			if( COVKEY_P ) break;
		}

		if( i == 650/clkmode ) //�ϸ�
		{
			if( Cover_Space_Timer )	return;		//������ʱδ��
			
			Cover.Status = 0;
			Note_Run_Event(ee_covern_inpage|0x80);	//��¼�ϸ��¼���¼
			if( RAMREG2 ) RAMREG2--;	//ͣ�翪�Ǵ�����1
		}
	}
	
	Cover_Space_Timer = Bcd_To_Hex(Threshold[3]); //ȡ������¼��ж���ʱʱ�䶨ֵ
}


void Relaystatus_Check(void) //�̵���״̬���(���巽ʽ,�Ͽ��ߵ�ƽ)
{
	uint16 i;

	run_relaychk_flag = 0;	//��̵���״̬����־
	
	if( Relay_Space_Timer || (PowerDetCnt0 < 4) || ( FLAG_EA == 0 ) )
	{
		if( relay_error ) Relayerror_Count = Defrelayerrorcount_r;
		else Relayerror_Count = 0;
		return; 
	}
	
	if( Relay_Mode != 0x55 )//���ü̵���
	{
		if ( Relayctrl.Timer ) return; 
	}
	else
	{
		if((Mode.Feature1&0x01)&&Relayctrl.Timer ) return; 
	}
	
	for( i=0; i<3600/clkmode; i++ )				//25msȥ���ж�
	{
		if( RELAYTEST_P ) break;
	}
	
	if( ((relay_status_flag==1) && ((power_zero_flag==0) || (i != 3600/clkmode))) //����Ϊ�Ͽ�,�����й��ʻ��߷���ͨ
	    ||  ( (relay_status_flag==0) && (power_zero_flag==1) && (i == 3600/clkmode) //�̵�������Ϊ�պ�
		  && ( (Relay_Mode != 0x55) || ((Relay_Mode == 0x55)&&(relay_cleardl_flag==0)) ) ) )
	{		
		if( relay_status_flag ) comm_relaystatus_flag = 0;//ˢ�¼̵���ʵ��״̬(ͨѶbit4ר��)
		else comm_relaystatus_flag = 1;
		if( Relayerror_Count1 < Defrelayerrorcount ) Relayerror_Count1++;
		else
		{
			if( relay_status_flag ) real_relaystatus_flag = 0;//ˢ�¼̵���ʵ��״̬
			else real_relaystatus_flag = 1;
		}
		Relayerror_Count2 = 0;
		
		if( relay_error ) return;
		
		if( Relayerror_Count < Defrelayerrorcount_r )
		{
			Relayerror_Count++;	//�̵�����������������1
		}
		else
		{
			Note_Run_Event(ee_relayerrn_inpage);	//���ɿ����쳣��ʼ��¼
			relay_error = 1;//�ü̵�������
		}
	}
	else
	{
		comm_relaystatus_flag = relay_status_flag;	//ˢ�¼̵���ʵ��״̬(ͨѶbit4ר��)
		if( Relayerror_Count2 < Defrelayerrorcount ) Relayerror_Count2++;
		else
		{
			real_relaystatus_flag = relay_status_flag;	//ˢ�¼̵���ʵ��״̬
		
			if( relay_success_flag == 0 )
			{
				relay_success_flag = 1;				//�ü̵��������ɹ���־
				Store_Meterstatus();
				
				if( relay_status_flag )
				{
					if( RdRecTimes( ee_openrelayn_inpage ) && (Relay_modechange_flag==0) )  Set_Reportdata(1*8+6);//������բ�ɹ��ϱ�
				}
				else
				{
					if( RdRecTimes( ee_closerelayn_inpage ) && (Relay_modechange_flag==0) )  Set_Reportdata(1*8+7);//���к�բ�ɹ��ϱ�
				}
			}
		}
		Relayerror_Count1 = 0;
		
		if( Relayerror_Count )	//�̵������������
		{
			Relayerror_Count--;
		}
		else
		{		
			if( relay_error ) Note_Run_Event(ee_relayerrn_inpage|0x80);	//���ɿ����쳣������¼
			relay_error = 0;
		}
	}	
}
