//����ȫ�ֺ������������Ҫ��עΪextern

#ifndef __EX_VAR_H__
#define __EX_VAR_H__

#include "define4.h"

extern const float ecode const_KL;
extern const float ecode const_KH;
extern const float ecode const_xtl_top;
extern const float ecode const_TmpsL;
extern const float ecode const_TmpsH;
extern const uint08 ecode constF_XcapH;
extern const uint08 ecode constF_XcapL;
extern const uint08 ecode constF_Volt;

extern const uint16 ecode BPS_SPBRGL1[];
extern const struct HisEvent_STR ecode EventTable[];

extern const uint08 ecode runwdtcon;
extern const uint08 ecode rundivsel;
extern const uint08 ecode clkmode;
extern const uint08 ecode init_tl0;
extern const uint08 ecode init_th0;
extern const uint08 ecode runoscen;
extern const uint08 ecode sleeposcen;
extern const uint08 ecode sleeptmr0ie;
extern const uint08 ecode sleepwakeuptimer;

extern const uint08 ecode Relay_Mode;
extern const uint08 ecode Correction_Mode;
extern const uint08 ecode Lasttesting_Valid;	
extern const uint08 ecode Compile_Polar_485;
extern const uint08 ecode Compile_Lvd4x;
extern const uint08 ecode Memoryerr_Ctrl;	
extern const uint08 ecode Beep_Io_Ctrl;

extern const unsigned char ecode Verdate[18];
extern const unsigned char ecode RatedVolt[6];
extern const unsigned char ecode RatedCurr[6];
extern const unsigned char ecode MaxCurr[6];
extern const unsigned char ecode ActiveClass[4];
extern const unsigned char ecode MaterType[10];
extern const unsigned char ecode Facdate[10];
extern const unsigned char ecode ProtocolVer[16];
extern const unsigned char ecode SoftwareVer1[32];
extern const unsigned char ecode HardwareVer1[32];
extern const unsigned char ecode FactoryNum[32];
extern const unsigned char ecode Softbk[8];
extern const unsigned char ecode DispVer[3];
extern const unsigned long ecode Def_Meterconst_Bcd;
extern const unsigned char ecode Def_Meterconst_Hex;
extern const float         ecode default_meterconst_dm;
extern const unsigned char ecode Def_Plusevalidtimer;
extern const unsigned char ecode Runet1Outcnt[2];
extern const unsigned char ecode Runrtctmls;
extern const unsigned char ecode resetworking_timer_delay;

extern const uint08 ecode Relaytimer_Delay_ZB;
extern const uint08 ecode Relaytimer_Delay_N;
extern const uint08 ecode Powertimer_Delay_N;	//150ms���ü̵�����ƽ��ʱ
extern const uint08 ecode Relaytimer_Delay_W;
extern const uint08 ecode Defaultspacetimer;
extern const uint08 ecode Defrelayerrorcnt_s;
extern const uint08 ecode Defrelayerrorcount;
extern const uint08 ecode Defrelayerrorcount_r;

extern const uint08 ecode Def_Dl_Difference;
extern const uint08 ecode Adjclk_timer_delay_s;
extern const uint08 ecode Adjclk_timer_delay;
extern const uint16 ecode Defendenergyvoltage;
extern const uint08 ecode Runcalbps;
extern const uint08 ecode Def_Shielding_I1;
extern const uint08 ecode Def_Shielding_I2;
extern const uint08 ecode Def_Shielding_Cos;
extern const uint08 ecode Defcurrovercount;
extern const uint08 ecode Deflowvoltagecount;
extern const uint16 ecode Uoffset;
extern const uint08 ecode Def_Powerdetcnt2;
extern const uint08 ecode Def_PowerDetCnt0;

extern unsigned char MterTX_Buff[50];
extern unsigned char MterRX_Buff[50];

extern unsigned char CoverDetCnt;

extern const uint08 ecode Fulllcdbias;
extern const uint08 ecode Fulllcdbias2;
extern const uint08 ecode Runlcdbias;
extern const uint08 ecode Runlcdbias2;
extern const uint08 ecode Sleeplcdbias;
extern const uint08 ecode Sleeplcdfreq;
extern const uint08 ecode Sleeplcdset;
extern const uint08 ecode Sleepenmode;
extern const uint08 ecode Sleeplvdefcfg;
extern const uint08 ecode Defaultauthtimer;

