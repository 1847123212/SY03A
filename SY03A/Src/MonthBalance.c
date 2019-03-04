/************************************************************************
* ��Ȩ���� (C)2016, �����������ܿƼ��ɷ����޹�˾��
*
* �ļ����ƣ� // MonthBalance.c
* �ļ���ʶ�� // 
* ����ժҪ�� //�½������
* ����˵���� // 
* ��ǰ�汾�� // V01.00
* �� �ߣ�    // yangxing
* ������ڣ� // 2016��08��06��
*
* �޸ļ�¼1��//
* �޸����ڣ� //
* �� �� �ţ� // V01
* �� �� �ˣ� //
* �޸����ݣ� //
************************************************************************/
#include "ex_func.h"
#include "ex_var.h"

unsigned char  Hour_Balance;

/************************************************************************
* ������ ��//EngRecombination
* ���� ��//�����������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void EngRecombination(void)
{
    unsigned char i;
    
    for( i = 0; i < 3; i++ )//��ϡ����򡢷����й��ܼ��ƽ�ȵ���
    {
        memcpy( g_Save_Buffer+20*i, AEngy_Blk.AEngyBuff+22*i, 20 );
    }
    for( i = 0; i < 6; i++ )//���1��2���������޹��ܼ��ƽ�ȵ���
    {
        memcpy( g_Save_Buffer+60+20*i, REngy_Blk.REngyBuff+22*i, 20 );
    }
    memcpy( g_Save_Buffer+180, (unsigned char*)&PEngy_Blk.PEngy.PhAPp[0], 24 );//�����й�����
    memcpy( g_Save_Buffer+204, (unsigned char*)&PEngy_Blk.PEngy.PhAGrp1[0], 24 );//��������޹�����
    memcpy( g_Save_Buffer+228, (unsigned char*)&PEngy_Blk.PEngy.PhAQur1[0], 48 );//����4�����޹�����
    for( i = 0; i < 2; i++ )
    {
        memcpy( g_Save_Buffer+276+20*i, VEngy_Blk.VEngyBuff+22*i, 20 );//���ڵ���
    }
}

/************************************************************************
* ������ ��//DmdRecombination
* ���� ��//�����������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void DmdRecombination(void)
{
    unsigned char i;
    
    for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
    {
        memcpy( (unsigned char*)&g_Save_Buffer[40*i],  (unsigned char*)&MaxDmd_Blk[i].MaxDmdBuff[0], 40 );
    }
}




unsigned char TimedateFormatCheck(unsigned char *buf)
{
    return 0;

}
/************************************************************************
* ������ ��//DmdSet0
* ���� ��//�������ݲ���
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void DmdSet0(void)
{
    unsigned char i;

    for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
    {
        memset( (unsigned char*)&g_Save_Buffer[40*i],  0x00, 40 );
    }
}

/************************************************************************
* ������ ��//PowerOffTimeCheck
* ���� ��//���1�ε���ʱ��Ϸ����ж�
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
unsigned char PowerOffTimeCheck(void)
{
    unsigned char TempBuffer[6];

    memset( (unsigned char*)&TempBuffer[0], 0x00, 6 );
    MEM_Read( (unsigned char*)&TempBuffer[0], E2A_PowerOffTim, 6, Mem_E2PROMA );//��ȡ���һ�ε���ʱ��

    if( TimedateFormatCheck((unsigned char*)&TempBuffer[1]) != 0  )//�ж��ꡢ�¡��ա�ʱ�������Ƿ�Ϸ�
        return 1;

    return 0;
}

/************************************************************************
* ������ ��//EngryBalanceProc
* ���� ��//�������㹦��
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void EngryBalanceProc(void)
{
    unsigned char Ptr;
    unsigned short Addr;
    unsigned long Num;
    
    if( Timer.EngSaveCnt < 5 )
    {
        MEM_Read( (unsigned char*)&Ptr, FM1_LastM_Engy_Ptr, 1, Mem_FRAM );//������������ָ��
        Addr = LastM_Act_Pz0 + EngyBalan_Len*Ptr;//�ôν������ʼ��ַ

        if( Timer.EngSaveCnt <= 3 )//ǰ4��ÿ��д64�ֽ�
        {
            MEM_Write( Addr+64*Timer.EngSaveCnt, (unsigned char*)&g_Save_Buffer[64*Timer.EngSaveCnt], 64, Mem_E2PROMA);
        }
        else if( Timer.EngSaveCnt == 4 )//���һ��д60�ֽ�
        {
            MEM_Write( Addr+64*Timer.EngSaveCnt, (unsigned char*)&g_Save_Buffer[64*Timer.EngSaveCnt], 60, Mem_E2PROMA);

            if(( Flag.Power & F_PwrOffTim )== 0 )//������ϵ�������ղ�����
            {
                Addr = E2A_LAST_1M_DATA + 4*Ptr;//�ôν������ʼ��ַ
                if( Flag.BalanBDong & F_BalanBD )         
                    MEM_Read( (unsigned char*)&Clockbak.Hour, E2A_PowerOffTim+2, 4, Mem_E2PROMA );//��ȡ����ʱ��
                MEM_Write( Addr, (unsigned char*)&Clockbak.Hour, 4, Mem_E2PROMA);//������ʱ��
                MEM_Write( E2A_LAST1M_DATA, (unsigned char*)&Clockbak.Hour, 4, Mem_E2PROMA);//������ʱ��
            }
            else
            {
                Addr = E2A_LAST_1M_DATA + 4*Ptr;//�ôν������ʼ��ַ
                MEM_Write( Addr, (unsigned char*)&ClockBlock.Clock.Hour, 4, Mem_E2PROMA);//����ʱ��
                MEM_Write( E2A_LAST1M_DATA, (unsigned char*)&ClockBlock.Clock.Hour, 4, Mem_E2PROMA);//����ʱ��
            }
            Ptr++;//ָ������
            if( Ptr >= EngyBalan_MNum )	Ptr = 0;
            MEM_DBWrite( FM1_LastM_Engy_Ptr, (unsigned char*)&Ptr, 1, Mem_FRAM );

            MEM_Read( (unsigned char*)&Num, LastM_Engy_Num, 4, Mem_E2PROMA );
            if( Num < 0x00999999 )//��������ۼ�
            {
                BCDX_INC( (unsigned char*)&Num, 4 );
                MEM_Write( LastM_Engy_Num, (unsigned char*)&Num, 4, Mem_E2PROMA );
            }
            Request.Flag_EngSave = 0x00;//����ת���־
        }
        Timer.EngSaveCnt++;
        if( Timer.EngSaveCnt >= 5 )
        {
            Timer.EngSaveCnt = 0;
            Flag.BalanRequest |= F_BalanEngEnd;//����ת�����
        }
    }
}

/************************************************************************
* ������ ��//BalanceCheck
* ���� ��//�����ϵ粹��ȷ��
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void BalanceCheck(void)
{
    unsigned char TempBuffer[6];
    unsigned char TempBuffer2[4];
    MEM_Read( (unsigned char*)&Flag.BalanBDong, BalanSave_Flag, 1, Mem_FRAM );
    MEM_Read( (unsigned char*)&TempBuffer[0], E2A_PowerOffTim, 6, Mem_E2PROMA );//��ȡ���һ�ε���ʱ��
     
    MEM_Read( (unsigned char*)&TempBuffer2[0], E2A_LAST1M_DATA, 4, Mem_E2PROMA );//�ϴν���ʱ��
    if( memcmp((unsigned char*)&TempBuffer2[0],(unsigned char*)&TempBuffer[2],4)==0 )
        return;
     
    if( (TempBuffer[0] == 0x00) &&(TempBuffer[1] ==0x00) &&(Flag.BalanBDong & F_ReBalanBD) )//�������
    {
        //�½���
        MEM_Read( g_Buffer, Balance_Time1, 6, Mem_E2PROMA );//����������1��2��3
        if( (TempBuffer[2] == g_Buffer[0]) && (TempBuffer[3] == g_Buffer[1]) )//������1
        {           
            Request.Flag_EngSave = 0x5A;//����ת���־
            Request.Flag_Dmd1Save = 0x5A;//����ת���־
        }
        else if( (TempBuffer[2] == g_Buffer[2]) && (TempBuffer[3] == g_Buffer[3]) )//������2
        {
            Request.Flag_EngSave = 0x5A;//����ת���־            
        }
        else if( (TempBuffer[2] == g_Buffer[4]) && (TempBuffer[3] == g_Buffer[5]) )//������3
        {
            Request.Flag_EngSave = 0x5A;//����ת���־
        }
        memcpy( g_Buffer,(unsigned char*)&Request.Flag_EngSave,3 );
        MEM_DBWrite( EngSave_Flag, (unsigned char*)&g_Buffer[0], 3,Mem_FRAM );
    }
}

/************************************************************************
* ������ ��//EngryBalanceAgain
* ���� ��//���������ϵ粹������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void EngryBalanceCheck(void)
{
    MEM_Read( (unsigned char*)&Request.Flag_EngSave, EngSave_Flag, 1, Mem_FRAM );
    if( Request.Flag_EngSave == 0x5A )
    {
        if( PowerOffTimeCheck() != 0 )//�����1�ε���ʱ��Ƿ��˳�
            return;

        EngRecombination();//�����������
        Timer.EngSaveCnt = 0;//����ת�������
    }
    while( Request.Flag_EngSave == 0x5A )
    {
        Flag.BalanBDong |= F_BalanBD;
        EngryBalanceProc();//����ת��
        if( Flag.BalanRequest & F_BalanEngEnd )//����ת�����ݷ�5��д�꣬�ܹ�д316���ֽ�
        {
            Flag.BalanBDong &= ~F_BalanBD;
            Flag.BalanRequest &= ~F_BalanEngEnd;
            Flag.BalanBDong &= ~F_ReBalanBD;
            Flag.BalanEngNum =0x00;
            MEM_Write( E2A_EngNum, (unsigned char*)&Flag.BalanEngNum, 1, Mem_E2PROMA );            
            CycleEngry_Resume();//����ת������¼��������ۼ��õ��� 
            MEM_Write( E2A_LastZ_Act_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 4, Mem_E2PROMA );//��ǰ����й��ܵ���д����1��������й��ܵ���    
            memcpy( (unsigned char*)&CycEnery.LastPz0[0], (unsigned char*)&CycEnery.CurntPz0[0], 4 );//��ǰ�ۼ�����й��ܵ���д����1�����ۼ�����й��ܵ���
            MEM_Write( E2A_LastZ2_Act_Pz0, (unsigned char*)&CycEnery.LastPz0[0], 4, Mem_E2PROMA );//ת���ۼ��õ���
            memset( (unsigned char*)&CycEnery.CurntPz0[0], 0x00, 4 );               
        }
    }
}


/************************************************************************
* ������ ��//DmdBalanceProc_ADD
* ���� ��//���������㹦��
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void DmdBalanceProc_ADD(void)
{
    unsigned char Ptr;
    unsigned short Addr;
    unsigned long Num;
    MEM_Read( (unsigned char*)&Ptr, FM1_LastM_Dmd_Ptr, 1, Mem_FRAM );//������������ָ��
    Addr = LastM_TotalDmdF + DmdBalan_Len*Ptr;//�ôν������ʼ��ַ

    MEM_Write( Addr, (unsigned char*)&g_Save_Buffer[0], 192, Mem_E2PROMA);
    MEM_Write( Addr+192, (unsigned char*)&g_Save_Buffer[192], 128, Mem_E2PROMA);

    Ptr++;//ָ������
    if( Ptr >= DmdBalan_MNum )	Ptr = 0;
    MEM_DBWrite( FM1_LastM_Dmd_Ptr, (unsigned char*)&Ptr, 1, Mem_FRAM );
    
    MEM_Read( (unsigned char*)&Num, LastM_Dmd_Num, 4, Mem_E2PROMA );
    if( Num < 0x00999999 )//��������ۼ�
    {
        BCDX_INC( (unsigned char*)&Num, 4 );
        MEM_Write( LastM_Dmd_Num, (unsigned char*)&Num, 4, Mem_E2PROMA );
    }
}
/************************************************************************
* ������ ��//DmdBalanceProc
* ���� ��//�������㹦��
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void DmdBalanceProc(void)
{
    unsigned char Ptr;
    unsigned short Addr;
    unsigned long Num;

    if( Timer.DmdSaveCnt < 5 )
    {
        MEM_Read( (unsigned char*)&Ptr, FM1_LastM_Dmd_Ptr, 1, Mem_FRAM );//������������ָ��
        Addr = LastM_TotalDmdF + DmdBalan_Len*Ptr;//�ôν������ʼ��ַ

        if( Timer.DmdSaveCnt <= 4 )//ÿ��д64�ֽ�
        {
            MEM_Write( Addr+64*Timer.DmdSaveCnt, (unsigned char*)&g_Save_Buffer[64*Timer.DmdSaveCnt], 64, Mem_E2PROMA);
        }

        if( Timer.DmdSaveCnt == 4 )
        {
            Ptr++;//ָ������
            if( Ptr >= DmdBalan_MNum )	Ptr = 0;
            MEM_DBWrite( FM1_LastM_Dmd_Ptr, (unsigned char*)&Ptr, 1, Mem_FRAM );
            
            MEM_Read( (unsigned char*)&Num, LastM_Dmd_Num, 4, Mem_E2PROMA );
            if( Num < 0x00999999 )//��������ۼ�
            {
                BCDX_INC( (unsigned char*)&Num, 4 );
                MEM_Write( LastM_Dmd_Num, (unsigned char*)&Num, 4, Mem_E2PROMA );
            }
        }

        Timer.DmdSaveCnt++;
        if( Timer.DmdSaveCnt >= 5 )
        {
            Timer.DmdSaveCnt = 0;
            Flag.BalanRequest |= F_BalanDmdEnd;//����ת�����
        }
    }
}

/************************************************************************
* ������ ��//DmdBalanceAgain
* ���� ��//���������ϵ粹������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void DmdBalanceCheck(void)
{
    MEM_Read( (unsigned char*)&Request.Flag_Dmd1Save, Dmd1Save_Flag, 2, Mem_FRAM );

//    if( (Request.Flag_Dmd1Save == 0x5A) || (Request.Flag_Dmd2Save == 0x5A) )
    if( Request.Flag_Dmd1Save == 0x5A )
    {
        if( PowerOffTimeCheck() != 0 )//�����1�ε���ʱ��Ƿ��˳�
            return;
    }

    if( Request.Flag_Dmd1Save == 0x5A )//������1��������
    {
        DmdRecombination();//�����������
        DmdBalanceProc_ADD();
        Demand_ClearRAM();//RAM�������������
        Demand_ClearE2();//E2�е�ǰ�����������
        
        Request.Flag_Dmd1Save = 0x00;//����ת���־
        MEM_DBWrite( Dmd1Save_Flag, (unsigned char*)&Request.Flag_Dmd1Save, 1,Mem_FRAM );
    }

//    if( Request.Flag_Dmd2Save == 0x5A )//������2��������
//    {
//        for( i = 0; i < 320; i++ )
//        {
//            g_Save_Buffer[i] = 0xFF;
//        }
//        DmdBalanceProc_ADD();
//        Request.Flag_Dmd2Save = 0x00;//����ת���־
//        MEM_DBWrite( Dmd2Save_Flag, (unsigned char*)&Request.Flag_Dmd2Save, 1,Mem_FRAM );
//    }
}

/************************************************************************
* ������ ��//BalanceTotalNum
* ���� ��//������Ҫ�����������Ŀ
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��// 
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
unsigned char BalanceTotalNum(void)
{
    unsigned char TempBuffer[6];
    unsigned char TempBuffer2[6];
    unsigned char BalanBuffer[6];
    unsigned char LastMonth = 0;
    unsigned char CurtMonth = 0;
    unsigned char BalanMonth = 0;
    unsigned char BalanNum = 0;
    unsigned char LastYear = 0;
    unsigned char Num = 0;
    unsigned char EngCnt0 = 0;
    unsigned char EngCnt1 = 0;
    unsigned char LastEngCnt = 0;
    unsigned char BalanceEng1_Cnt = 0;
    unsigned char i;
    unsigned int Tmp0 = 0;
    unsigned int Tmp1 = 0;
    unsigned long NowTmr,BalanceTmr,PowOffTmr;

    memset( (unsigned char*)&TempBuffer[0], 0x00, 6 );
    memset( (unsigned char*)&TempBuffer2[0], 0x00, 6 );
    memset( (unsigned char*)&BalanBuffer[0], 0x00, 6 );
    if(Flag.TimeChgBill == 0x5A)
      {
        MEM_Read( (unsigned char*)&TempBuffer[0], E2A_Timechange, 6, Mem_E2PROMA );//Уʱǰ��¼ʱ��
        MEM_Read( (unsigned char*)&Clockbak.Sec, E2A_TimechangeNOW, 6, Mem_E2PROMA );//Уʱ���¼ʱ��
      }
    else
      {
        MEM_Read( (unsigned char*)&TempBuffer[0], E2A_PowerOffTim, 6, Mem_E2PROMA );//��ȡ���һ�ε���ʱ��
        memcpy( (unsigned char*)&Clockbak.Sec, (unsigned char*)&ClockBlock.Clock.Sec, 6);//��ǰʱ��
      }

    if( TimedateFormatCheck((unsigned char*)&TempBuffer[1]) != 0  )//�ж��ꡢ�¡��ա�ʱ���֡����Ƿ�Ϸ�
        return 1;

     if( TempBuffer[5] > Clockbak.Year )
         return 1;
     if(( TempBuffer[5] == Clockbak.Year )&&( TempBuffer[4] > Clockbak.Mon ))
         return 1;
     if(( TempBuffer[5] == Clockbak.Year )&&( TempBuffer[4] == Clockbak.Mon )&&( TempBuffer[3] > Clockbak.Day ))
       return 1;
     if(( TempBuffer[5] == Clockbak.Year )&&( TempBuffer[4] == Clockbak.Mon )&&( TempBuffer[3] == Clockbak.Day )&&( TempBuffer[2] > Clockbak.Hour ))
       return 1;
     /*
     MEM_Read( (unsigned char*)&TempBuffer2[0], E2A_LAST1M_DATA, 4, Mem_E2PROMA );//�ϴν���ʱ��
     if( memcmp((unsigned char*)&TempBuffer2[0],(unsigned char*)&TempBuffer[2],4)==0 )
         return 1;
     */
    //��������¸���
    LastMonth = BCD_BYTE(TempBuffer[4]);
    CurtMonth = BCD_BYTE(Clockbak.Mon);
    if( TempBuffer[5] == Clockbak.Year )//ͬ��
        BalanMonth = CurtMonth - LastMonth;//������¸���
    else
    {
        LastYear = TempBuffer[5];
        BCDX_INC((unsigned char*)&LastYear,1);
        if( LastYear == Clockbak.Year )//ȥ�����
            BalanMonth = CurtMonth + 12 - LastMonth;
        else//ǰ����߸���
            BalanMonth = 13;
    }
    Flag.MonitorNum = BalanMonth;//��ѹ�ϸ�����Ҫ���ĸ���

    //�����������Ч����
    NowTmr = ((unsigned long)BCD_BYTE(Clockbak.Day))*86400+((unsigned long)BCD_BYTE(Clockbak.Hour))*3600+((unsigned long)BCD_BYTE(Clockbak.Min))*60+(unsigned long)BCD_BYTE(Clockbak.Sec);//��ǰ�·�ʱ����ʱ����
    PowOffTmr = ((unsigned long)BCD_BYTE(TempBuffer[3]))*86400+((unsigned long)BCD_BYTE(TempBuffer[2]))*3600+((unsigned long)BCD_BYTE(TempBuffer[1]))*60+(unsigned long)BCD_BYTE(TempBuffer[0]);//���һ�ε�����ʱ����
    MEM_Read( (unsigned char*)&BalanBuffer[0], Balance_Time1, 6, Mem_E2PROMA );
    for( i = 0; i < 3; i++ )
    {
        Tmp0 = ((unsigned int)(BalanBuffer[2*i+1]) << 8) + (unsigned int)BalanBuffer[2*i];//������ʱ
        Tmp1 = ((unsigned int)(BalanBuffer[2*((i+1)%3)+1]) << 8) + (unsigned int)BalanBuffer[2*((i+1)%3)];

        if((BalanBuffer[2*i+1] > 0x28) || (BalanBuffer[2*i+1] == 0) ||(BalanBuffer[2*i] > 0x23))//�жϽ���ʱ���ʽ
            continue;
        if(( BalanMonth > 1 )&&(BalanBuffer[1] <= 0x28)&&(BalanBuffer[0] <= 0x23))
        {
          Flag.BalanDmdClr = 0x5A;
        }
        BalanceTmr = ((unsigned long)BCD_BYTE(BalanBuffer[2*i+1]))*86400+((unsigned long)BCD_BYTE(BalanBuffer[2*i]))*3600;//������ʱ

        if( Tmp0 != Tmp1 )
        {
            BalanNum++;//������Ч��������
            if((TempBuffer[4] == Clockbak.Mon) && (TempBuffer[5] == Clockbak.Year))//��ͬ�������µ�
            {
                if( (BalanceTmr > PowOffTmr) && (NowTmr >= BalanceTmr) )//���㱾�¿��������
                {
                    Num++;
                    if( i == 0 )//�������1�����������ǰ��������
                    {
                        Flag.BalanDmdClr = 0x5A;
                        BalanceEng1_Cnt++;
                    }
                }
            }
            else
            {
                if( NowTmr >= BalanceTmr )//�ϵ�����¿��������
                {
                    Num++;
                    if( i == 0 )//�������1�����������ǰ��������
                    {
                        Flag.BalanDmdClr = 0x5A;
                        BalanceEng1_Cnt++;
                    }
                }

                if( PowOffTmr < BalanceTmr )//�����Ǹ��¿��������
                {
                    Num++;
                    if( i == 0 )//�������1�����������ǰ��������
                    {
                        Flag.BalanDmdClr = 0x5A;
                        BalanceEng1_Cnt++;
                    }
                }
            }
        }
        else
        {
            EngCnt0++;//������������ͬ�������������ն���ͬ��EngCnt0��3
            if((TempBuffer[4] == Clockbak.Mon) && (TempBuffer[5] == Clockbak.Year))//��ͬ�������µ�
            {
                if( (BalanceTmr > PowOffTmr) && (NowTmr >= BalanceTmr) )
                {
                    EngCnt1++;
                    if( i == 0 )//�������1�����������ǰ��������
                    {
                        Flag.BalanDmdClr = 0x5A;
                        BalanceEng1_Cnt++;
                    }
                }
            }
            else
            {
                if( PowOffTmr < BalanceTmr )//�����Ǹ��¿��������
                {
                    LastEngCnt++;
                    if( i == 0 )//�������1�����������ǰ��������
                    {
                        Flag.BalanDmdClr = 0x5A;
                        BalanceEng1_Cnt++;
                    }
                }
                if( NowTmr >= BalanceTmr )//�ϵ�����¿��������
                {
                    EngCnt1++;
                    if( i == 0 )//�������1�����������ǰ��������
                    {
                        Flag.BalanDmdClr = 0x5A;
                        BalanceEng1_Cnt++;
                    }
                }
            }
            continue;
        }
    }

    if( EngCnt0 == 3 )//3�������ն����
        BalanNum = 1;//��Ч��������Ϊ1

    if( EngCnt1 == 3 )//���¿��������Ϊ1
        Num++;

    if( LastEngCnt == 3 )//�����Ǹ��¿��������Ϊ1
        Num++;

    g_BalanNum = BalanNum;
    //������Ҫ��������ܸ���
    if( BalanMonth >= 1 )//�����Ǹ����Ѿ���������������
        BalanMonth--;
    BalanceEng1_Cnt += BalanMonth;
    Flag.BalanceEng1_Num += BalanceEng1_Cnt;//��������1������
    if( Flag.BalanceEng1_Num > 2 ) 
      Flag.BalanceEng1_Num = 2;
    Flag.BalanceEng1_MDNum = Flag.BalanceEng1_Num;
    
    Num += BalanMonth*BalanNum;
    if( Num > 12 )
        Num = 12;

    Flag.BalanEngNum += Num;//��������������
    if( Flag.BalanEngNum > 12 )
        Flag.BalanEngNum = 12;

    Flag.BalanDmdNum += Num;//��������������
    if( Flag.BalanDmdNum > 12 )
        Flag.BalanDmdNum = 12;
    
    MEM_Write( E2A_EngNum, (unsigned char*)&Flag.BalanEngNum, 5, Mem_E2PROMA );
    return 0;
}

