#include "SWM190.h"

volatile uint32_t Period0, Period1, Period2;
volatile uint32_t Duty0, Duty1, Duty2;

void SerialInit(void);
void SignalTest(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	SignalTest();
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, 1, 0xFFFFFFFF, 0);
	
	PORT_Init(PORTB, PIN3, PORTB_PIN3_HALL_IN0, 1);
	PORT_Init(PORTB, PIN4, PORTB_PIN4_HALL_IN1, 1);
	PORT_Init(PORTB, PIN8, PORTB_PIN8_HALL_IN2, 1);
	
	TIMRG->HALLMD = (3 << TIMRG_HALLMD_IN0_Pos) |
					(3 << TIMRG_HALLMD_IN1_Pos) |
					(3 << TIMRG_HALLMD_IN2_Pos);	//三个通道都是双边沿触发
	TIMRG->HALLIE = 1;
	TIMRG->HALLIM = 0;
	NVIC_EnableIRQ(TIMR3_HALL_IRQn);
	
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
		printf("%dHz  %%%d      %dHz  %%%d      %dHz  %%%d\n", SystemCoreClock/Period0, Duty0*100/Period0, SystemCoreClock/Period1, Duty1*100/Period1, SystemCoreClock/Period2, Duty2*100/Period2);
	}
}


void TIMR3_HALL_Handler(void)
{
	static uint32_t HALLV0Save, HALLV1Save, HALLV2Save;
	
	if(TIMRG->HALLIF & TIMRG_HALLIF_IN0_Msk)
	{
		TIMRG->HALLIF = (1 << TIMRG_HALLIF_IN0_Pos);
		
		if(TIMRG->HALLST & TIMRG_HALLST_IN0_Msk)
		{
			Period0 = HALLV0Save - TIMRG->HALLV0;
			
			HALLV0Save = TIMRG->HALLV0;
		}
		else
		{
			Duty0 = HALLV0Save - TIMRG->HALLV0; 
		}
	}
	
	if(TIMRG->HALLIF & TIMRG_HALLIF_IN1_Msk)
	{
		TIMRG->HALLIF = (1 << TIMRG_HALLIF_IN1_Pos);
		
		if(TIMRG->HALLST & TIMRG_HALLST_IN1_Msk)
		{
			Period1 = HALLV1Save - TIMRG->HALLV1;
			
			HALLV1Save = TIMRG->HALLV1;
		}
		else
		{
			Duty1 = HALLV1Save - TIMRG->HALLV1; 
		}
	}
	
	if(TIMRG->HALLIF & TIMRG_HALLIF_IN2_Msk)
	{
		TIMRG->HALLIF = (1 << TIMRG_HALLIF_IN2_Pos);
		
		if(TIMRG->HALLST & TIMRG_HALLST_IN2_Msk)
		{
			Period2 = HALLV2Save - TIMRG->HALLV2;
			
			HALLV2Save = TIMRG->HALLV2;
		}
		else
		{
			Duty2 = HALLV2Save - TIMRG->HALLV2; 
		}
	}
}


void SignalTest(void)
{
	PWM_InitStructure PWM_initStruct;
	
	PORT_Init(PORTB, PIN1, PORTB_PIN1_PWM0A,  0);
	PORT_Init(PORTB, PIN2, PORTB_PIN2_PWM0AN, 0);
	PORT_Init(PORTB, PIN5, PORTB_PIN5_PWM1A,  0);
	PORT_Init(PORTB, PIN6, PORTB_PIN6_PWM1AN, 0);
	
	PWM_initStruct.PWMnXN = 1;					//同时输出PWM0A和PWM0AN
	PWM_initStruct.clkdiv = PWM_CLKDIV_8;		//F_PWM = 24M/8 = 3M
	PWM_initStruct.cycle = 30000;				//3M/30000 = 100Hz，PWMnXN = 1时频率降低到50Hz
	PWM_initStruct.hduty =  7500;				//15000/30000 = 50%
	PWM_initStruct.initLevel = 1;
	PWM_initStruct.HEndIE = 0;
	PWM_initStruct.NCycleIE = 0;
	PWM_initStruct.HCycleIE = 0;	
	PWM_Init(PWM0A, &PWM_initStruct);
	PWM_initStruct.hduty = 10000;
	PWM_Init(PWM1A, &PWM_initStruct);
	
	PWMG->CHEN |= (1 << PWMG_CHEN_PWM0A_Pos) | (1 << PWMG_CHEN_PWM1A_Pos);	//多路同时启动
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN1, PORTA_PIN1_UART0_TX, 0);	//GPIOA.1配置为UART0 TXD
	PORT_Init(PORTA, PIN0, PORTA_PIN0_UART0_RX, 1);	//GPIOA.0配置为UART0 RXD
 	
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
