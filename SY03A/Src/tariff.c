#include "ex_func.h"
#include "ex_var.h"
//#include <intrins.h>
#include <string.h>
#include <absacc.h>



void RefreshPrice( void )
{
	run_price_flag = 1;
	Price.Crc16a.B16 += 1;
}


//offset=0ʱ��,offset=1ʱ��,offset=2����,offset=3����,offset=4����ʱ��
void Changecontrol_Process(unsigned char offset)//��ת�����ֽ�,������ʱ��ͽ���Լ������
{
	unsigned char i;
	
	Clear_Wdt();
	
	if( offset < 2 )
	{
		//��ת������
		I2C_Read_Eeprom44( ee_changecontrol_page, Eeprom.Buffer, ee_changecontrol_lenth);//�����������ֽ�
		if (Eeprom.Buffer[offset] == 0x55)
		{
			Eeprom.Buffer[offset]=0xff;
		}
		else
		{	
			Eeprom.Buffer[offset]=0x55;
		}
		
		I2C_Write_Eeprom44( ee_changecontrol_page, Eeprom.Buffer, ee_changecontrol_lenth);
	}
	else if( offset == 2 )
	{
		I2C_Read_Eeprom ( ee_tariffprice2_page, 0, Price.Tariff[0].B08, ee_tariffprice_lenth);//�����ݷ��ʵ�۸��ǵ�ǰ
		I2C_Write_Eeprom( ee_tariffprice1_page, 0, Price.Tariff[0].B08, ee_tariffprice_lenth);
	}
	else if( offset == 3 )
	{	
		for( i=0; i<4; i++ )
		{
			ReadE2WithJieti ( 4+i, Price.Ladder_Dl[0].B08 );//�������׽��ݵ���/���/�����ʱ�串�ǵ�ǰ
			WriteE2WithJieti( i, Price.Ladder_Dl[0].B08 );
		}
		RefreshPrice();
	} 
	else if( offset == 4 )
	{
		I2C_Read_Eeprom ( ee_jtshiqu2_page, ee_jtshiqu_inpage, Eeprom.Buffer, ee_jtshiqu_len ); //�����ݽ���ʱ�����ǵ�ǰ
		I2C_Write_Eeprom( ee_jtshiqu1_page, ee_jtshiqu_inpage, Eeprom.Buffer, ee_jtshiqu_len );
		RefreshPrice();
	}
	else return;
	
	//������ʱ��
	memset( Starttime.Shiqu+offset*5, 0x00, 5 );	
	WriteE2WithBackup(ee_starttimer_page, ee_starttimer_inpage, Starttime.Shiqu, ee_starttimer_lenth);
	
	//����Լ������
	if( offset >= 4 )
		Freeze_Dl(ee_jtsqswith_note_page+4*(offset-4),5+offset);
	else
		Freeze_Dl(ee_appointfreeze1_page+4*offset,5+offset);
}


unsigned char FindSQ( unsigned char Num )
{
	unsigned char i, k, n;

	Sequence( Eeprom.Buffer, Num );//ʱ�α�����
	
	for( i=0, k=0, n=0; i<Num; i++ )
	{
		if( (n != 0xff) && (Cmp_Bcd_Inv(Eeprom.Buffer+k+1,Sclock.Clockbackup+3,2) == 1) )
		{
			if( i == 0 ) n = 0xff; //С�ڵ�һʱ��,��Ϊ���ʱ��
			else break;
		}

		k += 3;
		if( ChkBufZero(Eeprom.Buffer+k+1, 2) == 0 ) break; //������+��Ϊ��,��Ч
	}
	
	return (Eeprom.Buffer[k-3]);//�õ�ʱ�α��
}

void Festival_holiday_Judge(void)
{	
  unsigned char i,t,k; 
	
	k=0;Festival_holiday_flag = 0;
	
	t = Bcd_To_Hex(Feik.Num[4]);
	//�����ڼ���
	if( t <= max_maxholiday )//���֧��60���ڼ���
	{
		for( i=0; i<t; i++ )
		{
			if( (i%15) == 0 )
			{
				k = 0;
				if( I2C_Read_Eeprom( ee_holiday_page+(i/15), 0x00, Eeprom.Buffer, ee_holiday_lenth ) )
				{
					I2C_Read_Eeprom( ee_holiday_page+(i/15), 0x00, Eeprom.Buffer, ee_holiday_lenth );//�ٶ�һ��
				}
			}
			
			if( Cmp_Data( Eeprom.Buffer+1+k, Sclock.Clockmain+4, 3 ) == 3 ) //�Ƚ�������
			{
				Festival_holiday_flag = 1;
				break;
			}
			k += 4;
		}
	}
	//�������ж�
	if((Sclock.Clockmain[3]==0 )||(Sclock.Clockmain[3]==6 ))  Festival_holiday_flag = 1;

}




