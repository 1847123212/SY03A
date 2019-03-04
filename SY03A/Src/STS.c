#include "ex_func.h"
#include "ex_var.h"
#include <absacc.h>
#include <string.h>
const APDU_KEY_EXRAM_VAR	cgs_stsKeyParam_default=
{
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01, 	//INT8U	secret_key[8];	//�����Կ���� ,������ȡ,8�ֽ�//
	0xFF,										//��Կ��Ч��,8bit, //
	0x00,										//��Կ����, 2bit ,//
	0x00,										//��Կ�汾��,4bit //
	0x00,										// ����������,8bit,0~99//	
	0x00,0X92,									// �������,����Ϊ0X92//	
 	0x99,0X99,0X92,								// SGC���//	
	(INT16U)0,//	INT16U	csck;		//16λ�ۼӺ�У��ֵ//
};

// CRC 16 ���ݱ� //
const INT16U crc_tab[256]=
                   {                       
                      0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
                      0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
                      0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
                      0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
                      0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
                      0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
                      0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
                      0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
                      0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
                      0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
                      0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
                      0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
                      0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
                      0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
                      0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
                      0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
                      0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
                      0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
                      0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
                      0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
                      0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
                      0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
                      0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
                      0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
                      0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
                      0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
                      0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
                      0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
                      0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
                      0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
                      0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
                      0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040,
                   };

//STA�ӽ���������س�������  //
const unsigned char HXDTTable1[16] = 
{12, 6, 2, 7, 13, 9, 5, 4, 1, 10, 14, 8, 0, 3, 15, 11};
const unsigned char HXDTTable2[16] = 
{9, 11, 6, 5, 12, 7, 14, 2, 13, 3, 1, 15, 4, 8, 0, 10};
const unsigned char HXETTable1[16] = 
{14, 10, 7, 9, 12, 3, 2, 5, 13, 0, 15, 1, 4, 8, 6, 11};
const unsigned char HXETTable2[16] = 
{12, 8, 2, 13, 7, 6, 1, 3, 11, 5, 9, 15, 0, 4, 10, 14};
const unsigned char HXPETable[64] =  {
                                      55, 42, 10, 18, 24, 21, 44, 35,
                                      2,  22, 56, 43, 27, 58, 9,  50,
                                      6,  36, 12, 61, 37, 38, 53, 16,
                                      62, 3,  7,  4,  32, 20, 63, 25,
                                      51, 52, 54, 33, 49, 19, 46, 29,
                                      48, 31, 23, 30, 41, 28, 13, 5,
                                      40, 60, 39, 11, 15, 17, 1,  0,
                                      57, 34, 59, 8,  47, 14, 45, 26
                                   };

unsigned char HXPDTable[64] =  {
                                      55, 54, 8,  25, 27, 47, 16, 26,
                                      59, 14, 2,  51, 18, 46, 61, 52,
                                      23, 53, 3,  37, 29, 5,  9,  42,
                                      4,  31, 63, 12, 45, 39, 43, 41,
                                      28, 35, 57, 7,  17, 20, 21, 50,
                                      48, 44, 1,  11, 6,  62, 38, 60,
                                      40, 36, 15, 32, 33, 22, 34, 0,
                                      10, 56, 13, 58, 49, 19, 24, 30
                                      
                                  };

/* Table - Binary */
unsigned char binary[64] = {
0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1,
0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1,
1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1,
1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1 };

/*********************************************************************************************************
**  ������
********************************************************************************************************/
/*****************************************************************************
** Function name    :INT16U checkSTS_crc(INT8U *ptr, INT8U len)
** Description      :CRC16У��ͼ���                  
**
** Parameters       :INT8U *ptr  У�����������׵�ַ       
**                   INT8U len У���������鳤��
** Returned value   :INT16U CRC16У���
**
** Note             :У����������ΪINT8U��������
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
INT16U checkSTS_crc(INT8U *ptr, INT8U len)
{
    INT8U i;
    INT16U tmp, short_c;
    INT16U crc = 0xffff;

    for(i=0; i<len; i++)                        
    {
        short_c = 0x00ff & (unsigned int) *ptr;
        tmp =  crc ^ short_c;
        crc = (crc >> 8) ^ crc_tab[ tmp & 0xff ];
        ptr --;
    }

    tmp  = crc & 0xff00;
    crc  = crc << 8;
    crc |= tmp >> 8;

    return crc;
}


/*****************************************************************************
** Function name    :void STA(INT8U *source,INT8U * dest,INT8U * inkey, INT8U type)
** Description      :STA�ӽ������㺯��  //             
**
** Parameters       :INT8U *source  Դ�����׵�ַ,8�ֽ�HEX��//  
**                  :INT8U * dest �����������׵�ַ(����/����),8�ֽ�HEX��//  
**                  :INT8U * inkey ��Կ�����׵�ַ,8�ֽ�HEX��//  
**                  :INT8U type �ӽ�������������ݣ� ����(1),����(0)   //
** Returned value   :NONE
**
** Note             :�ü����㷨Դ��IEC62055-41��׼��STSЭ���׼��ϵ
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
//void STA(INT8U *source,INT8U * dest,INT8U * inkey, INT8U type)
void STA(INT8U *source,INT8U * dest,INT8U * inkey, INT8U type)
{
  INT8U buffer1[64],buffer2[64],kwork[64];
  INT8U tmp,tmp1,tmp2;
  INT8U i,j,k;


    if(type)   
    {
        // ����ǰ�Ĳ��ִ��� //
        for(i=0; i<8; i++)
        *(inkey+i) = *(inkey+i) ^ 0xff;         //�������� //
        tmp1 = *inkey;                          //��ѭ��12bits//
        tmp2 = *(inkey+1);
        for(i=1; i<7; i++)
        {
            *(inkey+i-1) = (*(inkey+i) &0xf0) >> 4;
            *(inkey+i-1) |= (*(inkey+i+1) &0x0f) << 4;
        }
        *(inkey+6) = (*(inkey+7) &0xf0) >> 4;
        *(inkey+6) |= (tmp1 &0x0f) << 4;
        *(inkey+7) = (tmp1 &0xf0) >> 4;
        *(inkey+7) |= (tmp2 &0x0f) << 4;
    }

    //Դ���ݴ���:��8���ֽ����ݷ�Ϊ64λ��ÿλ��1���ֽڴ��//
    for(i = 0; i < 8; i++)
    {
        tmp = *(source + i);
        for(j=0; j<8; j++)
        {
            if(tmp & 0x01)       buffer2[8*i+j] = 1;
            else      buffer2[8*i+j] = 0;
            tmp >>= 1;
        }
    }

    //��Կ���ݴ���:��8���ֽ����ݷ�Ϊ64λ��ÿλ��1���ֽڴ��//
    for(i = 0; i < 8; i++)
    {
        tmp = *(inkey + i);
        for(j=0; j<8; j++)
        {
            if(tmp &0x01)    kwork[8*i+j] = 1;
            else     kwork[8*i+j] = 0;
            tmp >>= 1;
        }
    }

    // �ӽ��ܴ��� //
    if(type)
    {
        for(k=0; k<16; k++)
        {
            for(i=0; i<16; i++)//substitution process
            {
                tmp = 0x00;
                for(j = 3; j > 0; j--)
                tmp = (tmp + buffer2[4*i+j]) * 2;
                tmp = tmp + buffer2[4*i];
							
                if(kwork[i*4 + 3])
                {
                    tmp = *(HXETTable2  + tmp);
                }
                else
                {
                    tmp = *(HXETTable1  + tmp);
                }

                buffer1[ i*4+3] = binary[0+tmp*4];//��Ϊ������//
                buffer1[ i*4+2] = binary[1+tmp*4];
                buffer1[ i*4+1] = binary[2+tmp*4];
                buffer1[ i*4] = binary[3+tmp*4];
            }

            for(i=0; i<64; i++)//Permutation process
            {
                buffer2[HXPETable[i]] = buffer1[i];
            }

            tmp = kwork[63];//����һλ//
            for(i = 63; i > 0; i--)
            kwork[i] = kwork[i-1];
            kwork[0] = tmp;
        }
    }
    else
    {
        for(k=0; k<16; k++)
        {
            for(i=0; i<64; i++)//Permutation process
            {
                buffer1[HXPDTable[i]] = buffer2[i];
            }

            for(i=0; i<16; i++)//substitution process
            {
                tmp = 0x00;
                for(j = 3; j > 0; j--)
                tmp = (tmp + buffer1[4*i+j]) * 2;
                tmp = tmp + buffer1[4*i];

                if(kwork[i*4])
                {
                    tmp = *(HXDTTable2  + tmp);
                }
                else
                {
                    tmp = *(HXDTTable1  + tmp);
                }

                buffer2[ i*4+3] = binary[0+tmp*4];//��Ϊ������//
                buffer2[ i*4+2] = binary[1+tmp*4];
                buffer2[ i*4+1] = binary[2+tmp*4];
                buffer2[ i*4] = binary[3+tmp*4]; 
            }

            tmp = kwork[0];//����һλ//
            for(i = 0; i < 63; i++)
            kwork[i] = kwork[i+1];
            kwork[63] = tmp;
        }
    }

    //�����û�λ��  //
    j = 0;
    for(i = 0; i < 8; i++)
    {
        *(dest + i) = 0x00;
        for(k = 7; k > 0; k--)
        {
            *(dest + i) = ((*(dest + i)) + buffer2[j+k]) * 2;
        }
        
        *(dest + i) = *(dest + i) + buffer2[j];
        j += 8;
    }
}

/*****************************************************************************
** Function name    :INT8U Class_get(INT8U* token_array)
** Description      :��ȡָ��TOKEN����������class//          
**
** Parameters       :INT8U* token_array  TOKEN���9�ֽ�HEX����׵�ַ//
**                  :
** Returned value   :TOKEN�����//
**
** Note             :28��29��λ��65��66��λ��������ȡTOKEN�����//
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
INT8U Class_get(INT8U* token_array)
{
  INT8U CLASS_NO;

  CLASS_NO = 0;
  CLASS_NO |= (token_array[3] & 0X18) >> 3;

  if(token_array[8] & 0X01)
     token_array[3] = token_array[3] | 0X08;
  else
     token_array[3] = token_array[3] & 0XF7;

  if(token_array[8] & 0X02)
     token_array[3] = token_array[3] | 0X10;
  else
     token_array[3] = token_array[3] & 0XEF;  

  token_array[8] = CLASS_NO;

  return CLASS_NO;
}


/*****************************************************************************
** Function name    :INT32U Get_TID(INT8U* tokendata )
** Description      :��ȡָ��TOKEN�������TIDֵ//        
**
** Parameters       :INT8U* tokendata  ���ܺ��TOKEN���8�ֽ�HEX����׵�ַ//
**                  :
** Returned value   :TIDֵ//
**
** Note             :
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
INT32U Get_TID(INT8U* tokendata )
{
INT8U* Ptr;
INT32U  TID;
INT8U i;

    Ptr = tokendata +6;
    TID = 0;
    for(i=0; i<2; i++)
    {
       TID |= (unsigned long)(*Ptr);
       TID <<= 8;
       Ptr--;
    }
    TID |= (unsigned long)(*Ptr);
    return TID;
}


/***************************************************************************************************
* Function Name		: CalcTransferAmt
* Description		: calculate the power or money
* Input				��<data> token+2 address
* Output			��
* Return			: amount 4 bytes ��0.1kWh��
****************************************************************************************************/
INT32U CalcTransferAmt(INT8U* u8_data)
{
	INT32U  t;
//	double tt = 0;
	INT8U e;
	INT16U m;
	INT16U   amount;

	INT32U  tt_my;
//	INT32U  t_my;
	INT32U   t_d_my1,t_d_my2;	

	amount = (unsigned int)u8_data[1]*256 + u8_data[0];
	e = (char)((amount & 0xc000) >> 14);
	m = amount & 0x3fff;

	if(e == 0)
	{
		t = (unsigned long)m;    //   //
	}
	else
	{
		switch (e)
		{
			case 1:
				tt_my=1;
				t_d_my2 =10;
				break;
			case 2:
				tt_my=11;
				t_d_my2 =100;
				break;
			case 3:
				tt_my=111;
				t_d_my2 =1000;
				break;
			default:
				tt_my=0;
				t_d_my2 =1;
				break;
		}
		t_d_my1 = tt_my*16384; // 2��14�η� //
		t = t_d_my1 + (t_d_my2*((INT32U)(m)));
	}


	return t;
}

