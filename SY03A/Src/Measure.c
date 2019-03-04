/************************************************************************
* ��Ȩ���� (C)2016, �����������ܿƼ��ɷ����޹�˾��
*
* �ļ����ƣ� // Measure.c
* �ļ���ʶ�� // 
* ����ժҪ�� //�й����޹������ۼ�
* ����˵���� // 
* ��ǰ�汾�� // V01.00
* �� �ߣ�    // yangxing
* ������ڣ� // 2013��3��05��
*
* �޸ļ�¼1��//
* �޸����ڣ� //
* �� �� �ţ� // V01
* �� �� �ˣ� //
* �޸����ݣ� //
************************************************************************/
#include "ex_func.h"
#include "ex_var.h"
#include <string.h>
#define	 CHECKWORD   0xA55A
#define   MaxFeeNum      4



void BCDX_INC( unsigned char *BCD, unsigned char Len )
{
    unsigned char i;
    
    for( i=0;i<Len;i++ )
    {
        *BCD += 1;
        if( (*BCD & 0x0F) > 0x09 )
        {
            *BCD += 0x06;
            if( (*BCD & 0xF0) > 0x90 )
                *BCD += 0x60;
            else
                break;
        }
        else
            break;
        BCD++;
    }
}

unsigned char BCD_Check( unsigned char *Addr, unsigned int Len )
{
    while( Len > 0 )
    {
        if( (*Addr&0x0F) > 0x09 ) return 1;
        if( (*Addr&0xF0) > 0x90 ) return 1;
        Addr++;
        Len--;
    }
    
    return 0;
}

void BCD4_DEC( unsigned char *BCD )
{
    signed long Lng;
    
    if( (*(BCD+3)) & 0x80 )//??
    {
        *(BCD+3) &= ~0x80;
        Lng = BCD4_HEX4( BCD );
        Lng = ~Lng+1;
    }
    else
    	Lng = BCD4_HEX4( BCD );
    if( Lng <= (-79999999) )
        Lng = 0;
    else
        Lng--;
    if( Lng < 0)
    {
        Lng = ~Lng+1;
        HEX4_BCD4( BCD, Lng );
        *(BCD+3) |= 0x80;
    }
    else
        HEX4_BCD4( BCD, Lng );
}

void BCD4_INC( unsigned char *BCD )
{
    signed long Lng;
    
    if( (*(BCD+3)) & 0x80 )//??
    {
        *(BCD+3) &= ~0x80;
        Lng = BCD4_HEX4( BCD );
        Lng = ~Lng+1;
    }
    else
    {
    	Lng = BCD4_HEX4( BCD );
    }
    if( Lng >= 79999999 )
    {
        Lng = 0;
    }
    else
    {
        Lng++;
    }
    if( Lng < 0)
    {
        Lng = ~Lng+1;
        HEX4_BCD4( BCD, Lng );
        *(BCD+3) |= 0x80;
    }
    else
    {
        HEX4_BCD4( BCD, Lng );
    }
}
unsigned short CheckNum( unsigned char* Addr, unsigned int Lenth )
{
    unsigned short Num = CHECKWORD;

    while( Lenth > 0 )
    {
        Num += *Addr++;
        Lenth--;
    }

    return Num;
}
/************************************************************************
* ������ ��//Y_M_C_CycleEngry
* ���� ��//��ǰ��ȡ��¶ȡ���������й��ۼ��õ�������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//DI2��0����ǰ���ڣ�1����ǰ�£�2����ǰ�ꣻ
             //DI1��0����ǰ��1����1��
             //ReturnBuf:��������
* �޸����� :
* ------------------------------------------------------------------------
* 2013/04/06 V1.0 yangxing XXXX
**************************************************************************/
void Y_M_C_CycleEngry(unsigned char Mode, unsigned char *ReturnBuf)
{
    unsigned long Tmp0 = 0;
    unsigned long Tmp1 = 0;
    unsigned long CurEng = 0;
    unsigned long LastEng = 0;
    unsigned long MonEng = 0;
    
    if( Mode == 0 )
    {
//33        MEM_Read( (unsigned char*)&Tmp0, E2A_LastZ_Act_Pz0, 4, Mem_E2PROMA );//����������й��ܵ���
    }
    else if( Mode == 1 )
    {
//33        MEM_Read( (unsigned char*)&Tmp0, E2A_LastM_Act_Pz0, 4, Mem_E2PROMA );//��1������й��ܵ���(������1����)
    }
    else
    {
//33          MEM_Read( (unsigned char*)&Tmp0, E2A_LastY_Act_Pz0, 4, Mem_E2PROMA );//��1������й��ܵ���
    }
    memcpy( (unsigned char*)&Tmp1, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 4 );//��ǰ����й��ܵ���
        
    if( Tmp0 & 0x80000000 )//������Ϊ������
    {
        Tmp0 &= 0x7FFFFFFF;
        LastEng = BCD4_HEX4( (unsigned char*)&Tmp0 );
        if( Tmp1 & 0x80000000 )//��ǰ����Ϊ������
        {
            Tmp1 &= 0x7FFFFFFF;
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            if( CurEng > LastEng )
            {
              MonEng = CurEng - LastEng;
              HEX4_BCD4( ReturnBuf, MonEng );
              ReturnBuf[3] |= 0x80;
            }
            else//�����79999999��ת�󣬸÷�ʽ����ȷ
            {//���¸�Ϊ��-��������С�����´����������
              MonEng = CurEng + (79999999-LastEng);
              HEX4_BCD4( ReturnBuf, MonEng );
            }
        }
        else//��ǰ����Ϊ������
        {
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            MonEng = CurEng + LastEng;
            HEX4_BCD4( ReturnBuf, MonEng );
        }
    }
    else//��1��Ϊ������
    {
        LastEng = BCD4_HEX4( (unsigned char*)&Tmp0 );
        if( Tmp1 & 0x80000000 )//��ǰ��Ϊ������
        {
            Tmp1 &= 0x7FFFFFFF;
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            MonEng = CurEng + LastEng;
            HEX4_BCD4( ReturnBuf, MonEng );
            ReturnBuf[3] |= 0x80;
        }
        else//��ǰ��Ϊ������
        {
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            if( CurEng >= LastEng )
            {
              MonEng = CurEng - LastEng;
              HEX4_BCD4( ReturnBuf, MonEng );
            }
            else
            {
              MonEng = LastEng - CurEng;
              HEX4_BCD4( ReturnBuf, MonEng );
              ReturnBuf[3] |= 0x80;
            }
        }
    }
}

/************************************************************************
* ������ ��//Current_CycleEngry
* ���� ��//��ǰ��������й��ۼ��õ���
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/04/06 V1.0 yangxing XXXX
**************************************************************************/
void Current_CycleEngry(void)
{
    unsigned long Tmp0 = 0;
    unsigned long Tmp1 = 0;
    unsigned long CurEng = 0;
    unsigned long LastEng = 0;
    unsigned long MonEng = 0;

//33    MEM_Read( (unsigned char*)&Tmp0, E2A_LastZ_Act_Pz0, 4, Mem_E2PROMA );//����������й��ܵ���
    memcpy( (unsigned char*)&Tmp1, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 4 );//��ǰ�ۼ�����õ���

    if( Tmp0 & 0x80000000 )//������Ϊ������
    {
        Tmp0 &= 0x7FFFFFFF;
        LastEng = BCD4_HEX4( (unsigned char*)&Tmp0 );
        if( Tmp1 & 0x80000000 )//��ǰ����Ϊ������
        {
            Tmp1 &= 0x7FFFFFFF;
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            if( CurEng > LastEng )
            {
              MonEng = CurEng - LastEng;
              HEX4_BCD4( (unsigned char*)&CycEnery.CurntPz0[0], MonEng );
              CycEnery.CurntPz0[3] |= 0x80;
            }
            else
            {
              MonEng = LastEng - CurEng;
              HEX4_BCD4( (unsigned char*)&CycEnery.CurntPz0[0], MonEng );
            }
        }
        else//��ǰ����Ϊ������
        {
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            MonEng = CurEng + LastEng;
            HEX4_BCD4( (unsigned char*)&CycEnery.CurntPz0[0], MonEng );
        }
    }
    else//��1��Ϊ������
    {
        LastEng = BCD4_HEX4( (unsigned char*)&Tmp0 );
        if( Tmp1 & 0x80000000 )//��ǰ��Ϊ������
        {
            Tmp1 &= 0x7FFFFFFF;
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            MonEng = CurEng + LastEng;
            HEX4_BCD4( (unsigned char*)&CycEnery.CurntPz0[0], MonEng );
            CycEnery.CurntPz0[3] |= 0x80;
        }
        else//��ǰ��Ϊ������
        {
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            if( CurEng >= LastEng )
            {
              MonEng = CurEng - LastEng;
              HEX4_BCD4( (unsigned char*)&CycEnery.CurntPz0[0], MonEng );
            }
            else
            {
              MonEng = LastEng - CurEng;
              HEX4_BCD4( (unsigned char*)&CycEnery.CurntPz0[0], MonEng );
              CycEnery.CurntPz0[3] |= 0x80;
            }
        }
    }
}

