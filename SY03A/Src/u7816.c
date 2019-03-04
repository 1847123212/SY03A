#include "ex_var.h"
#include "ex_func.h"
//#include <intrins.h>
#include <string.h>
#include "cpu_card.h"

//====================================================================
extern unsigned char RF_TxRxFrame( uint08 Lc, uint08 Le );
extern unsigned char RF_Init(void);
//====================================================================
//��Ƭ�򽻻��ն����ݱ�����
//3����������Ϊ��
//��Ƭ�ļ���,ƫ�Ƶ�ַ����������+MAC
//====================================================================
const uint08 code BACKBUF[42] = 
{
	0x81, 4,  6+4,		//00�����ļ�1+MAC������1+�������±�־1+����4��
	0x81, 15, 15+4,		//01�����ļ�2+MAC������1+��1+��2+���1+���2��
	0x83, 0,  52+4,		//02�����ļ�1+MAC���ļ�ͷ4+����12*4��
	0x84, 0,  52+4,		//03�����ļ�2+MAC���ļ�ͷ4+����12*4��
	0x81, 10, 5+4,		//04�����л�ʱ��5+MAC
	
	0x83, 52, 52+4,		//05��ǰ�׽����ļ�1:����ֵ24+���ݵ��28+MAC
	0x83, 104,18+4,		//06��ǰ�׽����ļ�1:�������*6+MAC
	0x83, 122,52+4,		//07��ǰ�׽����ļ�2:����ֵ24+���ݵ��28+MAC
	0x83, 174,18+4,		//08��ǰ�׽����ļ�2:�������*6+MAC
	
	0x84, 52, 52+4,		//09�����׽����ļ�1:����ֵ24+���ݵ��28+MAC
	0x84, 104,18+4,		//10�����׽����ļ�1:�������*6+MAC
	0x84, 122,52+4,		//11�����׽����ļ�2:����ֵ24+���ݵ��28+MAC
	0x84, 174,23+4,		//12�����׽����ļ�2:�������*6+�л�ʱ��5+MAC
	0x81, 36, 6+4,		//13�ͻ����+MAC	
};

//====================================================================
//��MAC��д����ʱ��RAM�д�MAC�������ݵ�ƫ�Ƶ�ַ��˳��ͬBACKBUF��������
//��� 6
//�ͻ���� 6
//�����л�ʱ�� 5
//�����л�ʱ�� 5
//�����ļ�1 16
//�����ļ�2 16
//��ǰ�׽����ļ� 52
//��ǰ������� 12
//�����׽����ļ� 52
//����������� 12
//====================================================================
const uint16 code BACKADDR[10] =
	{ 0, 6, 12, 17, 22, 38, 54, 106, 118, 170};	
//====================================================================


void Delay_Netu( unsigned char Num )	//Num*1etu
{
	uint16 i;
	
	for(i=0; i<Num*50; i++);		//1etu=93us(4M),����1etu=186us(2M)
}

//��λ��Ӧ����ʱ��Ϊ400/f~40000/f��100us~10ms
//����ֵ: 1Ϊ�½���; 0Ϊ�ޱ仯
unsigned char Judge_Edge( void )
{
	uint16 i;

	Delay_Netu(1);

	for( i=0x00; i<3330; i++ ) 	//Լ10ms(4M),����20ms(2M)
	{
//		if( (U7816SECONDSTATUS&B0000_0001) == 1 ) //���ڽ���
		{
			return 1;
		}
	}
		
	return 0;
}

//�ʱ��ӦΪ9600etu,ĿǰΪ9600etu,1etu=93us
unsigned char Get_7816_Char( unsigned char CH, unsigned char *Data )
{
	unsigned long int i;

	if( CH == ESAM )
	{
//		U7816CTRL0 = B1110_0000;	//U7816_0
	}
	else
	{
//		U7816CTRL0 = B1111_0000;	//U7816_1
	}
	
//	U7816FRMCTRL0 &= B1011_1111;

	for( i=0x00; i<60000; i++ ) 	//Լ900ms(4M),����1800ms(2M)
	{
		Clear_Wdt();

//		if( U7816PRIMARYSTATUS&B0000_0001 ) //RX_FLAG�Ѿ���λ
		{
//			*Data = U7816RXBUF;

//			return 1; //�ɹ�
		}

//		if( U7816PRIMARYSTATUS&B0000_0100 ) //ERROR_FLAG�Ѿ���λ
		{
//			switch( U7816ERRSTATUS ) //���б�־λ: Ӳ����λ��д0����
//			{
//				case B0000_0001: //������������־λ
//					break;
//
//				case B0000_0010: //����֡��ʽ�����־λ
//					break;
//
//				case B0000_0100: //����������żУ������־λ
//					break;
//
//				case B0000_1000: //����������żУ������־λ
//					break;
//
//				default:
//					break;
//			}

//			U7816ERRSTATUS = B0000_0000; //clear error flag.

			return 2; //error
		}
		
//		if( EA == 0 ) return 3;
	}
	
	return 3;
}

unsigned char Snd_7816_Byte( unsigned char CH, unsigned char Data )
{
	uint16 i;

	if( CH == ESAM )
	{
//		U7816CTRL0 = B1010_0000;	//U7816_0
	}
	else
	{
//		U7816CTRL0 = B1011_0000;	//U7816_1
	}
	
	Delay_Netu(1);				//��ʱ1��etu,��֤���豸�ȶ�����

//	U7816FRMCTRL0 &= B1011_1111; 		//max one time.

//	U7816TXBUF = Data;
	
	Clear_Wdt();

	for( i=0x00; i<7000; i++ ) 	//Լ39ms(4M),����78ms(2M)
	{
//		if( U7816PRIMARYSTATUS&B0000_0010 )	//TX_FLAG��λ
		{
			return 0; //�ɹ�
		}

//		if( U7816PRIMARYSTATUS&B0000_0100)	//ERROR_FLAG��λ
		{
//			switch( U7816ERRSTATUS ) //���б�־λ: Ӳ����λ��д0����
//			{
//				case B0000_0001: //������������־λ
//					break;
//
//				case B0000_0010: //����֡��ʽ�����־λ
//					break;
//
//				case B0000_0100: //����������żУ������־λ
//					break;
//
//				case B0000_1000: //����������żУ������־λ
//					break;
//
//				default:
//					break;
//			}

//			U7816ERRSTATUS = B0000_0000; //clear error flag.

//			return 1; //error
		}
		
//		if( EA == 0 ) return 1;
	}
	
	return 1;
}

void Esam_Err_Report(void)
{
	if( esam_error==0 )
	{
		Set_Reportdata(0*8+1);//��esam�����ϱ���־
	}
	
	esam_error = 1;
}

//command-response pairs
unsigned char CardTxRxFrame( unsigned char CH, uint08 Lc, uint08 Le )
{
	uint08 k, result, Temp_Data, Temp_SW1, Temp_SW2, Proc_Byte;

	Ic_Comm_RxPtr = 0x00;
	memset( Ic_Comm_RxBuf, 0x00, u7816_buf_size );
	Delay_Netu(10);	//����ʱ10��etu
	Err_Sw12[0] = 0;
	Err_Sw12[1] = 0;
	
	if(CH == CARD)
	{
		result = RF_TxRxFrame(Lc, Le);
		return (result);
	}
	
	for( k=0; k<5; k++ )	//send command header (5bytes)
	{
		result = Snd_7816_Byte( CH, Ic_Comm_TxBuf[ k ] );	//;ȡ����������
		if( result == 1 ) { return 1; } //send failure.
	}

	for( ;; )
	{
		result = Get_7816_Char( CH, &Proc_Byte ); //receive procedure byte.
		if( result != 1 ) { return 2; } //receive failure.

		if( Proc_Byte == 0x60 ) //Null byte
		{
			//do nothing. continue to receive.
		}
		else if( Proc_Byte == Ic_Comm_TxBuf[1] ) //procedure byte = INS
		{
			if( k < (Lc+5) )	//send remaining data, then wait for procedure byte.
			{
				if( CH == ESAM ) Delay_Netu(16);	//���յ�INS���뷢������֮��������ʱ16��etu
				
				for( ; k<(Lc+5); )
				{
					result = Snd_7816_Byte( CH, Ic_Comm_TxBuf[ k++ ] ); //;ȡ����������
					if( result == 1 ) { return 3; }	//send failure.
				}
			}
			else
			{
				//;�����ֽ���֮ǰ�ѽ��գ�ֻ����2��״̬�ֽڼ��ɡ�
				for( Ic_Comm_RxPtr=0x00; Ic_Comm_RxPtr<Le; Ic_Comm_RxPtr++ )
				{
					result = Get_7816_Char( CH, &Temp_Data );
					if( result != 1 )	{ return 4; }	//;Receive fail
			
					Ic_Comm_RxBuf[ Ic_Comm_RxPtr ] = Temp_Data;
				}
			}
		}
//		else if( Proc_Byte == (Ic_Comm_TxBuf[1]^0xFF) ) //procedure byte = (INS XOR FFH)
//		{
//			if( k < (Lc+5) )	//send next one data, then wait for procedure byte.
//			{
//				result = Snd_7816_Byte( CH, Ic_Comm_TxBuf[ k++ ] ); //;ȡ����������
//				if( result == 1 ) { return 5;}	//send failure.
//			}
//			else
//			{
//				//;�����ֽ���֮ǰ�ѽ��գ�ֻ����2��״̬�ֽڼ��ɡ�
//				for( Ic_Comm_RxPtr=0x00; Ic_Comm_RxPtr<( Le+2 ); Ic_Comm_RxPtr++ )
//				{
//					result = Get_7816_Char( CH, &Temp_Data );
//					if( result != 1 )	{	return 6;	}	//;Receive fail
//			
//					Ic_Comm_RxBuf[ Ic_Comm_RxPtr ] = Temp_Data;
//				}
//
//				return 0;
//			}
//		}
		else	//other proc byte.
		{
			Temp_SW1 = ( Proc_Byte & 0xF0 );
			
			if( (Temp_SW1 == 0x60) || (Temp_SW1 == 0x90) ) //procedure byte = 6X or 9X (ex. 60H)	
			{
				result = Get_7816_Char( CH, &Temp_SW2 ); //receive sw2.
				if( result != 1 ) { return 7; } //receive failure.
		
				// 61H: send get response command, P3<=XX
				// 6CH: send last command again, P3 = XX;
				// other 6X or 9X, this communication is over.

				//'9000' command normally completed
				//'6E00' CLA not supported
				//'6D00' CLA supported, but INS not programmed or invalid
				//'6B00' CLA INS supported, but P1 P2 incorrect
				//'6700' CLA INS P1 P2 supported, but P3 incorrect
				//'6F00' command not supported and no precise diagnosis given
		
				Ic_Comm_RxBuf[ Ic_Comm_RxPtr++ ] = Proc_Byte;
				Ic_Comm_RxBuf[ Ic_Comm_RxPtr++ ] = Temp_SW2;
				Err_Sw12[0] = Proc_Byte;
				Err_Sw12[1] = Temp_SW2;
				
				if( (Proc_Byte == 0x6A) && (Temp_SW2 == 0x81)&& (CH == ESAM) ) Esam_Err_Report();
				
				if( (Proc_Byte == 0x61) || ((Proc_Byte == 0x90)&&(Temp_SW2 == 0x00)) )
					return 0;
				else
					return 8;
			}
			else
			{
				return 8;
			}
		}
	}

//	return 9;
}