/***************************************************************************************************
* Function Name   : INT8U get_token_test_type(INT8U *ptr)
* Description     : get TOKEN��s TEST TYPE
* Input           ��<data> token+3 address
* Output          ��
* Return          : TEST TYPE (36 BITs contel flg)
****************************************************************************************************/
INT8U get_token_test_type(INT8U *ptr)
{
 INT8U i,j;
 INT8U u8_test_type;
 INT8U test_data[5];
 
	i = 1;
	j = 1;
    
    test_data[0]=*(ptr+0);  //1~8bit //
    test_data[1]=*(ptr+1);  //9~16bit //    
    test_data[2]=*(ptr+2);  //17~24bit //
    test_data[3]=*(ptr+3);  //25~32bit //
    test_data[4]=(*(ptr+4))&0x0F;  //33~36bit //

    //��λ�Ƚ�36��λ������1�˳�ѭ�� //	
	while(i < 36)
	{
		if(*ptr & j)	break;   //BIT == 1 ���� //
		if(j == 0x80)
		{
			ptr++;
			j = 0x01;
		}
		else 
		{
			j <<= 1;
		}
		i++;
	}

    // ��ȡ TEST TYPE  val //	
	if( test_data[0] == 0xFF )
	{
		u8_test_type = 0;
	}
	else
	{
		u8_test_type = i;
	}

	return(u8_test_type);
}


/***************************************************************************************************
* Function Name   : Multi16
* Description     : �������ݳ�16
* Input           ��<strSource> 
* Output          ��<strDest>   
* Return          : 
****************************************************************************************************/
void Multi16(INT8S* strSource, INT8S* strDest)
{
  INT8U i;
  INT16U r,c;
  r = 0;  //��λ
  c = 0;  //�˻�
  
  for( i=0;i<20;i++ )
  {
    c = strSource[i]*16;
    c += r;
    strDest[i] = c%10;
    r = c/10;
  }
}



/***************************************************************************************************
* Function Name   : Multi16
* Description     : �������ݼ�
* Input           ��<strSource> 
* Output          ��<strDest>   
* Return          : 
****************************************************************************************************/
void longadd(INT8S* strSource, INT8S val)
{
  INT8S i;
  INT16U r,c;
  r = val;  //��λ
  c = 0;    //��
  
  for( i=0;i<20;i++ )
  {
    c = strSource[i]+r;
    strSource[i] = c%10;
    r = c/10;
    if( r<=0 ) break;  //û�н�λ��ֹͣ����
  }
}



/*****************************************************************************
** Function name    :void DecToHex_20BYTE(INT8S* strSouce, INT8S* strDest)
** Description      :20λ10����ת����16����           
**
** Parameters       :INT8S* strSouce  ��ת����10���������׵�ַ 20bytes
**                  :INT8S* strDest   ת�����16���������׵�ַ  8bytes                
** Returned value   :NO
**
** Note             :1 ת�����˳����10��������ķ�����
**                  :2 ��10����(strSouce)����16��������16����(strDest)��λ��
**                     �õõ�����ֵ�ظ����ϲ�����
**                  :3 TOKEN����ʱʹ��
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
void DecToHex_20BYTE(INT8S* strSouce, INT8S* strDest)
{
   INT16U i;
   INT16U s = 0;
   INT16U y = 0;
   INT16U sum = 0;
   INT16U k = 0;
   INT16U len;
   INT8U  end = 0;

   INT8S s1[20];
   INT8S s2[20];

   memset(s1,0,20);
   memset(s2,0,20);

   for(i=0;i<20;i++)
   {
       s1[i] = *strSouce;
       strSouce++;
   }
   len=20;

   while(1)
   {    
        end = 0;
        for(i=0; i<len; i++)
        {
            sum = y*10 + s1[i];
            s = sum/16;
            y = sum%16;
            s2[i] = s;
            if(y>0 || s>0) end = 1;
        }

        memcpy(s1,s2,20); //��
        strDest[k] = y;   //����
        y = 0;
        if(!end) break;
        k += 1;
  }

  for(i=0; i<8; i++)  //����λ-�ߵ�ַ,��λ-�͵�ַԭ���ŷ�����
      strDest[i] = strDest[i*2] + (strDest[i*2+1]<< 4 );
  strDest[i] = strDest[16];
}


/*****************************************************************************
** Function name    :void HexToDec_20BYTE(INT8S* strSouce, INT8S* strDest)
** Description      :convert from 66 bit binary data to 20 digit decimal     
**
** Parameters       :INT8S* strSouce  66 bit binary data
**                  :INT8S* strDest   20 digit decimal token               
** Returned value	:NO
**
** Note             :1 ת�����˳����16��������ķ�����
**                  :2 ����TOKENʱʹ��
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
void HexToDec_20BYTE(INT8S* strSouce, INT8S* strDest)
{
  INT8U i;
  INT8U k = 0;
  INT8U len;
  
  INT8S Souce[17];
  INT8S Dest[20];
  INT8S Dest1[20];
  
  memset(Souce, 0, sizeof(Souce));
  memset(Dest, 0, sizeof(Dest));
  memset(Dest1, 0, sizeof(Dest1));
  
  for(i=8;i>0;i--)
  {
      Souce[2*i] = (*strSouce) & 0x0F;
      Souce[2*i-1] = ((*strSouce) & 0xF0)>>4;
      strSouce++;
  }
  Souce[0] = *strSouce;
  len=17;
  
  for(k=0; k<len; k++)
  {
    memcpy(Dest1, Dest, 20);
    Multi16(Dest1, Dest);
    longadd(Dest, Souce[k]);
  }
  for( i=0;i<20;i++ )
    strDest[i] = Dest[19-i];
}


/***************************************************************
*    END
****************************************************************/


