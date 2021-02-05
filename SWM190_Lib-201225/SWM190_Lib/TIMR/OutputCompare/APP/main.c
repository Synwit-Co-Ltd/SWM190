#include "SWM190.h"


int main(void)
{	
	SystemInit();
	
	PORT_Init(PORTB, PIN6, PORTB_PIN6_TIMR0_OUT, 0);
	
	TIMR_Init(TIMR0, TIMR_MODE_OC, 1, SystemCoreClock/100, 0);
	
	TIMR_OC_Init(TIMR0, SystemCoreClock/400, 0, 1);
	
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
	}
}
