#include "ex_var.h"
#include "ex_func.h"
#include <string.h>

const union B32_B08_2 code SysATT7051[2]=
{     
      0x00001643,//     80
      0x00000003,// MT0 83     
};
//��Ƶѡ��
#define	MAINCLOCK	4	//ϵͳ��Ƶ,��λM
#define  MeterPara_Mode 5

#if( MAINCLOCK == 8 )
const uint08 ecode runwdtcon = 0x07;//12��Ƶ,cpuwdt��ʱ1.57s
const uint08 ecode rundivsel = 0x00;//ʱ��1��Ƶ,�Ĵ���bit2~4����
const uint08 ecode clkmode  = 0x01;//��ʱ1��Ƶ
const uint08 ecode init_tl0 = 0x55;//4ms��ʱ(8Mʱ��)
const uint08 ecode init_th0 = 0xF5;
#else
const uint08 ecode runwdtcon = 0x06;//12��Ƶ,cpuwdt��ʱ0.784s*2
const uint08 ecode rundivsel = 0x04;//ʱ��2��Ƶ,�Ĵ���bit2~4����
const uint08 ecode clkmode  = 0x02;//��ʱ2��Ƶ
const uint08 ecode init_tl0 = 0xAB;//4ms��ʱ(4Mʱ��)
const uint08 ecode init_th0 = 0xFA;
#endif
const uint08 ecode runoscen = 0x03;//��������ʹ�ܼĴ���,bit4~bit5Ϊxtlf��ǿ��ѡ�����,00����~11ǿ��
const uint08 ecode sleeposcen = 0x02;//ͣ������ʹ�ܼĴ���,bit4~bit5Ϊxtlf��ǿ��ѡ�����,00����~11ǿ��
const uint08 ecode sleeptmr0ie = 0x20;//0x20��tmr0ͣ�绽��,0x00�ر�tmr0ͣ�绽�ѽ�֧��1h����
const uint08 ecode sleepwakeuptimer = 10;//ͣ�绽����syswdtʱ��,16s-6,32s-12,65s-25
const uint16 code Rly_voltage = 0x1710;		//?????????
//���ܱ���
#define RELAYTYPE 0x00 //0x55:���ü̵���������:���ü̵���

#if( RELAYTYPE == 0x55 )
const uint08 ecode Relay_Mode = 0x55;		//0x55-���ü̵�����(�������޸ĳ�80A),����-���ü̵�����(�������޸ĳ�60A)
#else
const uint08 ecode Relay_Mode = 0x00;		//0x55-���ü̵�����(�������޸ĳ�80A),����-���ü̵�����(�������޸ĳ�60A)
#endif
const uint08 ecode Correction_Mode = 0x00;	//0x55-������ѹ��������,����-�ر�(Ŀǰrn8209c��ht7017֧��)
const uint08 ecode Lasttesting_Valid = 0x00;	//0x55-ʹ�óɲ��¶�����,����-ʹ�÷ǳɲ��¶�����
const uint08 ecode Compile_Polar_485 = 0x00;	//0x55-�����޼���485,����-ֻ֧���м���485
const uint08 ecode Compile_Lvd4x = 0x00;	//0x55-���緧ֵ4.2v,0xaa-���緧ֵ4.6v,����-���緧ֵ4.4v,
const uint08 ecode Memoryerr_Ctrl = 0x00;	//0x55-����洢���ϱ�,����-��ֹ�洢���ϱ�(����ر�)
const uint08 ecode Beep_Io_Ctrl = 0x55;		//0x55-���������ƽ�ΪGPIO42,����-���������ƽ�ΪGPIO57(CALCOMMMODE==0��Ч)

//�汾�͹��
const unsigned char ecode Verdate[18] =  "XLJN-181120-01";   //����,�汾�ż��汾���� 
const unsigned char ecode RatedVolt[6] = "230V";	//���ѹ
const unsigned char ecode RatedCurr[6] = "5A";		//�����/��������
#if( RELAYTYPE == 0x55 )
const unsigned char ecode MaxCurr[6] = "80A";	//������
#else
const unsigned char ecode MaxCurr[6] = "60A";	//������
#endif
const unsigned char ecode ActiveClass[4] = "1.0";	//�й�׼ȷ�ȵȼ�
const unsigned char ecode MaterType[10] = "DDSY311";	//����ͺ�
const unsigned char ecode ProtocolVer[16] = "DL/T645-2007-15";//Э��汾��
const unsigned char ecode Softbk[8] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};	//���������
const unsigned char ecode Facdate[10] = "";	//��������

const unsigned long ecode Def_Meterconst_Bcd = 0x1000;	//���峣��Ĭ��1200
const unsigned char ecode Def_Meterconst_Hex = 10;	//���峣��Ĭ��1200
const unsigned char ecode Def_Plusevalidtimer = 20;	//80ms������(ͬ��ͨѶ)
const	float         ecode default_meterconst_dm	=	1000.0; 		//Ĭ�ϵı���Ϊ1200(��������ר��)

const unsigned char ecode Runet1Outcnt[2] = {0x0a,0x3e};//80ms������������üĴ���(�ݲ�֧��)
const unsigned char ecode Runrtctmls = 0x04;		//ʹ����ʱ�����,�������л��ź�	
const unsigned char ecode resetworking_timer_delay=60;	//240ms��λ�ز�оƬʱ��

//�¶�������ز���
const float ecode const_xtl_top	= 25.2;		//�����¶�����ʹ�õĶ���
const float ecode const_KL	= 0.0283+0.003;	//�¶Ȳ���ϵ��
const float ecode const_KH	= 0.0381;
const float ecode const_TmpsL	= 5.1273;	//�¶�ADCб��
const float ecode const_TmpsH	= 5.1273;
const uint08 ecode constF_XcapH = 0x13;
const uint08 ecode constF_XcapL = 0x80;
const uint08 ecode constF_Volt	= 0;		//��ѹ������(������Ӳ��������Ӷ�����),3��ʾ0.03v/10��ʾ0.1v

//�̵������
const uint08 ecode Relaytimer_Delay_ZB= 250;	//1000ms�ز�����բ��ʱʱ��
const uint08 ecode Relaytimer_Delay_N = 16;	//90ms���ü̵�����ƽ��ʱ
const uint08 ecode Powertimer_Delay_N = 30;	//150ms���ü̵�����ƽ��ʱ

const uint08 ecode Relaytimer_Delay_W = 98;	//400ms�����ü̵�����ƽ��ʱ
const uint08 ecode Defaultspacetimer = 5;	//Ĭ��Ԥ��բ���ʱ��(5min)
const uint08 ecode Defrelayerrorcnt_s = 0;	//�̵��������ʾȥ����ʼ����
const uint08 ecode Defrelayerrorcount = 3;	//�̵��������ʾȥ������(ÿ��0.5s)
const uint08 ecode Defrelayerrorcount_r = 8;	//�̵��������ϼ�¼ȥ������(ÿ��0.5s)(����Ҫ������)

//energy