/************************************************************************
* ������ ��//Last_CycleEngry
* ���� ��//��1��������й��ۼ��õ���
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void Last_CycleEngry(void)
{
    unsigned long LastEngB = 0; //���ϴε���
    unsigned long LastEng = 0; //��1�ε���
    unsigned long MonEng = 0;
    unsigned long Tmp0 = 0;
    unsigned long Tmp1 = 0;

//33     MEM_Read( (unsigned char*)&Tmp0, E2A_LastZ2_Act_Pz0, 4, Mem_E2PROMA ); //��2������й��ܵ���
//33     MEM_Read( (unsigned char*)&Tmp1, E2A_LastZ_Act_Pz0, 4, Mem_E2PROMA ); //��1������й��ܵ���
    if( Tmp0 & 0x80000000 ) //��2��Ϊ������
    {
        Tmp0 &= 0x7FFFFFFF;
        LastEngB = BCD4_HEX4( (unsigned char*)&Tmp0 );
        if( Tmp1 & 0x80000000 ) //��1��Ϊ������
        {
            Tmp1 &= 0x7FFFFFFF;
            LastEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            if(LastEng > LastEngB )
            {
              MonEng = LastEng - LastEngB;
              HEX4_BCD4( (unsigned char*)&CycEnery.LastPz0[0], MonEng );           
              CycEnery.LastPz0[3] |= 0x80;
            }
            else
            {
              MonEng = LastEngB - LastEng;
              HEX4_BCD4( (unsigned char*)&CycEnery.LastPz0[0], MonEng );  
            }
              
        }
        else//��1��Ϊ������
        {
            LastEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            MonEng = LastEng + LastEngB;
            HEX4_BCD4( (unsigned char*)&CycEnery.LastPz0[0], MonEng );
        }
    }
    else//��2��Ϊ������
    {
        LastEngB = BCD4_HEX4( (unsigned char*)&Tmp0 );
        if( Tmp1 & 0x80000000 )//��1��Ϊ������
        {
            Tmp1 &= 0x7FFFFFFF;
            LastEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            MonEng = LastEng + LastEngB;
            HEX4_BCD4( (unsigned char*)&CycEnery.LastPz0[0], MonEng );
            CycEnery.LastPz0[3] |= 0x80;
        }
        else//��1��Ϊ������
        {
            LastEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            if(LastEng >= LastEngB )
            {
              MonEng = LastEng - LastEngB;
              HEX4_BCD4( (unsigned char*)&CycEnery.LastPz0[0], MonEng );
            }
            else
            {
              MonEng = LastEngB - LastEng;
              HEX4_BCD4( (unsigned char*)&CycEnery.LastPz0[0], MonEng );
              CycEnery.LastPz0[3] |= 0x80;
            }
        }
    }
}
/************************************************************************
* ������ ��//CycleEngry_Resume
* ���� ��//��ǰ����1��������й��ۼ��õ����ָ�
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2013/04/06 V1.0 yangxing XXXX
**************************************************************************/
void CycleEngry_Resume(void)
{
//    Current_CycleEngry();
    Y_M_C_CycleEngry( 0,(unsigned char*)&CycEnery.CurntPz0);
//    Last_CycleEngry();
}


/************************************************************************
* ������ ��//Current_MonthEngry
* ���� ��//��ǰ������й��ۼ��õ���
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void Current_MonthEngry(void)
{
    unsigned long Tmp0 = 0;
    unsigned long Tmp1 = 0;
    unsigned long CurEng = 0;
    unsigned long LastEng = 0;
    unsigned long MonEng = 0;

//33     MEM_Read( (unsigned char*)&Tmp0, E2A_LastM_Act_Pz0, 4, Mem_E2PROMA );//��1������й��ܵ���(������1����)
    memcpy( (unsigned char*)&Tmp1, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 4 );//��ǰ����й��ܵ���

    if( Tmp0 & 0x80000000 )//��1��Ϊ������
    {
        Tmp0 &= 0x7FFFFFFF;
        LastEng = BCD4_HEX4( (unsigned char*)&Tmp0 );
        if( Tmp1 & 0x80000000 )//��ǰ��Ϊ������
        {
            Tmp1 &= 0x7FFFFFFF;
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            if( CurEng > LastEng )
            {
              MonEng = CurEng - LastEng;
              HEX4_BCD4( (unsigned char*)&MonthEnery.CurntPz0[0], MonEng );
              MonthEnery.CurntPz0[3] |= 0x80;
            }
            else
            {
              MonEng = LastEng - CurEng;
              HEX4_BCD4( (unsigned char*)&MonthEnery.CurntPz0[0], MonEng );
            }
        }
        else//��ǰ��Ϊ������
        {
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            MonEng = CurEng + LastEng;
            HEX4_BCD4( (unsigned char*)&MonthEnery.CurntPz0[0], MonEng );//��ǰ�¶�����й����ۼ��õ���
        }
    }
    else//��1��Ϊ������
    {
        LastEng = BCD4_HEX4( (unsigned char*)&Tmp0 );
        if( Tmp1 & 0x80000000 )//��ǰ��Ϊ������
        {
            Tmp1 &= 0x7FFFFFFF;
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            MonEng = CurEng + LastEng;
            HEX4_BCD4( (unsigned char*)&MonthEnery.CurntPz0[0], MonEng );
            MonthEnery.CurntPz0[3] |= 0x80;
        }
        else//��ǰ��Ϊ������
        {
            CurEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            if( CurEng >= LastEng )
            {
              MonEng = CurEng - LastEng;
              HEX4_BCD4( (unsigned char*)&MonthEnery.CurntPz0[0], MonEng );
            }
            else
            {
              MonEng = LastEng - CurEng;
              HEX4_BCD4( (unsigned char*)&MonthEnery.CurntPz0[0], MonEng );
              MonthEnery.CurntPz0[3] |= 0x80;
            }
        }
    }
}

/************************************************************************
* ������ ��//Last_MonthEngry
* ���� ��//��1������й��ۼ��õ���
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void Last_MonthEngry(void)
{
    unsigned long LastEngB = 0; //���ϴε���
    unsigned long LastEng = 0; //��1�ε���
    unsigned long MonEng = 0;
    unsigned long Tmp0 = 0;
    unsigned long Tmp1 = 0;

 //33    MEM_Read( (unsigned char*)&Tmp0, E2A_LastM2_Act_Pz0, 4, Mem_E2PROMA );//��2������й��ܵ���
//33     MEM_Read( (unsigned char*)&Tmp1, E2A_LastM_Act_Pz0, 4, Mem_E2PROMA );//��1������й��ܵ���
    if( Tmp0 & 0x80000000 )//��2��Ϊ������
    {
        Tmp0 &= 0x7FFFFFFF;
        LastEngB = BCD4_HEX4( (unsigned char*)&Tmp0 );
        if( Tmp1 & 0x80000000 )//��1��Ϊ������
        {
            Tmp1 &= 0x7FFFFFFF;
            LastEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            if(LastEng > LastEngB )
            {
              MonEng = LastEng - LastEngB;
              HEX4_BCD4( (unsigned char*)&MonthEnery.LastPz0[0], MonEng );           
              MonthEnery.LastPz0[3] |= 0x80;
            }
            else
            {
              MonEng = LastEngB - LastEng;
              HEX4_BCD4( (unsigned char*)&MonthEnery.LastPz0[0], MonEng );  
            }
              
        }
        else//��1��Ϊ������
        {
            LastEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            MonEng = LastEng + LastEngB;
            HEX4_BCD4( (unsigned char*)&MonthEnery.LastPz0[0], MonEng );
        }
    }
    else//��2��Ϊ������
    {
        LastEngB = BCD4_HEX4( (unsigned char*)&Tmp0 );
        if( Tmp1 & 0x80000000 )//��1��Ϊ������
        {
            Tmp1 &= 0x7FFFFFFF;
            LastEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            MonEng = LastEng + LastEngB;
            HEX4_BCD4( (unsigned char*)&MonthEnery.LastPz0[0], MonEng );
            MonthEnery.LastPz0[3] |= 0x80;
        }
        else//��1��Ϊ������
        {
            LastEng = BCD4_HEX4( (unsigned char*)&Tmp1 );
            if(LastEng >= LastEngB )
            {
              MonEng = LastEng - LastEngB;
              HEX4_BCD4( (unsigned char*)&MonthEnery.LastPz0[0], MonEng );
            }
            else
            {
              MonEng = LastEngB - LastEng;
              HEX4_BCD4( (unsigned char*)&MonthEnery.LastPz0[0], MonEng );
              MonthEnery.LastPz0[3] |= 0x80;
            }
        }
    }
}
/************************************************************************
* ������ ��//MonthEngry_Resume
* ���� ��//��ǰ����1������й��ۼ��õ����ָ�
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void MonthEngry_Resume(void)
{
    //Current_MonthEngry();
    Y_M_C_CycleEngry( 1,(unsigned char*)&CycEnery.CurntPz0);//��ǰ���ۼ��õ���
//    Last_MonthEngry();
}

/************************************************************************
* ������ ��//EC_ActGroup
* ���� ��//����й��������¼���
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------

**************************************************************************/
void EC_ActGroup(void)
{
    unsigned char i,j;
    unsigned char EC_Flag = 0;
    unsigned long EC_Pp = 0;
    unsigned long EC_Pn = 0;
    signed long EC_Grp = 0;
    unsigned short EC_Check = CHECKWORD;

    for( i = 0; i < 5; i++ )
    {
        EC_Pp = 0;
        EC_Pn = 0;
        EC_Grp = 0;
        EC_Pp = BCD4_HEX4( (unsigned char*)(&AEngy_Blk.AEngy.ActPp0[0]+4*i) );
        EC_Pn = BCD4_HEX4( (unsigned char*)(&AEngy_Blk.AEngy.ActPn0[0]+4*i) );

        for( j = 0; j < 4; j++ )
        {
            switch( MeasureMode.ActMode & (F_IncPp<<j) )
            {
                case F_IncPp://�������й�
                    EC_Grp += EC_Pp;
                    break;
                case F_DecPp://�������й�
                    EC_Grp -= EC_Pp;
                    break;
                case F_IncPn://�ӷ����й�
                    EC_Grp += EC_Pn;
                    break;
                case F_DecPn://�������й�
                    EC_Grp -= EC_Pn;
                    break;
                default:
                    break;
            }
        }
        if( EC_Grp < 0 )
        {
            EC_Grp = ~EC_Grp+1;
            EC_Flag = 1;
        }
        HEX4_BCD4( (unsigned char*)(&AEngy_Blk.AEngy.ActPz0[0]+4*i), EC_Grp );
        if( EC_Flag == 1 )//����й�����Ϊ����
        {
            EC_Flag = 0;
            *(&AEngy_Blk.AEngy.ActPz0[3]+4*i) |= 0x80;
        }
    }

///33    EC_Check += ( CheckNum( (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 20 ) - CHECKWORD );//����֤
    memcpy( (unsigned char*)&AEngy_Blk.AEngy.ActPzChk[0], (unsigned char*)&EC_Check, 2 );

 ///33    MEM_CRCWrite( FM1_EC_Act_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 20, Mem_FRAM );//дFM
 ///33    MEM_CRCWrite( E2A_EC_Act_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 20, Mem_E2PROMA );//дE2
}