//**----------------------------------------------------------------------------
//** V01.01  MJ  2016-09-29
//******************************************************************************/
void api_Reset_APDU_dataOfDefault(void)
{
//	APDU_TID_REC  s_tidRec;

	//������ԿĬ�ϲ�������RAM���ⲿ�洢��	//
	//void *memcpy(void *dest, const void *src, size_t n);//
	memcpy(&gs_APDU_key_var, &cgs_stsKeyParam_default, sizeof(APDU_KEY_EXRAM_VAR));  // ����Ĭ�ϲ��� //
    //���ݱ��� //
//    gs_APDU_key_var.u16_csck = Lib_get_csck_int16u_num(&gs_APDU_key_var.secret_key[0],OFFSET_BLOCK51_CSCK,CHECKWORD); 
//    Lib_Mem_Write(F01_BLOCK51_ADR_START, &gs_APDU_key_var.secret_key[0], LEN_BLOCK51_STSKEY_E2P, TYPE_F01_MAIN);
//    Lib_Mem_Write(F01_BLOCK51_ADR_START +OFFSET_F01_BACK, &gs_APDU_key_var.secret_key[0], LEN_BLOCK51_STSKEY_E2P, TYPE_F01_BACK);     

//	//�����ʷTID��¼ //
//	Lib_Clr_String(&s_tidRec.hex24_tid[0][0],sizeof(APDU_TID_REC));
//	//���ݱ��� //
//	s_tidRec.u16_csck = Lib_get_csck_int16u_num(&s_tidRec.hex24_tid[0][0],OFFSET_BLOCK52_CSCK,CHECKWORD); 
//	Lib_Mem_Write(F01_BLOCK52_ADR_START, &s_tidRec.hex24_tid[0][0], LEN_BLOCK52_TIDREC_E2P, TYPE_F01_MAIN);
//	Lib_Mem_Write(F01_BLOCK52_ADR_START +OFFSET_F01_BACK, &s_tidRec.hex24_tid[0][0], LEN_BLOCK52_TIDREC_E2P, TYPE_F01_BACK);

}




/*****************************************************************************
** Function name	:void api_initialize_APDU_analyze_var(void)
** Description		:���STS���APDU��������Ĺ��̱��� 		 
**
** Parameters		:NO    
**					
** Returned value	:NO
**
** Note 			:
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-29
******************************************************************************/
void api_initialize_APDU_analyze_var(void)
{
	memset(&gs_APDU_analyze_var.apdu_data_hex[0],0,sizeof(APDU_ANALYZE_VAR));
}

/*****************************************************************************
** Function name    :void api_updata_APDU_key_var(void)
** Description      :��ȡAPDU�ӽ����������Կ��ر���             
**
** Parameters       :NO    
**                  
** Returned value	NO
**
** Note             :
**
**----------------------------------------------------------------------------
** V01.01  MJ  2017-11-07
******************************************************************************/
void api_updata_APDU_key_var(void)
{  
	if( KeySTS_OK_flag == 1) 
	{	
		memcpy(&gs_APDU_key_var.secret_key[0], &sts_kay_process_data.secret_key[0], ee_keysts_lenth );
		return;//����ʱ��Կ���³ɹ���ʹ���µ���Կ����������Կ��û����ʽ���浽EEPROM����ֹ������ֵʧ�ܺ���Կ�����ģ�
	}

	if( Verify_para( ee_keysts_page, ee_keysts_inpage, &gs_APDU_key_var.secret_key[0], ee_keysts_lenth ) )
	{
				memcpy(&gs_APDU_key_var.secret_key[0], &cgs_stsKeyParam_default.secret_key[0], ee_keysts_lenth );
	}	
}

/*****************************************************************************
** Function name    :void api_save_APDU_key_var(void)
** Description      :������Կ�������           
**
** Parameters       :NO    
**                  
** Returned value	NO
**
** Note             :
**
**----------------------------------------------------------------------------
** V01.01  MJ  2017-11-07
******************************************************************************/
void api_save_APDU_key_var(void)
{
  WriteE2WithBackup( ee_keysts_page, ee_keysts_inpage, &gs_APDU_key_var.secret_key[0], ee_keysts_lenth );
	ReadE2WithBackup( ee_keysts_page, ee_keysts_inpage, &gs_APDU_key_var.secret_key[0], ee_keysts_lenth );
}


/*****************************************************************************
** Function name    :void api_updata_APDU_TID_Record(INT32U u32_tid_tmp)
** Description      :���20��TID��¼����       
**
** Parameters       :INT32U u32_tid_tmp:������ЧTID���� 
**                  
** Returned value	NO
**
** Note             :��¼˳��ʱ0���Ϊ��С��19���Ϊ���
**
**----------------------------------------------------------------------------
** V01.01  MJ  2017-11-07
******************************************************************************/
void api_updata_APDU_TID_Record(INT32U u32_tid_tmp)
{
//  union  Union_DWordDef tmp_32u;
  union  Union_DWordDef tmp_32u_tid;
  union  Union_DWordDef u32_tidRec01, u32_tidRecMin;
  APDU_TID_REC  s_tidRec;
  INT8U TidCnt,	i;
	INT8U MinTidAres;
  MinTidAres=0;
	tmp_32u_tid.UDWORD = u32_tid_tmp; 
	
	I2C_Read_Eeprom( ee_TID_page, ee_TID_inpage, &s_tidRec.hex24_tid[0][0], ee_TID_lenth );	
	I2C_Read_Eeprom( ee_TIDCNT_page, ee_TIDCNT_inpage, &TidCnt, ee_TIDCNT_lenth );
	
	if( TidCnt>=TID_REC_MAXNUM) TidCnt =TID_REC_MAXNUM;
	else if( TidCnt==0xff) TidCnt =0;
	TidCnt++;
//	Lib_Mem_Read(&s_tidRec.hex24_tid[0][0], F01_BLOCK52_ADR_START, LEN_BLOCK52_TIDREC_E2P, TYPE_F01_MAIN);
	// TID ���¼�¼, 0���ʼ��Ϊ���£�19���ʼ��Ϊ���	//
	// ������λ //
	if( TidCnt>TID_REC_MAXNUM)
	{
	  for(i=0;i<TID_REC_MAXNUM;i++)
	  {
		  u32_tidRec01.UDWORD = 0;
		  u32_tidRec01.BYTE[0] = s_tidRec.hex24_tid[i][0];	//��λ��ǰ	//
		  u32_tidRec01.BYTE[1] = s_tidRec.hex24_tid[i][1];
		  u32_tidRec01.BYTE[2] = s_tidRec.hex24_tid[i][2];		
		  if(u32_tidRecMin.UDWORD > u32_tidRec01.UDWORD)
		  {
			  u32_tidRecMin.UDWORD = 	u32_tidRec01.UDWORD;	//��Сֵ�滻	��ѭ�����ȡ��Сֵ//
			  MinTidAres = i ;
				if(u32_tidRecMin.UDWORD==0)  break;				//=0Ϊ��С�����ڱȶ�����	//
		  }
		}
		s_tidRec.hex24_tid[MinTidAres][0] = tmp_32u_tid.BYTE[0];
	  s_tidRec.hex24_tid[MinTidAres][1] = tmp_32u_tid.BYTE[1];
	  s_tidRec.hex24_tid[MinTidAres][2] = tmp_32u_tid.BYTE[2];	
    
	}
	else
	{
	  for(i=(TID_REC_MAXNUM-1);i>0;i--)
	  {	
		  s_tidRec.hex24_tid[i][0] = s_tidRec.hex24_tid[i-1][0];
		  s_tidRec.hex24_tid[i][1] = s_tidRec.hex24_tid[i-1][1];
		  s_tidRec.hex24_tid[i][2] = s_tidRec.hex24_tid[i-1][2];		
	  }
	  // �������1������ //
	  s_tidRec.hex24_tid[0][0] = tmp_32u_tid.BYTE[0];
	  s_tidRec.hex24_tid[0][1] = tmp_32u_tid.BYTE[1];
  	s_tidRec.hex24_tid[0][2] = tmp_32u_tid.BYTE[2];
	}
	I2C_Write_Eeprom( ee_TID_page, ee_TID_inpage, &s_tidRec.hex24_tid[0][0], ee_TID_lenth );	
  I2C_Write_Eeprom( ee_TIDCNT_page, ee_TIDCNT_inpage, &TidCnt, ee_TIDCNT_lenth );				
}

