//RTC function
#include "ex_var.h"
#include "ex_func.h"
#include <string.h>
//#include <intrins.h>

/*============================================================================*
	������unsigned char Add_Bcd( unsigned char  *sr, unsigned char  *de, unsigned char len )
	���ܣ�	BCD��ӷ���
	
*============================================================================*/	

unsigned char Add_Bcd( unsigned char  *sr, unsigned char  *de, unsigned char len )
{
  unsigned char  i = 0;
	unsigned int sum = 0;
	unsigned char temp  = 0;
	unsigned char data1 = 0;
	unsigned char data2 = 0;
	
	while(len--)
	{
		data1 = sr[i];
		data2 = de[i];
		sum += (unsigned int)data1 + (unsigned int) data2;
		
		temp = (data1&0xF0) + (data2&0xF0);
		if((temp != (sum&0xF0)) || ((sum&0x0F) > 0x09))
		{
			sum += 0x06;	
		}
		if(sum >= 0xa0)
		{
			sum += 0x60;	
		}
		sr[i] = (unsigned char)sum;
		sum >>= 8;
		i++;
	}
  return(sum);	
}


unsigned char Lib_BcdArray_Inc(unsigned char *sr,unsigned char len)
{
	unsigned char byte = 1;
	while(len--)
	{
		if(byte == 1)
		{
			if(*sr == 0x99)
			{
				*sr = 0x00;
			}
			else
		{
				byte = 0;
				if((*sr&0x0f) == 0x09)
				{
					*sr = *sr + 0x10;
					*sr&=0xF0;
				}
				else
				{
					*sr = *sr + 0x01;	
				}
			}
		}
		sr++;
	}
	return(byte);	
}
/*============================================================================*
	������unsigned char Add_Bcd( unsigned char  *sr, unsigned char  *de, unsigned char len )
	���ܣ�	BCD�������
	
*============================================================================*/		
void Sub_Bcd(unsigned char  *sr,unsigned char  *de,unsigned char len)
{       
  unsigned char i = 0;	
	unsigned char data[10];

	memmove(data,de,len);
	for(i=0;i<len;i++)
	{
		data[i] = 0x99 - data[i];	
	}
	Lib_BcdArray_Inc(data,len);
	
	Add_Bcd(sr,data,len);

}
/*============================================================================*
	������	void Hex2Bcd( unsigned char *Hexop, unsigned char *BCDopt, unsigned char Len );
	���ܣ�	���ֽ�HEXת��BCD
	������	*Hexop  HEX ��ʼֵΪ��ת���ĸ��ֽڵ�ַ�� 
			*BCDopt  BCD ��ʼֵΪ��ת���ĸ��ֽڵ�ַ�� 
			Len  ���ȣ����4���ֽ�
  ����ֵ:	��
*============================================================================*/	
	
