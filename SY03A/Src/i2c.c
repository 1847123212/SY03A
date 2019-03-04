#include "ex_var.h"
#include "ex_func.h"
//#include <intrins.h>
#include <string.h>

#define		CRC_PAGE			0x2d		//CRCУ�����ҳ
#define 	Devi_Addr 			0xa0		//device addr
#define 	PAGE_SIZE 			64		//page size
#define 	SSP_TBIT 			8		//36us


void I2c_Wait_5ms( void ) 
{
	unsigned short Temp16;
	
	for( Temp16=0; Temp16<10000; Temp16++ )
	; 	
}
void I2c_Wait_2ms( void ) 
{
	unsigned int Temp16;
 
	for( Temp16=0; Temp16<2000; Temp16++ )
	; 	
}
void I2c_Wait_1ms( void ) 
{
	unsigned int Temp16;
 
	for( Temp16=0; Temp16<1000; Temp16++ )
	; 	
}

//350us
void sub_delay(unsigned char PB_delay)
{
	unsigned char i,j;
	
	for(i=PB_delay;i!=0;i--)
	{
	    for(j=0;j<255;j++)
	    {	
		  __NOP();__NOP();
			
	    } 
	}
	
}



void us_delay(unsigned char  d)
{
   unsigned char j;
   for(j=0;j<10;j++)
	{	
		  __NOP();			
	} 	
}
	
void Null01(void)
{
;	
}
//////////////////////////////////
void Eepr_Start(void)
{
	Set_OutPut_P_EeprScl;
	Set_OutPut_P_EeprSda;
	us_delay(1);
	
	if(IICPowerON)
	{
	  P_EeprSda_High;	
	  us_delay(1);
	
	  P_EeprScl_High;
	  us_delay(1);
	
	  P_EeprSda_Low;
	  us_delay(1);
	
  	P_EeprScl_Low;
    us_delay(1);
	}
	else
	{
		P_EeprSda_PowLowHigh;	
	  us_delay(1);
	
	  P_EeprScl_PowLowHigh;
	  us_delay(1);
	
	  P_EeprSda_PowLowLow;
	  us_delay(1);
	
	  P_EeprScl_PowLowLow;
    us_delay(1);
	}
}


void Eepr_Stop(void)
{
	Set_OutPut_P_EeprScl;
	us_delay(1);
	
	Set_OutPut_P_EeprSda;
	us_delay(1);
	
	if(IICPowerON)
	{
	P_EeprScl_Low;
  us_delay(1);
	
	P_EeprSda_Low;
	us_delay(1); 
	
	P_EeprScl_High;
	us_delay(1);
	
	P_EeprSda_High;
	us_delay(1);
	}
	else
	{
	P_EeprScl_PowLowLow;
  us_delay(1);
	
	P_EeprSda_PowLowLow;
	us_delay(1); 
	
	P_EeprScl_PowLowHigh;
	us_delay(1);
	
	P_EeprSda_PowLowHigh;
	us_delay(1);
	}
		
}


void Eepr_AckWr(void)
{
	Set_InPut_P_EeprSda;
	us_delay(1);
	if(IICPowerON)
	{
	P_EeprScl_High;	
	us_delay(1);
	
	P_EeprScl_Low;
	us_delay(1);
	}
	else
	{
	P_EeprScl_PowLowHigh;	
	us_delay(1);
	
	P_EeprScl_PowLowLow;
	us_delay(1);
	}	
	
	Set_OutPut_P_EeprSda;
	us_delay(1);
}

void Eepr_AckRd(void)
{
	Set_OutPut_P_EeprSda;
	us_delay(1);
	
	if(IICPowerON)
	{
	P_EeprSda_Low;
	us_delay(1);
	
	P_EeprScl_High;	
	us_delay(1);
	
	P_EeprScl_Low;
	
	us_delay(1);
	}	
	else
	{
	P_EeprSda_PowLowLow;
	us_delay(1);
	
	P_EeprScl_PowLowHigh;	
	us_delay(1);
	
	P_EeprScl_PowLowLow;
	
	us_delay(1);
	}
		
	
	Set_InPut_P_EeprSda;
	us_delay(1);
}