/************************************************************************
* ������ ��//EC_RactGroup
* ���� ��//����޹��������¼���
* ������� ��//mode:0 ���1 1 ���2
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------

**************************************************************************/
void EC_RactGroup(unsigned char MeasureSta, unsigned char Mode)
{
    unsigned char i,j,k;
    unsigned char EC_Flag = 0;
    unsigned char Dmd_Flag = 0;
    unsigned char Dmd_Tim[5];
    unsigned long Dmd_Use = 0;
    unsigned long Dmd_Grp = 0;
    signed long EC_Grp[4];//�ܡ�A��B��C����޹�
    unsigned short EC_Check = CHECKWORD;
    unsigned short Ph_Check = CHECKWORD;
    unsigned short Dmd_Check = CHECKWORD;
    unsigned long EC_Qur[4];
    unsigned long Dmd_Qur[4];
    unsigned long Phs_Qur[12];//����4�����޹�

    for( i = 0; i < 12; i++ )//����4�����޹�
    {
        Phs_Qur[i] = BCD4_HEX4( (unsigned char*)(&PEngy_Blk.PEngy.PhAQur1[0]+4*i) );
    }
    for( i = 0; i < 3; i++ )
    {
        EC_Grp[i+1] = 0;
    }
    for( i = 0; i < 5; i++ )//�ܡ��⡢�塢ƽ����
    {
        EC_Grp[0] = 0;
        Dmd_Grp = 0;
        for( k = 0; k < 4; k++ )
        {
            EC_Qur[k] = BCD4_HEX4( (unsigned char*)(&REngy_Blk.REngy.Quadr1P0[0]+4*i+22*k) );
            memcpy( (unsigned char*)&Dmd_Use,(unsigned char*)(&MaxDmd_Blk[k+4].MaxDmd.TotalDmd[0]+8*i),3 );
            Dmd_Qur[k] = BCD4_HEX4( (unsigned char*)&Dmd_Use );
        }
        memset( (unsigned char*)&Dmd_Tim[0], 0x00, 5 );
        for( j = 0; j < 8; j++ )
        {
            switch( MeasureSta & (F_IncQuadr1<<j) )
            {
                case F_IncQuadr1://��1�����޹�
                    EC_Grp[0] += EC_Qur[0];//�ܵ���
                    if( Dmd_Qur[0] > Dmd_Grp )//����
                    {
                        Dmd_Grp = Dmd_Qur[0];
                        memcpy( (unsigned char*)&Dmd_Tim[0],(unsigned char*)(&MaxDmd_Blk[4].MaxDmd.TotalDmdTime[0]+8*i),5 );
                        Dmd_Flag = 0;
                    }
                    if( i == 0 )//�������
                    {
                        EC_Grp[1] += Phs_Qur[0];//A�����
                        EC_Grp[2] += Phs_Qur[1];//B�����
                        EC_Grp[3] += Phs_Qur[2];//C�����
                    }
                    break;
                case F_DecQuadr1://��1�����޹�
                    EC_Grp[0] -= EC_Qur[0];//����
                    if( Dmd_Qur[0] > Dmd_Grp )//����
                    {
                        Dmd_Grp = Dmd_Qur[0];
                        memcpy( (unsigned char*)&Dmd_Tim[0],(unsigned char*)(&MaxDmd_Blk[4].MaxDmd.TotalDmdTime[0]+8*i),5 );
                        Dmd_Flag = 1;
                    }
                    if( i == 0 )//�������
                    {
                        EC_Grp[1] -= Phs_Qur[0];//A�����
                        EC_Grp[2] -= Phs_Qur[1];//B�����
                        EC_Grp[3] -= Phs_Qur[2];//C�����
                    }
                    break;
                case F_IncQuadr2://��2�����޹�
                    EC_Grp[0] += EC_Qur[1];//����
                    if( Dmd_Qur[1] > Dmd_Grp )//����
                    {
                        Dmd_Grp = Dmd_Qur[1];
                        memcpy( (unsigned char*)&Dmd_Tim[0],(unsigned char*)(&MaxDmd_Blk[5].MaxDmd.TotalDmdTime[0]+8*i),5 );
                        Dmd_Flag = 0;
                    }
                    if( i == 0 )
                    {
                        EC_Grp[1] += Phs_Qur[3];//A�����
                        EC_Grp[2] += Phs_Qur[4];//B�����
                        EC_Grp[3] += Phs_Qur[5];//C�����
                    }
                    break;
                case F_DecQuadr2://��2�����޹�
                    EC_Grp[0] -= EC_Qur[1];//����
                    if( Dmd_Qur[1] > Dmd_Grp )//����
                    {
                        Dmd_Grp = Dmd_Qur[1];
                        memcpy( (unsigned char*)&Dmd_Tim[0],(unsigned char*)(&MaxDmd_Blk[5].MaxDmd.TotalDmdTime[0]+8*i),5 );
                        Dmd_Flag = 1;
                    }
                    if( i == 0 )
                    {
                        EC_Grp[1] -= Phs_Qur[3];//A�����
                        EC_Grp[2] -= Phs_Qur[4];//B�����
                        EC_Grp[3] -= Phs_Qur[5];//C�����
                    }
                    break;
                case F_IncQuadr3://��3�����޹�
                    EC_Grp[0] += EC_Qur[2];//����
                    if( Dmd_Qur[2] > Dmd_Grp )//����
                    {
                        Dmd_Grp = Dmd_Qur[2];
                        memcpy( (unsigned char*)&Dmd_Tim[0],(unsigned char*)(&MaxDmd_Blk[6].MaxDmd.TotalDmdTime[0]+8*i),5 );
                        Dmd_Flag = 0;
                    }
                    if( i == 0 )
                    {
                        EC_Grp[1] += Phs_Qur[6];//A�����
                        EC_Grp[2] += Phs_Qur[7];//B�����
                        EC_Grp[3] += Phs_Qur[8];//C�����
                    }
                    break;
                case F_DecQuadr3://��3�����޹�
                    EC_Grp[0] -= EC_Qur[2];//����
                    if( Dmd_Qur[2] > Dmd_Grp )//����
                    {
                        Dmd_Grp = Dmd_Qur[2];
                        memcpy( (unsigned char*)&Dmd_Tim[0],(unsigned char*)(&MaxDmd_Blk[6].MaxDmd.TotalDmdTime[0]+8*i),5 );
                        Dmd_Flag = 1;
                    }
                    if( i == 0 )
                    {
                        EC_Grp[1] -= Phs_Qur[6];//A�����
                        EC_Grp[2] -= Phs_Qur[7];//B�����
                        EC_Grp[3] -= Phs_Qur[8];//C�����
                    }
                    break;
                case F_IncQuadr4://��4�����޹�
                    EC_Grp[0] += EC_Qur[3];//����
                    if( Dmd_Qur[3] > Dmd_Grp )//����
                    {
                        Dmd_Grp = Dmd_Qur[3];
                        memcpy( (unsigned char*)&Dmd_Tim[0],(unsigned char*)(&MaxDmd_Blk[7].MaxDmd.TotalDmdTime[0]+8*i),5 );
                        Dmd_Flag = 0;
                    }
                    if( i == 0 )
                    {
                        EC_Grp[1] += Phs_Qur[9];//A�����
                        EC_Grp[2] += Phs_Qur[10];//B�����
                        EC_Grp[3] += Phs_Qur[11];//C�����
                    }
                    break;
                case F_DecQuadr4://��4�����޹�
                    EC_Grp[0] -= EC_Qur[3];//����
                    if( Dmd_Qur[3] > Dmd_Grp )//����
                    {
                        Dmd_Grp = Dmd_Qur[3];
                        memcpy( (unsigned char*)&Dmd_Tim[0],(unsigned char*)(&MaxDmd_Blk[7].MaxDmd.TotalDmdTime[0]+8*i),5 );
                        Dmd_Flag = 1;
                    }
                    if( i == 0 )
                    {
                        EC_Grp[1] -= Phs_Qur[9];//A�����
                        EC_Grp[2] -= Phs_Qur[10];//B�����
                        EC_Grp[3] -= Phs_Qur[11];//C�����
                    }
                    break;
                default:
                    break;
            }
        }
        if( EC_Grp[0] < 0 )
        {
            EC_Grp[0] = ~EC_Grp[0]+1;
            EC_Flag = 1;
        }

        HEX4_BCD4( (unsigned char*)(&REngy_Blk.REngy.Grp1Pz0[0]+4*i+22*Mode), EC_Grp[0] );//����
        if( EC_Flag == 1 )//����޹�����Ϊ����
        {
            EC_Flag = 0;
            *(&REngy_Blk.REngy.Grp1Pz0[3]+4*i+22*Mode) |= 0x80;
        }

        HEX4_BCD4( (unsigned char*)&Dmd_Use, Dmd_Grp );//����
        memcpy( (unsigned char*)(&MaxDmd_Blk[2+Mode].MaxDmd.TotalDmd[0]+8*i),(unsigned char*)&Dmd_Use, 3 );
        memcpy( (unsigned char*)(&MaxDmd_Blk[2+Mode].MaxDmd.TotalDmdTime[0]+8*i),(unsigned char*)&Dmd_Tim[0], 5 );
        if( Dmd_Flag == 1 )
        {
            Dmd_Flag = 0;
            *(&MaxDmd_Blk[2+Mode].MaxDmd.TotalDmd[2]+8*i) |= 0x80;
        }
    }

    for( i = 0; i < 3; i++ )//�������
    {
        if( EC_Grp[i+1] < 0 )
        {
            EC_Grp[i+1] = ~EC_Grp[i+1]+1;
            EC_Flag = 1;
        }
        HEX4_BCD4( (unsigned char*)(&PEngy_Blk.PEngy.PhAGrp1[0]+4*i+12*Mode), EC_Grp[i+1] );//����
        if( EC_Flag == 1 )//����޹�����Ϊ����
        {
            EC_Flag = 0;
            *(&PEngy_Blk.PEngy.PhAGrp1[3]+4*i+12*Mode) |= 0x80;
        }
    }
    Ph_Check += ( CheckNum( (unsigned char*)&PEngy_Blk.PEngy.PhAGrp1[0], 24 ) - CHECKWORD );
    memcpy( (unsigned char*)&PEngy_Blk.PEngy.PhGrpChk[0], (unsigned char*)&Ph_Check, 2 );
//33    MEM_CRCWrite( FM1_EC_PhA_Grp1, (unsigned char*)&PEngy_Blk.PEngy.PhAGrp1[0], 24, Mem_FRAM );//дFM
//33    MEM_CRCWrite( E2A_EC_PhA_Grp1, (unsigned char*)&PEngy_Blk.PEngy.PhAGrp1[0], 24, Mem_E2PROMA );//дE2

    EC_Check += ( CheckNum( (unsigned char*)(&REngy_Blk.REngy.Grp1Pz0[0]+22*Mode), 20 ) - CHECKWORD );//�ܵ���
    memcpy( (unsigned char*)(&REngy_Blk.REngy.Grp1PzChk[0]+22*Mode), (unsigned char*)&EC_Check, 2 );
//33    MEM_CRCWrite( FM1_EC_Group1_Pz0+22*Mode, (unsigned char*)&REngy_Blk.REngy.Grp1Pz0[0], 20, Mem_FRAM );//дFM
// 33   MEM_CRCWrite( E2A_EC_Group1_Pz0+22*Mode, (unsigned char*)&REngy_Blk.REngy.Grp1Pz0[0], 20, Mem_E2PROMA );//дE2

    Dmd_Check += ( CheckNum( (unsigned char*)&MaxDmd_Blk[2+Mode].MaxDmdBuff[0], 40 ) - CHECKWORD );
    memcpy( (unsigned char*)&MaxDmd_Blk[2+Mode].MaxDmdBuff[40], (unsigned char*)&Dmd_Check, 2 );
//33    MEM_CRCWrite( E2A_TotalDemandGrp1+42*Mode, (unsigned char*)&MaxDmd_Blk[2].MaxDmdBuff[0], 40, Mem_E2PROMA );
}