//���㼤��&�临λ
//��⵽�������, RST�õ͵�ƽ, VCC�ϵ�, ��ʱһ��ʱ��, IO����Ϊ����̬, CLK���ʱ���ź�(T0ʱ��)
//��T0��40000~45000��CLK��, ��RST��Ϊ�ߵ�ƽ(T1ʱ��), ��T1���400~40000��CLK��, ��λӦ��Ӧ�ÿ�ʼ, 
//��IC���ĸ�λӦ��û���ڴ�ʱ����ڿ�ʼ, �ն˽������ͷ�ʱ��.���յ���λӦ��, ��������Э��淶, ����Ҫ�ȸ�λ
unsigned char ColdReset( unsigned char CH )
{
	unsigned char n, Result, Temp_ATR;	// T0, TD1, TD2;
	
	if( CH == ESAM )
	{
//		GPIO41CFG = B0110_0010; 	//SAMIO,
		RST_ESAM_L;	
	}
	ESAMPOWERON;
//	GPIO40CFG = B0100_0010; 		//MCUCLK
	Delay_Initial(20);			//��ʱ100ms
	
	if( CH == 0x00 )
	{
//		U7816CTRL0 = B0110_0000;	//U7816_0
	}
	else
	{
//		U7816CTRL0 = B0111_0000;	//U7816_1
	}
	
	//----------------------------------------------------------------------------------------------
//	Result = U7816RXBUF; //RX������, �����rx_flag=1, ����ͨ����ȡRXBUF������˱�־.

//	U7816ERRSTATUS = 0x00;    //�������Ĵ���������˳�����Ҫ, ���ܵߵ�
//	U7816SECONDSTATUS = 0x00;
//	U7816PRIMARYSTATUS = 0x00;

	if( CH == 0x00 )
	{
		RST_ESAM_L;		//clr PGIO4.3 to reset U7816_0
	}
	else
	{
		RST_CARD_L;		//clr PGIO4.4 to reset U7816_1
	}

	I2c_Wait_5ms();			//�ȴ�RSTʱ��,����400����,��100us
	
	//-------- T1ʱ��, RST����ߵ�ƽ --------
	if( CH == 0x00 )
	{
		RST_ESAM_H;		//set PGIO4.3
	}
	else
	{
		RST_CARD_H;		//set PGIO4.4
	}

	Result = Judge_Edge(); 		//40000��CLK�ڲ鿴IO�ڵĵ�ƽ�ر仯
	if( Result == 0 ) //IO��û�г����½���
	{
		//if(CH == ESAM) Esam_Err_Report();
		return 2;	//�쳣: 40000��CLK��û���յ���ʼλ
	}
	
	for(n=0; n<13; n++)
	{
		Result = Get_7816_Char( CH, &Temp_ATR ); //receive TS.
		if( Result != 1 )
		{ 
			//if(CH == ESAM) Esam_Err_Report();
			return 3; 
		}
		Ic_Comm_RxBuf[ n ] = Temp_ATR;
	}
	
	if((Ic_Comm_RxBuf[0] != 0x3b) || (Ic_Comm_RxBuf[1] != 0x69))
	{
		//if(CH == ESAM)	Esam_Err_Report();
		return 3; 
	}
//	if(CH == ESAM) esam_error = 0;	
	return 0;
}

//ESAM��λ
unsigned char EsamReset( void )
{
	unsigned char i;
	
	for(i=0; i<3; i++)
	{
		if( FLAG_EA == 0 ) return(1);		//���粻����,����ֱ�ӷ���
		
		if(ColdReset( ESAM )) 
		{
//			CloseEsam();		//�ͷ�ESAM
			Delay_Initial(40);	//��ʱ200ms		
		}
		else
		{
			esam_error = 0;
			memcpy( EsamRst8B, Ic_Comm_RxBuf+5, 8 ); //ESAM���к�, 8bytes.
			return(0);
		}
	}
	Esam_Err_Report();
	return(1);
}

//��ֹESAM����
//void CloseEsam( void )
//{
//	RST_ESAM_L;
////	U7816CTRL0 &= B1101_1111;	//�ر�CLK
//	ESAMPOWEROFF;
//	
////	GPIO40CFG = B0100_0000;	 	//MCUCLK��Ϊ���
////	GPIO41CFG = B0100_0000; 	//SAMIO��Ϊ���
////	GPIO4 &= ( B1111_1100);		//������͵�ƽ	
//}

//��λESAM
unsigned char Clear_Rand_Gene( void )
{
	unsigned char result=0;
	
	Identitytimer.B32 = 0x00;
	identity_prog_flag = 0;	//��� ��ȫ��֤��̱�־
	terminal_auth_flag = 0;	//��� �����ն���֤��־
	first_prog_flag = 0;//���״α�̱�־

	memset( PcRand,  0x00, 8 );
	memset( PcRst8B, 0x00, 8 );

//	result = EsamReset();

	return (result);
}

//�ײ�ͨѶ
void RxTxCommhead( uint08 inc, uint08 com, uint08 P1, uint08 P2, uint08 len)
{
	Ic_Comm_TxBuf[ 0] = inc;
	Ic_Comm_TxBuf[ 1] = com;
	Ic_Comm_TxBuf[ 2] = P1;
	Ic_Comm_TxBuf[ 3] = P2;
	Ic_Comm_TxBuf[ 4] = len;
}

unsigned char ReadBinFile( unsigned char CH, unsigned char P1, unsigned char P2, unsigned char Len )
{return 0;

}
//���������ļ�
unsigned char ReadM1File( unsigned char  section,unsigned char Block )
{
	unsigned char Result;	
	
	Ic_Comm_TxBuf[0] = 0x30;   			//����ͻ�ȼ�0x93,0x95,0x97(������ط���ͻ)
	Ic_Comm_TxBuf[1] = section+Block;
	
	Result = RF_Command_Send(FM17520, 2);	//��������
	
	if(Result != 0x10) return 1;	
	return 0;
}
unsigned char WriteM1File( unsigned char  section,unsigned char Block, unsigned char *BufA, unsigned char Len)
{
	unsigned char Result;	
	
	Ic_Comm_TxBuf[0] = 0xa0;   			//
	Ic_Comm_TxBuf[1] = section+Block;
	
	Result = RF_Command_Send(FM17520, 2);	//��������
	
	if((Ic_Comm_RxBuf[0]!=0x0a)&&(Result != 1) ) return 1;	
	
	memcpy(&Ic_Comm_TxBuf[0],BufA, Len);
	
	Result = RF_Command_Send(FM17520, Len);	//��������
	
	if((Ic_Comm_RxBuf[0]!=0x0a)&&(Result != 1) ) return 1;	
	return 0;
}


//���¶������ļ�
unsigned char UpdateBinFile( uint08 CH, uint08 P1, uint08 P2, uint08 Len )
{
	unsigned char Result;

	RxTxCommhead(0, 0xD6, P1, P2, Len);
	
	memcpy(&Ic_Comm_TxBuf[5], Ic_Comm_RxBuf, Len);
				
	Result = CardTxRxFrame( CH, Len, 0x00 ); //CARD Com
	
	return (Result);
}

//����¼�ļ�
unsigned char ReadRecFile( unsigned char P1 )
{
	unsigned char Result;

	RxTxCommhead(0, 0xb2, P1, 0x14, 0x04);
	
	Result = CardTxRxFrame( ESAM, 0x00, 0x04 );

	return (Result);
}

//ȡ��Ӧ����
unsigned char Read_Kx( unsigned char CH, unsigned char Len )
{
	unsigned char result;

	RxTxCommhead(0, 0xC0, 0, 0, Len);
	
	result = CardTxRxFrame( CH, 0x00, Len ); //Esam Com

	return (result);
}

//ȡ�����
unsigned char ReadRandom( unsigned char CH, unsigned char Len )
{
	unsigned char result;
	
	RxTxCommhead(0, 0x84, 0, 0, Len);
	
	result = CardTxRxFrame( CH, 0x00, Len );
	
	if( result == 0 )
	{
		if(CH == ESAM)	//ȡESAM�����
		{
			memcpy(EsamRand, Ic_Comm_RxBuf, Len);
		}
		else		//ȡ��Ƭ�����
		{
			memcpy(CardRand, Ic_Comm_RxBuf, Len);
		}
	}
	
	return (result);
}

//���ܣ��ڲ����ⲿ��֤����
unsigned char Auth_In_Out( unsigned char INS, unsigned char CH, unsigned char P2, unsigned char *BufA )
{
	unsigned char Result;

	if(INS == 0xfa)
	{
		RxTxCommhead(0x80, INS, 0, P2, 0x08);
	}
	else
	{
		RxTxCommhead(0, INS, 0, P2, 0x08);
	}

	memcpy(&Ic_Comm_TxBuf[5],BufA, 8);	

	Result = CardTxRxFrame( CH, 0x08, 0x00 ); //CARD Com

	return (Result);
}

unsigned char Cal_Mac_Pro(unsigned char P2, unsigned char *BufA , unsigned char Len)
{
	unsigned char Result;

	RxTxCommhead(0x80, 0xfa, 1, P2, Len);		//����Աȣ�MAC����
	memcpy(&Ic_Comm_TxBuf[5],BufA, Len);	

	Result = CardTxRxFrame( ESAM, Len, 0x00 ); 	//CARD Com
	if(Result) return 1;
	
	Result = Read_Kx( ESAM, 64 );			//ȡESAM���ܽ��
	return (Result);
}


//�ɴ�MAC���������е�M2ѡ��
const uint08 code ConstIns[12] =
	{ 
		0x04, 0xd6, 0x86,	//������MAC����(0x86Ϊ��MAC��ESAM��)
		0x84, 0x32, 0x01,	//�û�����Ǯ���ļ�
		0x04, 0xdc, 0x01,	//Ԥ�ƿ���ʣ����
		0x04, 0xdc, 0x03,	//Ԥ�ƿ����������
	};


//=====��MAC��ȡ�������ļ�
//Le����MAC����������ݳ���
unsigned char ReadBinWithMac( uint08 CH, uint08 P1, uint08 P2, uint08 Le )
{
	unsigned char result, Temp, Type;
	
	RxTxCommhead(0x04, 0xB0, P1, P2, 0x11);
	Clear_Wdt();
	
	Type = (CH&B1111_0000)>>4;
	if( (CH&B0000_1111) == CARD )	//��CARD����
	{
		Ic_Comm_TxBuf[4] = 9;
		memcpy(Ic_Comm_TxBuf+5, EsamRand, 4);//�Կ�Ƭ������ȡESAM�����
		
		memcpy( Ic_Comm_TxBuf+9, ConstIns+Type*3, 3 );
		if( Type == 0 )
		{
			Ic_Comm_TxBuf[11] = P1;	//��MAC����Ƭ�������ļ��ź�ƫ�Ƶ�ַͬESAM
			if( P1==0x85 ) Ic_Comm_TxBuf[11] = 0x87;//��բ��������
			Ic_Comm_TxBuf[12] = P2;
		}
		else
		{
			Ic_Comm_TxBuf[12] = 0x14;
		}
		Temp = Le;	//��Ƭֱ�ӷ������ݣ�����ȡ��
	}
	else//��ESAM����
	{
		if( Comm_Auth_Judge() )//Զ�̴�mac��
			memcpy(Ic_Comm_TxBuf+5, PcRand, 4);
		else
			memcpy(Ic_Comm_TxBuf+5, CardRand, 4);

		memcpy( Ic_Comm_TxBuf+9, ConstIns, 3 );
		
		Ic_Comm_TxBuf[12] = P2;
		if( P1<0x04 )
		{
			Ic_Comm_TxBuf[ 1] = 0xB2;	//��MAC��Ǯ����������0xB2
			Ic_Comm_TxBuf[12] = 0x00;
		}
		
		if(CH == ESAM)
			memcpy(Ic_Comm_TxBuf+14, CardRst8B, 8);
		else
			memcpy(Ic_Comm_TxBuf+14, PcRst8B, 8);
		
		Temp = 0;
	}
	Ic_Comm_TxBuf[13] = Le;
	if(CH == ESAM+0x10) Ic_Comm_TxBuf[13] += 8;	//���ݻس���״̬��ѯ��+8�ֽ�
	
	CH &= B0000_0001;
	result = CardTxRxFrame( CH, Ic_Comm_TxBuf[4], Temp );//����data+MAC
	if( result ) return (result);
	
	if(CH == ESAM)
	{
		result = Read_Kx(ESAM, Le ); //����data+MAC
	}

	return ( result );
}

//======��MAC���¶������ļ�
unsigned char WriteBinWithMac( uint08 CH, unsigned char P1, unsigned char P2, unsigned char Len )
{
	unsigned char  Result;

	RxTxCommhead(0x04, 0xd6, P1, P2, Len);
	if(CH>1) Ic_Comm_TxBuf[1] = 0xdc;		//��MACдǮ���ļ�
	if(Len == 4) Ic_Comm_TxBuf[ 0 ] = 0;//CLA ����MAC
	CH &= B0000_0001;

	memcpy(&Ic_Comm_TxBuf[5], Ic_Comm_RxBuf, Len);

	Result = CardTxRxFrame( CH, Len, 0x00 ); //CARD Com

	if( Result )
	{
		if( (Err_Sw12[0] == 0x69) && (Err_Sw12[1] == 0x88) ) return(IC_MAC_ERR);//MACУ�����(6988)
		else if(CH == ESAM) return(IC_ESAM_RDWR_ERR);
		else return(IC_NOIN_ERR);
	}
	return(Result);
}