void Config_Shiduan_Process(void)//�ҵ���ǰʱ���õ���ǰʱ�α�
{
	unsigned char t,n,page;
	unsigned char Buf[16];
	
	I2C_Read_Eeprom44( ee_changecontrol_page, Buf+7, ee_changecontrol_lenth);//�����������ֽ�
	if( Buf[7] != 0x55 ) page = ee_shiqu1_page;//��һ��Ϊ��ǰʱ��
	else page = ee_shiqu2_page;//�ڶ���Ϊ��ǰʱ��
	if ( I2C_Read_Eeprom( page, 0, Eeprom.Buffer, ee_shiqu_lenth )!= 0x00)
	{
		I2C_Read_Eeprom( page, 0, Eeprom.Buffer, ee_shiqu_lenth);
	}
	//ʱ���ж�(ʱ�α��+��+��)
	t = Bcd_To_Hex(Feik.Num[0]);
	if( (t == 0) || (t > 14) ) t = 1;
	n = FindSQ(t);	//�õ�ʱ�α��
	
	if( Buf[8] != 0x55 ) page = ee_shiduantable1_1_page;//��һ��Ϊ��ǰʱ�α�
	else page = ee_shiduantable2_1_page;//�ڶ���Ϊ��ǰʱ�α�

	//�������ж�
	if( Sclock.Clockmain[3] <= 6 )//����ǷǷ����ܾͲ��ж�������
	{
		if( ((Week_Holiday.Status>>Sclock.Clockmain[3]) & B0000_0001)==0 )//0������Ϣ,1������
		{
			n = Week_Holiday.Shiduan_N;
		}
	}
	//�ڼ����ж�
//	t = Bcd_To_Hex(Feik.Num[4]);
//	if( t <= max_maxholiday )//���֧��60���ڼ���
//	{
//		for( i=0; i<t; i++ )
//		{
//			if( (i%15) == 0 )
//			{
//				k = 0;
//				if( I2C_Read_Eeprom( ee_holiday_page+(i/15), 0x00, Eeprom.Buffer, ee_holiday_lenth ) )
//				{
//					I2C_Read_Eeprom( ee_holiday_page+(i/15), 0x00, Eeprom.Buffer, ee_holiday_lenth );//�ٶ�һ��
//				}
//			}
//			
//			if( Cmp_Data( Eeprom.Buffer+1+k, Sclock.Clockbackup+3, 3 ) == 3 ) //�Ƚ�������
//			{
//				n = Eeprom.Buffer[k];
//				break;
//			}

//			k += 4;
//		}
//	}
	if( (n == 0)||(n > 8) ) n=1; 
	n--;
	Tariff.Current_N = n;
	I2C_Read_Eeprom( page+n, 0x00, Tariff.Table, ee_shiduantable_lenth);	
}


//���ݽ���ʱ����ȷ�����ݱ�
void Config_ShiQuLad_Proc( void )
{
	unsigned char n;

	I2C_Read_Eeprom ( ee_jtshiqu1_page, ee_jtshiqu_inpage, Eeprom.Buffer, ee_jtshiqu_len ); //������ʱ����
	
	//ʱ���ж�(���ݱ��+��+��)
	n = FindSQ(4);	//�õ����ݱ��
	
	//����ǰ���ݱ�
	if( (n == 0) || (n > 2) ) n = 1;
	n--;
	Tariff.CurLad_N = n;
	ReadE2WithJieti( Tariff.CurLad_N, Price.Ladder_Dl[0].B08 ); //����ǰ�׽��ݵ���/���/�������
}


void Sequence( unsigned char *Buf, unsigned char Num )//����+����+Сʱ
{
	unsigned char i, n;
	unsigned char BufB[3];

	if( Num < 2 ) return;
	
	for( n=0; n<Num-1; n++ )
	{
		for (i=0; i<Num-1-n; i++)
		{
			if ( Cmp_Bcd_Inv( Buf+i*3+1, Buf+(i+1)*3+1, 2 ) == 0x01 ) //ǰ���� > ������
			{
				memcpy( BufB, Buf+i*3, 3 );//��������
				memcpy( Buf+i*3, Buf+(i+1)*3, 3 );//��������
				memcpy( Buf+(i+1)*3, BufB, 3 );//��������
			}
		}
	}
}
	