//�������5��TOKEN����
void api_updata_APDU_TOKEN_Record(unsigned char *Buf)
{
    I2C_Read_Eeprom( ee_TOKEN_page, ee_TOKEN_inpage, &Ic_Comm_TxBuf[10], ee_TOKEN_lenth );
    
	  //�������1������
	  memcpy(Ic_Comm_TxBuf,Buf, 10 );

    I2C_Write_Eeprom( ee_TOKEN_page, ee_TOKEN_inpage, Ic_Comm_TxBuf, ee_TOKEN_lenth );
}


/*****************************************************************************
** Function name    :void api_set_APDU_TID_Record(INT32U u32_tid_tmp)
** Description      :����TID��¼����λͳһ������ֵ      
**
** Parameters       :INT32U u32_tid_tmp:��Ƶ�TID���� 
**                  
** Returned value	NO
**
** Note             :
**
**----------------------------------------------------------------------------
** V01.01  MJ  2017-12-15
******************************************************************************/
void api_set_APDU_TID_Record(INT32U u32_tid_tmp,INT8U mode)
{
 union  Union_DWordDef tmp_32u_tid;
 APDU_TID_REC  s_tidRec;
 INT8U 	i;

	if(mode ==0)	//ȫ������	//
		{
		tmp_32u_tid.UDWORD = (u32_tid_tmp&0X00FFFFFF); 
		for(i=0;i<TID_REC_MAXNUM;i++)
			{	
			s_tidRec.hex24_tid[i][0] = tmp_32u_tid.BYTE[0];
			s_tidRec.hex24_tid[i][1] = tmp_32u_tid.BYTE[1];
			s_tidRec.hex24_tid[i][2] = tmp_32u_tid.BYTE[2]; 	
			}
		}
	else
		{
		//�޸����1��TID��¼��������¼����	//
		tmp_32u_tid.UDWORD = 0; 
		for(i=0;i<TID_REC_MAXNUM;i++)
			{	
			s_tidRec.hex24_tid[i][0] = tmp_32u_tid.BYTE[0];
			s_tidRec.hex24_tid[i][1] = tmp_32u_tid.BYTE[1];
			s_tidRec.hex24_tid[i][2] = tmp_32u_tid.BYTE[2]; 	
			}
		tmp_32u_tid.UDWORD = (u32_tid_tmp&0X00FFFFFF); 
		s_tidRec.hex24_tid[0][0] = tmp_32u_tid.BYTE[0];
		s_tidRec.hex24_tid[0][1] = tmp_32u_tid.BYTE[1];
		s_tidRec.hex24_tid[0][2] = tmp_32u_tid.BYTE[2]; 
		
		}
		I2C_Write_Eeprom( ee_TID_page, ee_TID_inpage, &s_tidRec.hex24_tid[0][0], ee_TID_lenth );
		  
}


/*****************************************************************************
** Function name	:INT8U api_TIDVali_Judge(INT8U *u8ptr_hex_input)
**
** Description		:TOKEN�������TID�ж�      
**
** Parameters		:INT8U *u8ptr_hex_inpu��ת�����APDU�����׵�ַ  
**                  
** Returned value	:INT8U �ɹ�����STS_TOKEN_OK��ʧ�ܷ��ش������
**
** Note             :
**----------------------------------------------------------------------------
** V01.01  MJ  2017-11-06
******************************************************************************/
INT8U api_TIDVali_Judge(INT32U u32_tid_tmp)
{
 union  Union_DWordDef u32_tidRec01, u32_tidRecMin;
 APDU_TID_REC  s_tidRec;
 INT8U 	i;

	
	// ��Կ��Ч���ж�  //
	if((u32_tid_tmp>>16) > gs_APDU_key_var.ken )
	{
		return (STS_TOKEN_REJRCT_6);	//TID������Կ��Ч��,rEJEct6//		
	}	


	//��ȡ�����Ч��20��TID��¼/
//	Lib_Mem_Read(&s_tidRec.hex24_tid[0][0], F01_BLOCK52_ADR_START, LEN_BLOCK52_TIDREC_E2P, TYPE_F01_MAIN);
  I2C_Read_Eeprom( ee_TID_page, ee_TID_inpage, &s_tidRec.hex24_tid[0][0], ee_TID_lenth );
	
	//���ж��Ƿ���ʹ�ù�	//
	for(i=0;i<TID_REC_MAXNUM;i++)
	{
		u32_tidRec01.UDWORD = 0;
		u32_tidRec01.BYTE[0] = s_tidRec.hex24_tid[i][0];	//��λ��ǰ	//
		u32_tidRec01.BYTE[1] = s_tidRec.hex24_tid[i][1];
		u32_tidRec01.BYTE[2] = s_tidRec.hex24_tid[i][2];		
		if(u32_tid_tmp == u32_tidRec01.UDWORD)
		{
			return (STS_TOKEN_REJRCT_USED);	// TID �ѽ�ʹ�ù�// 	
		}
	}

/////////////////////////////////////////////////////////////////////////////////
// �ڻ�ȡ20����¼��TIDֵ��С�����ݣ���ǰTIDֵС����СTIDֵʱ����ʾ����
/////////////////////////////////////////////////////////////////////////////////
	//��ȡ��¼��TIDֵ��С��һ��//
	u32_tidRecMin.UDWORD =0x00FFFFFF;
	for(i=0;i<TID_REC_MAXNUM;i++)
	{
		u32_tidRec01.UDWORD = 0;
		u32_tidRec01.BYTE[0] = s_tidRec.hex24_tid[i][0];	//��λ��ǰ	//
		u32_tidRec01.BYTE[1] = s_tidRec.hex24_tid[i][1];
		u32_tidRec01.BYTE[2] = s_tidRec.hex24_tid[i][2];		
		if(u32_tidRecMin.UDWORD > u32_tidRec01.UDWORD)
		{
			u32_tidRecMin.UDWORD = 	u32_tidRec01.UDWORD;	//��Сֵ�滻	��ѭ�����ȡ��Сֵ//
			if(u32_tidRecMin.UDWORD==0)  break;				//=0Ϊ��С�����ڱȶ�����	//
		}
	}
	
	if((u32_tid_tmp < u32_tidRecMin.UDWORD)&&(u32_tidRecMin.UDWORD !=0x00FFFFFF))
	{
		return (STS_TOKEN_REJRCT_OLD);	// TID ����//		
	}

	return (STS_TOKEN_OK);	
}



/*****************************************************************************
** Function name	:INT8U api_TIDVali_Judge(INT8U *u8ptr_hex_input)
**
** Description		:TOKEN�������TID�ж�      
**
** Parameters		:INT8U *u8ptr_hex_inpu��ת�����APDU�����׵�ַ  
**                  
** Returned value	:INT8U �ɹ�����STS_TOKEN_OK��ʧ�ܷ��ش������
**
** Note             :
**----------------------------------------------------------------------------
** V01.01  MJ  2017-11-06
******************************************************************************/
void api_Change_Key(void)
{
 	Clear_Wdt();
  memset(&sts_kay_process_data.secret_key[0],0,ee_keysts_lenth);
	//��ǰ��Կ���ݽṹ�����  //
	memcpy(&gs_APDU_key_var.secret_key[0], &sts_kay_change_data.kay[0],8); //��Կ���� //	
	gs_APDU_key_var.ken = sts_kay_change_data.ken;		//��Կ��Ч�� //	
	gs_APDU_key_var.kt = sts_kay_change_data.kt;		//��Կ����, 2bit ,//
	gs_APDU_key_var.krn = sts_kay_change_data.krn;		//��Կ�汾�� //
	gs_APDU_key_var.ti = sts_kay_change_data.ti;		// ����������//		

 	memcpy(&sts_kay_process_data.secret_key[0],&gs_APDU_key_var.secret_key[0] ,ee_keysts_lenth);
	//������Կ���� //
	if(sts_kay_change_data.ro == 1)
	{
		api_set_APDU_TID_Record(0,0);	//���������ʷTID��¼ // 
	}
	
	memset(&sts_kay_change_data.kay[0],0,sizeof(STS_KAY_CHANHE_DATA));
}


