#include "SWM190.h"

uint16_t *SPITXBuff = 0; 
uint32_t SPITXCount = 0;
uint32_t SPITXIndex = 0;

void SerialInit(void);
void SPIMstInit(void);
void SPIMstSend(uint16_t buff[], uint32_t cnt);

int main(void)
{	
	uint32_t i;
	uint16_t buff[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
	
	SystemInit();
	
	SerialInit();

	SPIMstInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//用作中断进入指示信号
	
	while(1==1)
	{
		SPIMstSend(buff, 16);
		
		for(i = 0; i < SystemCoreClock/100; i++) __NOP();
	}
}


void SPIMstInit(void)
{
	SPI_InitStructure SPI_initStruct;
	
//	PORT_Init(PORTA, PIN8,  PORTA_PIN8_SPI0_SSEL,  0);
	GPIO_Init(GPIOA, PIN8, 1, 0, 0, 0);					//软件控制片选
#define SPI_CS_Low()	GPIO_ClrBit(GPIOA, PIN8)
#define SPI_CS_High()	GPIO_SetBit(GPIOA, PIN8)
	SPI_CS_High();
	
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_SPI0_MISO,  1);
	PORT_Init(PORTA, PIN11, PORTA_PIN11_SPI0_SCLK, 0);
	PORT_Init(PORTA, PIN10, PORTA_PIN10_SPI0_MOSI, 0);
	
	SPI_initStruct.clkDiv = SPI_CLKDIV_32;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_SECOND_EDGE;
	SPI_initStruct.IdleLevel = SPI_HIGH_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 1;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold    = 4;	// 当发送FIFO中数据少于4个时产生中断
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn  = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	
	NVIC_EnableIRQ(SPI0_IRQn);
	
	SPI_Open(SPI0);
}

void SPIMstSend(uint16_t buff[], uint32_t cnt)
{	
	SPITXBuff = buff;
	SPITXCount = cnt;
	SPITXIndex = 0;
	
	SPI_CS_Low();
	
	SPI_INTTXCompleteEn(SPI0);
 	SPI_INTTXThresholdEn(SPI0);
}

void SPI0_Handler(void)
{
	if(SPI_INTTXThresholdStat(SPI0))
	{
		while((SPI_IsTXFull(SPI0) == 0) && (SPITXIndex < SPITXCount))
		{
			SPI_Write(SPI0, SPITXBuff[SPITXIndex++]);
		}
		
		if(SPITXIndex == SPITXCount)
		{
			SPI_INTTXThresholdDis(SPI0);
		}
		
		SPI_INTTXThresholdClr(SPI0);	//清除中断标志，必须在填充TX FIFO后清中断标志
	}
	
	if(SPI_INTTXCompleteStat(SPI0))
	{
		SPI_INTTXCompleteClr(SPI0);
		
		if(SPITXIndex == SPITXCount)	// 要发送的数据已全部填入SPI TX FIFO
		{
			SPI_CS_High();
			SPI_INTTXCompleteDis(SPI0);
		}
	}
	
	GPIO_InvBit(GPIOA, PIN5);
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN1, PORTA_PIN1_UART0_TX, 0);		//GPIOA.1配置为UART0 TXD
	PORT_Init(PORTA, PIN0, PORTA_PIN0_UART0_RX, 1);		//GPIOA.0配置为UART0 RXD
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* 函数名称: fputc()
* 功能说明: printf()使用此函数完成实际的串口打印动作
* 输    入: int ch		要打印的字符
*			FILE *f		文件句柄
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
