#include <reg52.h>
#include <intrins.h>
//
typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  无符号8位整型变量  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  有符号8位整型变量  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  无符号16位整型变量 */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  有符号16位整型变量 */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  无符号32位整型变量 */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  有符号32位整型变量 */
typedef float          F32;      /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         F64;      /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */
//
/******************************************************************
// nRF24L01寄存器地址
*******************************************************************/
#define CONFIG      0x00  // 配置寄存器
#define EN_AA       0x01  // “自动应答”功能寄存
#define EN_RX_ADDR  0x02  // 接收通道使能寄存器
#define SETUP_AW    0x03  // 地址宽度设置寄存器
#define SETUP_RETR  0x04  // 自动重发设置寄存器
#define RF_CH       0x05  // 射频通道频率设置寄存器
#define RF_SETUP    0x06  // 射频设置寄存器
#define STATUS      0x07  // 状态寄存器
#define OBSERVE_TX  0x08  // 发送检测寄存器
#define CD          0x09  // 载波检测寄存器
#define RX_ADDR_P0  0x0A  // 数据通道0接收地址寄存器
#define RX_ADDR_P1  0x0B  // 数据通道1接收地址寄存器
#define RX_ADDR_P2  0x0C  // 数据通道2接收地址寄存器
#define RX_ADDR_P3  0x0D  // 数据通道3接收地址寄存器
#define RX_ADDR_P4  0x0E  // 数据通道4接收地址寄存器
#define RX_ADDR_P5  0x0F  // 数据通道5接收地址寄存器
#define TX_ADDR     0x10  // 发送地址寄存器
#define RX_PW_P0    0x11  // 数据通道0有效数据宽度设置寄存器
#define RX_PW_P1    0x12  // 数据通道1有效数据宽度设置寄存器
#define RX_PW_P2    0x13  // 数据通道2有效数据宽度设置寄存器
#define RX_PW_P3    0x14  // 数据通道3有效数据宽度设置寄存器
#define RX_PW_P4    0x15  // 数据通道4有效数据宽度设置寄存器
#define RX_PW_P5    0x16  // 数据通道5有效数据宽度设置寄存器
#define FIFO_STATUS 0x17  // FIFO状态寄存器
//*********************************************************************************
#define TX_ADDR_WITDH 5//发送地址宽度设置为5个字节
#define RX_ADDR_WITDH 5//接收地址宽度设置为5个字节
#define TX_DATA_WITDH 8//
#define RX_DATA_WITDH 8
/******************************************************************
// nRF24L01指令格式：
*******************************************************************/
#define R_REGISTER    0x00  // 读寄存器
#define W_REGISTER    0x20  // 写寄存器
#define R_RX_PLOAD    0x61  // 读RX FIFO有效数据，1-32字节，当读数据完成后，数据被清除，应用于接收模式
#define W_TX_PLOAD    0xA0  // 写TX FIFO有效数据，1-32字节，写操作从字节0开始，应用于发射模式
#define FLUSH_TX    0xE1  // 清除TX FIFO寄存器，应用于发射模式
#define FLUSH_RX    0xE2  // 清除RX FIFO寄存器，应用于接收模式
#define REUSE_TX_PL 0xE3  // 重新使用上一包有效数据，当CE为高过程中，数据包被不断的重新发射
#define NOP         0xFF  // 空操作，可以用来读状态寄存器
U8 sta;    // 状态变量
#define RX_DR  (sta & 0x40)  // 接收成功中断标志
#define TX_DS  (sta & 0x20)  // 发射成功中断标志
#define MAX_RT (sta & 0x10)  // 重发溢出中断标志
U8 RX_Buffer[RX_DATA_WITDH];
U8 TX_Buffer[TX_DATA_WITDH];
U8 code TX_Addr[]={0x34,0x43,0x10,0x10,0x01};
//U8 TX_Addr[]="Rec01";
sbit CE =  P1^2;
sbit CSN=  P1^3;
sbit SCK=  P1^7;
sbit MOSI= P1^5;
sbit MISO= P1^6;
sbit IRQ = P1^4;