extern const unsigned char code UartErrCnt_const;
extern const unsigned long ecode disp_table[];
extern const unsigned char ecode Def_Limitmenory_table[];
extern const unsigned char ecode Def_DisplayPara_table[];
extern const unsigned char ecode Def_featureword_table[];
extern const unsigned char ecode Def_weekloadpara_table[];
extern const unsigned char Def_Demand_table[];
extern const unsigned char ecode Def_feik_table[];
extern const unsigned char ecode Def_Autodisp_table[16*2];
extern const unsigned char ecode Def_Keydisp_table[32*2];

extern const uint08 code BACKBUF[];
extern const uint16 code BACKADDR[];
extern const union B32_B08_2 code SysATT7051[2];
//��������
extern const uint08 ecode Keyswitch_Valid;
extern const uint08 ecode METERTYPE;
extern const uint08 ecode SELFCLOSE;
extern const uint08 ecode default_hearttime;
extern const uint08 ecode Sleepdispdays;
extern const uint08 ecode MAX_PSW_NUM;
extern const uint08 ecode Terminal_delay;
extern const uint08 ecode Defpassworderrcnt;
extern const uint08 ecode Def_GSN;
extern const uint08 ecode Def_CWGSP;
extern const uint08 ecode Def_eventthreshold_table[];
extern uint32				RTC_Status;
extern uint32				KEY_Status;
//---------------------- bit variable ------------------------------------------------------------------------


extern 	B08_Bit bdata			        Meter_Status_S;			      //
#define Meter_Status			        Meter_Status_S.B08      	
#define	remainder_alarm1_flag		  Meter_Status_S.Bit.bit0		//���ڱ���1״̬
#define	remainder_alarm2_flag		  Meter_Status_S.Bit.bit1		//���ڱ���2״̬
#define	remainder_zero_flag		    Meter_Status_S.Bit.bit2		//ʣ��Ϊ���־
#define	remainder_maxtick_flag		Meter_Status_S.Bit.bit3		//ʣ�����Ƿ����
#define	first_alaram2_flag		    Meter_Status_S.Bit.bit4		//�մ��ڱ���2״̬
#define	first_eqzero_flag		      Meter_Status_S.Bit.bit5		//�մ��ڹ���״̬
#define	relay_dl_status_flag		  Meter_Status_S.Bit.bit6		//�̵�����������״̬(0��ʾ�պ�1��ʾ�Ͽ�)
#define	relay_commu_close2_flag		Meter_Status_S.Bit.bit7	        //��������־(��������)

extern 	B08_Bit bdata			        Meter_Status1_S;		      //(�ı���Ҫ����)
#define Meter_Status1			        Meter_Status1_S.B08
#define	relay_status_flag		      Meter_Status1_S.Bit.bit0	//�̵���״̬(0��ʾ�պ�1��ʾ�Ͽ�)
#define	card_closerelay_flag		  Meter_Status1_S.Bit.bit1	//�忨�պϼ̵�����־(����2)
#define	card_closerelay_flag1		  Meter_Status1_S.Bit.bit2	//�忨�պϼ̵�����־(����)
#define	charge_relay_open_flag		Meter_Status1_S.Bit.bit3	//���粻�պϼ̵�����־
#define	relay_commu_alarm_flag		Meter_Status1_S.Bit.bit4	//ͨѶ�����̵���
#define	relay_commu_open_flag		  Meter_Status1_S.Bit.bit5	//ͨѶ�Ͽ��̵���
#define relay_commu_disable_close_flag	Meter_Status1_S.Bit.bit6	//�������պϼ̵�����־
#define	relay_commu_close_flag		Meter_Status1_S.Bit.bit7	//ͨѶ�պϼ̵���

extern 	B08_Bit bdata		        	Meter_Status2_S;		      //(�ı���Ҫ����)
#define Meter_Status2		        	Meter_Status2_S.B08
#define	relay_success_flag		    Meter_Status2_S.Bit.bit0	//�̵��������ɹ���־
#define	relay_failed_flag		      Meter_Status2_S.Bit.bit1	//�̵��������־(�ϱ�)
#define	relay_commu_open_1a_flag	Meter_Status2_S.Bit.bit2	//ͨѶ��բ�����־
#define	relay_commu_open_1d_flag	Meter_Status2_S.Bit.bit3	//ͨѶԤ��բ�����־
#define	relay_commu_open_1e_flag	Meter_Status2_S.Bit.bit4	//ͨѶԤ��բ�����־
#define	relay_cleardl_flag		    Meter_Status2_S.Bit.bit5	//���ñ������־
#define	Relay_modechange_flag		  Meter_Status2_S.Bit.bit6  //ģʽ�ı��־(����բ�ɹ����ϱ�)
#define	self_close_flag			      Meter_Status2_S.Bit.bit7  //�Ա����־