const unsigned long int code METER_PWZEROVAUL  = 0x00000035;
const unsigned long int code METER_I1ZEROVAUL  = 0x00000020;
const unsigned char code METER_BACKCNT  = 2;   //���ʷ�����ʱ�жϴ���  

const uint16 code Deflowledvoltage = 0x1160;	//
const unsigned char code UartErrCnt_const = 30;	     

const uint08 ecode Def_Dl_Difference    = 0x00;	//��ֵ<0x03�������֧��crc��bcd�ݴ�,���������֧���ܵ�������4���ʵ����ۼӺ�0.xxkwh���ݴ�����ܵ���С��4���ʵ����ۼӺ͵��ݴ���
const uint08 ecode Adjclk_timer_delay_s = 90;	//�޼���ͨѶ90s������8Mʱ�ӵ�У����,���Ϊ0��رն�ʱ8Mʱ�ӵ�У����
const uint08 ecode Adjclk_timer_delay   = 2;	//��ͨѶ2h������8Mʱ�ӵ�У����,���Ϊ0��رն�ʱ8Mʱ�ӵ�У����
const uint16 ecode Defendenergyvoltage  = 0;	//���Ϊ0x1250��ʾ125v�رռ���,���Ϊ0���ֹ�رռ���(�������ó�0)
const uint08 ecode Runcalbps = 25;		//����uartͨѶ������(BRGH=0):25/4800bps,51/2400bps
const uint08 ecode Def_Shielding_I1     = 0x15;	//��������������ֵ15mA
const uint08 ecode Def_Shielding_I2 = 0x18;	//���������������ֵ18mA
const uint08 ecode Def_Shielding_Cos = 0x50;	//������������λ��Сֵ0.05
const uint08 ecode Defcurrovercount = 3;	//��բ�����жϳ�������ȥ��ʱ��
const uint16 ecode Defaultpdvoltage = 0x1320;	//Ĭ�ϵ����ѹֵ(60%Un)
const uint08 ecode Deflowvoltagecount = 1;	//Ĭ�ϵ����¼�ϵ�ȥ��ʱ��(���߸���60%ȥ��ʱ��)(����Ҫ������)
const uint16 ecode Uoffset = 0x0000;		//Ĭ�ϵ�ѹϵ��ƫ����
const uint08 ecode Def_Powerdetcnt2 = 11;	//����170v��8s���ڲŽ��е�Դ�쳣���,�ϵ�Ĭ�ϵ���170vԼ11s
const uint08 ecode Def_PowerDetCnt0 = 4;	//���Ϊ4���ϵ�������̵ּ������������̽��м̵�����⣬���Ϊ2��ʾ��Ҫȥ��2s�Ž��м��

//interrupt
const uint08 ecode Fulllcdbias = 0x09;		//ȫ����ʾƫ�õ�ѹ(3.5vҺ��)
const uint08 ecode Fulllcdbias2 = 0x0b;		//����ȫ����ʾƫ�õ�ѹ
const uint08 ecode Runlcdbias = 0x08;		//����Һ����ʾƫ�õ�ѹ(3.5vҺ��)
const uint08 ecode Runlcdbias2 = 0x0a;		//��������Һ����ʾƫ�õ�ѹ
const uint08 ecode Sleeplcdbias = 0x0f;		//ͣ��Һ����ʾƫ�õ�ѹ(3.5vҺ��)
const uint08 ecode Sleeplcdfreq = 0x15;		//A����64hz,B����32hz
const uint08 ecode Sleeplcdset = 0x81;		//ͣ��Һ����ʾ����
const uint08 ecode Sleepenmode = 0x47;		//Biasl_en���Ϊ2������,�����δ�,Ƭ�ڵ�������ģʽ(LCDģ��ض�ʱ����Ϊ00,������������Ĺ���)
const uint08 ecode Sleeplvdefcfg = 0x5b;	//��ѹ����·��Ъʹ��(0x47��Ƶ��)(�ͻ��޸Ĵ�ֵ���븴��΢ȷ��)32Tռ�ձ�1/512

//����
const uint08 ecode Defaultauthtimer = 0x30;	//Ĭ�������֤ʱЧ(30min)
const uint08 ecode Defaultstartup = 0x00;	//Ĭ������ʱ��ȫ0x00����ȫ0x99

//��������
const uint08 ecode Keyswitch_Valid = 0x00;	//0xaa-�޿��ۣ�0x55-�����д��㣻����-�����޴��㣨Զ�̱�������Ϊ0xaa��
const uint08 ecode METERTYPE = 0;		//00������ģʽ����ʾ; 01��Զ��ģʽ����ʾ
const uint08 ecode SELFCLOSE = 0x00;		//�Ա��繦�ܣ�00�رգ�55����
const uint08 ecode default_hearttime = 30;	//Ĭ������ʱ��30����
const uint08 ecode Sleepdispdays = 7*24;	//ͣ����ʾʱ��(��)
const uint08 ecode MAX_PSW_NUM = 32;		//�����Կ��Ĭ��32��
const uint08 ecode Terminal_delay = 120;	//�����ն������֤��Чʱ��2����
const uint08 ecode Defpassworderrcnt = 5;	//�������5�α���
const uint08 ecode Def_GSN = 0xf4;		//Ĭ��GSN��ֵ
const uint08 ecode Def_CWGSP = 0x3f;		//Ĭ��CWGSP��ֵ


const  signed short  ecode  g_PerKwh = 100;
const  signed short  ecode g_Per1Kwh = 1000;

const uint08 ecode Def_eventthreshold_table[6] = //�¼��ж��趨��Χ
{
	0x60,			//ͣ���¼���ѹ������ֵ---0x040014F1,1�ֽ�,����0x60��ʾ60%Un
	0x05,			//ͣ���¼�����������ֵ---0x040014F2,1�ֽ�,����0x05��ʾ5%Ib
	0x60,			//ͣ���¼��ж���ʱʱ�䶨ֵ----0x040014F3,1�ֽ�,����0x60��ʾ60S
	0x05,			//������¼��ж���ʱʱ�䶨ֵ----0x040014F4,1�ֽ�,����0x05��ʾ5S
	0x30,			//ʱ�ӵ��Ƿѹ�¼��������޶�ֵ---0x040014F5,1�ֽ�,����0x30��ʾ3.0v
	0x60,			//ʱ�ӵ��Ƿѹ�¼��ж���ʱʱ�䶨ֵ----0x040014F6,1�ֽ�,����0x60��ʾ60S
};

