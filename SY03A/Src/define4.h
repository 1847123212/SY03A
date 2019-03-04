#ifndef	__DEFINE_H__
#define __DEFINE_H__

//���峣��, ����
#include "bintohex.h"
#include "V85XX.h"

#define ecode   
#define bdata
#define idata
#define near
#define code    
#define xdata
#define reentrant
#define bit unsigned char
#define huge

//#define EA 0

#define	CARDKEY 	0	//�������ͣ�0:����; 1:����
#define LCDTYPE		0	//Һ����0:��׼; 3:sld

//��0x811800��ʼ�ᱣ��������Ϣ
#define HXVAR(object, addr)   (*((object ecode *) (addr)))
#define const_tx_flag  		HXVAR( unsigned char, 0x811800 )	//��У��־(0xc3Ϊ��̵�У)
#define const_rtc_adj_top 	HXVAR( float,         0x811805 )	//�������ppm
#define const_XcapH		HXVAR( unsigned char, 0x81180d )	//��Уʱʹ�õĵ���ֵ
#define const_XcapL		HXVAR( unsigned char, 0x81180e )

#define const_adc_Testq	 	HXVAR( unsigned int , 0x81204e )	//�ɲ�ʱPTAT ADCֵ(��ADCtrimֵ�µ�ת��)
#define const_adc_TrimH 	HXVAR( unsigned char, 0x812054 )	//��Уʱʹ�õ�ADCtrimֵ
#define const_adc_TrimL 	HXVAR( unsigned char, 0x812055 )	//LSB=1.5mV
#define const_adc_OffsetA	HXVAR( unsigned int , 0x812056 )
#define const_adc_TrimBH 	HXVAR( unsigned char, 0x812064 )	//LSB=2.34mV
#define const_adc_TrimBL 	HXVAR( unsigned char, 0x812065 )
#define const_adc_OffsetB 	HXVAR( unsigned int , 0x812066 )
#define const_lvd_40v 		HXVAR( unsigned char, 0x812029 )	//lvd4.0
#define const_lvd_42v 		HXVAR( unsigned char, 0x81202A )	//lvd4.2
#define const_lvd_44v 		HXVAR( unsigned char, 0x81202B )	//lvd4.4
#define const_lvd_46v 		HXVAR( unsigned char, 0x81202C )	//lvd4.6

#define const_temp_TestA 	HXVAR( unsigned char, 0x812090 )	//����ʱ�¶�����λ
#define const_temp_TestB 	HXVAR( unsigned char, 0x812091 )	//����ʱ�¶�С��λ
#define const_adc_Test	 	HXVAR( unsigned int , 0x812092 )	//����ʱPTAT ADCֵ
//#define const_adc_TestH 	HXVAR( unsigned char, 0x812092 )	//����ʱPTAT ADCֵH
//#define const_adc_TestL 	HXVAR( unsigned char, 0x812093 )	//����ʱPTAT ADCֵL



#define __NOP                             __nop
//unsigned char GPIO0;

#define RD	0x80
#define WR	0x00
#define	 FM17520				 0x02

//FM17520оƬ����//
#define  PAGE0           0x00
#define  COMMAND         0x01
#define  COMMIEN         0x02
#define  DIVIEN          0x03
#define  COMMIRQ         0x04
#define  DIVIRQ          0x05
#define  REGERROR        0x06
#define  STATUS1         0x07
#define  STATUS2         0x08
#define  FIFODATA        0x09
#define  FIFOLEVEL       0x0A
#define  WATERLEVEL      0x0B
#define  CONTROL         0x0C
#define  BITFRAMING      0x0D
#define  COLL            0x0E
#define  RFU0F           0x0F

#define  PAGE1           0x10
#define  MODE            0x11
#define  TXMODE          0x12
#define  RXMODE          0x13
#define  TXCONTROL       0x14
#define  TXAUTO          0x15
#define  TXSEL           0x16
#define  RXSEL           0x17
#define  RXTRESHOLD      0x18

#define	 MODWIDTH				 0x24
#define  RFCFG           0x26
#define  GSN             0x27
#define  CWGSP					 0x28


#define  IntRegLen  		 13				//��ʼ���ļĴ����ֽ���

//FM11RF08оƬ����//
#define SECTION0  0*4
#define SECTION1  1*4
#define SECTION2  2*4
#define SECTION3  3*4
#define SECTION4  4*4
#define SECTION5  5*4
#define SECTION6  6*4
#define SECTION7  7*4
#define SECTION8  8*4
#define SECTION9  9*4
#define SECTION10  10*4
#define SECTION11  11*4
#define SECTION12  12*4
#define SECTION13  13*4
#define SECTION14  14*4
#define SECTION15  15*4

#define BLOCK0  0
#define BLOCK1  1
#define BLOCK2  2
#define BLOCK3  3


#define ERROR    0
#define SUCCESS  1

#define LCDLEDON 	GPIOB->DAT  |= BIT2
#define LCDLEDOFF 	GPIOB->DAT  &= ~BIT2
#define SHIDUANCTRL_ON	__NOP()//GPIO6 &= B1101_1111
#define SHIDUANCTRL_OFF __NOP()//GPIO6 |= B0010_0000

#define RELAYON1 	GPIOF->DAT  |= BIT1
#define RELAYOFF1 GPIOF->DAT  &= ~BIT1
#define RELAYON2 	GPIOF->DAT  |= BIT0
#define RELAYOFF2 	GPIOF->DAT  &= ~BIT0

#define RELAYON3 	GPIOF->DAT  |= BIT1
#define RELAYOFF3 	GPIOF->DAT  &= ~BIT1

#define PLUSELED_ON	 __NOP()	//Ԥ��
#define PLUSELED_OFF __NOP()	//Ԥ��

#define EVENOUTOFF __NOP()	//GPIO3 &= B1101_1111		//���¼��ϱ�
#define EVENOUTON __NOP()//	GPIO3 |= B0010_0000		//���¼��ϱ�

//�ز��ӿڶ���
#define PLC_RST0 	GPIOE->DAT &= ~BIT9   //GPIO3 &= B1111_0111
#define PLC_RST1 	GPIOE->DAT |= BIT9   //GPIO3 |= B0000_1000

#define BEEPALARMON 	__NOP()//do{if(Beep_Io_Ctrl==0x55)GPIO4 &= B1111_1011;else GPIO5 &= B0111_1111;}while(0)
#define BEEPALARMOFF __NOP()//	do{if(Beep_Io_Ctrl==0x55)GPIO4 |= B0000_0100;else GPIO5 |= B1000_0000;}while(0)

//#define RELAYCTRLON	1//GPIO6 |= B0000_0001
//#define RELAYCTRLOFF	1//GPIO6 &= B1111_1110
#define ESAMPOWEROFF	__NOP()//GPIO7 |= B0000_0001
#define ESAMPOWERON	__NOP()//GPIO7 &= B1111_1110
#define RELAYLEDON	NVIC_DisableIRQ(PMU_IRQn );NVIC_DisableIRQ(TMR1_IRQn);GPIOB->DAT  &= ~BIT5;NVIC_EnableIRQ(PMU_IRQn );NVIC_EnableIRQ(TMR1_IRQn);	//Ԥ��//GPIO6 &= B1111_0111
#define RELAYLEDOFF	NVIC_DisableIRQ(PMU_IRQn );NVIC_DisableIRQ(TMR1_IRQn);GPIOB->DAT |= BIT5;NVIC_EnableIRQ(PMU_IRQn );NVIC_EnableIRQ(TMR1_IRQn);	//Ԥ��//GPIO6 |= B0000_1000
#define R485CTRLS	__NOP()//GPIO6 &= B1111_1011
#define R485CTRLR	__NOP()//GPIO6 |= B0000_0100

#define RELAYTEST_P	((GPIOE->STS)& BIT3)//(GPIO6&B0000_0010) == B0000_0010
#define RELAYTEST_N	(!((GPIOE->STS)& BIT3))//(GPIO6&B0000_0010) != B0000_0010

#define DISKEY1_P 	((GPIOA->STS)& BIT13)//(GPIO4&B0010_0000) == B0010_0000
#define DISKEY1_N 	(!((GPIOA->STS)& BIT13))//(GPIO4&B0010_0000) != B0010_0000

#define DISKEY_P 	((GPIOA->STS)& BIT12)//(GPIO4&B0010_0000) == B0010_0000
#define DISKEY_N 	(!((GPIOA->STS)& BIT12))//(GPIO4&B0010_0000) != B0010_0000


#define COVKEY_N 	((GPIOA->STS)& BIT7)//(GPIO4&B0100_0000) == B0100_0000
#define COVKEY_P 	(!((GPIOA->STS)& BIT7))//(GPIO4&B0100_0000) != B0100_0000
//#define PROKEY_P	1
//#define PROKEY_N	1
#define RXHW_P		((GPIOB->STS)& BIT0)
#define RXHW_N		(!((GPIOB->STS)& BIT0))

#if(CARDKEY==1)
#define KEY_N		__NOP()
#define KEY_P		__NOP()
#elif(CARDKEY==0)
#define KEY_P		__NOP()
#define KEY_N		__NOP()
#endif

#define RST_ESAM_H	__NOP()
#define RST_ESAM_L	__NOP()
#define RST_CARD_H __NOP()
#define RST_CARD_L __NOP()

#define METER_RST1	__NOP() //NVIC_DisableIRQ(PMU_IRQn );NVIC_DisableIRQ(TMR1_IRQn); GPIOB->DAT |= BIT6;NVIC_EnableIRQ(PMU_IRQn );NVIC_EnableIRQ(TMR1_IRQn);//����оƬ��λ��
#define METER_RST0	__NOP() //NVIC_DisableIRQ(PMU_IRQn );NVIC_DisableIRQ(TMR1_IRQn); GPIOB->DAT &= ~BIT6;NVIC_EnableIRQ(PMU_IRQn );NVIC_EnableIRQ(TMR1_IRQn);

//��ƵSPI�ӿڶ���
#define	RFSPIRST0		NVIC_DisableIRQ(PMU_IRQn );NVIC_DisableIRQ(TMR1_IRQn);GPIOB->DAT &= ~BIT8;NVIC_EnableIRQ(PMU_IRQn );NVIC_EnableIRQ(TMR1_IRQn);
#define	RFSPIRST1		NVIC_DisableIRQ(PMU_IRQn );NVIC_DisableIRQ(TMR1_IRQn);GPIOB->DAT |= BIT8;NVIC_EnableIRQ(PMU_IRQn );NVIC_EnableIRQ(TMR1_IRQn);
#define	RFSPICLK0		GPIOB->DAT &= ~BIT10
#define	RFSPICLK1		GPIOB->DAT |= BIT10
#define	RFSPIMO0		GPIOB->DAT &= ~BIT11
#define	RFSPIMO1		GPIOB->DAT |= BIT11
#define	RFSPICS0		GPIOB->DAT &= ~BIT9
#define	RFSPICS1		GPIOB->DAT |= BIT9
#define	RFSPIMIGPIO		(GPIOB->STS)
//#define	RFSPIMIBIT		GPIO_Pin_2

//IIC�ӿڶ���
//#define	P_EeprScl			HF_P6_0
#define	P_EeprScl_Low			{NVIC_DisableIRQ(PMU_IRQn );NVIC_DisableIRQ(TMR1_IRQn); GPIOB->DAT &= ~BIT13;NVIC_EnableIRQ(PMU_IRQn );NVIC_EnableIRQ(TMR1_IRQn);}
#define	P_EeprScl_High		{NVIC_DisableIRQ(PMU_IRQn );NVIC_DisableIRQ(TMR1_IRQn);GPIOB->DAT |= BIT13;NVIC_EnableIRQ(PMU_IRQn );NVIC_EnableIRQ(TMR1_IRQn);}

#define	P_EeprScl_PowLowLow			{GPIOB->DAT &= ~BIT13;}
#define	P_EeprScl_PowLowHigh		{GPIOB->DAT |= BIT13;}

#define	Set_InPut_P_EeprScl		{GPIOB->OEN |=BIT13; GPIOB->IE |=BIT13;}
#define	Set_OutPut_P_EeprScl	{GPIOB->OEN &= ~BIT13;GPIOB->IE &= ~BIT13;}

#define	P_EeprSda			((GPIOB->STS)& BIT14)
#define	P_EeprSda_Low		{NVIC_DisableIRQ(PMU_IRQn );NVIC_DisableIRQ(TMR1_IRQn);	GPIOB->DAT &= ~BIT14;NVIC_EnableIRQ(PMU_IRQn );NVIC_EnableIRQ(TMR1_IRQn);}
#define	P_EeprSda_High	{NVIC_DisableIRQ(PMU_IRQn );NVIC_DisableIRQ(TMR1_IRQn);	GPIOB->DAT |= BIT14;NVIC_EnableIRQ(PMU_IRQn );NVIC_EnableIRQ(TMR1_IRQn);}

#define	P_EeprSda_PowLowLow		{GPIOB->DAT &= ~BIT14;}
#define	P_EeprSda_PowLowHigh	{GPIOB->DAT |= BIT14;}


#define	Set_InPut_P_EeprSda	       { GPIOB->OEN |=BIT14;GPIOB->IE |=BIT14;}
#define	Set_OutPut_P_EeprSda	     { GPIOB->OEN &= ~BIT14;GPIOB->IE &= ~BIT14;}                                  


//-------------------------RN8302-SPI??------------------------//
#define     P_RN8302_CS_PIN         { GPIOC->OEN |=BIT0;GPIOC->IE |=BIT0;} 
#define     P_RN8302_CS_PORT        { GPIOC->OEN &= ~BIT0;GPIOC->IE &= ~BIT0;}  
#define     P_RN8302_CS_H           GPIOC->DAT |= BIT0
#define     P_RN8302_CS_L           GPIOC->DAT &= ~BIT0