extern 	B08_Bit bdata			        System_Status_S;		      //(ͣ�粻��)
#define System_Status			        System_Status_S.B08
#define	real_relaystatus_flag		  System_Status_S.Bit.bit0	//�̵���ʵ��״̬��־
#define	remainder_tick_flag		    System_Status_S.Bit.bit1	//������Ƿ״̬(�ϱ�)
#define	clock_error			          System_Status_S.Bit.bit2	//ʱ�Ӵ����־
#define	low_voltage_flag		      System_Status_S.Bit.bit3	//��ص�ѹ��־
#define	esam_error			          System_Status_S.Bit.bit4	//esam�����־
#define	eeprom_error_flag		      System_Status_S.Bit.bit5	//�洢�����ϱ�־
#define	hangup_comm_flag		      System_Status_S.Bit.bit6	//����ͨѶ��־
#define	already_setclock_flag		  System_Status_S.Bit.bit7	//�Ѿ�Уʱ��־

extern 	B08_Bit bdata			        System_Status1_S;		      //
#define System_Status1			      System_Status1_S.B08		  //(ͣ������)
#define	identity_prog_flag		    System_Status1_S.Bit.bit0	//��֤��̱�־
#define	identity_ir_flag		      System_Status1_S.Bit.bit1	//���������־
#define	display_zaibo_flag		    System_Status1_S.Bit.bit2 //��ʾ�ز���־
#define	dl_back_flag			        System_Status1_S.Bit.bit3	//���������־
#define	first_prog_flag			      System_Status1_S.Bit.bit4	//�״α�̱�־
#define	progenable			          System_Status1_S.Bit.bit5	//���ʹ��
#define	comm_relaystatus_flag		  System_Status1_S.Bit.bit6	//�̵���ʵ��״̬��־(ͨѶר��)
#define	relay_error			          System_Status1_S.Bit.bit7	//�̵��������־

extern 	B08_Bit bdata			        System_Status2_S;		      //
#define System_Status2			      System_Status2_S.B08
#define	esam_biaohao_flag		      System_Status2_S.Bit.bit0	//�Ѳ���esam���ű�־
#define	display_rtc_flag		      System_Status2_S.Bit.bit1	//��ʾʱ���־
#define	disp_0_flag			          System_Status2_S.Bit.bit2	//��Ҫ���㴦����־
#define	terminal_auth_flag		    System_Status2_S.Bit.bit3	//�����ն�������֤��־
#define	below_60u_flag			      System_Status2_S.Bit.bit4	//����60%��ѹ��־
#define	below_70u_flag			      System_Status2_S.Bit.bit5	//����70%��ѹ��־
#define	relay_direct_flag		      System_Status2_S.Bit.bit6	//�̵���ֱ�Ӷ�����־
#define	relay_delay_flag		      System_Status2_S.Bit.bit7	//�������ʱ��բ��־

extern 	B08_Bit bdata			        System_Status3_S;
#define System_Status3			      System_Status3_S.B08
#define	run_readrtc_flag		      System_Status3_S.Bit.bit0	//��ȡʵʱʱ�ӱ�־
#define	run_display_flag		      System_Status3_S.Bit.bit1	//ˢ��LCD��ʾ��־
#define	run_relaychk_flag		      System_Status3_S.Bit.bit2	//�̵�������־
#define	run_freeze_flag			      System_Status3_S.Bit.bit3 //����ͽ����־
#define	run_loadchk_flag		      System_Status3_S.Bit.bit4 //���ɼ�¼��־
#define	run_timezone_flag		      System_Status3_S.Bit.bit5	//ʱ��ʱ�α���־
#define	run_price_flag			      System_Status3_S.Bit.bit6	//��۸��±�־
#define	run_alarm_flag			      System_Status3_S.Bit.bit7	//����״̬��־

extern 	B08_Bit bdata			        System_Status4_S;
#define System_Status4			      System_Status4_S.B08
#define	fwdl_veri_flag			      System_Status4_S.Bit.bit0	//�������У���־
#define	bkdl_veri_flag			      System_Status4_S.Bit.bit1	//�������У���־
#define	mode_veri_flag			      System_Status4_S.Bit.bit2	//����ģʽ��У���־
#define	reg_veri_flag			        System_Status4_S.Bit.bit3 //�Ĵ���У���־
#define	addr_veri_flag			      System_Status4_S.Bit.bit4	//ͨ�ŵ�ַУ���־
#define	disppm_veri_flag		      System_Status4_S.Bit.bit5	//��ʾ����У���־
#define	dispitem_veri_flag		    System_Status4_S.Bit.bit6	//��ʾ��ĿУ���־
#define	temperpara_veri_flag		  System_Status4_S.Bit.bit7	//�¶�ϵ��У���־