/*****************************************************************************
** Function name    :INT8U api_APDU_set_power_limit(void)
** Description      :set up Max Power Limit Mode       
**
** Parameters       :NONE
**                  
** Returned value   :INT8U ,���������=0����Ϸ���=�����������������
**
** Note             :1 ������������Դȫ�ֱ���gs_APDU_analyze_var.apdu_data_hex[8]
**                  :2 
**                  :3 
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
INT8U api_APDU_set_power_limit(void)
{
 INT8U u8_result;
 union Union_DWordDef tmp_32u_prod;
 
	// TID�����ж��ж� //
	gs_APDU_analyze_var.u32_TID = Get_TID(&gs_APDU_analyze_var.apdu_data_hex[0]);
	u8_result = api_TIDVali_Judge(gs_APDU_analyze_var.u32_TID);
	if(u8_result != STS_TOKEN_OK)
	{
		return (u8_result);
	}

	//���ݻ�ȡ//
	tmp_32u_prod.UDWORD = CalcTransferAmt(&gs_APDU_analyze_var.apdu_data_hex[2]);	 //����0.001kW //
	tmp_32u_prod.UDWORD *=10;		  // ��С��������0.0001kW// 
	gs_APDU_analyze_var.u32_PropData = tmp_32u_prod.UDWORD;
	//���ݱ���  //
//	Lib_Mem_DBWrite(ADR_CREDITPARAM1_poweLimit,&tmp_32u_prod.BYTE[0],4,MEM_RN_NVM);

	//���书����ֵ�޸ĺ�Ĺ�����ֵ�жϴ�����  //

	//���سɹ����//     
	return (STS_TOKEN_OK);	      
}

/*****************************************************************************
** Function name    :INT8U api_APDU_clear_balance(void)
** Description      :To clear balance(������)// Ŀǰ��Ϊ�����������������������//    
**
** Parameters       :NONE
**                  
** Returned value   :INT8U ,���������=0����Ϸ���=�����������������
**
** Note             :1 ������������Դȫ�ֱ���gs_APDU_analyze_var.apdu_data_hex[8]
**                  :2 ��������Ƿ�Ҫ�����õ���?
**                  :3 
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
INT8U api_APDU_clear_balance(void)
{
 INT8U u8_result;
 union Union_DWordDef tmp_32u_prod;
	
	// TID�����ж��ж� //
	gs_APDU_analyze_var.u32_TID = Get_TID(&gs_APDU_analyze_var.apdu_data_hex[0]);
	u8_result = api_TIDVali_Judge(gs_APDU_analyze_var.u32_TID);
	if(u8_result != STS_TOKEN_OK)
	{
		return (u8_result);
	}

	//���ݻ�ȡ//
	tmp_32u_prod.UDWORD = 0;    //�������ݼĴ���	//
	tmp_32u_prod.BYTE[0] = gs_APDU_analyze_var.apdu_data_hex[2];
	tmp_32u_prod.BYTE[1] = gs_APDU_analyze_var.apdu_data_hex[3];
	gs_APDU_analyze_var.u32_PropData = tmp_32u_prod.UDWORD;
	
	//���ݷ�Χ�ж�  //
	if((tmp_32u_prod.UDWORD !=0) &&(tmp_32u_prod.UDWORD !=0XFFFF))   //=0��������Ĵ�����=FFFFΪ�����мĴ���  //
	{
	return (STS_TOKEN_REJRCT_4);    
	}

	//��������ؼĴ�������  //
//	api_clr_credit();

	//���سɹ����//	  
	return(STS_TOKEN_OK);   	     
}


/*****************************************************************************
** Function name    :INT8U api_APDU_set_key_frist(void)
** Description      :key change Token -1  (�޸���Կ��1��TOKEN)
**
** Parameters       :NONE
**                  
** Returned value   :INT8U ,���������=0����Ϸ���=�����������������
**
** Note             :1 ������������Դȫ�ֱ���gs_APDU_analyze_var.apdu_data_hex[8]
**                  :2 �മTOKEN����ʱ�������ݼ��С��2����
**                  :3 ��Ҫ����˳������TOKEN��
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
INT8U api_APDU_set_key_frist(void)
{  
	sts_kay_change_data.flg &= ~KeyChangeHO;
	sts_kay_change_data.kay[4] = gs_APDU_analyze_var.apdu_data_hex[2];
	sts_kay_change_data.kay[5] = gs_APDU_analyze_var.apdu_data_hex[3];
	sts_kay_change_data.kay[6] = gs_APDU_analyze_var.apdu_data_hex[4];
	sts_kay_change_data.kay[7] = gs_APDU_analyze_var.apdu_data_hex[5];
	sts_kay_change_data.kt = (gs_APDU_analyze_var.apdu_data_hex[6]&0x03); //key type
	sts_kay_change_data.ro = (gs_APDU_analyze_var.apdu_data_hex[6] & 0x08) >>3;	// 
	sts_kay_change_data.krn = (gs_APDU_analyze_var.apdu_data_hex[6] & 0xF0) >>4; //key revision number
	sts_kay_change_data.ken |= ((gs_APDU_analyze_var.apdu_data_hex[7] & 0x0F)<<4);//key expire H
	
	sts_kay_change_data.flg |= KeyChangeHO;

/* �������ԣ����ֵ����жϻ������2���ͺ���¼��ĵ�1������������кܴ���գ����Ǳ���ԭ����2�������պ���ж�
	//����STS ����Ҫ�󣬵���ʱ����Ҫ�ж�	,���Ǿ������Է��ֵ���������ȷ������£����������Կ���档//
	if(sts_kay_change_data.kt == 3)   //��Կ���Ͳ��ܸ�Ϊ3  ,DCTK3ֻ����ΪIC��������Կ��������Ϊ���̱���Կ        //
	{
		//Lib_Clr_String(&sts_kay_change_data.kay[0],sizeof(STS_KAY_CHANHE_DATA));
		return (STS_TOKEN_REJRCT_5);
	}
	
	if(sts_kay_change_data.kt == 0)//�������Կ���ǳ�ʼ��������Կ��������Կ���ܸ�Ϊ��ʼ��������Կ
	{
		if((gs_APDU_key_var.kt &0x0f) != 0)
		{
			//Lib_Clr_String(&sts_kay_change_data.kay[0],sizeof(STS_KAY_CHANHE_DATA));
			return (STS_TOKEN_REJRCT_5);
		}
	}
*/
	//�������պ��ڳ����ж�һ��	//
	if((sts_kay_change_data.flg & 0x03)== (KeyChangeHO+ KeyChangeLO))	//64bits��Կ��λ,��MeterKey(��Կ2�����úã�
	{	//��ʾ��Կ��ȫ�޸����  //
//		if(sts_kay_change_data.kt == 3)   //��Կ���Ͳ��ܸ�Ϊ3  ,DCTK3ֻ����ΪIC��������Կ��������Ϊ���̱���Կ        //
//		{
//			memset(&sts_kay_change_data.kay[0],0,sizeof(STS_KAY_CHANHE_DATA));
//			return (STS_TOKEN_REJRCT_5);
//		}
//		else 
		if(sts_kay_change_data.kt == 0)//�������Կ���ǳ�ʼ��������Կ��������Կ���ܸ�Ϊ��ʼ��������Կ
		{
			if((gs_APDU_key_var.kt &0x0f) != 0)
			{
				memset(&sts_kay_change_data.kay[0],0,sizeof(STS_KAY_CHANHE_DATA));
				return (STS_TOKEN_REJRCT_5);
			}
		}
		api_Change_Key();
		//��Կ�޸ĺ��������	//
		memset(&sts_kay_change_data.kay[0],0,sizeof(STS_KAY_CHANHE_DATA));
	}
	else
	{
		//��ʾ�ȴ����յ�2����Կ//
		return (STS_TOKEN_CHG_KEY_PLS_2nd);
	}
	
    return (STS_TOKEN_OK);	
}

