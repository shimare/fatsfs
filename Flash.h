/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : fsmc_nand.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Header for fsmc_nand.c file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FSMC_NAND_H
#define __FSMC_NAND_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define BUFFER_SIZE			STM_FLASH_PAGE_SIZE

/* STM Flash memory address of physics */
#define STM32_FLASH_ADDR		((u32)0x08000000)
#define STM32_PAGE_SIZE			((u32)0x0800)
#define STM32_START_PAGE		((u32)150)
#define STM32_END_PAGE			((u32)255)

/* STM Flash  memory parameters */
#define STM_FLASH_PAGE_SIZE		((u16)0x0200)			/* 512 bytes per page w/o Spare Area */
#define STM_FLASH_BLOCK_SIZE		((u16)0x0004)			/* 4x512 bytes pages per block */
#define STM_FLASH_ZONE_SIZE		((u16)(STM32_END_PAGE-STM32_START_PAGE))	/* 256-200=56 */
#define STM_FLASH_MAX_ZONE		((u16)0x0001)			/* 1 zones of 1024 block */

/* NAND memory status */
#define STM_FLASH_VALID_ADDRESS		((u32)0x00010000)
#define STM_FLASH_INVALID_ADDRESS	((u32)0x00020000)
#define STM_FLASH_ERROR			((u32)0x00040000)
#define STM_FLASH_SUCCESS		((u32)0x00080000)

/* STM32 Flash memory address computation */
//#define ADDR_1st_CYCLE(ADDR)		(u8)((ADDR)& 0xFF)		/* 1st addressing cycle */
//#define ADDR_2nd_CYCLE(ADDR)		(u8)(((ADDR)& 0xFF00) >> 8)	/* 2nd addressing cycle */
//#define ADDR_3rd_CYCLE(ADDR)		(u8)(((ADDR)& 0xFF0000) >> 16)	/* 3rd addressing cycle */
//#define ADDR_4th_CYCLE(ADDR)		(u8)(((ADDR)& 0xFF000000) >> 24)/* 4th addressing cycle */

/* STM32 Flash memory data computation */
#define DATA_1st_CYCLE(DATA)       (u8)((DATA)& 0xFF)			/* 1st data cycle */
#define DATA_2nd_CYCLE(DATA)       (u8)(((DATA)& 0xFF00) >> 8)		/* 2nd data cycle */
#define DATA_3rd_CYCLE(DATA)       (u8)(((DATA)& 0xFF0000) >> 16)	/* 3rd data cycle */
#define DATA_4th_CYCLE(DATA)       (u8)(((DATA)& 0xFF000000) >> 24)	/* 4th data cycle */


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u32 STM_Flash_WritePage(u8 *pBuffer, u32 Address, u32 NumPageToWrite);
u32 STM_Flash_ReadPage (u8 *pBuffer, u32 Address, u32 NumPageToRead);
u32 STM_Flash_EraseBlock(u32 Address);

//u32 STM_Flash_MoveSmallPage(u32 SourcePageAddress, u32 TargetPageAddress);

//u32 STM_Flash_GetStatus(void);
//u32 STM_Flash_ReadStatus(void);
//u32 STM_Flash_AddressIncrement(u32* Address);

void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset);

#endif /* __FSMC_NAND_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
