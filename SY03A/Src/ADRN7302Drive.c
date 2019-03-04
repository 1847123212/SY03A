/************************************************************************
* ��Ȩ���� (C)2016, �����������ܿƼ��ɷ����޹�˾��
*
* �ļ����ƣ� // RN8302Drive.c
* �ļ���ʶ�� // 
* ����ժҪ�� //RN8302������������
* ����˵���� // 
* ��ǰ�汾�� // V01.00
* �� �ߣ�   
* ������ڣ� // 
*
* �޸ļ�¼1��//
* �޸����ڣ� //
* �� �� �ţ� // V01
* �� �� �ˣ� //
* �޸����ݣ� //
************************************************************************/
//#define  RN8302Drive_H
//#include "RN8302Drive.H"
#include "ex_func.h"
#include "ex_var.h"
/************************************************************************
* ������ ��//RN8302_SPI_Init
* ���� ��//RN8302�ܽų�ʼ��
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 
**************************************************************************/
void RN8302_SPI_Init(void)
{
    P_RN8302_SCL_H;
	 
    P_RN8302_SDO_H;
    P_RN8302_SCL_PORT;
    P_RN8302_SDO_PORT;
    P_RN8302_SDI_PIN;
}

/************************************************************************
* ������ ��//RN8302_Delay
* ���� ��//��ʱ�ӳ���
* ������� ��//n����ʱʱ��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 
**************************************************************************/
void RN8302_Delay( unsigned long n )
{
    unsigned long i;
 
    for( i = 0; i < n; i++ )
    {
       __NOP(); //__NOP(); __NOP(); __NOP(); __NOP();//Clear_Wdt();//�忴�Ź�
    }
}

/************************************************************************
* ������ ��//RN8302_SendByte
* ���� ��//����һ���ֽ����ݻ��ַ
* ������� ��//TxData:�����͵���Ϣ
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//������CLK�ߵ�ƽʱ����SD0�źţ��ӻ��ڵ͵�ƽʱ��������
* �޸����� :
* ------------------------------------------------------------------------
* 
**************************************************************************/
void RN8302_SendByte(unsigned char TxData)
{
    unsigned char i;

    Clear_Wdt();//�忴�Ź�
    RN8302_SPI_Init();
    for( i = 0; i < 8; i++ )
    {
        P_RN8302_SCL_H;// CLK=1
        RN8302_Delay(1);
        if( (TxData & 0x80) == 0x80 )
            P_RN8302_SDO_H;// SD0=1
        else
        {
            P_RN8302_SDO_L;// SD0 = 0
        }
        RN8302_Delay(3);
        P_RN8302_SCL_L;// CLK = 0
        RN8302_Delay(3);
        TxData <<= 1;
    }
}

/************************************************************************
* ������ ��//RN8302_ReceiveByte
* ���� ��//����һ���ֽ�����
* ������� ��//��
* ������� ��//RxData ��ȡ����
* ����ֵ˵�� ��//��
* ����˵�� ��//�ӻ���CLK�ߵ�ƽʱ����SDI�źţ������ڵ͵�ƽʱ��������
* �޸����� :
* ------------------------------------------------------------------------
*
**************************************************************************/
unsigned char RN8302_ReceiveByte(void)
{
    unsigned char i,RxData=0;
    
    Clear_Wdt();//�忴�Ź�
    RN8302_SPI_Init();
    for( i = 0; i < 8; i++ )
    {
        P_RN8302_SCL_H;// CLK=1
        RN8302_Delay(3);
        P_RN8302_SCL_L;// CLK=0
        RxData <<= 1;
        RN8302_Delay(3);
        if( P_RN8302_SDI_Read )
            RxData |= 0x01;
        RN8302_Delay(3);
    }
    return(RxData);
}