extern 	B08_Bit bdata			        System_Status5_S;		      //���ж�����λ������,��ѭ��������
#define System_Status5			      System_Status5_S.B08
#define	BDMODE		                System_Status5_S.Bit.bit0
#define	rf_card_flag		          System_Status5_S.Bit.bit1 //��ƵѰ����־
#define Festival_holiday_flag	    System_Status5_S.Bit.bit2 //�ӵع��ϱ�־
#define	KeySTS_OK_flag		        System_Status5_S.Bit.bit3 //STS_Key���³ɹ���־
#define Over_voltage              System_Status5_S.Bit.bit4 //�ӵع��ϱ�־
#define dl_back_flag1             System_Status5_S.Bit.bit5 //�״η����־
#define UartErrCnt                System_Status5_S.Bit.bit6

extern 	B08_Bit bdata			        System_Status6;		      //����������ر�־
#define Dm_process_flag           System_Status6.Bit.bit0 //����������־
#define Dm_veri_flag              System_Status6.Bit.bit1 //����������־





extern B08_Bit bdata			        Ic_Type_Reg_S;
#define Ic_Type_Reg			          Ic_Type_Reg_S.B08
#define	ic_display			          Ic_Type_Reg_S.Bit.bit4		//��ʾ�п�����
#define disp_remain_flag	        Ic_Type_Reg_S.Bit.bit5		//��ʾ�����ʣ�����־
#define	disp_readic_flag	        Ic_Type_Reg_S.Bit.bit6		//��ʾ������
#define	disp_over_flag		        Ic_Type_Reg_S.Bit.bit7		//��ʾ����ǰʣ����͸֧��־�����ţ�

#define	UserCardSerialBit		      Ic_Type_Reg_S.Bit.bit0		//�������к�һ��, 0:һ��; 1:��һ��
#define	UserCardNoNull 		        Ic_Type_Reg_S.Bit.bit1		//��д�ļ���Ϊ��, 0:�� �� 1������
#define	card_open_flag		        Ic_Type_Reg_S.Bit.bit2		//�����ѿ�����־
#define	comm_open_flag  	        Ic_Type_Reg_S.Bit.bit3		//Զ���ѿ�����־

extern B08_Bit bdata			        AT7051_Flag_S;	
#define AT7051_Flag			          AT7051_Flag_S.B08
#define	read_back_flag		        AT7051_Flag_S.Bit.bit0		//�����������־
#define	init_7051_flag		        AT7051_Flag_S.Bit.bit1		//��ʼ��AT7051��־1
#define	read_uip_flag1		        AT7051_Flag_S.Bit.bit2		//��AT7051˲ʱֵ��־1
#define	check_7051_flag1	        AT7051_Flag_S.Bit.bit3		//У��AT7051����ֵ��־1
#define	load_over_flag		        AT7051_Flag_S.Bit.bit4		//������־
#define	over_xxx_flag			        AT7051_Flag_S.Bit.bit5		//��բ��������־
#define	power_zero_flag		        AT7051_Flag_S.Bit.bit6		//���ʵ���0��־
#define	energy_rst_flag		        AT7051_Flag_S.Bit.bit7		//����оƬ��λ��־



//;=============================< IDATA �洢���� >================================
extern 	uint08			bdata	DisKeySamp;	//��ʾ����ȥ���Ĵ���
extern 	uint08			bdata	DisKeySamp1;	//��ʾ����ȥ���Ĵ���
extern 	uint08			bdata	CardKeySamp;	//�忨ȥ���Ĵ���
extern 	uint08			bdata	Uart1KeySamp;	//485���Լ��ȥ���Ĵ���
extern 	uint08		  idata	VectorTimer;	//���붨ʱ��
extern 	struct 	    SYS_CLOCK_FRM	idata	Sclock;		//ʱ������
extern 	uint08			idata	UpdateMoneytimer;//����ESAMǮ����ʱ��
extern 	uint08			idata	Key_Status;//��Կ״̬,00��ʾȫ����Կ,01��ʾȫ��˽Կ,FF���й���˽

