/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : fsmc_nand.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : This file provides a set of functions needed to drive the
*                      NAND512W3A2 memory mounted on STM3210E-EVAL board.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define FSMC_Bank_NAND     FSMC_Bank2_NAND
#define STM_FLASH_ADDR     Bank2_NAND_ADDR 
#define Bank2_NAND_ADDR    ((u32)0x70000000)

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************
* Function Name  : STM_Flash_WritePage
* Description    : This routine is for writing one or several 512 Bytes Page size.
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
u32 STM_Flash_WritePage(u8 *pBuffer, u32 PageNum, u32 NumPageToWrite)
{
  FLASH_Status status = FLASH_COMPLETE;
  
  u32 STMFlashStatus = STM_FLASH_SUCCESS;
  u32 Address;
  u32 index, data, data32;
  
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();
  /* Clear All pending flags */
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
  
  
  while( (NumPageToWrite != 0x00) && (STMFlashStatus != STM_FLASH_INVALID_ADDRESS) )
  {
    Address = STM32_FLASH_ADDR + (STM32_PAGE_SIZE * STM32_START_PAGE)
		+ (STM32_PAGE_SIZE / STM_FLASH_BLOCK_SIZE) * PageNum;
    
    for(index=0; index < STM_FLASH_PAGE_SIZE; index++)
    {
      data32 = *pBuffer++;
      
      switch(index%4)
      {
      case 0:
	data  = ( data32 << 0 ) & 0x000000FF;
	break;
      case 1:
	data += ( data32 << 8 ) & 0x0000FF00;
	break;
      case 2:
	data += ( data32 << 16) & 0x00FF0000;
	break;
      case 3:
	data += ( data32 << 24) & 0xFF000000;
	
	status = FLASH_ProgramWord(Address, data);
	Address += 4;
	
	if(status != FLASH_COMPLETE)
	{
	  FLASH_Lock();
	  return STM_FLASH_ERROR;
	}
	
	break;
      default:
	break;
      }
    }
    
    NumPageToWrite--;
    if(NumPageToWrite != 0)
    {
      PageNum++;
      
      /* Calculate Next page Address */
      if(PageNum >= (u32)(STM_FLASH_MAX_ZONE*STM_FLASH_ZONE_SIZE*STM_FLASH_BLOCK_SIZE))
	STMFlashStatus = STM_FLASH_INVALID_ADDRESS;
    }
  }
  
  
  FLASH_Lock();
  
  return STMFlashStatus;
}

/******************************************************************************
* Function Name  : STM_Flash_ReadPage
* Description    : This routine is for sequential read from one or several
* Input          : - pBuffer: pointer on the Buffer to fill  
* Output         : None
* Return         : New status of the NAND operation. This parameter can be:
*******************************************************************************/
u32 STM_Flash_ReadPage(u8 *pBuffer, u32 PageNum, u32 NumPageToRead)
{
  u32 STMFlashStatus = STM_FLASH_SUCCESS;
  u32 Address;
  u32 index, data;
  
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();
  /* Clear All pending flags */
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
  
  
  while( (NumPageToRead != 0x00) && (STMFlashStatus != STM_FLASH_INVALID_ADDRESS) )
  {
    Address = STM32_FLASH_ADDR + (STM32_PAGE_SIZE * STM32_START_PAGE)
		+ (STM32_PAGE_SIZE / STM_FLASH_BLOCK_SIZE) * PageNum;
    
    for(index=0; index < STM_FLASH_PAGE_SIZE; index++)
    {
      switch(index%4)
      {
      case 0:
	data = *(u32*)Address;
	Address += 4;
	
	*pBuffer++ = DATA_1st_CYCLE(data);
	break;
      case 1:
	*pBuffer++ = DATA_2nd_CYCLE(data);
	break;
      case 2:
	*pBuffer++ = DATA_3rd_CYCLE(data);
	break;
      case 3:
	*pBuffer++ = DATA_4th_CYCLE(data);
	break;
      default:
	break;
      }
    }
    
    NumPageToRead--;
    if(NumPageToRead != 0)
    {
      PageNum++;
      
      /* Calculate Next page Address */
      if(PageNum >= (u32)(STM_FLASH_MAX_ZONE*STM_FLASH_ZONE_SIZE*STM_FLASH_BLOCK_SIZE))
	STMFlashStatus = STM_FLASH_INVALID_ADDRESS;
    }
  }
  
  FLASH_Lock();
  
  return STMFlashStatus;
}

/******************************************************************************
* Function Name  : FSMC_NAND_EraseBlock
*******************************************************************************/
u32 STM_Flash_EraseBlock(u32 BlockNum)
{
  FLASH_Status status = FLASH_COMPLETE;
  u32 Address;
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();
  /* Clear All pending flags */
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
  
  Address = STM32_FLASH_ADDR + STM32_PAGE_SIZE * (STM32_START_PAGE + BlockNum);
  
  status = FLASH_ErasePage(Address);
  
  FLASH_Lock();
  
  if(status == FLASH_COMPLETE)
    return STM_FLASH_SUCCESS;
  else
    return STM_FLASH_ERROR;
}



/*******************************************************************************
* Function name : Fill_Buffer
* Description   : Fill the buffer
* Input         : - pBuffer: pointer on the Buffer to fill
*                 - BufferSize: size of the buffer to fill
*                 - Offset: first value to fill on the Buffer
* Output param  : None
*******************************************************************************/
void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset)
{
  u16 IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
