//****************************************************************//
//	   DHT21ʹ�÷��� 
//��Ƭ�� AT89S52 �� STC89C52RC 
//����   ���ڷ�����ʪ������ ���� 11.0592M ������ 9600 
//Ӳ��   P2.0��ΪͨѶ������DHT11,DHT11�ĵ�Դ�͵����ӵ�Ƭ���ĵ�Դ�͵أ���Ƭ�����ڼ�MAX232���ӵ��� 
//��˾  �������    
//****************************************************************//

#include <reg52.h>
#include <intrins.h> 
//
typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  �޷���8λ���ͱ���  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  �з���8λ���ͱ���  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  �޷���16λ���ͱ��� */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  �з���16λ���ͱ��� */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  �޷���32λ���ͱ��� */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  �з���32λ���ͱ��� */
typedef float          F32;      /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         F64;      /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */
//
#define uchar unsigned char
#define uint unsigned int
#define   Data_0_time    4

//----------------------------------------------//
//----------------IO�ڶ�����--------------------//
//----------------------------------------------//
sbit  P2_5  = P2^5 ;

//----------------------------------------------//
//----------------������--------------------//
//----------------------------------------------//
/*U8  U8FLAG,k;
U8  U8count,U8temp;
U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  U8comdata;
U8  outdata[5];  //���巢�͵��ֽ���	   
U8  indata[5];
U8  count, count_r=0;
U8  str[5]={"RS232"};
U16 U16temp1,U16temp2;*/
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
/*
void Uart0_Init()
{
    RCAP2L=0xD9;//9600�����ʶ�Ӧ FFD9����λΪD9
    RCAP2H=0xFF;//��λΪFF
    T2CON=0x34;//RCLK��TCLK��TR2��1
    SCON=0x50;//���ڹ���ģʽ1������ʹ��
    ES=1;//�򿪽����ж�
    EA=1;//�����ж�
}
*/	
//----------------------------------------------
//main()��������:  AT89C51  11.0592MHz 	���ڷ� 
//����ʪ������,������ 9600 
//----------------------------------------------
/*void main()
{
	U8  i,j;
	
	//uchar str[6]={"RS232"};

	Uart0_Init();
	SendData(str) ;   //���͵����� 
	Delay(1);         //��ʱ100US��12M����)
	while(1)
	{  

	   //------------------------
	   //������ʪ�ȶ�ȡ�ӳ��� 
	   RH();
	   //������ʾ���� 
	   //--------------------------

	   str[0]=U8RH_data_H;
	   str[1]=U8RH_data_L;
	   str[2]=U8T_data_H;
	   str[3]=U8T_data_L;
	   str[4]=U8checkdata;

	   SendData(str) ;  //���͵�����  

	   //��ȡģ���������ڲ���С�� 2S 
	   Delay(20000);
	}//elihw
	
}// main
*/