//lcd
const unsigned long ecode disp_table[disp_num]=
{
	//1~65����ǰ����12����ϵ���
	//66~130����ǰ����12���������
	//131~195����ǰ����12�·������
	0xffffffff,		//ȫ��200
	0x00000c00,		//�����õ���201
	0x01000c00,		//�����õ���202
	0xefefefef,		//�����ۼӺ�203(������������ʾ�����ͻ)
	0x00010102,		//��ѹ(A��)204
	0x00010202,		//����(A��)205
	0x00000302,		//����(��)206
	0x00010302,		//����(A��)207
	0x00000602,		//��������(��)208	
	0x00010602,		//��������(A��)209
	0x01008002,		//���ߵ���210
	0x09040004,		//����211
	0x010b0004,		//��1������212
	0x020b0004,		//��2������213
	0x030b0004,		//��3������214
	0x0e040004,		//���ŵ�215
	0x0e040004,		//���Ÿ�216
	0x02040004,		//��ŵ�217
	0x02040004,		//��Ÿ�218
	0x01040004,		//ͨѶ��ַ��219
	0x01040004,		//ͨѶ��ַ��220
	0x01010004,		//����221
	0x02010004,		//ʱ��222
	0x01003003,		//���һ�α������223
	0x01003003,		//���һ�α��ʱ��224
	0x01013003,		//���һ����������225
	0x01013003,		//���һ������ʱ��226
	0x00000d00,		//��ǰ���õ���227
	0x01000d00,		//�ϴ����õ���228
	0x00029000,		//ʣ����(DI0~DI3)229
	0x0b008002,		//��ǰ���ݵ��230
	0x21008002,		//��ǰ���ʵ��231
	0x20008002,		//��ǰ���232
};


//factory
const unsigned char ecode  Def_Limitmenory_table[]=
{
	0x00,0x00,0x10,0x00,		//͸֧�����ֵxxxxxx.xx
	0x99,0x99,0x99,0x99,		//�ڻ������ֵxxxxxx.xx
	0x00,0x00,0x02,0x00,		//��բ��������ֵxxxxxx.xx
};const unsigned char ecode Def_DisplayPara_table[8*2]=
{
//����
	2,	//�Զ�ѭ����ʾ����
	5, 	//ÿ����ʾʱ��    
	2, 	//��ʾ����С��λ��
	4, 	//��ʾ����С��λ��
	20,	//����ѭ����ʾ����
	0,	//Ԥ��
	0,	//�ѿ�ģʽ: 00����; 01Զ��
	5,	//�ϵ�ȫ��5s      
	
//Զ��
	7,	//�Զ�ѭ����ʾ����
	5,	//ÿ����ʾʱ��
	2,	//��ʾ����С��λ��
	4,	//��ʾ����С��λ��  
	23,	//����ѭ����ʾ����
	0,	//Ԥ��
	1,	//�ѿ�ģʽ: 00����; 01Զ��
	5,	//�ϵ�ȫ��5s    
};


const unsigned char ecode Def_featureword_table[]=
{
	0x08,		//ͨ�ſ�1 ������������	04000703	08H��2400bps��        
	0x05,		//�й���Ϸ�ʽ������	04000601	05H�������й�+�����й�
	0x0f,		//����ģʽ��
	0x83,		//��ʱ��������ģʽ�֣���ʱ���������й����ܡ������й����ܡ�������
	0x83,		//˲ʱ��������ģʽ�֣�˲ʱ���������й����ܡ������й����ܡ�������
	0x83,		//Լ����������ģʽ�֣�Լ�����������й����ܡ������й����ܡ�������
	0x03,		//���㶳������ģʽ�֣����㶳�������й��ܵ��ܡ������й��ܵ��ܣ� 
	0x83,		//�ն�������ģʽ��	���ն��������й����ܡ������й����ܡ������� 
	0x00,		//�������������1
	0xbb,		//�ϱ�ģʽ��1/���ڿ���ʼ������	
	0x04,		//�ϱ�ģʽ��2/����
	0x00,		//�ϱ�ģʽ��3/null
	0x00,		//�ϱ�ģʽ��4/null
	0x00,		//�ϱ�ģʽ��5/null
	0x00,		//�ϱ�ģʽ��6/null
	0x00,		//�ϱ�ģʽ��7/null
	0x00,		//�ϱ�ģʽ��8/null
	0x00,0x00,0x00,	//�̵�����բ���Ƶ�������ֵ(XXX.XXX)
	0x00,0x00,  	//��բ��ʱʱ��(bcd��)
	0x00,0x00,	//�������բ��ʱ(bcd��)
	0x30,		//������֤(bcd��)
	0x30,		//�ϱ���λ��ʱ(bcd��)
	0x00,		//���ü̵���0x55��ʾ�Զ���բ,���ü̵���0x55��ʾ�ֶ���բ
	0x00,		//��չģʽ��,0x0A����0xAAÿ��20:00��λ�ز�һ��(��Ҫ�����˹���ز�),0xA0����0xAA�����������
	0x00,		//Ԥ��
	0x08,		//ͨ�ſ�3 ������������	04000705	08H��2400bps�� (�ز�) 
};


const unsigned char ecode Def_weekloadpara_table[]=
{
	0x7F,				//������������7F����������������Ϣ��
	0x01,				//�����ղ��õ���ʱ�α��1����һ�׵�1��ʱ�α�
	0x13,0x01,0x01,0x00,0x00,	//���ɼ�¼��ʼʱ��(������ʱ��)
	0x00,0x01,			//��1~4�ฺ�ɼ�¼���ʱ��	
	0x00,0x05,
	0x00,0x02,
	0x00,0x10, 
#if(RELAYTYPE == 0x55)
	0x09,0x60,   			//������������1.2Imax,96.0A
#else
	0x07,0x20,   			//������������1.2Imax,72.0A
#endif
	0x60,				//������ʱʱ��,60s          
};

//ѭ����ʾĬ��֧��16���ʼ��,������������޸ı��ʵ��,ֻ��ͨѶ����ʵ��
const unsigned char ecode Def_Autodisp_table[16*2]=
{    
	// ����  
	229, //ʣ���� 
	232,//232, //��ǰ���
	1,	 // Ԥ��3   
	1,	 // Ԥ��4   
	1,	 // Ԥ��5   
	1,	 // Ԥ��6   
	1,	 // Ԥ��7   
	1,	 // Ԥ��8	  
	1,	 // Ԥ��9	  
	1,	 // Ԥ��10      
	1,	 // Ԥ��11      
	1,	 // Ԥ��12      
	1,	 // Ԥ��13      
	1,	 // Ԥ��14      
	1,	 // Ԥ��15      
	1,	 // Ԥ��16      

// Զ��
	 221, // ����      
	 1,	  // ����ܵ���
	 2,	  // ��ϼ����
	 3,	  // ��Ϸ����
	 4,	  // ���ƽ����
	 5,	  // ��Ϲȵ���
	 1,	 // Ԥ��7 	
	 1,	  // Ԥ��8     
	 1,	  // Ԥ��9     
	 1,	  // Ԥ��10    
	 1,	  // Ԥ��11    
	 1,	  // Ԥ��12    
	 1,	  // Ԥ��13    
	 1,	  // Ԥ��14    
	 1,	  // Ԥ��15    
	 1,	  // Ԥ��16   
};

