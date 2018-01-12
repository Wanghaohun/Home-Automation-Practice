#include <reg52.h>
#include <intrins.h>
#include <I2C.h>
#include <SPI.h>
#include <DA-AD.h>

typedef unsigned char  U8;      /* defined for unsigned 8-bits integer variable 	  无符号8位整型变量  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  有符号8位整型变量  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  无符号16位整型变量 */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  有符号16位整型变量 */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  无符号32位整型变量 */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  有符号32位整型变量 */
typedef float          F32;      /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         F64;      /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */

#define   Data_0_time    4
#define RX_DR  (sta & 0x40)  // 接收成功中断标志
#define TX_DS  (sta & 0x20)  // 发射成功中断标志
#define MAX_RT (sta & 0x10)  // 重发溢出中断标志
sbit duo = P2^3;
sbit CE =  P1^2;
sbit CSN=  P1^3;
sbit SCK=  P1^7;
sbit MOSI= P1^5;
sbit MISO= P1^6;
sbit IRQ = P1^4;
sbit P2_5  = P2^5;
sbit     SCL=P2^0;       //I2C  时钟 
sbit     SDA=P2^1;       //I2C  数据 
sbit set = P0^3;
sbit reset = P0^4;
U8  acount;
U8  idata rec[20];
U8  idata U8FLAG,k;
U8  idata U8count,U8temp;
U8  idata U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  idata U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  idata U8comdata;
U8  idata outdata[5];  //定义发送的字节数	   
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
/*nRF24L01初始化*/

 void UART_init()
{

    RCAP2L=0xD9;//9600波特率对应 FFD9，低位为D9
    RCAP2H=0xFF;//高位为FF
    T2CON=0x34;//RCLK、TCLK、TR2置1
    SCON=0x50;//串口工作模式1，接收使能
	EA  = 1;		//开总中断
	ES  = 1;		//串口中断打开

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
	   	//超时则跳出for循环		  
	   	 if(U8FLAG==1)break;
	   	//判断数据位是0还是1	 
	   	   
		// 如果高电平高过预定0高电平值则数据位为 1 
	   	 
		   U8comdata<<=1;
	   	   U8comdata|=U8temp;        //0
	     }//rof
	   
	}

	//--------------------------------
	//-----湿度读取子程序 ------------
	//--------------------------------
	//----以下变量均为全局变量--------
	//----温度高8位== U8T_data_H------
	//----温度低8位== U8T_data_L------
	//----湿度高8位== U8RH_data_H-----
	//----湿度低8位== U8RH_data_L-----
	//----校验 8位 == U8checkdata-----
	//----调用相关子程序如下----------
	//---- Delay();, Delay_10us();,COM(); 
	//--------------------------------

	void RH(void)
	{
	  //主机拉低18ms 
       P2_5=0;
	   Delay180ms();
	   P2_5=1;
	 //总线由上拉电阻拉高 主机延时20us
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	 //主机设为输入 判断从机响应信号 
	   P2_5=1;
	 //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	   if(!P2_5)		 //T !	  
	   {
	   U8FLAG=2;
	 //判断从机是否发出 80us 的低电平响应信号是否结束	 
	   while((!P2_5)&&U8FLAG++);
	   U8FLAG=2;
	 //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	   while((P2_5)&&U8FLAG++);
	 //数据接收状态		 
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
	 //数据校验 
	 
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
    P0=0xff;//初始化IO口
    P1=0xff;
    P2=0xff;
    P3=0xff;
	set=1;
	reset=1;
    _delay_us(100);
    nRF24L01_Init();//NRF24L01初始化
	UART_init();
	duo=0;
    while(1)
    {			
    		DataLight=DA_L();
			str[5]=DataLight;	
			str[7]=99;
			DA_TH();
            for(i=0;i<7;i++)//发送8次数据
            {
            nRF24L01_Set_TX_Mode(&str[i]);//发送数据
            while(Check_Ack());//等待发送完成
            }
    for(i=0;i<50;i++)	
{	
    	setRX_Mode();
    	if(Check_Rec())
		{
	if (RX_Buffer[0] == RX_Buffer[4])
    {
      //测试成功
		duo=1;
      
    }
	if (RX_Buffer[3] == RX_Buffer[4])
    {
      //测试成功
		duo=0;
      
    }
		}
	}
    }
}
void jieshou(void) interrupt 4
{
       if(RI==1)           //检测是否有数据接收
    {   
   rec[acount] = SBUF;   //接收数据
   SBUF = rec[acount];	
   while(!TI);
   TI = 0;
   acount++; 
   RI = 0;   //清除标志位
   if(acount==8) 	acount=0;	
   if(rec[0]==0x42&&rec[1]==0x4d)
	{
	str[6]=rec[7];
	}

    }
}