void Timezone_Judge(void)	//ʱ��ʱ�η����ж�
{
	unsigned char i,t,Result,Status=0;
	unsigned char BufA[42];

	feik_Num_Verify();
	if( run_timezone_flag && ( FLAG_EA == 0 ) ) return;
	Starttimer_Verify();//����ʱ��У��
	if( run_timezone_flag && ( FLAG_EA == 0 ) ) return;
	Week_Holiday_Verify();//�����ղ���У��
	
	run_timezone_flag = 0;
	if( BDMODE ) //����ģʽ
		run_price_flag = 1;
	else
		run_alarm_flag = 1;

	//����ʱ���ж�
	if( !clock_error ) 
	{
		if((Cmp_Clock( Starttime.Shiqu, 5 ) != 0x01) && ChkBufZero( Starttime.Shiqu, 5 ))
		{
			Changecontrol_Process(0);//��ת�����ֽ�,������ʱ��ͽ���Լ������
			Status = 0x01;//�����ж�ʱ�α�
		}
		if((Cmp_Clock( Starttime.Shiduan, 5 ) != 0x01) && ChkBufZero( Starttime.Shiduan, 5 ))
		{
			Changecontrol_Process(1);//��ת�����ֽ�,������ʱ��ͽ���Լ������
			Status = 0x01;//�����ж�ʱ�α�
		}
	}
	//ʱ��У��
	if( Status == 0x01 || Sclock.Clockbackup[1]==0 )//����ʱ�䵽����ÿСʱ
	{	
		Config_Shiduan_Process();//�ҵ���ǰʱ���õ���ǰʱ�α�
	}
	else if( Crc_Check( Tariff.Table, ee_shiduantable_lenth+2 ) ) 
	{
		Config_Shiduan_Process();//�ҵ���ǰʱ���õ���ǰʱ�α�
	}
	//�����ж�(����+����+Сʱ)
	t = Bcd_To_Hex(Feik.Num[2]);
	if ( (t == 0) || (t > max_shiduancount) ) t=1;
	memcpy( BufA, Tariff.Table, 42 );//����ʱ�α�
	Sequence( BufA, t );//ʱ�α�����
	t = t*3;//ʱ����*3
	for (i=0; i<t; i++)
	{
		if ( ((BufA[i+2] > Sclock.Clockbackup[2])) 
			|| ((BufA[i+2] == Sclock.Clockbackup[2]) 
			&& (BufA[i+1] > Sclock.Clockbackup[1])) ) 
		{
			if( i == 0 ) i=t;//С�ڵ�һʱ��,��Ϊ���ʱ��
			i = i - 3;
			break;
		}
//		if ( (i >=3) && (BufA[i]==0) && (BufA[i+1]==0) )
//		{
//			i = i - 1;//����ʱ�η��Ӻ�Сʱ��Ϊ0,��Ч
//			break;
//		}
		i++;
		i++;	
	}
	if( i == t ) i = i - 3; //���һ��ʱ��(��ע�����һ��ʱ������Ϊ0)
	if( (BufA[i]==0) || (BufA[i]>4) ) Tariff.Current = 0;	//�Ƿ�Ĭ��Ϊ�����
	else Tariff.Current = (BufA[i]-1);
	Result = i/3;//�õ���ǰʱ�κ�
	if(Result != Tariff.Period_N) 
	{
		Init_Demand1();
		
		if( Outputtype==0x02 )
		{
			Shiduanctrl.Timer = shiduanctrl_timer_delay;
			SHIDUANCTRL_ON;//
		}
	}
	Tariff.Period_N = Result;//���µ�ǰʱ�κ�	
	
	//����״̬��
	Tariff.Status &= B0000_0011;
	I2C_Read_Eeprom44( ee_changecontrol_page, Eeprom.Buffer, ee_changecontrol_lenth);//�����������ֽ�
	if ( Eeprom.Buffer[0] == 0x55 ) Tariff.Status |= B0000_1000;//��ǰʱ��
	if ( Eeprom.Buffer[1] == 0x55 ) Tariff.Status |= B0000_0100;//��ǰʱ�α�
}


unsigned char JudgeYearClock( unsigned char *YearLadder )//�ж�������պϷ���(�������ֻ����1��12����ĳ�µ�1����28���ڵ�����ʱ��)
{
	unsigned char i;
	
	for (i=0; i<6; i++)
	{
		if( JudgeClockNoWeek( 2, YearLadder+3*i, 3 ) == 0 )
		{
			if( YearLadder[1+3*i] <= 0x28 )  return 0;//��ȷ
		}
	}
	
	return 1;//����
}


