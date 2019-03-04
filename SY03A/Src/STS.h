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
#define TID_REC_MAXNUM  50		//TID��¼����50��//

// 2����Կ�ļ��ʱ�䶨�壬��λ�� //
#define MAX_SECONDS_OF_MULTIPLE_TOKEN_INTERVAL		90    // 90�� //   

//******************************************************************
//             4     ��������                                      
//******************************************************************
extern void api_Reset_APDU_dataOfDefault(void);
extern void api_initialize_APDU_when_reset(void);
extern void api_save_APDU_key_var(void);
extern void api_updata_APDU_key_var(void);
extern void api_set_APDU_TID_Record(INT32U u32_tid_tmp,INT8U mode);
extern INT8U api_Handle_key_sts_code(INT8U *ptr_kay_data);
extern void api_judge_keyChg_TOKEN_interval_pre_second(void);
extern ErrorStatus Lnk_OBIS_D7X_STS_deal(void);

//******************************************************************
//             5     �������ݣ�����αָ���             
//******************************************************************
//gs_APDU_analyze_var.err_flg ���������ֶ���//
#define	STS_TOKEN_OK					0	//����token�������OK//
#define	STS_TOKEN_REJRCT_1				1	//����ʧ�ܣ���������Կ����//				
#define	STS_TOKEN_REJRCT_2				2	//��֧�ֵ�TOKEN��//	
#define	STS_TOKEN_REJRCT_3				3	//DDTK��Կ����֧�ֳ�ֵTOKEN//	
#define	STS_TOKEN_REJRCT_4				4	//���ܳɹ�������TOKEN�ڵ����ݳ�����Χ//	
#define	STS_TOKEN_REJRCT_5				5	//��Կ�滻Ȩ�޲��㣬������Կ��ֹ�޸�DITK��Կ//	
#define	STS_TOKEN_REJRCT_6				6	//TID������Կ��Ч��//	
#define	STS_TOKEN_REJRCT_USED			7	//TOKEN�Ѿ���ʹ�ù�//	
#define	STS_TOKEN_REJRCT_OLD			8	//TOKEN�ѹ��ڣ���ǰTOKEN��TID<��¼�����1��TID��//	
#define	STS_TOKEN_REJRCT_OVER_CRD		9	//��ֵ������ȷ���ǳ����ڻ�����//	
#define	STS_TOKEN_CHG_KEY_PLS_2nd		10	//��Կ���ĵ�1�������ɹ����ȴ������2��//	
#define	STS_TOKEN_CHG_KEY_PLS_1st		11	//��Կ���ĵ�2�������ɹ����ȴ������1��//	

//sts_kay_change_data.flg define 
#define KeyChangeHO BIT0
#define KeyChangeLO BIT1



//******************************************************************
//             6     ���ݴ洢����        
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
 #define F01_BLOCK52_ADR_START	ADR_BLOCK52_TIDREC_E2P	//�����׵�ַ  //
 #define LEN_BLOCK52_TIDREC_E2P	(sizeof(APDU_TID_REC))	//������Ч�������ݳ���  //
 #define OFFSET_BLOCK52_CSCK	(ST_MB_OFFSET(APDU_TID_REC,u16_csck))	//����16bitsУ�������ƫ����  //
 #define ADR_BLOCK52_CSCK_E2P	(F01_BLOCK52_ADR_START+OFFSET_BLOCK52_CSCK)		//����16bitsУ�������洢��ַ  //
 
#define ADR_TIDREC_01	(F01_BLOCK52_ADR_START +ST_MB_OFFSET(APDU_TID_REC,hex24_tid[0][0]))//���1��TID��¼//


/***************************************************************
*    END
****************************************************************/