void Hex2Bcd( unsigned char  *sr, unsigned char  *de, unsigned char len )
{
        unsigned char i = 0;
	unsigned char num = 0;
	unsigned char cbit = 0;
	unsigned char flag = 0;
	unsigned short  temp = 0;
	unsigned char hex[8] = {0,};
	unsigned char bcd[8] = {0,};
	
	memmove(hex,sr,len);
	for(num=0;num<len*8;num++)
	{
		cbit = 0;
		for(i=0;i<len;i++)
		{
			flag = 0;
			if((hex[i]&0x80) == 0x80)
			{
				flag = 1;
			}
			hex[i] = hex[i] << 1;
			if(cbit == 1)
			{
				hex[i] |= 0x01;
			}
			cbit = flag;
		}
		for(i=0;i<8;i++)
		{
			flag = 0;
			if((bcd[i]&0x80) == 0x80)
			{
				flag = 1;	
			}
			bcd[i] = bcd[i] << 1;
			if(cbit == 1)
			{
				bcd[i] |= 0x01;
			}
			cbit = flag;
		}
		if(num != (len*8-1))
		{
			for(i=0;i<8;i++)
			{			
				temp = (bcd[i]&0x0F) + 0x03;
				if(temp>0x07)
				{
					bcd[i] += 0x03;
				}
				temp = (bcd[i]&0xF0) + 0x30;
				if(temp>0x70)
				{
					bcd[i]+=0x30;
				}		
			}
		}
	}
	memmove(de,bcd,len);
}
/*==========================================================================*/
/*============================================================================*
	������	void Bcd2Hex( uint08 *BCDopt, uint08 *HEXopt, uint08 Len );
	���ܣ�	���ֽ�BCDת��HEX
	������	*BCDopt  BCD ��ʼֵΪ��ת���ĸ��ֽڵ�ַ�� 
			*HEXopt  HEX ��ʼֵΪ��ת���ĸ��ֽڵ�ַ��
			Len  ���ȣ����4���ֽ�
  ����ֵ:	��
*============================================================================*/	
void Bcd2Hex( unsigned char  *sr, unsigned char  *dr, unsigned char len )
{       
	unsigned char i = 0;
	unsigned char num = 0;
	unsigned char cbit = 0;
	unsigned char flag = 0;
	unsigned short temp = 0;
	unsigned char hex[8] = {0,};
	unsigned char bcd[8] = {0,};
	
	memmove(bcd,sr,len);
	for(num=0;num<8*8;num++)
	{
		cbit = 0;
		for(i=len;i>0;i--)
		{
			flag = 0;
			if((bcd[i-1]&0x01) == 0x01)
			{
				flag = 1;
			}
			bcd[i-1] = bcd[i-1] >> 1;
			if(cbit == 1)
			{
				bcd[i-1] |= 0x80;
			}
			cbit = flag;
			}
		for(i=8;i>0;i--)
		{
			flag = 0;
			if((hex[i-1]&0x01) == 0x01)
			{
				flag = 1;
			}
			hex[i-1] = hex[i-1] >> 1;
			if(cbit == 1)
			{
				hex[i-1] |= 0x80;
			}
			cbit = flag;
		}
		if(num != (len*8-1))
		{
			for(i=0;i<8;i++)
			{			
				temp = bcd[i]&0x0F;
				if(temp>0x07)	
				{
					bcd[i] -= 0x03;
				}
				temp = bcd[i]&0xF0;
				if(temp>0x70)
				{
					bcd[i] -= 0x30;
				}
			}		
		}
	}
	memmove(dr,hex,len);
}
unsigned char Hex_To_Bcd( unsigned char value )
{
	return ( ((value/10)<<4)+(value%10) );
}

unsigned char Bcd_To_Hex( unsigned char value )
{
	return ( ((value&0xF0)>>4)*10+(value&0x0F) );
}

//============================================================================
unsigned char ChkBufZero( unsigned char *Buf,unsigned char Len )
{
	unsigned char i;
	
	for( i=0; i<Len; i++ )
	{
		if( Buf[i] )	return(1);	//��0
	}
	
	return(0);	//ȫ0
}


//���ֽ���ǰ�����ݱȽ�
unsigned char Cmp_Data( unsigned char *BufA, unsigned char *BufB, unsigned char Len )
{
	unsigned char i;

	for( i=0; i<Len; i++ )
	{
		if( BufA[i] > BufB[i] )
		{
			return 1; //A>B
		}
		else
		{
			if( BufA[i] < BufB[i] )
			{
				return 2; //A<B
			}
		}
	}

	return 3; //A=B
}
//??????????????
unsigned char Cmp_DataLH( unsigned char *BufA, unsigned char *BufB, unsigned char Len )
{
	unsigned char i;

	for( i=0; i<Len; i++ )
	{
		if( BufA[Len-1-i] > BufB[i] )
		{
			return 1; //A>B
		}
		else
		{
			if( BufA[Len-1-i] < BufB[i] )
			{
				return 2; //A<B
			}
		}
	}

	return 3; //A=B
}

//���ֽ���ǰ�����ݱȽ�
unsigned char Cmp_Bcd_Inv( uint08 *BcdA, uint08 *BcdB, uint08 Len )
//return: 0-A=B; 1:A>B; 2:A<B
{
	unsigned char i;

	for( i=0; i<Len; i++ )
	{
		if( BcdA[Len-1-i] > BcdB[Len-1-i] )
		{
			return 1; //A > B
		}
		else
		{
			if( BcdA[Len-1-i] < BcdB[Len-1-i] )
			{
				return 2; //A < B
			}
		}
	}

	return 0; //A==B
}

