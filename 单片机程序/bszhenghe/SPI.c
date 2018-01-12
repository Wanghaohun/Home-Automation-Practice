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
/******************************************************************
// nRF24L01�Ĵ�����ַ
*******************************************************************/
#define CONFIG      0x00  // ���üĴ���
#define EN_AA       0x01  // ���Զ�Ӧ�𡱹��ܼĴ�
#define EN_RX_ADDR  0x02  // ����ͨ��ʹ�ܼĴ���
#define SETUP_AW    0x03  // ��ַ������üĴ���
#define SETUP_RETR  0x04  // �Զ��ط����üĴ���
#define RF_CH       0x05  // ��Ƶͨ��Ƶ�����üĴ���
#define RF_SETUP    0x06  // ��Ƶ���üĴ���
#define STATUS      0x07  // ״̬�Ĵ���
#define OBSERVE_TX  0x08  // ���ͼ��Ĵ���
#define CD          0x09  // �ز����Ĵ���
#define RX_ADDR_P0  0x0A  // ����ͨ��0���յ�ַ�Ĵ���
#define RX_ADDR_P1  0x0B  // ����ͨ��1���յ�ַ�Ĵ���
#define RX_ADDR_P2  0x0C  // ����ͨ��2���յ�ַ�Ĵ���
#define RX_ADDR_P3  0x0D  // ����ͨ��3���յ�ַ�Ĵ���
#define RX_ADDR_P4  0x0E  // ����ͨ��4���յ�ַ�Ĵ���
#define RX_ADDR_P5  0x0F  // ����ͨ��5���յ�ַ�Ĵ���
#define TX_ADDR     0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0    0x11  // ����ͨ��0��Ч���ݿ�����üĴ���
#define RX_PW_P1    0x12  // ����ͨ��1��Ч���ݿ�����üĴ���
#define RX_PW_P2    0x13  // ����ͨ��2��Ч���ݿ�����üĴ���
#define RX_PW_P3    0x14  // ����ͨ��3��Ч���ݿ�����üĴ���
#define RX_PW_P4    0x15  // ����ͨ��4��Ч���ݿ�����üĴ���
#define RX_PW_P5    0x16  // ����ͨ��5��Ч���ݿ�����üĴ���
#define FIFO_STATUS 0x17  // FIFO״̬�Ĵ���
//*********************************************************************************
#define TX_ADDR_WITDH 5//���͵�ַ�������Ϊ5���ֽ�
#define RX_ADDR_WITDH 5//���յ�ַ�������Ϊ5���ֽ�
#define TX_DATA_WITDH 8//
#define RX_DATA_WITDH 8
/******************************************************************
// nRF24L01ָ���ʽ��
*******************************************************************/
#define R_REGISTER    0x00  // ���Ĵ���
#define W_REGISTER    0x20  // д�Ĵ���
#define R_RX_PLOAD    0x61  // ��RX FIFO��Ч���ݣ�1-32�ֽڣ�����������ɺ����ݱ������Ӧ���ڽ���ģʽ
#define W_TX_PLOAD    0xA0  // дTX FIFO��Ч���ݣ�1-32�ֽڣ�д�������ֽ�0��ʼ��Ӧ���ڷ���ģʽ
#define FLUSH_TX    0xE1  // ���TX FIFO�Ĵ�����Ӧ���ڷ���ģʽ
#define FLUSH_RX    0xE2  // ���RX FIFO�Ĵ�����Ӧ���ڽ���ģʽ
#define REUSE_TX_PL 0xE3  // ����ʹ����һ����Ч���ݣ���CEΪ�߹����У����ݰ������ϵ����·���
#define NOP         0xFF  // �ղ���������������״̬�Ĵ���
U8 sta;    // ״̬����
#define RX_DR  (sta & 0x40)  // ���ճɹ��жϱ�־
#define TX_DS  (sta & 0x20)  // ����ɹ��жϱ�־
#define MAX_RT (sta & 0x10)  // �ط�����жϱ�־
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
    CE=0;//����ģʽ��
    CSN=1;
    SCK=0;
    IRQ=1;
}
/*SPIʱ����*/
U8 SPI_RW(U8 byte)
{
    U8 i;
    for(i=0;i<8;i++)//һ�ֽ�8λѭ��8��д��
    {
        if(byte&0x80)//����������λ��1
            MOSI=1;//��NRF24L01д1
        else //����д0
            MOSI=0;
        byte<<=1;//��һλ�Ƶ����λ
        SCK=1;//SCK���ߣ�д��һλ���ݣ�ͬʱ��ȡһλ����
        if(MISO)
            byte|=0x01;
        SCK=0;//SCK����
    }
    return byte;//���ض�ȡһ�ֽ� 
}
/*SPIд�Ĵ���һ�ֽں���*/
/*reg:�Ĵ�����ַ*/
/*value:һ�ֽڣ�ֵ��*/
U8 SPI_W_Reg(U8 reg,U8 value)
{
    U8 status;//����״̬
    CSN=0;//SPIƬѡ
    status=SPI_RW(reg);//д��Ĵ�����ַ��ͬʱ��ȡ״̬
    SPI_RW(value);//д��һ�ֽ�
    CSN=1;//
    return status;//����״̬
}
/*SPI��һ�ֽ�*/
U8 SPI_R_byte(U8 reg)
{
    U8 reg_value;
    CSN=0;//SPIƬѡ
    SPI_RW(reg);//д���ַ
    reg_value=SPI_RW(0);//��ȡ�Ĵ�����ֵ
    CSN=1;
    return reg_value;//���ض�ȡ��ֵ
}
/*SPI��ȡRXFIFO�Ĵ�������*/
/*reg:�Ĵ�����ַ*/
/*Dat_Buffer:�������ȡ������*/
/*DLen:���ݳ���*/
U8 SPI_R_DBuffer(U8 reg,U8 *Dat_Buffer,U8 Dlen)
{
    U8 status,i;
    CSN=0;//SPIƬѡ
    status=SPI_RW(reg);//д��Ĵ�����ַ��ͬʱ״̬
    for(i=0;i<Dlen;i++)
    {
        Dat_Buffer[i]=SPI_RW(0);//�洢����
    }
    CSN=1;
    return status;
}
/*SPI��TXFIFO�Ĵ���д������*/
/*reg:д��Ĵ�����ַ*/
/*TX_Dat_Buffer:�����Ҫ���͵�����*/
/*Dlen:���ݳ���*/  
U8 SPI_W_DBuffer(U8 reg,U8 *TX_Dat_Buffer,U8 Dlen)
{
    U8 status,i;
    CSN=0;//SPIƬѡ������ʱ��
    status=SPI_RW(reg);
    for(i=0;i<Dlen;i++)
    {
        SPI_RW(TX_Dat_Buffer[i]);//��������
    }
    CSN=1;
    return status;  
}
U8 SPI_Read_Buf(U8 reg,U8 *pBuf,U8 uchars)//������ֽڲ������˵�ַ��״̬
{
	U8 status,i;
	CSN=0;
	status=SPI_RW(reg);
	for(i=0;i<uchars;i++)
	pBuf[i]=SPI_RW(0);
	CSN=1;
	return status;	
}
/*���÷���ģʽ*/
void nRF24L01_Set_TX_Mode(U8 *TX_Data)
{
    CE=0;//������д�Ĵ���֮ǰһ��Ҫ�������ģʽ�����ģʽ��
    SPI_W_DBuffer(W_REGISTER+TX_ADDR,TX_Addr,TX_ADDR_WITDH);/*д�Ĵ���ָ��+���սڵ��ַ+��ַ���*/
    SPI_W_DBuffer(W_REGISTER+RX_ADDR_P0,TX_Addr,TX_ADDR_WITDH);/*Ϊ�˽����豸Ӧ���źţ�����ͨ��0��ַ�뷢�͵�ַ��ͬ*/
    SPI_W_DBuffer(W_TX_PLOAD,TX_Data,TX_DATA_WITDH);/*д��Ч���ݵ�ַ+��Ч����+��Ч���ݿ��*/
    SPI_W_Reg(W_REGISTER+EN_AA,0x00);/*����ͨ��0�Զ�Ӧ��*/
    SPI_W_Reg(W_REGISTER+EN_RX_ADDR,0x01);/*ʹ�ܽ���ͨ��0*/
    SPI_W_Reg(W_REGISTER+SETUP_RETR,0x00);/*�Զ��ط���ʱ250US+86US���ط�10��*/
    SPI_W_Reg(W_REGISTER+RF_CH,40);/*(2400)MHZ��Ƶͨ��*/
    SPI_W_Reg(W_REGISTER+RF_SETUP,0x07);/*1Mbps����,���书��:0DBM,�������Ŵ�������*/
    SPI_W_Reg(W_REGISTER+CONFIG,0x0e);/*����ģʽ,�ϵ�,16λCRCУ��,CRCʹ��*/
    CE=1;//��������
    _delay_ms(5);/*CE�ߵ�ƽ����ʱ������10US����*/
}
void setRX_Mode()
{
	CE=0;
  	SPI_W_DBuffer(W_REGISTER + RX_ADDR_P0, TX_Addr,TX_ADDR_WITDH); // д���յ�ַ��0ͨ��
  	SPI_W_Reg(W_REGISTER + EN_AA, 0x00);      // Enable Auto.Ack:Pipe0
  	SPI_W_Reg(W_REGISTER + EN_RX_ADDR, 0x01);  // Enable Pipe0
  	SPI_W_Reg(W_REGISTER + RF_CH,40);        // Select ����Ƶ�� channel 2.4G
  	SPI_W_Reg(W_REGISTER + RX_PW_P0, RX_DATA_WITDH); // Select same RX payload width as TX Payload width
  	SPI_W_Reg(W_REGISTER + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_W_Reg(W_REGISTER + CONFIG, 0x0f);     //IRQ�ж���Ӧ16λCRCУ�飬����ģʽ

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
/*���Ӧ���ź�*/
U8 Check_Ack(void)
{
    sta=SPI_R_byte(R_REGISTER+STATUS);/*��ȡ�Ĵ�״̬*/
    if(TX_DS||MAX_RT)/*���TX_DS��MAX_RTΪ1,������жϺ����TX_FIFO�Ĵ�����ֵ*/
    {	
		
        SPI_W_Reg(W_REGISTER+STATUS,0xff);
        CSN=0;
        SPI_RW(FLUSH_TX);/*���û����һ��ֻ�ܷ�һ�����ݣ����Ҫע��*/
        CSN=1;
        return 0;
    }
    else
        return 1;
}