#include "ex_func.h"
#include "ex_var.h"
//#include <intrins.h>
#include <string.h>
#include <absacc.h>


const struct HisEvent_STR ecode   EventTable[]=
{
	0x3001,	ee_cleardl_note_page,		  ee_cleardlnote_inpage,		  ee_cleardlnote_lenth,	    3*8+1,	//�������,0
	0x3003,	ee_clearevent_note_page,	ee_cleareventknote_inpage,	ee_cleareventnote_lenth,  3*8+3,	//�¼�����,0
	0x0311,	ee_powerdown_note_page,		ee_powerdownnote_inpage,	  ee_powerdownnote_lenth,  	2*8+7, 	//�����¼
	0x3004,	ee_setclock_note_page,		ee_setclocknote_inpage,		  ee_setclocknote_lenth,	  3*8+4,	//����ʱ��,
	0x300d,	ee_opencover_note_page,		ee_opencovernote_inpage,	  ee_opencovernote_lenth,	  1*8+2,	//�����,
	0x1901, ee_maxi_note_page, 			  ee_maxinote_inpage,			    ee_maxinote_lenth,        2*8+4,	//����
	0x1d00,	ee_openrelay_note_page,		ee_openrelaynote_inpage,	  ee_openrelaynote_lenth,	  1*8+6,	//��բ8           
	0x1e00,	ee_closerelay_note_page,	ee_closerelaynote_inpage,  	ee_closerelaynote_lenth,  1*8+7,	//��բ8           
	0x3000,	ee_program_note_page,		  ee_programnote_inpage,		  ee_programnote_lenth,	    3*8+0,	//��̼�¼,
	0x3006,	ee_shiqurecord_page,		  0x00,				                ee_shiqurecord_lenth,	    3*8+6,	//ʱ������,
	0x3005,	ee_shiduanrecord_page,		0x00,				                ee_shiduanrecord_lenth,	  3*8+5,	//ʱ�α���,
	0x300c,	ee_jieshuanrecord_page,		ee_jieshuanrecord_inpage, 	ee_jieshuanrecord_lenth,  4*8+4,	//�����ձ��,0
	0x3012, ee_keysetrecord_page, 		ee_keysetrecord_inpage, 	  ee_keysetrecord_len,	    4*8+7,	//��Կ���±��     
	0x3009,	ee_combstatusrecord_page,	ee_combstatusrecord_inpage,	ee_combstatusrecord_lenth,4*8+1,  //�й���Ϸ�ʽ���
	0x3600,	ee_relayerr_note_page,		ee_relayerrnote_inpage,		  ee_relayerrnote_lenth,	  0*8+0,	//���ɿ���������¼
	0x3700,	ee_powererr_note_page,		ee_powererrnote_inpage,		  ee_powererrnote_lenth,	  1*8+5,	//��Դ�쳣��¼
	0x3007, ee_week_note_page,			  ee_week_note_inpage,		    ee_week_note_len,		      3*8+7,	//�����ձ��
	0x3013,	ee_errcardrecord_page,		ee_errcardrecord_inpage,	  ee_errcardrecord_len,	    0xff,	  //�쳣�忨
	0x2200,	ee_batterlow_note_page,		ee_batterlownote_inpage,	  ee_batterlownote_len,    	0*8+3,	//���Ƿѹ
	0x0333,	ee_buydl_note_page,			  ee_buydlnote_inpage,		    ee_buydlnote_lenth,		    0xff,	  //����8
	0x3400,	ee_dmoney_note_page,		  ee_dmoneynote_inpage,		    ee_dmoneynote_lenth,	    0xff,	  //�˷Ѽ�¼
	0x300f,	ee_flpricerecord_page,		ee_flpricerecord_inpage,	  ee_flpricerecord_len,	    4*8+5,	//���ʱ���
	0x3010,	ee_jtpricerecord_page,		ee_jtpricerecord_inpage,	  ee_jtpricerecord_len,	    4*8+6,	//���ݱ���
	0x3002,	ee_clear_demand_page,		  ee_clear_demand_inpage,	    ee_clear_demand_lenth,	  3*8+2,	//��������,0
};


void Exchange(unsigned char *BufA, unsigned char *BufB, unsigned char Len )
{
	unsigned char n;
	for( n=0; n<Len; n++ )
	{
		BufA[n] = BufB[Len-1-n];//��������
	}
}

void Inverse( unsigned char *Buf, unsigned char Len )
{
	unsigned char i, Temp08;
	
	for( i=0; i<Len/2; i++ )
	{
		Temp08 = Buf[i];
		Buf[i] = Buf[Len-1-i];
		Buf[Len-1-i] = Temp08;//��������
	}
}


void Number_Add1_Direct(unsigned char *BufA) //������1
{	
	unsigned char  BufB[4];
	union B32_B08 Add;
	
	if ( ( BufA[0]==0xff) || (BufA[1]==0xff) || (BufA[2]==0xff) )
	{
		BufA[0] = 0;//�Ƿ���������
		BufA[1] = 0;
		BufA[2] = 0;
	}
	BufB[0] = BufA[0];
	BufB[1] = BufA[1];
	BufB[2] = BufA[2];
	BufB[3] = 0;	//���ֽ���ǰ��ɸ��ֽ���ǰ
	Add.B32 = 1;
	Add_Bcd( BufB, Add.B08, 4 );
	BufA[0] = BufB[0];	//���ֽ���ǰ��ɵ��ֽ���ǰ
	BufA[1] = BufB[1];
	BufA[2] = BufB[2];
}


