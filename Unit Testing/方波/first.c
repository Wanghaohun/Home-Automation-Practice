/*��ʱ����������*/
#include <reg52.h>
#define uchar unsigned char
#define uint  unsigned int
sbit Sign = P1^0;
sbit SignFromMain = P2^3;
uint count;
uint culer;
uint tmp;
void init()
{
	
	TMOD = 0x21;  //��ʱ��16Ϊ��������ģʽ
	TH0 = 0xff;
	TL0 = 0xa3; //0.1ms
	TH1 = 0xfd;
	TL1 = 0xfd; 	//������9600
	TR1 = 1;		//����T1��ʱ��
	SM0 = 0;
	SM1 = 1; 		//���ڹ�����ʽ1 10λ�첽
	REN = 1;		//�����������
	ES  = 1;		//�����жϴ�
	ET0 = 1; //����ʱ��0�ж�
	TR0 = 1;//������ʱ��0

}

	void main()
	{
	 SignFromMain==0;//����ź�λ��ʼ��Ϊ0
	 count = 0; //��������
	 culer =11;//��������
	 init(); //��ʱ����ʼ��
	 while(1)
	{	
	if(SignFromMain==1)
	{
	EA = 1;	//�����ж�
	}
	else
	{
	EA = 0;//�����ж�
	}
	}
	}
void timer0() interrupt 1  //��ʱ���ж�
{
	TH0 =0xff;  //�趨��ֵ
	TL0 =0xa3; //0.1ms
	count++;
	if (count == (180-culer) && Sign == 0)
	{
		Sign = 1;
		count = 0;	
	}
	if (count == culer && Sign == 1)
	{
		Sign = 0;
		count = 0;	
	}
}

