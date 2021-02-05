#include "SWM190.h"


void SerialInit(void);

int main(void)
{
	uint32_t addr;
	
	/* 注意：
	*  执行CACHE_Unlock(addr)时，CPU不能访问addr地址的代码，即下面这段代码和CACHE_Prefet()、CACHE_Unlock()两个函数不能在要锁存的地址范围内
	*  所以，最好将下面这段代码以及CACHE_Prefet()、CACHE_Unlock()两个函数定位到程序的尾部、或者拷贝到RAM中
	***************************************/
	for(addr = 0; addr < 256; addr += 128)	// 将程序前256字节锁存在Cache中，在分散加载文件中将向量表和中断服务程序定位到程序前部
	{
		uint32_t res;
		
		do {
			res = CACHE_Prefet(addr);
			
			if(res == CACHE_RES_FAIL) CACHE_Unlock(addr);
		} while(res == CACHE_RES_FAIL);
	}
	
	SystemInit();
	
	SerialInit();
	
	SysTick_Config(SystemCoreClock / 10);
   	
	while(1==1)
	{
	}
}

__attribute__((section("..LockInCache")))
void SysTick_Handler(void)
{
	
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