/************************************************************************
* ������ ��//RN8302_Read
* ���� ��//��RN8302�������
* ������� ��//RAM_Addr:���ݵ�ַ,RN8302_Addr:�Ĵ�����ַ,Lenth:���ݳ���
* ������� ��//�ȸߺ��
* ����ֵ˵�� ��//
* ����˵�� ��//��CS�͵�ƽ����������
* �޸����� :
* ------------------------------------------------------------------------
* 
**************************************************************************/
void RN8302_Read( unsigned char *RAM_Addr, unsigned int RN8302_Addr, unsigned char Lenth )
{
    unsigned char i,Temp=0;
    unsigned char chksum=0;
    
    RN8302Status = SUCCESS;
    P_RN8302_CS_L;// CS = 0
    P_RN8302_CS_PORT;
    RN8302_Delay(30);
    Temp = (unsigned char)(RN8302_Addr & 0x00ff);
    chksum = Temp;
    RN8302_SendByte( Temp );//�Ĵ����͵�ַ
    Temp = (((unsigned char)(RN8302_Addr >> 4)) & 0xf0) + 0x00;
    chksum += Temp;
	  RN8302_Delay(30);
    RN8302_SendByte( Temp );//�Ĵ����ߵ�ַ+������
    RN8302_Delay(30);
    for( i = Lenth; i > 0; i-- )
    {
        RAM_Addr[i-1] = RN8302_ReceiveByte(); //�ȸߺ��
        chksum += RAM_Addr[i-1];
    }
    chksum = ~chksum;
    Temp = RN8302_ReceiveByte();   
    if( Temp != chksum )//У���������Ϊ��SPI���ߴ��������SPIУ��Ĵ��� 
    {
        RN8302_SendByte( 0x8c );//ǰ��SPI�������ݼĴ����͵�ַ
        chksum = 0x8c;
        Temp = (((unsigned char)(0x018C >> 4)) & 0xf0) + 0x00;
        chksum += Temp;
        RN8302_SendByte( Temp );//�Ĵ����ߵ�ַ+������
        for(i = 4 ; i > 0 ; i--)//RData�Ĵ���Ϊ4�ֽ�
        {
            Temp = RN8302_ReceiveByte();
            if(Lenth >= i)
            {					
                if(Temp != RAM_Addr[i-1]) 
                {
                    RAM_Addr[i-1] = Temp;//�滻�ϴζ�������
                }	
            }
            chksum += Temp;
        }
        chksum = ~chksum;
        Temp = RN8302_ReceiveByte();   
        if( Temp != chksum )//SPIУ��Ĵ���������У����Ҳ���򷵻ش���Ҫ��λоƬ
        {
            //for( i = 0; i < Lenth; i++ )
            for(i = Lenth ; i > 0 ; i--)
            {
                RAM_Addr[i-1] = 0;//�����߶������建����
            }
            RN8302Status = ERROR;
        }
    }
    P_RN8302_CS_H;  //CS = 1
    if( (RN8302Status == ERROR)&&(RN8302Errcum<10) )
        RN8302Errcum++;
}

/************************************************************************
* ������ ��//RN8302_Write
* ���� ��//��RN8302��д����
* ������� ��//RN8302_Addr:�Ĵ�����ַ,RAM_Addr:���ݵ�ַ,Lenth:���ݳ���
* ������� ��//��
* ����ֵ˵�� ��//0��ȷ
* ����˵�� ��//��CLK������ʱ������SD0�ź�
* �޸����� :
* ------------------------------------------------------------------------
* 
**************************************************************************/
void RN8302_Write( unsigned int RN8302_Addr, unsigned char *RAM_Addr, unsigned char Lenth )
{
    unsigned char i,Temp,Repeat;
    unsigned char chksum=0;
    
    for( Repeat =10; Repeat != 0 ; Repeat--)		
    {
        RN8302Status = SUCCESS;
        P_RN8302_CS_L;// CS = 0
        P_RN8302_CS_PORT;
        RN8302_Delay(30);
        Temp = (unsigned char)(RN8302_Addr & 0x00ff);
        chksum = Temp;
        RN8302_SendByte( Temp );//�Ĵ����͵�ַ
        Temp = (((unsigned char)(RN8302_Addr >> 4)) & 0xf0) + 0x80;
        chksum += Temp;
        RN8302_SendByte( Temp );//�Ĵ����ߵ�ַ+д����
        RN8302_Delay(30);
        for(i = Lenth; i > 0;i-- )
        {
            RN8302_SendByte(RAM_Addr[i-1]);//�ȸߺ�ʹ�������
            chksum += RAM_Addr[i-1];
        }
        chksum = ~chksum;
        RN8302_SendByte( chksum );//У����
//-------------------��WData�Ĵ������-----------------------
        RN8302_SendByte( 0x8D );//ǰ��SPIд�����ݼĴ����͵�ַ
        chksum = 0x8D;
        Temp = (((unsigned char)(0x018D >> 4)) & 0xf0) + 0x00;
        chksum += Temp;
        RN8302_SendByte( Temp );//�Ĵ����ߵ�ַ+������
        RN8302_Delay(30);
        for(i = 3 ; i > 0 ; i--)//WDataΪ3���ֽ�
        {
            Temp = RN8302_ReceiveByte(); //�ȸߺ��
            if(Lenth >= i)
            {					
                if(Temp != RAM_Addr[i-1]) 
                {
                    RN8302Status = ERROR;
                    break;	
                }	
            }
            chksum += Temp;
        }
        if(RN8302Status == SUCCESS) 
        {
            chksum = ~chksum;
            Temp = RN8302_ReceiveByte();   
            if(Temp != chksum)  RN8302Status = ERROR; 
        }
        P_RN8302_CS_H;  //CS = 1
        if( RN8302Status == SUCCESS )
            break; 
				
			RN8302_Delay(100);	
    }
    if( (RN8302Status == ERROR)&&(RN8302Errcum<10) )//���д��������RAM�в�һ���������д�����10��
        RN8302Errcum++;
}