/*****************************************************************************
** Function name    :INT8U api_APDU_set_key_second(void)
** Description      :key change Token -2  (�޸���Կ��2��TOKEN)
**
** Parameters       :NONE
**                  
** Returned value   :INT8U ,���������=0����Ϸ���=�����������������
**
** Note             :1 ������������Դȫ�ֱ���gs_APDU_analyze_var.apdu_data_hex[8]
**                  :2 �മTOKEN����ʱ�������ݼ��С��2����
**                  :3 ��Ҫ����˳������TOKEN��
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
INT8U api_APDU_set_key_second(void)
{
	sts_kay_change_data.flg &= ~KeyChangeLO;
	sts_kay_change_data.kay[0] = gs_APDU_analyze_var.apdu_data_hex[2];
	sts_kay_change_data.kay[1] = gs_APDU_analyze_var.apdu_data_hex[3];
	sts_kay_change_data.kay[2] = gs_APDU_analyze_var.apdu_data_hex[4];
	sts_kay_change_data.kay[3] = gs_APDU_analyze_var.apdu_data_hex[5];
	sts_kay_change_data.ti = gs_APDU_analyze_var.apdu_data_hex[6]; //triff index
	sts_kay_change_data.ken |= (gs_APDU_analyze_var.apdu_data_hex[7] & 0x0F);//key expire L
	sts_kay_change_data.flg |= KeyChangeLO;

	//����STS ����Ҫ�󣬵���ʱ����Ҫ�ж�	��������Կ��2���޹ؼ��ж�����//

	if((sts_kay_change_data.flg & 0x03)== (KeyChangeHO+ KeyChangeLO))	//64bits��Կ��λ,��MeterKey(��Կ2�����úã�
	{	//��ʾ��Կ��ȫ�޸����  //
//		if(sts_kay_change_data.kt == 3)   //��Կ���Ͳ��ܸ�Ϊ3  ,DCTK3ֻ����ΪIC��������Կ��������Ϊ���̱���Կ        //
//		{
//			memset(&sts_kay_change_data.kay[0],0,sizeof(STS_KAY_CHANHE_DATA));
//			return (STS_TOKEN_REJRCT_5);
//		}
//		else 
		if(sts_kay_change_data.kt == 0)//�������Կ���ǳ�ʼ��������Կ��������Կ���ܸ�Ϊ��ʼ��������Կ
		{
			if((gs_APDU_key_var.kt &0x0f) != 0)
			{
				memset(&sts_kay_change_data.kay[0],0,sizeof(STS_KAY_CHANHE_DATA));
				return (STS_TOKEN_REJRCT_5);
			}
		}
		api_Change_Key();
		//��Կ�޸ĺ��������	//
		memset(&sts_kay_change_data.kay[0],0,sizeof(STS_KAY_CHANHE_DATA));
	}
	else
	{
		//��ʾ�ȴ����յ�1����Կ//
		return (STS_TOKEN_CHG_KEY_PLS_1st);
	}
	
    //���سɹ����//     
    return (STS_TOKEN_OK);	  
}

/*****************************************************************************
** Function name    :INT8U api_APDU_clear_even(void)
** Description      :To clear event
**
** Parameters       :NONE
**                  
** Returned value   :INT8U ,���������=0����Ϸ���=�����������������
**
** Note             :1 ������������Դȫ�ֱ���gs_APDU_analyze_var.apdu_data_hex[8]
**                  :2 ����¼���¼ע��̵���״̬
**                  :3 
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
INT8U api_APDU_clear_even(void)
{
 INT8U u8_result;
 union Union_DWordDef tmp_32u_prod;
 
	// TID�����ж��ж� //
	gs_APDU_analyze_var.u32_TID = Get_TID(&gs_APDU_analyze_var.apdu_data_hex[0]);
	u8_result = api_TIDVali_Judge(gs_APDU_analyze_var.u32_TID);
	if(u8_result != STS_TOKEN_OK)
	{
		return (u8_result);
	}

	//���ݻ�ȡ//
	tmp_32u_prod.UDWORD = 0;	//�������ݼĴ���  //
	tmp_32u_prod.BYTE[0] = gs_APDU_analyze_var.apdu_data_hex[2];
	tmp_32u_prod.BYTE[1] = gs_APDU_analyze_var.apdu_data_hex[3];
	gs_APDU_analyze_var.u32_PropData = tmp_32u_prod.UDWORD;

	//���ݷ�Χ�ж�  //
	if(tmp_32u_prod.UDWORD !=0)   //=0  //
	{
		return (STS_TOKEN_REJRCT_4);	
	}

	gs_APDU_analyze_var.u32_PropData = tmp_32u_prod.UDWORD;
	//�Ե��¼����  //
//	api_Event_ram_INIT();

	//���سɹ����//     
	return (STS_TOKEN_OK);	 
}


/*****************************************************************************
** Function name    :INT8U sts_process_apdu_00_purchase(INT8U *u8ptr_hex_input)
** Description      :TOKEN��ֵ����             
**
** Parameters       :INT8U *ptr_kay_data  token�������HEX�������׵�ַ   
**                  
** Returned value	:INT8U ���ش�����
**
** Note             :����APDU��˵���ļ����н�������
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
INT8U sts_process_apdu_00_purchase(INT8U *u8ptr_hex_input)
{
    INT16U crc_tmp;
//    union Union_DWordDef tmp_32u;
    union Union_DWordDef tmp_32u_buy;
//    union Union_DWordDef tmp_u32_creditMAX; 
//    INT32S tmp_s32_credit;
    INT8U 	u8_result;

//    INT8U  amount_add_fg;	
   
    api_updata_APDU_key_var();  //��ȡ��Ʒ��ǰ��Կ //
//	 memset(&gs_APDU_key_var.secret_key[0], 1, ee_keysts_lenth );  
 	// ��Կ������ȡ���ж�  // 
 	///*  �������������жϣ�����ע��STSЭ����֤����Ҫ���ж�  MJ20171210
//	 if((gs_APDU_key_var.kt == 1) ||(sts_kay_change_data.kt == 3))   //This key type can't use credit token
//	 {
//		 return(STS_TOKEN_REJRCT_3);  //DDTK��Կ����֧�ֳ�ֵTOKEN//	
//	 }
	//*/
	
	//��64bit���ܲ������sts_analyzing_unit.apdu_data_hex[0]������// 
	STA(u8ptr_hex_input,&gs_APDU_analyze_var.apdu_data_hex[0],&gs_APDU_key_var.secret_key[0],0); 
	gs_APDU_analyze_var.apdu_data_hex[8] = (*(u8ptr_hex_input+8));
	
	// CRC16 ������֤ // 
	crc_tmp = checkSTS_crc(&gs_APDU_analyze_var.apdu_data_hex[8],7);      //�����ǽ��ܺ��64λ�ĸ�48λ����ǰ����Ŀ����//
	if(((crc_tmp & 0x00ff) != gs_APDU_analyze_var.apdu_data_hex[0] ) || (((crc_tmp & 0xff00)>>8) != gs_APDU_analyze_var.apdu_data_hex[1])) 
	{
		return(STS_TOKEN_REJRCT_1);	//����ʧ�ܣ���������Կ����//			
	}
	//  ��ȡ �����//
	gs_APDU_analyze_var.sub_class = (gs_APDU_analyze_var.apdu_data_hex[7]& 0xf0)>>4;
	if((gs_APDU_analyze_var.sub_class != STS0_SUB_CLASS_PURCHASE_KWH )&&(gs_APDU_analyze_var.sub_class != STS0_SUB_CLASS_PURCHASE_MONEY ) )
	{
		return(STS_TOKEN_REJRCT_2);	//�����ʹ��󣬲�֧�ֵ�TOKEN����//		
	}

	// TID�����ж��ж� //
	gs_APDU_analyze_var.u32_TID = Get_TID(&gs_APDU_analyze_var.apdu_data_hex[0]);
	u8_result = api_TIDVali_Judge(gs_APDU_analyze_var.u32_TID);
 	if(u8_result != STS_TOKEN_OK)
 	{
 		return(u8_result);
 	}

	//��ȡ��ֵ���  //
//	tmp_32u_buy.UDWORD = CalcTransferAmt(&gs_APDU_analyze_var.apdu_data_hex[2]);	//����0.1kWh //
	tmp_32u_buy.UDWORD = 0;	
	tmp_32u_buy.BYTE[0]=gs_APDU_analyze_var.apdu_data_hex[2];
	tmp_32u_buy.BYTE[1]=gs_APDU_analyze_var.apdu_data_hex[3];
	Hex2Bcd( tmp_32u_buy.BYTE, Buy_Money_Mac[0].B08, 4 );
//	tmp_32u_buy.UDWORD *=10;		 // ��С��������0.01KWH // 
//  memcpy( Buy_Money_Mac[0].B08+0, tmp_32u_buy.BYTE, 4 );

//	//��ֵ��Ķڻ������жϣ������ڻ������������ֵ  //
////    Lib_Mem_Read(&tmp_u32_creditMAX.BYTE[0], ADR_CREDITPARAM1_creditMAX, 4, TYPE_F01_MAIN);
////	tmp_s32_credit = api_getCredit();  // ��ȡ��ǰʣ����� //
//	if(tmp_u32_creditMAX.DWORD < (tmp_32u_buy.DWORD + tmp_s32_credit))
//	{
// 		return(STS_TOKEN_REJRCT_OVER_CRD);  //��ֵ������ȷ���ǳ����ڻ�����//		
//	}