#define     P_RN8302_SCL_PIN        { GPIOC->OEN |=BIT1;GPIOC->IE |=BIT1;}
#define     P_RN8302_SCL_PORT      { GPIOC->OEN &= ~BIT1;GPIOC->IE &= ~BIT1;}
#define     P_RN8302_SCL_H          GPIOC->DAT |= BIT1
#define     P_RN8302_SCL_L          GPIOC->DAT &= ~BIT1

#define     P_RN8302_SDI_PIN       { GPIOC->OEN |=BIT2;GPIOC->IE |=BIT2;}
#define     P_RN8302_SDI_PORT       { GPIOC->OEN &= ~BIT2;GPIOC->IE &= ~BIT2;}
#define     P_RN8302_SDI_H         GPIOC->DAT |= BIT2
#define     P_RN8302_SDI_L          GPIOC->DAT &= ~BIT2
#define     P_RN8302_SDI_Read       ((GPIOC->STS)&BIT2)

#define     P_RN8302_SDO_PIN         { GPIOC->OEN |=BIT3;GPIOC->IE |=BIT3;}
#define     P_RN8302_SDO_PORT        { GPIOC->OEN &= ~BIT3;GPIOC->IE &= ~BIT3;}
#define     P_RN8302_SDO_H           GPIOC->DAT |= BIT3
#define     P_RN8302_SDO_L           GPIOC->DAT &= ~BIT3

#define     P_RN8302_INTN_PIN        { GPIOA->OEN |=BIT15;GPIOA->IE |=BIT15;}
#define     P_RN8302_INTN_PORT      { GPIOA->OEN &= ~BIT15;GPIOA->IE &= ~BIT15;}
#define     P_RN8302_INTN_H        GPIOA->DAT |= BIT15
#define     P_RN8302_INTN_L         GPIOA->DAT &= ~BIT15
#define     P_RN8302_INTN_Read       ((GPIOA->STS)&BIT15)

//????
#define     P_RN8302_RST_PIN          __NOP()
#define     P_RN8302_RST_PORT        __NOP()
#define     P_RN8302_RST_H         __NOP()
#define     P_RN8302_RST_L          __NOP()


//-------------------------RN8302-SPI??------------------------//



//;============================EEPROM�洢����==========================================
//------------------�Զ�crcУ������������	
#define		ee_remainmoney_page		0x00		//ʣ��������ҳ��ַ
	#define		ee_remainmoney_inpage		0
	#define		ee_remainmoney_lenth		16
#define		ee_esampara_page		0x00		//esam�ﱨ���������֤ʱЧ����ҳ��ַ
	#define		ee_esampara_inpage		ee_remainmoney_inpage+ee_remainmoney_lenth+4
	#define		ee_esampara_lenth		29	
#define		ee_temperature_page		0x00		//�¶Ȳ���ϵ��
	#define		ee_temperature_inpage		ee_esampara_inpage+ee_esampara_lenth+2
	#define		ee_temperature_lenth		11	
				
#define		ee_fwdl_page			0x02		//�����������ҳ��ַ
	#define		ee_fwdl_inpage			0
	#define		ee_fwdl_lenth			24
#define		ee_bkdl_page			0x02		//�����������ҳ��ַ
	#define		ee_bkdl_inpage			ee_fwdl_inpage+ee_fwdl_lenth+4
	#define		ee_bkdl_lenth			24
#define		ee_commaddr_page		0x02		//ͨѶ��ַҳ��ַ
	#define		ee_commaddr_inpage		ee_bkdl_inpage+ee_bkdl_lenth+4
	#define		ee_commaddr_lenth		6	

#define		ee_limitmenory_page		0x04		//͸֧���ڻ����ͺ�բ������ҳ��ַ
	#define		ee_limitmenory_inpage		0
	#define		ee_limitmenory_lenth		12
#define		ee_meterconst_page		0x04		//���峣��ҳ��ַ
	#define		ee_meterconst_inpage		ee_limitmenory_inpage+ee_limitmenory_lenth+2
	#define		ee_meterconst_lenth		4
#define		ee_emupara_page			0x04		//����оƬ����ҳ��ַ
	#define		ee_emupara_inpage		ee_meterconst_inpage+ee_meterconst_lenth+2
	#define		ee_emupara_lenth		20
#define		ee_password_page		0x04		//03����04������ҳ��ַ
	#define		ee_password_inpage		ee_emupara_inpage+ee_emupara_lenth+2
	#define		ee_password4_inpage		ee_emupara_inpage+ee_emupara_lenth+8
	#define		ee_password_lenth		4
//------------------
#define		ee_loop_displayitem_page	0x06		//ѭ����ʾ��Ŀҳ��ַ
#define		ee_loop_displayitem_1_page	0x07		//ѭ����ʾ��Ŀҳ��ַ
#define		ee_key_displayitem_page		0x08		//������ʾ��Ŀҳ��ַ
#define		ee_key_displayitem_1_page	0x09		//������ʾ��Ŀҳ��ַ
	#define		ee_displayitem_lenth		62		
#define		ee_tariffprice1_page	0x0a		//��ǰ�׷��ʵ��ҳ��ַ
#define		ee_tariffprice2_page	0x0b		//�����׷��ʵ��ҳ��ַ
	#define		ee_tariffprice_inpage		0
	#define		ee_tariffprice_lenth		16
#define		ee_event_point_page		0x0a		//������¼�ָ��ҳ��ַ
	#define		ee_eventpoint_inpage		18
#define		ee_validrec_page		0x0c		//������¼���Чָ��ҳ
	#define		ee_timingf_inpage		0x00	//00h��ʱ����
	#define		ee_dayf_inpage			0x01	//01h�ն���
	#define		ee_histroyf_inpage		0x02	//02h�������
	#define		ee_hourf_inpage			0x03	//03h���㶳��
	#define		ee_immedf_inpage		0x04	//˲ʱ����
	#define		ee_agreesqf_inpage		0x05	//Լ������ʱ�����л�
	#define		ee_agreesdf_inpage		0x06	//Լ��������ʱ�α��л�
	#define		ee_agreeflf_inpage		0x07	//Լ��������ʵ���л�
	#define		ee_agreejtf_inpage		0x08	//Լ����������л�
	#define		ee_jtsqswith_inpage		0x09	//����ʱ���л�
	#define		ee_modeswith_inpage		0x0A	//�ѿ�ģʽ�л�
	#define		ee_clearn_inpage		0x0B	//����
	#define		ee_cleareventn_inpage		0x0C	//���¼�
	#define		ee_powerdownn_inpage		0x0D	//����
	#define		ee_setclockn_inpage		0x0E	//Уʱ
	#define		ee_covern_inpage		0x0F	//�����
	#define		ee_maxi_inpage			0x10	//����
	#define		ee_openrelayn_inpage		0x11	//��բ
	#define		ee_closerelayn_inpage		0x12	//��բ
	#define		ee_programn_inpage		0x13	//���
	#define		ee_shiqun_inpage		0x14	//ʱ������
	#define		ee_shiduann_inpage		0x15	//ʱ�α���
	#define		ee_jieshuann_inpage		0x16	//�����ձ��
	#define		ee_keysetn_inpage		0x17	//��Կ���±��
	#define		ee_combn_inpage			0x18	//��Ϸ�ʽ���
	#define		ee_relayerrn_inpage		0x19	//���ɿ�������
	#define		ee_powererrn_inpage		0x1A	//��Դ�쳣
	#define		ee_weekdayn_inpage		0x1B	//�����ձ��
	#define		ee_errcardn_inpage		0x1C	//�쳣�忨	
	#define		ee_batterlow_inpage		0x1D	//���Ƿѹ
	#define		ee_buymoneyn_inpage		0x1E	//����
	#define		ee_dmoneyn_inpage		0x1F	//�˷Ѽ�¼
	#define		ee_flpricen_inpage		0x20	//���ʵ�۱��
	#define		ee_jtpricen_inpage		0x21	//���ݱ���	
	#define		ee_cleardemand_inpage	0x22	//0dh����  2018-09-28
	#define		ee_histroyDM_inpage		0x23	//��������   2018-09-28
	#define		ee_eventpoint_lenth		0x24	//ָ�����ݳ���
#define		ee_curyearjs_page		0x0c		//��ǰ��1~6�������
	#define		ee_curyearjs_inpage	0x00+ee_eventpoint_lenth+2
	#define		ee_curyearjs_len		18
	
#define		ee_Demand_Period_page		0x0c		//��ǰ��1~6�������
	#define		ee_Demand_Period_inpage	ee_curyearjs_inpage+ee_curyearjs_len+2
	#define		ee_Demand_Period_len		2
	
#define		ee_shiqu1_page			0x0e		//��һ��ʱ��ҳ��ַ
#define		ee_shiqu2_page			0x0f		//�ڶ���ʱ��ҳ��ַ
	#define		ee_shiqu_lenth			42
//------------------
#define		ee_displaypara_page		0x0a		//��ʾ����ҳ��ַ
	#define		ee_displaypara_inpage		54		
	#define		ee_displaypara_lenth		8	
#define		ee_weekholiday_page		0x0e		//�����պ�������ʱ�α��ҳ��ַ
	#define		ee_weekholiday_inpage		44
	#define		ee_weekholiday_lenth		18
//------------------		
#define		ee_shiduantable1_1_page		0x10		//��һ�׵�1ʱ�α�ҳ��ַ
#define		ee_shiduantable1_2_page		0x11		//��һ�׵�2ʱ�α�ҳ��ַ
#define		ee_shiduantable1_3_page		0x12		//��һ�׵�3ʱ�α�ҳ��ַ
#define		ee_shiduantable1_4_page		0x13		//��һ�׵�4ʱ�α�ҳ��ַ
#define		ee_shiduantable1_5_page		0x14		//��һ�׵�5ʱ�α�ҳ��ַ
#define		ee_shiduantable1_6_page		0x15		//��һ�׵�6ʱ�α�ҳ��ַ
#define		ee_shiduantable1_7_page		0x16		//��һ�׵�7ʱ�α�ҳ��ַ
#define		ee_shiduantable1_8_page		0x17		//��һ�׵�8ʱ�α�ҳ��ַ
#define		ee_shiduantable2_1_page		0x18		//�ڶ��׵�1ʱ�α�ҳ��ַ
#define		ee_shiduantable2_2_page		0x19		//�ڶ��׵�2ʱ�α�ҳ��ַ
#define		ee_shiduantable2_3_page		0x1a		//�ڶ��׵�3ʱ�α�ҳ��ַ
#define		ee_shiduantable2_4_page		0x1b		//�ڶ��׵�4ʱ�α�ҳ��ַ
#define		ee_shiduantable2_5_page		0x1c		//�ڶ��׵�5ʱ�α�ҳ��ַ
#define		ee_shiduantable2_6_page		0x1d		//�ڶ��׵�6ʱ�α�ҳ��ַ
#define		ee_shiduantable2_7_page		0x1e		//�ڶ��׵�7ʱ�α�ҳ��ַ
#define		ee_shiduantable2_8_page		0x1f		//�ڶ��׵�8ʱ�α�ҳ��ַ
	#define		ee_shiduantable_lenth		42
//------------------

#define		ee_keysts_page		0x10		//KEY(8)+crc(2) 2018.8.27
  #define		ee_keysts_inpage		44
  #define		ee_keysts_lenth		17	
	
#define		ee_eventthreshold_page		0x12		//�¼��ж��趨��Χҳ��ַ
	#define		ee_eventthreshold_inpage	44
	#define		ee_eventthreshold_lenth		6			
#define		ee_histroyparameter_page	0x14		//����ʱ�����ҳ��ַ
	#define		ee_histroyparameter_inpage	44
	#define		ee_histroyparameter_lenth	6
#define		ee_feik_number_page		0x14		//ʱ��ʱ�η��ʽ�����ҳ��ַ
	#define		ee_feiknumber_inpage		ee_histroyparameter_inpage+ee_histroyparameter_lenth+2
	#define		ee_feiknumber_lenth		8				
#define		ee_freezeparameter_page		0x16		//����ʱ�����ҳ��ַ
	#define		ee_freezeparameter_inpage	44
	#define		ee_freezeparameter_lenth	12	
#define		ee_factorymode_page		0x17		//����ģʽ������(��������Ҫ��)	
	#define		ee_factorymoder_inpage		44
	#define		ee_factorymode_lenth		1
#define		ee_pre_dayfreeze_page		0x18		//ר�������ϵ粹�����һ���ն���ʱ��ҳ��ַ
	#define		ee_predayfreeze_inpage		44
	#define		ee_predayfreeze_lenth		5			
#define		ee_rtcadj_time_page		0x18		//д���¶����Уֵʱʱ���¼ҳ��ַ(ʼ�ղ���)
	#define		ee_rtcadjtime_inpage		ee_predayfreeze_inpage+ee_predayfreeze_lenth+2
	#define		ee_rtcadjtime_lenth		10		
#define		ee_usercard_serial_page		0x19		//�����к�ҳ��ַ
	#define		ee_usercard_serial_inpage	44
	#define		ee_usercard_serial_lenth	6		
#define		ee_changecontrol_page		0x1a		//ʱ��ʱ������������ҳ��ַ
	#define		ee_changecontrol_inpage		44
	#define		ee_changecontrol_lenth		2
#define 	ee_operatorcode_page		0x1a		//�����ߴ���ҳ��ַ
	#define		ee_operatorcode_inpage		ee_changecontrol_inpage+ee_changecontrol_lenth+2
	#define		ee_operatorcode_lenth		4	