void _delay_us(U16 x)
{
    U16 i,j;
    for (j=0;j<x;j++)
        for (i=0;i<12;i++);
}
void _delay_ms(U16 x)
{
    U16 i,j;
    for (j=0;j<x;j++)
        for (i=0;i<120;i++);
}
void nRF24L01_Init(void)
{
    CE=0;//待机模式Ⅰ
    CSN=1;
    SCK=0;
    IRQ=1;
}
/*SPI时序函数*/
U8 SPI_RW(U8 byte)
{
    U8 i;
    for(i=0;i<8;i++)//一字节8位循环8次写入
    {
        if(byte&0x80)//如果数据最高位是1
            MOSI=1;//向NRF24L01写1
        else //否则写0
            MOSI=0;
        byte<<=1;//低一位移到最高位
        SCK=1;//SCK拉高，写入一位数据，同时读取一位数据
        if(MISO)
            byte|=0x01;
        SCK=0;//SCK拉低
    }
    return byte;//返回读取一字节 
}
/*SPI写寄存器一字节函数*/
/*reg:寄存器地址*/
/*value:一字节（值）*/
U8 SPI_W_Reg(U8 reg,U8 value)
{
    U8 status;//返回状态
    CSN=0;//SPI片选
    status=SPI_RW(reg);//写入寄存器地址，同时读取状态
    SPI_RW(value);//写入一字节
    CSN=1;//
    return status;//返回状态
}
/*SPI读一字节*/
U8 SPI_R_byte(U8 reg)
{
    U8 reg_value;
    CSN=0;//SPI片选
    SPI_RW(reg);//写入地址
    reg_value=SPI_RW(0);//读取寄存器的值
    CSN=1;
    return reg_value;//返回读取的值
}
/*SPI读取RXFIFO寄存器数据*/
/*reg:寄存器地址*/
/*Dat_Buffer:用来存读取的数据*/
/*DLen:数据长度*/
U8 SPI_R_DBuffer(U8 reg,U8 *Dat_Buffer,U8 Dlen)
{
    U8 status,i;
    CSN=0;//SPI片选
    status=SPI_RW(reg);//写入寄存器地址，同时状态
    for(i=0;i<Dlen;i++)
    {
        Dat_Buffer[i]=SPI_RW(0);//存储数据
    }
    CSN=1;
    return status;
}
/*SPI向TXFIFO寄存器写入数据*/
/*reg:写入寄存器地址*/
/*TX_Dat_Buffer:存放需要发送的数据*/
/*Dlen:数据长度*/  
U8 SPI_W_DBuffer(U8 reg,U8 *TX_Dat_Buffer,U8 Dlen)
{
    U8 status,i;
    CSN=0;//SPI片选，启动时序
    status=SPI_RW(reg);
    for(i=0;i<Dlen;i++)
    {
        SPI_RW(TX_Dat_Buffer[i]);//发送数据
    }
    CSN=1;
    return status;  
}
U8 SPI_Read_Buf(U8 reg,U8 *pBuf,U8 uchars)//读多个字节并读出此地址的状态
{
	U8 status,i;
	CSN=0;
	status=SPI_RW(reg);
	for(i=0;i<uchars;i++)
	pBuf[i]=SPI_RW(0);
	CSN=1;
	return status;	
}
/*设置发送模式*/
void nRF24L01_Set_TX_Mode(U8 *TX_Data)
{
    CE=0;//待机（写寄存器之前一定要进入待机模式或掉电模式）
    SPI_W_DBuffer(W_REGISTER+TX_ADDR,TX_Addr,TX_ADDR_WITDH);/*写寄存器指令+接收节点地址+地址宽度*/
    SPI_W_DBuffer(W_REGISTER+RX_ADDR_P0,TX_Addr,TX_ADDR_WITDH);/*为了接收设备应答信号，接收通道0地址与发送地址相同*/
    SPI_W_DBuffer(W_TX_PLOAD,TX_Data,TX_DATA_WITDH);/*写有效数据地址+有效数据+有效数据宽度*/
    SPI_W_Reg(W_REGISTER+EN_AA,0x00);/*接收通道0自动应答*/
    SPI_W_Reg(W_REGISTER+EN_RX_ADDR,0x01);/*使能接收通道0*/
    SPI_W_Reg(W_REGISTER+SETUP_RETR,0x00);/*自动重发延时250US+86US，重发10次*/
    SPI_W_Reg(W_REGISTER+RF_CH,40);/*(2400)MHZ射频通道*/
    SPI_W_Reg(W_REGISTER+RF_SETUP,0x07);/*1Mbps速率,发射功率:0DBM,低噪声放大器增益*/
    SPI_W_Reg(W_REGISTER+CONFIG,0x0e);/*发送模式,上电,16位CRC校验,CRC使能*/
    CE=1;//启动发射
    _delay_ms(5);/*CE高电平持续时间最少10US以上*/
}
void setRX_Mode()
{
	CE=0;
  	SPI_W_DBuffer(W_REGISTER + RX_ADDR_P0, TX_Addr,TX_ADDR_WITDH); // 写接收地址到0通道
  	SPI_W_Reg(W_REGISTER + EN_AA, 0x00);      // Enable Auto.Ack:Pipe0
  	SPI_W_Reg(W_REGISTER + EN_RX_ADDR, 0x01);  // Enable Pipe0
  	SPI_W_Reg(W_REGISTER + RF_CH,40);        // Select 工作频段 channel 2.4G
  	SPI_W_Reg(W_REGISTER + RX_PW_P0, RX_DATA_WITDH); // Select same RX payload width as TX Payload width
  	SPI_W_Reg(W_REGISTER + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_W_Reg(W_REGISTER + CONFIG, 0x0f);     //IRQ中断响应16位CRC校验，接收模式

  	CE = 1; // Set CE pin high to enable RX device
	_delay_us(35);
} 
U8 Check_Rec(void)
{
    U8 status;
    sta=SPI_R_byte(R_REGISTER+STATUS);
    if(RX_DR)
    {
        CE=0;
        SPI_R_DBuffer(R_RX_PLOAD,RX_Buffer,RX_DATA_WITDH);
        status=1;
    }
    SPI_W_Reg(W_REGISTER+STATUS,0xff);
    return status;  
}
/*检测应答信号*/
U8 Check_Ack(void)
{
    sta=SPI_R_byte(R_REGISTER+STATUS);/*读取寄存状态*/
    if(TX_DS||MAX_RT)/*如果TX_DS或MAX_RT为1,则清除中断和清除TX_FIFO寄存器的值*/
    {	
		
        SPI_W_Reg(W_REGISTER+STATUS,0xff);
        CSN=0;
        SPI_RW(FLUSH_TX);/*如果没有这一句只能发一次数据，大家要注意*/
        CSN=1;
        return 0;
    }
    else
        return 1;
}