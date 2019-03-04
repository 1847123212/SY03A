#include "ex_func.h"
#include "ex_var.h"
#include <string.h>
#include <absacc.h>

void Init_Demand1(void)	//��λ���������������ݺͲ���(ʱ��ת��)
{
	Tar_Slidingtimer = 1;//�����������ʱ��
	Tar_Demandtimer = Demand_Para.Max_Demand_Period+1;//������������������ʱ��
	memset( Tar_Demand, 0x00, 122 );//���ݳ�ʼ����������������������(����ʱ����)
}

void Calculate_Demand( unsigned short *Buf, unsigned char Type )	//�������������ж�
{
	unsigned char i,Z1,Z2,Inpage,BufA[42];
	float Temp_Demand;
	union  B32_B08  Temp_Current_Demand;
	//��������
	Temp_Demand = 0;
	Z1 = Demand_Para.Max_Demand_Period/Demand_Para.Sliding_Time;//��������/����ʱ��
	Z2 = 60/Demand_Para.Max_Demand_Period;//60/��������
	for( i=0; i<Z1; i++ )
	{
		Temp_Demand += Buf[i+1];
	}
	Temp_Demand = Temp_Demand*Z2/default_meterconst_dm;//���㵱ǰ��������
	Temp_Current_Demand.B32 = (long int)(Temp_Demand*10000);//���㵱ǰ��������
	Hex2Bcd( Temp_Current_Demand.B08, Temp_Current_Demand.B08, 4 );//���㵱ǰ����bcd��
		
	//��������ж�
	if( dl_back_flag )
	{
		I2C_Read_Eeprom( ee_n_demand_page, ee_n_demand_inpage, BufA, ee_ndemand_lenth );//����ǰ���������������
	}
	else	
	{
		I2C_Read_Eeprom( ee_p_demand_page, ee_p_demand_inpage, BufA, ee_pdemand_lenth );//����ǰ���������������
	}
	Inpage = Type*8;
	//Inverse( BufA+Inpage, 3 );//�������ת���ɸ��ֽ���ǰ
	
	if( Cmp_Data( Temp_Current_Demand.B08, BufA+Inpage, 3 ) == 0x01 )//��ǰ����>�������
	{
		//Inverse( Temp_Current_Demand.B08, 4 );//��ǰ��������ת���ɵ��ֽ���ǰ
		memcpy( BufA+Inpage, Temp_Current_Demand.B08, 3 );//��ǰ��������Ϊ�������
		memcpy( BufA+Inpage+3, Sclock.Clockbackup+1, 5 );//������������ʱ��
		if( dl_back_flag )
		{
			I2C_Write_Eeprom( ee_n_demand_page, ee_n_demand_inpage, BufA, ee_ndemand_lenth );//д��ǰ���������������
		}
		else	
		{
			I2C_Write_Eeprom( ee_p_demand_page, ee_p_demand_inpage, BufA, ee_pdemand_lenth );//д��ǰ���������������
		}
	}
//	else
//	{
//		Inverse( Temp_Current_Demand.B08, 4 );//��ǰ��������ת���ɵ��ֽ���ǰ
//	}
	
	if( Type==0 ) Current_Demand.B32 = Temp_Current_Demand.B32;//���µ�ǰ����
}


void Init_Demand(void)	//��λ�����������ݺͲ���(�ϵ�,����,ʱ�ӵ���,�������ڱ��,����仯)
{
	Slidingtimer = 1;//�����������ʱ��
	Demandtimer = Demand_Para.Max_Demand_Period+1;//��������������ʱ��
	memset( Demand, 0x00, 122 );//���ݳ�ʼ��������������������(����ʱ����)
	Init_Demand1();
	Current_Demand.B32 = 0;
}



void Cal_Pav(void)
{
	union B32_B08 S1_Temp;	
	
	if(Pav_Cnt == 0) return;
	
	S1_Temp.B32 = Pav_Temp/(float)Pav_Cnt;	//ÿ2���һ�ι���ֵ
	
	Hex2Bcd(S1_Temp.B08, Emu_Data.Pav.B08, 4);
	
	Pav_Temp = 0;
	Pav_Cnt = 0;
}


void Demand_Process()	//��������
{
	
	unsigned char i;
	
	Dm_process_flag = 0; //���账���־
	//ÿ����ƽ������
	Cal_Pav();
	
	
	//����ʱ���ж�
	Tar_Slidingtimer--;
	if( Tar_Slidingtimer==0 )
	{
		for( i=0; i<60; i++ )//������������ʱ������������λ60��
		{
			Tar_Demand[60-i] = Tar_Demand[59-i];
		}
		Tar_Demand[0] = 0;
		Tar_Slidingtimer = Demand_Para.Sliding_Time;//�����������ʱ��	
	}
	Slidingtimer--;
	if( Slidingtimer==0 )
	{
		for( i=0; i<60; i++ )//����������ʱ������������λ60��
		{
			Demand[60-i] = Demand[59-i];
		}
		Demand[0] = 0;
		Slidingtimer = Demand_Para.Sliding_Time;//�����������ʱ��
		if( Outputtype==0x01 )//����ʱ�䵽�������
		{
			Shiduanctrl.Timer = shiduanctrl_timer_delay;
			SHIDUANCTRL_ON;//�˿ڸ����������
		}		
	}
			
	//���������ж�
	Demandtimer--;
	if( Demandtimer==0 )//����������ʱ����������
	{
		Calculate_Demand( &Demand[0],0 );//���������������ж�
		Demandtimer = Demand_Para.Sliding_Time;//��������������ʱ��(�Ի���ʱ�����)
		if( Outputtype==0x81 )//��һ���������ڵ��������
		{
			Outputtype = 0x01;
			Shiduanctrl.Timer = shiduanctrl_timer_delay;
			SHIDUANCTRL_ON;//�˿ڸ����������
		}
	}
	Tar_Demandtimer--;
	if( Tar_Demandtimer==0 )//������������ʱ����������
	{
		Calculate_Demand( &Tar_Demand[0],Tariff.Current+1 );//�����������������ж�	
		Tar_Demandtimer = Demand_Para.Sliding_Time;//��������������ʱ��(�Ի���ʱ�����)
	}

}