#define		ee_keystatus_page		0x1b		//��Կ״̬ҳ��ַ
	#define		ee_keystatus_inpage		44
	#define		ee_keystatus_lenth		18	
#define		ee_meterstatus1_page		0x1c		//���״̬��1ҳ��ַ
	#define		ee_meterstatus1_inpage		44
	#define		ee_meterstatus1_lenth		3
#define		ee_keystatus1_page		0x1c		//��Կ״̬ҳ1��ַ
	#define		ee_keystatus1_inpage		ee_meterstatus1_inpage+ee_meterstatus1_lenth+2
	#define		ee_keystatus1_lenth		9	
#define		ee_xtlfstop_time_page		0x1d		//����ͣ����ʼʱ��ҳ��ַ
	#define		ee_xtlfstoptime_inpage		44
	#define		ee_xtlfstoptime_lenth		7		
#define		ee_wdt_page			0x1e		//�ϵ縴λ�Ϳ��Ź���λ����ҳ��ַ
	#define		ee_wdt_inpage			44	
	#define		ee_wdt_lenth			8
#define		ee_newclock_page		0x1e		//��ʱ����ʼʱ��ҳ��ַ
	#define		ee_newclock_inpage		ee_wdt_inpage+ee_wdt_lenth+2
	#define		ee_newclock_lenth		7
#define		ee_esam_money_page		0x1f		//esam�ָ�eeʱ����ҳ��ַ
	#define		ee_esam_money_inpage	44
	#define		ee_esam_money_lenth	15	

#define		ee_featureword_page		0x20		//����ģʽ��ҳ��ַ
	#define		ee_featureword_inpage		0
	#define		ee_featureword_lenth		30
#define		ee_starttimer_page		0x20		//ʱ��ʱ�η��ʵ�۽׺��ݵ������ʱ��ҳ��ַ
	#define		ee_starttimer_inpage		ee_featureword_inpage+ee_featureword_lenth+2
	#define		ee_starttimer_lenth		30
#define		ee_7051_page			0x22		//����оƬ��Уҳ��ַ
  #define	ee_7051_lenth				32	
#define		ee_7051_para_page			0x22		//?????????
  #define	ee_7051_para_inpage		ee_7051_lenth+2	
  #define	ee_7051_para_lenth				16	

#define		ee_7051adj_page			0x24		//��ѹ����ҳ
#define		ee_reportdata_page		0x26		//ͣ���¼��ϱ����ݴ洢ҳ(״̬��1,�ϱ�״̬��,����������,��λ��ʱ��)
	#define		ee_reportdata_lenth		56	
#define		ee_power_down_page		0x27		//����ҳ����ҳ��ַ
	#define		ee_powerdown_inpage		0
	#define		ee_powerdown_lenth		41	//������11�ֽ�Ԥ��
	#define		ee_loadpoint_lenth		16	
#define		ee_jtshiqu1_page		0x28		//��ǰ�׽���ʱ����
#define		ee_jtshiqu2_page		0x29		//�����׽���ʱ����
	#define		ee_jtshiqu_inpage		0
	#define		ee_jtshiqu_len			12	
#define		ee_ptatvalue_page		0x2a		//����¶�ADCҳ��ַ
	#define		ee_ptatvalue_inpage		0
	#define		ee_ptatvalue_lenth		42
#define		ee_capvalue_page		0x2b		//������ƫ��ҳ��ַ
	#define		ee_capvalue_inpage		0
	#define		ee_capvalue_lenth		42
#define		ee_hearttime_page					0x2a		//�������ʱ��ҳ��ַ
	#define		ee_hearttime_inpage			ee_ptatvalue_lenth+2
	#define		ee_hearttime_lenth			1	

#define ee_TOKEN_page	0x2d	//�����׵�ַ  //
  #define		ee_TOKEN_inpage		0
	#define		ee_TOKEN_lenth		50	

#define ee_DayUserINFO_page	0x2e	//�����׵�ַ  //
  #define		ee_UserINFO_inpage		0
	#define		ee_UserINFO_lenth		6	
	
#define ee_MonthUserINFO_page	0x2e	//�����׵�ַ  //
  #define		ee_MonthUserINFO_inpage		8
	#define		ee_MonthUserINFO_lenth		6			
	
#define ee_TIDCNT_page	0x2e	//�����׵�ַ  //
  #define		ee_TIDCNT_inpage		16 
	#define		ee_TIDCNT_lenth		1  //1����	
	
//------------------���Զ�crcУ����	
#define		ee_holiday_page			0x30		//�ڼ���ʱ�α�ҳ��ַ,֧��60����������(��4ҳ)
#define		ee_holiday_lenth		60									
//------------------�õ���������
#define		ee_pre1_histroyusedl_page	0x36		//��1-2������ʷ�õ���ҳ��ַ	
#define		ee_pre1_mhistroyusedl_page	0x37		//��1-2����ʷ�õ���ҳ��ַ
	#define		ee_histroyusedl_lenth		16	//���ݳ���16�ֽ�
#define		ee_pre1_yhistroyusedl_page	0x38		//��1-7����ʷ�õ���ҳ��ַ	
	#define		ee_yhistroyusedl_lenth		56	//���ݳ���56�ֽ�	
//------------------�����
#define		ee_protocolver_page		0x34		//Э��汾��ҳ��ַ
	#define		ee_protocolver_inpage		48
	#define		ee_protocolver_lenth		16
#define		ee_softver_page			0x35		//����汾��ҳ��ַ
	#define		ee_softver_inpage		0
	#define		ee_softver_lenth		32
#define		ee_hardver_page			0x35		//Ӳ���汾��ҳ��ַ
	#define		ee_hardver_inpage		32
	#define		ee_hardver_lenth		32	
#define		ee_factorynum_page		0x36		//���ұ���ҳ��ַ
	#define		ee_factorynum_inpage		24
	#define		ee_factorynum_lenth		32
#define		ee_softbk_page			0x36		//���������ҳ��ַ
	#define		ee_softbk_inpage		56
	#define		ee_softbk_lenth			8
#define		ee_ratedcurr_page		0x37		//�����ҳ��ַ
	#define		ee_ratedcurr_inpage		24
	#define		ee_ratedcurr_lenth		6
#define		ee_maxcurr_page			0x37		//������ҳ��ַ
	#define		ee_maxcurr_inpage		30
	#define		ee_maxcurr_lenth		6
#define		ee_activeclass_page		0x37		//����ȵȼ�ҳ��ַ 
	#define		ee_activeclass_inpage		36
	#define		ee_activeclass_lenth		4	
#define		ee_matertype_page		0x37		//����ͺ�ҳ��ַ
	#define		ee_matertype_inpage		40
	#define		ee_matertype_lenth		10						
#define		ee_meterdate_page		0x37		//��������
	#define		ee_meterdate_inpage		50
	#define		ee_meterdate_lenth		10			
//------------------���طѿر�̼�¼��	
#define		ee_recordnumber_page		0x39		//�¼���¼����ҳ
	#define		ee_recordnumber_lenth		63	//���ݳ���63�ֽ�
#define         ee_maxi_note_page             	0x3a		//�������ؼ�¼ҳ��ַ
	#define		ee_maxinote_inpage		0
	#define		ee_maxinote_lenth		38	//���ݳ���38�ֽ�
#define		ee_jtpricerecord_page		0x3a		//���ݱ��̼�¼(����ʱ��6�Ͳ����ߴ���4)
	#define		ee_jtpricerecord_inpage		ee_maxinote_inpage+ee_maxinote_lenth	
	#define		ee_jtpricerecord_len		10
#define		ee_keysetrecord_page		0x3a		//��Կ���¼�¼ҳ
	#define		ee_keysetrecord_inpage		ee_jtpricerecord_inpage+ee_jtpricerecord_len
	#define		ee_keysetrecord_len		15
#define		ee_batterlow_note_page		0x43		//���Ƿѹ��¼ҳ��ֻ��һ����¼
	#define		ee_batterlownote_inpage		ee_keysetrecord_inpage
	#define		ee_batterlownote_len		14
#define		ee_powerdown_note_page		0x44		//�����¼���¼ҳ��ַ
	#define		ee_powerdownnote_inpage		0
	#define		ee_powerdownnote_lenth		28
#define		ee_dmoney_note_page		0x44		//�˷��¼���¼ҳ��ַ
	#define		ee_dmoneynote_inpage		ee_powerdownnote_inpage+ee_powerdownnote_lenth
	#define		ee_dmoneynote_lenth		19
	
#define		ee_clear_demand_page		0x4e
  #define		ee_clear_demand_inpage		0
	#define		ee_clear_demand_lenth		26

#define		ee_errcardrecord_page		0x58		//�쳣�忨��¼
	#define		ee_errcardrecord_inpage		0
	#define		ee_errcardrecord_len		36	
#define		ee_buydl_note_page		0x58		//�����¼���¼ҳ��ַ
	#define		ee_buydlnote_inpage		ee_errcardrecord_inpage+ee_errcardrecord_len	
	#define		ee_buydlnote_lenth		23	//���ݳ���23�ֽ�	
	
//------------------�¼���¼��
#define		ee_cleardl_note_page		0x62		//������¼�ҳ��ַ
	#define		ee_cleardlnote_inpage		0
	#define		ee_cleardlnote_lenth		18
#define		ee_clearevent_note_page		0x62		//���¼���¼ҳ��ַ
	#define		ee_cleareventknote_inpage	ee_cleardlnote_inpage+ee_cleardlnote_lenth
	#define		ee_cleareventnote_lenth		14
#define		ee_week_note_page			0x62	//�����ձ�̼�¼
	#define		ee_week_note_inpage		ee_cleareventknote_inpage+ee_cleareventnote_lenth
	#define		ee_week_note_len		11
#define		ee_powererr_note_page		0x62		//��Դ�쳣��¼ҳ��ַ
	#define		ee_powererrnote_inpage		ee_week_note_inpage+ee_week_note_len
	#define		ee_powererrnote_lenth		20	
	
#define		ee_closerelay_note_page		0x6c		//��բ�¼���¼ҳ��ַ
	#define		ee_closerelaynote_inpage	0
	#define		ee_closerelaynote_lenth		18
#define		ee_openrelay_note_page		0x6c		//��բ�¼���¼ҳ��ַ
	#define		ee_openrelaynote_inpage		ee_closerelaynote_inpage+ee_closerelaynote_lenth		
	#define		ee_openrelaynote_lenth		18	
#define		ee_opencover_note_page		0x6c		//���Ǽ�¼ҳ��ַ
	#define		ee_opencovernote_inpage		ee_openrelaynote_inpage+ee_openrelaynote_lenth
	#define		ee_opencovernote_lenth		28

#define		ee_jieshuanrecord_page		0x76		//�����ձ�̼�¼ҳ��ַ
	#define		ee_jieshuanrecord_inpage	0
	#define		ee_jieshuanrecord_lenth		16	
#define		ee_setclock_note_page		0x76		//����ʱ�Ӽ�¼ҳ��ַ
	#define		ee_setclocknote_inpage		ee_jieshuanrecord_inpage+ee_jieshuanrecord_lenth
	#define		ee_setclocknote_lenth		16
#define		ee_relayerr_note_page		0x76		//���ɿ����쳣��¼ҳ��ַ
	#define		ee_relayerrnote_inpage		ee_setclocknote_inpage+ee_setclocknote_lenth
	#define		ee_relayerrnote_lenth		29
	
#define		ee_program_note_page		0x80		//����¼�ҳ��ַ
	#define		ee_programnote_inpage		0
	#define		ee_programnote_lenth		50
	#define		max_eventrecord_p		10	//�¼�ָ�����ֵ(10��)
#define		ee_combstatusrecord_page	0x80		//�й����״̬�ּ�¼ҳ��ַ
	#define		ee_combstatusrecord_inpage	ee_programnote_inpage+ee_programnote_lenth
	#define		ee_combstatusrecord_lenth	11
	
#define		ee_shiqurecord_page		0x8a		//ʱ�����̼�¼ҳ��ַ
	#define		ee_shiqurecord_lenth		57
#define		ee_shiduanrecord_page		0x94		//ʱ�α��̼�¼ҳ��ַ
	#define		ee_shiduanrecord_lenth		57
	
//------------------���㶳���¼��
#define         ee_hourfreezedl_page	        0x9e		//���㶳�����ҳ��ַ
	#define		ee_hourfreezedl_lenth		13	//���ݳ���13�ֽ�
	#define		max_hourfreeze_p		254	//���㶳�����ָ�����ֵ(254����)
#define         ee_dayfreezemoney_page	        0x9e		//�ն�����ҳ��ַ
	#define		ee_dayfreezemoney_inpage	52
	#define		ee_dayfreezemoney_lenth		10	//���ݳ���10�ֽ�
	
//------------------�����
#define		ee_meterpos_page		0xdc		//���ܱ�λ����Ϣ
	#define		ee_meterpos_inpage		52
	#define		ee_meterpos_lenth		11	
#define		ee_zichannum_page		0xdd		//�ʲ��������ҳ��ַ
	#define		ee_zichannum_inpage		26
	#define		ee_zichannum_lenth		32	
	
//------------------���������¼��
#define		ee_timingfreeze_dl_page		0xde		//��ʱ��������ҳ��ַ
	#define		max_timingfreeze_p		60	//Լ���������ָ�����ֵ(60��)
#define		ee_dayfreezedl_page		0x156		//�ն�������ҳ��ַ
	#define		ee_dayfreezedl_len		55
	#define		max_dayfreeze_p			62	//�ն������ָ�����ֵ(62��)