unsigned char Freeze_Change_Judge( unsigned char Change )//���㷽ʽ�任�жϼ�����
{
	if( JudgeYearClock( Price.Year_Ladder ) == 0 )
	{
		if( Price.Ladder_Mode == 0x00 )
		{
			//�½��㵽�����
			Freeze_Usedl(2);//�½���,���õ���ת�������
			Freeze_Dl(ee_histroy_dl_page,ee_histroyf_inpage);	
			Price.Ladder_Mode = 0x55;//�����ģʽ	
			Process_Usedl(6);//�������õ�����0��ʼ	
			return 0x5a;	//���ڷ�ֹ�ظ��������õ������½���
		}
		else if( Change == 1 ) 	//������ոı�
		{
			//������л��������
			Freeze_Usedl(0);//���õ���ת�������
			return 0x55;	//���ڷ�ֹ�ظ��������õ���
		}
	}
	else
	{
		if( Price.Ladder_Mode == 0x55 )
		{
			//����㵽�½���
			Freeze_Usedl(0);//���õ���ת�������
			Price.Ladder_Mode = 0x00;//�½���ģʽ
			Process_Usedl(1);//���õ�����0��ʼ
			Freeze_Dl(ee_histroy_dl_page,ee_histroyf_inpage);
			return 0xaa;	//���ڷ�ֹ�ظ��������õ���
		}
	}
	
	return 0x00;
}

void Ladderdata_Check(void)
{
	if( Crc_Check( Price.Ladder_Dl[0].B08, ee_ladder_lenth+2 ) ) 
	{
		Config_ShiQuLad_Proc();//�ҵ���ǰʱ���õ���ǰ���ݱ�
	}
}	


unsigned char Year_Ladder_Judge(void)//����������жϺ����ݺϷ����ж�
{
	unsigned char Buf[ee_curyearjs_len+2];
	
	ReadE2WithBackup( ee_curyearjs_page, ee_curyearjs_inpage, Buf, ee_curyearjs_len );
	
	if( JudgeYearClock( Buf ) == 0 ) Price.Ladder_Mode = 0x55;//�����ģʽ
	else  Price.Ladder_Mode = 0x00;//�½���ģʽ
	
	//��������ʱ���ж�
	if( !clock_error )
	{
		//����ʱ������ʱ���ж�
		if((Cmp_Clock( Starttime.LadSQ, 5 ) != 0x01) && ChkBufZero( Starttime.LadSQ, 5 ))
		{
			Changecontrol_Process(4);//��ת�����ֽ�,������ʱ��ͽ���Լ������
		}

		if((Cmp_Clock( Starttime.Ladder, 5 ) != 0x01) && ChkBufZero( Starttime.Ladder, 5 ))
		{
			Changecontrol_Process(3);//��ת�����ֽ�,������ʱ��ͽ���Լ������			
		}
	}

	//���ݱ�У��
	if( Sclock.Clockbackup[1]==0 )//����ʱ�䵽����ÿСʱ
	{	
		Config_ShiQuLad_Proc();//�ҵ���ǰʱ���õ���ǰ���ݱ�
	}
	else 
	{
		Ladderdata_Check();
	}
		
	//�µ�������պ͵�ǰ��������б仯����ת��Ͷ���
	if( Cmp_Data(Price.Year_Ladder, Buf, ee_curyearjs_len) != 3 )  
	{
		WriteE2WithBackup( ee_curyearjs_page, ee_curyearjs_inpage, Price.Year_Ladder, ee_curyearjs_len );
		
		return Freeze_Change_Judge(1);	//������ոı�
	}
	else return( Freeze_Change_Judge(0) );	//��������ʱ�䵽,���㷽ʽ�任�жϼ�����,�������δ�ı�
}


