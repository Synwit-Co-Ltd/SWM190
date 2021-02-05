#ifndef __SWM190_FLASH_H__
#define __SWM190_FLASH_H__


void FLASH_Init(void);
uint32_t FLASH_Erase(uint32_t addr);
uint32_t FLASH_Write(uint32_t addr, uint32_t buff[], uint32_t cnt);
void FLASH_Read(uint32_t addr, uint32_t buff[], uint32_t cnt);


#define FLASH_RES_OK	0
#define FLASH_RES_TO	1	//Timeout
#define FLASH_RES_ERR	2


#define FLASH_CMD_READ_Identi
#define FLASH_CMD_READ_ID
#define FLASH_CMD_READ_STATL
#define FLASH_CMD_READ_STATH
#define FLASH_CMD_READ_DATA
#define FLASH_CMD_WRITE_STAT
#define FLASH_CMD_WRITE_PAGE
#define FLASH_CMD_ERASE_SECT		0x20
#define FLASH_CMD_ERASE_BLK32K
#define FLASH_CMD_ERASE_BLK64K
#define FLASH_CMD_ERASE_CHIP
#define FLASH_CMD_WRITE_SUSP
#define FLASH_CMD_WRITE_RESUME
#define FLASH_CMD_SLEEP
#define FLASH_CMD_WAKEUP
#define FLASH_CMD_WRITE_EN
#define FLASH_CMD_WRITE_DIS
#define FLASH_CMD_WRITE_SR_EN


#endif //__SWM190_FLASH_H__