void Number_Add1(unsigned char Offset)	//�¼���¼������1
{
	unsigned char  Inpage, Buf[3];
	
	Inpage = Read_Event_Number( Buf, Offset );
	
	Number_Add1_Direct( Buf );
	
	I2C_Write_Eeprom( ee_recordnumber_page, Inpage, Buf, 3 );
	
	if( (Offset!=ee_openrelayn_inpage)&&(Offset!=ee_closerelayn_inpage)&&(Offset!=ee_powerdownn_inpage) )	//����բ�ڳɹ��Ժ��ϱ�,�����ڽ������ϱ�
	{
		if( (Offset!=ee_clearn_inpage) || (Key_Status==1) )	//˽Կ�����������ϱ�
		{
			Set_Reportdata(EventTable[Offset-ee_clearn_inpage].offset2);	//�����¼��ϱ�
		}
	}	
}


unsigned char Read_Event_Number( unsigned char *BufA, unsigned char Offset )
{
	unsigned char  Inpage;
	
	Inpage = (Offset-ee_clearn_inpage)*3;//�ҵ�ҳ�ڵ�ַ
	if( Offset > ee_errcardn_inpage ) Inpage -= 9;	//�������ܴ������¼�: ���Ƿѹ�������¼���˷Ѽ�¼
	
	if( RdRecTimes( Offset ) == 0 ) //�ж���Чָ��,���Ϊ0��ͷ��ʼ�������
	{
		BufA[0] = 0;//���㵱ǰ����
		BufA[1] = 0;
		BufA[2] = 0;
	}
	else
	{
		I2C_Read_Eeprom( ee_recordnumber_page, Inpage, BufA, 3 );
	}
	
	return Inpage;
}


const uint08 code Max[] =
	{ 	max_timingfreeze_p,max_dayfreeze_p,   max_histroy_p,     max_hourfreeze_p,
		3,2,2,2,2,2,2,
		max_eventrecord_p, max_eventrecord_p ,max_eventrecord_p, max_eventrecord_p,
		max_eventrecord_p, max_eventrecord_p ,max_eventrecord_p, max_eventrecord_p,
		max_eventrecord_p, max_eventrecord_p ,max_eventrecord_p, max_eventrecord_p,2,
		max_eventrecord_p, max_eventrecord_p ,max_eventrecord_p, max_eventrecord_p, max_eventrecord_p,1,
		max_eventrecord_p, max_eventrecord_p, max_eventrecord_p, max_eventrecord_p,max_histroydm_p,max_cleardemand_p
	};
	
unsigned char Read_Event_Point( unsigned char Inpage )
{
	unsigned char Result;
	unsigned char Buf[ee_eventpoint_lenth+2];
	
	ReadE2WithBackup( ee_event_point_page, ee_eventpoint_inpage, Buf, ee_eventpoint_lenth );
	Result = Buf[Inpage];
	if( Result >= Max[Inpage] ) Result = 0;	//���ݷǷ�
	return ( Result );
}


unsigned char Write_Event_Point( unsigned char Inpage )	
{
	unsigned char Buf[ee_eventpoint_lenth+2];
	
	//������¼���Чָ���1����
	ReadE2WithBackup( ee_validrec_page, 0x00, Buf, ee_eventpoint_lenth );
	if( Buf[Inpage] < Max[Inpage] ) 
	{
		Buf[Inpage]++;//��Чָ���1
		WriteE2WithBackup ( ee_validrec_page, 0x00, Buf, ee_eventpoint_lenth );
	}
	
	ReadE2WithBackup( ee_event_point_page, ee_eventpoint_inpage, Buf, ee_eventpoint_lenth );
	if( Buf[Inpage] >= Max[Inpage] ) Buf[Inpage] = 0;	//���ݷǷ�
	Buf[Inpage]++;
	if( Buf[Inpage] >= Max[Inpage] ) Buf[Inpage] = 0;//ѭ����0
	WriteE2WithBackup ( ee_event_point_page, ee_eventpoint_inpage, Buf, ee_eventpoint_lenth );
	return ( Buf[Inpage] );
}

//unsigned char Write_DmEvent_Point( void )	
//{
//	unsigned char Buf[4];
//	
//	//������¼���Чָ���1����
//	ReadE2WithBackup( ee_validrecdemand_page, ee_validrecdemand_inpage, Buf, ee_validrecdemand_lenth );
//	if( Buf[0] < 12 ) 
//	{
//		Buf[0]++;//��Чָ���1
//		WriteE2WithBackup ( ee_validrecdemand_page, ee_validrecdemand_inpage, Buf, ee_validrecdemand_lenth );
//	}
//	
//	ReadE2WithBackup( ee_event_demand_page, ee_event_demand_inpage, Buf, ee_event_demand_lenth );
//	if( Buf[0] >= 12 ) Buf[0] = 0;	//���ݷǷ�
//	Buf[0]++;
//	if( Buf[0] >= 12 ) Buf[0] = 0;//ѭ����0
//	WriteE2WithBackup ( ee_event_demand_page, ee_event_demand_inpage, Buf, ee_event_demand_lenth);
//	return ( Buf[0] );
//}

unsigned char RdRecTimes( unsigned char Inpage )//������Чָ��
{
	unsigned char Buf[ee_eventpoint_lenth+2];
	
	ReadE2WithBackup( ee_validrec_page, 0x00, Buf, ee_eventpoint_lenth );
	return ( Buf[Inpage] );
}

//void ClrRecTimes( unsigned char Inpage, unsigned char mode )//����Чָ��
//{
//	unsigned char Temp1;
//	unsigned char Buf[ee_eventpoint_lenth+2];
//	union B16_B08 Temp16;
//	
//	Temp16.B16 = 0;
////	Temp16.B08[0] = 0;
////	Temp16.B08[1] = 0;
//	
//	if( mode == 1 )//����(�����¼�����)
//	{
//		Temp16.B08[0] = Inpage;
//		Temp16.B08[1] = Inpage+1;
//		Clr_Reportdata(Inpage);	//�嵥���ϱ�״̬�ֺ���������(���¼���¼��)
//	}
//	else if(mode == 0x55)//�¼�ȫ��
//	{
//		{Report_Data[0] = 0;Clr_Reportnum(0);}	//���ϱ�״̬�ֺ���������
//		{Report_Data[1] = 0;Clr_Reportnum(1);}	//���ϱ�״̬�ֺ���������
//		for( Temp1=ee_clearn_inpage+2; Temp1<ee_eventpoint_lenth; Temp1++ )
//		{
//			Clr_Reportdata(Temp1);	//���ϱ�״̬�ֺ���������(���¼���¼��)
//		}
//		
//		Reset_Rep_Timer = 0;