//;=============================< XDATA �洢���� >================================
extern 	struct      TIMER_FRM	near	Progkey;	//240min���(ͣ������)
extern 	struct      TIMER_FRM	near	Cover;		//���ǲ���(ͣ�籣��)
extern 	uint08			near	Mac_Err_Times;	//MACУ��ʧ�ܴ���
extern 	uint08			near	System_St_Bk;	//ϵͳ״̬�ֱ���(ͣ������)
extern 	uint08			near	Report_Data[5];	//��Ч�ϱ�״̬��
extern 	uint08			near	Report_Data2[5];//��Ч�ϱ�״̬�ֱ���
extern 	uint08			near	Report_Num[40];	//�¼��ϱ���������
extern 	uint16			near	Reset_Rep_Timer;//��λ�ϱ�״̬�ֶ�ʱ��
extern 	uint08			near	RReport_Status;	//֮ǰ�Ѷ��ϱ�״̬������λ�ϱ�״̬
extern  uint08			near	Password_Err[2];//����������
extern 	struct	    LOADREC_FRM	near	Load;		//����ָ�빲18�ֽ�
extern 	uint08			near	Wr_Ext_Number;	//�ڲ���չ����дee��������
extern 	uint16			near	Self_Timer24;	//�Ա���24Сʱ��ʱ��
extern 	struct	    MONEY_FRM	near	Money;		//ʣ����(6)+͸֧���(6)+�������(4)+Crc(2)
extern  struct      USERINFOR_FRM  Money_UserINFO;	//��һ��ʹ�ý�� BCD 4 ��  //ʣ�����������BCD	
extern 	struct	    FWDL_FRM	near	Fwdl;		//�����й��ܵ���(4)�������(4)�������(4)��ƽ����(4)���ȵ���(4)�����������й��õ���(4)��Crc(2)
extern 	struct	    BKDL_FRM	near	Bkdl;		//�����й��ܵ���(4)�������(4)�������(4)��ƽ����(4)���ȵ���(4)�����·����й��õ���(4)��Crc(2)
extern	uint08			near	RAMREG0;	//��������Ĵ��� 
extern	uint08			near	RAMREG1;	//
extern	uint08			near	RAMREG2;	//
extern	uint08			near	RAMREG3;	//
extern	uint08			near	RAMREG4;	//
extern	uint08			near	RAMREG5;	//
extern	uint08			near	RAMREG6;	//
extern	uint08			near	RAMREG7;	//
extern	uint08			near	RAMREG8;	//
extern	uint08			near	RAMREG9;	//
extern	uint08			near	RAMREG10;	//
extern	uint08			near	RAMREG11;	//
extern	uint08			near	RAMREG12;	//

extern APDU_ANALYZE_VAR      gs_APDU_analyze_var;
extern APDU_KEY_EXRAM_VAR    gs_APDU_key_var;
extern APDU_KEY_EXRAM_VAR   sts_kay_process_data;