//============================================================================
//����Ϊ��ʼ�ֽڵ�ʱ�ӱȽϳ���
unsigned char Cmp_Clock( unsigned char *Buf, unsigned char Len )
{
	unsigned char i;
	
	for( i=0; i<Len; i++ )
	{
		if( Buf[ i ] > Sclock.Clockbackup[ Len-i ] )
		{
			return 1; //���ڵ�ǰʱ��
		}
		else
		{
			if( Buf[ i ] < Sclock.Clockbackup[ Len-i ] )
			{
				return 2; //С�ڵ�ǰʱ��
			}
		}
	}

	return 3; //���
}


uint08 Judge_Clock( uint08 Start, uint08 *Ck, uint08 Len )//�����м�
{
	unsigned char  i,j;
	uint08 code Max[]={ 0x59,0x59,0x23,0x6,0x31,0x12,0x99 }, code Min[]={ 0x00,0x00,0x00,0x00,0x01,0x01,0x00 };

	for( i=0; i<(Len&0x07f); i++ )
	{
		j=i+Start;
		if((j>=3)&&(Len&0x80))	j++;
		
		if( (Ck[i] > Max[j]) || (Ck[i] < Min[j]) ) return 1;//�Ϸ��Լ�����
		if( IsBCD(Ck[i]) ) return 1;	//BCD�������		   
	}
	return 0;//��ȷ
}


unsigned char JudgeClockNoWeek( uint08 Start, uint08 *Ck, uint08 Len )
{
	return(Judge_Clock( Start, Ck, Len+0x80 ));
}


unsigned char Read_Rtc( unsigned char *RTC_Buf )
{
	unsigned char i, n, Result;
	unsigned char BufA[7], BufB[7];

	for( n=0; n<3; n++ )
	{
		Result  = RTC->LOAD;
		RTC_WaitForSynchro();
		BufA[0] = RTC->SEC;
		BufA[1] = RTC->MIN;
		BufA[2] = RTC->HOUR;
		BufA[3] = RTC->WEEK;
		BufA[4] = RTC->DAY;
		BufA[5] = RTC->MON;
		BufA[6] = RTC->YEAR;
    
		sub_delay(100);
		
//		Result  = RTC->LOAD;
//		RTC_WaitForSynchro();
		BufB[0] = RTC->SEC;
		BufB[1] = RTC->MIN;
		BufB[2] = RTC->HOUR;
		BufB[3] = RTC->WEEK;
		BufB[4] = RTC->DAY;
		BufB[5] = RTC->MON;
		BufB[6] = RTC->YEAR;

		for( i=0; i<7; i++ ) //�ȶ�����ʱ���Ƿ�һ��
		{
			if( BufA[i] != BufB[i] ) break;
		}

		if( i == 7 ) //����һ��, ��ʾ��ȡ�ɹ�
		{
			Result = Judge_Clock( 0, BufA, 7 ); //�ж�ʱ���Ƿ�Ϸ�
			if( Result == 0 ) //ʱ�ӺϷ�
			{
				for( i=0; i<7; i++ )
				{
					RTC_Buf[i] = BufA[i];
				}
			
				return 0;
			}
		}
	}
	
	for( i=0; i<7; i++ )
	{
		RTC_Buf[i] = BufA[i];//��ʹ����ҲҪ����ʱ��
	}
	return 1;
}