//	//������  //	
//	gs_APDU_analyze_var.u32_PropData = tmp_32u_buy.UDWORD; 
//	api_add_credit(gs_APDU_analyze_var.u32_PropData);
	
	// ��������ɹ�������TID���� //
	api_updata_APDU_TID_Record(gs_APDU_analyze_var.u32_TID);
	
	
	api_updata_APDU_TOKEN_Record(gs_APDU_analyze_var.apdu_data_bcd);
  
	// �������¼���¼ //
	return(STS_TOKEN_OK);
}



/*****************************************************************************
** Function name    :INT8U sts_process_apdu_01_test(INT8U *u8ptr_hex_input)
** Description      :TOKEN ���Դ���           
**
** Parameters       :INT8U *ptr_kay_data  token�������HEX�������׵�ַ   
**                  
** Returned value	:INT8U ���ش�����
**
** Note             :����APDU��˵���ļ����н�������
**                  :����TOKENΪ���ģ��޼��ܴ���
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
INT8U sts_process_apdu_01_test(INT8U *u8ptr_hex_input)
{
 INT16U crc_tmp;
 INT8U u8_test_type;
// INT8U LCD_DIS = 0;

    // ��ȡ16��������֡  �������//
	memcpy(&gs_APDU_analyze_var.apdu_data_hex[0], u8ptr_hex_input,9);
	gs_APDU_analyze_var.apdu_data_hex[8] = (*(u8ptr_hex_input+8)); 

	
	// CRC16 ������֤ // 
	crc_tmp = checkSTS_crc(&gs_APDU_analyze_var.apdu_data_hex[8],7);      //�����ǽ��ܺ��64λ�ĸ�48λ����ǰ����Ŀ����//
	if(((crc_tmp & 0x00ff) != gs_APDU_analyze_var.apdu_data_hex[0] ) || (((crc_tmp & 0xff00)>>8) != gs_APDU_analyze_var.apdu_data_hex[1])) 
	{
		return (STS_TOKEN_REJRCT_1);	//����ʧ�ܻ����ģʽCRCУ��ʧ�ܣ���������Կ����//			
	}	

	//  ��ȡ �����//
	gs_APDU_analyze_var.sub_class = (gs_APDU_analyze_var.apdu_data_hex[7]& 0xf0)>>4;
	if(gs_APDU_analyze_var.sub_class != 0)
		{
		return (STS_TOKEN_REJRCT_4);	//���ܲ�֧�ִ��� //			
		}

    //��ȡ���Ժű���  //
    u8_test_type = get_token_test_type(&gs_APDU_analyze_var.apdu_data_hex[3]);

	// 36bit ���������ݻ�ȡ   //
	switch (u8_test_type)
	{
		case STS_TEST_ALL:   // test  all //
		    ///////////////////////////////////////////
            // CIU ��ʾ mj 2017-12-07 //
            ///////////////////////////////////////////
// 33           api_deal_relay_of_token_relay_test();	//�̵���������բ	//
            gs_APDU_analyze_var.u32_PropData = u8_test_type;
            //Turn_Dis_Mode_To_TOKEN(0,STS_TEST_ALL);	
			break;
		case STS_TEST_RELAY:   // test realy //
		    ///////////////////////////////////////////
            //CIU ��ʾ mj 2017-12-07 //
            ///////////////////////////////////////////
//   33         api_deal_relay_of_token_relay_test();	//�̵���������բ	//
			gs_APDU_analyze_var.u32_PropData = u8_test_type;
			break;
			
		case STS_TEST_LCD:   // test  lcd dispaly //
            ///////////////////////////////////////////
            //  CIU ��ʾ mj 2017-12-07 //
            ///////////////////////////////////////////
            gs_APDU_analyze_var.u32_PropData = u8_test_type;
			break;
		case STS_TEST_TOTAL_ENERGY:   // display total energy  //
            ///////////////////////////////////////////
            // CIU ��ʾ mj 2017-12-07 //
            ///////////////////////////////////////////		
            gs_APDU_analyze_var.u32_PropData = u8_test_type;
			break;
			
		case STS_TEST_KRN:   //Display KRN
			///////////////////////////////////////////
			// // CIU ��ʾ mj 2017-12-07 //
			/////////////////////////////////////////// 	
			gs_APDU_analyze_var.u32_PropData = u8_test_type;	
			break;
		case STS_TEST_TI:	 //display TI
			///////////////////////////////////////////
			// // CIU ��ʾ mj 2017-12-07 //
			/////////////////////////////////////////// 	
			gs_APDU_analyze_var.u32_PropData = u8_test_type;	
			break;
		case STS_TEST_MPL:   // display test max power limit //
            ///////////////////////////////////////////
            // // CIU ��ʾ mj 2017-12-07 //
            ///////////////////////////////////////////	
            gs_APDU_analyze_var.u32_PropData = u8_test_type;
			break;
		/*  �����Ե���Բ���  MJ20171218
		case STS_TEST_TAMPER_STATUS:   //display tamper status //
            ///////////////////////////////////////////
            // �����Ե�״̬����ʾ���ԵĹ��ܴ��� mj 2016-9-28 //
            ///////////////////////////////////////////	
            gs_APDU_analyze_var.u32_PropData = u8_test_type;
			break;
			*/
		case STS_TEST_CURRENT_POWER:   // display current power//
            ///////////////////////////////////////////
            // // CIU ��ʾ mj 2017-12-07 //
            ///////////////////////////////////////////	
            gs_APDU_analyze_var.u32_PropData = u8_test_type;
			break;

		case STS_TEST_METER_VERSION:   // display meter version number//
			///////////////////////////////////////////
			// CIU ��ʾ mj 2017-12-07 //
			///////////////////////////////////////////	
			gs_APDU_analyze_var.u32_PropData = u8_test_type;
			break;
			
		default:
			return (STS_TOKEN_REJRCT_4);	//���ܲ�֧�ִ��� //			
	}

	return (STS_TOKEN_OK);
}


/*****************************************************************************
** Function name    :INT8U sts_process_apdu_02_set(INT8U *u8ptr_hex_input)
** Description      :TOKEN ��SUB_class=2������APDU����          
**
** Parameters       :INT8U *ptr_kay_data  token�������HEX�������׵�ַ   
**                  
** Returned value	:INT8U ���ش�����
**
** Note             :����APDU��˵���ļ����н�������
**                  :
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
INT8U sts_process_apdu_02_set(INT8U *u8ptr_hex_input)
{
 INT16U crc_tmp;
 INT8U u8_result;
// union  Union_DWordDef tmp_32u_mpl,tmp_bcd4_mpl;

    //��ȡ��Ʒ��ǰ��Կ //
  api_updata_APDU_key_var();
 
	//��64bit���ܲ������sts_analyzing_unit.apdu_data_hex[0]������// 
	STA(u8ptr_hex_input,&gs_APDU_analyze_var.apdu_data_hex[0],&gs_APDU_key_var.secret_key[0],0); 
	gs_APDU_analyze_var.apdu_data_hex[8] = (*(u8ptr_hex_input+8));
	
	// CRC16 ������֤ // 
	crc_tmp = checkSTS_crc(&gs_APDU_analyze_var.apdu_data_hex[8],7);      //�����ǽ��ܺ��64λ�ĸ�48λ����ǰ����Ŀ����//
	if(((crc_tmp & 0x00ff) != gs_APDU_analyze_var.apdu_data_hex[0] ) || (((crc_tmp & 0xff00)>>8) != gs_APDU_analyze_var.apdu_data_hex[1])) 
	{
		return (STS_TOKEN_REJRCT_1);	//crc����,������Կ���ܺ˶�ʧ��//
	}
	
	//  ��ȡ �����//
	gs_APDU_analyze_var.sub_class = (gs_APDU_analyze_var.apdu_data_hex[7]& 0xf0)>>4;
	switch (gs_APDU_analyze_var.sub_class)
	{
		case STS2_SUB_CLASS_SET_MAXPOWER:  //set up Max Power Limit Mode Token		
            u8_result = api_APDU_set_power_limit();
			if(u8_result ==STS_TOKEN_OK )  api_updata_APDU_TID_Record(gs_APDU_analyze_var.u32_TID);
			break;
			
		case STS2_SUB_CLASS_CLR_BALANCE://To clear balance(������)//
            u8_result = api_APDU_clear_balance();
			if(u8_result ==STS_TOKEN_OK )  api_updata_APDU_TID_Record(gs_APDU_analyze_var.u32_TID);
			break;

		case STS2_SUB_CLASS_SET_SECRET_KEY_H://key change Token -1
            u8_result = api_APDU_set_key_frist();
			break;

		case STS2_SUB_CLASS_SET_SECRET_KEY_L://key change Token -2
            u8_result = api_APDU_set_key_second();
			break;
		
/* STS ��֤ʱΪ�˲���˳��ͨ���������Ե��鹦��  MJ20171218
		case STS2_SUB_CLASS_CLR_TAMPER_EVEN://To clear event
            u8_result = api_APDU_clear_even();
			if(u8_result ==STS_TOKEN_OK )  api_updata_APDU_TID_Record(gs_APDU_analyze_var.u32_TID);
			break;
*/
		default:
			return (STS_TOKEN_REJRCT_2);	// �������ݴ��� //			
	}
 /////////////////������ʾ/////////////////////////////