unsigned char Authorization( unsigned char *Buf, unsigned char Len )
{
	unsigned char Result;

	//���ܱ��ESAM����������⣺
	//a����Կ��ɢ���80fa000108+8�ֽڷ�ɢ���ӣ�
	//;Esam encryp random data.
	Result = Auth_In_Out(0xfa, ESAM, 0x03, Buf+16 );
	if( Result != 0x00 )	return 1;
	
	//b��������������80fa000008+�������
	Result = Auth_In_Out(0xfa, ESAM, 0x00, Buf+8 );
	if( Result != 0x00 )	return 1;
	
	//c��ȡ�������00c0000008��
	//�Ƚ�����1������2, ���߲�����򱨾�, ���������������
	//;Read K2 from Esam
	Result = Read_Kx( ESAM, 0x08 );
	if( Result != 0x00 )	return 1;
	
	if( Len == 4 )
	{
		if( Cmp_Data(Buf, Ic_Comm_RxBuf, 8) != 3 ) return 1;//Reset failure.
	}
	else
	{
		memcpy( Buf, Ic_Comm_RxBuf, 8 );
	}

	//ȡ�����2��0084000004
	//;Read random in ESAM
	Result = ReadRandom( ESAM, Len );
	return ( Result );
}


//ѡ���ļ�
unsigned char SelectFile( unsigned char CH, unsigned char F1, unsigned char F2 )
{
	unsigned char Result;
	
	RxTxCommhead(0,0xa4, 0, 0, 2);	
	Ic_Comm_TxBuf[ 5 ] = F1;	//DATA0
	Ic_Comm_TxBuf[ 6 ] = F2;	//DATA1
	
	Result = CardTxRxFrame( CH, 0x02, 8 );	//����8�ֽ�
	
	return (Result);
}


//�жϿ�Ƭ���ݸ�ʽ
unsigned char JudgeFile(unsigned char Len)
{
	unsigned char Csdata;

	Csdata = Cal_ChkSum(Ic_Comm_RxBuf+1, Len+3);
	if( (Ic_Comm_RxBuf[0] != 0x68) || (Ic_Comm_RxBuf[Len+4] != Csdata) || (Ic_Comm_RxBuf[Len+5] != 0x16) ) return 1;//֡��ʽ����: 68H/CS/16H
	return 0;
}

//��Ƭ�ڲ���֤
unsigned char Auth_CARD_IN(unsigned char CH)
{
	unsigned char Result;
	
	RxTxCommhead(0, 0x83, 1, 2, 0x10);
	
	if(CH == ESAM)
	{
		memcpy(&Ic_Comm_TxBuf[5], CardRst8B, 8);	
		memcpy(&Ic_Comm_TxBuf[5+8], CardRand, 8);
		Result = CardTxRxFrame( CH, 0x10, 0x00 );
	}
	else
	{
		Ic_Comm_TxBuf[ 1] = 0x86;
		memcpy(&Ic_Comm_TxBuf[5], Ic_Comm_RxBuf, 16);
		Result = CardTxRxFrame( CH, 0x10, 0x08 );
	}
	return (Result);
}

//ϵͳ�ڲ���֤
unsigned char Inter_Auth(void)
{
	unsigned char Result;
	unsigned char idata  K1[8];
	
	
	Result = ReadRandom( CARD, 8 );			//��CARDȡ8�ֽ������
	if(Result) return(IC_NOIN_ERR);
		
	Result = Auth_CARD_IN(ESAM);
	if(Result) return(IC_ESAM_RDWR_ERR);
	
	Result = Read_Kx(ESAM, 0x18 );			//ȡESAM���ܽ��K1
	if(Result) return(IC_ESAM_RDWR_ERR);
		
	memcpy(K1, Ic_Comm_RxBuf+16, 8);
	
	Result = Auth_CARD_IN(CARD);
	if(Result) return(IC_NOIN_ERR);
	
	if ( Cmp_Data( K1, Ic_Comm_RxBuf, 8) != 3 ) return(IC_AUTH_ERR);//�����֤����(ͨ�ųɹ������Ĳ�ƥ��)
	
	return 0;
}

//��Ƭ�ⲿ��֤
unsigned char Card_Ext_Auth( unsigned char P2 )
{
	unsigned char Result;

	Result = ReadRandom( CARD, 8 );			//�ӿ�Ƭȡ8�ֽ������
	if(Result) return(IC_NOIN_ERR);
	
	Result = Auth_In_Out(0xfa, ESAM, 0x04, CardRst8B ); //ESAM��ɢ�ⲿ��֤��Կ
	if(Result) return(IC_ESAM_RDWR_ERR);
	
	Result = Auth_In_Out(0xfa, ESAM, 0x00, CardRand );//ESAM���������
	if(Result) return(IC_ESAM_RDWR_ERR);
	
	Result = Read_Kx( ESAM, 0x08 );			//ȡ���ܽ��
	if(Result) return(IC_ESAM_RDWR_ERR);

	Result = Auth_In_Out(0x82, CARD, P2, Ic_Comm_RxBuf );//��Ƭ���ⲿ��֤
	if( Result ) return(IC_EXAUTH_ERR);

	return 0;
}


//======��MAC������
unsigned char IncreseWithMac( void )
{
	unsigned char  Result;

	RxTxCommhead(0x84, 0x32, 0x01, 0x14, 8+4);

	memcpy(&Ic_Comm_TxBuf[5], Ic_Comm_RxBuf, 8+4);
			
	Result = CardTxRxFrame( ESAM, 8+4, 0x00 ); //CARD Com
		
	if( Result )
	{
		if( (Err_Sw12[0] == 0x69) && (Err_Sw12[1] == 0x88) ) return(IC_MAC_ERR);//MACУ�����(6988)
		else return(IC_ESAM_RDWR_ERR);	//����ESAM����
	}
	return (Result);
}


//======��MAC�ۿ����
unsigned char DecreaseWithMac( unsigned char *InBuf )
{
	unsigned char Result;
	
	//04D68C0014+����+MAC
	//00B08C0008
	//8430020C08+4�ֽ��˷ѽ��+4�ֽ�MAC
	Result = Update_ESAM_Bin_Mac( 0x99, 0x00, InBuf, 16 );
	if( Result ) return 0x04;

	Result = ReadBinFile( ESAM, 0x99, 0x00, 8 );
	if( Result ) return 0x01;

	RxTxCommhead( 0x84, 0x30, 0x02, 0x14, 0x08 );

	memcpy(&Ic_Comm_TxBuf[5], Ic_Comm_RxBuf, 8);
	memcpy(InBuf, &Ic_Comm_TxBuf[5], 8);
	
	Result = CardTxRxFrame( ESAM, 8, 0x00 ); //CARD Com
	if( Result ) return 0x04;
	
	return 0x00;
}

//��⿨Ƭ
unsigned char Check_Card( void )
{
	if( (CardCheck == 0) && (CardKeySamp==0xff) )
	{
		CardCheck = 1;			//���¿�Ƭ����
		Beep_Timer = 0;
		return 0;
	}
	
	if( (CardCheck == 1) && (CardKeySamp==0x00) )
	{
		CardCheck = 0;			//��Ƭ�Ѿ�����
	}
	return 1; 				//û���¿�Ƭ����
}

//�ж�ʣ��������բ���
void Judge_Closerelay_Money()
{			
	uint08 Result;
	Result = Cmp_Data( Money.Remain, Money_Para.Closerelay.B08, 4 );
	if( Result > 1)//ʣ����<=��բ�޶�
	{
		if( remainder_maxtick_flag && relay_status_flag ) //͸֧��բ���жϺ�բ��������		
		{
			charge_relay_open_flag = 1;
		}
	}
	else
	{
		charge_relay_open_flag = 0;
		remainder_maxtick_flag = 0;
	}
}


void Get_Usercard_Serial(void)
{
	unsigned char Buf[16];
	
	I2C_Read_Eeprom44(ee_usercard_serial_page, Buf, ee_usercard_serial_lenth);
	if(Buf[4] == 0x55) card_open_flag = 1;	//�����ѿ���
	else card_open_flag = 0;
	if(Buf[5] == 0x55) comm_open_flag = 1;	//Զ���ѿ���
	else comm_open_flag = 0;
	UserCardSerialBit = 0;
	if ( Cmp_Data( UID, Buf, 4) != 3 ) UserCardSerialBit = 1;	//����Ų����
}

//�����û������кźͿ�����־
//Type=0: ���汾�ؿ�����־���û������к�
//Type=1: ����Զ�̿�����־
void Updata_Usercard_Serial( unsigned char Type )
{
	unsigned char Buf[16];
	
	I2C_Read_Eeprom44(ee_usercard_serial_page, Buf, ee_usercard_serial_lenth);
	if(Type == 0)
	{
		if(UserCardFlag == 0x01) Buf[4] = 0x55;	//ֻ�п������ñ��ؿ�����־
		memcpy(Buf, UID, 4);
	}
	if(Type == 1)	Buf[5] = 0x55;	//Զ�̿�����־
	I2C_Write_Eeprom44(ee_usercard_serial_page, Buf, ee_usercard_serial_lenth);
}

//����esam�������ݵ�ee
void Store_Esampara(void)
{
	WriteE2WithBackup(ee_esampara_page, ee_esampara_inpage, Esam_Para.Alarm_Limit1.B08, ee_esampara_lenth);
	I2C_Read_Eeprom(ee_esampara_page, ee_esampara_inpage, Esam_Para.Alarm_Limit1.B08, ee_esampara_lenth);
}


//����ʣ�������ݵ�ee
void Store_Remainmoney(void)
{
	WriteE2WithBackup( ee_remainmoney_page, ee_remainmoney_inpage, Remain_back, ee_remainmoney_lenth );
	I2C_Read_Eeprom(ee_remainmoney_page, ee_remainmoney_inpage, Money.Remain, ee_remainmoney_lenth);
}

//׼��Ǯ������
//���ݱ�������ʣ����BCD6(�ֽ�)+͸֧���BCD(6�ֽ�)+�������HEX(4�ֽ�)+����ǰʣ����BCD(4�ֽ�)
unsigned char Ready_Remainback(void)
{
//	unsigned char result;
	
//	result = ReadRecFile(3);		//��Ǯ���������
//	if(result) return 1;
	
	memcpy( Remain_back, Money.Remain, 12 );				//ʣ��͸֧BCD�ŵ�������
	memset( Remain_back+12, 0, 4 );			//���������esamΪ׼
	memcpy( Remain_back+16, Money.Remain+2, 4); 			//��ֵǰʣ����
	if(remainder_tick_flag)
	{
		memcpy( Remain_back+20, Money.Tick+2, 4); 	//��ֵǰ͸֧���
		disp_over_flag = 1;
	}
	return 0;
}

//Ǯ������
unsigned char Moneybag_Process(void)		
{
	unsigned char i, result;
	union B32_B08 Temp32_Ram, Temp32_Esam;
	unsigned char BufA[64];

	for(i=0; i<4; i++)
	{
		result = ReadRecFile((i&B0000_0010)+1);		//��Ǯ��ʣ����(1)�͹������(3)
		if(result) return(result);
		memcpy( BufA+4*i, Ic_Comm_RxBuf, 4 );
	}
	
	if( (Cmp_Data( BufA, BufA+4, 4) != 3) || (Cmp_Data( BufA+8, BufA+12, 4) != 3)  ) return 0;//���ζ���һ�����˳�����
	
	Remainmoney_Verify();	//ʣ�������У��
	i = Cmp_Data( BufA+8, Money.Buy_Times.B08, 4);		//�ȽϹ�������Ƿ����,i=3��ʾ���
	memcpy( Temp32_Esam.B08, BufA, 4 );			//Temp32_Esam �ݴ�ESAM ʣ����
	Bcd2Hex(&Money.Remain[0], Temp32_Ram.B08, 4);		//Temp32_Ram �ݴ�ʣ����HEX
	
	if( (i==3) && (Temp32_Esam.B32 > Temp32_Ram.B32) )	//����������ͬʱʣ��ram<Ǯ��,��ramΪ׼
	{		
		Temp32_Esam.B32 -= Temp32_Ram.B32;		//Ǯ�� - ʣ��
		
		memcpy(&Ic_Comm_TxBuf[5], Temp32_Esam.B08, 4);
	
		RxTxCommhead( 0x80, 0x30, 0x01, 0x14, 0x05 );
		
		Ic_Comm_TxBuf[ 9 ] = Cal_ChkSum(Ic_Comm_TxBuf, 9);
	
		CardTxRxFrame( ESAM, 0x05, 0x00 );
		
		if((Decimal.Status & B0000_0001) == 0) WriteE2WithBackup( ee_remainmoney_page, ee_remainmoney_inpage, Money.Remain, ee_remainmoney_lenth );//ʣ����д����ee(������ͬ��)	
	}
	else if( (i!=3) || (Temp32_Esam.B32 < Temp32_Ram.B32) )	//����������Ȼ���ʣ��ram>Ǯ��,��esamΪ׼
	{
		//Ǯ�����ָ�����ee���
		memset( Remain_back ,0 ,ee_remainmoney_lenth );
		Hex2Bcd( Temp32_Esam.B08, Remain_back, 4 );	//Ǯ��ʣ����ת����ramʣ����
		memcpy( Remain_back+12, BufA+8, 4 );		//���������esamΪ׼
		
		memcpy( BufA+16, Sclock.Clockbackup, 7 );	//���ݸ���ʱ��
		memcpy( BufA+23, Temp32_Ram.B08, 4 );		//���ݸ���ǰramʣ����
		memcpy( BufA+27, Money.Buy_Times.B08, 4 );	//���ݸ���ǰram�������
		I2C_Write_Eeprom( ee_esam_money_page, ee_esam_money_inpage, BufA+16, ee_esam_money_lenth );//�ָ�����
		run_alarm_flag = 1;
		
		Store_Remainmoney();	//����ʣ�������ݵ�ee	
	}
	
	Decimal.Status &= B1111_1101;		//����仯��־
	
	return 0;	
}