//		Temp16.B08[0] = ee_clearn_inpage+2;
//		Temp16.B08[1] = ee_eventpoint_lenth;
//	}
//	else if(mode == 0xaa)//�������
//	{
//		Temp16.B08[0] = 0;
//		Temp16.B08[1] = ee_eventpoint_lenth;
//	}
//	else if(mode == 0xff)//����ȫ��
//	{
//		Temp16.B08[0] = 0;
//		Temp16.B08[1] = ee_eventpoint_lenth;
//	}
//	else
//	{
//		return;
//	}
//	
//	if( mode != 1 )
//	{		
//		System_Status &= (~0x3e);//���Ѿ��ϱ���־,����������ϱ�
//		
//		Clr_Otherdata(0);	//��Ƿ��忨����
//	}
//	
//	ReadE2WithBackup( ee_validrec_page, 0x00, Buf, ee_eventpoint_lenth );
//	
//	if(mode == 0xaa)//����ȫ��
//	{
//		Temp1 = Buf[ee_clearn_inpage];//������������ָ��
//		memset( Buf, 0x00, ee_eventpoint_lenth );
//		Buf[ee_clearn_inpage] = Temp1;
//	}
//	else if(mode == 0xff)//����ȫ��
//	{
//		memset( Buf, 0x00, ee_eventpoint_lenth );
//	}
//	
//	WriteE2WithBackup( ee_validrec_page, 0x00, Buf, ee_eventpoint_lenth );
//	ReadE2WithBackup( ee_validrec_page, 0x00, Buf, ee_eventpoint_lenth );
//}

void ClrRecTimes( unsigned char Inpage, unsigned char mode )//����Чָ��
{
	unsigned char Temp1;
	unsigned char Buf[ee_eventpoint_lenth+2];
	union B16_B08 Temp16;
	
	if( mode == 1 )//����(�����¼�����)
	{
		Temp16.B08[0] = Inpage;
		Temp16.B08[1] = Inpage+1;
		Clr_Reportdata(Inpage);	//�嵥���ϱ�״̬�ֺ���������(���¼���¼��)
	}
	else if(mode == 0x55)//�¼�ȫ��
	{
		{Report_Data[0] = 0;Clr_Reportnum(0);}	//���ϱ�״̬�ֺ���������
		{Report_Data[1] = 0;Clr_Reportnum(1);}	//���ϱ�״̬�ֺ���������
		for( Temp1=ee_clearn_inpage+2; Temp1<ee_eventpoint_lenth; Temp1++ )
		{
			Clr_Reportdata(Temp1);	//���ϱ�״̬�ֺ���������(���¼���¼��)
		}
		
		Reset_Rep_Timer = 0;

		Temp16.B08[0] = ee_clearn_inpage+2;
		Temp16.B08[1] = ee_eventpoint_lenth;
	}
	else if(mode == 0xaa)//�������
	{
		Temp16.B08[0] = 0;
		Temp16.B08[1] = ee_eventpoint_lenth;
	}
	else if(mode == 0xff)//����ȫ��
	{
		Temp16.B08[0] = 0;
		Temp16.B08[1] = ee_eventpoint_lenth;
	}
	else
	{
		return;
	}
	
	if( mode != 1 )
	{		
		System_Status &= (~0x3e);//���Ѿ��ϱ���־,����������ϱ�
		
		Clr_Otherdata(0);	//��Ƿ��忨����
	}
	
	ReadE2WithBackup( ee_validrec_page, 0x00, Buf, ee_eventpoint_lenth );
	for( Temp1=Temp16.B08[0]; Temp1<Temp16.B08[1]; Temp1++ )
	{
		switch( Temp1 )
		{
			case ee_clearn_inpage: 
				if( mode == 0xaa ) continue; //�����������ʱ��������������¼
				break;
				
			case ee_covern_inpage: 
				Cover.Status = 0;  //���������²������Ǽ�¼
				break;
				
			case ee_maxi_inpage:  
				load_over_flag = 0;	
				Current_Over_Count2 = 0;
				Clr_Otherdata(1); //��������ʱ��(4)
				break;
				
			case ee_batterlow_inpage: 
				low_voltage_flag = 0; 
				Bat_Delay_Timer = 0;
				Clr_Otherdata(2); //���Ƿѹ���ۼ�ʱ��(4)
				break;
				
			case ee_powerdownn_inpage:  //��������¼
				below_60u_flag = 0;	
				Voltage_Low_Count = 0;
				Clr_Otherdata(3); //�������ۼ�ʱ��(4)
				break;
				
			case ee_programn_inpage:  //��̼�¼
				first_prog_flag = 0; 
				break;

//			case ee_relayerrn_inpage: 
//				relay_error = 0;  //���ɿ������������²�����¼
//				break;

			case ee_relayerrn_inpage: 
				Init_Metererrstatus();  //����̵�������״̬
				break;
				
			default: break;
		}

		Buf[Temp1] = 0x00; //�����Ч����
	}
	
	WriteE2WithBackup( ee_validrec_page, 0x00, Buf, ee_eventpoint_lenth );
}