////////////////////////////////////////////////
	return (u8_result);
}


/*****************************************************************************
** Function name	:void api_initialize_APDU_when_reset(void)
** Description		:��ʼ��STS���APDU���������ģ���ڵ���ر���������ϵͳ��λ��ĵ��� 
**
** Parameters		:NO    
**					
** Returned value	:NO
**
** Note 			:
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-29
******************************************************************************/
void api_initialize_APDU_when_reset(void)
{
	//���STS APDU ����ȫ�ֱ���  //
	memset(&gs_APDU_analyze_var.apdu_data_hex[0],0,sizeof(APDU_ANALYZE_VAR));
	//���STS APDU �����޸ĶമTOKEN����ȫ�ֱ���  //
	memset(&sts_kay_change_data.kay[0],0,sizeof(STS_KAY_CHANHE_DATA));
	//��EXRAM�ڵ���Կ������ݵ���RAM��ȫ�ֱ�����  //
	api_updata_APDU_key_var();
}
//INT8U api_Handle_tokenjudge_code(INT8U *ptr_kay_data)
//{
// INT8U i;
// INT8U tmp_bcd_byte[10];
// INT8U tmp_dec_byte[20];
// INT8U tmp_hex_byte[17];
// INT16U crc_tmp;

//	//0 ���STS APDU ����ȫ�ֱ�������  //
//	api_initialize_APDU_analyze_var();
//	gs_APDU_analyze_var.u8_result = STS_TOKEN_OK;
//	
//    // 1 ���̲��������20���ַ�����ϳ�10���ֽڵ�BCD��  �ݴ�//
//	for( i=0; i<10; i++ ) 
//	{
//		gs_APDU_analyze_var.apdu_data_bcd[9-i] = (*(ptr_kay_data+(i*2))<<4);
//		gs_APDU_analyze_var.apdu_data_bcd[9-i] += (*(ptr_kay_data+(i*2+1)));
//	}
//	
//    // 2 �����̲��������20��10��������ת��Ϊ9�ֽڵ�HEX������  //
//    memset(&tmp_hex_byte[0],0,17);
//	  memcpy(&tmp_dec_byte[0], ptr_kay_data, 20); 
//    
//	  DecToHex_20BYTE((INT8S*)(&tmp_dec_byte[0]), (INT8S*)(&tmp_hex_byte[0])); // ��ת�����66λ�����ư���λ-�ߵ�ַ,��λ-�͵�ַ�������HEX[] //

//    // 3 ��ȡ�����ܵ� [���]����  ������֧����//
//    gs_APDU_analyze_var.main_class = Class_get(&tmp_hex_byte[0]);
//		switch (gs_APDU_analyze_var.main_class)
//    {
//		case STS_MAIN_CLASS_PURCHASE:  	// = 0,purchase  apdu//
//			if((gs_APDU_key_var.kt == 1) ||(sts_kay_change_data.kt == 3))   //This key type can't use credit token
//	    {
//		       return(STS_TOKEN_REJRCT_3);  //DDTK��Կ����֧�ֳ�ֵTOKEN//	
//	    }
//			break;
//			
//		case STS_MAIN_CLASS_TEST:	// = 1, test  apdu//
//			gs_APDU_analyze_var.u8_result = sts_process_apdu_01_test(&tmp_hex_byte[0]);
//			break;
//			
//		case STS_MAIN_CLASS_SET:		// =2, meter set apdu //
//			gs_APDU_analyze_var.u8_result = sts_process_apdu_02_set(&tmp_hex_byte[0]);
//			break;
//			
//		default:
//			gs_APDU_analyze_var.u8_result = STS_TOKEN_REJRCT_2; //���ʹ���  //	
//		break;    	
//    }
//}
/*****************************************************************************
** Function name    :void api_Handle_key_sts_code(INT8U *ptr_kay_data)
** Description      :���ռ�������Ļ������ݽ��д���               
**
** Parameters       :INT8U *ptr_kay_data  �����������ݻ������׵�ַ,20�ֽ�BCD������     
**                  
** Returned value	:INT8U ,�����������ݽ������;
**
** Note             :����APDU��˵���ļ����н�������
**----------------------------------------------------------------------------
** V01.01  MJ  2017-11-21
******************************************************************************/
INT8U api_Handle_key_sts_code(INT8U *ptr_kay_data)
{
 INT8U i;
// INT8U tmp_bcd_byte[10];
 INT8U tmp_dec_byte[20];
 INT8U tmp_hex_byte[17];
// INT16U crc_tmp;

	//0 ���STS APDU ����ȫ�ֱ�������  //
	api_initialize_APDU_analyze_var();
	gs_APDU_analyze_var.u8_result = STS_TOKEN_OK;
	Clear_Wdt();
    // 1 ���̲��������20���ַ�����ϳ�10���ֽڵ�BCD��  �ݴ�//
	for( i=0; i<10; i++ ) 
	{
		gs_APDU_analyze_var.apdu_data_bcd[9-i] = (*(ptr_kay_data+(i*2)))<<4;
		gs_APDU_analyze_var.apdu_data_bcd[9-i] += (*(ptr_kay_data+(i*2+1)));
	}
	
    // 2 �����̲��������20��10��������ת��Ϊ9�ֽڵ�HEX������  //
    memset(&tmp_hex_byte[0],0,17);
	  memset(&tmp_dec_byte[0],0,20);


	  memcpy(&tmp_dec_byte[0], ptr_kay_data, 20); 
    Clear_Wdt();
	
	  
	  DecToHex_20BYTE((INT8S*)(&tmp_dec_byte[0]), (INT8S*)(&tmp_hex_byte[0])); // ��ת�����66λ�����ư���λ-�ߵ�ַ,��λ-�͵�ַ�������HEX[] //

    // 3 ��ȡ�����ܵ� [���]����  ������֧����//
    gs_APDU_analyze_var.main_class = Class_get(&tmp_hex_byte[0]);

    switch (gs_APDU_analyze_var.main_class)
    {
		case STS_MAIN_CLASS_PURCHASE:  	// = 0,purchase  apdu//
			gs_APDU_analyze_var.u8_result = sts_process_apdu_00_purchase(&tmp_hex_byte[0]);
			break;
			
		case STS_MAIN_CLASS_TEST:	// = 1, test  apdu//
			gs_APDU_analyze_var.u8_result = sts_process_apdu_01_test(&tmp_hex_byte[0]);
			break;
			
		case STS_MAIN_CLASS_SET:		// =2, meter set apdu //
			gs_APDU_analyze_var.u8_result = sts_process_apdu_02_set(&tmp_hex_byte[0]);
			break;
			
		default:
			gs_APDU_analyze_var.u8_result = STS_TOKEN_REJRCT_2; //���ʹ���  //	
		break;    	
    }

	return (gs_APDU_analyze_var.u8_result);   //���ؽ������  //
}


/*****************************************************************************
** Function name    :void api_judge_multiple_TOKEN_interval_pre_second(void)
** Description      :����മTOKEN��Ч���ʱ���жϳ���           
**
** Parameters       :  
**                  
** Returned value   :NO
**
** Note             :����MAX_SECONDS_OF_MULTIPLE_TOKEN_INTERVAL����������  
**----------------------------------------------------------------------------
** V01.01  MJ  2016-09-28
******************************************************************************/
void api_judge_keyChg_TOKEN_interval_pre_second(void)
{
	if(sts_kay_change_data.flg & 0x03)
	{
		sts_kay_change_data.tmr_second ++;
		if(sts_kay_change_data.tmr_second > MAX_SECONDS_OF_MULTIPLE_TOKEN_INTERVAL)
		{
			memset(&sts_kay_change_data.kay[0],0,sizeof(STS_KAY_CHANHE_DATA));	
		}
	}

}	