void Eepr_NoAck(void)
{
	Set_OutPut_P_EeprSda;
	us_delay(1);
	if(IICPowerON)
	{
	P_EeprSda_High;
	us_delay(1);
	
	P_EeprScl_High;	
	us_delay(1);
	
	P_EeprScl_Low;
	us_delay(1);
	}
	else
	{
	
	P_EeprSda_PowLowHigh;
	us_delay(1);
	
	P_EeprScl_PowLowHigh;	
	us_delay(1);
	
	P_EeprScl_PowLowLow;
	us_delay(1);
	}
	
}
	

	
unsigned char I2c_Send_Bit( unsigned char x_byte ) 
{
	switch(x_byte)
	{
	     case B1000_0001:
	     case B1000_0010:   
	         Eepr_Start();
	         break;
		 
	     case B1000_0100:
	         Eepr_Stop();
	         break;
		 
	     case B1001_0000:
	         Eepr_AckRd();
	         break;
	     
	    default:
	         break;

	}
        return 0;
}

unsigned char I2c_Send_Byte( unsigned char x_byte ) 
{
  unsigned char i;
	B08_Bit LUB_data;
	Set_OutPut_P_EeprSda;
	LUB_data.B08  = x_byte;
	us_delay(2);
	for(i=0;i!=8;i++)
	{
		if(IICPowerON)
	  {
    if(LUB_data.Bit.bit7) {P_EeprSda_High;}
		else {P_EeprSda_Low;}		
		us_delay(1);
		P_EeprScl_High;		
		LUB_data.B08 <<= 1;
		us_delay(1); 
		P_EeprScl_Low;
	  }
		else
		{
    if(LUB_data.Bit.bit7) {P_EeprSda_PowLowHigh;}
		else {P_EeprSda_PowLowLow;}		
		us_delay(1);
		P_EeprScl_PowLowHigh;		
		LUB_data.B08 <<= 1;
		us_delay(1); 
		P_EeprScl_PowLowLow;
	  }
			
	}
	if(IICPowerON)
	{
	  P_EeprSda_Low;
	}
	else
	{
	  P_EeprSda_PowLowLow;
	}	
	
	Eepr_AckWr();       
  return 0;
}

void I2C_CycleErrChk(void)
{
	unsigned char i;
	Set_InPut_P_EeprSda;
	Set_OutPut_P_EeprScl;
	if(IICPowerON)
	{
	  P_EeprScl_High;
	}
	else
	{
		P_EeprScl_PowLowHigh;
	}
	
	if(!P_EeprSda)
	{
		//there is and error on I2C bus
		for(i=0;i<10;i++)
		{
			if(IICPowerON)
	    {
			P_EeprScl_Low;
			Null01();Null01();
			P_EeprScl_High;
			if(P_EeprSda)break;
			}
			else
			{
			P_EeprScl_PowLowLow;
			Null01();Null01();
			P_EeprScl_PowLowHigh;
			if(P_EeprSda)break;
			}
			
		}
	}
}

unsigned char I2c_Receive_Byte( unsigned char *x_byte ) 
{
	B08_Bit LUB_data;
	unsigned char i;
	Set_InPut_P_EeprSda;
	LUB_data.B08=0;us_delay(1);
	
	for(i=0;i!=8;i++)
	{
		if(IICPowerON)	  
		{ P_EeprScl_High;}
		else	
		{	P_EeprScl_PowLowHigh;}
		
		us_delay(1);
		
		if(P_EeprSda)	LUB_data.B08+=1;	
		else LUB_data.B08+=0;
		
		if(IICPowerON)	  
		{		
		  P_EeprScl_Low;
		}	
	  else		
    {		
		  P_EeprScl_PowLowLow;
		}		
		
		if(i==7)
		{
			Set_OutPut_P_EeprSda;
			if(IICPowerON)	  
	  	{	
			  P_EeprSda_Low;
			}	
			else
			{	
			  P_EeprSda_PowLowLow;
			}			
			break;
		}	
		LUB_data.B08<<=1;
		us_delay(1);	
	}		
	*x_byte = LUB_data.B08;
  return 0;
}



