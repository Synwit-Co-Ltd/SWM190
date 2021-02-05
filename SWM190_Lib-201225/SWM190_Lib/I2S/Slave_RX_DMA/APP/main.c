#include "SWM190.h"

#define RX_LEN	24*3
uint16_t RX_Buffer[RX_LEN] = {0};

volatile uint32_t RX_Complete = 0;

void SerialInit(void);

int main(void)
{	
	uint32_t i;
	I2S_InitStructure I2S_initStruct;
	DMA_InitStructure DMA_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		// 调试指示信号
	
	PORT_Init(PORTA, PIN8,  PORTA_PIN8_SPI0_SSEL,  1);	//I2S_WS
	PORT_Init(PORTA, PIN11, PORTA_PIN11_SPI0_SCLK, 1);	//I2S_CK
 	PORT_Init(PORTA, PIN10, PORTA_PIN10_SPI0_MOSI, 1);	//I2S_DI
// 	PORT_Init(PORTA, PIN9,  PORTA_PIN9_SPI0_MISO,  0);	//I2S_DO
	
	I2S_initStruct.Mode = I2S_SLAVE_RX;
	I2S_initStruct.FrameFormat = I2S_I2S_PHILIPS;
	I2S_initStruct.DataLen = I2S_DATALEN_16;
	I2S_initStruct.ClkFreq = 0;
	I2S_initStruct.RXThresholdIEn = 0;
	I2S_initStruct.TXThresholdIEn = 0;
	I2S_initStruct.TXCompleteIEn  = 0;
	I2S_Init(SPI0, &I2S_initStruct);
	
	I2S_Open(SPI0);
	
	
	// SPI0 RX DMA
	SPI0->CTRL |= (1 << SPI_CTRL_DMARXEN_Pos);
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_HALFWORD;
	DMA_initStruct.Count = RX_LEN;
	DMA_initStruct.SrcAddr = (uint32_t)&SPI0->DATA;
	DMA_initStruct.SrcAddrInc = 0;
	DMA_initStruct.DstAddr = (uint32_t)RX_Buffer;
	DMA_initStruct.DstAddrInc = 1;
	DMA_initStruct.Trigger = DMA_CH1_SPI0RX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.DoneIE = 1;
	DMA_CH_Init(DMA_CH1, &DMA_initStruct);
	
	DMA_CH_Open(DMA_CH1);
	
	while(1==1)
	{
		if(RX_Complete == 1)
		{
			RX_Complete = 0;
			
			for(i = 0; i < RX_LEN; i++)
			{
				printf("%04X, ", RX_Buffer[i]);
			}
			printf("\r\n\r\n");
			
			I2S_Close(SPI0);
			I2S_Open(SPI0);
			DMA_CH_Open(DMA_CH1);	// 重新开始，再次搬运
		}
	}
}

void DMA_Handler(void)
{	
	if(DMA_CH_INTStat(DMA_CH1))
	{
		DMA_CH_INTClr(DMA_CH1);
		
		GPIO_InvBit(GPIOA, PIN5);
		
		RX_Complete = 1;
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