//Ǯ������1(������ͬ��ר��)
void Moneybag_Process1(void)
{
	if( Moneybag_Process() ) 
	{
		EsamReset();		//��λ
		Moneybag_Process();	//�ٴ�Ǯ������
	}
}

//Ǯ������2(ͨѶר��)
void Moneybag_Process2(void)
{
	if( (Decimal.Status & B0000_0010) == 0 ) return;	//���δ�����仯������Ǯ��
	
	if( UpdateMoneytimer <= default_updatemoneytimer )
	{
		Moneybag_Process();//����ESAMǮ��
		UpdateMoneytimer = default_updatemoneytimer+15;//����Ҫ��15min�ڵ�������ѯ�������,ֻҪ��һ�οۼ�
	}
}


void GetBuyTimes2Byte( unsigned char Offset )
{
	union B32_B08 Temp32;
	
	Hex2Bcd( Money.Buy_Times.B08, Temp32.B08, 4 );
	Eeprom.Buffer[Offset+0] = 0;	//�������(bcd)
	Eeprom.Buffer[Offset+1] = 0;
}

//�Ƚϱ���Ƿ���ͬ
unsigned char Cmp_Meter_Id(unsigned char *Buf)
{
	if ( Cmp_Data( Esam_Para.Meter_Id, Buf, 6) != 3 ) return 1;
	return 0;
}

//�쳣�忨��¼
void Card_Err_Record(void)
{
	if( (Keyswitch_Valid == 0x55)  && (Ic_Err_Reg == IC_NOIN_ERR) && (Find_Card != 0x55)) Ic_Err_Reg = IC_PREIC_ERR;	//��ǰ�ο�
	
	//------ �쳣�忨 ��¼------
	memcpy( Eeprom.Buffer+6, UID, 4);				//�����к�
	memset(Eeprom.Buffer+10, 0x00, 4);
	Eeprom.Buffer[14] = Ic_Err_Reg;				//������Ϣ��
	
	if((Ic_Err_Reg==IC_ESAM_RDWR_ERR)||(Ic_Err_Reg==IC_NOFX_ERR)||(Ic_Err_Reg==IC_MAC_ERR) 
		  ||(Ic_Err_Reg == IC_NOIN_ERR)||(Ic_Err_Reg == IC_EXAUTH_ERR))
	{
		Exchange( Eeprom.Buffer+15, Ic_Comm_TxBuf, 5 );	//��������ͷ
		Eeprom.Buffer[20] = 0xFF;	//������Ӧ״̬SW2
		Eeprom.Buffer[21] = 0xFF;//������Ӧ״̬SW1
	}
	else
	{
		memset(Eeprom.Buffer+15, 0x00, 7);
	}
	
	GetBuyTimes2Byte(22);	//�������(bcd)
	
	memcpy( Eeprom.Buffer+24, Money.Remain+2, 4 );	//ʣ����
	
	DltoEepromBuf( 28 );	//�������й��ܵ���
	
	Event_Record( ee_errcardn_inpage );
	//------------- end ----------------
	
	//��¼�Ƿ��忨�����������֤����MACУ������û����ⲿ��֤����
	if((Ic_Err_Reg == IC_AUTH_ERR) || (Ic_Err_Reg == IC_MAC_ERR) || (Ic_Err_Reg == IC_EXAUTH_ERR) || (Ic_Err_Reg == IC_NOFX_ERR))
	{
		Keeptime.Times[0].B32++; //�ܴ���+1
	}
}

//����/�˷Ѽ�¼
void Buy_Record(unsigned char Inpage)
{
	unsigned char result;
	
	if( Inpage == ee_buymoneyn_inpage )
	{
		result = Read_Event_Point( Inpage );	//�����¼���¼ָ��
		I2C_Read_Eeprom( ee_buydl_note_page+result, ee_buydlnote_inpage, Eeprom.Buffer, ee_buydlnote_lenth );
	}
	
	GetBuyTimes2Byte(5); //�������(bcd)
	memcpy( Eeprom.Buffer+7, Buy_Money_Bcd, 4 );		//������
	memcpy( Eeprom.Buffer+11, Remain_back+16, 4 );	//����ǰʣ����
	
	memcpy( Eeprom.Buffer+15, Money.Remain+2, 4 );		//�����ʣ����
	
	if( Inpage == ee_buymoneyn_inpage )
	{
		if( RdRecTimes( Inpage ) == 0 )//�ж���Чָ��,���Ϊ0��ͷ��ʼ�������
		{
			memcpy( Eeprom.Buffer+19, Buy_Money_Bcd, 4 );	//������۹����
		}
		else
		{
		//	Inverse( Eeprom.Buffer+19, 4 );		//ת���ɸ��ֽ���ǰ
			Add_Bcd( Eeprom.Buffer+19, Buy_Money_Bcd, 4);	//������۹����
			//Inverse( Eeprom.Buffer+19, 4 );	//ת���ɵ��ֽ���ǰ
		}
		
		memset( Eeprom.Buffer, 0xff, 4 );	//�����ߴ������ó�0xff
		I2C_Write_Eeprom( ee_operatorcode_page, ee_operatorcode_inpage, Eeprom.Buffer, 4 );
	}
	
	Event_Record( Inpage );
}

//==================================================
//P1 != 0����MAC�ӿ�Ƭ�����ݵ�������
//P1 == 0������������+MACд��ESAM
//==================================================
unsigned char Backup_With_Mac(uint08 Type)
{
	unsigned char Result, Ramaddr;
	
	Ramaddr = Type*3;
	Result = ReadBinWithMac( CARD, BACKBUF[Ramaddr], BACKBUF[Ramaddr+1], BACKBUF[Ramaddr+2] );	//��MAC����Ƭ��ز���
	memcpy(&Refers_File_Mac[BACKADDR[Type]], Ic_Comm_RxBuf, BACKBUF[Ramaddr+2]);//�������+MAC����Ӧ������
	return( Result );
}

unsigned char BackupW_With_Mac(uint08 Type)
{
	unsigned char Result, Ramaddr;
	
	Ramaddr = Type*3;
	memcpy(Ic_Comm_RxBuf, &Refers_File_Mac[BACKADDR[Type]], BACKBUF[Ramaddr+2]);											//����RAM����������
	Result = WriteBinWithMac( ESAM, BACKBUF[Ramaddr], BACKBUF[Ramaddr+1], BACKBUF[Ramaddr+2] );//��MACд��ESAM��ز���
	return( Result );
}

void Updata_Feilv_Page(unsigned char CH)
{
	Clear_Wdt();
	I2C_Write_Eeprom(ee_tariffprice1_page+CH, 0, &Refers_File_Mac[BACKADDR[CH+4]], ee_tariffprice_lenth);//��1�׷��ʵ��
	Price.Crc16c.B16 += 1;	
}

//���µ�ǰ(0/1)/������(2/3)���ݵ��������ݵ�ۡ��������
void Updata_Ladder_Page(unsigned char CH)
{
	unsigned char Temp08;
	
	Clear_Wdt();
	Temp08 = CH;
	if(Temp08 > 1) Temp08 += 2; 
//	LoopInverse(&Refers_File_Mac[BACKADDR[6+CH*2]], 6, 3 ); //��������ֽڽ���˳��
//	memcpy(&Refers_File_Mac[BACKADDR[6+CH*2]]-4, &Refers_File_Mac[BACKADDR[6+CH*2]], 18);
	
	//���ݵ��������
	WriteE2WithJieti(Temp08, &Refers_File_Mac[BACKADDR[6+CH*2]]);
	RefreshPrice();
}

//==================================================
//��MAC������ز�����Ϣ��EEPROM
//Fefer1_File_Mac�� �û�����+�������±�־+Ԥ��4�ֽ�+MAC
//Fefer2_File_Mac�� �������1��2+��ѹ���������������+MAC
//Feilv_File1_Mac�� �����ļ�1+MAC
//Feilv_File2_Mac�� �����ļ�2+MAC
//Feilv_Change_Mac�������ļ��л�ʱ��+MAC
//Ladder_File1_Mac����1�׽���ֵ+���ݵ��+MAC
//Ladder_File2_Mac����2�׽���ֵ+���ݵ��+����ʱ��+MAC
//==================================================
unsigned char UpdataRefer_ToEe_WithMac(void)
{
	unsigned char Buf[10];
	
	Exchange( Buf, UID, 4 );	//�����ߴ��루����ź�4�ֽڣ�
	
//	//���ݲ������±�־λbit7��д����+MAC
//	if((UpdataFlag & B1000_0000)!=0)
//	{
//		memcpy(Esam_Para.Alarm_Limit1.B08, &Refers_File_Mac[BACKADDR[1]+1], 14);//����EE����		
//		Store_Esampara();//����esam�������ݵ�ee
//	}
	
	//���ݲ������±�־λbit0����Ƭ��ǰ�׷��ʵ���ļ������±��ڵ�ǰ�׷��ʵ�۲���
	if((UpdataFlag & B0000_0001)!=0)
	{
		if( CardType == UserCardCommand ) FLRecord( Buf );	//���ʱ�̼�¼
		Updata_Feilv_Page(0);//��ǰ�׷��ʵ��
	}
			
	//���ݲ������±�־λbit1����Ƭ���÷��ʵ���ļ������±��ڱ����׷��ʵ�۲���
	if((UpdataFlag & B0000_0010)!=0)
	{
 		if( CardType == UserCardCommand ) FLRecord( Buf );	//���ʱ�̼�¼
 		Updata_Feilv_Page(1);//�����׷��ʵ��
		memcpy( Starttime.Rate, &Refers_File_Mac[BACKADDR[2]], 5 );	//������������
	}
	
	//���ݲ������±�־λbit2����Ƭ��ǰ�׽����ļ������±��ڵ�ǰ�׽��ݵ���+���ݵ��+������ղ���
	if((UpdataFlag & B0000_0100)!=0)
	{
		if(CardType == UserCardCommand ) JTRecord( Buf );	//���ݱ�̼�¼
		
		Updata_Ladder_Page(0);	//��ǰ�׽���1����
		Updata_Ladder_Page(1);	//��ǰ�׽���2����
	}
	
	//���ݲ������±�־λbit3����Ƭ�����׽����ļ������±��ڱ����׽��ݵ���+���ݵ��+�������+����ʱ�����
	if((UpdataFlag & B0000_1000)!=0)
	{
		if(CardType == UserCardCommand ) JTRecord( Buf );	//���ݱ�̼�¼
		
		Updata_Ladder_Page(2);	//�����׽���1����
		Updata_Ladder_Page(3);	//�����׽���2����
		memcpy( Starttime.Ladder, &Refers_File_Mac[BACKADDR[3]], 5 );//������������
	}
	
	//��ʱ����RAM���ݣ����¼��㵱ǰ���
	if((UpdataFlag & B0000_1111)!=0)
	{
		run_price_flag = 1;	//���²��ҵ�ǰʱ�μ����
		if((UpdataFlag & B0000_1010)!=0)//���·����л�ʱ�����ݵ���л�ʱ��
		{
			WriteE2WithBackup(ee_starttimer_page, ee_starttimer_inpage, Starttime.Shiqu, ee_starttimer_lenth);
			Starttime.Crc16.B08[0] ++;
			
			if( (CardType == PresetCardCommand) //Ԥ�ÿ����׷����л�ʱ�估���׽����л�ʱ���ж�(��Լ������)
			&& (((Cmp_Clock( Starttime.Ladder, 5 ) != 0x01) && ChkBufZero( Starttime.Ladder, 5 ))
			|| ((Cmp_Clock( Starttime.Rate, 5 ) != 0x01) && ChkBufZero( Starttime.Rate, 5 ))) )
			{
				Year_Ladder_Judge();	//����������жϺ����ݺϷ����ж�
				Price_Judge();		//���ʽ��ݵ���ж�
			}
		}
	}
	
	if( terminal_auth_flag ) return 0; //�����ն˲��ڴ�����̼�¼
	
	//��̼�¼
	if((CardType == UserCardCommand) && (UpdataFlag & B1000_1111))//�û���
	{
		Buf[0] = UpdataFlag;			//���±�־
		Buf[1] = CardType;			//�翨����
		Buf[2] = 0x98;				//˽Կ
		Buf[3] = 0x99;				//��ʶ
		Exchange( Buf+4, UID, 4 );	//�����ߴ��루����ź�4�ֽڣ�
		ProgramRecord( Buf, Buf+4 );
	}
	
	return 0;
}

