//variable.h


#include "define4.h"

const uint16 ecode BPS_SPBRGL1[4] = { 1200, 2400, 4800, 9600 };		//T2��ʼ���ֵ�λ@8MHz

//---------------------- bit variable --------------------
B08_Bit			bdata	Meter_Status_S;   		//
B08_Bit			bdata	Meter_Status1_S;		//(�ı���Ҫ����)
B08_Bit			bdata	Meter_Status2_S;		//(�ı���Ҫ����)
B08_Bit			bdata	System_Status_S;		//(ͣ�粻��)
B08_Bit			bdata	System_Status1_S;				
B08_Bit			bdata	System_Status2_S;
B08_Bit			bdata	System_Status3_S;
B08_Bit			bdata	System_Status4_S;
B08_Bit			bdata	System_Status5_S;
B08_Bit			bdata	Ic_Type_Reg_S;

B08_Bit bdata			        System_Status6;		      //����������ر�־
B08_Bit			bdata	AT7051_Flag_S;

//;=============================< IDATA �洢���� >================================
uint08			bdata	DisKeySamp;	//��ʾ����ȥ���Ĵ���
uint08			bdata	DisKeySamp1;	//��ʾ����ȥ���Ĵ���
uint08			bdata	CardKeySamp;	//�忨ȥ���Ĵ���
uint08			bdata	Uart1KeySamp;	//485���Լ��ȥ���Ĵ���
uint08			idata	VectorTimer;	//���붨ʱ��
struct 	SYS_CLOCK_FRM	idata	Sclock;		//ʱ������
uint08			idata	UpdateMoneytimer;//����ESAMǮ����ʱ��
uint08			idata	Key_Status;//��Կ״̬,00��ʾȫ����Կ,01��ʾȫ��˽Կ,FF���й���˽

//;=============================< XDATA �洢���� >================================
struct  TIMER_FRM	near	Progkey	;	//240min���(ͣ������)
struct  TIMER_FRM	near	Cover		;	//���ǲ���(ͣ�籣��) 
uint08			near 	Mac_Err_Times	;	//MACУ��ʧ�ܴ���
uint08			near 	System_St_Bk	;	//ϵͳ״̬�ֱ���(ͣ������)
uint08			near 	Report_Data[5]	;	//��Ч�ϱ�״̬��
uint08			near 	Report_Data2[5]	;	//��Ч�ϱ�״̬�ֱ���
uint08			near 	Report_Num[40]	;	//�¼��ϱ���������
uint16			near 	Reset_Rep_Timer;	//��λ�ϱ�״̬�ֶ�ʱ��
uint08			near 	RReport_Status;	//֮ǰ�Ѷ��ϱ�״̬����λ�ϱ�״̬
uint08			near	Password_Err[2];	//����������
struct	LOADREC_FRM	near	Load	;	//����ָ�빲18�ֽ�
uint08			near	Wr_Ext_Number;	//�ڲ���չ����дee�������
uint16			near	Self_Timer24;	//�Ա���24Сʱ��ʱ��
struct	MONEY_FRM	near	Money	;	//ʣ����(6)+͸֧���(6)+�������(4)+Crc(2)

struct USERINFOR_FRM  Money_UserINFO;	//��һ��ʹ�ý�� BCD 4 ��  //ʣ�����������BCD	
struct	FWDL_FRM	near	Fwdl;	//�����й��ܵ���(4)�������(4)�������(4)��ƽ����(4)���ȵ���(4)�����������й��õ���(4)��Crc(2)
struct	BKDL_FRM	near	Bkdl;	//�����й��ܵ���(4)�������(4)�������(4)��ƽ����(4)���ȵ���(4)�����·����й��õ���(4)��Crc(2)
uint08			near	RAMREG0	;	//��������Ĵ���
uint08			near	RAMREG1	;	//
uint08			near	RAMREG2		;	//
uint08			near	RAMREG3		;	//
uint08			near	RAMREG4	;	//
uint08			near	RAMREG5		;	//
uint08			near	RAMREG6		;	//
uint08			near	RAMREG7		;	//
uint08			near	RAMREG8		;	//
uint08			near	RAMREG9		;	//
uint08			near	RAMREG10	;	//
uint08			near	RAMREG11	;	//
uint08			near	RAMREG12	;	//
uint32			RTC_Status;
uint32			KEY_Status;