unsigned char Sendaddr( uint16 Page, unsigned char offset, unsigned char Opt ) 
{
	unsigned char result;
	union B16_B08 Addr;
	
	if( Page < 0x300 )
	{
		Addr.B16 = Page*PAGE_SIZE+offset;
	}
	else
	{
		Addr.B16 = 0x300*PAGE_SIZE+(Page-0x300)*128+offset;
	}

	//-------------- reset i2c -------------
	for(result=0x00; result<0x03; result++ );
	
	//-------------- start bit -------------
	result = I2c_Send_Bit( B1000_0001 );//������ʼλ
	if( result != 0 ) return 1; //failure.

	//-------------- device addr -------------
	result = I2c_Send_Byte( Devi_Addr ); //����������ַ
	if( result != 0 ) return 2; //failure.

	//--------------- data addr --------------
	result = I2c_Send_Byte( Addr.B08[1] ); //�������ݵ�ַ
	if( result != 0 ) return 3; //failure.

	result = I2c_Send_Byte( Addr.B08[0] ); //�������ݵ�ַ
	if( result != 0 ) return 3; //failure.

	if( Opt == 1 ) //������
	{
		result = I2c_Send_Bit( B1000_0010 ); //��������ʼλ
		if( result != 0 ) return 5; //failure.
		
		result = I2c_Send_Byte( Devi_Addr|1 ); //����������ַ,��ȡ
		if( result != 0 ) return 5; //failure.
	}
		
	return 0; //ok
}
  

unsigned char Wait_for_end( void )
{
	I2c_Wait_5ms( );
	return 0; 
}


uint08  ReadE2WithBackup( uint16 Page, uint08 inpage, uint08 *Buf, uint08 Len )
{
	if( I2C_Read_Eeprom( Page, inpage, Buf, Len ) )
	{
		if( I2C_Read_Eeprom( Page+1, inpage, Buf, Len ) ) return 1;
	}
	
	return 0;
}

uint08 WriteE2WithBackup( uint16 Page, uint08 inpage, uint08 *Buf, uint08 Len )
{
	if( I2C_Write_Eeprom( Page, inpage, Buf, Len ) ) 
	{
		if( I2C_Write_Eeprom( Page, inpage, Buf, Len ) ) return 1;	//ʧ����дһ��
	}
	
	I2C_Write_Eeprom( Page+1, inpage, Buf, Len );
	 
	return 0;
}

uint08  I2C_Read_Eeprom44( uint16 Page, uint08 *Buf, uint08 Len )
{
	if( I2C_Read_Eeprom( Page, 44, Buf, Len ) )
	{
		 if( I2C_Read_Eeprom( Page, 44, Buf, Len ) ) return 1;	//ʧ���ٶ�һ��
	}	
	return 0;
}

uint08  I2C_Write_Eeprom44( uint16 Page, uint08 *Buf, uint08 Len )
{	
	if( I2C_Write_Eeprom( Page, 44, Buf, Len ) )
	{
		if( I2C_Write_Eeprom( Page, 44, Buf, Len ) ) return 1;	//ʧ����дһ��
	}
	
	return 0;
}