extern STS_KAY_CHANHE_DATA   sts_kay_change_data;
extern  uint08			near 	PLC_RST1_Timer;
extern 	uint08			near	Outputtype;	//0-������,1-����,2-ʱ��Ͷ���ź�,0x50-FOUT���
extern 	struct      TIMER_FRM	near	Successled,Shiduanctrl,Relayctrl,Powertimer;
extern 	struct      TIMER_FRM	near	Adcctrl;
extern 	union       B16_B08		near	Lcdled;//���ⶨʱ��
extern	union   B32_B08			Current_Demand;//��ǰ����(���ֽ���ǰ)
extern 	union       B32_B08		near	Identitytimer;	//��ȫ��֤��ʱ��
extern 	uint16			near	IR_EnTimer;	//������֤��ʱ��
extern 	union       B32_B08		near	Opendelaytimer;	//��բ��ʱ��ʱ��
extern 	union	      B32_B08		near	CurrentTimer;	//����բ������ʱ��
extern 	uint08			near	Relayerror_Count,Relayerror_Count1,Relayerror_Count2;//�̵������������
extern 	uint08			near	Relay_Space_Timer;//�ϵ���ʱ���Ƽ̵�����ʱ��
extern 	uint08			near	Keyclose_timer;//�����պϼ̵�����ʱ��
extern 	uint08			near	Display_Space_Timer;//���������ʾ�����ʱ��
extern 	uint16      near	Stop_Disptimer;//ͣ����ʾʱ�䶨ʱ
extern 	signed char		near	Temperature_Hex;//�����¶�(hex��)
extern	union   B16_B08 	near	Temperature_Adc;//��ǰ�¶�ADCֵ(hex��)
//extern	uint08			near	Low_Voltage_Num;//���Ƿѹȥ������
//extern 	unsigned long int	near	Voltage_Time_Inc;//��ع���ʱ��
extern	float			near	Rtcadj_Offset;//�¶Ȳ�����Уֵ
extern	uint08			near	RtcStopProcFlag;//ֹͣ�²���־
extern 	uint16		near	Hw_Lcdled_Timer;//�����������ʱ��
extern 	uint08			near	Beep_Timer;//���������ƶ�ʱ��
extern 	uint08			near	Ext_Disp_Cnt;//��չLCD��ѯ����
extern 	uint08			near	Init_Led_Timer;//����ָʾ�ƶ�ʱ��
extern 	uint08			near	Cover_Space_Timer;//������ʱ��ʱ��
extern 	uint08			near	RdLoadPara[3][22]; //���ݱ�ʶ(4)+ʣ���¼����(1)+����֡���(1)+��ʼ��ַ(8)++ʣ����Ч��¼��(8)
extern  uint08			near	PowerDetCnt0,PowerDetCnt1,PowerDetCnt2,PowerDetCnt3;//��Դ��������
extern	uint08			near	Resetworking_Timer;//�ز���λʱ�䶨ʱ��
extern	uint08			near	Current_Over_Count;//����բ������ֵʱ�������
extern	uint08			near	Current_Over_Count2;//��������ʱ�������
extern	uint08			near	Voltage_Low_Count;//��ѹ����ʱ�������
//extern	uint08			near	Last_Timer;	//Ԥ��բ����ʱ��
//extern	unsigned long int	near	CloseDelayTimer;//��բ����ʱ��բ��ʱ��
extern	union	B32_B08		near	Loadtime;	//�ܹ����ۼ�ʱ��(��3�ֽ���Ч)
extern	uint08			near	Adj_Clk_Timer_S;//��ʱ8Mʱ�ӵ�У��ʱ��(����uartר��)
extern	uint08			near	Adj_Clk_Timer;	//��ʱ8Mʱ�ӵ�У��ʱ��

extern	uint16		near		Self_Timer3;	//����֡���ʱ�䶨ʱ��
extern	uint08		near		Self_Cnt;	//����֡����
extern  uint08  	near		Bat_Delay_Timer;//���Ƿѹ��ʱʱ�䶨ʱ��
extern  uint08		near		Find_Card;
extern  uint08		near		Threshold[8];	//�¼��ж��趨��Χ:ͣ���¼���ѹ������ͣ�����������ͣ����ʱʱ��,��������ʱʱ��,Ƿѹ�¼��������ޡ�Ƿѹ��ʱʱ��
extern  uint08		near		F_Plcevtout_Sta;//PLCEVTOUT��״̬
extern  uint08		near		Zb_Relayctrl_Timer;//�ز�����բ��ʱ��ʱ��
extern uint32				near	DispBuf[40];	//?????
//extern  uint08				near	DispBuf[160];	//��ʾ������
extern 	struct	PARA_ESAM_FRM		near	Esam_Para;	//�������1��ֵ(4)���������2��ֵ(4)+���(6)������(6)+�ͻ����(6)+�翨����(1)+������֤ʱЧ(2)+Crc(2)
extern 	struct 	PARA_LIMITMENORY_FRM	near	Money_Para;	//͸֧�����ֵ(4)���ڻ������ֵ(4)����բ���������ֵ(4)��Crc(2)
extern 	struct	PARA_DISPLAY_FRM	near	Display_Para;	//ѭ������(1)��ÿ����ʾʱ��(1)����ʾ����С��λ��(1) ��������(1)+ ��բ��ʱʱ�䣨2����crc��2��
extern 	struct 	COMM_ADDRESS_FRM	near	Comm_Addr;	//ͨ�ŵ�ַ(6) ��crc��2��
extern 	struct 	STARTTIME_FRM		near	Starttime;	//����ʱ�����л�ʱ�䣨5����������ʱ�α��л�ʱ�䣨5�������׷��ʵ���л�ʱ�䣨5�������׽����л�ʱ�䣨5����crc��2��
extern 	struct 	EMU_FRM			near	Emu_Para;	//����1(2)����λ1(1)������2(2)����λ2(1)+����2����(2)+����(2)����ѹϵ��(4)������ϵ��(4)������ϵ��(4)��Crc(2)
extern 	struct	PRICE_FRM		near	Price;		//��ǰ���ݵ�����16����crc��2������ǰ���ݵ�ۣ�20����crc��2������ǰ���ʵ�۱���16����crc��2��
extern 	struct 	TARIFF_FRM		near	Tariff;		//��ǰ����ʱ�α���3*14����crc��2��
extern 	struct 	ITEM_FRM		near	Item[4];	//ѭ��/������ʾ��Ŀ��1*62����crc��2��
extern 	struct	UIP_FRM			near	Emu_Data;	//��ѹ��3��+����1��3������2��3��+����1��3��+����2��3��+����������2��
extern 	struct  FUNCTION_FRM		near	Fun_Para;	//���ܲ���
extern 	struct	DECIMAL_FRM		near	Decimal;	//����β������
extern 	struct 	DISPALY_FRM		near	Display;
extern 	struct 	EEPROM_FRM		near	Eeprom;
extern 	struct 	COMMUNICATION_FRM	near	Uart[3];
extern 	struct	WEEK_FRM		near	Week_Holiday;
extern 	struct	TEMPERATURE_FRM		near	Tpara;
extern 	struct 	METERCONST_FRM		near	Meter;
extern 	struct 	PARA_FEATUREWORD_FRM	near	Mode;
extern	struct	ADCDATA_FRM		near	ADCResultArr;
extern	struct	RTC_TRIMA_FRM		near	RtcTadcBuf;
extern	struct	RTC_TRIMC_FRM		near	RtcTcapBuf;
extern 	struct	FEIK_FRM		near	Feik;
extern  struct  TIMER2_FRM		near  	Keeptime;	//��������ʱ�䣬���Ƿѹ�ۼ�ʱ�䣬�����ۼ�ʱ��
extern struct   Att7051Eep              Att7051EepD;
extern  union   Att7051Para              Att7051_Calib;

