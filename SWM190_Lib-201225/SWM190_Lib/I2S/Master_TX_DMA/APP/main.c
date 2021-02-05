#include "SWM190.h"

#define TX_LEN	24
uint16_t TX_Buffer[TX_LEN] = {0x0001, 0x0102, 0x0203, 0x0304, 0x0405, 0x0506, 0x0607, 0x0708, 0x0809, 0x090A, 0x0A0B, 0x0B0C, 
							  0x0C0D, 0x0D0E, 0x0E0F, 0x0F10, 0x1011, 0x1112, 0x1213, 0x1314, 0x1415, 0x1516, 0x1617, 0x1718};

volatile uint32_t TX_Times = 0;
volatile uint32_t TX_Complete = 0;

void SerialInit(void);

int main(void)
{	
	uint32_t i;
	I2S_InitStructure I2S_initStruct;
	DMA_InitStructure DMA_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		// 调试指示信号
	
	PORT_Init(PORTA, PIN8,  PORTA_PIN8_SPI0_SSEL,  0);	//I2S_WS
	PORT_Init(PORTA, PIN11, PORTA_PIN11_SPI0_SCLK, 0);	//I2S_CK
	PORT_Init(PORTA, PIN10, PORTA_PIN10_SPI0_MOSI, 0);	//I2S_DO
// 	PORT_Init(PORTA, PIN9,  PORTA_PIN9_SPI0_MISO,  1);	//I2S_DI
	PORT_Init(PORTD, PIN7,  PORTD_PIN7_I2S0_MCLK,  0);	//I2S_MCLK
	
	I2S_initStruct.Mode = I2S_MASTER_TX;
	I2S_initStruct.FrameFormat = I2S_I2S_PHILIPS;
	I2S_initStruct.DataLen = I2S_DATALEN_16;
	I2S_initStruct.ClkFreq = 44100 * 2 * 16;	// 44.1K
	I2S_initStruct.RXThresholdIEn = 0;
	I2S_initStruct.TXThresholdIEn = 0;
	I2S_initStruct.TXCompleteIEn  = 1;
	I2S_Init(SPI0, &I2S_initStruct);
	
	I2S_MCLKConfig(SPI0, 1, 44100 * 256);
	
	I2S_Open(SPI0);
	
	
	// SPI0 TX DMA
	SPI0->CTRL |= (1 << SPI_CTRL_DMATXEN_Pos);
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_HALFWORD;
	DMA_initStruct.Count = TX_LEN;
	DMA_initStruct.SrcAddr = (uint32_t)TX_Buffer;
	DMA_initStruct.SrcAddrInc = 1;
	DMA_initStruct.DstAddr = (uint32_t)&SPI0->DATA;
	DMA_initStruct.DstAddrInc = 0;
	DMA_initStruct.Trigger = DMA_CH0_SPI0TX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.DoneIE = 1;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	
	DMA_CH_Open(DMA_CH0);
	
	while(1==1)
	{
		if(TX_Complete)
		{
			TX_Complete = 0;
			
			for(i = 0; i < SystemCoreClock/20; i++)  __NOP();		// 延时一会儿
			
			TX_Times = 0;
						
			I2S_Open(SPI0);	
			DMA_CH_Open(DMA_CH0);	// 重新开始，再次搬运
		}
	}
}

void DMA_Handler(void)
{
	if(DMA_CH_INTStat(DMA_CH0))
	{
		DMA_CH_INTClr(DMA_CH0);
		
		GPIO_InvBit(GPIOA, PIN5);
		
		if(++TX_Times < 3)
		{
			DMA_CH_Open(DMA_CH0);
		}
	}
}

void SPI0_Handler(void)
{
	if(SPI_INTTXCompleteStat(SPI0))
	{
		SPI_INTTXCompleteClr(SPI0);
		
		GPIO_InvBit(GPIOA, PIN5);
		
		I2S_Close(SPI0);
		
		TX_Complete = 1;
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
