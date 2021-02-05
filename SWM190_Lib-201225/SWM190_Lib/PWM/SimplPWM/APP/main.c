#include "SWM190.h"

uint32_t Buf_Test[4] = {0x100,0x200,0x300,0x400};
int main(void)
{
	PWM_InitStructure  PWM_initStruct;
	DMA_InitStructure DMA_initStruct;
  ADC_InitStructure ADC_initStruct;
  
	SystemInit();
	
  //PWM
	PWM_initStruct.PWMnXN = 0;					//只输出PWM0A信号、不输出PWM0AN信号
	PWM_initStruct.clkdiv = PWM_CLKDIV_8;		//F_PWM = 24M/8 = 3M	
	PWM_initStruct.cycle = 10000;				//3M/10000 = 300Hz，PWMnXN = 1时频率降低到150Hz
	PWM_initStruct.hduty =  2500;				//2500/10000 = 25%
	PWM_initStruct.deadzone = 00;
	PWM_initStruct.initLevel = 1;
	PWM_initStruct.HEndIE = 0;
	PWM_initStruct.NCycleIE = 0;
	PWM_initStruct.HCycleIE = 0;	
	PWM_Init(PWM0A, &PWM_initStruct);
//	PWM_Init(PWM0B, &PWM_initStruct);
//	PWM_Init(PWM1A, &PWM_initStruct);
//	PWM_Init(PWM1B, &PWM_initStruct);
	
	PORT_Init(PORTB, PIN1, PORTB_PIN1_PWM0A,  0);
//	PORT_Init(PORTB, PIN2, PORTB_PIN2_PWM0AN, 0);
//	PORT_Init(PORTB, PIN3, PORTB_PIN3_PWM0B,  0);
//	PORT_Init(PORTB, PIN4, PORTB_PIN4_PWM0BN, 0);
//	PORT_Init(PORTB, PIN5, PORTB_PIN5_PWM1A,  0);
//	PORT_Init(PORTB, PIN6, PORTB_PIN6_PWM1AN, 0);
//	PORT_Init(PORTD, PIN2, PORTD_PIN2_PWM1B,  0);
//	PORT_Init(PORTD, PIN3, PORTD_PIN3_PWM1BN, 0);
	
	PWM_Start(PWM0A);
//	PWM_Start(PWM0B);
//	PWM_Start(PWM1A);
//	PWM_Start(PWM1B);
	PWMG->CHEN |= (1 << PWMG_CHEN_PWM0A_Pos) | (1 << PWMG_CHEN_PWM0B_Pos) | (1 << PWMG_CHEN_PWM1A_Pos) | (1 << PWMG_CHEN_PWM1B_Pos);	//多路同时启动
	
  //ADC
	PORT_Init(PORTD, PIN1,  PORTD_PIN1_ADC1_IN0,  0);	//PD.1 => ADC1.CH0
//	PORT_Init(PORTD, PIN0,  PORTD_PIN0_ADC1_IN1,  0);	//PD.0 => ADC1.CH1
//	PORT_Init(PORTC, PIN5,  PORTC_PIN5_ADC1_IN2,  0);	//PC.5 => ADC1.CH2
//	PORT_Init(PORTC, PIN4,  PORTC_PIN4_ADC1_IN3,  0);	//PC.4 => ADC1.CH3
//	PORT_Init(PORTC, PIN3,  PORTC_PIN3_ADC1_IN4,  0);	//PC.3 => ADC1.CH4
//	PORT_Init(PORTC, PIN2,  PORTC_PIN2_ADC1_IN5,  0);	//PC.2 => ADC1.CH5
//	PORT_Init(PORTB, PIN15, PORTB_PIN15_ADC1_IN6, 0);	//PB.15=> ADC1.CH6
//	PORT_Init(PORTB, PIN14, PORTB_PIN14_ADC1_IN7, 0);	//PB.14=> ADC1.CH7
	
	ADC_initStruct.clk_src = ADC_CLKSRC_HRC;
	ADC_initStruct.channels = ADC_CH0;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE1;
	ADC_initStruct.trig_src = ADC_TRIGGER_TIMR2;
	ADC_initStruct.Continue = 0;						//连续模式
	ADC_initStruct.EOC_IEn = ADC_CH0;	
	ADC_initStruct.OVF_IEn = 0;
	ADC_Init(ADC1, &ADC_initStruct);					//配置ADC
	
	ADC_Open(ADC1);										//使能ADC
	
	ADC1->CTRL |= (1 << ADC_CTRL_RES2FIFO_Pos) | (1 << ADC_CTRL_DMAEN_Pos);
  
  
  
  //DMA
  DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_WORD;
	DMA_initStruct.Count = 4;
	DMA_initStruct.SrcAddr = (uint32_t)&Buf_Test;
	DMA_initStruct.SrcAddrInc = 1;
	DMA_initStruct.DstAddr = (uint32_t)&PWM0->HIGHA;
	DMA_initStruct.DstAddrInc = 0;
	DMA_initStruct.Trigger = DMA_CH1_ADC1;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.DoneIE = 1;

	DMA_CH_Init(DMA_CH1, &DMA_initStruct);
	DMA_CH_Open(DMA_CH1);

  
  //Timer
  TIMR_Init(TIMR2, TIMR_MODE_TIMER, 1, SystemCoreClock, 1);	//每0.5秒钟触发一次中断
	TIMR_Start(TIMR2);
  
  
  
	while(1==1)
	{
	
	}
}

void DMA_Handler(void)
{
	uint32_t i;
	
	if(DMA_CH_INTStat(DMA_CH1))
	{
		DMA_CH_INTClr(DMA_CH1);
		
		DMA_CH_Open(DMA_CH1);	// 重新开始，再次搬运
	}
}


void ADC1_GPIOA14_Handler(void)
{
	ADC1->IF = (1 << ADC_IF_CH0EOC_Pos);
	

}
void TIMR2_GPIOA15_Handler(void)
{
	TIMR_INTClr(TIMR2);
	
}