/************************************************************************
* ������ ��//PzEC_INC
* ���� ��//����й����޹���������1
* ������� ��//RamAddr������RAM��ַ��OffAddr,ƫ�Ƶ�ַ
             //Len: ���ݳ��ȣ�TouFee: 0-�з���, 1-�޷���
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void PzEC_INC( unsigned char* RamAddr, unsigned char OffAddr, unsigned char Len, unsigned char TouFee )
{
    unsigned short EC_Check = 0;

    if( BCD_Check((RamAddr),4) == 0 )
    {
        BCD4_INC( RamAddr+OffAddr );
        EC_Check = CheckNum( RamAddr, Len );//����ʼ��ַ��ʼ��У���
        memcpy( RamAddr+Len, (unsigned char*)&EC_Check, 2 );//����У���

        if( FramGrpWriteFlag != 0x5A )
            FramGrpWriteFlag = 0x5A;

        if( (*(RamAddr+OffAddr) == 0x00) &&
            (((*(RamAddr+OffAddr+1)&0x0F) == 0x00) || ((*(RamAddr+OffAddr+1)&0x0F) == 0x05)) )//5kWhдE2
        {
            if( E2PGrpWriteFlag != 0x5A )
                E2PGrpWriteFlag = 0x5A;
        }
      /*
        if( (Fee.TOUNo > 0) && (Fee.TOUNo <= MaxFeeNum) && (TouFee == 0) )//���ʵ����ۼ�
        {
            if( BCD_Check((RamAddr+OffAddr+4*Fee.TOUNo),4) == 0 )//��ʱ����
            {
                BCD4_INC( RamAddr+OffAddr+4*Fee.TOUNo );
                EC_Check = CheckNum( RamAddr, Len );//����ʼ��ַ��ʼ��У���
                memcpy( RamAddr+Len, (unsigned char*)&EC_Check, 2 );//����У���
            }
        }*/
    }
}

/************************************************************************
* ������ ��//PzEC_DEC
* ���� ��//����й����޹���������1
* ������� ��//RamAddr������RAM��ַ��OffAddr,ƫ�Ƶ�ַ
             //Len: ���ݳ��ȣ�TouFee: 0-�з���, 1-�޷���
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void PzEC_DEC( unsigned char* RamAddr, unsigned char OffAddr, unsigned char Len, unsigned char TouFee )
{
    unsigned short EC_Check = 0;

    if( BCD_Check((RamAddr),4) == 0 )
    {
        BCD4_DEC( RamAddr+OffAddr );
        EC_Check = CheckNum( RamAddr, Len );//����ʼ��ַ��ʼ��У���
        memcpy( RamAddr+Len, (unsigned char*)&EC_Check, 2 );//����У���

        if( FramGrpWriteFlag != 0x5A )
            FramGrpWriteFlag = 0x5A;

        if( (*(RamAddr+OffAddr) == 0x00) &&
            (((*(RamAddr+OffAddr+1)&0x0F) == 0x00) || ((*(RamAddr+OffAddr+1)&0x0F) == 0x05)) )//5kWhдE2
        {
            if( E2PGrpWriteFlag != 0x5A )
                E2PGrpWriteFlag = 0x5A;
        }
        /*
        if( (Fee.TOUNo > 0) && (Fee.TOUNo <= MaxFeeNum) && (TouFee == 0) )//���ʵ����ۼ�
        {
            if( BCD_Check((RamAddr+OffAddr+4*Fee.TOUNo),4) == 0 )//��ʱ����
            {
                BCD4_DEC( RamAddr+OffAddr+4*Fee.TOUNo );
                EC_Check = CheckNum( RamAddr, Len );//����ʼ��ַ��ʼ��У���
                memcpy( RamAddr+Len, (unsigned char*)&EC_Check, 2 );//����У���
            }
        }*/
    }
}

