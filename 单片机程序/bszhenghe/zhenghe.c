#include <reg52.h>
#include <intrins.h>
#include <I2C.h>
#include <SPI.h>
#include <DA-AD.h>

typedef unsigned char  U8;      /* defined for unsigned 8-bits integer variable 	  �޷���8λ���ͱ���  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  �з���8λ���ͱ���  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  �޷���16λ���ͱ��� */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  �з���16λ���ͱ��� */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  �޷���32λ���ͱ��� */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  �з���32λ���ͱ��� */
typedef float          F32;      /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         F64;      /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */

#define   Data_0_time    4
#define RX_DR  (sta & 0x40)  // ���ճɹ��жϱ�־
#define TX_DS  (sta & 0x20)  // ����ɹ��жϱ�־
#define MAX_RT (sta & 0x10)  // �ط�����жϱ�־
sbit duo = P2^3;
sbit CE =  P1^2;
sbit CSN=  P1^3;
sbit SCK=  P1^7;
sbit MOSI= P1^5;
sbit MISO= P1^6;
sbit IRQ = P1^4;
sbit P2_5  = P2^5;
sbit     SCL=P2^0;       //I2C  ʱ�� 
sbit     SDA=P2^1;       //I2C  ���� 
sbit set = P0^3;
sbit reset = P0^4;
U8  acount;
U8  idata rec[20];
U8  idata U8FLAG,k;
U8  idata U8count,U8temp;
U8  idata U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  idata U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  idata U8comdata;
U8  idata outdata[5];  //���巢�͵��ֽ���	   
U8  idata indata[5];
U8  idata count, count_r=0;
U8  idata str[8]={16,10,15,23,62};
U16 idata U16temp1,U16temp2;
U16 idata DataLight;
U8 idata rx_buf[10];

SendData(U8 *a)
{
	outdata[0] = a[0]; 
	outdata[1] = a[1];
	outdata[2] = a[2];
	outdata[3] = a[3];
	outdata[4] = a[4];
	count = 1;
	SBUF=outdata[0];
}
/*nRF24L01��ʼ��*/

 void UART_init()
{

    RCAP2L=0xD9;//9600�����ʶ�Ӧ FFD9����λΪD9
    RCAP2H=0xFF;//��λΪFF
    T2CON=0x34;//RCLK��TCLK��TR2��1
    SCON=0x50;//���ڹ���ģʽ1������ʹ��
	EA  = 1;		//�����ж�
	ES  = 1;		//�����жϴ�

}
       void Delay180ms()
    {      U8 i;
			U16 j=190;
	    for(;j>0;j--)
	  { 	
		for(i=0;i<27;i++);

	  }
    }
       void  Delay_10us(void)
      {
        U8 i;
        i--;
        i--;
        i--;
        i--;
        i--;
        i--;
       }
	
        void  COM(void)
      {
     
	        U8 i;
          
       for(i=0;i<8;i++)	   
	    {
		
	   	    U8FLAG=2;	
	   	while((!P2_5)&&U8FLAG++);
			Delay_10us();
		    Delay_10us();
			Delay_10us();
	  		U8temp=0;
	     if(P2_5)U8temp=1;
		    U8FLAG=2;
		 while((P2_5)&&U8FLAG++);
	   	//��ʱ������forѭ��		  
	   	 if(U8FLAG==1)break;
	   	//�ж�����λ��0����1	 
	   	   
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
	   	 
		   U8comdata<<=1;
	   	   U8comdata|=U8temp;        //0
	     }//rof
	   
	}

	//--------------------------------
	//-----ʪ�ȶ�ȡ�ӳ��� ------------
	//--------------------------------
	//----���±�����Ϊȫ�ֱ���--------
	//----�¶ȸ�8λ== U8T_data_H------
	//----�¶ȵ�8λ== U8T_data_L------
	//----ʪ�ȸ�8λ== U8RH_data_H-----
	//----ʪ�ȵ�8λ== U8RH_data_L-----
	//----У�� 8λ == U8checkdata-----
	//----��������ӳ�������----------
	//---- Delay();, Delay_10us();,COM(); 
	//--------------------------------

	void RH(void)
	{
	  //��������18ms 
       P2_5=0;
	   Delay180ms();
	   P2_5=1;
	 //������������������ ������ʱ20us
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	 //������Ϊ���� �жϴӻ���Ӧ�ź� 
	   P2_5=1;
	 //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	   if(!P2_5)		 //T !	  
	   {
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
	   while((!P2_5)&&U8FLAG++);
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	   while((P2_5)&&U8FLAG++);
	 //���ݽ���״̬		 
	   COM();
	   U8RH_data_H_temp=U8comdata;
	   COM();
	   U8RH_data_L_temp=U8comdata;
	   COM();
	   U8T_data_H_temp=U8comdata;
	   COM();
	   U8T_data_L_temp=U8comdata;
	   COM();
	   U8checkdata_temp=U8comdata;
	   P2_5=1;
	 //����У�� 
	 
	   U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	   if(U8temp==U8checkdata_temp)
	   {
	   	  U8RH_data_H=U8RH_data_H_temp;
	   	  U8RH_data_L=U8RH_data_L_temp;
		  U8T_data_H=U8T_data_H_temp;
	   	  U8T_data_L=U8T_data_L_temp;
	   	  U8checkdata=U8checkdata_temp;
	   }
	   }

	}
void DA_TH()
{
	   RH();
	   str[0]=U8RH_data_H;
	   str[1]=U8RH_data_L;
	   str[2]=U8T_data_H;
	   str[3]=U8T_data_L;
	   str[4]=U8checkdata;
}
void main()
{	    
	U8 i;
	U8 buf0[5]={0,0,0,0,0};
    P0=0xff;//��ʼ��IO��
    P1=0xff;
    P2=0xff;
    P3=0xff;
	set=1;
	reset=1;
    _delay_us(100);
    nRF24L01_Init();//NRF24L01��ʼ��
	UART_init();
	duo=0;
    while(1)
    {			
    		DataLight=DA_L();
			str[5]=DataLight;	
			str[7]=99;
			DA_TH();
            for(i=0;i<7;i++)//����8������
            {
            nRF24L01_Set_TX_Mode(&str[i]);//��������
            while(Check_Ack());//�ȴ��������
            }
    for(i=0;i<50;i++)	
{	
    	setRX_Mode();
    	if(Check_Rec())
		{
	if (RX_Buffer[0] == RX_Buffer[4])
    {
      //���Գɹ�
		duo=1;
      
    }
	if (RX_Buffer[3] == RX_Buffer[4])
    {
      //���Գɹ�
		duo=0;
      
    }
		}
	}
    }
}
void jieshou(void) interrupt 4
{
       if(RI==1)           //����Ƿ������ݽ���
    {   
   rec[acount] = SBUF;   //��������
   SBUF = rec[acount];	
   while(!TI);
   TI = 0;
   acount++; 
   RI = 0;   //�����־λ
   if(acount==8) 	acount=0;	
   if(rec[0]==0x42&&rec[1]==0x4d)
	{
	str[6]=rec[7];
	}

    }
}