const uint08 code WRNUM[8] =	{ 1, 2, 4, 4, 0, 0, 0, 1};//��Ӧ�����޸�ʱbit0~7��Ҫ��д�Ĳ�������
//==================================================
//��MAC��д��������
//�ֱ��дUpdataFlag��bit0~7
//Type��0����1д
//==================================================
unsigned char UpdataRefer_WithMac(unsigned char Type)
{
	unsigned char Result;
	union B16_B08 CRCtemp[2];
	
	if( (UpdataFlag&B0000_0001))	
	{	
		Result = Inter_Authrf08( SECTION2 );	    
		if(Result) return(IC_AUTH_ERR);
		
		Result = ReadM1File( SECTION2,BLOCK0 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[4]], Ic_Comm_RxBuf, 16);//		
	
		Result = ReadM1File( SECTION2,BLOCK2 );
	  if( Result )return (IC_NOIN_ERR);	
		
		memcpy(CRCtemp[1].B08, Ic_Comm_RxBuf, 2);		
		if( CRCtemp[1].B16 != Crc_16Bits( &Refers_File_Mac[BACKADDR[4]], 16 ) ) return IC_FORMAT_ERR;	
	}
	Clear_Wdt();
	if( (UpdataFlag&B0000_0010))	
	{	
		Result = Inter_Authrf08( SECTION2 );	    
		if(Result) return(IC_AUTH_ERR);
		
		Result = ReadM1File( SECTION2,BLOCK1 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[5]], Ic_Comm_RxBuf, 16);//
		
		Result = ReadM1File( SECTION2,BLOCK2 );
	  if( Result )return (IC_NOIN_ERR);		
		memcpy(CRCtemp[1].B08, &Ic_Comm_RxBuf[2], 2);
		if( CRCtemp[1].B16 != Crc_16Bits( &Refers_File_Mac[BACKADDR[5]], 16 ) ) return IC_FORMAT_ERR;	
		
		memcpy(CRCtemp[1].B08, &Ic_Comm_RxBuf[9], 2);
		if( CRCtemp[1].B16 != Crc_16Bits( &Ic_Comm_RxBuf[9], 2 ) ) return IC_FORMAT_ERR;	
	}
	Clear_Wdt();
	if( (UpdataFlag&B0000_0100))	
	{	
		Result = Inter_Authrf08( SECTION3 );	    
		if(Result) return(IC_AUTH_ERR);
		
		Result = ReadM1File( SECTION3,BLOCK0 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[6]], Ic_Comm_RxBuf, 16);//		
	
		Result = ReadM1File( SECTION3,BLOCK1 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[6]+16], Ic_Comm_RxBuf, 8);//		
	  memcpy(CRCtemp[1].B08, &Ic_Comm_RxBuf[2], 2);
		if( CRCtemp[1].B16 != Crc_16Bits( &Refers_File_Mac[BACKADDR[6]], 24 ) ) return IC_FORMAT_ERR;		
		Clear_Wdt();
		Result = Inter_Authrf08( SECTION5 );	    
		if(Result) return(IC_AUTH_ERR);
		
		Result = ReadM1File( SECTION5,BLOCK0 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[6]+24], Ic_Comm_RxBuf, 16);//		
	
		Result = ReadM1File( SECTION5,BLOCK1 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[6]+40], Ic_Comm_RxBuf, 12);//		
	  memcpy(CRCtemp[1].B08, &Ic_Comm_RxBuf[12], 2);
		if( CRCtemp[1].B16 != Crc_16Bits( &Refers_File_Mac[BACKADDR[6]+24], 28 ) ) return IC_FORMAT_ERR;		
		
		Result = ReadM1File( SECTION5,BLOCK2 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[7]], Ic_Comm_RxBuf, 12);//		
		memcpy(CRCtemp[1].B08, &Ic_Comm_RxBuf[12], 2);
		if( CRCtemp[1].B16 != Crc_16Bits( &Refers_File_Mac[BACKADDR[7]], 12 ) ) return IC_FORMAT_ERR;		
		
	}
	Clear_Wdt();
	if( (UpdataFlag&B0000_1000))	
	{	
		Result = Inter_Authrf08( SECTION4 );	    
		if(Result) return(IC_AUTH_ERR);
		
		Result = ReadM1File( SECTION4,BLOCK0 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[8]], Ic_Comm_RxBuf, 16);//		
	
		Result = ReadM1File( SECTION4,BLOCK1 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[8]+16], Ic_Comm_RxBuf, 8);//		
	  memcpy(CRCtemp[1].B08, &Ic_Comm_RxBuf[2], 2);
		if( CRCtemp[1].B16 != Crc_16Bits( &Refers_File_Mac[BACKADDR[8]], 24 ) ) return IC_FORMAT_ERR;		
		
		Result = ReadM1File( SECTION4,BLOCK2 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[3]], Ic_Comm_RxBuf, 5);//		
	  memcpy(CRCtemp[1].B08, &Ic_Comm_RxBuf[5], 2);
		if( CRCtemp[1].B16 != Crc_16Bits( &Refers_File_Mac[BACKADDR[3]], 5 ) ) return IC_FORMAT_ERR;	
	  Clear_Wdt();	
		Result = Inter_Authrf08( SECTION6 );	    
		if(Result) return(IC_AUTH_ERR);
		
		Result = ReadM1File( SECTION6,BLOCK0 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[8]+24], Ic_Comm_RxBuf, 16);//		
	
		Result = ReadM1File( SECTION6,BLOCK1 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[8]+40], Ic_Comm_RxBuf, 12);//		
	  memcpy(CRCtemp[1].B08, &Ic_Comm_RxBuf[12], 2);
		if( CRCtemp[1].B16 != Crc_16Bits( &Refers_File_Mac[BACKADDR[8]+24], 28 ) ) return IC_FORMAT_ERR;		
		
		Result = ReadM1File( SECTION6,BLOCK2 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&Refers_File_Mac[BACKADDR[9]], Ic_Comm_RxBuf, 12);//		
		memcpy(CRCtemp[1].B08, &Ic_Comm_RxBuf[12], 2);
		if( CRCtemp[1].B16 != Crc_16Bits( &Refers_File_Mac[BACKADDR[9]], 12 ) ) return IC_FORMAT_ERR;		
		
	}
	Clear_Wdt();
	return 0;
}

//==================================================
//��ʼ��Ǯ���ļ�
//Buy_Money_Mac����ֵ���+MAC
//Buy_Times_Mac���������+MAC
//Buy_Money_Bcd�����ι�����BCD
//==================================================
unsigned char Init_Money(void)
{
//	unsigned char Result;
	
//	memcpy(Ic_Comm_RxBuf, Buy_Money_Mac, 4+4);							//��ֵ���+MAC
//	Result = WriteBinWithMac(ESAM+0x10, 0x01, 0x14, 4+4 );	//����Ǯ�����
//	if( Result ) return (Result);
//	
//	memcpy(Ic_Comm_RxBuf, Buy_Times_Mac, 4+4);				//�������+MAC
//	Result = WriteBinWithMac(ESAM+0x10, 0x03, 0x14, 4+4 );	//����Ǯ���������
//	if( Result ) return (Result);
	
	Hex2Bcd(Buy_Money_Mac[0].B08, Buy_Money_Bcd, 4);			//������HEXת����BCD
	memset(Remain_back, 0, 20);				//��͸֧���������,����ǰʣ����
	memcpy(Remain_back, Buy_Money_Bcd, 4);	//������Ϊʣ����
	
	//����RAM��EE��ʣ�࣬͸֧������
	Store_Remainmoney();		//����ʣ�������ݵ�ee	
	
	//�ж�ʣ��������բ���
	charge_relay_open_flag = 0;//��С�ں�բ��������
	remainder_maxtick_flag = 0;
	card_closerelay_flag   = 0;
	card_closerelay_flag1  = 0;
	Store_Meterstatus();

	return 0;
}

//==================================================
//�жϹ���������ڻ���͸֧���
//��ڣ�
//		  Buy_Money_Bcd��	 ���γ�ֵ���BCD
//			Buy_Times�����ι������
//			Remain_back��	 ����ԭʣ����+͸֧���+�������
//���ڣ�
//00������������
//01�����������1
//02�����������ֵ>=2�������������
//55�����������1�������ڻ������
//FF: ������� < ���ڹ������
//Remain_back����ֵ��ʣ����BCD
//==================================================
unsigned char JudgeBuyTimes( void )
{
	unsigned char Result;
//	union B32_B08 Temp32;
	
//	memcpy(Temp32.B08, Remain_back+12, 4);			//���Ʊ��ڹ������
//	
////	if(Buy_Times.B32 < Temp32.B32) return (0xff);	//��Ƭ�������<����ֵ

//	Temp32.B32 = Buy_Times.B32-Temp32.B32;			//��Ƭ������� - ����ֵ
//	
//	if(Temp32.B32 == 0) return (0);	//����������
//	if(Temp32.B32 == 1) 			//���������1
//	{
		if(Buy_Money_Mac[0].B32 > 99999999)  return(0x55); 	//�����������
		
		Add_Bcd(Remain_back+2, Buy_Money_Bcd, 4);			//ʣ��= ʣ��+��ֵ
		Result = Cmp_Bcd_Inv( Remain_back, Money.Remain, 6);
		if(Result == 2) return(0x55); 						//�����������
		
		//�ж�͸֧���	
		Result = Cmp_Bcd_Inv( Remain_back+2 , &Remain_back[8], 4 );
		if(Result == 2)	//ʣ�� < ͸֧
		{
			Sub_Bcd( &Remain_back[6], &Remain_back[0], 6 );	//͸֧=͸֧-ʣ��
			memset( &Remain_back[0], 0x00, 6 );		   			//ʣ��=0
		}
		else						//ʣ�� >=͸֧  
		{
			Sub_Bcd( &Remain_back[2], &Remain_back[8], 4 );	//ʣ��=ʣ��-͸֧
			memset( &Remain_back[8], 0x00, 4 );		  			//͸֧=0
		}
	
		//�ж϶ڻ����
		if(Money_Para.Regrate.B32)
		{
			if( Cmp_DataLH( Remain_back+2, Money_Para.Regrate.B08, 4) == 1 ) return(0x55);//ʣ��>�ڻ�
		}
		
		memset( &Remain_back[12], 0, 4 );	//�¹������
		
		return (0);	//��������� 1
//	}
//	else return(2);	//���������>1
}

//==================================================
//Ǯ���ļ�����
//��ڣ�Buy_Money_Mac��������+�������+MAC
//==================================================
unsigned char MoneyIn()
{
//	unsigned char Result;
	
//	memcpy(Ic_Comm_RxBuf, Buy_Money_Mac, 8+4);	//ʣ����+MAC
//	Result = IncreseWithMac();
//	if( Result ) return (Result);
//		
	//����RAM��EE��ʣ�࣬͸֧������
	Store_Remainmoney();	//����ʣ�������ݵ�ee
	
//	Moneybag_Process();	//����ESAMǮ��
//	
	//�����¼����̼�¼
	Buy_Record(ee_buymoneyn_inpage);
	
	//�ж�ʣ��������բ���
	Judge_Closerelay_Money();
	card_closerelay_flag   = 0;
	card_closerelay_flag1  = 0;
	Store_Meterstatus();
	
	return 0;
}


