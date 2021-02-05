/****************************************************************************************************************************************** 
* 文件名称:	SWM190_flash.c
* 功能说明:	使用芯片的IAP功能将片上Flash模拟成EEPROM来保存数据，掉电后不丢失
* 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项:
* 版本日期: V1.0.0		2016年1月30日
* 升级记录: 
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
#include "SWM190_flash.h"


void FLASH_Init(void)
{
	SFC->TIM &= ~(SFC_TIM_WIP_CHK_ITV_Msk | SFC_TIM_WIP_CHK_LMT_Msk);
	SFC->TIM |= (0xFF << SFC_TIM_WIP_CHK_ITV_Pos) |
				(0xFF << SFC_TIM_WIP_CHK_LMT_Pos);
}


/****************************************************************************************************************************************** 
* 函数名称:	FLASH_Erase()
* 功能说明:	FLASH扇区擦除，每个扇区4K字节
* 输    入: uint32_t addr		要擦除扇区的地址，必须4K对齐，即addr%4096 == 0
* 输    出: uint32_t			FLASH_RES_OK、FLASH_RES_TO、FLASH_RES_ERR
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t FLASH_Erase(uint32_t addr)
{
	uint32_t res;
	
	__disable_irq();
	
	SFC->ADDR = addr;
	
	SFC->CFG &= ~SFC_CFG_CMDTYPE_Msk;
	SFC->CFG |= (1 << SFC_CFG_WREN_Pos) |
				(1 << SFC_CFG_CMDWREN_Pos) |
				(7 << SFC_CFG_CMDTYPE_Pos);
	SFC->CMD = FLASH_CMD_ERASE_SECT;
	
	SFC->GO = 1;
	while(SFC->GO);
	SFC->CFG &= ~SFC_CFG_WREN_Msk;
	
	if(SFC->IF & SFC_IF_ERR_Msk)     res = FLASH_RES_ERR;
	else if(SFC->IF & SFC_IF_TO_Msk) res = FLASH_RES_TO;
	else                             res = FLASH_RES_OK;
	
	__enable_irq();
	
	return res;
}

/****************************************************************************************************************************************** 
* 函数名称:	FLASH_Write()
* 功能说明:	FLASH数据写入
* 输    入: uint32_t addr		数据要写入到Flash中的地址，字对齐
*			uint32_t buff[]		要写入Flash中的数据
*			uint32_t cnt		要写的数据的个数，以字为单位，最大64
* 输    出: uint32_t			FLASH_RES_OK、FLASH_RES_TO、FLASH_RES_ERR
* 注意事项: 要写入的数据必须全部在同一页内，即addr/256 == (addr+(cnt-1)*4)/256
******************************************************************************************************************************************/
uint32_t FLASH_Write(uint32_t addr, uint32_t buff[], uint32_t cnt)
{	
	uint32_t res, i;
	
	__disable_irq();
	
	SFC->CFG |= (1 << SFC_CFG_WREN_Pos);
	for(i = 0; i < cnt; i++)
		*((volatile unsigned int *)(0x41000000+addr+i*4)) = buff[i];
	
	while(SFC->SR & SFC_SR_BUSY_Msk) __NOP();
	SFC->CFG &= ~SFC_CFG_WREN_Msk;
	
	if(SFC->IF & SFC_IF_ERR_Msk)     res = FLASH_RES_ERR;
	else if(SFC->IF & SFC_IF_TO_Msk) res = FLASH_RES_TO;
	else                             res = FLASH_RES_OK;
	
	__enable_irq();
	
	return res;
}

/****************************************************************************************************************************************** 
* 函数名称:	FLASH_Read()
* 功能说明:	FLASH数据读取
* 输    入: uint32_t addr		要读取的数据在Flash中的地址，字对齐
*			uint32_t buff[]		读取到的数据存入buff指向的内存
*			uint32_t cnt		要读取的数据的个数，以字为单位
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void FLASH_Read(uint32_t addr, uint32_t buff[], uint32_t cnt)
{	
	uint32_t i;
	
	for(i = 0; i < cnt; i++)
		buff[i] = *((volatile unsigned int *)(0x41000000+addr+i*4));
}