#define		ee_realtimefreeze1_page		0x1d2		//˲ʱ���ᶳ��ҳ��ַ
#define		ee_appointfreeze1_page		0x1d8		//ʱ��ת��Լ������ҳ��ַ
#define		ee_appointfreeze2_page		0x1dc		//ʱ��ת��Լ������ҳ��ַ
#define		ee_appointfreeze3_page		0x1e0		//����ת��Լ������ҳ��ַ
#define		ee_appointfreeze4_page		0x1e4		//����ת��Լ������ҳ��ַ
	#define		ee_freezedl_lenth		48	//���ݳ���24�ֽ�(��48�ֽڴ��2ҳ)
#define         ee_histroy_dl_page              0x1e8		//�����յ���ҳ��ַ
	#define		ee_histroydl_lenth		48	//���ݳ���24�ֽ�(��48�ֽڴ��2ҳ)
	#define		max_histroy_p			12	//�������ָ�����ֵ(12)
	
//******************************************************************
 //BLOCK51	 Ԥ����_ STS ��Կ��������������//
 #define F01_BLOCK51_ADR_START	0							//�����׵�ַ  //
 #define LEN_BLOCK51_STSKEY_E2P	1					//������Ч�������ݳ���  //
 #define OFFSET_BLOCK51_CSCK	1		//����16bitsУ�������ƫ����  //
 #define ADR_BLOCK51_CSCK_E2P	1 //����16bitsУ�������洢��ַ  //
 
#define ADR_STSKEY_DATA8	1//�����Կ���� ,������ȡ,8�ֽ�//
#define ADR_STSKEY_KEN		1	//��Կ��Ч��,8bit, //
#define ADR_STSKEY_KT		1//��Կ����, 2bit ,//
#define ADR_STSKEY_KRN	1	//��Կ�汾��,4bit //
#define ADR_STSKEY_TI		1// ����������,8bit,0~99//	
#define ADR_STSKEY_FACID	1	// sts ע�Ṥ���������0092//	
#define ADR_STSKEY_SGC		1	// sts ע��SGC���//	


 //BLOCK52	 Ԥ����_���20��TID��¼����������//
 #define F01_BLOCK52_ADR_START	1	//�����׵�ַ  //
 #define LEN_BLOCK52_TIDREC_E2P	1	//������Ч�������ݳ���  //
 #define OFFSET_BLOCK52_CSCK	1//����16bitsУ�������ƫ����  //
 #define ADR_BLOCK52_CSCK_E2P	1		//����16bitsУ�������洢��ַ  //
 
#define ADR_TIDREC_01 1//���1��TID��¼//


//------------------���ɼ�¼��
#define		ee_ploadrec_page		0xde
	#define		max_ploadrec_p			289
	#define		ee_ploadrec1_inpage		24	//ʱ��(5)+��ѹ(2)+����(3)+Ƶ��(2)
		#define		ee_ploadrec1_len	12
	#define		ee_ploadrec2_inpage     	36	//ʱ��(5)+�й�����(3)
		#define		ee_ploadrec2_len	8
	#define		ee_ploadrec3_inpage		44	//ʱ��(5)+��������(2)
		#define		ee_ploadrec3_len	7
	#define		ee_ploadrec4_inpage		51	//ʱ��(5)+�������ܵ���(8)
		#define		ee_ploadrec4_len	13

#define		ee_illegalcardnum_page		0x1ff		//�Ƿ��忨����ҳ
	#define		ee_illegalcardnum_inpage	24
	#define		ee_illegalcardnum_len		20

//------------------��Կ��
#define		ee_keydata_1_page			0x240		//���0 ��32�ֽ���Կ+���1 ��32�ֽ���Կ����������ÿҳ2��
#define		ee_jtsqswith_note_page		0x250		//����ʱ���л�����
	#define		ee_jtsqswith_note_inpage	0
#define		ee_modeswith_note_page		0x254		//�ѿ�ģʽ�л�����
	#define		ee_modeswith_note_inpage	0
#define		ee_flpricerecord_page		0x258	//���ʱ��̼�¼
	#define		ee_flpricerecord_inpage		0	
	#define		ee_flpricerecord_len		42			
#define		ee_jtpricerecord2_page		0x300		//���ݱ��̼�¼
	#define		ee_jtpricerecord2_inpage	0	
	#define		ee_jtpricerecord2_len		70

#define		ee_ladder1_page			0x350		//��ǰ�׽��ݱ����/��ۣ�4�ű�ÿҳһ�ű�
#define		ee_ladder2_page			0x354		//�����׽��ݱ����/��ۣ�4�ű�ÿҳһ�ű�
	#define		ee_ladder_lenth			70	
	
//BLOCK52	 Ԥ����_���20��TID��¼����������//
#define ee_TID_page	            0x360	//�����׵�ַ  //
  #define		ee_TID_inpage		    0
	#define		ee_TID_lenth		    60  //1����
	
		
#define ee_Pre12monthMon_page	  0x362	//�����׵�ַ  //
  #define		ee_Pre12monthMon_inpage		0
	#define		ee_Pre12monthMon_lenth		72	
	
#define ee_Pre7DayMon_page	0x363	//�����׵�ַ  //
  #define		ee_Pre7DayMon_inpage	0
	#define		ee_Pre7DayMon_lenth		42	


#define		ee_p_demand_page		0x364		//������������ҳ��ַ
  #define		ee_p_demand_inpage		0
	#define		ee_pdemand_lenth		40
#define		ee_n_demand_page		0x364		//�����������ҳ��ַ
  #define		ee_n_demand_inpage		42
	#define		ee_ndemand_lenth		40
	
#define		ee_histroyp_demand_page		0x366		//��������������ҳ��ַ,һ����ҳ
  #define		ee_histroyp_demand_inpage		0
	#define		ee_histroydemand_lenth		40	
#define		ee_histroypn_demand_page		0x366		//��������������ҳ��ַ,һ����ҳ
  #define		ee_histroypn_demand_inpage		42
	#define		ee_histroydemand_lenth		40
	
	
#define		ee_Chk_RN8302_page		0x380		//	У�����У���
  #define		ee_Chk_RN8302_inpage		100		//	У�����У���
  #define		ee_Chk_RN8302_lenth		4
	
#define		ee_para1_RN8302_page		0x380		//	У�����
  #define		ee_para1_RN8302_inpage		0		//	У�����
  #define		ee_para1_RN8302_lenth		30	
	
#define		ee_para2_RN8302_page		0x380		//	У�����
  #define		ee_para2_RN8302_inpage		32		//	У�����
  #define		ee_para2_RN8302_lenth		30	
	
	#define		ee_para3_RN8302_page		0x380		//	У�����
  #define		ee_para3_RN8302_inpage		64		//	У�����
  #define		ee_para3_RN8302_lenth		30	
	

	
	
	
	
	
	
	
//�����ܴ���(3)+����ʱ��(6)+�����ߴ���(4)+����ʱ�����й�����������ͷ���ʱ��(8)+����ʱ�����й�����������ͷ���ʱ��(8)

#define		max_histroydm_p			12	//��������ָ�����ֵ(12)	
#define	  max_cleardemand_p   10

//#define		ee_event_demand_page		0x362		//������¼�ָ��ҳ��ַ
//	#define		ee_event_demand_inpage		74
//	#define		ee_event_demand_lenth		1
//	
//#define		ee_validrecdemand_page		0x362		//������¼���Чָ��ҳ	
//  #define		ee_validrecdemand_inpage		77
//	#define		ee_validrecdemand_lenth		1


//#define		ee_clear_demand_page		0x372		//��������¼ҳ��ַ
//	#define		ee_cleardemand_lenth		29
//;=============================��س������壽==========================================
#define		maxtimes			6		//������屶��,96Aʱ,7������/s
#define		max_maxholiday			60		//��󹫹�������
#define		default_updatemoneytimer	120		//Ĭ��Ǯ������ʱ��(min),��Χ������15min~240min֮��
#define		ms_timer_delay			100	//460msʱ�궨ʱ,ˢ����ʾ
#define		comm_led_timer_delay		5		//5sͨѶָʾ��
#define		shiduanctrl_timer_delay		20		//80msʱ�θı������ʱ����ʱ
#define		max_shiduancount		14		//���ʱ��14�ֽ�ʱ�α�
#define		default_loopdisplaytimer	5		//Ĭ�ϵ�ѭ��ʱ��5s
#define		default_keydisplaytimer		60		//Ĭ�ϰ�����ʾʱ��Ϊ60s
#define		lcdled_timer_delay		60		//60s������
#define	 	disp_num			33		//��ʾ�б��е���Ŀ��

#define 	ESAM				0		//esam
#define 	CARD				1		//IC card
#define 	u7816_buf_size			220		//

//;=============================��س�������======================
#define   STS_MAIN_CLASS_PURCHASE   0X00     //������TOKEN
//  main_class= 0  sub_classs define 
#define   STS0_SUB_CLASS_PURCHASE_KWH       0X00     //purchase  by KWH
#define   STS0_SUB_CLASS_PURCHASE_MONEY 0X04     //purchase  by MONEY

////////////////////////////////////////////////////////
//main_class= 1 ��� ������
////////////////////////////////////////////////////////
#define   STS_MAIN_CLASS_TEST   0X01     //����TOKEN
#define   STS1_SUB_CLASS_TEST  0X00     //  main_class= 1  sub_classs define 

//  main_class= 1  sub_classs=0     STS TEST  CODE define
#define   STS_TEST_ALL                  0   //test all the contents
#define   STS_TEST_RELAY                1   ///test relay 
#define   STS_TEST_LCD                  2   //test LCD display
#define   STS_TEST_TOTAL_ENERGY         3   //Display total energy
#define   STS_TEST_KRN       			4   //Display KRN
#define   STS_TEST_TI         			5   //display TI
#define   STS_TEST_REV_6        		6   //����
#define   STS_TEST_MPL        			7   //display maximum power limit
#define   STS_TEST_TAMPER_STATUS		8   //display tamper status
#define   STS_TEST_CURRENT_POWER		9   //display power consumption
#define   STS_TEST_METER_VERSION		10  //display meter version number

////////////////////////////////////////////////////////
//main_class= 2 ��� ������
////////////////////////////////////////////////////////
#define   STS_MAIN_CLASS_SET        0X02     //��������TOKEN
//  main_class= 2  sub_classs define 
#define   STS2_SUB_CLASS_SET_MAXPOWER           0    //set up Max Power Limit Mode Token
#define   STS2_SUB_CLASS_CLR_BALANCE            1     //To clear balance(������)
#define   STS2_SUB_CLASS_SET_SINGLE_TARRIF      2     //To set up single tarrif
#define   STS2_SUB_CLASS_SET_SECRET_KEY_H       3     //key change Token -1
#define   STS2_SUB_CLASS_SET_SECRET_KEY_L       4     //key change Token -2
#define   STS2_SUB_CLASS_CLR_TAMPER_EVEN        5     //To clear event

////////////////////////////////////////////////////////
//TID��¼����
////////////////////////////////////////////////////////
#define TID_DATA_LEN  	3		//TID��¼���ݳ���3�ֽ�  //
#define TID_REC_MAXNUM  20		//TID��¼����50��//

// 2����Կ�ļ��ʱ�䶨�壬��λ�� //
#define MAX_SECONDS_OF_MULTIPLE_TOKEN_INTERVAL		90    // 90�� //   

//gs_APDU_analyze_var.err_flg ���������ֶ���//
#define	STS_TOKEN_OK					0	//����token�������OK//
#define	STS_TOKEN_REJRCT_1				31	//����ʧ�ܣ���������Կ����//				
#define	STS_TOKEN_REJRCT_2				32	//��֧�ֵ�TOKEN��//	
#define	STS_TOKEN_REJRCT_3				33	//DDTK��Կ����֧�ֳ�ֵTOKEN//	
#define	STS_TOKEN_REJRCT_4				34	//���ܳɹ�������TOKEN�ڵ����ݳ�����Χ//	
#define	STS_TOKEN_REJRCT_5				35	//��Կ�滻Ȩ�޲��㣬������Կ��ֹ�޸�DITK��Կ//	
#define	STS_TOKEN_REJRCT_6				36	//TID������Կ��Ч��//	
#define	STS_TOKEN_REJRCT_USED			37	//TOKEN�Ѿ���ʹ�ù�//	
#define	STS_TOKEN_REJRCT_OLD			38	//TOKEN�ѹ��ڣ���ǰTOKEN��TID<��¼�����1��TID��//	
#define	STS_TOKEN_REJRCT_OVER_CRD		39	//��ֵ������ȷ���ǳ����ڻ�����//	
#define	STS_TOKEN_CHG_KEY_PLS_2nd		40	//��Կ���ĵ�1�������ɹ����ȴ������2��//	
#define	STS_TOKEN_CHG_KEY_PLS_1st		41	//��Կ���ĵ�2�������ɹ����ȴ������1��//	

//sts_kay_change_data.flg define 
#define KeyChangeHO BIT0
#define KeyChangeLO BIT1

//LineV ѡ�����ѹ����Ƶ��
#define	    F_LineA	  0x01//��A���ѹ����Ϊ��׼��־
#define	    F_LineB	  0x02//��B���ѹ����Ϊ��׼��־
#define	    F_LineC	  0x04//��C���ѹ����Ϊ��׼��־
#define	    F_LineV	  0x07

//ECDir
#define	    F_ActRevZ	  0x01//�й��ܹ��ʷ���
#define	    F_ActRevA	  0x02//A���й�����
#define	    F_ActRevB	  0x04//B���й�����
#define	    F_ActRevC	  0x08//C���й�����
#define	    F_RActRevZ	  0x10//�޹��ܹ��ʷ���
#define	    F_RActRevA	  0x20//A���޹�����
#define	    F_RActRevB	  0x40//B���й�����
#define	    F_RActRevC	  0x80//C���й�����


//Run
#define	    F_Disp        0x01//????????
#define	    F_Fist	  0x02
#define	    F_Time	  0x04//?????
#define	    F_RevZP0	  0x08
#define	    F_RevZP	  0x10
#define	    F_ZBRest	  0x20//??????

//ECDir1
#define	    F_RevDmdStar  0x01//����״̬�ı��־1
#define	    F_RevDmdStar1 0x02//����״̬�ı��־2
#define	    F_RevDmdStar2 0x04//����״̬�ı��־3
#define	    F_RevDmdStar3 0x08//����״̬�ı��־4
#define	    F_RevDmdStar4 0x10//����״̬�ı��־5
#define	    F_RevDmdStar5 0x20//����״̬�ı��־6
#define	    F_RevDmdStar_Z 0x40//����״̬�ı��־7

//ActMode����й�ģʽ��
#define	    F_IncPp  	  0x01//�������й�
#define	    F_DecPp       0x02//�������й�
#define	    F_IncPn       0x04//�ӷ����й�
#define	    F_DecPn       0x08//�������й�

//RactMode����޹�ģʽ��
#define	    F_IncQuadr1   0x01//��1�����޹�
#define	    F_DecQuadr1   0x02//��1�����޹�
#define	    F_IncQuadr2   0x04//��2�����޹�
#define	    F_DecQuadr2   0x08//��2�����޹�
#define	    F_IncQuadr3   0x10//��3�����޹�
#define	    F_DecQuadr3   0x20//��3�����޹�
#define	    F_IncQuadr4   0x40//��4�����޹�
#define	    F_DecQuadr4   0x80//��4�����޹�


//;==============================�������Ͷ��壽============================================
typedef unsigned char	uint08; 		//unsigned 8 bit data
typedef unsigned short	uint16;
typedef	unsigned long int uint32;


typedef signed char	int08; 			//unsigned 8 bit data
typedef signed short	int16;
typedef	signed long int	int32;

typedef unsigned char INT8U;
typedef unsigned short	INT16U;
typedef	unsigned long int INT32U;


typedef	signed	short	INT16S;
typedef signed	long	INT32S;

typedef	unsigned char  BOOLEAN;
typedef	unsigned char  BOOL;
typedef	unsigned char  ErrorStatus; 

typedef	unsigned char  uchar;
typedef	unsigned char  uint8;
typedef	unsigned char  u8;
typedef	signed char	INT8S;
typedef	signed char	schar;



typedef	unsigned 	short	uint;
typedef	unsigned 	short	uint16;
typedef	unsigned 	short	u16;
typedef	signed	short	INT16S;
typedef	signed	short	sint;



#pragma pack(push,1)
typedef union
{
  unsigned char B08;
  struct
  {
    unsigned char bit0:1;
    unsigned char bit1:1;
    unsigned char bit2:1;
    unsigned char bit3:1;
    unsigned char bit4:1;
    unsigned char bit5:1;
    unsigned char bit6:1;
    unsigned char bit7:1;
  }Bit;
}B08_Bit;



union Union_WordDef
{
	unsigned short WORD;
	unsigned char BYTE[2];
};
union Union_DWordDef
{
	signed long   DWORD;
	unsigned long  UDWORD;
	unsigned short WORD[2];
	unsigned char BYTE[4];
};

union B16_B08
{
	unsigned char B08[2];
	uint16  B16;
};

union S16_B08
{
	unsigned char B08[2];
	int16  S16;
};

union B32_B08
{
	unsigned char     B08[4];
	unsigned long int B32;
};
union B32_B08_2						//С�˴洢(51Ϊ���)
{
	unsigned long int B32;
	unsigned char     B08[4];
};

union B16_B08_2						//С�˴洢(51Ϊ���)
{
	unsigned short	B16;
	unsigned char     B08[2];
};

struct B32ADDB08
{
	union B32_B08     E32;
	uint08		  B08;
};

struct MONEY_FRM
{
	uint08		Remain[6];			//ʣ����
	uint08		Tick[6];			//��Ƿ���
	union B32_B08	Buy_Times;			//�������
	union B16_B08	Crc16;				//crc16
};

struct USERINFOR_FRM
{
	uint08	Day_Moneyused[6];			//��һ��ʹ�ý�� BCD		
  union B16_B08	DayCrc16;	 
	uint08	Month_Moneyused[6];			//��һ��ʹ�ý�� BCD		
	union B16_B08	MonthCrc16;				//crc16
	union B32_B08		Day_Moneyviable;			//ʣ�����������BCD
};

struct FWDL_FRM
{
	union B32_B08	Fwdl_T;				//�����ܵ���
	union B32_B08	Fwdl[4];			//����1~4�������
	union B32_B08	Usedl;				//���������õ���
	union B16_B08	Crc16;				//crc
};

struct BKDL_FRM
{
	union B32_B08	Bkdl_T;				//�����ܵ���
	union B32_B08	Bkdl[4];			//����1~4�������
	union B32_B08	Usedl;				//���·����õ���
	union B16_B08	Crc16;				//crc	
};


struct LOADREC_FRM
{
	union B16_B08	Validpoint[4];			//��Чָ��
	union B16_B08	Point[4];			//����ָ��
	union B16_B08 	Crc16;				//CRCУ����
};


struct METERCONST_FRM
{
	union B32_B08	Const;				//���峣��bcd��,4�ֽ�,���ֽ���ǰ
	union B16_B08 	Crc16;				//CRCУ����
	uint08		Const_H;			//0.01kwh��������
};

struct PARA_ESAM_FRM
{
	union B32_B08	Alarm_Limit1;			//�������1
	union B32_B08	Alarm_Limit2;			//�������2
	uint08		Current_Ratio[3];		//�������
	uint08		Voltage_Ratio[3];		//��ѹ���
	uint08		Meter_Id[6];			//���
	uint08		User_Id[6];			//�û���
	uint08		Card_Type;			//������
	union B16_B08	Identity_Delay;			//�����֤ʱЧ
	union B16_B08 	Crc16;				//CRCУ����
};

struct PARA_LIMITMENORY_FRM
{
	union B32_B08	Tick;				//͸֧���
	union B32_B08	Regrate;			//�ڻ����
	union B32_B08	Closerelay;			//�պϼ̵���������
	union B16_B08 	Crc16;				//CRCУ����
};

struct PARA_DISPLAY_FRM
{
	uint08		S_Number;			//��ʾ����
	uint08		Loop_Time;			//ѭ��ʱ��
	uint08		Dl_Dot;				//����С��λ��
	uint08		P_Dot;				//����С��λ��
	uint08		K_Number;			//������ʾ����
	uint08		Vthreshold;			//���Ƿѹ��ֵ
	uint08		MeterCtrl;			//�ѿ�ģʽ��
	uint08		A_Time;				//�ϵ�ȫ��ʱ��
	union B16_B08 	Crc16;				//CRCУ����
};

struct PARA_DEMAND_FRM
{	
	uint08		Max_Demand_Period;		//�����������
	uint08		Sliding_Time;			    //����ʱ��	
	union B16_B08 	Crc16;				  //CRCУ����
};

struct COMM_ADDRESS_FRM
{
	uint08		Comm_Id[6];			      //ͨѶ��ַ
	union B16_B08 	Crc16;				  //CRCУ����
};

struct RTC_TRIMA_FRM
{
	union B16_B08	PTAT_Value[21];		//�¶�ADC
	union B16_B08 	Crc16;				  //CRCУ����
};

struct RTC_TRIMC_FRM
{
	union S16_B08	CAP_Value[21];		//����ƫ��
	union B16_B08 	Crc16;				  //CRCУ����
};

struct STARTTIME_FRM
{
	uint08		Shiqu[5];			//��ʱ������ʱ��
	uint08		Shiduan[5];			//��ʱ�α�����ʱ��
	uint08		Rate[5];			//�·��ʵ������ʱ��
	uint08		Ladder[5];			//�½��ݵ������ʱ��
	uint08		LadSQ[5];			//����ʱ���л�ʱ��
	uint08		FKmode[5];			//�ѿ�ģʽ�л�ʱ��(Ԥ��)
	union B16_B08 	Crc16;				//CRCУ����
};

struct EMU_FRM
{
	union B32_B08	rate[5];
//	union B32_B08	Uxxx;				//��ѹϵ��
//	union B32_B08	I1xxx;				//����1ϵ��
//	union B32_B08	I2xxx;				//����2ϵ��
//	union B32_B08   P1xxx;				//����ϵ��
//	union B32_B08 	Fxxx;				//BLƵ��ϵ��
	union B16_B08 	Crc16;				//CRCУ����
};


struct WEEK_FRM
{
	uint08		Status;				//������
	uint08		Shiduan_N;			//ʱ�α��(��Χ0~7)	
	uint08		Lstarttimer[5];			//���ɼ�¼��ʼʱ��,�ڲ���չ��,��5�ֽ�(����ǰ)
	uint16		Lspacetimer[4];			//��1~4�ฺ�ɼ�¼���ʱ��,��8�ֽ�(���ֽ���ǰ)
	union B16_B08	Max_I;				//�����¼�������������    ��λ��ǰ �����ж�
	uint08		Max_I_Period;			//�����¼�������ʱʱ��							
	union B16_B08 	Crc16;				//CRCУ����	
};


struct PARA_FEATUREWORD_FRM
{
	uint08		Bps[1];				//ͨ�ſ�1
	uint08		Comp_B;				//�й������
	uint08		Freeze[6];			//����/��ʱ/˲ʱ/Լ��/����/�ն���ģʽ��		
	uint08		Feature1;			//������1
	uint08		Report[8];			//�ϱ�ģʽ��
	uint08		Limit_I[3];			//�̵�����բ���Ƶ�������ֵ 
	uint08		Exp_Timer[6];			//��չ���Ӷ�ʱbcd��,�ֱ�Ϊ��բ��ʱ2,�������բ��ʱ2,������֤ʱ��1,�ϱ��Զ���λʱ��1
	uint08		Wz_Clocerelay;			//�̵�����բ����ʽ(0x55���ñ�ʾ�Զ���բ,0x55���ñ�ʾ�ֶ���բ)
	uint08		Plc;				//��չģʽ��,0x0A����0xAAÿ��20:00��λ�ز�һ��(��Ҫ�����˹���ز�),0xA0����0xAA�����������
	uint08		Other;				//Ԥ��
	uint08		Zbps[1];			//�ز�ͨ�ſ�
	union B16_B08 	Crc16;				//CRCУ����
};


struct TEMPERATURE_FRM
{
	uint08		Offset[8];			//�¶ȵ���ϵ��8�ֽ�
	uint08		RtcTMode;			//RTC��������ģʽ��0x55:����ģʽ��0xAA���²���ֵģʽ����������ͨģʽ
	union B16_B08	XLcap;				//����ƥ�����
	union B16_B08 	Crc16;				//CRCУ����
};


struct PRICE_FRM
{
	union B32_B08	Ladder_Dl[6];			//��ǰ���ݵ���
	union B32_B08	Ladder[7];			//��ǰ���ݵ��
	uint08		Year_Ladder[18];		//��ǰ����ݽ�����1~6
	union B16_B08 	Crc16a;				//CRCУ����
	union B32_B08	Tariff[4];			//��ǰ���ʵ�۱�
	union B16_B08 	Crc16c;				//CRCУ����
	union B32_B08   Current_L;			//��ǰ���ݵ��
	union B32_B08   Current_F;			//��ǰ���ʵ��
	union B32_B08   Current;			//��ǰ���
	union B32_B08   Current_X;			//��ǰ��۳��Ա�Ⱥ���
	uint08		Ladder_Mode;		//����ģʽ��,0x00Ϊ�½���,0x55Ϊ�����
};

struct TARIFF_FRM
{
	uint08		Table[42];			//��ǰ����ʱ�α�,3*14��42�ֽ�
	union B16_B08 	Crc16;				//CRCУ����
	uint08		Current;			//��ǰ����(��Χ0~3)
	uint08		Current_Ladder;			//��ǰ����(��Χ0~4)
	uint08		Current_N;			//��ǰʱ�α��(��Χ0~7)
	uint08		Period_N;			//��ǰʱ�κ�(��Χ0~13)
	uint08		CurLad_N;			//��ǰ���ݱ��(��Χ0~3)
	uint08		Status;				//bit3ʱ��/bit2ʱ�α�/bit1����/bit0����(0:��һ�ף�1:�ڶ���)
		
};


struct ITEM_FRM
{
	uint08		 Display[62];			//��ʾ��Ŀ1*62�����1ҳ,��4ҳ
	union B16_B08 	 Crc16;
};

struct SYS_CLOCK_FRM
{
	uint08   	Clockmain[7];			//ϵͳʱ��,[0]��[1]��[2]ʱ[3]��[4]��[5]��[6]��
	uint08   	Clockbackup[7];			//ϵͳʱ��,[0]��[1]��[2]ʱ[3]��[4]��[5]��[6]��
//	uint08   	Status;				//ʱ��״̬��
};

struct UIP_FRM
{
	union B32_B08   U;				//��ѹxxxxxxx.x
	union B32_B08   I1;				//����xxxxx.xxx
	union B32_B08   P1;				//�й�����xxxx.xxxx
	union B32_B08   I2;				//���ߵ���xxxxx.xxx
  union B32_B08   P2;				//���ڹ���xxxx.xxxx	
	union B32_B08   Cosa;				//��������xxxxx.xxx	
	union B32_B08   Freq;				//����Ƶ��
	
	union B32_B08   Pav;				//һ����ƽ������xxxx.xxxx
};