//������ʾĬ��֧��32���ʼ��,������������޸ı��ʵ��,ֻ��ͨѶ����ʵ��
const unsigned char ecode Def_Keydisp_table[32*2]=
{
	//����	
	221, // ����+ʱ��			   
	247, //��1���õ���
	250, //��1���õ���
	249, //ʣ����+��������
	237, //��1�γ�ֵToken ��4λ
	238, //��1�γ�ֵToken ��16λ
	239, //��2�γ�ֵToken ��4λ
	240, //��2�γ�ֵToken ��16λ
	241, //��3�γ�ֵToken ��4λ
	242, //��3�γ�ֵToken ��16λ
	243, //��4�γ�ֵToken ��4λ
	244, //��4�γ�ֵToken ��16λ
	245, //��5�γ�ֵToken ��4λ
	246, //��5�γ�ֵToken ��16λ
	204, // ��ѹ+���� A
  205, // ��ѹ+���� B//	207, // ����+��������
  206, // ��ѹ+���� C//	1,	// Ԥ��17 		  
	207, //����+�������� A//1,	// Ԥ��18 		  
	208, //����+�������� B//1,	// Ԥ��19 		  
	209, //����+�������� C//1,	// Ԥ��20 		  
	1,	// Ԥ��21 		  
	1,	// Ԥ��22 		  
	1,	// Ԥ��23 		  
	1,	// Ԥ��24 		  
	1,	// Ԥ��25 		  
	1,	// Ԥ��26 		  
	1,	// Ԥ��27 		  
	1,	// Ԥ��28 	   
	1,	// Ԥ��29 	   
	1,	// Ԥ��30 	   
	1,	// Ԥ��31 	   
	1,	// Ԥ��32 	   
	//Զ��
	1,	 //����ܵ���  ��ǰ  
	2,	 //��ϼ����  ��ǰ 
	3,	 //��Ϸ����  ��ǰ 
	4,	 //���ƽ����  ��ǰ 
	5,	 //��Ϲȵ���  ��ǰ 
	6,	 //����ܵ���  ��1��
	7,	 //��ϼ����  ��1��
	8,	 //��Ϸ����  ��1��
	9,	 //���ƽ����  ��1��
	10,	 //��Ϲȵ���  ��1��
	11,	 //����ܵ���  ��2��
	12,	 //��ϼ����  ��2��
	13,	 //��Ϸ����  ��2��
	14,	 //���ƽ����  ��2��
	15,	 //��Ϲȵ���  ��2��
	219, // ͨѶ��ַ��8λ   
	220, // ͨѶ��ַ��4λ   
	221, // ����            
	222, // ʱ��            
	204, // ��ѹ
	205, // ����
	207, // ����A��
	209, // ��������A��       
	1,	 // Ԥ��24          
	1,	 // Ԥ��25          
	1,	 // Ԥ��26          
	1,	 // Ԥ��27          
	1,	 // Ԥ��28          
	1,	 // Ԥ��29          
	1,	 // Ԥ��30          
	1,	 // Ԥ��31          
	1,	 // Ԥ��32          
};
const unsigned char ecode Def_freezeparameter_table[]=
{
	0x10,0x01,0x01,0x00,0x00,	//���㶳����ʼʱ��	10��01��01��00ʱ00��
	0x60,				//���㶳��ʱ����	60����
	0x00,0x00,			//�ն���ʱ��		00ʱ00��
	0x99,0x99,0x12,0x00,		//��ʱ����ʱ��		12ʱ00��
};

const unsigned char Def_Demand_table[]=
{
	0x05,  //����������� 15                                           
	0x01,  //����ʱ��	1        
};

const unsigned char ecode Def_feik_table[]=
{
	0x02,  //��ʱ����	2                                               
	0x02,  //��ʱ�α���	2                                               
	0x08,  //��ʱ���� 	8                                               
	0x04,  //������	4                                               
	0x00,  //����������	0�������ã�����ʱ���ٿ�����1��14�չ������ղ�����
	0x00,  //г����������	0                                               
	0x00,  //������	0        
	0x11,	//��Կ�������õ�����                                
};

//������
const unsigned char ecode Def_histroyparameter_table[]=
{
	0x00,0x01,								//01��00��
	0x99,0x99,
	0x99,0x99,
};

//����
const uint08  Def_Password3_table[4]=
{
	0x03,0x00,0x00,0x00,
};

const uint08  Def_Password4_table[4]=
{
	0x04,0x11,0x11,0x11,
};

//ʱ����1
const unsigned char ecode Def_sqb_table1[]=
{
	0x01,0x28,0x12,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
};

//ʱ����2
const unsigned char ecode Def_sqb_table2[]=
{
	0x01,0x28,0x12,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
	0x02,0x22,0x06,	   //ʱ�α�š��ա���
};

//ʱ�α�1
const unsigned char ecode Def_sdb_table1[]=
{										//���ʺ�,��,ʱ,���14��
	0x01,0x00,0x00,        
	0x02,0x00,0x08,
	0x03,0x00,0x16,
	0x03,0x00,0x16,
	0x03,0x00,0x16,
	0x03,0x00,0x16,
	0x03,0x00,0x16,
	0x03,0x00,0x16,
	0x03,0x00,0x16,
	0x03,0x00,0x16,
	0x03,0x00,0x16,
	0x03,0x00,0x16,
	0x03,0x00,0x16,
	0x03,0x00,0x16,
};

//ʱ�α�2
const unsigned char ecode Def_sdb_table2[]=
{										//���ʺ�,��,ʱ,���14��
	0x01,0x00,0x00,        
	0x02,0x00,0x06,
	0x03,0x00,0x12,
	0x04,0x00,0x18,
	0x04,0x00,0x18,
	0x04,0x00,0x18,
	0x04,0x00,0x18,
	0x04,0x00,0x18,
	0x04,0x00,0x18,
	0x04,0x00,0x18,
	0x04,0x00,0x18,
	0x04,0x00,0x18,
	0x04,0x00,0x18,
	0x04,0x00,0x18,
};

//��ǰ����ʱ����
const unsigned char ecode Def_jtshiqu_table[]=
{										//���ݱ��ź�+��+��
	0x01,0x01,0x01,        
	0x01,0x01,0x01,
	0x01,0x01,0x01,
	0x01,0x01,0x01,
};