unsigned char Event_Record( unsigned char Inpage )
{
	unsigned char i, result, ptr, offset, len;
	uint16  page;
	
	ptr = (Inpage&0x7F);
	if( ptr < ee_clearn_inpage ) return 0x00;
	
	page   = EventTable[ptr-ee_clearn_inpage].page;
	offset = EventTable[ptr-ee_clearn_inpage].offset;
	len    = EventTable[ptr-ee_clearn_inpage].len;
	
	if( (Inpage&0x80) == 0x00 ) //�¼���¼Ϊ���μ�¼
	{
		if( (ptr == ee_buymoneyn_inpage) || (ptr == ee_dmoneyn_inpage) ) //�����¼���˷Ѽ�¼�ӷ��ӿ�ʼ��������뿪ʼ
		{
			memcpy( Eeprom.Buffer, Sclock.Clockbackup+1, 5 );
		}
		else
		{
			if( ptr != ee_setclockn_inpage ) memcpy( Eeprom.Buffer, Sclock.Clockbackup, 6 ); //����ʱ�Ӳ��ڴ˺������÷���ʱ��
			if( ptr != ee_batterlow_inpage ) Number_Add1(ptr);	//Ƿѹ��¼û���ܴ���
		}

		result = Write_Event_Point(ptr);
	}
	else	//�¼���¼�ֳɿ�ʼ�ͽ�������
	{
		result = Read_Event_Point(ptr);
		I2C_Read_Eeprom( page+result, offset, Eeprom.Buffer, len );	//ʱ��

		if( RdRecTimes( ptr ) ) //����Ϊ0�������¼
		{
			if (ptr == ee_programn_inpage)
			{
				for( i=0; i<4*9; i++ )
				{
					Eeprom.Buffer[49-i] = Eeprom.Buffer[45-i];
				}
				memcpy( Eeprom.Buffer+10, Eeprom.Buffer+50, 4 );	//���ݱ�ʶ(4)
			}
			else
			{
				memcpy( Eeprom.Buffer+6, Sclock.Clockbackup, 6 );
			
				if( (ptr != ee_powererrn_inpage) )	//����Դ�쳣����,������Ҫ��¼����ʱ�̵���
				{
					DltoEepromBuf( len-8 );
				}
			}
		}
	}
	
	I2C_Write_Eeprom( page+result, offset, Eeprom.Buffer, len );
	
	return (result);
}


void Read_Uip_Data(void)	//����ѹ�������ʹ�������(���ڼ�¼)
{
	memcpy( Eeprom.Buffer+20,  Emu_Data.U.B08, 2 );	//��ѹxxx.x
	memcpy( Eeprom.Buffer+22,  Emu_Data.I1.B08, 3 );	//����xxx.xxx
	if( dl_back_flag ) Eeprom.Buffer[24] |= 0x80;
	memcpy( Eeprom.Buffer+25,  Emu_Data.P1.B08, 3 );	//�й�����xx.xxxx
	if( dl_back_flag ) Eeprom.Buffer[27] |= 0x80;
	memcpy( Eeprom.Buffer+28,  Emu_Data.Cosa.B08, 2 );	//��������x.xxx
	if( dl_back_flag ) Eeprom.Buffer[29] |= 0x80;
}


void DltoEepromBuf( unsigned char st )
{
	memcpy( Eeprom.Buffer+st+0,  Fwdl.Fwdl_T.B08, 4 );	//�����ܵ���
	memcpy( Eeprom.Buffer+st+4,  Bkdl.Bkdl_T.B08, 4 );	//�����ܵ���
}


void Note_Relay_Event(void)//��¼�Ͽ��պϼ̵����¼�
{
	I2C_Read_Eeprom( ee_operatorcode_page, ee_operatorcode_inpage, Eeprom.Buffer+6, 4 );//��4�ֽڲ����ߴ���

	DltoEepromBuf( 10 );
	
	if( relay_status_flag == 0 ) //��բ��¼
	{
		if( self_close_flag )
		{
			memset(Eeprom.Buffer+6, 0xff, 4);
			I2C_Write_Eeprom( ee_operatorcode_page, ee_operatorcode_inpage, Eeprom.Buffer+6, 4 );//д4�ֽڲ����ߴ���
		}
		Event_Record(ee_closerelayn_inpage);
	}
	else
	{
		Event_Record(ee_openrelayn_inpage);
	}
	
//	if(PowerDetCnt0 <= 2)//�����ѹ����0.8Un�̵�����������,��ʱ���޷�����̵������,��̵���ʵ��״̬Ĭ��Ϊ����״̬
//	{
//		real_relaystatus_flag = relay_status_flag;
//		comm_relaystatus_flag = real_relaystatus_flag;//ˢ�¼̵���ʵ��״̬(ͨѶbit4ר��)
//	}
}


//���ǺϸǼ�¼/��Դ�쳣��ʼ�ͽ�����¼/���ɿ���������ʼ�ͽ�����¼/������¼/�����¼
void Note_Run_Event( unsigned char Type )	//��¼�����¼���¼
{
	unsigned char Buf[6], Temp08;
	
	if( (Type&0x80) ==  0x00 )	//��ʼ��¼
	{
		memset( Eeprom.Buffer, 0, 42 );

		if( Type == ee_relayerrn_inpage )	//���ɿ����쳣
		{
			if( (Relay_Mode == 0x55) && (relay_status_flag == 0) && (relay_success_flag==0) ) return;	//���ü̵������բ�ɹ��������ɿ���������¼���ϱ�
			
			if( relay_failed_flag ) return;		//�����ڼ�ֻ��һ�μ�¼
			else
			{
				relay_failed_flag = 1;		//�ü̵��������־
				Store_Meterstatus();
			}
			
			Eeprom.Buffer[12] = real_relaystatus_flag;	//�̵���״̬
			DltoEepromBuf( 13 );
		}
		else
		{
			if( Type == ee_maxi_inpage )	//����
			{
				Read_Uip_Data();	//����ѹ�������ʹ�������(���ڼ�¼)
			}
			
			DltoEepromBuf( 12 );
		}
	}
	else
	{
		if((Type&0x7F) == ee_relayerrn_inpage )	//���ɿ����쳣
		{
			if( relay_failed_flag == 0 ) return;		//�����ڼ�ֻ��һ�μ�¼
			
			Temp08 = Read_Event_Point( ee_relayerrn_inpage );
			I2C_Read_Eeprom( ee_relayerr_note_page+Temp08, ee_relayerrnote_inpage+6, Buf, 6 );
			if( ChkBufZero(Buf,6) ) return;			//����ʱ���0��ʾ�Ѿ���¼��
		}
	}
	
	Event_Record( Type );
}