void Clear_MeterMoney()
{	
  	memset(Remain_back,0, 24);			//���Ʊ��ڹ������
    Store_Remainmoney();
	  card_closerelay_flag   = 0;
	  card_closerelay_flag1  = 0;
	  Store_Meterstatus();
}
//�������˵��
/*
Type: ͨѶ��ʽ����Ƭ����
Bit7: 0���ز忨, 1Զ�̻򽻻��ն�; 
Bit4: 0Զ��, 1��Ƭ�򽻻��ն�;
Bit1~0: 01����, 02��ֵ, 03����;
*/
unsigned char OpenAddMoney( unsigned char Type )
{
	unsigned char Result, Ch = 0;
	
	if( Type & 0x80 ) Ch = 1;	//1ΪԶ�̻򽻻��նˣ�0Ϊ����
	Type &= B0001_1111;	
	
	if( IsMultiBcd( &Refers_File_Mac[BACKADDR[1]], 6 ) )	//�ͻ����
	{
		if( Ch == 0 ) //���ز忨
			return (IC_FORMAT_ERR);	//�ͻ���Ŵ�
		else  //Զ������
			return 0x01;	//�ͻ���Ų�ƥ��
	}
	
	
	//========== ��Ƭ��ż������ж� ===========
	Get_Usercard_Serial();
	
	if( card_open_flag || comm_open_flag )	//�����ѿ��� or Զ���ѿ���
	{
		Result = Cmp_Data( &Refers_File_Mac[BACKADDR[1]], Esam_Para.User_Id, 6 );
		if( Result != 3 ) //�ѿ����ı��ͻ���Ų����
		{
			if( Ch == 0 ) //���ز忨
				return (IC_HUHAO_ERR);	//�ͻ���Ŵ�
			else  //Զ������
				return 0x10;	//�ͻ���Ų�ƥ��
		}
		
		if( comm_open_flag )
		{
			if(Type == 0x01 ) return (0x01);	//Զ���Ѿ������������ظ�����		
		}	
		if( card_open_flag )
		{
			if(Type == 0x11 ) return (0x0A);	//Զ���Ѿ������������ظ�����		
		}	
		//����Զ�̿����ĵ��ܱ�ֻ���ܿ������Ͳ��������������ܹ��翨����.
		if( (Type == 0x12) && UserCardSerialBit ) 
		{
			if( Ch == 0 ) return (IC_SERIAL_ERR);	//���翨����Ŵ�
			else return 0x01;
		}
	}
	else	//δ����
	{
		switch( Type )
		{
			case 0x02:	return 0x01;	//��ֵ��������
			case 0x12:	
				if( Ch == 0 ) return (IC_NOOPEN_ERR ); //���翨����δ������
				else return 0x01;	//��ֵ��������
			case 0x13:	
				if( Ch == 0 ) return (IC_NOOPEN2_ERR); //��������δ������
				else return 0x01;	//��ֵ��������
			default: break;
		}
	}	
	Clear_Wdt();
  
	KeySTS_OK_flag = 0; //��Կ���±�־����
	
  if(( Type == 0x11 )||( Type == 0x01 ))
	{
		Result=api_Handle_key_sts_code(&KeyH_Token[0]);
	  if( Result != 0x28  ) return (Result);
		Clear_Wdt();
    Result=api_Handle_key_sts_code(&KeyL_Token[0]);
	  if( Result) return (Result);
    
		KeySTS_OK_flag = 1;		//��Կ���±�־
		
	}	
	Clear_Wdt();
	Result=api_Handle_key_sts_code(&Charge_Token[0]);
	KeySTS_OK_flag = 0;			
	if( Result) return (Result);	
	
	if((gs_APDU_analyze_var.sub_class != STS0_SUB_CLASS_PURCHASE_KWH )&&(gs_APDU_analyze_var.sub_class != STS0_SUB_CLASS_PURCHASE_MONEY ) )
	{
		return(STS_TOKEN_REJRCT_2);	//�����ʹ��󣬲�֧�ֵ�TOKEN����//		
	}
//	Hex2Bcd( Buy_Money_Mac[0].B08+0, Buy_Money_Bcd, 4 );	//������HEXת����BCD
  memcpy(  Buy_Money_Bcd,Buy_Money_Mac[0].B08+0, 4 );

	//======== �жϹ����������ʱ��ֵ ==========
	Ready_Remainback();		//׼��Ǯ������	
	
	Result = JudgeBuyTimes();	
	if(Result) return(IC_OVERMAX_ERR);		//���糬�ڻ�
	
	MoneyIn();//Ǯ����ֵ
	
	if(( Type == 0x11 )||( Type == 0x01 )) 	
	{
			api_save_APDU_key_var();	//	������ʽ��Կ��EEPROM
		  KeySTS_OK_flag = 0;		
		
			memcpy( Esam_Para.User_Id, &Refers_File_Mac[BACKADDR[1]], 6 );	//����EE�ͻ����	
      
		  if( Type == 0x11 ) 	Updata_Usercard_Serial(0);
      else 	Updata_Usercard_Serial(1);		
			Store_Esampara();	//����esam�������ݵ�ee
	}
	if( (Type == 0x13) && UserCardSerialBit )  Updata_Usercard_Serial(0);
	
	
	//���ز忨��ʾʣ����(��ǰ�͵�ǰ)
	if( Ch == 0 ) 
	{	
		disp_remain_flag = 1;    
		
	//	Result = UpdataRefer_WithMac(0);//����ESAM��EEPROM����
	//	if( Result ) return( Result);
	
		UpdataRefer_ToEe_WithMac();	//����Eeprom����
  }
	
	return 0x00;
}


//��Ƭ����
unsigned char Card_Esam_Proc( void )
{
	unsigned char Result;
//	unsigned char Buf[64];
	unsigned char i;
	union B16_B08 Crc16,TempCrc16;	
	Clear_Wdt();
	
	Ic_Type_Reg = 0x00;
	Ic_Err_Reg  = 0x00;
	UpdataFlag = 0x00;		
  
//	Delay_Initial(150);	
	
//	for(i = 0; i<10; i++)
//  {
//	  Result = RF_Anticoll();					//����ͻ
//	  if(Result==0) break;
//		Clear_Wdt();
//	}	
//	if(Result) return(IC_NOIN_ERR);
//	
//	for(i = 0; i<3; i++)
//  {
//	  Result = RF_Select_Tag();				//ѡ��
//	  if(Result==0) break;
//	}		
//	if(Result) return(IC_NOIN_ERR);
	
	for(i = 0; i<3; i++)
  {
	  Result = Inter_Authrf08( SECTION7 );
	  if(Result==0) break;
	}		
	if(Result) return(IC_AUTH_ERR);
	
//=========== �жϿ�Ƭ���� ================	
	Result = ReadM1File( SECTION7,BLOCK0 );
	if( Result )
	{
		 return (IC_NOIN_ERR);			//��Ƭ��������
	}	
	
	Crc16.B16=Crc_16Bits( Ic_Comm_RxBuf, 14 );
	TempCrc16.B08[0]=Ic_Comm_RxBuf[15];
	TempCrc16.B08[1]=Ic_Comm_RxBuf[14];
	if( Crc16.B16 != TempCrc16.B16)
	{
		 return (IC_FORMAT_ERR);			//��Ƭ�ļ����Ϸ�
	}	
	
	UserCardFlag = Ic_Comm_RxBuf[0];
	if( UserCardFlag >0x04 )
	{
		 return (IC_FORMAT_ERR);			//��Ƭ�ļ����Ϸ�
	}	
	UpdataFlag = Ic_Comm_RxBuf[1];	
	
	if( UserCardFlag ==0x04 )
	{
		 return (0);			//�������ÿ�   
	}	
	
  Result =	UpdataRefer_WithMac(0);	
  if( Result )
	{
		 return (Result);			//��Ƭ��������
	}
	
	memcpy( &Refers_File_Mac[BACKADDR[1]], &Ic_Comm_RxBuf[2], 6 ); //�ͻ����

	if((UserCardFlag == UserOpenCardType)||(UserCardFlag == UserKeyCardType))
	{
		Result = Inter_Authrf08( SECTION1 );
	  if(Result) return(IC_AUTH_ERR);		
		
		Result = ReadM1File( SECTION1,BLOCK0 );
	  if( Result )return (IC_NOIN_ERR);			//��Ƭ��������	  
		memcpy(&KeyH_Token[0], Ic_Comm_RxBuf, 16);//key token1
		
		Result = ReadM1File( SECTION1,BLOCK1 );
	  if( Result )return (IC_NOIN_ERR);			//��Ƭ��������
	  memcpy(&KeyH_Token[16], Ic_Comm_RxBuf, 4);//key token1
		memcpy(&KeyL_Token[0], &Ic_Comm_RxBuf[4], 12);//key token2
		
		Result = ReadM1File( SECTION1,BLOCK2 );
	  if( Result )return (IC_NOIN_ERR);	
		memcpy(&KeyL_Token[12], &Ic_Comm_RxBuf[0],8);//key token2		
	}
	Clear_Wdt();
	Result = Inter_Authrf08( SECTION0 );
	if(Result) return(IC_AUTH_ERR);	
	
	Result = ReadM1File( SECTION0,BLOCK1 );
	if( Result )return (IC_NOIN_ERR);			//��Ƭ��������	  
	memcpy(&Charge_Token[0], Ic_Comm_RxBuf, 16);//Charge_Token
		
	Result = ReadM1File( SECTION0,BLOCK2 );
	if( Result )return (IC_NOIN_ERR);			//��Ƭ��������
	memcpy(&Charge_Token[16], Ic_Comm_RxBuf, 4);//Charge_Token
	
  //�����Ϳ�Ƭ�Ĵ���
	//���п��жϱ��
	//��ֵ����Ҫ�ж�ϵ�к��Լ��ͻ����
	//������Ҫ�жϿͻ����
//	if((CardType == UserchargeCardType)
//	{
//	if( Cmp_Meter_Id(&Ic_Comm_RxBuf[8]) ) 
//	}
//	else if	(CardType == UserRecuitCardType))
//	else 
	Result = OpenAddMoney( UserCardFlag|0x10 );
	if(Result) return(Result);
	//����Eeprom����
	//UpdataRefer_ToEe_WithMac();	//����KeyToken��Eeprom����
		
	//================ ��д��Ƭ =============
	//UpdataRunFile();	//���������ļ�
			
	return OK_CARD_PROC;
}