unsigned char Set_Rtc( unsigned char Start, unsigned char *Ck, unsigned char Len )
{
	unsigned char i, n, Result;
	unsigned char BufA[7],BufB[7];
	
	if( Start || (Len!=7) ) return 1; //��ڲ����Ƿ�

	Result = Judge_Clock( Start, Ck, Len );
	if( Result == 1 )
	{
		return 1;
	}

	if( Len < 7 )
	{
		Result = Read_Rtc( BufA );
		if( Result == 1 )
		{
			return 1;
		}
	}

	for( i=0; i<Len; i++ )
	{
		BufA[ Start+i ] = Ck[i];
	}
  
	//RTC_WriteProtection(DISABLE);
	  
	for( n=0; n<3; n++ )
	{
		RTC->PWD = RTCPWD_KEY;
    RTC->CE = RTCCE_SETKEY; 
		
		RTC->SEC 	= Ck[0];
		RTC->MIN	= Ck[1];
		RTC->HOUR	= Ck[2];
		RTC->WEEK	= Ck[3];
		RTC->DAY	= Ck[4];
		RTC->MON	= Ck[5];
		RTC->YEAR	= Ck[6];
	  
		RTC->PWD = RTCPWD_KEY;
    RTC->CE = RTCCE_CLRKEY; 
//		RTCLOAD = 0x55;
    RTC_WaitForSynchro();
//		I2c_Wait_5ms();//RTC��һ��5�ĵ��˲����ڴ�֮ǰ��ʱ�ӻ�ȡ������
////		if( (RTCSTAT&1) == 1 )  //���سɹ�
		{
			Result = Read_Rtc( BufB );
			if( Result == 0 ) //��ȡ�ɹ�
			{
				for( i=0; i<7; i++ )
				{
					if( Ck[i] != BufB[i] ) break;
				}

				if( i == 7 ) //��ǰֵ������ֵ���, ���óɹ�
				{
//					RTC_WriteProtection(ENABLE);
					return 0; //���óɹ�
				}
			}
		}
	}
//  RTC_WriteProtection(ENABLE);
	return 1; //����ʧ��
}


void Syn_Clock( void )	//ͬ�����ͱ���ʱ��
{
	Sclock.Clockbackup[0] = Sclock.Clockmain[0];//�����м��ŵ����
	Sclock.Clockbackup[1] = Sclock.Clockmain[1];
	Sclock.Clockbackup[2] = Sclock.Clockmain[2];
	Sclock.Clockbackup[3] = Sclock.Clockmain[4];
	Sclock.Clockbackup[4] = Sclock.Clockmain[5];
	Sclock.Clockbackup[5] = Sclock.Clockmain[6];
	Sclock.Clockbackup[6] = Sclock.Clockmain[3];
}