/************************************************************************
* ������ ��//PowOnBalance
* ���� ��//�ϵ�/�޸�ʱ�Ӳ�����
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
unsigned char PowOnBalance(void)
{
    unsigned char j=0,i=0,Tempnum=0,CNT = 0;
    unsigned char TempBufferA[6];
    unsigned char TempBufferB[6];
    unsigned char TempBuffer[4];
    unsigned char TempBuffer2[4];
    unsigned short CurrentTime;
    unsigned short LastBalanceTmr;
    unsigned short BillTime1;
    unsigned short BillTime2;
    unsigned short BillTime3;
    unsigned short Lng2;
    unsigned char DMDREC_CNT;
    unsigned char Temp,lastmonth;

    MEM_Read( (unsigned char*)&Flag.BalanEngNum, E2A_EngNum, 3, Mem_E2PROMA );
    MEM_Read( (unsigned char*)&Flag.BalanceEng1_Num, E2A_Eng1_Num, 1, Mem_E2PROMA );

    Flag.BalanBDong |= F_ReBalanBD;
    Flag.Power &=~ F_PwrOffTim3;
    if( (Flag.BalanEngNum < 12) || (Flag.BalanDmdNum < 12) )
    {
        if( BalanceTotalNum() == 1 )//����Ҫ����ĸ���
        {
            return 0;
        }
    }

    if( (Flag.BalanEngNum > 12) || (Flag.BalanDmdNum > 12) )
    {
        Flag.BalanEngNum = 0;
        Flag.BalanDmdNum = 0;
        Flag.BalanDmdClr = 0;
        Flag.BalanceEng1_Num = 0;
        MEM_Write( E2A_EngNum, (unsigned char*)&Flag.BalanEngNum, 3, Mem_E2PROMA );
        MEM_Write( E2A_Eng1_Num, (unsigned char*)&Flag.BalanceEng1_Num, 1, Mem_E2PROMA );
        return 0;
    }

    MEM_Read( TempBufferA, Balance_Time1, 6, Mem_E2PROMA );//����������1��2��3         
    BillTime1 = BCD_BYTE(TempBufferA[1])*24 + BCD_BYTE(TempBufferA[0]);
    BillTime2 = BCD_BYTE(TempBufferA[3])*24 + BCD_BYTE(TempBufferA[2]);
    BillTime3 = BCD_BYTE(TempBufferA[5])*24 + BCD_BYTE(TempBufferA[4]);
    EngRecombination();//�����������
    Timer.EngSaveCnt = 0;//����ת�������
    while( Flag.BalanEngNum > 0 )//��������������
    {
        WatchDog_Clear;//�忴�Ź�
        Request.Flag_EngSave = 0x5A;//����ת���־
        if( PowerOff_Ack() == 1 )
            return 1;
        if( Timer.EngSaveCnt == 4 )
        {
            j++;
            if( j==1 )
            {
                MEM_Read( (unsigned char*)&TempBufferB[2], E2A_PowerOffTim+2, 4, Mem_E2PROMA );//��ȡ���һ�ε���ʱ��
                if( Flag.BalanEngNum ==12)
                {
                    Lng2 = ((unsigned short)BCD_BYTE(ClockBlock.Clock.Day))*24+((unsigned short)BCD_BYTE(ClockBlock.Clock.Hour));//��ǰʱ��
                    memcpy( (unsigned char*)&TempBufferB[2], (unsigned char*)&ClockBlock.Clock.Hour, 4);//��ǰʱ��  
                    switch( g_BalanNum )//��Ч��������
                     {
                       case 3:                            
                           if(( Lng2 >= BillTime1 )&&( Lng2 >= BillTime2 )&&( Lng2 >= BillTime3 ))
                               lastmonth=3;
                           else
                           {
                               if( Lng2 < BillTime3 ) {TempBufferB[2]=TempBufferA[2];TempBufferB[3]=TempBufferA[3];}
                               if( Lng2 < BillTime2 ) {TempBufferB[2]=TempBufferA[0];TempBufferB[3]=TempBufferA[1];}
                               if( Lng2 < BillTime1 ) {TempBufferB[2]=0;TempBufferB[3]=0;}
                               lastmonth=4;   
                           }
                            break;
                       case 2: 
                           if((( Lng2 >= BillTime1 )&&( Lng2 >= BillTime2 ))||(( Lng2 >= BillTime2 )&&( Lng2 >= BillTime3 ))||(( Lng2 >= BillTime1 )&&( Lng2 >= BillTime3 )))
                               lastmonth=5;
                           else
                           {
                               if(BillTime3 ==2475 )
                               {
                                   if(Lng2 >=BillTime1)
                                   {
                                       TempBufferB[2]=TempBufferA[0];
                                       TempBufferB[3]=TempBufferA[1];
                                   }
                                   else
                                   {
                                       TempBufferB[2]=0;
                                       TempBufferB[3]=0;
                                   }
                               }
                               lastmonth=6;   
                           }                               
                            break;
                       case 1: 
                           if(( Lng2 >= BillTime1 )||( Lng2 >= BillTime2 )||( Lng2 >= BillTime3 ))
                               lastmonth=11;
                           else
                               lastmonth=12;                                         
                            break;                              
                     }
                    if((lastmonth==3)||(lastmonth==5)||(lastmonth==11)||(lastmonth==12))
                    {
                        TempBufferB[2]=0;
                        TempBufferB[3]=0;
                    }
                    LastBalanceTmr = (unsigned short)BCD_BYTE(TempBufferB[4]);//��
                    if(LastBalanceTmr <= lastmonth)
                    {
                        Temp = BYTE_BCD(BCD_BYTE(TempBufferB[5])-1);  
                        TempBufferB[5] = Temp;
                        Temp = (12-(lastmonth - LastBalanceTmr));
                        TempBufferB[4] = BYTE_BCD(Temp);
                    }
                    else 
                    {
                        Temp = ( LastBalanceTmr - lastmonth );
                        TempBufferB[4] = BYTE_BCD(Temp);
                    }
                    memcpy( (unsigned char*)&TempBuffer2, (unsigned char*)&TempBufferB[2], 4 );//��������������
                }
            }
            else 
                MEM_Read( (unsigned char*)&TempBufferB[2], E2A_LAST1M_DATA, 4, Mem_E2PROMA );//�ϴν���ʱ��
            LastBalanceTmr = ((unsigned short)BCD_BYTE(TempBufferB[3]))*24+(unsigned short)BCD_BYTE(TempBufferB[2]);//������ʱ

             switch( g_BalanNum )//��Ч��������
             {
               case 2:
                   memcpy( (unsigned char*)&Clockbak.Hour, (unsigned char*)&TempBufferB[2], 4 );
                   if((( TempBufferA[3] ==0x99)||( TempBufferA[2] ==0x99))||( BillTime1 == BillTime2 ))//������2��Ч
                   {
                       if( BillTime1 < BillTime3 )//1С�ڵ���3
                       {
                           if( LastBalanceTmr < BillTime1 )
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                           }
                           else if(( LastBalanceTmr >= BillTime1 )&&( LastBalanceTmr < BillTime3 ))//���ڽ�����1���򲹽�����3
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                           }
                           else 
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           }
                       }
                       else
                       {
                           if( LastBalanceTmr < BillTime3 )
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                           }
                           else if(( LastBalanceTmr >= BillTime3 )&&( LastBalanceTmr < BillTime1 ))//���ڽ�����3���򲹽�����1
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                           }
                           else 
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           }
                       }
                   } 
                   else if((( TempBufferA[5] ==0x99)||( TempBufferA[4] ==0x99))||( BillTime1 == BillTime3 )||( BillTime2 == BillTime3 ))//������3��Ч
                   {
                       if( BillTime1 < BillTime2 )//1С�ڵ���2
                       {
                           if( LastBalanceTmr < BillTime1 )
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                           }
                           else if(( LastBalanceTmr >= BillTime1 )&&( LastBalanceTmr < BillTime2 ))//���ڽ�����1���򲹽�����2
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                           }
                           else 
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           }
                       }
                       else
                       {
                           if( LastBalanceTmr < BillTime2 )
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                           }
                           else if(( LastBalanceTmr >= BillTime2 )&&( LastBalanceTmr < BillTime1 ))//���ڽ�����2���򲹽�����1
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                           }
                           else 
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           }
                       }
                   } 
                   else if(( TempBufferA[1] ==0x99)||( TempBufferA[0] ==0x99))//������1��Ч
                   {
                       if( BillTime2 <= BillTime3 )//2С�ڵ���3
                       {                       
                           if( LastBalanceTmr < BillTime2 )
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                           }
                           else if(( LastBalanceTmr >= BillTime2 )&&( LastBalanceTmr < BillTime3 ))//���ڽ�����2���򲹽�����3
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                           }
                           else 
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           }
                       }
                       else
                       {
                           if( LastBalanceTmr < BillTime3 )
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                           }
                           else if(( LastBalanceTmr >= BillTime3 )&&( LastBalanceTmr < BillTime2 ))//���ڽ�����2���򲹽�����3
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                           }
                           else 
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           } 
                       }
                   }  
                    break;
                   case 3:
                       memcpy( (unsigned char*)&Clockbak.Hour, (unsigned char*)&TempBufferB[2], 4 );
                       if(( BillTime1 < BillTime2 )&&( BillTime2 < BillTime3 ))//1<2<3
                       {  
                           if( LastBalanceTmr < BillTime1 )
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                           }
                           else if(( LastBalanceTmr >= BillTime1 )&&( LastBalanceTmr < BillTime2 ))//���ڽ�����1���򲹽�����3
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                           }
                           else if(( LastBalanceTmr >= BillTime2 )&&( LastBalanceTmr < BillTime3 ))
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                           }
                           else
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           }
                       }
                       else if(( BillTime1 < BillTime3 )&&( BillTime3 < BillTime2 ))//1<3<2
                       {
                           if( LastBalanceTmr < BillTime1 )
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                           }
                           else if(( LastBalanceTmr >= BillTime1 )&&( LastBalanceTmr < BillTime3 ))//���ڽ�����1���򲹽�����3
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                           }
                           else if(( LastBalanceTmr >= BillTime3 )&&( LastBalanceTmr < BillTime2 ))
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                           }
                           else
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           }                           
                       }
                       else if((BillTime2 < BillTime1)&&(BillTime1 < BillTime3))//2<1<3
                       {
                           if( LastBalanceTmr < BillTime2 )
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                           }
                           else if(( LastBalanceTmr >= BillTime2 )&&( LastBalanceTmr < BillTime1 ))//���ڽ�����1���򲹽�����3
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                           }
                           else if(( LastBalanceTmr >= BillTime1 )&&( LastBalanceTmr < BillTime3 ))
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                           }
                           else
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           }                           
                       } 
                       else if((BillTime2 < BillTime3)&&(BillTime3 < BillTime1))//2<3<1
                       {
                           if( LastBalanceTmr < BillTime2 )
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                           }
                           else if(( LastBalanceTmr >= BillTime2 )&&( LastBalanceTmr < BillTime3 ))//���ڽ�����1���򲹽�����3
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                           }
                           else if(( LastBalanceTmr >= BillTime3 )&&( LastBalanceTmr < BillTime1 ))
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                           }
                           else
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           }                           
                       }  
                       else if((BillTime3 < BillTime1)&&(BillTime1 < BillTime2))//3<1<2
                       {
                           if( LastBalanceTmr < BillTime3 )
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                           }
                           else if(( LastBalanceTmr >= BillTime3 )&&( LastBalanceTmr < BillTime1 ))//���ڽ�����1���򲹽�����3
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                           }
                           else if(( LastBalanceTmr >= BillTime1 )&&( LastBalanceTmr < BillTime2 ))
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                           }
                           else
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           }                           
                       } 
                       else if((BillTime3 < BillTime2)&&(BillTime2 < BillTime1))//3<2<1
                       {
                           if( LastBalanceTmr < BillTime3 )
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                           }
                           else if(( LastBalanceTmr >= BillTime3 )&&( LastBalanceTmr < BillTime2 ))//���ڽ�����1���򲹽�����3
                           {
                               Clockbak.Hour = TempBufferA[2];
                               Clockbak.Day = TempBufferA[3];
                           }
                           else if(( LastBalanceTmr >= BillTime2 )&&( LastBalanceTmr < BillTime1 ))
                           {
                               Clockbak.Hour = TempBufferA[0];
                               Clockbak.Day = TempBufferA[1];
                           }
                           else
                           {
                               Clockbak.Hour = TempBufferA[4];
                               Clockbak.Day = TempBufferA[5];
                               Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                               if(Clockbak.Mon > 0x12)
                               {
                                  Clockbak.Mon = 1; 
                                  Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                               }
                           }                           
                       }                       
                 break;
               default:
                      memcpy( (unsigned char*)&Clockbak.Hour, (unsigned char*)&TempBufferB[2], 4 );
                      if(( TempBufferA[1] !=0x99)&&( TempBufferA[0] !=0x99))                                 
                        {
                            Clockbak.Hour = TempBufferA[0];
                            Clockbak.Day = TempBufferA[1];
                            if( LastBalanceTmr >= BillTime1 )
                            {
                                 Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                                 if(Clockbak.Mon > 0x12)
                                 {
                                    Clockbak.Mon = 1; 
                                    Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                                 }
                            }
                         }
                       else if(( TempBufferA[3] !=0x99)&&( TempBufferA[2] !=0x99))                                  
                        {
                            Clockbak.Hour = TempBufferA[2];
                            Clockbak.Day = TempBufferA[3];
                            if( LastBalanceTmr >= BillTime2 )
                            {
                                 Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                                 if(Clockbak.Mon > 0x12)
                                 {
                                    Clockbak.Mon = 1; 
                                    Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1); 
                                 }
                            }
                         }
                       else if(( TempBufferA[5] !=0x99)&&( TempBufferA[4] !=0x99))                                 
                        {
                            Clockbak.Hour = TempBufferA[4];
                            Clockbak.Day = TempBufferA[5];
                            if( LastBalanceTmr >= BillTime3 )
                            {
                                 Clockbak.Mon = BYTE_BCD(BCD_BYTE(TempBufferB[4])+1);     
                                 if(Clockbak.Mon > 0x12)
                                 {
                                    Clockbak.Mon = 1; 
                                    Clockbak.Year = BYTE_BCD(BCD_BYTE(TempBufferB[5])+1); 
                                 }
                            }
                         }           
                    break;
             }
        }
        //Request.Flag_EngSave = 0x5A;//����ת���־
        EngryBalanceProc();//����ת��
        if( Flag.BalanRequest & F_BalanEngEnd )//����ת�����ݷ�5��д�꣬�ܹ�д316���ֽ�
        {
            //Request.Flag_EngSave = 0x00;//����ת���־
            Flag.BalanRequest &= ~F_BalanEngEnd;
            Timer.EngSaveCnt = 0;
            Flag.BalanEngNum--;
            MEM_Write( E2A_EngNum, (unsigned char*)&Flag.BalanEngNum, 1, Mem_E2PROMA );             
            CycleEngry_Resume();//���������ۼ��õ���  
            MEM_Write( E2A_LastZ_Act_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 4, Mem_E2PROMA );//��ǰ����й��ܵ���д����1��������й��ܵ���    
            memcpy( (unsigned char*)&CycEnery.LastPz0[0], (unsigned char*)&CycEnery.CurntPz0[0], 4 );//��ǰ�ۼ�����й��ܵ���д����1�����ۼ�����й��ܵ���
            MEM_Write( E2A_LastZ2_Act_Pz0, (unsigned char*)&CycEnery.LastPz0[0], 4, Mem_E2PROMA );//ת���ۼ��õ���
            memset( (unsigned char*)&CycEnery.CurntPz0[0], 0x00, 4 );                
            if( Flag.BalanEngNum ==0 )
                Flag.BalanBDong &= ~ F_ReBalanBD;
        }
        //MEM_DBWrite( EngSave_Flag, (unsigned char*)&Request.Flag_EngSave, 1,Mem_FRAM );
    }
    while( Flag.BalanceEng1_Num > 0 )//��������1�Ķ�������
    {
        Flag.BalanceEng1_Num--;
        MonthEngry_Resume();//�����¶��ۼ��õ���  
        MEM_Write( E2A_Eng1_Num, (unsigned char*)&Flag.BalanceEng1_Num, 1, Mem_E2PROMA );
        MEM_Write( E2A_LastM_Act_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 4, Mem_E2PROMA );//��ǰ����й��ܵ���д����1������й��ܵ��� 
        memcpy( (unsigned char*)&MonthEnery.LastPz0[0], (unsigned char*)&MonthEnery.CurntPz0[0], 4 );//��ǰ�ۼ�����й��ܵ���д����1���ۼ�����й��ܵ���
        MEM_Write( E2A_LastM2_Act_Pz0, (unsigned char*)&MonthEnery.LastPz0[0], 4, Mem_E2PROMA );//ת���ۼ��õ���
        memset( (unsigned char*)&MonthEnery.CurntPz0[0], 0x00, 4 );        
    }

    Timer.DmdSaveCnt = 0;//����ת�������
        
    if(Flag.TimeChgBill == 0x5A)
      {
        MEM_Read( (unsigned char*)&TempBufferB[0], E2A_Timechange, 6, Mem_E2PROMA );//Уʱǰ��¼ʱ��
      }
    else if( Flag.BalanDmdNum == 12 )
      {
        memcpy( (unsigned char*)&TempBufferB[2], (unsigned char*)&TempBuffer2[0], 4 );//��һ������������ʱ��
      }
    else
      {
        MEM_Read( (unsigned char*)&TempBufferB[0], E2A_PowerOffTim3, 6, Mem_E2PROMA );//��ȡ���һ�ε���ʱ��
      }
    CurrentTime = BCD_BYTE(TempBufferB[3])*24 + BCD_BYTE(TempBufferB[2]);
    while( Flag.BalanDmdNum > 0 )
    {
        WatchDog_Clear;//�忴�Ź�
       if( PowerOff_Ack() == 1 )
            return 1;
         Flag.Power |= F_PwrOffTim3;
         Tempnum++;
         switch( g_BalanNum )//��Ч��������
         {
           case 2:
               CNT++;
               if( TempBufferA[0] != 0x99 )//������1��Ч
               {
                 if(( (BillTime1 < BillTime2)&&((TempBufferA[5] == 0x99)||(BillTime2 == BillTime3))) || ( (BillTime1 < BillTime3)&&((TempBufferA[3] == 0x99)||(BillTime2 == BillTime3))) )//1С��2��3  
                 {
                   if((CurrentTime < BillTime1)||(CurrentTime >= BillTime2) ||(CurrentTime >= BillTime3) )//��ʱ��>=����2 3ʱ��
                   {
                     DMDREC_CNT=1;//��һ����������1����
                     if(( CNT % 2 )==0)
                     {
                       for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                          {
                              memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                          }
                     }
                    else 
                      DmdRecombination();//�����������
                   }
                   else//��ʱ��<����2 3ʱ��
                   {
                     
                     DMDREC_CNT=2;
                     if(( CNT % 2 )==1)
                     {
                       for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                          {
                              memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                          }
                     }
                    else 
                      DmdRecombination();//�����������
                   }
  
                 }
                 else//1����2��3
                 {
                  if( ((CurrentTime >= BillTime2)||(CurrentTime >= BillTime3)) && (CurrentTime < BillTime1) )
                   {
                     DMDREC_CNT=1;
                     if(( CNT % 2 )==0)
                       {
                         for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                            {
                                memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                            }
                       }
                       else
                        DmdRecombination();//�����������
                   }
                   else
                   {
                     DMDREC_CNT=2;
                      if(( CNT % 2 )==1)
                       {
                         for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                            {
                                memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                            }
                       }
                       else
                        DmdRecombination();//�����������                   
                   }
                 }
               }
               else
               {
                 for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                    {
                        memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                    }
               }
             break;
           case 3:
              CNT++;
             if( (BillTime1 < BillTime2) &&  (BillTime1 < BillTime3) )//1<2 / 3
             {
                if((CurrentTime < BillTime1)||(CurrentTime >= BillTime3) )
                {
                   DMDREC_CNT=1;

                   if((CNT != 1 )&&(CNT != 4 )&&(CNT != 7 )&&(CNT != 10 ))
                     {
                       for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                          {
                              memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                          }
                     }
                   else 
                      DmdRecombination();//�����������
                }
                else if((CurrentTime >= BillTime2)&&(CurrentTime < BillTime3) )
                {
                   DMDREC_CNT=2;
                  if((CNT != 2 )&&(CNT != 5 )&&(CNT != 8 )&&(CNT != 11 ))
                     {
                       for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                          {
                              memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                          }
                     }
                   else 
                      DmdRecombination();//�����������
                }
                 else if((CurrentTime >= BillTime1)&&(CurrentTime < BillTime2) )
                {
                   DMDREC_CNT=3;
                   if((CNT != 3 )&&(CNT != 6 )&&(CNT != 9 )&&(CNT != 12 ))
                     {
                       for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                          {
                              memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                          }
                     }
                   else 
                      DmdRecombination();//�����������
                }
             }
             else if( (BillTime1 > BillTime2) &&  (BillTime1 < BillTime3) )
             {
                if((CurrentTime >= BillTime2)&&(CurrentTime < BillTime1) )
                {
                   DMDREC_CNT=1;
                   if((CNT != 1 )&&(CNT != 4 )&&(CNT != 7 )&&(CNT != 10 ))
                     {
                       for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                          {
                              memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                          }
                     }
                   else 
                      DmdRecombination();//�����������
                }
                 else if((CurrentTime < BillTime2)||(CurrentTime >= BillTime3) )
                {
                   DMDREC_CNT=2;
                  if((CNT != 2 )&&(CNT != 5 )&&(CNT != 8 )&&(CNT != 11 ))
                     {
                       for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                          {
                              memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                          }
                     }
                   else 
                      DmdRecombination();//�����������
                }
                else if((CurrentTime >= BillTime1)&&(CurrentTime < BillTime3) )
                {
                   DMDREC_CNT=3;
                   if((CNT != 3 )&&(CNT != 6 )&&(CNT != 9 )&&(CNT != 12 ))
                     {
                       for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                          {
                              memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                          }
                     }
                   else 
                      DmdRecombination();//�����������
                }
             }
             else if( (BillTime1 > BillTime2) &&  (BillTime1 > BillTime3) )//1���� 2 / 3
             {
                if((CurrentTime >= BillTime3)&&(CurrentTime < BillTime1) )
                {
                   DMDREC_CNT=1;
                   if((CNT != 1 )&&(CNT != 4 )&&(CNT != 7 )&&(CNT != 10 ))
                     {
                       for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                          {
                              memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                          }
                     }
                   else 
                      DmdRecombination();//�����������
                }
                else if((CurrentTime >= BillTime2)&&(CurrentTime < BillTime3) )
                {
                   DMDREC_CNT=2;
                  if((CNT != 2 )&&(CNT != 5 )&&(CNT != 8 )&&(CNT != 11 ))
                     {
                       for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                          {
                              memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                          }
                     }
                   else 
                      DmdRecombination();//�����������
                }
                else if((CurrentTime >= BillTime1)||(CurrentTime < BillTime2) )
                {
                   DMDREC_CNT=3;
                   if((CNT != 3 )&&(CNT != 6 )&&(CNT != 9 )&&(CNT != 12 ))
                     {
                       for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                          {
                              memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                          }
                     }
                   else 
                      DmdRecombination();//�����������
                }
               
             }
             break;
           default:
             DMDREC_CNT=1;
             if( TempBufferA[0] == 0x99 )
              {
               for( i = 0; i < 8; i++ )//���򡢷�������޹�1��2��4�����޹��ܼ��ƽ���������
                  {
                      memset( (unsigned char*)&g_Save_Buffer[40*i],  0xFF, 40 );
                  }
              }
             else
             {
               DmdRecombination();//�����������
             }
             break;
         }
          //�޸�Ϊһ���Խ������1��
        DmdBalanceProc_ADD();//��������

        Flag.BalanDmdNum--;
        MEM_Write( E2A_DmdNum, (unsigned char*)&Flag.BalanDmdNum, 1, Mem_E2PROMA );
        if(( Flag.BalanDmdClr == 0x5A )&&(DMDREC_CNT==Tempnum))//��һ�ι�������1ʱ����ǰ����Ҫ����
        {
           if( Flag.BalanceEng1_MDNum == 1 )
             {
                memcpy( (unsigned char*)&MD_Freeze_Buffer[0],  (unsigned char*)&MaxDmd_Blk[0].MaxDmdBuff[0], 40 );//���ڲ��ն����������
                memcpy( (unsigned char*)&MD_Freeze_Buffer[40],  (unsigned char*)&MaxDmd_Blk[1].MaxDmdBuff[0], 40 );
             }
            Demand_ClearRAM();//RAM�������������
            Demand_ClearE2();//E2�������������
            Request.Flag_Dmd1Save = 0x00;//����ת���־
            Flag.BalanDmdClr = 0;
            MEM_Write( E2A_DmdClr, (unsigned char*)&Flag.BalanDmdClr, 1, Mem_E2PROMA );
        }
//        else
//            Request.Flag_Dmd2Save = 0x00;//����ת���־
    }
    if( ( Flag.BalanDmdNum == 0) && ( Flag.BalanEngNum == 0 ) )
      Flag.TimeChgBill = 0x00;
    
    return 0;
}



/************************************************************************
* ������ ��//MonthBalanceProc
* ���� ��//�½��㹦��
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
**************************************************************************/
void MonthBalanceProc(void)
{  
    unsigned char i;
    unsigned char LadderNO;
    
    if( Hour_Balance != ClockBlock.Clock.Hour )
    {
        Hour_Balance = ClockBlock.Clock.Hour;
        if( ClockBlock.Clock.Min != 0 )
            return;
        
        if(Flag.CpuCard & F_CardBDMODE) 
        {
            MeterPrice.Ladder_Mode = BalanceMode_Judge();//�����ģʽ
            LadderNO = Fee_JtChange_Judge();
	    MEM_Read( &g_Buffer[0], Set1Step_Table0_kWh+70*4*Fee.StepTable+70*LadderNO+52, 18, Mem_E2PROMA );  
            for( i=0; i<18; i+=3 )
	    {
	        if( Cmp_Bcd_Inv( g_Buffer+i, &ClockBlock.Clock.Hour, 3 ) == 0x03 ) break;
	    }
            if( (i<18) && (MeterPrice.Ladder_Mode == 0x55)&& (ClockBlock.Clock.Day <=0x28)) YearBalanceProc(); 
        
        }
        else MeterPrice.Ladder_Mode = 0x00;//�½���ģʽ  
        
        //�½���
        MEM_Read( g_Buffer, Balance_Time1, 6, Mem_E2PROMA );//����������1��2��3
        if( (ClockBlock.Clock.Hour == g_Buffer[0]) && (ClockBlock.Clock.Day == g_Buffer[1]) )//������1
        {           
            Flag.BalanRequest |= ( F_BalanEng + F_BalanDmd1 + F_BalanEngStar + F_BalanDmd1Star );
            Request.Flag_EngSave = 0x5A;//����ת���־
            Request.Flag_Dmd1Save = 0x5A;//����ת���־
        }
        else if( (ClockBlock.Clock.Hour == g_Buffer[2]) && (ClockBlock.Clock.Day == g_Buffer[3]) )//������2
        {
            Flag.BalanRequest |= ( F_BalanEng + F_BalanEngStar );
//            Flag.BalanRequest |= ( F_BalanEng + F_BalanDmd2 + F_BalanEngStar + F_BalanDmd2Star );
            Request.Flag_EngSave = 0x5A;//����ת���־
//            Request.Flag_Dmd2Save = 0x5A;//����ת���־
            
        }
        else if( (ClockBlock.Clock.Hour == g_Buffer[4]) && (ClockBlock.Clock.Day == g_Buffer[5]) )//������3
        {
            Flag.BalanRequest |= ( F_BalanEng + F_BalanEngStar );
//            Flag.BalanRequest |= ( F_BalanEng + F_BalanDmd2 + F_BalanEngStar + F_BalanDmd2Star );//��Ϊ������3�ͽ�����2����ת��һ�£�������ͬһ����־
            Request.Flag_EngSave = 0x5A;//����ת���־
//            Request.Flag_Dmd2Save = 0x5A;//����ת���־
        }
        
       
          
    }
    if(Flag.TimeChgBill == 0x5A)//�޸���ʱ��������̹����㣬���Ȳ�����
          return;
    Flag.BalanBDong &= ~F_ReBalanBD;
    if( (Flag.BalanRequest != 0)&& (g_SysCtlTime == 0) )//ϵͳ����ʱ��ִ�е�������
    {
        g_SysCtlTime = 1;//�н����¼��Ͳ�ִ��ת���ѹ�ϸ���

        if( Flag.BalanRequest & F_BalanEng )//����ת����ɺ�����ñ�־
        {
            if( Flag.BalanRequest & F_BalanEngStar )
            {
                Flag.BalanRequest &= ~F_BalanEngStar;
                EngRecombination();//�����������
                Timer.EngSaveCnt = 0;//����ת�������
            }
            EngryBalanceProc();//����ת��
            if( Flag.BalanRequest & F_BalanEngEnd )//����ת�����ݷ�5��д�꣬�ܹ�д316���ֽ�
            {
                Flag.BalanRequest &= ~F_BalanEngEnd;
                Flag.BalanRequest &= ~F_BalanEng;//����ת�����

                if( Flag.BalanRequest & F_BalanDmd1 )//������1����ת���꣬��ת�����ۼ��õ���
                {
                    MEM_Write( E2A_LastM_Act_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 4, Mem_E2PROMA );//��ǰ����й��ܵ���д����1������й��ܵ��� 
                    memcpy( (unsigned char*)&MonthEnery.LastPz0[0], (unsigned char*)&MonthEnery.CurntPz0[0], 4 );//��ǰ�ۼ�����й��ܵ���д����1���ۼ�����й��ܵ���
                    MEM_Write( E2A_LastM2_Act_Pz0, (unsigned char*)&MonthEnery.LastPz0[0], 4, Mem_E2PROMA );//ת���ۼ��õ���
                    memset( (unsigned char*)&MonthEnery.CurntPz0[0], 0x00, 4 ); //�½���,���õ���ת������� 
                    if( MeterPrice.Ladder_Mode == 0x00 ) //�½�������ݵ���жϵ���
                    {     
                        memset( (unsigned char*)&AEngy_Blk.AEngy.ActUsedPz0[0], 0x00, 4 );//�����ж�ʹ�õ����Ĵ���
                        MEM_CRCWrite( FM1_EC_ActUsed_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActUsedPz0[0], 4, Mem_FRAM );
                       
                    }
                }
                
                MEM_Write( E2A_LastZ_Act_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 4, Mem_E2PROMA );//��ǰ����й��ܵ���д����1��������й��ܵ���    
                memcpy( (unsigned char*)&CycEnery.LastPz0[0], (unsigned char*)&CycEnery.CurntPz0[0], 4 );//��ǰ�ۼ�����й��ܵ���д����1�����ۼ�����й��ܵ���
                MEM_Write( E2A_LastZ2_Act_Pz0, (unsigned char*)&CycEnery.LastPz0[0], 4, Mem_E2PROMA );//ת���ۼ��õ���
                memset( (unsigned char*)&CycEnery.CurntPz0[0], 0x00, 4 );
            }
        }
        else if( Flag.BalanRequest & F_BalanDmd1 )//������1����ת���꣬����ñ�־
        {
            if( Flag.BalanRequest & F_BalanDmd1Star )
            {
                Flag.BalanRequest &= ~F_BalanDmd1Star;
                DmdRecombination();//�����������
                Timer.DmdSaveCnt = 0;//����ת�������
                Demand_ClearRAM();//RAM�������������
//                MaxDemInt();//���������ʼ��
 //               GrpDemInt();
 //               CurrDemInt();
            }

            DmdBalanceProc();//��������

            if( Flag.BalanRequest & F_BalanDmdEnd )//����ת�����ݷ�5��д�꣬�ܹ�д320���ֽ�
            {
                Flag.BalanRequest &= ~F_BalanDmdEnd;
                Flag.BalanRequest &= ~F_BalanDmd1;//����ת�����
                Request.Flag_Dmd1Save = 0x00;//����ת���־
            }

            if( ( Flag.BalanRequest & F_BalanDmd1 ) == 0 )//������1����ת������������������
                Demand_ClearE2();//E2�������������
        }
//        else if( Flag.BalanRequest & F_BalanDmd2 )//������2��3������ת����0XFF��
//        {
//            if( Flag.BalanRequest & F_BalanDmd2Star )
//            {
//                Flag.BalanRequest &= ~F_BalanDmd2Star;
//                for( i = 0; i < 320; i++ )
//                {
//                    g_Save_Buffer[i] = 0xFF;
//                }
//                Timer.DmdSaveCnt = 0;//����ת�������
//            }
//
//            DmdBalanceProc();//��������
//
//            if( Flag.BalanRequest & F_BalanDmdEnd )//����ת�����ݷ�5��д�꣬�ܹ�д320���ֽ�
//            {
//                Flag.BalanRequest &= ~F_BalanDmdEnd;
//                Flag.BalanRequest &= ~F_BalanDmd2;//����ת�����
//                Request.Flag_Dmd2Save = 0x00;//����ת���־
//            }
//        }
    }
}