//�忨�������
void Card_Proc( void )
{
	unsigned char result;
//	
//	Release_Rf();		return;	
	
	
	if( FLAG_EA == 0 ) return;							//���粻����,����ֱ�ӷ���
	if( Keyswitch_Valid == 0xaa ) return;//�޿���
//	if( Find_Card == 0xaa ) 					//��ֹѰ��
//	{
//		CardCheck = 0;
//		CardCheck1 = 1;							//��֤��ֹѰ�����պ󣬲�������Ѱ��
//		return;	
//	}
	
	if( Beep_Timer ) return;	//�����Ѱ��������Ѱ�������͹���

//========= �ж��Ƿ�Ѱ�� ==============
  if( Keyswitch_Valid == 0x55 )	//�����д���
	{
		if( Find_Card == 0x55 )
		{
			CardCheck = 0;
//			if( rf_card_flag == 0 ) return;	//�Զ�Ѱ��ģʽ�£����жϿ���
		}
		else
		{
			CardCheck1 = 0;
			if( Check_Card() != 0 ) return;	//��������Ѱ��ģʽ
			memset(CardRst8B, 0xff, 8);			//���Ĭ��0xff
		}
	}
  else	//�����޴���
	{
		CardCheck = 0;
		if( rf_card_flag == 0 ) return;		//�޴��㣬1sѰ��1��
	}
		
	rf_card_flag = 0;

//=========== ��Ƭ��λ ================
	result = RF_Init();
	if( ((result == 0) && (CardCheck1 == 0)) || CardCheck )//�п���δ���������򣬿������¿�����
	{
		Lcdled.B16 = lcdled_timer_delay;		//����60s���ⶨʱ��
		LCDLEDON;
		if((Real.U[0] > 0x1450)||(Real.U[1] > 0x1450)||(Real.U[2] > 0x1450))
		{
			if((Display.Number==0x00)||(Display.Number == 0xff)) Display.Number = 1;//��ֹ��ʾ�쳣
			ic_display = 0;
			disp_readic_flag = 1;
			Lcd_Display();	//��ʾ�������С�
			
			Ic_Err_Reg = Card_Esam_Proc();		//��Ƭ����
					
			Release_Rf();							//�����ͷ���Ƶģ�飬���͹���
			
			if( BDMODE && (Ic_Err_Reg == 0) && (UserCardFlag == PresetCardCommand)) //����ʱʹ�ã�������ǰ�ᣬ������Ҫ�ڱ����жϺ����
			{
//				Clear_Meter(3, CardRst8B);	//�������,���������¼
				  Clear_MeterMoney();
			}
			Alarmstatus_Judge();			//�����״̬�ж�
			Enable_Closerelay();			//�忨�պϼ̵���
//			
//			if( BDMODE && (Ic_Err_Reg == 0) && (UserCardFlag == PresetCardCommand))
//			{
//				Clear_Meter(3, CardRst8B);	//�������,���������¼
//				 
//			}
		}
		else Ic_Err_Reg = IC_VLOW_ERR;			//��ѹ����
		
		if( Ic_Err_Reg == 0 )
		{
			InsertCardSta = 0x01;			//�忨�ɹ�
			
			Beep_Timer = 156;			//�ɹ���������ʱ����625ms
		}
		else
		{
			InsertCardSta = 0x02;			//�忨ʧ��
			
			Beep_Timer = 156;			//ʧ�ܷ�������ʱ������125*5

			if( !result ) Card_Err_Record();//�������Ϊ���ܱ���ʶ��Ľ��ʣ�����Ƭ�򷴲忨��ʱ�����ܱ�������зǷ��忨���쳣�忨��¼��
			
			disp_remain_flag = 0;			//��ֹ��ֵ�ɹ�������������ʧ�ܵ����
			
			// Ic_Err_Reg = 1;��ƬͨѶ����
			// Ic_Err_Reg = 2;��Ƭ��֤ʧ��
			// Ic_Err_Reg = 3;�ͻ���Ų�һ��
			// Ic_Err_Reg = 4;�ļ���ʽ����ȷ
			// Ic_Err_Reg = 5;��Ƭϵ�кŲ�һ��
			// Ic_Err_Reg = 6;�ظ�����
			// Ic_Err_Reg = 7;���翨����δ�������
			// Ic_Err_Reg = 8;��������δ�������
			// Ic_Err_Reg = 9;ʣ����ڻ�
			// Ic_Err_Reg = 10;��Ƶ�ѹ����176V(80%Un)
			// Ic_Err_Reg = 11;����ʧ�ܣ���������Կ����//	
			// Ic_Err_Reg = 12;��֧�ֵ�TOKEN��//	
			// Ic_Err_Reg = 13;���ܳɹ�������TOKEN�ڵ����ݳ�����Χ//	
			// Ic_Err_Reg = 14;/TID������Կ��Ч��//	
			// Ic_Err_Reg = 15;TOKEN�Ѿ���ʹ�ù�//	
			// Ic_Err_Reg = 16;TOKEN�ѹ���	
			// Ic_Err_Reg = 17;��ֵ������ȷ���ǳ����ڻ�����
			// Ic_Err_Reg = 18;������
			// Ic_Err_Reg = 19;������
			// Ic_Err_Reg = 20;������	
			if(Ic_Err_Reg < 2)Ic_Err_Reg = 10;
			else if((Ic_Err_Reg < 5)||(Ic_Err_Reg == 19)) Ic_Err_Reg = 1;
			else if(Ic_Err_Reg < 10) Ic_Err_Reg = 2;
			else if(Ic_Err_Reg == 10) Ic_Err_Reg = 6;
			else if(Ic_Err_Reg < 13) Ic_Err_Reg = 3;
			else if(Ic_Err_Reg < 14) Ic_Err_Reg = 5;
			else if(Ic_Err_Reg < 15) Ic_Err_Reg = 4;
			else if(Ic_Err_Reg < 16) Ic_Err_Reg = 7;
			else if(Ic_Err_Reg < 17) Ic_Err_Reg = 8;			
			else if(Ic_Err_Reg == 21) Ic_Err_Reg =9 ;
			else if(Ic_Err_Reg == 31) Ic_Err_Reg = 11;
			else if(Ic_Err_Reg == 32) Ic_Err_Reg =12 ;
			else if(Ic_Err_Reg == 34) Ic_Err_Reg = 13;
			else if(Ic_Err_Reg == 36) Ic_Err_Reg = 14;
			else if(Ic_Err_Reg == 37) Ic_Err_Reg = 15;
			else if(Ic_Err_Reg == 38) Ic_Err_Reg =16 ;
			else Ic_Err_Reg = 18;
		}
		
		Clear_Wdt();
		
	  if(Keyswitch_Valid == 0x55)
	  {
	  	if(Find_Card == 0x55) CardCheck1 = 1;//�����д��㣬����Ѱ��ģʽ����Ƭ�Ѵ���
	  }
		else CardCheck1 = 1;	//��Ƭ�Ѵ���
		ic_display = 1;
		disp_readic_flag = 0;
		Display.Timer = Display_Para.Loop_Time;
		Display.Number = 0;
		Display.Status &= B1111_1110;
		if( (CardType == UserCardCommand) && (Ic_Err_Reg == 0) && BDMODE )
		{
			if(disp_remain_flag) Display.Timer += 2;//����ɹ����û���,��ʾ��ǰʣ����2s
			else	disp_remain_flag = 1;		//��д�ɹ����û���,��ʾ��ǰʣ����
		}
		Lcd_Display();	//ˢ����ʾ
	}
	else if(( result )&&( RFTESTCNT ==0 ))
	{
		
		CardCheck1 = 0;
	}
	
	Release_Rf();			//�ͷ���Ƶģ��
}


//������Զ�̷ѿصĳ���
//MACУ����������
//�ۼ�MACʧ��100�ι���
void Mac_Err_Proc( void )
{
	if( terminal_auth_flag ) return;
	
	if(Mac_Err_Times < 200) Mac_Err_Times++;
	if(Mac_Err_Times >= 200)
	{
		Mac_Err_Times = 0x00;	//����
		hangup_comm_flag = 1;	//Ҫ����ܱ�Զ�����ò������ܹ���
	}
}

unsigned char Update_ESAM_Bin_Mac( unsigned char P1, unsigned char P2, unsigned char *Buf, unsigned char Len )
{
	unsigned char result;
	
	Exchange( Ic_Comm_RxBuf, Buf, Len );
	Exchange( Ic_Comm_RxBuf+Len, Buf+Len, 4 );

	result = WriteBinWithMac( ESAM, P1, P2, Len+4 );
	if( result )
	{
		Mac_Err_Proc();	//mac���������1
	}
	
	return (result);
}


//ͬ������esam������Ϣ�ļ�
void UpdataRunFile(void)
{
	unsigned char Buf[60];
	
	ReadBinFile( ESAM, 0x86, 0, 50 );	//��ȫ��06�ļ�
	memcpy( Buf, Ic_Comm_RxBuf, 50 );	//06�ļ����ݱ���
	
	Ic_Comm_RxBuf[0] = 0x68;
	Ic_Comm_RxBuf[1] = 0x11;							//�û�����д
	Ic_Comm_RxBuf[2] = 0x00;
	Ic_Comm_RxBuf[3] = 0x2c;
	
	Bcd2Hex( Money.Remain, Ic_Comm_RxBuf+23, 4);		//ʣ����
	memcpy( Ic_Comm_RxBuf+27, Money.Buy_Times.B08, 4 );	//�������
	memcpy( Ic_Comm_RxBuf+31, Money.Tick, 4 );					//͸֧(BCD)	
	
	Save_Keep_Timer(0);
	Hex2Bcd( Eeprom.Buffer+1, Ic_Comm_RxBuf+39, 3 ); 	//�Ƿ����������� 3 BCD
		
	memcpy( Ic_Comm_RxBuf+5, Esam_Para.Current_Ratio,18 );//���1+���2+���+�ͻ����
	Ic_Comm_RxBuf[47] = Display_Para.MeterCtrl;	//���ģʽ��
	
	if( (Cmp_Data( Ic_Comm_RxBuf, Buf, 48) != 3) || (CardType == UserCardCommand))//�ж�07�ļ������Ƿ����仯,����仯�˲ŷ�д07�ļ�
	{
		Exchange( Ic_Comm_RxBuf+42, Sclock.Clockbackup+1, 5 );//��ʱ������
		//����CS
		Ic_Comm_RxBuf[48] = Cal_ChkSum(Ic_Comm_RxBuf+1, 47);
		Ic_Comm_RxBuf[49] = 0x16;
		
		UpdateBinFile(ESAM, 0x86, 0, 50 );
	}
}

unsigned char GetPswBit( unsigned char *InBuf )
{
	unsigned char i, n, Temp08;

	Temp08 = Feik.Num[7];
	if( Temp08 > MAX_PSW_NUM ) Temp08 = MAX_PSW_NUM;	//��Կ�������Ƿ�Ĭ��

	for( i=0; i<Temp08; i++ )	//�����������õ���Կ������.
	{
		if( (i%8) == 0 ) n = 1;
		else n <<= 1;
		
		if( InBuf[0]&1 )
		{
			if( (InBuf[i/8]&n) == 0x00 ) return 0xFF;
		}
		else
		{
			if( InBuf[i/8]&n ) return 0xFF;
		}
	}
	
	return (InBuf[0]&1);
}

//����Կ״̬��
void GetKey( unsigned char *KeyStatus )
{
	I2C_Read_Eeprom44( ee_keystatus_page, KeyStatus, ee_keystatus_lenth );
}

//д��Կ״̬��
void SetKey( unsigned char *KeyStatus )
{
	I2C_Write_Eeprom44( ee_keystatus_page, KeyStatus, ee_keystatus_lenth );
	GetKeyStatus();
}

//����ǰ��Կ״̬
void GetKeyStatus()
{
	unsigned char Buf[32];
	
	GetKey( Buf ); //����Կ״̬
	Key_Status = GetPswBit( Buf );
}


unsigned char GetSetBitNum( unsigned char *InBuf )
{
	unsigned char i, k, n;

	for( i=0, k=0; i<MAX_PSW_NUM; i++ )	//ͳ�������ص���Կ������
	{
		if( (i%8) == 0 ) n = 1;
		else n <<= 1;
		if( InBuf[i/8] & n ) k++;
	}
	
	return (k);
}


