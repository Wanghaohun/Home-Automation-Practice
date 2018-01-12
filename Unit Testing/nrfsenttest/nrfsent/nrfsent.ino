#include <SPI.h>
#include <Ethernet.h>
#define uint unsigned int
#define uchar unsigned char

#define TX_ADDR_WITDH 5//发送地址宽度设置为5个字节
#define RX_ADDR_WITDH 5
#define TX_DATA_WITDH 8
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
/******************************************************************
  // nRF24L01寄存器地址
*******************************************************************/
#define CONFIG      0x00  // 配置寄存器
#define EN_AA       0x01  // “自动应答”功能寄存器
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
uchar  sta; // 状态变量

//#define RX_DR  (sta&0x40)  // 接收成功中断标志
//#define TX_DS  (sta&0x20)  // 发射成功中断标志
#define MAX_RT (sta&0x10)  // 重发溢出中断标志

int CE = 3;
int CSN = 8;
int IRQ = 2;
int nrfSCK = 7;
int nrfMOSI = 5;
int nrfMISO = 6;  //这几个引脚arduino自己就已经定义好了


uchar TX_Addr[] = {0x34, 0x43, 0x10, 0x10, 0x01};
uchar TX_Buffer[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};
uchar RX_Buffer[RX_DATA_WITDH];

void nRF24L01_Init(void)
{
  delay(2);
  digitalWrite(CE, LOW);
  digitalWrite(CSN, HIGH);
  digitalWrite(nrfSCK, LOW);
  //    digitalWrite(nrfMOSI,LOW);
  //    digitalWrite(nrfMISO,LOW);
  digitalWrite(IRQ, HIGH);
   SPI_W_Reg(W_REGISTER + EN_AA, 0x00); /*接收通道0自动应答*/
  SPI_W_Reg(W_REGISTER + EN_RX_ADDR, 0x01); /*使能接收通道0*/
  SPI_W_Reg(W_REGISTER + SETUP_RETR, 0x00); /*自动重发延时250US+86US，重发10次*/
  SPI_W_Reg(W_REGISTER + RF_CH, 40); /*(2400)MHZ射频通道*/
  SPI_W_Reg(W_REGISTER + RF_SETUP, 0x07); /*1Mbps速率,发射功率:0DBM,低噪声放大器增益*/
  SPI_W_Reg(W_REGISTER + RX_PW_P0,TX_DATA_WITDH); /*设置接收数据长度*/
}

uchar SPI_RW(uchar byte)
{
  uchar i;
  for (i = 0; i < 8; i++)
  {
    if (byte & 0x80)
      digitalWrite(nrfMOSI, HIGH);
    else
      digitalWrite(nrfMOSI, LOW);
    byte <<= 1;
    digitalWrite(nrfSCK, HIGH);
    if (digitalRead(nrfMISO))
      byte |= 0x01;
    digitalWrite(nrfSCK, LOW);
  }
  return byte;
}

uchar SPI_W_Reg(uchar reg, uchar value)
{
  uchar status;
  digitalWrite(CSN, LOW);
  status = SPI_RW(reg);
  SPI_RW(value);
  digitalWrite(CSN, HIGH);
  return status;
}

uchar SPI_R_byte(uchar reg)
{
  uchar status;
  digitalWrite(CSN, LOW);
  SPI_RW(reg);
  status = SPI_RW(0);
  digitalWrite(CSN, HIGH);
  return status;
}

uchar SPI_R_DBuffer(uchar reg, uchar *Dat_Buffer, uchar Dlen)
{
  uchar reg_value, i;
  digitalWrite(CSN, LOW);
  reg_value = SPI_RW(reg);
  for (i = 0; i < Dlen; i++)
  {
    Dat_Buffer[i] = SPI_RW(0);
  }
  digitalWrite(CSN, HIGH);
  return reg_value;
}

uchar SPI_W_DBuffer(uchar reg, uchar *TX_Dat_Buffer, uchar Dlen)
{
  uchar reg_value, i;
  digitalWrite(CSN, LOW);
  reg_value = SPI_RW(reg);
  for (i = 0; i < Dlen; i++)
  {
    SPI_RW(TX_Dat_Buffer[i]);
  }
  digitalWrite(CSN, HIGH);
  return reg_value;
}

 uchar nRF24L01_Set_TX_Mode(uchar *TX_Data)
{
  digitalWrite(CE, LOW); //待机（写寄存器之前一定要进入待机模式或掉电模式）
  SPI_W_DBuffer(W_REGISTER + TX_ADDR, TX_Addr, TX_ADDR_WITDH); /*写寄存器指令+接收节点地址+地址宽度*/
  SPI_W_DBuffer(W_REGISTER + RX_ADDR_P0, TX_Addr, TX_ADDR_WITDH); /*为了接收设备应答信号，接收通道0地址与发送地址相同*/
  SPI_W_DBuffer(W_TX_PLOAD, TX_Data, TX_DATA_WITDH); /*写有效数据地址+有效数据+有效数据宽度*/
  SPI_W_Reg(W_REGISTER + CONFIG, 0x0e); /*发送模式,上电,16位CRC校验,CRC使能*/
  digitalWrite(CE, HIGH); //启动发射
  _delay_ms(10);/*CE高电平持续时间最少10US以上*/
}

uchar Check_Ack(void)
{ uchar TX_DS;
uchar stafifo;
  sta = SPI_R_byte(R_REGISTER + STATUS); /*读取寄存状态*/
  stafifo= SPI_R_byte(R_REGISTER + FIFO_STATUS);
  TX_DS=sta&0x20;
  if (TX_DS || MAX_RT) /*如果TX_DS或MAX_RT为1,则清除中断和清除TX_FIFO寄存器的值*/
  {
    SPI_W_Reg(W_REGISTER + STATUS, 0xff);
   digitalWrite(CSN, LOW);
    SPI_RW(FLUSH_TX);/*如果没有这一句只能发一次数据，大家要注意*/
   digitalWrite(CSN, HIGH);
    return 0;
  }
  else
    return 1;
}
void setup() {
  // 初始化串口通信
  Serial.begin(9600);
  pinMode(CE, OUTPUT);
  pinMode(CSN, OUTPUT);
  pinMode(nrfSCK, OUTPUT);
  pinMode(nrfMOSI, OUTPUT);
  pinMode(nrfMISO, INPUT);
  delay(1);
  nRF24L01_Init();
  // 等待1秒钟用于Ethernet扩展板或W5100芯片完成初始化
  delay(1000);
  // 如果连接成功，通过串口输出返回数据
}

void loop()
{ 
uchar shut[8]={'s','h','u','t','d','o','w','n'};
nRF24L01_Set_TX_Mode(shut);//发送数据
while (Check_Ack()); //等待发送完成
Serial.println("check out");
delay(3000);
}