/*


//offset=0��������,offset=3���������
//offset=1�½���,offset=4���½���
//offset=2�����,offset=5�������,offset=6�����ʱ������
void Process_Usedl( unsigned char offset )
{
	unsigned char i, len;
	unsigned char  Buf[ee_yhistroyusedl_lenth+8];	
		
	Y_M_C_CycleEngry( offset,(unsigned char*)&Buf[4]);//���㵱ǰ����ۼ��õ���
        memcpy( (unsigned char*)&MonthEnery.LastPz0[0], (unsigned char*)&MonthEnery.CurntPz0[0], 4 );//��ǰ�ۼ�����й��ܵ���д����1���ۼ�����й��ܵ���
        MEM_Write( E2A_LastM2_Act_Pz0, (unsigned char*)&MonthEnery.LastPz0[0], 4, Mem_E2PROMA );//ת���ۼ��õ���
        memset( (unsigned char*)&MonthEnery.CurntPz0[0], 0x00, 4 );
         if( (ClockBlock.Clock.Hour == g_Buffer[0]) && (ClockBlock.Clock.Day == g_Buffer[1]) )//������1
        {  
            if( Price.Ladder_Mode == 0x55 )
            { 
                 Y_M_C_CycleEngry( offset,(unsigned char*)&MonthEnery.CurntPz0);//���㵱ǰ�¶��ۼ��õ���
                 memcpy( (unsigned char*)&MonthEnery.LastPz0[0], (unsigned char*)&MonthEnery.CurntPz0[0], 4 );//��ǰ�ۼ�����й��ܵ���д����1���ۼ�����й��ܵ���
                 MEM_Write( E2A_LastM2_Act_Pz0, (unsigned char*)&MonthEnery.LastPz0[0], 4, Mem_E2PROMA );//ת���ۼ��õ���
               //�����,��ת��
            }
            else  
            {
                 Y_M_C_CycleEngry( offset,(unsigned char*)&MonthEnery.CurntPz0);//���㵱ǰ����ۼ��õ���
                 memcpy( (unsigned char*)&MonthEnery.LastPz0[0], (unsigned char*)&MonthEnery.CurntPz0[0], 4 );//��ǰ�ۼ�����й��ܵ���д����1���ۼ�����й��ܵ���
                 MEM_Write( E2A_LastM2_Act_Pz0, (unsigned char*)&MonthEnery.LastPz0[0], 4, Mem_E2PROMA );//ת���ۼ��õ���
                 memset( (unsigned char*)&MonthEnery.CurntPz0[0], 0x00, 4 );
	         //�½���,���õ���ת������� 
            }
        } 
	
}

*/
/************************************************************************
* ������ ��//YearBalanceProc
* ���� ��//����㹦��
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//��
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2016/12/12 V1.0 yangxing XXXX
**************************************************************************/
void YearBalanceProc(void)
{
    unsigned char Ptr;
    unsigned short Addr;
    unsigned long Num;
    unsigned char  TempBuf[8];

    MEM_Read( (unsigned char*)&Ptr, FM1_CurrentY_Ptr, 1, Mem_FRAM );//�������������ָ��
    Y_M_C_CycleEngry( 2,(unsigned char*)&YearEnery.CurntPz0);//���㵱ǰ����ۼ��õ���
    memcpy( (unsigned char*)&TempBuf[0], (unsigned char*)&ClockBlock.Clock.Hour,4 );
    memcpy( (unsigned char*)&TempBuf[4], (unsigned char*)&YearEnery.CurntPz0,4 );
    memcpy( (unsigned char*)&YearEnery.LastPz0, (unsigned char*)&YearEnery.CurntPz0,4 );
    
    Addr = E2A_CurrentY_DATA + 8*Ptr;//�ô���������ʼ��ַ
    MEM_Write( Addr, (unsigned char*)&TempBuf[0], 8, Mem_E2PROMA);//ת��
    Ptr++;//ָ������
    if( Ptr >= CurrentY_MNum )	Ptr = 0;
    MEM_DBWrite( FM1_CurrentY_Ptr, (unsigned char*)&Ptr, 1, Mem_FRAM );
    
    MEM_Read( (unsigned char*)&Num, CurrentY_Num, 4, Mem_E2PROMA );
    if( Num < 0x00999999 )//��������ۼ�
    {
        BCDX_INC( (unsigned char*)&Num, 4 );
        MEM_Write( CurrentY_Num, (unsigned char*)&Num, 4, Mem_E2PROMA );
    }
    MEM_Write( E2A_LastY_Act_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActPz0[0], 4, Mem_E2PROMA );//��ǰ����й��ܵ���ת����1�������й��ܵ���  
    memset( (unsigned char*)&YearEnery.CurntPz0[0], 0x00, 4 );//��ǰ����ۼ��õ�������   
    memset( (unsigned char*)&AEngy_Blk.AEngy.ActUsedPz0[0], 0x00, 4 );//�����ж�ʹ�õ����Ĵ���
    MEM_CRCWrite( FM1_EC_ActUsed_Pz0, (unsigned char*)&AEngy_Blk.AEngy.ActUsedPz0[0], 4, Mem_FRAM );
}