unsigned int Read_Load_Point( unsigned char Inpage )
{
	if( Load.Point[Inpage].B16 >= max_ploadrec_p ) return 0;	//���ݷǷ�
	return ( Load.Point[Inpage].B16 );
}


unsigned int Write_Load_Point( unsigned char Inpage )	
{
	unsigned char n;
	union B16_B08 Addr;
	
	//������Чָ���1����
	if( Load.Validpoint[Inpage].B16 < max_ploadrec_p ) 
	{
		Load.Validpoint[Inpage].B16++;//��Чָ���1
	}
	
	if( Load.Point[Inpage].B16 >= max_ploadrec_p ) Load.Point[Inpage].B16 = 0;	//���ݷǷ�
	Load.Point[Inpage].B16++;
	if( Load.Point[Inpage].B16 >= max_ploadrec_p ) Load.Point[Inpage].B16 = 0;//ѭ����0
	
	Load.Crc16.B16 = Crc_16Bits( Load.Validpoint[0].B08, ee_loadpoint_lenth ); //CRC����
	
	for( n=0; n<3; n++ ) //3ͨ��
	{
		if( RdLoadPara[n][4] )
		{
			memcpy( Addr.B08, RdLoadPara[n]+6, 2 );
			if( (Load.Validpoint[Inpage].B16 == max_ploadrec_p) //��Ч�����ﵽ���ֵ
			 && (Addr.B16 == Load.Point[Inpage].B16) ) //���´ζ�ȡ�ĵ�ַ������ʱ��ָ����һ����ַ��
			{
				if( ++Addr.B16 >= max_ploadrec_p ) Addr.B16 = 0x00;
				memcpy( RdLoadPara[n]+6, Addr.B08, 2 );
			}
		}
	}
	
	return ( Load.Point[Inpage].B16 );
}


//�ú��������ǰʱ�����2000��1��1��0��0�ֵķ�������ֵ
//ÿ����ʼ���������
const uint16 code gui_MonthBeginDay[12] = {0, 31, 59, 90,120, 151, 181, 212, 243, 273, 304, 334};
uint16 CalcAbsDay(unsigned char * psTm)
{//��2000��1��1��Ϊ��׼�����ڵ���������,���ʱ�䲻����2000��֮ǰ(��ʵ2000����00��ʾ,�������϶���ȥ)
	uint08 ucYear = Bcd_To_Hex(psTm[4]);        //��ת����Hex
	uint08 ucMonth = Bcd_To_Hex(psTm[3])-1;      //��ת����Hex
	uint08 ucDay = Bcd_To_Hex(psTm[2]) - 1;      //��ת����Hex
	if((ucYear & 0x03) == 0)                            //�ж��Ƿ���������2��
	{
	    if(ucMonth >= 2)
	        ucDay++;                                    //�ټ�һ��
	}
	else
	{
	    ucDay++;                                        //���������00�����¶������һ��
	}
	//year >> 2,�Ǽ��Ϲ�ȥ����4��������һ��
	return ( (uint16)ucYear*365 + gui_MonthBeginDay[ucMonth] + (ucDay + (ucYear>>2)) );
}
uint16 CalcAbsMin(unsigned char* psTm)
{
	//�Ե���00ʱ00��00��Ϊ��׼��
	uint08 ucMin = Bcd_To_Hex(psTm[0]);       //��ת����Hex
	uint08 ucHour = Bcd_To_Hex(psTm[1]);        //ʱת����Hex
	return ( (uint16)ucHour*60 + ucMin );
}
//[0]��[1]ʱ[2]��[3]��[4]��
unsigned long int CalcTime(unsigned char* psTm)
{
    	uint16 Temp16a,Temp16b;
    	Temp16a = CalcAbsDay(psTm);
    	Temp16b = CalcAbsMin(psTm);
   	return( (unsigned long int)Temp16a*1440 + Temp16b );
}