void Price_Judge(void)	//���ʽ��ݵ���ж�
{
	unsigned char i,t;
	union B32_B08 BufA[4];

	run_price_flag = 0;
	run_alarm_flag = 1;
	
	//��������У��
	Ladderdata_Check();
	
	//��������ʱ���ж�
	if( !clock_error )
	{
		if((Cmp_Clock( Starttime.Rate, 5 ) != 0x01) && ChkBufZero( Starttime.Rate, 5 ))
		{
			Changecontrol_Process(2);//��ת�����ֽ�,������ʱ��ͽ���Լ������
		}
	}
	//���ʵ�۱�У��
	if( Crc_Check( Price.Tariff[0].B08, ee_tariffprice_lenth+2 ) ) 
	{
		I2C_Read_Eeprom( ee_tariffprice1_page, 0, Price.Tariff[0].B08, ee_tariffprice_lenth);//����ǰ���ʵ��
	
	 
	}
	
	//�жϵ�ǰ����
//	t = Feik.Num[6];//������(1~6),������Ϊ0���޽��ݹ���
	t = 6;//6�����ݵ���
	for (i=0; i<5; i++)
	{
		if ( Price.Ladder_Dl[5-i].B32 == Price.Ladder_Dl[4-i].B32 ) t--;//��ͬ��������1
		else break;
	}
	
	if( (t > 0) && (Price.Ladder_Dl[t-1].B32 == 0) ) t--;//�����ݵ���Ϊ0,���������1
	
	if( t == 0 )	//�жϽ��ݵ����Ƿ�Ϊȫ0
	{
		Tariff.Current_Ladder = 0xff;//�޽���
		Price.Current_L.B32 = 0;//��ǰ���ݵ��Ĭ��Ϊ0
	}
	else
	{	
		BufA[0].B32 = Fwdl.Usedl.B32;//�����õ���
		Add_Bcd( BufA[0].B08, Bkdl.Usedl.B08, 4 );	//�����õ���+�����õ���=���õ���
		for (i=0; i<t; i++)
		{
			if ( BufA[0].B32 <= Price.Ladder_Dl[i].B32 ) 
			{
				break;//�ҵ�
			}
		}//i�ķ�Χ0~6
		Tariff.Current_Ladder = i;//��ǰ����
		Price.Current_L.B32 = Price.Ladder[i].B32;//���ݵ��
	}
		
	//������
	Price.Current_F.B32 = Price.Tariff[Tariff.Current&B0000_0011].B32;//���ʵ��		
	Exchange( BufA[1].B08, Price.Current_F.B08, 4 );
	
	Price.Current.B32 = Price.Current_L.B32;//���㵱ǰ���=���ݵ��+���ʵ��
	Exchange( BufA[2].B08,Price.Current.B08, 4 );
	Add_Bcd( BufA[1].B08, BufA[2].B08, 4 );
	Price.Current.B32 = BufA[1].B32;		
	//������
	BufA[1].B08[3] = 0;
	BufA[2].B08[3] = 0;
	Exchange( BufA[1].B08, Esam_Para.Current_Ratio, 3 );//�������
	Exchange( BufA[2].B08, Esam_Para.Voltage_Ratio, 3 );//��ѹ���
	Bcd2Hex( BufA[1].B08, BufA[1].B08, 4 );//�������
	Bcd2Hex( BufA[2].B08, BufA[2].B08, 4 );//��ѹ���
	
	BufA[2].B32 = BufA[1].B32*BufA[2].B32;//���hex
	if ( (BufA[2].B32==0) || (BufA[2].B32>=10000) ) BufA[2].B32 = 1;	
		
	//������Ա�ȵĽ��ݵ�ۺͷ��ʵ��
	Bcd2Hex( Price.Current.B08, BufA[1].B08, 4 );//��ǰ���hex
	BufA[3].B32 = BufA[1].B32*BufA[2].B32;//���*��ǰ���(hex)
	Hex2Bcd( BufA[3].B08, Price.Current_X.B08, 4 );//���*��ǰ���(bcd)
	
	//����״̬��
	Tariff.Status &= B0000_1100;
	if( JudgeSwitch( Starttime.Rate,1) == 0 )  Tariff.Status |= B0000_0010;//����ʱ��Ϸ�,��ʾ��2�׷��ʵ��
	if( JudgeSwitch( Starttime.Ladder,1) == 0 ) Tariff.Status |= B0000_0001;//����ʱ��Ϸ�,��ʾ��2�׽��ݵ��
}


//Ch=0,1,2,3����ǰ��,��������CRC
//Ch=4,5,6,7��������
void ReadE2WithJieti( uint08 Ch, uint08 *Buf )	//�����ݵ���/���/�������/CRC
{
	I2C_Read_Eeprom( ee_ladder1_page+Ch, 0, Buf, ee_ladder_lenth );
}
		

void WriteE2WithJieti( uint08 Ch, uint08 *Buf )	//д���ݵ���/���/�������
{
	I2C_Write_Eeprom( ee_ladder1_page+Ch, 0, Buf, ee_ladder_lenth );//��дһ��
}