unsigned char I2C_Write_Bottom(uint16 Page, unsigned char offset, unsigned char *Buf, unsigned char h)
{
	unsigned char k, n, status;
	
	for( k=0; k<3; k++ )//ÿ�����д3�� 
	{                                   	
		status = 0;
		if( Sendaddr( Page, offset, 0 ) ) status = 1; //д��ʧ��
		else 
		{
			for( n=0; n<h; n++ )
			{
				//����һ���ֽ�
				if( I2c_Send_Byte( Buf[n] ) ) {status = 1; break;} //д��ʧ��
			}
		}
		//����ֹͣλ
		if( I2c_Send_Bit( B1000_0100 ) ) status = 1;
		
		if( status == 0 ) {Wait_for_end(); break;}//д��ȷ
	}
	
	return( status );
}

unsigned char I2C_Read_Bottom(uint16 Page, unsigned char offset, unsigned char *Buf, unsigned char h)
{
	unsigned char k, n, status;
	
	for( k=0; k<3; k++ )//ÿ������3��
	{
		status = 0;
		if( Sendaddr( Page, offset, 1 ) ) status = 1; //д��ʧ��
		else 
		{
			for( n=0; n<h; n++ )
			{
				//����һ���ֽ�
				if( I2c_Receive_Byte( Buf+n ) ) {status = 1; break;} 
        		
				if( n < (h-1) )					
				{
					
					if( I2c_Send_Bit( B1001_0000) )  {status = 1; break;}
				}
			}
		}
		//����ֹͣλ
		if( I2c_Send_Bit( B1000_0100 ) ) status = 1;	//ʧ��
		
		if( status == 0 ) break;//����ȷ
	}
	
	return( status );
}

unsigned char I2C_Write_Eeprom( uint16 Page, unsigned char offset, unsigned char *IIC_Buf, unsigned char Len )
{
	unsigned char h, i;
	union B16_B08 Crc16;
	unsigned char BufA[80],BufB[80];
	
	if( Page >= 0x380 ) return 1; //��ַ��� 
	if( Len > 72 ) return 1; //�ռ����
	
	h = Len;
	memcpy( BufA, IIC_Buf, Len );
	if( (Page < CRC_PAGE) || (Page >= 0x350) ) 
	{
		Crc16.B16=Crc_16Bits( BufA, Len );		//����crc
		BufA[Len+1]=Crc16.B08[0];
		BufA[Len]=Crc16.B08[1];
		h = h + 2;					//���ȼ�2�ֽ�crc 
	}
	if( ((offset+h)>PAGE_SIZE) && (Page < 0x300) ) return 1; //�ռ����
	
	for( i=0; i<2; i++ )//���д2��
  {                              	
    if( I2C_Write_Bottom( Page, offset, BufA, h ) )continue;//�����������д
        	
		if( I2C_Read_Bottom( Page, offset, BufB, h ) )continue;//����������ζ�
		
		if( Cmp_Data( BufA, BufB, h )==3 ) return 0;	//��дһ�³ɹ�����
	}
	return 2; //fail
}


unsigned char I2C_Read_Eeprom( uint16 Page, unsigned char offset, unsigned char *IIC_Buf, unsigned char Len ) 
{ 
	unsigned char h, i;
	unsigned char BufA[80];
	
	if( Len > 72 ) return 1; //�ռ����
	
	h = Len;
	if( (Page < CRC_PAGE) || (Page >= 0x350) ) h = h + 2;	//���ȼ�2�ֽ�crc 
	
	if( ((offset+h)>PAGE_SIZE) && (Page < 0x300) ) return 1; //�ռ����

	for( i=0; i<2; i++ )//����2��
	{
		if( I2C_Read_Bottom( Page, offset, IIC_Buf, h ) )continue;//����������ζ�
		
		if( I2C_Read_Bottom( Page, offset, BufA, h ) )continue;//����������ζ�
				
		if( Cmp_Data( IIC_Buf, BufA, h )!=3 ) continue;//�Ƚϲ�һ�´���
		
		if( (Page >= CRC_PAGE) && (Page < 0x350) ) return 0;//���ɹ�
		
		if( Crc_16Bits( IIC_Buf, h ) == 0 ) return 0;//���ɹ�

	}
	 return 2; //fail
} 