//���ý���ʱ����
const unsigned char ecode Def_jtshiqu_table2[]=
{										//���ݱ��ź�+��+��
	0x01,0x01,0x01,        
	0x01,0x01,0x01,
	0x01,0x01,0x01,
	0x01,0x01,0x01,
};
const FACTOR ecode Factor =
{
    #if MeterPara_Mode == 0           
     ///////����������Ϊ1.5(10)A/5mA�����õ���Ϊ2��4.7ŷķ//////
    //��ѹϵ��
    0.00000677424168,//(float)MeterUn/(float)RN8302Urms/10,
    //����ϵ��
    0.000000200000002,//(float)MeterIb/(float)RN8302Irms/10,
    //����ϵ��
    0.000113652924,//8388608/(((float)RN8302Urms/(float)MeterUn)*((float)RN8302Irms/(float)MeterIb))/10,
    //0.0001kwh��Ҫ������
    64,//MeterCP2*0.0001,һ���������0.0001KWH
    //��������ֵ
    11000,//(signed long)((float)MeterUn*MeterImax*3*(MeterCP2/MeterCP)/6000),
    //���ڡ��й�������������ֵ�Ĵ���
    234,//(unsigned short)(RN8302Irms*0.0006/16),
    //������ʾ����
    500000,//MeterImax*1000*5,//5Imax���ϲ���ʾ xxx.xxxx
    //��ѹ��ʾ����ֵ
    660000,//MeterUn*100*3,//3Un���ϲ���ʾ xxx.xxxУ���ѹ��3λС���������ٳ���100����3λС����ʽ
    //��ѹ��ʾ����ֵ
    440,//MeterUn/5,//0.02Un���µ�ѹ����ʾ
    //����
    6400//MeterCP 
#elif MeterPara_Mode == 1    
 ///////����������Ϊ1.5(6)A/5mA�����õ���Ϊ2��4.7ŷķ//////
    //��ѹϵ��
    0.00000677424168,//(float)MeterUn/(float)RN8302Urms/10,
    //����ϵ��
    0.000000200000002,//(float)MeterIb/(float)RN8302Irms/10,
    //����ϵ��
    0.000113652924,//8388608/(((float)RN8302Urms/(float)MeterUn)*((float)RN8302Irms/(float)MeterIb))/10,
    //0.0001kwh��Ҫ������
    64,//MeterCP2*0.0001,һ���������0.0001KWH
    //��������ֵ
    6600,//(signed long)((float)MeterUn*MeterImax*3*(MeterCP2/MeterCP)/6000),
    //���ڡ��й�������������ֵ�Ĵ���
    281,//(unsigned short)(RN8302Irms*0.0006/16),
    //������ʾ����
    300000,//MeterImax*1000*5,//5Imax���ϲ���ʾ xxx.xxxx
    //��ѹ��ʾ����ֵ
    660000,//MeterUn*100*3,//3Un���ϲ���ʾ xxx.xxxУ���ѹ��3λС���������ٳ���100����3λС����ʽ
    //��ѹ��ʾ����ֵ
    440,//MeterUn/5,//0.02Un���µ�ѹ����ʾ
    //����
    6400//MeterCP              
#elif MeterPara_Mode == 3
    //��ѹϵ��
    0.00000667642007,//(float)MeterUn/(float)RN8302Urms/10,
    //����ϵ��
    0.00000120481922,//(float)MeterIb/(float)RN8302Irms/10,
    //����ϵ��
    0.000674769573,//8388608/(((float)RN8302Urms/(float)MeterUn)*((float)RN8302Irms/(float)MeterIb))/10,
    //0.0001kwh��Ҫ������
    4,//MeterCP2*0.0001,һ���������0.0001KWH
    //��������ֵ
    66000,//(signed long)((float)MeterUn*MeterImax*3*(MeterCP2/MeterCP)/6000),
    //���ڡ��й�������������ֵ�Ĵ���
    518,//(unsigned short)(RN8302Irms*0.002/16),
    //������ʾ����
    3000000,//MeterImax*1000*5,//5Imax���ϲ���ʾ xxx.xxxx
    //��ѹ��ʾ����ֵ
    660000,//MeterUn*100*3,//3Un���ϲ���ʾ xxx.xxxУ���ѹ��3λС���������ٳ���100����3λС����ʽ
    //��ѹ��ʾ����ֵ
    440,//MeterUn/5,//0.02Un���µ�ѹ����ʾ
    //����
    400//MeterCP  
#elif MeterPara_Mode == 4
    //��ѹϵ��
    0.00000673145768,//(float)MeterUn/(float)RN8302Urms/10,
    //����ϵ��
    0.00000166666666,//(float)MeterIb/(float)RN8302Irms/10,
    //����ϵ��
    0.000941125967,//8388608/(((float)RN8302Urms/(float)MeterUn)*((float)RN8302Irms/(float)MeterIb))/10,
    //0.0001kwh��Ҫ������
    8,//MeterCP2*0.0001,һ���������0.0001KWH
    //��������ֵ
    66000,//(signed long)((float)MeterUn*MeterImax*3*(MeterCP2/MeterCP)/6000),
    //���ڡ��й�������������ֵ�Ĵ���
    375,//(unsigned short)(RN8302Irms*0.002/16),
    //������ʾ����
    3000000,//MeterImax*1000*5,//5Imax���ϲ���ʾ xxx.xxxx
    //��ѹ��ʾ����ֵ
    660000,//MeterUn*100*3,//3Un���ϲ���ʾ xxx.xxxУ���ѹ��3λС���������ٳ���100����3λС����ʽ
    //��ѹ��ʾ����ֵ
    440,//MeterUn/5,//0.02Un���µ�ѹ����ʾ
    //����
    800//MeterCP          
#elif MeterPara_Mode == 5    
    //��ѹϵ��
    0.00000668746088,//(float)MeterUn/(float)RN8302Urms/10,
    //����ϵ��
    0.00000166666666,//(float)MeterIb/(float)RN8302Irms/10,
    //����ϵ��
    0.000934974872,//8388608/(((float)RN8302Urms/(float)MeterUn)*((float)RN8302Irms/(float)MeterIb))/10,
    //0.0001kwh��Ҫ������
    3,//MeterCP2*0.0001,һ���������0.0001KWH
    //��������ֵ
    110000,//(signed long)((float)MeterUn*MeterImax*3*(MeterCP2/MeterCP)/6000),
    //���ڡ��й�������������ֵ�Ĵ���
    750,//(unsigned short)(RN8302Irms*0.002/16),
    //������ʾ����
    5000000,//MeterImax*1000*5,//5Imax���ϲ���ʾ xxx.xxxx
    //��ѹ��ʾ����ֵ
    660000,//MeterUn*100*3,//3Un���ϲ���ʾ xxx.xxxУ���ѹ��3λС���������ٳ���100����3λС����ʽ
    //��ѹ��ʾ����ֵ
    440,//MeterUn/5,//0.02Un���µ�ѹ����ʾ
    //����
    300//MeterCP 
#elif MeterPara_Mode == 11    
    //��ѹϵ��
    0.0000017869936,//(float)MeterUn/(float)RN8302Urms/10,
    //����ϵ��
    0.000000142857146,//(float)MeterIb/(float)RN8302Irms/10,
    //����ϵ��
    0.0000214148422,//8388608/(((float)RN8302Urms/(float)MeterUn)*((float)RN8302Irms/(float)MeterIb))/10,
    //0.00001kwh��Ҫ������
    2,//MeterCP2*0.00001,һ���������0.00001KWH
    //��������ֵ
    300,//(signed long)((float)MeterUn*MeterImax*3*(MeterCP2/MeterCP)/6000),
    //���ڡ��й�������������ֵ�Ĵ���
    392,//(unsigned short)(RN8302Irms*0.0006/16),
    //������ʾ����
    300000,//MeterImax*1000*5,//5Imax���ϲ���ʾ xxx.xxxx
    //��ѹ��ʾ����ֵ
    300000,//MeterUn*100*3,//3Un���ϲ���ʾ xxx.xxxУ���ѹ��3λС���������ٳ���100����3λС����ʽ
    //��ѹ��ʾ����ֵ
    200,//MeterUn/5,//0.02Un���µ�ѹ����ʾ
    //����
    20000//MeterCP
#elif MeterPara_Mode == 13    
    //1.5(6)A 5mA 12.4ŷ
    //��ѹϵ��
    0.00000181968266,//(float)MeterUn/(float)RN8302Urms/10,
    //����ϵ��
    0.000000142857146,//(float)MeterIb/(float)RN8302Irms/10,
    //����ϵ��
    0.0000218065779,//8388608/(((float)RN8302Urms/(float)MeterUn)*((float)RN8302Irms/(float)MeterIb))/10,
    //0.00001kwh��Ҫ������
    2,//MeterCP2*0.00001,һ���������0.00001KWH
    //��������ֵ
    173,//(signed long)((float)MeterUn*MeterImax*3*(MeterCP2/MeterCP)/6000),
    //���ڡ��й�������������ֵ�Ĵ���
    393,//(unsigned short)(RN8302Irms*0.0006/16),
    //������ʾ����
    300000,//MeterImax*1000*5,//5Imax���ϲ���ʾ xxx.xxxx
    //��ѹ��ʾ����ֵ
    173100,//MeterUn*100*3,//3Un���ϲ���ʾ xxx.xxxУ���ѹ��3λС���������ٳ���100����3λС����ʽ
    //��ѹ��ʾ����ֵ
    115,//MeterUn/5,//0.02Un���µ�ѹ����ʾ
    //����
    20000//MeterCP         
#endif  
};
const METERSPECIAL ecode Meterspecial =
{
    0x5A, //0x5A-????7???
    0xAA, //0x00-????,0x5A-??,0xAA-?? 
    0x00 //0x5A-??????  
};


