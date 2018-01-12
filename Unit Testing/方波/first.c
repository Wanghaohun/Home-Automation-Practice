/*定时器产生方波*/
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
	
	TMOD = 0x21;  //定时器16为计数工作模式
	TH0 = 0xff;
	TL0 = 0xa3; //0.1ms
	TH1 = 0xfd;
	TL1 = 0xfd; 	//比特率9600
	TR1 = 1;		//启动T1定时器
	SM0 = 0;
	SM1 = 1; 		//串口工作方式1 10位异步
	REN = 1;		//串口允许接收
	ES  = 1;		//串口中断打开
	ET0 = 1; //开定时器0中断
	TR0 = 1;//启动定时器0

}

	void main()
	{
	 SignFromMain==0;//舵机信号位初始化为0
	 count = 0; //方波参数
	 culer =11;//方波参数
	 init(); //定时器初始化
	 while(1)
	{	
	if(SignFromMain==1)
	{
	EA = 1;	//开总中断
	}
	else
	{
	EA = 0;//关总中断
	}
	}
	}
void timer0() interrupt 1  //定时器中断
{
	TH0 =0xff;  //设定初值
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