/************************************************************************
* ������ ��//EC_INC
* ���� ��//�й����޹���������1
* ������� ��//RamAddr��������ʼ��ַ��OffAddr,ƫ�Ƶ�ַ
             //Len: ���ݳ��ȣ�TouFee: 0-�з���, 1-�޷���
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void EC_INC( unsigned char* RamAddr, unsigned char OffAddr, unsigned char Len, unsigned char TouFee )
{
    unsigned short EC_Check = 0;

    if( BCD_Check((RamAddr),4) == 0 )
    {
        BCDX_INC( (RamAddr+OffAddr), 4 );
        EC_Check = CheckNum( RamAddr, Len );//����ʼ��ַ��ʼ��У���
        memcpy( RamAddr+Len, (unsigned char*)&EC_Check, 2 );//����У���

        if( FramWriteFlag != 0x5A )
            FramWriteFlag = 0x5A;

        if( (*(RamAddr+OffAddr) == 0x00) &&
            (((*(RamAddr+OffAddr+1)&0x0F) == 0x00) || ((*(RamAddr+OffAddr+1)&0x0F) == 0x05)) )//5kWhдE2
        {
            if( E2PWriteFlag != 0x5A )
                E2PWriteFlag = 0x5A;
        }
/*
        if( (Fee.TOUNo > 0) && (Fee.TOUNo <= MaxFeeNum) && (TouFee == 0) )//���ʵ����ۼ�
        {
            if( BCD_Check((RamAddr+OffAddr+4*Fee.TOUNo),4) == 0 )//��ʱ����
            {
                BCDX_INC( (RamAddr+OffAddr+4*Fee.TOUNo), 4 );
                EC_Check = CheckNum( RamAddr, Len );//����ʼ��ַ��ʼ��У���
                memcpy( RamAddr+Len, (unsigned char*)&EC_Check, 2 );//����У���
            }
        }
        */
    }
}

/************************************************************************
* ������ ��//APulseCnt
* ���� ��//���������й��������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void APulseCnt(void)
{
    if( ( Quadrant[0] < 1 ) || ( Quadrant[0] > 4 ) )
    {
        Quadrant[0] = 1;
    }

    if( ( Quadrant[0] == 1 ) || ( Quadrant[0] == 4 ) )
    {
        PCnt_Blk.PCnt.ActPpCnt[0]++;//�����й��������ۼ�
    }
    else
    {
        PCnt_Blk.PCnt.ActPnCnt[0]++;//�����й��������ۼ�
    }
}

/************************************************************************
* ������ ��//RPulseCnt
* ���� ��//�޹������ۼ�
* ������� ��//CntAdd: ���������ַ,Channel:ͨ���� 0--���޹� 1��2��3--A��B��C���޹�
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void RPulseCnt( unsigned short* CntAdd, unsigned char Channel )
{
    if( ( Quadrant[Channel] < 1 ) || ( Quadrant[Channel] > 4 ) )
    {
        Quadrant[Channel] = 1;
    }

    switch( Quadrant[Channel] )
    {
        case 1:
            if( Channel > 0)//����
                (*CntAdd)++;
            else
            {//����
                (*CntAdd)++;
                (*(CntAdd+((Tariff.Current&3)+1)))++;
            }
            break;
        case 2:
            if( Channel > 0)
                (*(CntAdd+3))++;
            else
            {
                (*(CntAdd+5))++;
                (*(CntAdd+5+((Tariff.Current&3)+1)))++;
            }
            break;
        case 3:
            if( Channel > 0)
                (*(CntAdd+6))++;
            else
            {
                (*(CntAdd+10))++;
                (*(CntAdd+10+((Tariff.Current&3)+1)))++;
            }
            break;
        case 4:
            if( Channel > 0)
                (*(CntAdd+9))++;
            else
            {
                (*(CntAdd+15))++;
                (*(CntAdd+15+((Tariff.Current&3)+1)))++;
            }
            break;
        default:
            break;
    }
}

/************************************************************************
* ������ ��//PulseCount
* ���� ��//�������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 
**************************************************************************/
void PulseCount(void)
{
    unsigned char i;

//    for( i = 0; i < 3; i++ )
//    {
//        while( PowerValue.ActPowerPp[i] >= Factor.Ke )//A��B��C�����й������ۼ�
//        {
//            PowerValue.ActPowerPp[i] -= Factor.Ke;
//            (*(&PCnt_Blk.PCnt.PhAPpCnt+i))++;//����һ���������0.0001KWH
//        }
//        while( PowerValue.ActPowerPn[i] >= Factor.Ke )//A��B��C�����й������ۼ�
//        {
//            PowerValue.ActPowerPn[i] -= Factor.Ke;
//            (*(&PCnt_Blk.PCnt.PhAPnCnt+i))++;
//        }
//        while( PowerValue.RactPowerPp[i] >= Factor.Ke )//A��B��C 4�����޹������ۼ�
//        {
//            PowerValue.RactPowerPp[i] -= Factor.Ke;
//            RPulseCnt( (unsigned short*)(&PCnt_Blk.PCnt.AQuadr1PCnt+i), (i+1) );
//        }

//        if( i < 2 )
//        {
//            while( PowerValue.VahrPower[i] >= Factor.Ke )//��������������
//            {
//                PowerValue.VahrPower[i] -= Factor.Ke;
//                (*(&PCnt_Blk.PCnt.VAHRCnt[0]+i*5))++;
//                (*(&PCnt_Blk.PCnt.VAHRCnt[((Tariff.Current&3)+1)]+i*5))++;
//            }
//        }
//    }

    while( PowerValue.ActPowerP >= Factor.Ke )
    {
        PowerValue.ActPowerP -= Factor.Ke;
//        PCnt_Blk.PCnt.ActPpCnt[0]++;//�����й��������ۼ�,���峣��С��20000ʱ����һ���������0.0001KWH������Ϊ0.00001KWH
//        PCnt_Blk.PCnt.ActPpCnt[((Tariff.Current&3)+1)]++;//�����й����������ۼ�
			  Decimal.Fw[0]++;
        
    }
                                                                                                            
   //�����й�������
    while( PowerValue.ActPowerN  >= Factor.Ke )
    {
        PowerValue.ActPowerN -= Factor.Ke;
//        PCnt_Blk.PCnt.ActPnCnt[0]++;//�����й��������ۼ�
//        PCnt_Blk.PCnt.ActPnCnt[((Tariff.Current&3)+1)]++;//�����й����������ۼ� 
			  Decimal.Bk[0]++;	
    }

   //4�����޹�������
//    while( PowerValue.RactPower >= Factor.Ke )
//    {
//        PowerValue.RactPower -= Factor.Ke;
//        RPulseCnt( (unsigned short*)&PCnt_Blk.PCnt.Quadr1PCnt, 0 );
//    } 
}