APDU_ANALYZE_VAR      gs_APDU_analyze_var;
APDU_KEY_EXRAM_VAR    gs_APDU_key_var;
STS_KAY_CHANHE_DATA   sts_kay_change_data;
APDU_KEY_EXRAM_VAR   sts_kay_process_data;

uint08			near 	PLC_RST1_Timer;
uint08			near 	Outputtype;	//0-������,1-����,2-ʱ��Ͷ���ź�,0x50-FOUT���
struct  TIMER_FRM	near	Successled,Shiduanctrl,Relayctrl,Powertimer;
struct  TIMER_FRM	near	Adcctrl;	//adcת��ʱ������ʱ��
union   B16_B08		near	Lcdled;		//���ⶨʱ��
union   B32_B08			Current_Demand;//��ǰ����(���ֽ���ǰ)
union   B32_B08		near	Identitytimer;	//��ȫ��֤��ʱ��
uint16			near 	IR_EnTimer;	//������֤��ʱ��
union   B32_B08		near	Opendelaytimer;//��բ��ʱ��ʱ��
union	B32_B08		near	CurrentTimer;	//����բ������ʱ��
uint08			near	Relayerror_Count,Relayerror_Count1,Relayerror_Count2;//�̵������������
uint08			near	Relay_Space_Timer;//�ϵ���ʱ���Ƽ̵�����ʱ��
uint08			near	Keyclose_timer;//�����պϼ̵�����ʱ��
uint08			near	Display_Space_Timer;//���������ʾ�����ʱ��
uint16		near	Stop_Disptimer;//ͣ����ʾʱ�䶨ʱ
signed char		near	Temperature_Hex;//�����¶�(hex��)
union   B16_B08 	near	Temperature_Adc;//��ǰ�¶�ADCֵ(hex��)
//uint08			near	Low_Voltage_Num;//���Ƿѹȥ������
//unsigned long int	near	Voltage_Time_Inc;//��ع���ʱ��
float			near	Rtcadj_Offset;//�¶Ȳ�����Уֵ
uint08			near	RtcStopProcFlag;//ֹͣ�²���־
uint16		near	Hw_Lcdled_Timer;//�����������ʱ��
uint08			near	Beep_Timer;//���������ƶ�ʱ��
uint08			near	Ext_Disp_Cnt;	//��չLCD��ѯ����
uint08			near	Init_Led_Timer;//����ָʾ�ƶ�ʱ��
uint08			near	Cover_Space_Timer;//������ʱ��ʱ��
uint08			near	RdLoadPara[3][22]; //���ݱ�ʶ(4)+ʣ���¼����(1)+����֡���(1)+��ʼ��ַ(8)+ʣ����Ч��¼��(8)
uint08			near	PowerDetCnt0,PowerDetCnt1,PowerDetCnt2,PowerDetCnt3;	//��Դ��������
uint08			near	Resetworking_Timer;//�ز���λʱ�䶨ʱ��
uint08			near	Current_Over_Count;//����բ������ֵʱ�������
uint08			near	Current_Over_Count2;//��������ʱ�������
uint08			near	Voltage_Low_Count;//��ѹ����ʱ�������
//uint08			near	Last_Timer;	//Ԥ��բ����ʱ��
//unsigned long int	near	CloseDelayTimer;//��բ����ʱ��բ��ʱ��
union   B32_B08		near	Loadtime;	//�ܹ����ۼ�ʱ��(��3�ֽ���Ч)
uint08			near	Adj_Clk_Timer_S;//��ʱ8Mʱ�ӵ�У��ʱ��(����uartר��)
uint08			near	Adj_Clk_Timer;	//��ʱ8Mʱ�ӵ�У��ʱ��

uint16		near		Self_Timer3;	//����֡���ʱ�䶨ʱ��
uint08		near		Self_Cnt;	//����֡����
uint08  	near		Bat_Delay_Timer;//���Ƿѹ��ʱʱ�䶨ʱ��
uint08		near		Find_Card;	//Ѱ����־��0x55����Ѱ����0xaa��ֹѰ����0x00����Ѱ��
uint08		near		Threshold[8];	//�¼��ж��趨��Χ:ͣ���¼���ѹ������ͣ�����������ͣ����ʱʱ��,�������ʱʱ��,Ƿѹ�¼��������ޡ�Ƿѹ��ʱʱ��
uint08		near		F_Plcevtout_Sta;//PLCEVTOUT��״̬
uint08		near		Zb_Relayctrl_Timer;//�ز�����բ��ʱ��ʱ��
uint32				near	DispBuf[40];	//?????
//uint08				near	DispBuf[160];	//��ʾ������
struct	PARA_ESAM_FRM		near	Esam_Para;	//�������1��ֵ(4)���������2��ֵ(4)+���(6)�����(6)+�ͻ����(6)+�翨����(1)+�����֤ʱЧ(2)+Crc(2)
struct 	PARA_LIMITMENORY_FRM	near	Money_Para;	//͸֧�����ֵ(4)���ڻ������ֵ(4)����բ��������ֵ(4)��Crc(2)
struct	PARA_DISPLAY_FRM	near	Display_Para;	//ѭ������(1)��ÿ����ʾʱ��(1)����ʾ����С��λ��(1) ��������(1)+ ��բ��ʱʱ�䣨2����crc��2��
struct 	COMM_ADDRESS_FRM	near	Comm_Addr;	//ͨ�ŵ�ַ(6) ��crc��2��
struct 	STARTTIME_FRM		near	Starttime;	//����ʱ�����л�ʱ�䣨5����������ʱ�α��л�ʱ�䣨5�������׷��ʵ���л�ʱ�䣨5�������׽����л�ʱ�䣨5����crc��2��
struct 	EMU_FRM			near	Emu_Para;	//����1(2)����λ1(1)������2(2)����λ2(1)+����2����(2)+����(2)����ѹϵ��(4)������ϵ��(4)������ϵ��(4)��Crc(2)
struct	PRICE_FRM		near	Price;		//��ǰ���ݵ�����16����crc��2������ǰ���ݵ�ۣ�20����crc��2������ǰ���ʵ�۱�16����crc��2��
struct 	TARIFF_FRM		near	Tariff;		//��ǰ����ʱ�α�3*14����crc��2��
struct 	ITEM_FRM		near	Item[4];	//ѭ��/������ʾ��Ŀ��1*62����crc��2��
struct	UIP_FRM			near	Emu_Data;	//��ѹ��3��+����1��3������2��3��+����1��3��+����2��3��+����������2��
struct  FUNCTION_FRM		near	Fun_Para;	//���ܲ���
struct	DECIMAL_FRM		near	Decimal;	//����β������
struct 	DISPALY_FRM		near	Display;
struct 	EEPROM_FRM		near	Eeprom;
struct 	COMMUNICATION_FRM	near	Uart[3];
struct	WEEK_FRM		near	Week_Holiday;
struct	TEMPERATURE_FRM		near	Tpara;
struct 	METERCONST_FRM		near	Meter;
struct 	PARA_FEATUREWORD_FRM	near	Mode;
struct	ADCDATA_FRM		near	ADCResultArr;
struct	RTC_TRIMA_FRM		near	RtcTadcBuf;
struct	RTC_TRIMC_FRM		near	RtcTcapBuf;
struct	FEIK_FRM		near	Feik;		//��ʱ����+��ʱ�α���+��ʱ����+������+����������+г������+������+��Կ������+CRC(2)
struct  TIMER2_FRM		near	Keeptime;	//��������ʱ�䣬���Ƿѹ�ۼ�ʱ�䣬�����ۼ�ʱ��

struct 	EMU_FRM			Emu_Para,Emu_Para1;	//??1(2)+??1(1)+??2(2)+??2(1)+??2??(2)+??(2)+????(4)+????(4)+????(4)+Crc(2)
struct   Att7051Eep              Att7051EepD;
union   Att7051Para              Att7051_Calib;
unsigned char MterTX_Buff[50];
unsigned char MterRX_Buff[50];

uint08			near 	RFTESTCNT;
uint08			near 	PowerLow;
unsigned char CoverDetCnt;
uint08 Flg_60lev,Flg_53lev,Flg_35lev;

//uart-u7816
unsigned char 	near	Ic_Comm_RxBuf[u7816_buf_size]	;	//7816���ջ�����
unsigned char 	near	Ic_Comm_RxPtr			;	//7816����ָ��
unsigned char 	near	Ic_Comm_TxBuf[u7816_buf_size]	;	//7816���ͻ�����
unsigned char 	near	Ic_Comm_TxPtr			;	//7816����ָ��

unsigned char 	near	UID[4]	;
unsigned char 	Charge_Token[20]	;
unsigned char 	KeyH_Token[20]	;
unsigned char 	KeyL_Token[20]	;

unsigned char 	KeyA_M1[6]	;
unsigned char 	KeyB_M1[6]	;
unsigned char 	Key_User[8]	;