struct FUNCTION_FRM
{
	union B16_B08   Temper_M;			//��ǰ����¶�xxx.x
	union B16_B08   Voltage_B;			//��ǰ��ص�ѹxx.xx
	union B32_B08   Voltage_B_T;			//��ص�ѹ����ʱ��xxxxxxxx(hex����)
	union B16_B08 	Crc16;				//
};

struct TIMER_FRM
{
	uint08		Timer;				//��ʱ��
	uint08		Status;				//״̬
};

struct DECIMAL_FRM
{
	uint08		Fw[1];	//��������β��
	uint08		Bk[1];	//��������β��
	uint08		Unit_Pluse;		//��λ��������ۼ�(0.01kwh�ı���)
	uint08		Unit_Bkpluse;	//��λ��������ۼ�(0.01kwh�ı���)
	uint08		Status;			//bit0���ȸ���;bit1���仯;bit2Ǯ������,����Ԥ��
};

struct DISPALY_FRM
{
	uint08		Number;				//��ʾ��
	uint08		Timer;				//��ʾʱ��
	uint08		Status;				//״̬
};

struct EEPROM_FRM
{
	uint08		Buffer[64];			//����ee������/lcd��ʾ���帴��
	uint08		Status;				//״̬�ֽ�
};

struct HisEvent_STR
{
	uint16 	        DI21;	
	uint16  	page;
	unsigned char  	offset;
	unsigned char  	len;
	unsigned char  	offset2;
};

struct ADCDATA_FRM
{
	uint16		Buffer[16];			//ADC���
	uint08		ReadCount;			//����
};

struct FEIK_FRM
{
	unsigned char Num[8];
	union B16_B08 Crc16;
};

struct TIMER2_FRM
{
	//0�Ƿ��忨����(4)
	//1��������ʱ��(4)
	//2���Ƿѹ���ۼ�ʱ��(4)
	//3�������ۼ�ʱ��(4)
	//4��ع���ʱ��(4) 
	union B32_B08	Times[5];
};

//����ͨ�ų�ʱʱ�䳣��
#define txtimeout_max	125				//500ms�������ַ��������ʱ
#define rxtimeout_max	125  				//500ms 
#define framedelay_mx	15 				//60ms
//ͨ�Ż�������󳤶ȶ���
#define	utx_buf_size	230				//ͨ�Ż�������󳤶ȣ�220
#define	urx_buf_size	230				//ͨ�Ż�������󳤶ȣ�220

struct COMMUNICATION_FRM
{
	uint08 Buf[ urx_buf_size ];			//���ջ���/���ͻ���
	uint08 Len;					//���ճ����ֽ�/��������
	uint08 Total_Len;				//�����ܳ���/�����ܳ���
	uint08 Step;  					//���ղ���
	uint08 Flag;  					//ͨѶ��־
	uint08 Response_Timer;				//ͨѶ��Ӧ��ʱ��
	uint08 Overtime_Timer;				//ͨѶ���ջ��߷�����ʱ��ʱ��
};

typedef struct 
{
    INT8U   apdu_data_bcd[10]; 	//ԭʼ20λTOKEN����//
    INT8U   apdu_data_hex[9]; 	//ת�����9�ֽ����� //
    INT8U   main_class;			//���� //
    INT8U   sub_class;			//����//        
	  INT32U  u32_TID;			//TID//     
	  INT32U  u32_PropData;		//��������//   	
	  INT8U	u8_result;			//�������  //
}APDU_ANALYZE_VAR;

typedef struct 
{
    INT8U   secret_key[8];  //�����Կ���� ,������ȡ,8�ֽ�//
    INT8U   ken;			//��Կ��Ч��,8bit, //
    INT8U   kt;				//��Կ����, 2bit ,//
    INT8U   krn;			//��Կ�汾��,4bit //
	  INT8U	ti;				// ����������,8bit,0~99//	
	  INT8U	id_factory[2];	// �������//		
	  INT8U	u24_SGC[3];		// SGC���//		
    INT16U  u16_csck;               //16λ�ۼӺ�У��ֵ//	
}APDU_KEY_EXRAM_VAR;

typedef struct  
{
    INT8U   kay[8];                	//�����Կ����  //
    INT8U   ken;					//��Կ��Ч�� //
    INT8U   kt;                    	//��Կ���� //
    INT8U   krn;					//��Կ�汾�� //
    INT8U   ro;						//������Կ��ʶ //
	  INT8U	ti;						// ����������//
	  INT8U	flg;					//��һ����Կ��Ч��ʶ//
	  INT16U   tmr_second;             // ��һ����Կ��Ч����ʱ������� //
}STS_KAY_CHANHE_DATA;

typedef struct 
{
    INT8U   hex24_tid[TID_REC_MAXNUM][TID_DATA_LEN];  //TID��¼�����20��//
    INT16U  u16_csck;			//16λ�ۼӺ�У��ֵ//	
}APDU_TID_REC;

//���������� ���ӽṹ���Աʱ��Ҫ�޸����ݳ���
typedef struct//��ӳ�Աʱע��ṹ�����
{
    unsigned short  Un150;
    unsigned short  Un90;
    unsigned short  Un75;
    unsigned short  Un68;
    unsigned short  Un60;
    unsigned short  Un40;
    unsigned long   StartP;//��������
    unsigned long   StartI;//��������
    unsigned long   StartP2;//��������2
    unsigned long   StartI2;//��������
    unsigned long   Ib05;//0.05Ib
    unsigned long   Ib30;//0.3Ib
    unsigned long   Ib350;//3.5Ib
    unsigned long   Imax120;//1.2Imax
    unsigned long   DmdCp;
} MEASPARA;

#define  MeasParaLen  sizeof(MEASPARA)
//˲ʱ������
typedef struct
{
    unsigned short U[3];    //XXX.X A��B��C���ѹ BCD
    unsigned short PF[4];   //X.XXX �ܡ�A��B��C�๦������ BCD
    unsigned long  I[3];    //XXX.XXX A��B��C����� BCD
    unsigned long  PKva[4]; //XX.XXXX �ܡ�A��B��C�����ڹ��� BCD
    unsigned long  PKw[4];  //XX.XXXX �ܡ�A��B��C���й����� BCD
    unsigned long  PKvar[4];//XX.XXXX �ܡ�A��B��C���޹����� BCD
    unsigned short Freq;    //����Ƶ��
    unsigned short PhaseAngle[3];//A��B��C���
    unsigned short ComBatU;//ͣ����ص�ѹ
    unsigned short RTCBatU;//ʱ�ӵ�ص�ѹ
    unsigned short Temp;//�����¶�
} REAL;


//У��˲ʱ���ṹ�嶨��
typedef struct
{
    unsigned long  UAdj[3]; //XXXX.XXXX A��B��C���ѹ BCD
    unsigned long  IAdj[3]; //XX.XXXX A��B��C����� BCD
    unsigned long  PAdj[4]; //XX.XXXXXX �ܡ�A��B��C���й����� BCD
    unsigned long  RAdj[4]; //XX.XXXXXX �ܡ�A��B��C���޹����� BCD
} REALADJ;


//˲ʱ���Ĵ����ṹ�嶨��
typedef struct
{
    unsigned long  Urms[3];//��ѹ�Ĵ�����Чֵ
    unsigned long  Irms[3];//����������Чֵ
    unsigned long  DmdP[2]; //���򡢷��򡢵�ǰ�й�������һ�����ۼƹ���
    unsigned long  DmdTouP[2];//���򡢷����ʱ����һ�����ۼƹ���
    unsigned long  CurDmdP[3];//��ǰ�С��޹�������һ�����ۼƹ���
    unsigned long  QuadrDmdP[4];//4�������޹�����һ�����ۼƹ���
    unsigned long  QuadrDmdTouP[4];//4���޷�ʱ�����޹�����һ�����ۼƹ���
    signed long    GrpDmdP[2]; //����޹�1��2������һ�����ۼƹ���
    signed long    GrpDmdTouP[2];//����޹�1��2��ʱ����һ�����ۼƹ���
    unsigned long    ActWatthrP;//�й�����������
    unsigned long    ActWatthrN;//�й�����������
    unsigned long    RactVarhr;//�޹�������
    unsigned long    Vahr;//����������
    unsigned long    EnergyBuf[9];
} CHIPREG;




//��ʶ ���ӽṹ���Աʱ��Ҫ�޸����ݳ���
typedef struct
{
    unsigned char  Error;
    unsigned char  MeterErrAlarm;
    unsigned char  EvenErrAlarm;
    unsigned char  Key;
    unsigned char  Card;
    unsigned char  DispKey;
    unsigned short Power;
    unsigned char  ResetReport;//��λ�����ϱ���־
    unsigned char  CardErr;
    unsigned char  RemoteCmd;
    unsigned char  Run;
    unsigned char  Disp;
    unsigned char  Clock;
    unsigned short Comm;
    unsigned short CommDisp;
    unsigned char  LineV;//Ƶ�ʲ������ѹѡ��
    unsigned char  ECDir;
    unsigned char  ECDir1;//���ڹ��ʱ�־
    unsigned short Relay;
    unsigned char  RelayChk;//�̵���״̬���
    unsigned char  MeterState;//���״̬
    unsigned char  MeterState1;//���״̬1
    unsigned char  Alarm;
    unsigned short MeterRun1;
    unsigned short MeterRun2;
    unsigned short MeterRun3;
    unsigned short MeterRun4;
    unsigned short MeterRun5;
    unsigned short MeterRun6;
    unsigned short MeterRun7;
    unsigned char  Fee;
    unsigned char  MulPortMode;
    unsigned char  LoadMode;//���ɼ�¼ģʽ�ֱ�־
    unsigned char  LoadTime;//ͨ��ʱ������־
    unsigned char  FrzType;//�����־
    unsigned char  FrzType1;//�����־
    unsigned char  BalanRequest;//�����־
    unsigned char  DmdSave;//��������洢
    unsigned char  EvenLostV;//ʧѹ��־
    unsigned char  EvenLowV;//Ƿѹ��־
    unsigned char  EvenOverV;//��ѹ��־
    unsigned char  EvenLossP;//�����־
    unsigned char  EvenUnbla;//��ѹ�����������򡢲�ƽ���־
    unsigned char  EvenLostI;//ʧ����־
    unsigned char  EvenOverI;//������־
    unsigned char  EvenDuanI;//������־
    unsigned char  EvenRevP;//���ʷ����־
    unsigned char  EvenOverP;//���ر�־
    unsigned char  EvenOverDmdA;//�й��������ޱ�־
    unsigned char  EvenOverDmdR;//�޹��������ޱ�־
    unsigned char  EletPar;//����ǡ�����ť�Ǳ�־
    unsigned char  EleIYZBla;//�������ز�ƽ���־
    unsigned char  EvenLosPower;//������Դʧ���־
    unsigned char  AlllostV;
    unsigned char  AllLostVEecF;
    unsigned char  RecordDelF;
    unsigned char  RecordDelTim;
    unsigned char  LowPower;
    unsigned char  DataSave;
    unsigned char  MeterPowerState;//������ǰ��״̬��ʱ�ӵ�ء�ͣ����ء�����ǡ����˸ǡ���բ����բ
    unsigned char  RevAlarm;
    unsigned char  SymbolGprs;//����ͨ���ź�ǿ��
    unsigned short PhOfset0;//��λ����
    unsigned short PhOfset1;//��λ����
    unsigned long  LedAlarmEn;//������ʹ�ܿ���
    unsigned long  BellAlarmEn;//����������ʹ�ܿ���
    unsigned long  ExternAlarmEn;//����������ʹ�ܿ���
    unsigned long  AlarmEn0;
    unsigned long  AlarmEn1;
    unsigned long  AlarmEn2;
    unsigned long  AlarmEn3;
    unsigned long  AlarmEn00;
    unsigned long  AlarmEn11;
    unsigned long  AlarmEn22;
    unsigned long  AlarmEn33;
    unsigned short  MinFrzType; //�����־���16ͨ��

    unsigned char  BalanEngNum;//��������������
    unsigned char  BalanDmdNum;//��������������
    unsigned char  BalanDmdClr;//�������������־
    unsigned char  MonitorNum;//����ѹ�ϸ�����
    unsigned char  BalanceEng1_Num;//��������1�Ķ�������
    unsigned char  EsamState;//��ǰ��Կ״̬
    unsigned char  EsamChg;//��Կ�ı�
    unsigned char  TimeChgBill;
    unsigned char  TimeChgFrz;
    unsigned char  BalanceEng1_MDNum;//��������1�Ķ������������ն�����
    unsigned char  BalanceEng1_MDNum2;//��������1�Ķ�����������������
    unsigned char  NewEvents;//�ų����š���Դ�쳣�����ɿ���������־
    unsigned char  RunCharact;//�������������
    unsigned char  EvenOverPF;//������������
    unsigned long  Key_State;//��Կ״̬��
    unsigned long  Key_LastState;//�ϴ���Կ״̬��
    unsigned short CardState;//IC������״̬��־
    unsigned char  EsamStateErr;
    unsigned char  AAAAstate;
    unsigned char  LoadState;//ֱ�Ӳ����ɼ�¼��־
    unsigned char  Addr99;//��ŵ�ַ
    unsigned short ProgramState;//���״̬��־  
    unsigned char  BalanBDong;//����ʱ�����ϵ粹����
    unsigned char  RemoteRelayAllow;//���к�բ����
    unsigned long  CpuCard;//CPU����־
    unsigned char  Eng_Moneystate; //�������仯��־    2016-11-23
    unsigned char  EvenBatLow;//���Ƿѹ��־

    unsigned short Comm698;
    unsigned char ProtectState;//����״̬��־
    unsigned char RelayOutState;//�̵������״̬��bit0��bit7�ֱ�˳���λ��ʾ1��8�ż̵���ң����բ���״̬���á�1������բ״̬���á�0������բ״̬��
    unsigned char RelayAlarmState;//�澯״̬��bit0��bit7�ֱ�˳���λ��ʾ1��8�ż̵���ң�ظ澯���״̬���á�1�������ڸ澯״̬���á�0����δ���ڸ澯״̬��
    unsigned char RelayCmdState;//�̵�������״̬��bit0��bit7�ֱ�˳���λ��ʾ1��8�ż̵���ң����բ�������״̬���á�1������բ����á�0������բ���
    unsigned char AlarmOutState;

} FLAG;