/************************************************************************
* ������ ��//BalanceMode_Judge
* ���� ��//����ģʽ�ж�
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//0x00 �¼��㣻0x55�����
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
*************************************************************************/
unsigned char BalanceMode_Judge(void )
{
    unsigned char LadderNO;
 
    LadderNO = Fee_JtChange_Judge();
    
    MEM_Read( &g_Buffer[0], Set1Step_Table0_kWh+70*4*Fee.StepTable+70*LadderNO+52, 18, Mem_E2PROMA );    
    if( JudgeYearClock( g_Buffer ) == 0 ) return 0x55;//�����ģʽ
    else return 0x00;   
 
    
}
/************************************************************************
* ������ ��//ClearYearBalance
* ���� ��//���������
* ������� ��//��
* ������� ��//��
* ����ֵ˵�� ��//
* ����˵�� ��//��
* �޸����� :
* ------------------------------------------------------------------------
* 2010/07/06 V1.0 yangxing XXXX
*************************************************************************/
void ClearYearBalance(void)
{
    unsigned long Num;
    unsigned char  TempBuf[48];    
    Num=0;
    MEM_Write( CurrentY_Num, (unsigned char*)&Num, 4, Mem_E2PROMA );
    MEM_DBWrite( FM1_CurrentY_Ptr, (unsigned char*)&Num, 1, Mem_FRAM );
    memset( (unsigned char*)&TempBuf[0], 0x00, 48 );
    MEM_Write( E2A_CurrentY_DATA, (unsigned char*)&TempBuf[0], 48, Mem_E2PROMA);//ת��
    MEM_Write( E2A_LastY_Act_Pz0, (unsigned char*)&TempBuf[0], 4, Mem_E2PROMA );
}