extern 	struct 	EMU_FRM			Emu_Para,Emu_Para1;	//??1(2)+??1(1)+??2(2)+??2(1)+??2??(2)+??(2)+????(4)+????(4)+????(4)+Crc(2)

//demand
extern unsigned char Pav_Cnt ;//�������������� 
extern float		Pav_Temp			;	//����1����ƽ������
extern uint08				Slidingtimer,Tar_Slidingtimer;//���ʱ(�ܺͷ���)
extern uint08				Demandtimer,Tar_Demandtimer;//���������ʱ(�ܺͷ���)
extern unsigned short			Demand[61];//������������������(����ʱ����)
extern unsigned short			Tar_Demand[61];//��������������
extern struct PARA_DEMAND_FRM    Demand_Para;
extern uint08		Back_Num;	
//uart-u7816
extern  unsigned char 	near	UID[4];

extern  unsigned char 	Charge_Token[20]	;
extern  unsigned char 	KeyH_Token[20]	;
extern  unsigned char 	KeyL_Token[20]	;

extern  unsigned char 	KeyA_M1[6]	;
extern  unsigned char 	KeyB_M1[6]	;

extern const uint16 code Rly_voltage;		//?????????
extern const unsigned long int code METER_PWZEROVAUL;
extern const unsigned long int code METER_I1ZEROVAUL;
extern const unsigned char code METER_BACKCNT ;
extern  unsigned char 	Key_User[8]	;
extern const uint16 code Defaultpdvoltage ;	//???????(60%Un)
extern const uint16 code Deflowledvoltage;	//
extern const uint08 code Deflowvoltagecount;	//????????????(????60%????)
extern uint08 Flg_60lev,Flg_53lev,Flg_35lev;
extern uint08			near 	PowerLow;



extern uint08			near 	RFTESTCNT;
extern 	unsigned char 	near	Ic_Comm_RxBuf[u7816_buf_size]	;	//7816���ջ�����
extern 	unsigned char 	near	Ic_Comm_RxPtr			;	//7816����ָ��
extern 	unsigned char 	near	Ic_Comm_TxBuf[u7816_buf_size]	;	//7816���ͻ�����
extern 	unsigned char 	near	Ic_Comm_TxPtr			;	//7816����ָ��

extern union B32_B08 	near	Buy_Total_Money		;	//�����ܽ��
extern  unsigned char 	near	Refers_File_Mac[180]		;	//�����ļ�+MAC(480�ֽ�)                       
extern  union B32_B08 	near	Buy_Money_Mac[3]		;	//ʣ����+MAC
extern  union B32_B08 	near	Buy_Times_Mac[2]		;	//�������+MAC
extern  unsigned char 	near	BuyTimesFlag			;	//���������־��00:��ȣ�01:��1��2:���ڻ�������:�Ƿ���
extern  unsigned char 	near	Err_Sw12[2]			;	//�ݴ��������ص�SW1��SW2