/************************************************************************
* ������ ��//ECMeasure
* ���� ��//���������ۼƵ�һ��ֵʱ,�������ۼ�
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//����Ҫ�������й�=����+����
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void ECMeasure(void)
{
    unsigned char i;
    unsigned short EC_Check2 = 0;
    unsigned char FeeNo;
	  union B16_B08 PBlk;
	
	
	
	while( Decimal.Fw[0] >= g_PerKwh )
	{
		Decimal.Fw[0] -= g_PerKwh;
		Decimal.Unit_Pluse++;
	}
	
	while( Decimal.Bk[0] >= g_PerKwh )
	{
		Decimal.Bk[0] -= g_PerKwh;
		Decimal.Unit_Bkpluse++;
	}
	
	
	Calc_Pwr_Proc();
//	
//	Decimal.Unit_Pluse++;
//	
//    for( i = 0; i < 2; i++ )//��ǰ���򡢷�������й��ۼ�
//    {
//        Clear_Wdt();//�忴�Ź�
//			  PBlk.B08[0] =PCnt_Blk.PCntBuff[58+(10*i)];
//			  PBlk.B08[1] =PCnt_Blk.PCntBuff[59+(10*i)];
//			
//        while( PBlk.B16 >= g_PerKwh )
//        {
//            PBlk.B16 -= g_PerKwh;
//					  PCnt_Blk.PCntBuff[58+(10*i)] = PBlk.B08[0] ;
//			      PCnt_Blk.PCntBuff[59+(10*i)] = PBlk.B08[1] ;
//            if( !((MeasureMode.ActMode & (F_IncPp<<(2*i)))&&(MeasureMode.ActMode & (F_DecPp<<(2*i)))) )//��ǰ����й�����
//            {
//                if( MeasureMode.ActMode & (F_IncPp<<(2*i)) )
//                {
//// 33                   Energy_checksum(0);
//                    PzEC_INC( (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 0, 20, 0 );//��ǰ����й�=+����+����
//                    
//                }
//                else if( MeasureMode.ActMode & (F_DecPp<<(2*i)) )
//                {
//// 33                   Energy_checksum(0);
//                    PzEC_DEC( (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 0, 20, 0 );//��ǰ����й�=-����-����                   
//                }
//                
//            }           
////33            Energy_checksum(i+1);//�����������й�
//            EC_INC( (unsigned char*)(&AEngy_Blk.AEngy.ActPp0[0]+22*i), 0, 20, 0 );
//          
//            PzEC_INC( (unsigned char*)&AEngy_Blk.AEngy.ActUsedPz0[0], 0, 4, 0 );//+����+���� �����������������ֵ��ʽ
//            
////33            if((unsigned char*)(&AEngy_Blk.AEngy.ActPp0[0]+22*i)==0) Flag.Eng_Moneystate |=F_EenZD;//��������
////33            Amount_deduction(1);      
//             
//        }
//        PBlk.B08[0] =PCnt_Blk.PCntBuff[58+FeeNo*5+(10*i)];
//			  PBlk.B08[1] =PCnt_Blk.PCntBuff[59+FeeNo*5+(10*i)];
//        while( PBlk.B16 >= g_PerKwh )//�ַ���
//        {
//            FramWriteFlag = 0x5A;
//            PBlk.B16 -= g_PerKwh;
//            PCnt_Blk.PCntBuff[58+FeeNo*5+(10*i)] = PBlk.B08[0];
//			      PCnt_Blk.PCntBuff[59+FeeNo*5+(10*i)] = PBlk.B08[1] ;
//            if( !((MeasureMode.ActMode & (F_IncPp<<(2*i)))&&(MeasureMode.ActMode & (F_DecPp<<(2*i)))) )//����й�����
//            {
//                FramGrpWriteFlag = 0x5A;
//                if( MeasureMode.ActMode & (F_IncPp<<(2*i)) )
//                {
////33                    Energy_checksum(0);
//                    if( (FeeNo > 0) && (FeeNo <= MaxFeeNum) )//���ʵ����ۼ�
//                    {
//                        if( BCD_Check((&AEngy_Blk.AEngy.ActPz0[0]+4*FeeNo),4) == 0 )//��ʱ����
//                        {
//                            BCD4_INC( &AEngy_Blk.AEngy.ActPz0[0]+4*FeeNo );
//                            EC_Check2 = CheckNum( &AEngy_Blk.AEngy.ActPz0[0], 20 );//����ʼ��ַ��ʼ��У���
//                            memcpy( &AEngy_Blk.AEngy.ActPz0[0]+20, (unsigned char*)&EC_Check2, 2 );//����У���
//                        }
//                    }
//                }
//                else if( MeasureMode.ActMode & (F_DecPp<<(2*i)) )
//                {
////33                    Energy_checksum(0);
//                    if( (FeeNo > 0) && (FeeNo <= MaxFeeNum) )//���ʵ����ۼ�
//                    {
//                        if( BCD_Check((&AEngy_Blk.AEngy.ActPz0[0]+4*FeeNo),4) == 0 )//��ʱ����
//                        {
//                            BCD4_DEC( &AEngy_Blk.AEngy.ActPz0[0]+4*FeeNo );
//                            EC_Check2 = CheckNum( &AEngy_Blk.AEngy.ActPz0[0], 20 );//����ʼ��ַ��ʼ��У���
//                            memcpy( &AEngy_Blk.AEngy.ActPz0[0]+20, (unsigned char*)&EC_Check2, 2 );//����У���
//                        }
//                    }
//                }
//            }

////33            Energy_checksum(i+1);//���������й����ʵ���
//            if( ( FeeNo > 0) && ( FeeNo <= MaxFeeNum) )//���ʵ����ۼ�
//            {
//                if( BCD_Check((&AEngy_Blk.AEngy.ActPp0[0]+i*22+4*FeeNo),4) == 0 )//��ʱ����
//                {
//                    BCD4_INC( &AEngy_Blk.AEngy.ActPp0[0]+i*22+4*FeeNo );
//                    EC_Check2 = CheckNum( &AEngy_Blk.AEngy.ActPp0[0]+i*22, 20 );//����ʼ��ַ��ʼ��У���
//                    memcpy( &AEngy_Blk.AEngy.ActPp0[0]+i*22+20, (unsigned char*)&EC_Check2, 2 );//����У���
//                }
////33                Amount_deduction(0);           
//                
//            }
//           
//        }

//        if( FramWriteFlag == 0x5A )//д����
//        {
//            FramWriteFlag = 0;
////33            MEM_CRCWrite( FM1_EC_Act_Pp0+22*i, (unsigned char*)(&AEngy_Blk.AEngy.ActPp0[0]+22*i), 20, Mem_FRAM );
//        }
//        if( E2PWriteFlag == 0x5A )//дEEPROM
//        {
//            E2PWriteFlag = 0;
////33            MEM_CRCWrite( E2A_EC_Act_Pp0+22*i, (unsigned char*)(&AEngy_Blk.AEngy.ActPp0[0]+22*i), 20, Mem_E2PROMA );
//        }
//    }
//    
//    if( FramGrpWriteFlag == 0x5A )//д����й�����
//    {
//        FramGrpWriteFlag = 0;
//////33        MEM_CRCWrite( FM1_EC_Act_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 20, Mem_FRAM );//������д����й�����
//        Y_M_C_CycleEngry( 0,(unsigned char*)&MonthEnery.CurntPz0);//��ǰ�����ۼ��õ���  
//        Y_M_C_CycleEngry( 1,(unsigned char*)&CycEnery.CurntPz0);//��ǰ�¶��ۼ��õ���
//        Y_M_C_CycleEngry( 2,(unsigned char*)&YearEnery.CurntPz0);//��ǰ����ۼ��õ���
//    }
//    if( E2PGrpWriteFlag == 0x5A )
//    {
//        E2PGrpWriteFlag = 0;
////33        MEM_CRCWrite( E2A_EC_Act_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 20, Mem_E2PROMA );//��E2д����й�����
//    }
//    PCnt_Blk.PCnt.ActPzCnt[0] =0;
//    PCnt_Blk.PCnt.ActPzCnt[FeeNo]=0;
//    
//    if( (MeasureMode.ActMode & 0x0F) != 0x00 )//����й�ʣ�������ۼ�
//    {
//       
//       if( MeasureMode.ActMode & F_IncPp )
//       {
//         PCnt_Blk.PCnt.ActPzCnt[0] += PCnt_Blk.PCnt.ActPpCnt[0];
//         PCnt_Blk.PCnt.ActPzCnt[FeeNo] += PCnt_Blk.PCnt.ActPpCnt[FeeNo];
//       }

//       if( MeasureMode.ActMode & F_IncPn )
//       {
//         PCnt_Blk.PCnt.ActPzCnt[0] += PCnt_Blk.PCnt.ActPnCnt[0];
//         PCnt_Blk.PCnt.ActPzCnt[FeeNo] += PCnt_Blk.PCnt.ActPnCnt[FeeNo];
//       }
       /*
         if( PCnt_Blk.PCnt.ActPzCnt[0] >= g_PerKwh )
             PCnt_Blk.PCnt.ActPzCnt[0] = (g_PerKwh-1);
         if( PCnt_Blk.PCnt.ActPzCnt[Fee.TOUNo] >= g_PerKwh )
             PCnt_Blk.PCnt.ActPzCnt[Fee.TOUNo] = (g_PerKwh-1);
       */
    
      
//    for( i = 0; i < 4; i++ )//4�����޹�������޹�1������޹�2�����ۼ�
////    {
////        Clear_Wdt();//�忴�Ź�
//        while( *(&PCnt_Blk.PCnt.Quadr1PCnt[0]+i*5) >= g_PerKwh )
//        {
//            *(&PCnt_Blk.PCnt.Quadr1PCnt[0]+i*5) -= g_PerKwh;

//            if( !((MeasureMode.RactMode1 & (F_IncQuadr1<<(2*i)))&&(MeasureMode.RactMode1 & (F_DecQuadr1<<(2*i)))) )//����޹�1
//            {
//                if( MeasureMode.RactMode1 & (F_IncQuadr1<<(2*i)) )
//                {
////33                    Energy_checksum(3);
//                    PzEC_INC( (unsigned char*)&REngy_Blk.REngy.Grp1Pz0[0], 0, 20, 0 );//����޹�1+����
//                }
//                else if( MeasureMode.RactMode1 & (F_DecQuadr1<<(2*i)) )
//                {
////33                    Energy_checksum(3);
//                    PzEC_DEC( (unsigned char*)&REngy_Blk.REngy.Grp1Pz0[0], 0, 20, 0 );//����޹�1-����
//                }
//            }

//            if( !((MeasureMode.RactMode2 & (F_IncQuadr1<<(2*i)))&&(MeasureMode.RactMode2 & (F_DecQuadr1<<(2*i)))) )//����޹�2
//            {
//                if( MeasureMode.RactMode2 & (F_IncQuadr1<<(2*i)) )
//                {
////33                    Energy_checksum(4);
//                    PzEC_INC( (unsigned char*)&REngy_Blk.REngy.Grp2Pz0[0], 0, 20, 0 );//����޹�2+����
//                }
//                else if( MeasureMode.RactMode2 & (F_DecQuadr1<<(2*i)) )
//                {
////33                    Energy_checksum(4);
//                    PzEC_DEC( (unsigned char*)&REngy_Blk.REngy.Grp2Pz0[0], 0, 20, 0 );//����޹�2+����
//                }
//            }