unsigned char UpdateEepromKey( unsigned char Ch )
{
	unsigned char i, k, result;
	unsigned char Buf[64], ChangeBuf[32];
	unsigned char FramePswNum, MeterPswNum, SettedPswNum;

	if( Uart[Ch].Buf[9] <= 12 ) return 1;
	
	//FramePswNum:  ����֡�а�������Կ����.
	//MeterPswNum:  �����������õ���Կ������.
	//SettedPswNum: �����ص���Կ������(�������ظ����µ���Կ)
	FramePswNum = (Uart[Ch].Buf[9]-8-4)/36;
	if( (FramePswNum==0) || (FramePswNum>4) || ((Uart[Ch].Buf[9]-8-4)%36) ) return 1;

	MeterPswNum = Feik.Num[7];
	if( MeterPswNum > MAX_PSW_NUM ) MeterPswNum = MAX_PSW_NUM;	//��Կ�������Ƿ�Ĭ��

	//ee_keystatus_page: 
	//��Կ״̬��(4)  : ���Ϲ����涨�ġ���Կ״̬�֡���
	//��Կ״̬(1)    : (Ԥ��)
	//����״̬(1)    : ��Կ���µĹ���״̬ (00����дE2PROM, 55����дESAM, AA�������).
	//���µ�E2PROM(4): ���չ����涨�ġ���Կ״̬�֡���ʽ��ÿ����һ����E2PROM ��Ӧbit��1.
	//���µ�ESAM(4)  : ���չ����涨�ġ���Կ״̬�֡���ʽ��ÿ����һ����ESAM ��Ӧbit��1.(Ԥ��)
	//����Կ״̬��(4): ���չ����涨�ġ���Կ״̬�֡���ʽ��0Ϊ��Կ��1Ϊ˽Կ��
	//ee_keystatus1_page:
	//�����ߴ���(4)  : ���¼�¼�еĲ����ߴ����ǵ���յ���һ����Կ���������еĲ����ߴ���.
	//��Կ���µ�������(1) HH
	//����ǰ��Կ״̬��(4):
	GetKey( ChangeBuf );
	if( ChangeBuf[5] == 0x55 )return 0; //����״̬: 00����������E2PROM; 55����������ESAM; AA�Ѹ��½���.
	if( ChangeBuf[5] != 0x00 ) //����������Կ
	{
//		ChangeBuf[4] &= 0x3F;	//���Bit7��Bit6��
		ChangeBuf[5] = 0x00;	//ֻ�и��½�����������һ�ֵĸ��¡�
		memset( ChangeBuf+6, 0x00, 12 ); //��ո��µı�־λ��
		
		memcpy( Eeprom.Buffer, Uart[Ch].Buf+14, 4 );	//�����ߴ���
		Eeprom.Buffer[4] = MeterPswNum;	//��Կ���µ�������(1) HH
		memcpy( Eeprom.Buffer+5, ChangeBuf, 4 );	//����ǰ����Կ״̬��
		I2C_Write_Eeprom( ee_keystatus1_page, ee_keystatus1_inpage, Eeprom.Buffer, ee_keystatus1_lenth ); 
	}

	memcpy( Buf, Uart[Ch].Buf+18, 36 );
//	for( i=1; i<FramePswNum; i++ ) Xor_Data( Buf, Uart[Ch].Buf+18+i*36, 36 );

	//��Ч����Կ����
	for( i=0, k=0; i<FramePswNum; i++, k+=36 )
	{
		if(i) Xor_Data( Buf, Uart[Ch].Buf+18+k, 36 );
		
		//��Կ������+Ԥ��+��Կ���+��Կ״̬
		if((Uart[Ch].Buf[18+32+k] > 0x01) //��Կ״̬��־λ��00-����״̬��01-��ʽ״̬��
		 ||(Uart[Ch].Buf[18+32+k] != Uart[Ch].Buf[18+32+0]) ) return 1; //����Ӧ��: ���ĳ�����Կ״̬��־λ��һ��
		if( Uart[Ch].Buf[18+33+k] >= MeterPswNum ) return 1;	//��Կ��ų���.
		if( Uart[Ch].Buf[18+35+k] != MeterPswNum ) return 1;	//����Ӧ��: �����е���Կ�����������������Կ��������һ��.

		ChangeBuf[6+Uart[Ch].Buf[18+33+k]/8] |= (1<<(Uart[Ch].Buf[18+33+k]%8));	//��¼������Կ���.
		
		if( Uart[Ch].Buf[18+32+k] )
			ChangeBuf[14+Uart[Ch].Buf[18+33+k]/8] |= (1<<(Uart[Ch].Buf[18+33+k]%8));
		else
			ChangeBuf[14+Uart[Ch].Buf[18+33+k]/8] &= ~(1<<(Uart[Ch].Buf[18+33+k]%8));
	}

//	memset( Buf+36, 0x00, 8 );
//	if( (Uart[Ch].Buf[18+32+0]&0x7F) == 0x00 )	Buf[36] = 0x01; //��Կ�ָ�ʱ�̶�Ϊ01
//	else	Exchange( Buf+36, Esam_Para.Meter_Id, 6 );	//��Կ��װʱȡ���
	Exchange( Buf+36, Esam_Para.Meter_Id, 6 );	//��Կ��װʱȡ���
	Buf[42] = 0x00;
	Buf[43] = 0x00;
	memcpy( Buf+44, Uart[Ch].Buf+10+Uart[Ch].Buf[9]-4, 4 ); //MAC
	
	result = Update_ESAM_Bin_Mac( 0x95, 0x00, Buf, 44 );	//
	if( result != 0x00 ) return 4;	//�����������MAC У����ǲ��ô�����Ϣ��ERR ��bit2�������/δ��Ȩ����λ���������쳣Ӧ��֡��Ӧ��
	
	SettedPswNum = GetSetBitNum( ChangeBuf+6 ); //ͳ�������ص���Կ������
	if( SettedPswNum == MeterPswNum ) //���µ���Կ�����ﵽ������
	{
		result = GetPswBit( ChangeBuf+14 );
		if( result != 0xFF ) 	//�ж���Կ״̬�Ƿ�һ��
		{
			ChangeBuf[5] = 0x55; //������������ESAM��־
		}
	}
	else if( SettedPswNum > MeterPswNum ) return 1;	//����Ӧ�����ص���Ч��Կ����>������Կ������

	for( i=0, k=0; i<FramePswNum; i++, k+=36 )
	{
		Inverse( Uart[Ch].Buf+18+k, 32 );	//ֻ������Կ
		I2C_Write_Eeprom( ee_keydata_1_page+Uart[Ch].Buf[18+33+k]/2, 32*(Uart[Ch].Buf[18+33+k]%2), Uart[Ch].Buf+18+k, 32 );
	}

	SetKey( ChangeBuf ); //���ȼ�¼����Կд��E2PROM�����.

	return 0;
}


//����Ƿ�����Կ����µ�ESAM��������¡�
unsigned char UpdateEsamKey( void )
//��������: ��ǰ�����Կ״̬
{
	unsigned char i, k, n, result, key, Bkmain;
	unsigned char TmpBuf[32];
	
	GetKey( TmpBuf );
	if( TmpBuf[5] == 0x55 ) //����״̬: 00����������E2PROM; 55����������ESAM; AA�Ѹ��½���.
	{
		key = GetPswBit( TmpBuf+14 );
		for( i=0; i<MAX_PSW_NUM; i++ )
		{
			
			if( (i%8) == 0 ) k = 1;
			else k <<= 1;
			
			if( (TmpBuf[6+i/8]&k)  )
			{
				//��������ESAMָ��
				for( n=0; n<3; n++ )
				{
					if( i == 0x00 )
						RxTxCommhead(0x84, 0xd4, 1, 0x00, 0x20); //������Կ����
					else
						RxTxCommhead(0x84, 0xd4, 1, 0xff, 0x20); //������Կ����

					result = I2C_Read_Eeprom( ee_keydata_1_page+i/2, 32*(i%2), Ic_Comm_TxBuf+5, 0x20 );
					if( result ) continue;
					
					result = CardTxRxFrame( ESAM, 0x20, 0x00 );
					if( result == 0x00 ) break;
				}
				
				if( ( i == 0 )||( result == 0 ) )
				{
					if( i == 0 ) Bkmain = TmpBuf[0]; 
					
					if( key == 0 ) TmpBuf[i/8] &= (~k);
					else TmpBuf[i/8] |= k;
					
					if( ( i == 0 )&&( result == 0 ) ) Bkmain = TmpBuf[0]; 	//�ɹ�����������Կ
				}
				else
				{
					if( FLAG_EA == 0 ) return 0x00; 	//���¹����г���ͣ��,�ϵ����Կ���Ϊ0����Կ��ʼ��ESAM���½�����Կ����
					break;				//����ʧ��,���ٶԱ�����Կ֮�����Կ���и���
				}
			}
		}
		
//		if( i == MAX_PSW_NUM )
//		{			
			key = GetPswBit( TmpBuf);
			
			if( key == 0xFF )	
			{
				if( (Bkmain & 0x01) == 0 ) TmpBuf[0] &= 0xFE;		//����ʧ��,�ָ�����״̬
				else TmpBuf[0] |= 1;
			}
			
			TmpBuf[5] = 0xAA;	//ȫ���������
//		}

		for( n=0; n<3; n++ )
		{
			Exchange( Ic_Comm_RxBuf, TmpBuf, 4 );//����4�ֽ���Կ״̬
			result = UpdateBinFile( ESAM, 0x85, 0x00, 4 );	//дesam��05�ļ�
			if( result == 0x00 ) break;
			if( FLAG_EA == 0 ) return 0x00; 
		}
		
		SetKey( TmpBuf ); //���ȼ�¼����Կд��E2PROM�����.
		
		//--- ��Կ���¼�¼ ---
		I2C_Read_Eeprom( ee_keystatus1_page, ee_keystatus1_inpage, Eeprom.Buffer+6, ee_keystatus1_lenth ); 
		Event_Record( ee_keysetn_inpage );
		//--- ��Կ���¼�¼ end---
	}

	return 0x00;
}


void FailKey( void )
{
	unsigned char Buf[64];

	GetKey( Buf );
	if( Buf[5] != 0xAA )
	{
		memset( Buf+4, 0x00, ee_keystatus_lenth-4 ); //��ո��µı�־λ��
		Buf[5] = 0xAA;
		SetKey( Buf );
	}
}
//void Clear_MeterMoney(void)
//{
//	
//	memset( Remain_back, 0x00, 16 );
//	Store_Remainmoney();	//����ʣ�������ݵ�ee
//	
////	Moneybag_Process();	//����ESAMǮ��
////	
//	//�����¼����̼�¼
//	Buy_Record(ee_buymoneyn_inpage);
//	
//	//�ж�ʣ��������բ���
//	Judge_Closerelay_Money();
//	card_closerelay_flag   = 0;
//	card_closerelay_flag1  = 0;
//	Store_Meterstatus();
//	
//	return 0;	
//	;	
//}
//�������
void Clear_Meter( unsigned char Ch, unsigned char *Opt )
{
	unsigned char Buf[72];
	Clear_Wdt();
		
	ClrRecTimes( 0, 0xaa );//�嶳����¼���Чָ��(�������¼)
	
	//�ϱ����,������غ͸��ɼ�¼ָ������
	memset( &System_St_Bk, 0x00, ee_reportdata_lenth+18 );
	
	//�������������
	Clr_Zero_Data();
	
	//�������ʱҪ���Զ����բ��Զ�̱���״̬�����������״̬
	if( Relay_Mode == 0x55 )//���ü̵���
	{
		if( (!BDMODE) || ((remainder_maxtick_flag == 0) || relay_commu_close_flag) )//����Ԥ�ÿ����ں�բ״̬���ü̵����ɹ���־
		{
			relay_cleardl_flag = 1;	//�����ñ������־
		}
	}
	relay_commu_alarm_flag = 0;
	
	Clr_Commu_Relaydata();
//	Store_Meterstatus();
	ClrSelfClose();	//���Ա���״̬(��Ϊ��բ״̬�������)
	
	//������õ���/�����õ���/��1~4���õ���
	Process_Usedl(3);
	Process_Usedl(4);
	Process_Usedl(5);

	//��¼�����¼�
	//�����¼���¼������ʱ��(6)+�����ߴ���(4)+�������ǰ�����й�(4)+�������ǰ����(4)	
	//�洢��������ʱ���򣬵��ֽ���ǰ�����ֽ��ں�
	if(Ch != 3)
	{			
		memcpy( Eeprom.Buffer+6, Opt, 4 );	//�����ߴ���(4),Զ������
	}
	else
	{
		Exchange( Eeprom.Buffer+6, Opt+4, 4 );	//�����ߴ��루����ź�4�ֽڣ�
	}
	
	DltoEepromBuf( 10 );
	Event_Record( ee_clearn_inpage );

	//�� XRAM��E2����
	Veri_Etmr1_Pcount(1);		//������β��
	
	memset( Fwdl.Fwdl_T.B08, 0x00, ee_fwdl_lenth+2 );		//���������
	WriteE2WithBackup( ee_fwdl_page, ee_fwdl_inpage, Fwdl.Fwdl_T.B08, ee_fwdl_lenth );

	memset( Bkdl.Bkdl_T.B08, 0x00, ee_bkdl_lenth+2 );		//�巴�����
	WriteE2WithBackup( ee_bkdl_page, ee_bkdl_inpage, Bkdl.Bkdl_T.B08, ee_bkdl_lenth );
	
	Clear_Wdt();
	if( BDMODE ) //����ģʽ
	{
		run_price_flag = 1;		//�������������жϽ���ֵ
	
		//��ʼ�������
		memset( Eeprom.Buffer, 0xff, ee_usercard_serial_lenth );
		I2C_Write_Eeprom44( ee_usercard_serial_page, Eeprom.Buffer, ee_usercard_serial_lenth );//crc�Ƿ���ȷ���жϵ����
		
		//���汾�ι����¼
		Buy_Record(ee_buymoneyn_inpage);
		
	
	memset( Buf, 0xff, 72 );			
	I2C_Write_Eeprom( ee_TID_page, ee_TID_inpage, Buf, ee_TID_lenth );
	
	memset( Buf, 0x00, 72 );
  I2C_Write_Eeprom( ee_TIDCNT_page, ee_TIDCNT_inpage, Buf, ee_TIDCNT_lenth );	
	I2C_Write_Eeprom( ee_TOKEN_page, ee_TOKEN_inpage, Buf, ee_TOKEN_lenth );
	I2C_Write_Eeprom( ee_Pre7DayMon_page, ee_TOKEN_inpage, Buf, ee_Pre7DayMon_lenth );
	I2C_Write_Eeprom( ee_Pre12monthMon_page, ee_Pre12monthMon_inpage, Buf, ee_Pre12monthMon_lenth );
	I2C_Write_Eeprom( ee_DayUserINFO_page, ee_UserINFO_inpage, Buf, 16 );
	
	}
	
	Mt_Alarmstatus_Judge();
	if( relay_status_flag ) ClrRecTimes( ee_openrelayn_inpage, 0x01 );//����բ��¼
	else ClrRecTimes( ee_closerelayn_inpage, 0x01 );//���բ��¼
	
	Wr_Powerdown_Data(0);//д����������ݵ�����ee
	Wr_Pretime_Dayfreeze();	//д��һ���ն���ʱ��
	
	
}