extern 	unsigned char	near	PcRst8B[8]			;	//PCԶ���·��� ��ɢ���� 8�ֽ�
extern 	unsigned char	near	PcRand[8]			;	//PCԶ���·��� �����1  8�ֽ�
extern 	unsigned char	near	EsamRst8B[8]			;	//ESAM��λ��Ϣ�к�8�ֽ�
extern 	unsigned char	near	EsamRand[8]			;	//ESAM 8�ֽ������
extern 	unsigned char	near	Esam_Timer			;	//ESAM�ϵ���ʱ��λ��ʱ��

//u7816
extern 	unsigned char 	near	Remain_back[24]			;	//����ʣ����͹������ BCD+HEX+BCD
extern 	unsigned char 	near	Buy_Money_Bcd[4]		;	//��ֵ���     BCD
extern 	union B32_B08 	near	Buy_Times			;	//�������     HEX

extern 	unsigned char	near	CardRst8B[8]			;	//CARD��λ��Ϣ�к�8�ֽ�
extern 	unsigned char	near	CardRand[8]			;	//CARD 8�ֽ������
extern 	unsigned char	near	CardCheck			;	//�����
extern  unsigned char	near	CardCheck1			;	//Ѱ��������
extern 	unsigned char	near	CardType			;	//������
extern  unsigned char	near  	UpdataFlag			;	//���±�־
extern 	unsigned char	near	Ic_Err_Reg			;	//IC�����������ʶ
extern  unsigned char	near	UserCardFlag			;	//�û������ͱ�־
extern  unsigned char	near	InsertCardSta			;	//�忨״̬��

extern uint32  FLAG_EA;
extern unsigned char	near	RTCIF	;
extern unsigned char  IICPowerON;

extern unsigned char  RN8302Status;
extern unsigned char  RN8302Errcum;
extern unsigned char  Second_8302;
extern unsigned char  Second_Power;
extern unsigned char  PowerRead_Flag;
extern unsigned char  Quadrant[4];//����ָʾ
extern unsigned char  Angle_I;//�ǶȲ���
extern unsigned char  U_InvOrder_Flag;//��ѹ������״̬λ
extern unsigned char  I_InvOrder_Flag;//����������״̬λ
extern unsigned char  FactoryCFDEN;
extern unsigned long  RN8302Checksum;
extern unsigned char  PowerReact1_Flag;
extern unsigned char  PowerReact2_Flag;

extern ADJ_BLOCK Adj_Block[3];
extern POWERVALUE PowerValue;
extern CHIPREG ChipReg;
extern REALADJ RealAdj;
extern	REAL Real;
extern	MEASPARA  MeasPara;
extern	   FLAG	     Flag;
extern	TIMER	Timer;
extern const FACTOR ecode Factor;
extern const METERSPECIAL ecode Meterspecial;
extern const METERPARAMETER ecode MeterParameter;
extern const unsigned char FirmWareVersion[50];
extern const unsigned char FirmWareRecord[8];
extern const unsigned char HardWareVersion[32];



extern	MAXDMD_BLOCK  MaxDmd_Blk[8];//���򡢷�������޹�1��2��4�����޹��й��������
extern	CURRENTDEMAND  CurrDmd;//��ǰ����
extern	PULSECOUNT_BLOCK  PCnt_Blk;
extern	VARENERY_BLOCK  VEngy_Blk;
extern	PHASEENERGY_BLOCK  PEngy_Blk;
extern	RACTENERGY_BLOCK  REngy_Blk;
extern	ACTENERGY_BLOCK  AEngy_Blk;
extern CYCENERY CycEnery;
extern MONTHENERY MonthEnery;
extern YEARENERY YearEnery;
extern	MEASUREMODE  MeasureMode;
extern NVM1KI Nvm1Ki;
extern ADJ_BLOCK2 Adj_Block2[3];


extern const  signed short  ecode  g_PerKwh;
extern const  signed short  ecode g_Per1Kwh;
 
extern  unsigned char FramGrpWriteFlag;
extern  unsigned char E2PGrpWriteFlag;
extern  unsigned char FramWriteFlag;
extern  unsigned char E2PWriteFlag;


//extern 	unsigned char 	bdata	Ic_Type_Reg			;		
//extern 	unsigned char 	bdata	AT7051_Flag			;	

#endif