////33            Energy_checksum(i+5);//4�����޹�����
//            EC_INC( (unsigned char*)(&REngy_Blk.REngy.Quadr1P0[0]+22*i), 0,20, 0 );
//        }
//        
//        while( *(&PCnt_Blk.PCnt.Quadr1PCnt[FeeNo]+i*5) >= g_PerKwh )//���ʵ���
//        {
//            FramWriteFlag = 0x5A;
//            FramGrpWriteFlag = 0x5A;
//            *(&PCnt_Blk.PCnt.Quadr1PCnt[FeeNo]+i*5) -= g_PerKwh;

//            if( !((MeasureMode.RactMode1 & (F_IncQuadr1<<(2*i)))&&(MeasureMode.RactMode1 & (F_DecQuadr1<<(2*i)))) )//����޹�1
//            {
//                if( MeasureMode.RactMode1 & (F_IncQuadr1<<(2*i)) )
//                {
////33                    Energy_checksum(3);
//                    //PzEC_INC( (unsigned char*)&REngy_Blk.REngy.Grp1Pz0[0], 0, 20, 0 );
//                    if( (FeeNo > 0) && (FeeNo <= MaxFeeNum) )//���ʵ����ۼ�
//                    {
//                        if( BCD_Check((&REngy_Blk.REngy.Grp1Pz0[0]+4*FeeNo),4) == 0 )//��ʱ����
//                        {
//                            BCD4_INC( &REngy_Blk.REngy.Grp1Pz0[0]+4*FeeNo );
//                            EC_Check2 = CheckNum( &REngy_Blk.REngy.Grp1Pz0[0], 20 );//����ʼ��ַ��ʼ��У���
//                            memcpy( &REngy_Blk.REngy.Grp1Pz0[0]+20, (unsigned char*)&EC_Check2, 2 );//����У���
//                        }
//                    }
//                }
//                else if( MeasureMode.RactMode1 & (F_DecQuadr1<<(2*i)) )
//                {
//// 33                   Energy_checksum(3);
//                    //PzEC_DEC( (unsigned char*)&REngy_Blk.REngy.Grp1Pz0[0], 0, 20, 0 );
//                    if( (FeeNo > 0) && (FeeNo <= MaxFeeNum) )//���ʵ����ۼ�
//                    {
//                        if( BCD_Check((&REngy_Blk.REngy.Grp1Pz0[0]+4*FeeNo),4) == 0 )//��ʱ����
//                        {
//                            BCD4_DEC( &REngy_Blk.REngy.Grp1Pz0[0]+4*FeeNo );
//                            EC_Check2 = CheckNum( &REngy_Blk.REngy.Grp1Pz0[0], 20 );//����ʼ��ַ��ʼ��У���
//                            memcpy( &REngy_Blk.REngy.Grp1Pz0[0]+20, (unsigned char*)&EC_Check2, 2 );//����У���
//                        }
//                    }
//                }
//            }

//            if( !((MeasureMode.RactMode2 & (F_IncQuadr1<<(2*i)))&&(MeasureMode.RactMode2 & (F_DecQuadr1<<(2*i)))) )//����޹�2
//            {
//                if( MeasureMode.RactMode2 & (F_IncQuadr1<<(2*i)) )
//                {
////33                    Energy_checksum(4);
//                    //PzEC_INC( (unsigned char*)&REngy_Blk.REngy.Grp2Pz0[0], 0, 20, 0 );
//                    if( (FeeNo > 0) && (FeeNo <= MaxFeeNum) )//���ʵ����ۼ�
//                    {
//                        if( BCD_Check((&REngy_Blk.REngy.Grp2Pz0[0]+4*FeeNo),4) == 0 )//��ʱ����
//                        {
//                            BCD4_INC( &REngy_Blk.REngy.Grp2Pz0[0]+4*FeeNo );
//                            EC_Check2 = CheckNum( &REngy_Blk.REngy.Grp2Pz0[0], 20 );//����ʼ��ַ��ʼ��У���
//                            memcpy( &REngy_Blk.REngy.Grp2Pz0[0]+20, (unsigned char*)&EC_Check2, 2 );//����У���
//                        }
//                    }
//                }
//                else if( MeasureMode.RactMode2 & (F_DecQuadr1<<(2*i)) )
//                {
//// 33                   Energy_checksum(4);
//                    //PzEC_DEC( (unsigned char*)&REngy_Blk.REngy.Grp2Pz0[0], 0, 20, 0 );
//                    if( (FeeNo > 0) && (FeeNo <= MaxFeeNum) )//���ʵ����ۼ�
//                    {
//                        if( BCD_Check((&REngy_Blk.REngy.Grp2Pz0[0]+4*FeeNo),4) == 0 )//��ʱ����
//                        {
//                            BCD4_DEC( &REngy_Blk.REngy.Grp2Pz0[0]+4*FeeNo );
//                            EC_Check2 = CheckNum( &REngy_Blk.REngy.Grp2Pz0[0], 20 );//����ʼ��ַ��ʼ��У���
//                            memcpy( &REngy_Blk.REngy.Grp2Pz0[0]+20, (unsigned char*)&EC_Check2, 2 );//����У���
//                        }
//                    }
//                }
//            }

////33            Energy_checksum(i+5);//4�����޹�����
//            //EC_INC( (unsigned char*)(&REngy_Blk.REngy.Quadr1P0[0]+22*i), 0,20, 0 );
//            if( (FeeNo > 0) && (FeeNo <= MaxFeeNum) )//���ʵ����ۼ�
//            {
//                if( BCD_Check((&REngy_Blk.REngy.Quadr1P0[0]+i*22+4*FeeNo),4) == 0 )//��ʱ����
//                {
//                    BCD4_INC( &REngy_Blk.REngy.Quadr1P0[0]+i*22+4*FeeNo );
//                    EC_Check2 = CheckNum( &REngy_Blk.REngy.Quadr1P0[0]+i*22, 20 );//����ʼ��ַ��ʼ��У���
//                    memcpy( &REngy_Blk.REngy.Quadr1P0[0]+i*22+20, (unsigned char*)&EC_Check2, 2 );//����У���
//                }
//            }
//        }

//        if( FramWriteFlag == 0x5A )
//        {
//            FramWriteFlag = 0;
////33            MEM_CRCWrite( FM1_EC_Quadr1_P0+22*i, (unsigned char*)(&REngy_Blk.REngy.Quadr1P0[0]+22*i), 20, Mem_FRAM );
//        }
//        if( E2PWriteFlag == 0x5A )
//        {
//            E2PWriteFlag = 0;
////33            MEM_CRCWrite( E2A_EC_Quadr1_P0+22*i, (unsigned char*)(&REngy_Blk.REngy.Quadr1P0[0]+22*i), 20, Mem_E2PROMA );
//        }
//    }
//    if( FramGrpWriteFlag == 0x5A )//д����޹�1��2����
//    {
//        FramGrpWriteFlag = 0;
////33        MEM_CRCWrite( FM1_EC_Group1_Pz0, (unsigned char*)&REngy_Blk.REngy.Grp1Pz0[0], 20, Mem_FRAM );
////33        MEM_CRCWrite( FM1_EC_Group2_Pz0, (unsigned char*)&REngy_Blk.REngy.Grp2Pz0[0], 20, Mem_FRAM );
//    }
//    if( E2PGrpWriteFlag == 0x5A )
//    {
//        E2PGrpWriteFlag = 0;
////33        MEM_CRCWrite( E2A_EC_Group1_Pz0, (unsigned char*)&REngy_Blk.REngy.Grp1Pz0[0], 20, Mem_E2PROMA );
////33        MEM_CRCWrite( E2A_EC_Group2_Pz0, (unsigned char*)&REngy_Blk.REngy.Grp2Pz0[0], 20, Mem_E2PROMA );
//    }

//    PCnt_Blk.PCnt.Grp1PzCnt[0]=0;
//    PCnt_Blk.PCnt.Grp1PzCnt[FeeNo]=0;
//    PCnt_Blk.PCnt.Grp2PzCnt[0]=0;
//    PCnt_Blk.PCnt.Grp2PzCnt[FeeNo]=0;
//����޹�����0.01�ȵ�������
//    for( i = 0; i < 4; i++ )//����޹�1������޹�2ʣ�������Ե�ǰ�����ڵ�����Ϊ׼
//    {
//        if( MeasureMode.RactMode1 & (F_IncQuadr1<<(2*i)) )
//        {
//            PCnt_Blk.PCnt.Grp1PzCnt[0] +=*(&PCnt_Blk.PCnt.Quadr1PCnt[0]+i*5);
//            PCnt_Blk.PCnt.Grp1PzCnt[FeeNo] +=*(&PCnt_Blk.PCnt.Quadr1PCnt[FeeNo]+i*5);
//        }