const uint08 code LoadOffset[4] = { ee_ploadrec1_inpage, ee_ploadrec2_inpage, ee_ploadrec3_inpage, ee_ploadrec4_inpage };	//4������ҳ��ƫ����
const uint08 code LoadLen[4] = { ee_ploadrec1_len, ee_ploadrec2_len, ee_ploadrec3_len, ee_ploadrec4_len };	//4������ҳ���ݳ���
const uint08 code LoadOffset2[4] = { 0, 5, 8, 10 };
//const uint08 code LoadLen2[4] = { 5, 3, 2, 8 };
void Load_Judge(void)		//���ɼ�¼�ж�
{
	unsigned char i;
	uint16 Result;
	unsigned long int Temp32;
	unsigned char TimeRcd;
	
	if( (FLAG_EA == 0) || clock_error ) return;//�������ʱ�Ӵ��󲻴���

	run_loadchk_flag = 0;
	run_timezone_flag = 1;

//	if( (Cmp_Clock( Week_Holiday.Lstarttimer, 5 ) != 0x01) )//���ɼ�¼��ʼʱ��<=��ǰʱ��
	{
		Temp32 = CalcTime( Sclock.Clockbackup+1 );
//		Exchange( Eeprom.Buffer, Week_Holiday.Lstarttimer, 5 );//��ɵ��ֽ���ǰ
//		Temp32 -= CalcTime( Eeprom.Buffer );//���㵱ǰʱ�������ʱ��ķ��Ӳ�

		for(i=0; i<4; i++)
		{
			TimeRcd = ((Week_Holiday.Lspacetimer[i])>>8)&0x00ff;
			if( Temp32%Bcd_To_Hex(TimeRcd) == 0 )
			{
				if( (Mode.Freeze[0]>>i)&B0000_0001 )
				{
					Clear_Wdt();
					Read_Uip_Data();	//����ѹ�������ʹ�������(���ڼ�¼),���ǲ�֧��Ƶ��
					DltoEepromBuf( 30 );	//���������+�������
					memcpy( Eeprom.Buffer+15+LoadOffset2[i], Sclock.Clockbackup+1, 5 );	//����ʱ��
					        				
					Result = Write_Load_Point(i);
					I2C_Write_Eeprom( ee_ploadrec_page+Result, LoadOffset[i], Eeprom.Buffer+15+LoadOffset2[i], LoadLen[i] );
				}
			}
		}
		
		if( Sclock.Clockbackup[1]==0 )
		{
			Wr_Powerdown_Data(0);//д����������ݵ�����ee(ÿСʱдһ��)
			Save_Keep_Timer(1);//��������ۼ�ʱ��
		}
	}
}

uint16 CalStAddr( uint16 RecTimes, uint16 Addr )
{
	if( Addr < (RecTimes-1) )	Addr += max_ploadrec_p;
	Addr -= (RecTimes-1);	//�����¼���ַ
	return Addr;
}


uint16 CalRecTimes( uint16 StAddr, uint16 EndAddr )
{
	uint16 Temp16;
	
	if( StAddr <= EndAddr ) Temp16 = EndAddr-StAddr+1;
	else Temp16 = max_ploadrec_p+EndAddr-StAddr+1;
	
	return Temp16;
}

//------------------------- ���ɼ�¼ ��������� ----------------------------
uint16 FindSpecPos( unsigned char Type, unsigned char *Clock, uint16 RecTimes, uint16 Addr )
{
	unsigned char i, Result, Temp08;
	unsigned char RecTimer[5];
	uint16 StAddr, EndAddr;
	uint16 QStAddr, QEndAddr;

	StAddr = CalStAddr( RecTimes, Addr );
	EndAddr = Addr;	//�����¼���ַ

	QStAddr = StAddr;
	QEndAddr = EndAddr;
	
	//���ȸ�����Ч��¼��������ֺ�ĵ�ַ
	Temp08 = 1;
	for( i=0; i<11; i++ )	//max_ploadrec_p�����ֳ�11��
	{
		//  �������֮һ����ַ
		if( QStAddr <= QEndAddr )//  ��ʼ��ַС�ڵ��ڽ�����ַ��û���Ƶ����
			Addr = (QStAddr+QEndAddr)/2;
		else//  ��ʼ��ַ���ڽ�����ַ�����Ƶ����
			Addr = ((QStAddr+QEndAddr+max_ploadrec_p)/2)%max_ploadrec_p; //��ʼ��ַ������ʼ��������һ������
		
		I2C_Read_Eeprom( ee_ploadrec_page+Addr, LoadOffset[Type], RecTimer, 5 );
		Result = Cmp_Bcd_Inv( Clock, RecTimer, 5 );
		if( Result == 0 ) break; //����ʱ�� == ��ǰʱ��
		
		if( (Temp08 == 0) || (RecTimes == 1) )
		{
			if( Result == 1 ) //����ʱ�� > ��ǰʱ��
			{
				if( Addr == EndAddr )
				{
					Addr = 0x0FFF;
					break;
				}

				Addr += 1;
				if( Addr >= max_ploadrec_p ) Addr -= max_ploadrec_p;
			}
			
			break;
		}
		
		if( RecTimes == 2 )
		{
			Temp08--;	//�������ж�һ��
			if( Result == 1 ) //����ʱ�� > ��ǰʱ��
			{
				Addr += 1;
				if( Addr >= max_ploadrec_p ) Addr -= max_ploadrec_p;
			}
			else if( Result == 2 ) //����ʱ�� < ��ǰʱ��
			{
				if( Addr == StAddr ) break;
				
				if( Addr == 0 ) Addr = max_ploadrec_p;
				Addr -= 1;
			}
		}

		if( Result == 2 ) //����ʱ�� < ��ǰʱ��
		{
			QEndAddr = Addr;
		}
		else //����ʱ�� > ��ǰʱ��
		{
			QStAddr = Addr;
		}

		RecTimes = CalRecTimes( QStAddr, QEndAddr );
	}

	return (Addr|(Result<<14));
}


void ClearPloadRec( unsigned char *Clock )
{
	unsigned char i, Result;
	uint16 StAddr, EndAddr, RecTimes;
	
	memset( RdLoadPara, 0x00, sizeof(RdLoadPara) );	//ֻҪɾ�����ɼ�¼��ȡ�����ɼ�¼�ĺ���֡��ȡ
	
	for( i=0; i<4; i++ ) //�ҵ�����������¼��ʼ��ַ
	{
		RecTimes = Load.Validpoint[i].B16; //��ȡÿ�������Ч��¼��
		if( RecTimes )
		{
			EndAddr = Read_Load_Point(i); //�����¼��
			StAddr = CalStAddr( RecTimes, EndAddr ); //�����¼��

			EndAddr = FindSpecPos( i, Clock, RecTimes, EndAddr );

			Result = (EndAddr>>14);  //���2BitΪ�ȽϽ��.
			EndAddr &= 0x0FFF;  //ȥ�����2Bit�ıȽϽ��.
			if( EndAddr != 0x0FFF )
			{
				RecTimes = CalRecTimes( StAddr, EndAddr );

				if( Result == 2 )
				{
					RecTimes--;
					if( EndAddr == 0 ) EndAddr = max_ploadrec_p;
					EndAddr--;
				}
				
				Load.Validpoint[i].B16 = RecTimes;
				Load.Point[i].B16 = EndAddr;
			}
		}
	}
	
	Load.Crc16.B16 = Crc_16Bits( Load.Validpoint[0].B08, ee_loadpoint_lenth ); //CRC����
}

