//-------------- CPU ---------------------------
#define		UserCardCommand		0x03	//;�û���������
#define		UserCardLength		39	//;�û�������
#define		InitCardType		0x01	//;��ʼ����������
#define		UserCardType		0x02	//;��ͨ�û���������
#define		RecuitCardType		0x03	//;����������

#define		UserOpenCardType		0x01	//;������������
#define		UserchargeCardType	0x02	//;��ͨ�û���������
#define		UserRecuitCardType	0x03	//;����������
#define		UserTestCardType		0x04	//;���Կ�������
#define		UserKeyCardType		  0x05	//;��Կ��������



#define		PresetCardCommand	0x04	//;Ԥ�ƿ�������
#define		PresetCardLength	26	//;Ԥ�ƿ�����

#define		OK_CARD_PROC		0	//��Ƭ�����ɹ�
#define		IC_VLOW_ERR			1	//��Ƶ�ѹ����176V(80%Un)
#define		IC_ESAM_RDWR_ERR	2	//����ESAM����
#define		IC_ESAM_RST_ERR		3	//ESAM��λ����
#define		IC_IC_RST_ERR		4	//��Ƭ��λ����:���忨�����Ƭ
#define		IC_AUTH_ERR			5	//�����֤����(ͨ�ųɹ������Ĳ�ƥ��)
#define		IC_EXAUTH_ERR		6	//�ⲿ�����֤����(ͨ�ųɹ�����֤��ͨ��)
#define		IC_NOFX_ERR			7	//δ���еĿ�Ƭ(����Ƭʱ����6B00)
#define		IC_TYPE_ERR			8	//�����ʹ���
#define		IC_KER_ERR			9	//��Ƭ����δ��Ȩ����Կ״̬��Ϊ��Կ�ǲ����Ԥ�ƿ�
#define		IC_MAC_ERR			10	//�ظ����� 
#define		IC_ID_ERR			11	//��Ų�һ��
#define		IC_HUHAO_ERR		12	//�ͻ���Ų�һ��
#define		IC_SERIAL_ERR		13	//����Ų�һ��
#define		IC_FORMAT_ERR		14	//��Ƭ�ļ���ʽ���Ϸ�
#define		IC_NOOPEN_ERR		15	//���翨����δ�������
#define		IC_NOOPEN2_ERR		16	//��������δ�������
#define		IC_TIMES_ERR		17	//�����������
#define		IC_NONULL_ERR		18	//�û�����д��Ϣ�ļ���Ϊ��
#define		IC_NOIN_ERR			19	//������ƬͨѶ����(��д��Ƭʧ��)
#define		IC_PREIC_ERR		20	//��ǰ�ο�
#define		IC_OVERMAX_ERR		21	//ʣ����ڻ�
#define		IC_SELECTION_ERR			22	//��Ų�һ��


//-------------- CPU end -----------------------