void Read_Sysclock( void )
{
	unsigned char Result,Buf[10];

	run_readrtc_flag = 0;	//��ˢ��ʱ�ӱ�־ 
	
	Result = Read_Rtc( Sclock.Clockmain );//��ʱ��
	if( Result == 1 )
	{
		I2c_Wait_5ms(); 	//��ʱ
		Result = Read_Rtc( Sclock.Clockmain );//�ٶ�ʱ��
		if( Result == 1 )
		{
			if( clock_error == 0 ) 
			{
				Set_Reportdata(0*8+7);//��ʱ�Ӵ����ϱ���־
				clock_error = 1;//��ʱ�Ӵ����־
				memcpy( Buf, Sclock.Clockbackup, 3 );
				Buf[3] = Sclock.Clockbackup[6];
				memcpy( Buf+4, Sclock.Clockbackup+3, 3 );
				Set_Rtc( 0, Buf, 7 );//��ram����ȷ��ʱ��д��rtc
				return;
			}
		}
	}
	if( Result == 0 ) clock_error = 0;//��ʱ�Ӵ����־
		
	if(FLAG_EA)
	{	
		if ( Sclock.Clockbackup[0] != Sclock.Clockmain[0] )//��ı�
		{
						
			if( Keyswitch_Valid != 0x55 )	//�����޴��㣬1sѰ��һ��
			{
				if( Find_Card != 0xaa ) rf_card_flag = 1;	//ÿ��Ѱ��һ��
			}
			else
			{
				if( Find_Card == 0x55 ) rf_card_flag = 1;	//ÿ��Ѱ��һ��
			}
			
			if( Sclock.Clockmain[0]&B0000_0001 ) read_uip_flag1 =1;		//ÿ�뽻���ȡI1,I2,U (80ms)
			if( Sclock.Clockmain[0] == 0x10 ) fwdl_veri_flag = 1;		//�������У��
			if( Sclock.Clockmain[0] == 0x15 ) bkdl_veri_flag = 1;		//�������У��
			if( Sclock.Clockmain[0] == 0x20 ) mode_veri_flag = 1;		//����ģʽ��У��
			if( Sclock.Clockmain[0] == 0x25 ) reg_veri_flag = 1;		//�Ĵ���У��
			if( Sclock.Clockmain[0] == 0x30 ) addr_veri_flag = 1;		//ͨѶ��ַУ��
			if( Sclock.Clockmain[0] == 0x35 ) disppm_veri_flag = 1;		//��ʾ����У��
			if( Sclock.Clockmain[0] == 0x40 ) dispitem_veri_flag = 1;	//��ʾ��ĿУ��
			
//			if( Sclock.Clockmain[0] == 0x42) init_7051_flag = 1;		//��ʼ���Ĵ���
			if( Sclock.Clockmain[0] == 0x46) check_7051_flag1 = 1;		//У��У��Ĵ���
			if( Sclock.Clockmain[0] == 0x55) temperpara_veri_flag = 1;	//�¶�ϵ��У��
			if( Sclock.Clockmain[0] == 0x02 ) Dm_process_flag= 1;//ÿ���ӵĵ�2���������
			
			
			//�ر��ز���λ���¼��ϱ�����
			//if( progenable && (F_Plcevtout_Sta == 0x55) ) EVENOUTON;//�¼��ϱ�
			//else if( progenable && (F_Plcevtout_Sta == 0xaa) ) EVENOUTOFF;//���¼��ϱ������
			//else
			//{
//				if( ChkBufZero( Report_Data, 5 ) )
//				{
//					EVENOUTON;		//�¼��ϱ�
//				}
//				else
//				{
//					EVENOUTOFF;		//���¼��ϱ������
//				}
//			}
		if( PLC_RST1_Timer == 0 )
		  PLC_RST1;	
			//�����֤ʱЧ
			if( Identitytimer.B32 != 0 )
			{
				Identitytimer.B32--;
	
				if( Identitytimer.B32 == 0 )
				{
					Clear_Rand_Gene();
					FailKey();	//������Կʧ�ܴ���
				}	
			}
			
			//��բ��ʱʱ��
			if( Opendelaytimer.B32 != 0 ) //�̵�����բ��ʱ
			{
				Opendelaytimer.B32--;
				if( Opendelaytimer.B32 == 0 ) run_alarm_flag = 1;	//��ʱ���ñ�־
			}
						
			Judge_Relay_Delay();	//�������ʱ��բ�ж�
		 	if( relay_delay_flag )
		 	{
		 		CurrentTimer.B32--; //�������ʱ��բ��ʱ����1
		 		if( CurrentTimer.B32 == 0 ) run_alarm_flag = 1;	//��ʱ���ñ�־
		 	}
			
			//������֤ʱЧ
			if( IR_EnTimer )
			{
				IR_EnTimer--;
				if( IR_EnTimer == 0 ) identity_ir_flag = 0;	//����������־
			}
			
			//�ϱ��Զ���λʱ��
			if( Reset_Rep_Timer )
			{
				Reset_Rep_Timer--;	//�ϱ���λ��ʱ����1
				
				if( Reset_Rep_Timer == 0 )
				{
					Xor_Data( Report_Data, Report_Data2, 5 );//����30min���µ��ϱ�
					for( Result=0; Result<5; Result++ ) Clr_Reportnum(Result);//���������ϱ�����
				}
			}
			//��ʱ8Mʱ�ӵ�У����(����uartר��)
			if( Adj_Clk_Timer_S )
			{
				Adj_Clk_Timer_S--;	//��ʱ8Mʱ�ӵ�У��ʱ����1s
				if( Adj_Clk_Timer_S == 0 )  Clk_Adj_Proc();//�����ⲿ��32K�����������ڲ�8M��ƫ��
			}
			
//			//��չ����
//			if( (Key_Status==1) && (esam_error==0) )
//			{
//				progenable = 0;	//��̽�ֹ	
//			}
			Lcdled_Control();	//�������	
//			if( FDETFLAG )	//xtlf����ͣ��
			{
//				FDETFLAG = 0;
				Result = I2C_Read_Eeprom( ee_xtlfstop_time_page, ee_xtlfstoptime_inpage, Buf, ee_xtlfstoptime_lenth );
				if( (Result || (ChkBufZero(Buf,ee_xtlfstoptime_lenth)==0)) && (clock_error==0) ) //�ж��Ƿ�����¼
				{
					//��¼����ͣ��ķ���ʱ��
					I2C_Write_Eeprom( ee_xtlfstop_time_page, ee_xtlfstoptime_inpage, Sclock.Clockbackup, ee_xtlfstoptime_lenth );
				}
			}
			
			//������ʱ��ʱ����1
			if( Cover_Space_Timer != 0 ) Cover_Space_Timer--;		 
		}
		if ( Sclock.Clockbackup[0] > Sclock.Clockmain[0] )//���Ӹı�
		{
			if( UpdateMoneytimer != 0 ) 	//����ESAMǮ����ʱ����1
			{
				UpdateMoneytimer--;
				
				if( UpdateMoneytimer == default_updatemoneytimer )
				{
					UpdateMoneytimer = 0;
				}
			}
		 	
		 	if( Sclock.Clockmain[1]&B0000_0001 )
			{
				GetKeyStatus();//����ǰ��Կ״̬(2minһ��)
			}
		 	
		 	if( relay_error || (Sclock.Clockmain[1]==0x30) ) relay_direct_flag = 1;
		 	
 		 	if( load_over_flag ) 
 		 	{
 		 		Keeptime.Times[1].B32++; 	//���������ܳ���ʱ���1����
 		 	}
 		 	if( low_voltage_flag )  
 		 	{
 		 		Keeptime.Times[2].B32++;	//���Ƿѹ����ʱ���1����
 		 	}
 		 	if( below_60u_flag )  
 		 	{
 		 		Keeptime.Times[3].B32++;	//�����ܳ���ʱ���1����
 		 	}
			
			//��ʱ8Mʱ�ӵ�У����
			if( Sclock.Clockmain[1]==0x00 )
			{
				if( Adj_Clk_Timer )
				{
					Adj_Clk_Timer--;	//��ʱ8Mʱ�ӵ�У��ʱ����1h
					if( Adj_Clk_Timer == 0 )  Clk_Adj_Proc();//�����ⲿ��32K�����������ڲ�8M��ƫ��
				}
			}
		 	
		 	//��չ����
		 	if(( Sclock.Clockmain[1]== 0x00)&&( Sclock.Clockmain[2]== 0x20))	//ÿ��20:00һ��
			{	
				if((Mode.Plc==0x0a)||(Mode.Plc==0xaa))
				{
					Resetworking_Timer = resetworking_timer_delay;//������λ����
					PLC_RST0;		//��λ�ز�(�����)
				}
			}

			if( (!BDMODE) && (SELFCLOSE == 0x55) )	//�Ա��繦��
			{
				if(relay_commu_open_flag && (Opendelaytimer.B32 == 0x00))//�յ���բ������Ѿ���բ
				{
					Result = ReadE2WithBackup( ee_hearttime_page, ee_hearttime_inpage, Buf, ee_hearttime_lenth );
					if( Result ) Buf[0] = default_hearttime;
						
					Self_Timer3++;
					if(Self_Timer3 >= (Buf[0] + 5)) Self_Cnt = 0;
					
					if( Self_Timer24 ) 
					{
						Self_Timer24--;
						if( Self_Timer24 == 0x00 ) Self_Cnt = 0;
					}
	
					if((self_close_flag == 0) && (Self_Timer3 >= (3*Buf[0] + 5))) //����3��û���յ��Լ�֡
					{
						self_close_flag = 1;
						Self_Timer24 = 24*60;		//�����Ա���1����ʱ
						Store_Meterstatus();
					}
				}
			}
			else self_close_flag = 0;
		}
		if ( Sclock.Clockbackup[2] > Sclock.Clockmain[2] )//Сʱ�ı�
		{		
		  Festival_holiday_Judge(); //�����ڼ����ж�		
	  }
	}
	
	if ( Sclock.Clockbackup[0] > Sclock.Clockmain[0] )//���Ӹı�(��ֹ����㸽�����ֻ�������)
	{
		run_freeze_flag = 1;	//����ͽ����ж�
		
		if(( Sclock.Clockmain[1]== 0x00)&&( Sclock.Clockmain[2]== 0x00))	//ÿ��00:00һ��
		{
			Clr_Zero_Data();	//��������������
		//	Find_Card = 0x00;	//����Ѱ��ģʽ
		}	
	}	
		
	Syn_Clock();//ͬ�����ͱ���ʱ��	 
}