const METERPARAMETER ecode MeterParameter =
{
    #if MeterPara_Mode == 0 //220V 1(10)A 6400 imp/kWh        
    1,//1-����ʽ��3-ֱ��ʽ
    0x34,//0x34-�������ߣ�0x33��������
    0x00,//0-�޵�Ƶ����1-�е�Ƶ����
    183.1806828,//��ѹ����1��1200
    47,//mv��Ҫ��������ϵ�� 5ma 9.4      
    32475957.31,//RN8302��ѹ��׼ֵ
    7500000,//RN8302������׼ֵ
    29504299,//RN8302���ʱ�׼ֵ1.0 Ib
    0x003B,//HFConst1(������)
    0x170C,//HFConst2(������)
    2200,//��ѹ
    15, //����IB
    100,//����IMAX
    6400, //����  
    640000//��������
#elif MeterPara_Mode == 1     //220V 1.5(6)A 6400 imp/kWh

//    1,//1-����ʽ��3-ֱ��ʽ
//    0x34,//0x34-�������ߣ�0x33��������
//    0x01,//0-�޵�Ƶ����1-�е�Ƶ����
//    183.1806828,//��ѹ����1��1200
//    20,//mv��Ҫ��������ϵ�� 1ma 20      
//    32525997.31,//RN8302��ѹ��׼ֵ
//    3300000,//RN8302������׼ֵ
//    12981892,//RN8302���ʱ�׼ֵ
//    0x001A,//HFConst1(������)
//    0x0A28,//HFConst2(������)
//    2200,//��ѹ
//    15, //����IB
//    60,//����IMAX
//    6400, //����  
//    640000//��������     

    1,//1-����ʽ��3-ֱ��ʽ
    0x34,//0x34-�������ߣ�0x33��������
    0x00,//0-�޵�Ƶ����1-�е�Ƶ����
    183.1806828,//��ѹ����1��1200
    47,//mv��Ҫ��������ϵ�� 5ma 9.4      
    32475957.31,//RN8302��ѹ��׼ֵ
    7500000,//RN8302������׼ֵ
    29504299,//RN8302���ʱ�׼ֵ1.0 Ib
    0x003B,//HFConst1(������)
    0x170C,//HFConst2(������)
    2200,//��ѹ
    15, //����IB
    60,//����IMAX
    6400, //����  
    640000//��������          
#elif MeterPara_Mode == 3 //220V 5(80)A 400 imp/kWh
    3,//1-����ʽ��3-ֱ��ʽ
    0x34,//0x34-�������ߣ�0x33��������
    0x00,//0-�޵�Ƶ����1-�е�Ƶ����
    183.1806828,//��ѹ����1��1200
    18.8,//��������mv��Ҫ��������ϵ�� 2ma 9.4
    32969059.77,//43863183.87,//RN8302��ѹ��׼ֵ
    3000000,//RN8302������׼ֵ
    11801720,//15735626,//RN8302���ʱ�׼ֵ
    0x0073,//0x0099,//HFConst1(������)
    0x2CEC,//0x3BC4,//HFConst2(������)
    2200,//��ѹ
    50, //����IB
    800,//����IMAX
    400,//����
    40000//��������    
#elif MeterPara_Mode == 4 //220V 5(60)A 800 imp/kWh
    3,//1-����ʽ��3-ֱ��ʽ
    0x34,//0x34-�������ߣ�0x33��������
    0x00,//0-�޵�Ƶ����1-�е�Ƶ����
    183.1806828,//219.7802198,//��ѹ����mv
    18.8,//��������mv��Ҫ��������ϵ�� 2ma 9.4 
    32682372.3,//43863183.87,//RN8302��ѹ��׼ֵ
    3000000,//RN8302������׼ֵ
    11801720,//15735626,//RN8302���ʱ�׼ֵ
    0x0039,//0x0099,//HFConst1(������)
    0x1644,//0x3BC4,//HFConst2(������)
    2200,//��ѹ
    50, //����IB
    600,//����IMAX
    800,//����
    80000//��������            
#elif MeterPara_Mode == 5 //220V 10(100)A 300 imp/kWh
    3,//1-����ʽ��3-ֱ��ʽ
    0x34,//0x34-�������ߣ�0x33��������
    0x00,//0-�޵�Ƶ����1-�е�Ƶ����
    183.1806828,//��ѹ����1��1200
    37.6,//��������mv��Ҫ��������ϵ�� 2ma 9.4 
    32897387.9,//RN8302��ѹ��׼ֵ
    6000000,//RN8302������׼ֵ
    23603439,//RN8302���ʱ�׼ֵ
    0x0099,//HFConst1(������)
    0x3BC4,//HFConst2(������)
    2200,//��ѹ
    100, //����IB
    1000,//����IMAX
    300,//����
    30000//��������      
#elif MeterPara_Mode == 11 //100V 1.5(6)A 20000 imp/kWh     
    //��ѹ1/600K������5mA/9.4ŷ        
    1,//1-����ʽ��3-ֱ��ʽ
    0x33,//0x34-�������ߣ�0x33��������
    0x00,//0-�޵�Ƶ����1-�е�Ƶ����
    166.3893511,//��ѹ����mv 1/600K
    47,//��������mv��Ҫ��������ϵ�� 5ma 9.4 
    27912935.19,//RN8302��ѹ��׼ֵ
    7500000,//RN8302������׼ֵ
    12479201,//RN8302���ʱ�׼ֵ
    0x0165,//HFConst1(������)
    0x0DF2,//HFConst2(������)
    1000,//��ѹ
    15, //����IB
    60,//����IMAX
    20000,//����
    200000//��������        
#elif MeterPara_Mode == 13 //57.7V 1.5(6)A 20000 imp/kWh
    1,//1-����ʽ��3-ֱ��ʽ
    0x34,//0x34-�������ߣ�0x33��������
    0x00,//0-�޵�Ƶ����1-�е�Ƶ����
    191.6943522,//��ѹ����mv 2/600
    47,//��������mv��Ҫ��������ϵ�� 5ma 9.4 
    31715271.19,//RN8302��ѹ��׼ֵ
    7500000,//RN8302������׼ֵ
    28373301,//RN8302���ʱ�׼ֵ
    0x02BF,//HFConst1(������)
    0x1B76,//HFConst2(������)
    577,//��ѹ
    15, //����IB
    60,//����IMAX
    20000,//����
    200000//��������       
#endif        
};