#define  FlagLen  sizeof(FLAG)


//������ ���ӽṹ���Աʱ��Ҫ�޸����ݳ���
typedef struct
{
    unsigned long  Authen_Tmr;//�����֤��Чʱ��
    //unsigned long  TerminalAuthen_Tmr;//�����ն���֤��Чʱ��    //2016-11-21�����ն�����
    unsigned long  InfAuthen_Tmr;//���������֤��Чʱ�� 
    unsigned long   Monitor_Tmr;//�����ܵ�ѹ�ϸ�ʱ��
    unsigned short  RemoteRelayOn_DelayTmr;//�޵�ʱ��
    unsigned long   RemoteRelayOff_DelayTmr;//��բ�澯ʱ��
    unsigned long   Bat_Useing_Tmr;//���ʹ��ʱ��
    unsigned short  LoadInter_Tmr[6];//���ɼ�¼���ʱ�������
    unsigned short   Bell_Tmr;//��������ʱ
    unsigned char   CommMACErrCnt;//����֡MACУ�������
    unsigned char   Pass02or03Err_Cnt;//ͨѶ03��������������
    unsigned char   Pass04Err_Cnt;//ͨѶ04��������������    
    unsigned char   PowerRead_Tmr;//������ܶ�ȡ������
    unsigned char   ChkAde_Cnt;
    unsigned char   KeyAble_Tmr;//���簴��������ʱ
    unsigned char   Wake_Tmr;//���绽����������
    unsigned char   IrWake_Cnt;//ÿ����⻽�Ѵ���
    unsigned char   KeyUp_Tmr;//�Ϸ�����
    unsigned char   Keydown_Tmr;//�·�����
    unsigned char  CloseESAM_Tmr;//ESAM��ʱ�ص�Դ
    unsigned char   PLCInit_Tmr;//�ϵ��ز���RESETʱ��
    unsigned char   ChkEsam_Cnt;//ESAM�����������    
    unsigned short  ResetAute_Tmr;//��λ�����ϱ��ӳ�ʱ��
    unsigned short  Relayon_Tmr;//�̵����ֶ���բ�ӳ�ʱ��
    unsigned long   UpProtRelay_Tmr;//��բ�����������ʱʱ��
    unsigned char   Disp_Tmr;//Һ��ˢ����ʱ
    unsigned char   CheckRelay_Tmr2;//�̵��������Լ���ʱ2
    unsigned char   CommReadCredit_Tmr;
    unsigned char   MulPortCnt;//�๦�ܿ��������ʱ��
    unsigned char   Rev_Cnt[5];
    unsigned char   MulPort_Cnt;//�๦�������ȼ�����
    unsigned char   KeyUpWake_Cnt;//�Ϸ��������Ѽ�����
    unsigned char   KeyDownWake_Cnt;//�·��������Ѽ�����
    unsigned char   IRWakeUp_Cnt;//���⻽�Ѽ�����
    unsigned char   IRCommWait_Cnt;//ͨѶ�ȴ�������������
    unsigned char   AllLostDel_Cnt;//ȫʧѹ��ʱ������
    unsigned char   EngSaveCnt;//����ת�������
    unsigned char   DmdSaveCnt;//����ת�������
    unsigned char   InvOrderV_Tmr;//��ѹ������ʼ��ʱ������
    unsigned char   InvOrderV_TmrEnd;//��ѹ�����������ʱ������
    unsigned char   InvOrderI_Tmr;//����������ʼ��ʱ������
    unsigned char   InvOrderI_TmrEnd;//���������������ʱ������
    unsigned char   OpenCov_Tmr;//�������ʱ������
    unsigned char   OpenCov_TmrEnd;//�������ʱ������
    unsigned char   OpenTer_Tmr;//����ť����ʱ������
    unsigned char   OpenTer_TmrEnd;//����ť����ʱ������
    unsigned char   OverMagn_Tmr;//�ų�������ʱ������
    unsigned char   OverMagn_TmrEnd;//�ų�������ʱ������
    unsigned char   ErrPower_Tmr;//��Դ�쳣��ʱ������
    unsigned char   ErrPower_TmrEnd;//��Դ�쳣��ʱ������
    unsigned char   ErrRelay_Tmr;//���ɼ�¼����ʱ������
    unsigned char   ErrRelay_TmrEnd;//���ɼ�¼������ʱ������   
    unsigned char   ErrClock_Tmr;//ʱ�ӹ��ϼ�����
    unsigned char   ErrClock_TmrEnd;//ʱ�ӹ�����ʱ������
    unsigned char   ErrEMU_Tmr;//����оƬ���ϼ�����
    unsigned char   ErrEMU_TmrEnd;//����оƬ������ʱ������
    unsigned char   OpenRelyDel_Tmr;//�ϵ�����̵�����ʱ
    unsigned char   VoltChkCnt;//�ϵ籨����ѹ��ֵ�����ʱ
    unsigned short   Led_Tmr;//�ϵ�Led����ʱ��
    unsigned char   SaveTim_Cnt;//��ʱ���������
    unsigned char   IntTim_Cnt;
    unsigned char   IntErr_Cnt;
    unsigned char   IntTim_Tmr;
    unsigned short  InfTim_Cnt;
    unsigned char   LossPower_Tmr;//������Դ������ʱ������
    unsigned char   LossPower_TmrEnd;//������Դ������ʱ������
    unsigned short  UpIProtect_Tmr;
    unsigned char   IRAuthen_Tmr;
    unsigned char   ResetReportState_Tmr;
    unsigned char   IntLoad_Cnt;
    unsigned char   PowerUpTim_Tmr;//�ϵ���ʱ������
    unsigned char   LowRTCBat_Tmr;//ʱ�ӵ��Ƿѹ��ʱ������
    unsigned char   LowRTCBat_TmrEnd;//ʱ�ӵ��Ƿѹ��ʱ������
    unsigned char   LowComBat_Tmr;//ͣ�����Ƿѹ��ʱ������
    unsigned char   LowComBat_TmrEnd;//ͣ�����Ƿѹ��ʱ������
    unsigned char   TestRelay_Tmr;//�̵������
    unsigned char   MeterClr_Tmr;//������ʱ�е�ѹ
    unsigned char   RelayOn_Tmr;//�̵�����բ��ʱ�ж�
    unsigned short InstFrzDy;
//    unsigned short MinFrzDy;
//    unsigned short HourFrzDy;
//    unsigned short DayFrzDy;
//    //unsigned short BalanceFrzDy;
//    unsigned short MonthFrzDy;
//    unsigned short SqFrzDy;
//    unsigned short SdFrzDy;
    //unsigned long  DmdPeriodSec;   //�ڼ��������ڼ�����
    unsigned char Delay0p5s;
} TIMER;

#define  TimerLen  sizeof(TIMER)

//��������ṹ�嶨��
typedef struct
{
    unsigned long    ActPowerP;//���й���������˲ʱֵ
    unsigned long    ActPowerN;//���й���������˲ʱֵ
    unsigned long    ActPowerPp[3]; //A��B��C�����й�����˲ʱֵ
    unsigned long    ActPowerPn[3]; //A��B��C�����й�����˲ʱֵ
    unsigned long    RactPower;//���޹�����˲ʱֵ
    unsigned long    RactPowerPp[3]; //A��B��C�޹�����˲ʱֵ
    unsigned long    VahrPower[2];//����������������˲ʱֵ
} POWERVALUE;
#define  LenValue  56


//У������ṹ�嶨��
typedef struct
{
    unsigned short  Igain; //��������
    unsigned short  Ugain; //��ѹ����
    unsigned short  Pgain; //�й���������
    unsigned short  Qgain; //�޹���������
    unsigned short  Iofst; //����ƫ��
    unsigned short  Uofst; //��ѹƫ��
    unsigned short  Pofst; //����ƫ��
    unsigned char  UTofst; //����ͨ��U��λƫ��
    unsigned char  ITofst[3]; //����ͨ��I��λƫ��
    unsigned short PTofst[3]; //�й��С��͡��߹�����λƫ��
    unsigned short QTofst[3]; //�޹��С��͡��߹�����λƫ��
    unsigned short AdjChk;
} ADJ;
#define  LenAdjBuff  30
typedef union
{
    unsigned char   AdjBuff[LenAdjBuff];
    ADJ 	    Adj;
} ADJ_BLOCK;

typedef struct
{
    unsigned short  Iofst2; 
} ADJ2;
#define  LenAdjBuff2  2
typedef union
{
    unsigned char   AdjBuff2[LenAdjBuff2];
    ADJ2 	    Adj2;
} ADJ_BLOCK2;


typedef struct
{
    unsigned char CT;
    unsigned char Meter_Type;
    unsigned char EXTERN_LSE;
    float  MeterUrms;
    float  MeterIrms;   
    float  RN8302Urms;
    unsigned long  RN8302Irms;
    unsigned long  RN8302Prms;
    unsigned short MeterHFConst1;
    unsigned short MeterHFConst2;
    unsigned short MeterUn;
    unsigned short MeterIb;
    unsigned short MeterImax;
    unsigned long MeterCP;
    unsigned long MeterCP2;
}METERPARAMETER;
//Kϵ���ṹ�嶨��
typedef struct
{    
    float  Ku;//��ѹ��Чֵϵ��
    float  Ki;//������Чֵϵ��
    float  Kp;//������Чֵϵ��
    unsigned long    Ke;//1����������ֵ
    unsigned long    EnergyUp;//�����Ĵ�����ȡ������ֵ
    unsigned short  IStart_P;//���ڡ��й�������������ֵ�Ĵ���
    unsigned long  ImaxUp;//������ʾ����
    unsigned long DispUnUp;//��ѹ��ʾ����
    unsigned long DispUnDown;//��ѹ��ʾ����
    unsigned long MConst;//��Ƴ���
} FACTOR;

typedef struct  
{
    unsigned char  b0:1;
    unsigned char  b1:1;
    unsigned char  b2:1;
    unsigned char  b3:1;
    unsigned char  b4:1;
    unsigned char  b5:1;
    unsigned char  b6:1;
    unsigned char  b7:1;
}BITDEFINE;

//�ֽڶ���
typedef union
{
    BITDEFINE      BitDefine;
    unsigned char  ByteDefine;
}BYTEDEFINE;

//4�ֽڶ���
typedef union
{
    unsigned char  Byte[4];
    unsigned long  LongDefine;
}LONGDEFINE;

typedef struct
{
    unsigned char  CurntPz0[4];//��ǰ������������й��ܵ���
    unsigned char  LastPz0[4];//��1������������й��ܵ���
}CYCENERY;

typedef struct
{
    unsigned char  CurntPz0[4];//��ǰ�¶�����й����ۼ��õ���
    unsigned char  LastPz0[4];//��1�¶�����й����ۼ��õ���
}MONTHENERY;

typedef struct
{
    unsigned char  BalanceTime[4];//��ǰʱ��
    unsigned char  CurntPz0[4];//��ǰ�������й����ۼ��õ���
    unsigned char  LastPz0[4];//��1�������й����ۼ��õ���
}YEARENERY;

//�й������� ���ӽṹ���Աʱ��Ҫ�޸����ݳ���
typedef struct
{
    unsigned char  ActPz0[4];//����й�
    unsigned char  ActPz1[4];
    unsigned char  ActPz2[4];
    unsigned char  ActPz3[4];
    unsigned char  ActPz4[4];
    unsigned char  ActPzChk[2];//У��
    unsigned char  ActPp0[4];//�����й�
    unsigned char  ActPp1[4];
    unsigned char  ActPp2[4];
    unsigned char  ActPp3[4];
    unsigned char  ActPp4[4];
    unsigned char  ActPpChk[2];//У��
    unsigned char  ActPn0[4];//�����й�
    unsigned char  ActPn1[4];
    unsigned char  ActPn2[4];
    unsigned char  ActPn3[4];
    unsigned char  ActPn4[4];
    unsigned char  ActPnChk[2];//У��
    unsigned char  ActUsedPz0[4];//���ݵ���ж�ʹ���õ���
    unsigned char  ActUsedPzChk[2];//У��
} ACTENERY;
#define  LenAct  72    ///       66
typedef union
{
    unsigned char   AEngyBuff[LenAct];
    ACTENERY 	    AEngy;
} ACTENERGY_BLOCK;