extern unsigned char Add_Min_Clock( unsigned char *Clock, unsigned char Min );


unsigned char ReadLoadRec( unsigned char Ch, unsigned char *InBuf, unsigned char *OutBuf )
//InBuf[0]: 0x11������; 0x12:������֡ 
//05�������ݱ�ʾ; 0A����ʱ��
//InBuf[2~5]:DI0~DI3
//InBuf[6]: NN��SN
//InBuf[7~10]:mmhhDDMMYY, ����ʱ��ʱ��Ч 
{
	unsigned char i, t, Result, Spec; //00:δ��E2��ȡ; FF:��E2�ж�ȡ
	unsigned char RdBuf[16];
	struct
	{
		union B16_B08  Addr[2];	//[0~3]�����¼��; [4~7]�����¼��
		union B16_B08  RecTimes;
	} RdStr;
	unsigned char offset, len;
	union B16_B08 Temp16;
	unsigned char BkClock[6], FirstVal[5]; //����ʱ��
	unsigned long TmpA;

	Temp16.B08[1] = InBuf[3];
	Temp16.B08[0] = InBuf[2];

	switch( Temp16.B16 )
	{
		case 0x0101:	//A���ѹ
			i = 0; //��ʼ��
			offset = 5;
			len = 2;
			break;

		case 0x0201:	//A�����
			i = 0; //��ʼ��
			offset = 7;
			len = 3;
			break;
		
		case 0x0300:	//���й�����
			i = 1; //��ʼ��
			offset = 5;
			len = 3;
			break;

		case 0x0500:	//�ܹ�������
			i = 2; //��ʼ��
			offset = 5;
			len = 2;
			break;

		case 0x0601:	//�����й�����
			i = 3; //��ʼ��
			offset = 5;
			len = 4;
			break;
			
		case 0x0602:	//�����й�����
			i = 3; //��ʼ��
			offset = 9;
			len = 4;
			break;

		default:
			OutBuf[0] = 0xFF;
			OutBuf[1] = 0x02;	//���ݱ�ʶ��, ����������
			return 2;
	}

	Spec = Bcd_To_Hex((Week_Holiday.Lspacetimer[i]>>8)&0x00ff); //���ɼ�¼���

	RdStr.RecTimes.B16 = Load.Validpoint[i].B16; //��ȡÿ�������Ч��¼��
	if( RdStr.RecTimes.B16 == 0x00 ) 	return 0x00;
	
	//Addr: ��Ӧ��Ŀ��ַ [0~3]: �����¼; [4~7]: �����¼
	//RecTimes: ��Ӧ���ʣ����Ч��¼��
	//RdLoadPara: ���ݱ�ʶ(4)+ʣ���¼����(1)+����֡���(1)+��ʼ��ַ(4)+ʣ����Ч��¼��(2)+����ʱ��(5)+��һ�ε���(4)
	if( InBuf[0] == 0x12 ) //��ȡ����֡
	{
		if( RdLoadPara[Ch][4] == 0 ) return 0x00; //�޺���֡

		Result = Cmp_Data( RdLoadPara[Ch], InBuf+2, 4 );
		if( Result != 3 ) return 0x00; //���ݱ�ʶ��

		if( InBuf[6] != RdLoadPara[Ch][5] ) return 0x00; //֡��Ŵ���

		memcpy( RdStr.Addr[0].B08, RdLoadPara[Ch]+6, 6 );
		memcpy( BkClock+1, RdLoadPara[Ch]+12, 5 );

		t = 0x00;
	}
	else if( InBuf[0] == 0x11 ) //�����ɼ�¼
	{
		memset( RdLoadPara[Ch], 0x00, sizeof(RdLoadPara[Ch]) ); //ɾ����ʷ������¼, ���¼�¼

		RdLoadPara[Ch][4] = Bcd_To_Hex(InBuf[6]);	//�����ļ�¼����
		if( RdLoadPara[Ch][4] == 0x00 ) return 0x00; //��������������

		RdStr.Addr[1].B16 = Read_Load_Point(i); //�����¼��
		Temp16.B16 = CalStAddr( RdStr.RecTimes.B16, RdStr.Addr[1].B16 );
		I2C_Read_Eeprom( ee_ploadrec_page+Temp16.B16, LoadOffset[i], FirstVal, 5 );

		//�Ӹ���ʱ�俪ʼ����һ�����ɼ�¼ʱ��
		memcpy( BkClock+1, InBuf+7, 5 );
		Result = CalcTime(BkClock+1)%Spec;
		if( Result )
		{
			Result = Spec-Result;
			Add_Min_Clock( BkClock, Result );
		}

		if( Cmp_Bcd_Inv( FirstVal, BkClock+1, 5 ) == 1 ) //�����¼ʱ����ڸ���ʱ��(0-A=B; 1:A>B; 2:A<B)
		{
			TmpA = CalcTime(FirstVal)/Spec-CalcTime(BkClock+1)/Spec;
			
			if( TmpA >= RdLoadPara[Ch][4] ) return 0x00;
			if( (TmpA==0) && (RdLoadPara[Ch][4]==1) ) return 0x00;
			
			RdLoadPara[Ch][4] -= TmpA;	//�۳�����Ŀ���
			
			//�������¼��ʼ����һ�����ɼ�¼ʱ��
			memcpy( BkClock+1, FirstVal, 5 );
			Result = CalcTime(BkClock+1)%Spec;
			if( Result )
			{
				Result = Spec-Result;
				Add_Min_Clock( BkClock, Result );
			}
		}
		
		if( Cmp_Bcd_Inv(BkClock+1, Sclock.Clockbackup+1, 5) == 1 ) return 0x00; //����ʱ����ڵ�ǰʱ�䣬�޼�¼
		if( FLAG_EA == 0 ) return 0x00;	//���粻����
		
		RdStr.Addr[0].B16 = FindSpecPos( i, BkClock+1, RdStr.RecTimes.B16, RdStr.Addr[1].B16 );
		Result = (RdStr.Addr[0].B16>>14);
		RdStr.Addr[0].B16 &= 0x0FFF; //ȥ�����2Bit�ıȽϽ��.
		if( RdStr.Addr[0].B16 == 0x0FFF )
		{
			RdStr.RecTimes.B16 = 0x00; //����ʱ��������м�¼ʱ�䣬��¼��Ч��������
		}
		else
		{
			RdStr.RecTimes.B16 = CalRecTimes( RdStr.Addr[0].B16, RdStr.Addr[1].B16 );
		}
		
		if( i == 3 ) //��������ʱ����Ҫȷ����һ�εĵ���������䶳����©��
		{
			if( Result == 1 ) //����ʱ��������м�¼ʱ��
				Temp16.B16 = RdStr.Addr[1].B16;
			else //����ʱ��С�ڷ��صĵ�ַ��ָ��ļ�¼ʱ��
			{
				Temp16.B16 = RdStr.Addr[0].B16;
				if( Temp16.B16 == 0 ) Temp16.B16 = max_ploadrec_p;
				Temp16.B16--;
			}
				
			I2C_Read_Eeprom( ee_ploadrec_page+Temp16.B16, LoadOffset[3]+offset, RdLoadPara[Ch]+17, 4 ); //����һ�ε���					
		}
		
		t = 5;
		memcpy( OutBuf+1, BkClock+1, 5 );
	}
	else return 0x00;

	if( RdStr.RecTimes.B16 ) 	//���ڼ�¼ʱ��ȷ����E2�ж�ȡ
		memset( RdBuf, 0x00, 5 );
	else //û�м�¼ʱ, ȷ������ȡE2����
		memset( RdBuf, 0x99, 5 );
	
	OutBuf[0] = 0x00;
	while( RdLoadPara[Ch][4] )
	{
		Clear_Wdt();
		
		if( FLAG_EA == 0 ) return 0x00;	//���粻����
		
		if( Cmp_Bcd_Inv(BkClock+1, Sclock.Clockbackup+1, 5) == 1 )
		{
			RdLoadPara[Ch][4] = 0;
			break;
		}

		//----- Ԥ����ʱ�������м�¼�ܷ���ڴ�֡��------
		if( InBuf[0] == 0x11 ) Result = 4;	//���ݱ�ʶ
		else Result = 5; //���ݱ�ʶ+���

		if( t+len > 200-Result )
		{
			//������ǰ��Сʱ��ͳ������ڼ�����¼, ��ȡ����֡Ҫʹ��, �ﵽ���֡����, ������ѯ.
			//RdLoadPara: ���ݱ�ʶ(4)+ʣ���¼����(1)+����֡���(1)+��ʼ��ַ(4)+ʣ����Ч��¼��(2)+����ʱ��(5)+��һ�ε���(4)
			memcpy( RdLoadPara[Ch]+0, InBuf+2, 4 );
			RdLoadPara[Ch][5]++;	//����֡���
			memcpy( RdLoadPara[Ch]+ 6, RdStr.Addr[0].B08, 6 ); //��ַ(4)+ʣ���¼��(2)
			memcpy( RdLoadPara[Ch]+12, BkClock+1, 5 ); //����ʱ��
			break;
		}

		Result = Cmp_Bcd_Inv( RdBuf, BkClock+1, 5 ); //0-A=B; 1:A>B; 2:A<B
		if( Result == 2 ) //E2������������Ҫ���
		{
			while( RdStr.RecTimes.B16 )
			{
				Clear_Wdt();
				if( FLAG_EA == 0 ) return 0x00;	//���粻����
				I2C_Read_Eeprom( ee_ploadrec_page+RdStr.Addr[0].B16, LoadOffset[i], RdBuf, LoadLen[i] );

				Result = Cmp_Bcd_Inv( RdBuf, BkClock+1, 5 ); //0-A=B; 1:A>B; 2:A<B
				if( Result != 1 ) //��¼ʱ��С�ڵ����ڴ�ʱ��
				{
					RdStr.RecTimes.B16--;
					if( ++RdStr.Addr[0].B16 >= max_ploadrec_p ) RdStr.Addr[0].B16 = 0x00;
				}
				
				if( Result != 2 ) break; //��¼ʱ����ڵ����ڴ�ʱ�䣬������ѯ
				if( i == 3 ) memcpy( RdLoadPara[Ch]+17, RdBuf+offset, 4 ); //����һ�ε���
			}
		}
		
		if( Result == 0 )
		{
			memcpy( OutBuf+1+t, RdBuf+offset, len );
			if( i == 3 ) memcpy( RdLoadPara[Ch]+17, RdBuf+offset, 4 ); //����һ�ε���
		}
		else //������
		{
			if( i != 3 ) memset( OutBuf+1+t, 0xFF, len );
			else memcpy( OutBuf+1+t, RdLoadPara[Ch]+17, 4 ); //����һ�ε���
		}
		
		t += len;
		RdLoadPara[Ch][4]--;
		Add_Min_Clock( BkClock, Spec );
	}

	if( InBuf[0] == 0x12 ) OutBuf[++t] = InBuf[6];
	
	return (++t);
}