union B32_B08 	near	Buy_Total_Money		;	//�����ܽ��
unsigned char 	near	Refers_File_Mac[180]		;	//�����ļ�+MAC(180�ֽ�)
union B32_B08 	near	Buy_Money_Mac[3]		;	//ʣ����+MAC
union B32_B08 	near	Buy_Times_Mac[2]		;	//�������+MAC
unsigned char 	near	BuyTimesFlag			;	//���������־��00:��ȣ�01:��1��2:���ڻ�������:�Ƿ���
unsigned char 	near	Err_Sw12[2]			;	//�ݴ��������ص�SW1��SW2

unsigned char	near	PcRst8B[8]			;	//PCԶ���·��� ��ɢ���� 8�ֽ�
unsigned char	near	PcRand[8]			;	//PCԶ���·��� �����1  8�ֽ�
unsigned char	near	EsamRst8B[8]			;	//ESAM��λ��Ϣ�к�8�ֽ�
unsigned char	near	EsamRand[8]			;	//ESAM 8�ֽ������
unsigned char	near	Esam_Timer			;	//ESAM�ϵ���ʱ��λ��ʱ��

//demand
unsigned char Pav_Cnt ;//�������������� 
float		Pav_Temp			;	//����1����ƽ������
uint08				Slidingtimer,Tar_Slidingtimer;//���ʱ(�ܺͷ���)
uint08				Demandtimer,Tar_Demandtimer;//���������ʱ(�ܺͷ���)
unsigned short			Demand[61];//������������������(����ʱ����)
unsigned short			Tar_Demand[61];//��������������(����ʱ����)		
struct PARA_DEMAND_FRM    Demand_Para;
uint08		Back_Num;		
//u7816
unsigned char 	near	Remain_back[24]			;	//����ʣ����͹������ BCD+HEX+BCD
unsigned char 	near	Buy_Money_Bcd[4]		;	//��ֵ���     BCD
union B32_B08 	near	Buy_Times			;	//�������     HEX

unsigned char	near	CardRst8B[8]			;	//CARD��λ��Ϣ�к�4�ֽ�
unsigned char	near	CardRand[8]			;	//CARD 8�ֽ������
unsigned char	near	CardCheck			;	//�����
unsigned char	near	CardCheck1			;	//Ѱ��������
unsigned char	near	CardType			;	//������
unsigned char	near  	UpdataFlag			;	//���±�־
unsigned char	near	Ic_Err_Reg			;	//IC�����������ʶ
unsigned char	near	UserCardFlag			;	//�û������ͱ�־
unsigned char	near	InsertCardSta			;	//�忨״̬��
uint32 FLAG_EA;
unsigned char	near	RTCIF		;

//unsigned char  EA;
unsigned char  IICPowerON;



unsigned char  RN8302Status;
unsigned char  RN8302Errcum;

unsigned char  Second_8302;
 unsigned char  Second_Power;
 unsigned char  PowerRead_Flag;
 unsigned char  Quadrant[4];//����ָʾ
 unsigned char  Angle_I;//�ǶȲ���
 unsigned char  U_InvOrder_Flag;//��ѹ������״̬λ
 unsigned char  I_InvOrder_Flag;//����������״̬λ
 unsigned char  FactoryCFDEN;
 unsigned long  RN8302Checksum;
 unsigned char  PowerReact1_Flag;
 unsigned char  PowerReact2_Flag;
CHIPREG ChipReg;
REALADJ RealAdj;
REAL    Real;
MEASPARA             MeasPara;
TIMER	Timer;
FLAG	     Flag;

ADJ_BLOCK Adj_Block[3];
POWERVALUE PowerValue;
CHIPREG ChipReg;
ADJ_BLOCK2 Adj_Block2[3];
//const METERPARAMETER MeterParameter;

MAXDMD_BLOCK  MaxDmd_Blk[8];
CURRENTDEMAND  CurrDmd;
PULSECOUNT_BLOCK  PCnt_Blk;
VARENERY_BLOCK  VEngy_Blk;
PHASEENERGY_BLOCK  PEngy_Blk;
RACTENERGY_BLOCK  REngy_Blk;
ACTENERGY_BLOCK  AEngy_Blk;
CYCENERY CycEnery;
MONTHENERY MonthEnery;
YEARENERY YearEnery;
MEASUREMODE  MeasureMode;

NVM1KI Nvm1Ki;
unsigned char FramGrpWriteFlag;
unsigned char E2PGrpWriteFlag;
unsigned char FramWriteFlag;
unsigned char E2PWriteFlag;