//        if( MeasureMode.RactMode2 & (F_IncQuadr1<<(2*i)) )
//        {
//            PCnt_Blk.PCnt.Grp2PzCnt[0] +=*(&PCnt_Blk.PCnt.Quadr1PCnt[0]+i*5);
//            PCnt_Blk.PCnt.Grp2PzCnt[FeeNo] +=*(&PCnt_Blk.PCnt.Quadr1PCnt[FeeNo]+i*5);
//        }
//    }
    /*
    if(PCnt_Blk.PCnt.Grp1PzCnt[0] >= g_PerKwh)
        PCnt_Blk.PCnt.Grp1PzCnt[0] = (g_PerKwh-1);
    if(PCnt_Blk.PCnt.Grp2PzCnt[0] >= g_PerKwh)
        PCnt_Blk.PCnt.Grp1PzCnt[0] = (g_PerKwh-1);
    if(PCnt_Blk.PCnt.Grp1PzCnt[Fee.TOUNo] >= g_PerKwh)
        PCnt_Blk.PCnt.Grp1PzCnt[Fee.TOUNo] = (g_PerKwh-1);  
    if(PCnt_Blk.PCnt.Grp2PzCnt[Fee.TOUNo] >= g_PerKwh)
        PCnt_Blk.PCnt.Grp2PzCnt[Fee.TOUNo] = (g_PerKwh-1);
    */
//    for( i = 0; i < 6; i++ )//A��B��c�����򡢷����й�
//    {
//        while( *(&PCnt_Blk.PCnt.PhAPpCnt+i) >= g_PerKwh )
//        {
//            *(&PCnt_Blk.PCnt.PhAPpCnt+i) -= g_PerKwh;
////33            Energy_checksum(9);
//            EC_INC( (unsigned char*)&PEngy_Blk.PEngy.PhAPp[0], 4*i, 24, 1 );
//        }
//    }
//    if( FramWriteFlag == 0x5A )
//    {
//        FramWriteFlag = 0;
//// 33       MEM_CRCWrite( FM1_EC_PhA_Pp, (unsigned char*)&PEngy_Blk.PEngy.PhAPp[0], 24, Mem_FRAM );
//    }
//    if( E2PWriteFlag == 0x5A )
//    {
//        E2PWriteFlag = 0;
//// 33       MEM_CRCWrite( E2A_EC_PhA_Pp, (unsigned char*)&PEngy_Blk.PEngy.PhAPp[0], 24, Mem_E2PROMA );
//    }
////��Ҫ������޹�����
//    for( i = 0; i < 12; i++ )//A��B��c��1��2��3��4�޹��ۼ�
//    {
//        while( *(&PCnt_Blk.PCnt.AQuadr1PCnt+i) >= g_PerKwh )
//        {
//            *(&PCnt_Blk.PCnt.AQuadr1PCnt+i) -= g_PerKwh;

//            if( !((MeasureMode.RactMode1 & (F_IncQuadr1<<(2*(i/3))))&&(MeasureMode.RactMode1 & (F_DecQuadr1<<(2*(i/3))))) )//��������޹�1
//            {
//                if( MeasureMode.RactMode1 & (F_IncQuadr1<<(2*(i/3))) )//��4����
//                {
////33                    Energy_checksum(10);
//                    PzEC_INC( (unsigned char*)&PEngy_Blk.PEngy.PhAGrp1[0], 4*(i%3), 24, 1 );
//                }
//                else if( MeasureMode.RactMode1 & (F_DecQuadr1<<(2*(i/3))) )//��4����
//                {
//// 33                   Energy_checksum(10);
//                    PzEC_DEC( (unsigned char*)&PEngy_Blk.PEngy.PhAGrp1[0], 4*(i%3), 24, 1 );
//                }
//            }

//            if( !((MeasureMode.RactMode2 & (F_IncQuadr1<<(2*(i/3))))&&(MeasureMode.RactMode2 & (F_DecQuadr1<<(2*(i/3))))) )//��������޹�2
//            {
//                if( MeasureMode.RactMode2 & (F_IncQuadr1<<(2*(i/3))) )
//                {
////33                    Energy_checksum(10);
//                    PzEC_INC( (unsigned char*)&PEngy_Blk.PEngy.PhAGrp1[0], 4*(i%3)+12, 24, 1 );
//                }
//                else if( MeasureMode.RactMode2 & (F_DecQuadr1<<(2*(i/3))) )
//                {
////33                    Energy_checksum(10);
//                    PzEC_DEC( (unsigned char*)&PEngy_Blk.PEngy.PhAGrp1[0], 4*(i%3)+12, 24, 1 );
//                }
//            }

////33            Energy_checksum(11);//������4�����޹�
//            EC_INC( (unsigned char*)&PEngy_Blk.PEngy.PhAQur1[0], 4*i, 48, 1 );
//        }
//    }

//    if( FramGrpWriteFlag == 0x5A )//��������޹�
//    {
//        FramGrpWriteFlag = 0;
////33        MEM_CRCWrite( FM1_EC_PhA_Grp1, (unsigned char*)&PEngy_Blk.PEngy.PhAGrp1[0], 24, Mem_FRAM );
//    }
//    if( E2PGrpWriteFlag == 0x5A )
//    {
//        E2PGrpWriteFlag = 0;
////33        MEM_CRCWrite( E2A_EC_PhA_Grp1, (unsigned char*)&PEngy_Blk.PEngy.PhAGrp1[0], 24, Mem_E2PROMA );
//    }

//    if( FramWriteFlag == 0x5A )//������4�����޹�
//    {
//        FramWriteFlag = 0;
////33        MEM_CRCWrite( FM1_EC_PhA_Qudr1, (unsigned char*)&PEngy_Blk.PEngy.PhAQur1[0], 48, Mem_FRAM );
//    }
//    if( E2PWriteFlag == 0x5A )
//    {
//        E2PWriteFlag = 0;
////33        MEM_CRCWrite( E2A_EC_PhA_Qudr1, (unsigned char*)&PEngy_Blk.PEngy.PhAQur1[0], 48, Mem_E2PROMA );
//    }

//    for( i = 0; i < 2; i++ )//�����������ڵ���
//    {
//        while( *(&PCnt_Blk.PCnt.VAHRCnt[0]+i*5) >= g_PerKwh )
//        {
//            *(&PCnt_Blk.PCnt.VAHRCnt[0]+i*5) -= g_PerKwh;
////33            Energy_checksum(i+12);
//            EC_INC( (unsigned char*)(&VEngy_Blk.VEngy.VarPp0[0]+22*i), 0, 20, 0 );
//        }
//        
//        while( *(&PCnt_Blk.PCnt.VAHRCnt[FeeNo]+i*5) >= g_PerKwh )
//        {
//            FramWriteFlag = 0x5A;
//            *(&PCnt_Blk.PCnt.VAHRCnt[FeeNo]+i*5) -= g_PerKwh;
//// 33           Energy_checksum(i+12);
//            //EC_INC( (unsigned char*)(&VEngy_Blk.VEngy.VarPp0[0]+22*i), 0, 20, 0 );
//            if( (FeeNo > 0) && ( FeeNo <= MaxFeeNum) )//���ʵ����ۼ�
//            {
//                if( BCD_Check((&VEngy_Blk.VEngy.VarPp0[0]+i*22+4*FeeNo),4) == 0 )//��ʱ����
//                {
//                    BCD4_INC( &VEngy_Blk.VEngy.VarPp0[0]+i*22+4*FeeNo );
//                    EC_Check2 = CheckNum( &VEngy_Blk.VEngy.VarPp0[0]+i*22, 20 );//����ʼ��ַ��ʼ��У���
//                    memcpy( &VEngy_Blk.VEngy.VarPp0[0]+i*22+20, (unsigned char*)&EC_Check2, 2 );//����У���
//                }
//            }
//        }
//        if( FramWriteFlag == 0x5A )
//        {
//            FramWriteFlag = 0;
////33            MEM_CRCWrite( FM1_EC_Vahr_Pp0+22*i, (unsigned char*)(&VEngy_Blk.VEngy.VarPp0[0]+22*i), 20, Mem_FRAM );
//        }
//        if( E2PWriteFlag == 0x5A )
//        {
//            E2PWriteFlag = 0;
////33            MEM_CRCWrite( E2A_EC_Vahr_Pp0+22*i, (unsigned char*)(&VEngy_Blk.VEngy.VarPp0[0]+22*i), 20, Mem_E2PROMA );
//        }
//    }
}

/************************************************************************
* ������ ��//PowerMeasure
* ���� ��//�����ۼ�
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 
**************************************************************************/
void PowerMeasure(void)
{
    if( PowerReact1_Flag== 0x5A)
    {
        PowerReact1_Flag= 0x00;
        EC_RactGroup( MeasureMode.RactMode1, 0 );
    }
    if( PowerReact2_Flag== 0x5A)
    {
        PowerReact2_Flag= 0x00;
        EC_RactGroup( MeasureMode.RactMode2, 1 );
    }
    if( PowerRead_Flag == 0x5A )
    {
        PowerRead_Flag = 0;
        InstantPowerMeas();//˲ʱֵ
    }
		
    PulseCount();//�����ۼ�
		
		if( FLAG_EA != 0 )
       ECMeasure();//�����ۼ�
   else 
       return;
}
