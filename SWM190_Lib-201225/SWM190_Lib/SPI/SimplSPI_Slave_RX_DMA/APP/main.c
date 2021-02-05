#include "SWM190.h"

#define BUF_SIZE  3
char RX_Buffer[BUF_SIZE] = {0};

#define TX_LEN  BUF_SIZE/1

void SerialInit(void);
void SPIMstInit(void);
void SPISlvInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	SPIMstInit();
	SPISlvInit();
	
	GPIO_Init(GPIOA, PIN4, 0, 1, 0, 0);
	
	while(1==1)
	{
		while(GPIO_GetBit(GPIOA, PIN4) == 1) __NOP();	//等待下降沿
		
		GPIO_ClrBit(GPIOA, PIN8);	// SPI_CS_Low()
		for(i = 0; i < 240; i++);	// CS拉低后需要延时一下再发送
		for(i = 0; i < TX_LEN; i++)
		{
			SPI0->DATA = i;
			while(SPI_IsTXFull(SPI0)) __NOP();
		}
		while(SPI_IsTXEmpty(SPI0) == 0);
		for(i = 0; i < 120; i++);	// 发送FIFO虽已空，但最后一个数据还在发送移位寄存器里，需要延时等待它发送出去
		GPIO_SetBit(GPIOA, PIN8);	// SPI_CS_High()
		
		for(i = 0; i < SystemCoreClock/16; i++) __NOP();	//按键消抖
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
	SPI_initStruct.RXThreshold = 4;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold = 4;
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn  = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	
	SPI_Open(SPI0);
}

void SPISlvInit(void)
{
	SPI_InitStructure SPI_initStruct;
	DMA_InitStructure DMA_initStruct;
	
	PORT_Init(PORTC, PIN4, PORTC_PIN4_SPI1_SSEL, 1);
	PORT_Init(PORTC, PIN5, PORTC_PIN5_SPI1_MISO, 0);
	PORT_Init(PORTC, PIN6, PORTC_PIN6_SPI1_MOSI, 1);
	PORT_Init(PORTC, PIN7, PORTC_PIN7_SPI1_SCLK, 1);
	
	SPI_initStruct.clkDiv = 4;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_SECOND_EDGE;
	SPI_initStruct.IdleLevel = SPI_HIGH_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 0;
	SPI_initStruct.RXThreshold = 4;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold = 4;
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn  = 0;
	SPI_Init(SPI1, &SPI_initStruct);
	
	SPI_Open(SPI1);
	

	// RX DMA
	SPI1->CTRL |= (1 << SPI_CTRL_DMARXEN_Pos);
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.Count = BUF_SIZE;
	DMA_initStruct.SrcAddr = (uint32_t)&SPI1->DATA;
	DMA_initStruct.SrcAddrInc = 0;
	DMA_initStruct.DstAddr = (uint32_t)RX_Buffer;
	DMA_initStruct.DstAddrInc = 1;
	DMA_initStruct.Trigger = DMA_CH0_SPI1RX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.DoneIE = 1;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	
	DMA_CH_Open(DMA_CH0);
}

void DMA_Handler(void)
{
	uint32_t i;
	
	if(DMA_CH_INTStat(DMA_CH0))
	{
		DMA_CH_INTClr(DMA_CH0);
		
		for(i = 0; i < BUF_SIZE; i++)  printf("%02X, ", RX_Buffer[i]);
		printf("\r\n\r\n");
		
		DMA_CH_Open(DMA_CH0);	// 重新开始，再次搬运
	}
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