//�޹������� ���ӽṹ���Աʱ��Ҫ�޸����ݳ���
typedef struct
{
    unsigned char  Grp1Pz0[4];//����޹�1
    unsigned char  Grp1Pz1[4];
    unsigned char  Grp1Pz2[4];
    unsigned char  Grp1Pz3[4];
    unsigned char  Grp1Pz4[4];
    unsigned char  Grp1PzChk[2];//У��
    unsigned char  Grp2Pz0[4];//����޹�2
    unsigned char  Grp2Pz1[4];
    unsigned char  Grp2Pz2[4];
    unsigned char  Grp2Pz3[4];
    unsigned char  Grp2Pz4[4];
    unsigned char  Grp2PzChk[2];//У��
    unsigned char  Quadr1P0[4];//��һ�����޹�
    unsigned char  Quadr1P1[4];
    unsigned char  Quadr1P2[4];
    unsigned char  Quadr1P3[4];
    unsigned char  Quadr1P4[4];
    unsigned char  Quadr1Chk[2];//У��
    unsigned char  Quadr2P0[4];//�ڶ������޹�
    unsigned char  Quadr2P1[4];
    unsigned char  Quadr2P2[4];
    unsigned char  Quadr2P3[4];
    unsigned char  Quadr2P4[4];
    unsigned char  Quadr2Chk[2];//У��
    unsigned char  Quadr3P0[4];//���������޹�
    unsigned char  Quadr3P1[4];
    unsigned char  Quadr3P2[4];
    unsigned char  Quadr3P3[4];
    unsigned char  Quadr3P4[4];
    unsigned char  Quadr3Chk[2];//У��
    unsigned char  Quadr4P0[4];//���������޹�
    unsigned char  Quadr4P1[4];
    unsigned char  Quadr4P2[4];
    unsigned char  Quadr4P3[4];
    unsigned char  Quadr4P4[4];
    unsigned char  Quadr4Chk[2];//У��
} RACTENERY;
#define  LenRact  132
typedef union
{
    unsigned char   REngyBuff[LenRact];
    RACTENERY       REngy;
} RACTENERGY_BLOCK;

typedef struct
{    
    float  Ki[3];//������Чֵϵ��
    float  B_Ki;//������Чֵϵ��
    float  C_Ki;//������Чֵϵ��
} NVM1KI;


//��������� ���ӽṹ���Աʱ��Ҫ�޸����ݳ���
typedef struct
{
    unsigned char  PhAPp[4];//A�������й���
    unsigned char  PhBPp[4];//B�������й���
    unsigned char  PhCPp[4];//C�������й���
    unsigned char  PhAPn[4];//A�෴���й���
    unsigned char  PhBPn[4];//B�෴���й���
    unsigned char  PhCPn[4];//C�෴���й���
    unsigned char  PhChk[2];//У��
    unsigned char  PhAGrp1[4];//A������޹�1
    unsigned char  PhBGrp1[4];//B������޹�1
    unsigned char  PhCGrp1[4];//C������޹�1
    unsigned char  PhAGrp2[4];//A������޹�2
    unsigned char  PhBGrp2[4];//B������޹�2
    unsigned char  PhCGrp2[4];//C������޹�2
    unsigned char  PhGrpChk[2];//У��
    unsigned char  PhAQur1[4];//A���1�����޹�
    unsigned char  PhBQur1[4];//B���1�����޹�
    unsigned char  PhCQur1[4];//C���1�����޹�
    unsigned char  PhAQur2[4];//A���2�����޹�
    unsigned char  PhBQur2[4];//B���2�����޹�
    unsigned char  PhCQur2[4];//C���2�����޹�
    unsigned char  PhAQur3[4];//A���3�����޹�
    unsigned char  PhBQur3[4];//B���3�����޹�
    unsigned char  PhCQur3[4];//C���3�����޹�
    unsigned char  PhAQur4[4];//A���4�����޹�
    unsigned char  PhBQur4[4];//B���4�����޹�
    unsigned char  PhCQur4[4];//C���4�����޹�
    unsigned char  PhQurChk[2];//У��
} PHASEENERY;
#define  LenPhase  102
typedef union
{
    unsigned char   PEngyBuff[LenPhase];
    PHASEENERY 	    PEngy;
} PHASEENERGY_BLOCK;


//���ڵ����� ���ӽṹ���Աʱ��Ҫ�޸����ݳ���
typedef struct
{
    unsigned char  VarPp0[4];//����
    unsigned char  VarPp1[4];
    unsigned char  VarPp2[4];
    unsigned char  VarPp3[4];
    unsigned char  VarPp4[4];
    unsigned char  VarPpChk[2];//У��
    unsigned char  VarPn0[4];//����
    unsigned char  VarPn1[4];
    unsigned char  VarPn2[4];
    unsigned char  VarPn3[4];
    unsigned char  VarPn4[4];
    unsigned char  VarPnChk[2];//У��
} VARENERY;
#define  LenVar  44
typedef union
{
    unsigned char   VEngyBuff[LenVar];
    VARENERY 	    VEngy;
} VARENERY_BLOCK;


//������ ���ӽṹ���Աʱ��Ҫ�޸����ݳ���
typedef struct
{
    unsigned short   PhAPpCnt;//A�������й�������
    unsigned short   PhBPpCnt;//B�������й�������
    unsigned short   PhCPpCnt;//C�������й�������
    unsigned short   PhAPnCnt;//A�෴���й�������
    unsigned short   PhBPnCnt;//B�෴���й�������
    unsigned short   PhCPnCnt;//C�෴���й�������
    unsigned short   AQuadr1PCnt;//A���һ�����޹�������
    unsigned short   BQuadr1PCnt;//B���һ�����޹�������
    unsigned short   CQuadr1PCnt;//C���һ�����޹�������
    unsigned short   AQuadr2PCnt;//A��ڶ������޹�������
    unsigned short   BQuadr2PCnt;//B��ڶ������޹�������
    unsigned short   CQuadr2PCnt;//C��ڶ������޹�������
    unsigned short   AQuadr3PCnt;//A����������޹�������
    unsigned short   BQuadr3PCnt;//B����������޹�������
    unsigned short   CQuadr3PCnt;//C����������޹�������
    unsigned short   AQuadr4PCnt;//A����������޹�������
    unsigned short   BQuadr4PCnt;//B����������޹�������
    unsigned short   CQuadr4PCnt;//C����������޹�������
    signed   short   AGrp1Cnt;//A������޹�1������
    signed   short   BGrp1Cnt;//B������޹�1������
    signed   short   CGrp1Cnt;//C������޹�1������
    signed   short   AGrp2Cnt;//A������޹�2������
    signed   short   BGrp2Cnt;//B������޹�2������
    signed   short   CGrp2Cnt;//C������޹�2������
    signed   short   ActPzCnt[5];//����й�������
    unsigned short   ActPpCnt[5];//�����й�������
    unsigned short   ActPnCnt[5];//�����й�������
    signed   short   Grp1PzCnt[5];//����޹�1������
    signed   short   Grp2PzCnt[5];//����޹�2������
    unsigned short   Quadr1PCnt[5];//��һ�����޹�������
    unsigned short   Quadr2PCnt[5];//�ڶ������޹�������
    unsigned short   Quadr3PCnt[5];//���������޹�������
    unsigned short   Quadr4PCnt[5];//���������޹�������
    unsigned short   VAHRCnt[5];//�������ڵ���������
    unsigned short   VAHFCnt[5];//�������ڵ���������

}PULSECOUNT;
#define  LenCnt  158
#define  LenCnt1  48
#define  LenCnt2  110
typedef union
{
    unsigned char   PCntBuff[LenCnt];
    PULSECOUNT      PCnt;
} PULSECOUNT_BLOCK;


//��ǰ�����ṹ�嶨��
typedef struct
{
    unsigned char  ActDemand[3];//��ǰ�й�����
    unsigned char  RactDemand[3];//��ǰ�޹�����
    unsigned char  VarDemand[3];//��ǰ��������
    unsigned char  Act_F_Demand[3];//��ǰ�����й�����
    unsigned char  Act_R_Demand[3];//��ǰ�����й�����
    unsigned char  Ract_Q1_Demand[3];//��ǰ�޹�1��������   
    unsigned char  Ract_Q2_Demand[3];//��ǰ�޹�2�������� 
    unsigned char  Ract_Q3_Demand[3];//��ǰ�޹�3�������� 
    unsigned char  Ract_Q4_Demand[3];//��ǰ�޹�4�������� 
    
}CURRENTDEMAND;


//��������ṹ�嶨��
typedef struct
{
    unsigned char  TotalDmd[3];//���������
    unsigned char  TotalDmdTime[5];//�������������ʱ��
    unsigned char  Tou1Dmd[3];//���������
    unsigned char  Tou1DmdTime[5];//�������������ʱ��
    unsigned char  Tou2Dmd[3];//���������
    unsigned char  Tou2DmdTime[5];//�������������ʱ��
    unsigned char  Tou3Dmd[3];//ƽ�������
    unsigned char  Tou3DmdTime[5];//ƽ�����������ʱ��
    unsigned char  Tou4Dmd[3];//���������
    unsigned char  Tou4DmdTime[5];//�������������ʱ��
    unsigned char  DmdChk[2];
}MAXDEMAND;
#define  LenDmd  42
typedef union
{
    unsigned char   MaxDmdBuff[LenDmd];
    MAXDEMAND       MaxDmd;
} MAXDMD_BLOCK;



//��ϼ���ģʽ
typedef struct
{
    unsigned char  ActMode;
    unsigned char  RactMode1;
    unsigned char  RactMode2;
} MEASUREMODE;




struct Att7051Eep 
{

  union B32_B08 HFConstD;	
	union B32_B08 PStartD;	
	union B32_B08 GPQAD;	
	union B32_B08 PhsAD;	
	union B32_B08 APOSAD;	
	union B32_B08 IARMSOSD;	
	union B32_B08 IBRMSOSD;
  union B32_B08 EMUCON2D ;
	
	union B16_B08 Crc;	
			//2?????
};
			
struct Att7051 
{
	union B32_B08 HFConst;
  union B32_B08 PStart;		//03H PStart ?????????
	union B32_B08 GPQA;		//0AH L?????????
	union B32_B08 PhsA;		//0EH L??????????
	union B32_B08 APOSA;		//0FH N??????????
	union B32_B08 IARMSOS;		//02H ???????
	union B32_B08 IBRMSOS;		//00H ???????
	union B32_B08 EMUCON2 ;
        
	union B32_B08 DStart;		//01H ???????
	union B32_B08  GPQB;	
	union B32_B08 PhsB;	
	union B32_B08 QPhsCal;	
	union B32_B08 APOSB;	
	union B32_B08 RPOSA;	
	union B32_B08 RPOSB;	
	union B32_B08 IBGain;
	union B32_B08 D2FPL;		//05H L???????
	union B32_B08 D2FPH;		//07H L?????
	union B32_B08 DCIAH;	
	union B32_B08 DCIBH;	
	union B32_B08 DCUH;	
	union B32_B08 DCL;	
		
	
  union B32_B08 SYSCON;	
	union B32_B08 EMUCON;
	
	union B32_B08 CKSUM;	
			//2?????
};

 
union Att7051Para
{
	struct Att7051 A25;
	unsigned long int  E32[25];
};




//����
typedef struct
{
   
	
	
	
	unsigned char TimBef ;//�������������� 
    unsigned char TimRec ;//ʵʱ�������� ����ʹ�ú�E2�洢Ϊ16������
    unsigned char SlidTimB;//�����û���ʱ�� 
    unsigned char SlidTimR;//ʵʱ����ʱ�� ����ʹ�ú�E2�洢Ϊ16������

    unsigned char TotalTimCnt;//���������ڼ�����
    unsigned char TouTimCnt;//��ʱ�������ڼ�����

    unsigned char Grp1TimCnt;//����޹�1�������ڼ�����
    unsigned char Grp1TouTimCnt;//����޹�1��ʱ�������ڼ�����
    
    unsigned char Grp2TimCnt;//����޹�2�������ڼ�����
    unsigned char Grp2TouTimCnt;//����޹�2��ʱ�������ڼ�����

    unsigned char SlidTimCnt;//���������������
    unsigned char Grp1SlidTimCnt;//����޹�1�������������
    unsigned char Grp2SlidTimCnt;//����޹�2�������������

    unsigned char SlidTimTouCnt;//��ʱ�������������
    unsigned char Grp1SlidTimTouCnt;//����޹�1��ʱ�������������
    unsigned char Grp2SlidTimTouCnt;//����޹�2��ʱ�������������

    unsigned char AddFCnt;//1���������й������ۼӼ�����
    unsigned char AddRCnt;//1���ӷ����й������ۼӼ�����
    unsigned char AddFTouCnt;//1���������й��ȹ����ۼӼ�����
    unsigned char AddRTouCnt;//1���ӷ����й��ȹ����ۼӼ�����
    unsigned char AddZCnt[3];//1���ӵ�ǰ���С��޹������ڹ����ۼӼ�����
    unsigned char QuadrAddCnt[4];//4�������޹������ۼӼ�����
    unsigned char QuadrAddTouCnt[4];//1����4���޹��޹������ۼӼ�����
    unsigned char GrpAddCnt[2];//����޹�1��2�����ۼӼ�����
    unsigned char GrpAddTouCnt[2];//����޹�1��2�ȹ��ʼ�����
    
    unsigned char DmdSec;//�������־
    unsigned char DmdMinCnt;//�����ּ�����
    unsigned char TouDmdMinCnt;//��ʱ�����ּ�����
    unsigned char Grp1DmdMinCnt;//����޹�1�����ּ�����
    unsigned char TouGrp1DmdMinCnt;//����޹�1��ʱ�����ּ�����
    unsigned char Grp2DmdMinCnt;//����޹�2�����ּ�����
    unsigned char TouGrp2DmdMinCnt;//����޹�2��ʱ�����ּ�����
    unsigned char DmdTouNo;//��ʱ�������ʺ�
    unsigned char DmdTouWait;
    
    unsigned char RMode1;//����޹�1����ģʽ
    unsigned char RMode2;//����޹�2����ģʽ
    
    unsigned char DmdoutCnt;//����Ͷ�м�����
}DMDCOMM;

/*********************��ѹ����������ֵ����Ӳ������***********************/
typedef struct
{
    unsigned char Addendum_File7;//�����ļ�7
    unsigned char Relay_Mode;//�̵�������
    unsigned char AdjCheck_Mode;
}METERSPECIAL;







#pragma pack(pop)














#endif

