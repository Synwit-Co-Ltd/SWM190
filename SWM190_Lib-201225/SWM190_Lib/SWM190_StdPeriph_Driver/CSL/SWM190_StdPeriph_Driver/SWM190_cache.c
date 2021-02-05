/****************************************************************************************************************************************** 
* 文件名称: SWM190_cache.c
* 功能说明:	SWM190单片机的CACHE操作驱动函数
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
#include "SWM190_cache.h"


/****************************************************************************************************************************************** 
* 函数名称:	CACHE_Reset()
* 功能说明:	CHCHE复位，清除CHCHE所有标志和内容
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CACHE_Reset(void)
{
	while((CACHE->CR & CACHE_CR_IDLE_Msk) == 0) __NOP();
	
	CACHE->CR |= (1 << CACHE_CR_RST_Pos);
}

/****************************************************************************************************************************************** 
* 函数名称:	CACHE_Prefet()
* 功能说明:	CHCHE预取锁存
* 输    入: uint32_t addr		将Flash中addr开始的128字节代码预取到Cache中并锁定
* 输    出: uint32_t			CACHE_RES_OK、CACHE_RES_FAIL
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t CACHE_Prefet(uint32_t addr)
{
	uint32_t res;
	
	CACHE->IE |= (1 << CACHE_IE_PREFET_Pos);	// 只有中断使能时相应中断状态才能置起
	
	while((CACHE->PREFCR & CACHE_PREFCR_RDY_Msk) == 0) __NOP();
	
	CACHE->PREFCR = (1 << CACHE_PREFCR_PREFET_Pos) |
					(addr << CACHE_PREFCR_ADDR_Pos);
	
	while((CACHE->IF & (CACHE_IF_PREOK_Msk | CACHE_IF_PREFAIL_Msk)) == 0) __NOP();
	
	if(CACHE->IF & CACHE_IF_PREOK_Msk) res = CACHE_RES_OK;
	else							   res = CACHE_RES_FAIL;
	
	CACHE->IF = CACHE_IF_PREOK_Msk | CACHE_IF_PREFAIL_Msk;	//清除标志
		
	return res;
}

/****************************************************************************************************************************************** 
* 函数名称:	CACHE_Unlock()
* 功能说明:	释放锁存在Cache中的代码
* 输    入: uint32_t addr		要解锁释放的代码的地址
* 输    出: uint32_t			CACHE_RES_OK、CACHE_RES_FAIL
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t CACHE_Unlock(uint32_t addr)
{
	uint32_t res;
	
	CACHE->IE |= (1 << CACHE_IE_UNLOCK_Pos);	// 只有中断使能时相应中断状态才能置起
	
	while((CACHE->PREFCR & CACHE_PREFCR_RDY_Msk) == 0) __NOP();
	
	CACHE->PREFCR = (1 << CACHE_PREFCR_UNLOCK_Pos) |
					(addr << CACHE_PREFCR_ADDR_Pos);
	
	while((CACHE->IF & (CACHE_IF_UNLOK_Msk | CACHE_IF_UNLFAIL_Msk)) == 0) __NOP();
	
	if(CACHE->IF & CACHE_IF_UNLOK_Msk) res = CACHE_RES_OK;
	else 							   res = CACHE_RES_FAIL;
	
	CACHE->IF = CACHE_IF_UNLOK_Msk | CACHE_IF_UNLFAIL_Msk;	//清除标志
	
	return res;
}
