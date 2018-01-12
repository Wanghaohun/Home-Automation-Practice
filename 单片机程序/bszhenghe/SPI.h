/*nrf初始化*/
extern void nRF24L01_Init(void);
/*SPI时序函数*/
extern unsigned char SPI_RW(unsigned char byte);
/*SPI写寄存器一字节函数*/
/*reg:寄存器地址*/
/*value:一字节（值）*/
extern unsigned char SPI_W_Reg(unsigned char reg,unsigned char value);
/*SPI读一字节*/
extern unsigned char SPI_R_byte(unsigned char reg);
/*SPI读取RXFIFO寄存器数据*/
/*reg:寄存器地址*/
/*Dat_Buffer:用来存读取的数据*/
/*DLen:数据长度*/
extern unsigned char SPI_R_DBuffer(unsigned char reg,unsigned char *Dat_Buffer,unsigned char Dlen);
/*SPI向TXFIFO寄存器写入数据*/
/*reg:写入寄存器地址*/
/*TX_Dat_Buffer:存放需要发送的数据*/
/*Dlen:数据长度*/             
extern unsigned char SPI_W_DBuffer(unsigned char reg,unsigned char *TX_Dat_Buffer,unsigned char Dlen);
/*设置发送模式*/  
extern void nRF24L01_Set_TX_Mode(unsigned char *TX_Data);
/*检测应答信号*/            
extern unsigned char Check_Rec(void);
/*检测应答信号*/
extern unsigned char Check_Ack(void);
extern void _delay_ms(unsigned int x);
extern void _delay_us(unsigned int x);
extern void setRX_Mode();
extern unsigned char nRF24L01_RxPacket(unsigned char *rx_buf);
extern unsigned char SPI_Read_Buf(unsigned char reg,unsigned char *pBuf,unsigned char uchars);
#define R_REGISTER    0x00  // 读寄存器
#define RX_ADDR_P0  0x0A  // 数据通道0接收地址寄存器
extern unsigned char RX_Buffer[8];