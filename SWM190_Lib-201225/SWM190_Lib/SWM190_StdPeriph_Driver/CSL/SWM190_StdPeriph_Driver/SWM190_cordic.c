/****************************************************************************************************************************************** 
* 文件名称: SWM190_cordic.c
* 功能说明:	SWM190单片机的CORDIC功能驱动库
* 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项:
* 版本日期:	V1.0.0		2016年1月30日
* 升级记录:  
*
*
*******************************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology
*******************************************************************************************************************************************/
#include "SWM190.h"
#include "SWM190_cordic.h"


/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_Init()
* 功能说明:	CORDIC初始化
* 输    入: CORDIC_TypeDef * CORDICx	指定要被设置的CORDIC，有效值包括CORDIC
* 输    出: 无
* 注意事项: CORDIC模块依赖DIV模块，因为CORDIC_Arctan()要使用硬件除法模块计算参数值
******************************************************************************************************************************************/
void CORDIC_Init(CORDIC_TypeDef * CORDICx)
{
	switch((uint32_t)CORDICx)
	{
	case ((uint32_t)CORDIC):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_CORDIC_Pos);
		break;
	}
}


/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_FullCycleSin()
* 功能说明:	使用CORDIC模块计算sin
* 输    入: double radian		要计算的弧度值
* 输    出: double
* 注意事项: 无
******************************************************************************************************************************************/
double CORDIC_FullCycleSin(double radian)
{
	double res;
	uint32_t negative = 0;
	
	while(radian < 0) radian += 2*CORDIC_PI;
	
	while(radian > 2*CORDIC_PI) radian -= 2*CORDIC_PI;
	
	if(radian > CORDIC_PI)
	{
		negative = 1;
		
		radian -= CORDIC_PI;
	}
	
	if(radian > CORDIC_PI/2)
	{
		radian = CORDIC_PI - radian;
	}
	
	if(radian < 0.01)
	{
		res = 0;
	}
	else if(radian > 1.56)
	{
		res = 1;
	}
	else
	{
		CORDIC_Sin(radian * 16384);
		while(CORDIC_Sin_IsDone() == 0);
		res = CORDIC_Sin_Result()/16384.0;
	}
	
	if(negative) res = 0 - res;
	
	return res;
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_FullCycleSin_Angle()
* 功能说明:	使用CORDIC模块计算sin
* 输    入: double angle		要计算的角度值
* 输    出: double
* 注意事项: 无
******************************************************************************************************************************************/
double CORDIC_FullCycleSin_Angle(double angle)
{
	double radian = angle / 180 * CORDIC_PI;
	
	return CORDIC_FullCycleSin(radian);
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_FullCycleCos()
* 功能说明:	使用CORDIC模块计算cos
* 输    入: double radian		要计算的弧度值
* 输    出: double
* 注意事项: 无
******************************************************************************************************************************************/
double CORDIC_FullCycleCos(double radian)
{
	double res;
	uint32_t negative = 0;
	
	while(radian < 0) radian += 2*CORDIC_PI;
	
	while(radian > 2*CORDIC_PI) radian -= 2*CORDIC_PI;
	
	if(radian > CORDIC_PI)
	{
		radian = 2*CORDIC_PI - radian;
	}
	
	if(radian > CORDIC_PI/2)
	{
		negative = 1;
		
		radian = CORDIC_PI - radian;
	}
	
	if(radian < 0.01)
	{
		res = 1;
	}
	else if(radian > 1.56)
	{
		res = 0;
	}
	else
	{
		CORDIC_Cos(radian * 16384);
		while(CORDIC_Cos_IsDone() == 0);
		res = CORDIC_Cos_Result()/16384.0;
	}
	
	if(negative) res = 0 - res;
	
	return res;
}

/****************************************************************************************************************************************** 
* 函数名称:	CORDIC_FullCycleCos_Angle()
* 功能说明:	使用CORDIC模块计算cos
* 输    入: double angle		要计算的角度值
* 输    出: double
* 注意事项: 无
******************************************************************************************************************************************/
double CORDIC_FullCycleCos_Angle(double angle)
{
	double radian = angle / 180 * CORDIC_PI;
	
	return CORDIC_FullCycleCos(radian);
}
