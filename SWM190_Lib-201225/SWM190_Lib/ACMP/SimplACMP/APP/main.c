#include "SWM190.h"

void SerialInit(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
// 	PORT_Init(PORTA, PIN1,  PORTA_PIN1_ACMP0_INP,  0);	//GPIOA.1配置为UART0 TXD
	PORT_Init(PORTA, PIN6,  PORTA_PIN6_ACMP0_INN,  0);
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_ACMP1_INN,  0);
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_ACMP1_INP,  0);
	PORT_Init(PORTA, PIN13, PORTA_PIN13_ACMP2_INN, 0);
	PORT_Init(PORTA, PIN14, PORTA_PIN14_ACMP2_INP, 0);
	
// 	SYS->ACMPCR &= ~SYS_ACMPCR_CMP1MODE_Msk;	//比较器模式
// 	SYS->ACMPCR |= (1 << SYS_ACMPCR_CMP1ON_Pos) | (1 << SYS_ACMPCR_CMP1HYS_Pos) | (1 << SYS_ACMPCR_CMP1IE_Pos);	//开启迟滞、使能中断
	SYS->ACMPCR &= ~SYS_ACMPCR_CMP2MODE_Msk;	//比较器模式
	SYS->ACMPCR |= (1 << SYS_ACMPCR_CMP2ON_Pos) | (1 << SYS_ACMPCR_CMP2HYS_Pos) | (1 << SYS_ACMPCR_CMP2IE_Pos);	//开启迟滞、使能中断
	
	NVIC_EnableIRQ(XTAL_ACMP_IRQn);
	
	while(1==1)
	{
	}
}


void XTAL_ACMP_Handler(void)
{
	if(SYS->ACMPSR & SYS_ACMPSR_CMP0IF_Msk)
	{
		SYS->ACMPSR = (1 << SYS_ACMPSR_CMP0IF_Pos);
		
		printf("ACMP0 Interrupt Happened\r\n");
	}
	else if(SYS->ACMPSR & SYS_ACMPSR_CMP1IF_Msk)
	{
		SYS->ACMPSR = (1 << SYS_ACMPSR_CMP1IF_Pos);
		
		printf("ACMP1 Interrupt Happened\r\n");
	}
	else if(SYS->ACMPSR & SYS_ACMPSR_CMP2IF_Msk)
	{
		SYS->ACMPSR = (1 << SYS_ACMPSR_CMP2IF_Pos);
		
		printf("ACMP2 Interrupt Happened\r\n");
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