void Factory_Init( void )
{
	unsigned char i;
	unsigned char Buf[72];
	
	//����Чָ��ҳ
	ClrRecTimes( 0, 0xff );//�嶳����¼���Чָ��
	
	//����ͣ��, ��λ����, �ָ���ʱ����ʼʱ��,�ָ�����
	Clear_Wdt();
	memset( Buf, 0x00, 64 );
	for( i=0; i<3; i++ )
	{
		I2C_Write_Eeprom44( ee_xtlfstop_time_page+i, Buf, 18 );
	}
	Wr_Ext_Number = 0;//����չ��������дee����
	Clr_Otherdata(4); //��ع���ʱ��
	relay_commu_close_flag = 0;//Ĭ�ϱ�����״̬
	Store_Meterstatus();
	
	//��ʼ�����ܱ�λ����Ϣ
	I2C_Write_Eeprom( ee_meterpos_page, ee_meterpos_inpage, Buf, ee_meterpos_lenth );	
		
	//��1~60�����������ڼ���ʱ�α��: 00000000
	for( i=0; i<4; i++ )
	{
		I2C_Write_Eeprom( ee_holiday_page+i, 0x00, Buf, ee_holiday_lenth );
	}
	
	//�����ʼ��
	WriteE2WithBackup( ee_password_page, ee_password_inpage, (uint08*)Def_Password3_table, ee_password_lenth );
	WriteE2WithBackup( ee_password_page, ee_password4_inpage, (uint08*)Def_Password4_table, ee_password_lenth );
		
	//���,��ŵ�(˫����)
	ReadE2WithBackup( ee_esampara_page, ee_esampara_inpage, Buf, ee_esampara_lenth );
	if( IsMultiBcd( Buf, 14 ) )//�Ƿ�����
	{
		memset( Buf, 0x00, 14 );	//�������1��ֵ(4)���������2��ֵ(4)+���(6)
		Buf[10] = 0x01;//�������1
		Buf[13] = 0x01;//��ѹ���1
	}
//	memset( Buf+14, 0x00, 6 );	//���(6),��Ų���ʼ��	
//	Buf[19] = 0x01;//���0x000000000001
	memset( Buf+20, 0x00, 9 );	//�ͻ����(6)+�翨����(1)+�����֤ʱЧ(2)
	Buf[27] = 0x00;//�����֤��Чʱ��
	Buf[28] = Defaultauthtimer;
	WriteE2WithBackup( ee_esampara_page, ee_esampara_inpage, Buf, ee_esampara_lenth );
	memcpy( Esam_Para.Alarm_Limit1.B08, Buf, ee_esampara_lenth+2 );
	
	//������(˫����)
	WriteE2WithBackup( ee_limitmenory_page, ee_limitmenory_inpage, (uint08*)Def_Limitmenory_table, ee_limitmenory_lenth );
	memcpy( Money_Para.Tick.B08, Def_Limitmenory_table, ee_limitmenory_lenth );//��Ƿ�����ֵ/�ڻ������ֵ/��բ������ָ�Ĭ��
	
	//��ʾ����(˫����)
	WriteE2WithBackup( ee_displaypara_page, ee_displaypara_inpage, (uint08*)Def_DisplayPara_table+8*METERTYPE, ee_displaypara_lenth );
	memcpy( &Display_Para.S_Number, Def_DisplayPara_table+8*METERTYPE, ee_displaypara_lenth );
	RdMeterMode();
	
	//�л�ʱ��(˫����)
	memset( Starttime.Shiqu, Defaultstartup, ee_starttimer_lenth );
	WriteE2WithBackup( ee_starttimer_page, ee_starttimer_inpage, Starttime.Shiqu, ee_starttimer_lenth );
	memset( Buf, 0xff, ee_changecontrol_lenth );	//ʱ��ʱ�ξ�ʹ�õ�һ��
	I2C_Write_Eeprom44( ee_changecontrol_page, Buf, ee_changecontrol_lenth );
	run_timezone_flag = 1;
	
	//����ģʽ��(˫����)
	WriteE2WithBackup( ee_featureword_page, ee_featureword_inpage, (uint08*)Def_featureword_table, ee_featureword_lenth );
	memcpy( Mode.Bps, Def_featureword_table, ee_featureword_lenth );
	mode_veri_flag = 1;
	reg_veri_flag = 1;	
	
	
	//�¼��ж��趨��ΧУ��(��������)
	WriteE2WithBackup( ee_eventthreshold_page, ee_eventthreshold_inpage, (uint08*)Def_eventthreshold_table, ee_eventthreshold_lenth );
	memcpy( Threshold, Def_eventthreshold_table, ee_eventthreshold_lenth );
	
	memset( Buf, 0xff, 72 );			
	I2C_Write_Eeprom( ee_TID_page, ee_TID_inpage, Buf, ee_TID_lenth );
	
	memset( Buf, 0x00, 72 );
  I2C_Write_Eeprom( ee_TIDCNT_page, ee_TIDCNT_inpage, Buf, ee_TIDCNT_lenth );	
	I2C_Write_Eeprom( ee_TOKEN_page, ee_TOKEN_inpage, Buf, ee_TOKEN_lenth );
	I2C_Write_Eeprom( ee_Pre7DayMon_page, ee_TOKEN_inpage, Buf, ee_Pre7DayMon_lenth );
	I2C_Write_Eeprom( ee_Pre12monthMon_page, ee_Pre12monthMon_inpage, Buf, ee_Pre12monthMon_lenth );
	I2C_Write_Eeprom( ee_DayUserINFO_page, ee_UserINFO_inpage, Buf, 16 );
	
	
//	memset( Buf, 0xFF, 72 );
//	I2C_Read_Eeprom( ee_Pre12monthMon_page, ee_Pre12monthMon_inpage, Buf, ee_Pre12monthMon_lenth );
//	
	memset( Buf, 0x01, 8 );
	WriteE2WithBackup( ee_keysts_page, ee_keysts_inpage, Buf, ee_keysts_lenth );
	ReadE2WithBackup( ee_keysts_page, ee_keysts_inpage, &gs_APDU_key_var.secret_key[0], ee_keysts_lenth );
	
	//�����պ͸��ɲ���(˫����)
	memset( Buf, 0x00, 72 );
	
	WriteE2WithBackup( ee_weekholiday_page, ee_weekholiday_inpage, (uint08*)Def_weekloadpara_table, ee_weekholiday_lenth );
	memcpy( &Week_Holiday.Status, Def_weekloadpara_table, ee_weekholiday_lenth );
//	ReadE2WithBackup( ee_weekholiday_page, ee_weekholiday_inpage, &Week_Holiday.Status, ee_weekholiday_lenth );
//	ReadE2WithBackup( ee_weekholiday_page, ee_weekholiday_inpage, Buf, ee_weekholiday_lenth );
	//�ѿ���
	WriteE2WithBackup( ee_feik_number_page, ee_feiknumber_inpage, (uint08*)Def_feik_table, ee_feiknumber_lenth );
	memcpy( Feik.Num, Def_feik_table, ee_feiknumber_lenth );
  
	memset( Money.Remain,0, ee_remainmoney_lenth);
	Money.Remain[3] = 0x05;	
	WriteE2WithBackup( ee_remainmoney_page, ee_remainmoney_inpage,  Money.Remain, ee_remainmoney_lenth );
	I2C_Read_Eeprom(ee_remainmoney_page, ee_remainmoney_inpage, Money.Remain, ee_remainmoney_lenth);
  
	memset( Buf, 0x00, 5 );
  I2C_Write_Eeprom44( ee_meterstatus1_page, Buf, ee_meterstatus1_lenth);
	//������
	WriteE2WithBackup( ee_histroyparameter_page, ee_histroyparameter_inpage, (uint08*)Def_histroyparameter_table, ee_histroyparameter_lenth );
	
	//�������
	I2C_Write_Eeprom44( ee_freezeparameter_page, (uint08*)Def_freezeparameter_table, ee_freezeparameter_lenth );
  
	WriteE2WithBackup( ee_Demand_Period_page, ee_Demand_Period_inpage,(uint08*)Def_Demand_table, ee_Demand_Period_len );//�������ڣ�����ʱ��
	
	memset( Buf, 0x00, 42 );
	I2C_Write_Eeprom( ee_p_demand_page, ee_p_demand_inpage, Buf, ee_pdemand_lenth );
	I2C_Write_Eeprom( ee_n_demand_page, ee_n_demand_inpage, Buf, ee_ndemand_lenth );
	
	Clear_Wdt();
		
	//��һ�ף��ڶ���ʱ��������
	I2C_Write_Eeprom( ee_shiqu1_page, 0x00, (uint08*)Def_sqb_table1, ee_shiqu_lenth );
	I2C_Write_Eeprom( ee_shiqu2_page, 0x00, (uint08*)Def_sqb_table2, ee_shiqu_lenth );
	
	//��һ�ף��ڶ��� ����8����ʱ�α�
	I2C_Write_Eeprom( ee_shiduantable1_1_page, 0x00, (uint08*)Def_sdb_table1, ee_shiduantable_lenth );
	for( i=1; i<8; i++ )
	{
		I2C_Write_Eeprom( ee_shiduantable1_1_page+i, 0x00, (uint08*)Def_sdb_table2, ee_shiduantable_lenth );
	}
	
	I2C_Write_Eeprom( ee_shiduantable2_1_page, 0x00, (uint08*)Def_sdb_table2, ee_shiduantable_lenth );
	for( i=1; i<8; i++ )
	{
		I2C_Write_Eeprom( ee_shiduantable2_1_page+i, 0x00, (uint08*)Def_sdb_table1, ee_shiduantable_lenth );
	}
	
	Tariff.Crc16.B16 += 1;
	
	//�Զ�ѭ����ʾ��Ŀ�б�
	memset( Buf, 0x01, ee_displayitem_lenth );
	I2C_Write_Eeprom( ee_loop_displayitem_page+1, 0x00, Buf, ee_displayitem_lenth );
	I2C_Write_Eeprom( ee_key_displayitem_page+1, 0x00, Buf, ee_displayitem_lenth );
	
	memcpy( Buf, Def_Autodisp_table+16*METERTYPE, sizeof(Def_Autodisp_table)/2 );
	I2C_Write_Eeprom( ee_loop_displayitem_page, 0x00, Buf, ee_displayitem_lenth );
	memset( Buf, 0x01, ee_displayitem_lenth );
	memcpy( Buf, Def_Keydisp_table+32*METERTYPE, sizeof(Def_Keydisp_table)/2 );
	I2C_Write_Eeprom( ee_key_displayitem_page, 0x00, Buf, ee_displayitem_lenth );
	
	for( i=0; i<4; i++ ) Item[i].Crc16.B16 += 1;
	dispitem_veri_flag = 1;
	
	//��ǰ/�����׽���ʱ����
	I2C_Write_Eeprom( ee_jtshiqu1_page, ee_jtshiqu_inpage, (uint08*)Def_jtshiqu_table, ee_jtshiqu_len );
	I2C_Write_Eeprom( ee_jtshiqu2_page, ee_jtshiqu_inpage, (uint08*)Def_jtshiqu_table2, ee_jtshiqu_len );
	
	//��ʼ��ee����ʵ�ۺͽ��ݲ���
	Clear_Wdt();
	memset( Buf, 0x00, 72 );
	if( Crc_Check( Price.Tariff[0].B08, ee_tariffprice_lenth+2 ) ) 
	{
		I2C_Write_Eeprom( ee_tariffprice1_page, 0x00, Buf, ee_tariffprice_lenth );//���ʵ��
		I2C_Write_Eeprom( ee_tariffprice2_page, 0x00, Buf, ee_tariffprice_lenth );//���ʵ��
	}
	if( Crc_Check( Price.Ladder_Dl[0].B08, ee_ladder_lenth+2 ) ) 
	{
		for( i=0; i<8; i++ ) I2C_Write_Eeprom( ee_ladder1_page+i, 0, Buf, ee_ladder_lenth );//���ݲ���
	}
	
	//Զ��ģʽ���嵱ǰ��۲���
	if( !BDMODE )
	{
		memset(Price.Current_L.B08, 0x00, 16);
	}
	
	//��ʼ���¶�ϵ��
	Temperpara_Verify();	//У�����ݺϷ���
	WriteE2WithBackup( ee_temperature_page, ee_temperature_inpage, Tpara.Offset, ee_temperature_lenth); 
	
	//��ʼ������ʱ��
	Buf[0] = default_hearttime;
	WriteE2WithBackup( ee_hearttime_page, ee_hearttime_inpage, Buf, ee_hearttime_lenth );
	(*( void (*)( ) )0x00000321) ();	//��λ
	//while(1)
	//{	;}
										
}

