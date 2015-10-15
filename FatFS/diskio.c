/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "includes.h"

static u8 BackUpData[STM_FLASH_BLOCK_SIZE][STM_FLASH_PAGE_SIZE];		//BackUpData[4][512];

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	return RES_OK;		//初始化成功
}


/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/* The disk_read function reads sector(s) from the disk drive.		 */

DRESULT disk_read (
  BYTE  Drive,			/* Physical drive number (0..9)		*/
  BYTE* Buffer,			/* Pointer to the read data buffer	*/
  DWORD SectorNumber,		/* Specifies the start sector number in logical block address (LBA).	*/
  BYTE  SectorCount		/* Specifies number of sectors to read. The value can be 1 to 255.	*/
)
{
  u32 result;
  
  if (Drive || !SectorCount)return RES_PARERR;
  
  result = STM_Flash_ReadPage(Buffer, SectorNumber, SectorCount);
  
  if(result == STM_FLASH_SUCCESS)return RES_OK;
  else return RES_ERROR;
}


/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/* The disk_write writes sector(s) to the disk.				 */

#if _READONLY == 0
DRESULT disk_write (
  BYTE  Drive,		/* Specifies the physical drive number(0..9)		*/
  const BYTE* Buffer,	/* Pointer to the byte array to be written.		*/
  DWORD SectorNumber,	/* Specifies the start sector number in logical block address (LBA).	*/
  BYTE  SectorCount	/* Specifies the number of sectors to write. The value can be 1 to 255.	*/
)
{
  //u32 result;
  u32 WriteBlockNum;
  u16 IndexTmp, OffsetPage;
  
  if(Drive || !SectorCount)return RES_PARERR;
  
  //块码: 写数据所在块号码
  WriteBlockNum = SectorNumber/STM_FLASH_BLOCK_SIZE;
  //页码: 写数据在块中(偏移)页码
  OffsetPage = SectorNumber%STM_FLASH_BLOCK_SIZE;
  //将块数据拷贝到数组 BackUpData[4][512] 中:
  STM_Flash_ReadPage((u8*)(&BackUpData[0][0]), WriteBlockNum*STM_FLASH_BLOCK_SIZE, STM_FLASH_BLOCK_SIZE);
  
  //擦除写页所在块:
  STM_Flash_EraseBlock(WriteBlockNum);
  
  // return write the block  with modify //
  for (IndexTmp = 0; IndexTmp < STM_FLASH_BLOCK_SIZE; IndexTmp++ )
  {
    if((IndexTmp>=OffsetPage)&&(IndexTmp < (OffsetPage+SectorCount)))
    {
      STM_Flash_WritePage((u8 *)Buffer, WriteBlockNum*STM_FLASH_BLOCK_SIZE+IndexTmp, 1);
      Buffer = (u8 *)Buffer + STM_FLASH_PAGE_SIZE;
    }
    else
    {
      //将数组 BackUpData[4][512] 中块数据回写到STM Flash中:
      STM_Flash_WritePage((u8 *)(&BackUpData[IndexTmp][0]), WriteBlockNum*STM_FLASH_BLOCK_SIZE+IndexTmp, 1);
    }
  }
  
  //if(result == NAND_READY)return RES_OK;
  //else return RES_ERROR;
  
  return RES_OK;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
  BYTE Drive,		/* Drive number */
  BYTE Command,		/* Control command code */
  void* Buffer		/* Parameter and data buffer */
)
{
  DRESULT res = RES_OK;
  
  if(Drive)res = RES_PARERR;
  
  switch(Command)
  {
  case CTRL_SYNC:
    
    break;
  
  case GET_SECTOR_SIZE:
    
    *(WORD*)Buffer = STM_FLASH_PAGE_SIZE;
    break;
  
  case GET_SECTOR_COUNT:
    
    *(DWORD*)Buffer = STM_FLASH_MAX_ZONE * STM_FLASH_ZONE_SIZE * STM_FLASH_BLOCK_SIZE;
    break;
  
  case GET_BLOCK_SIZE:
    
    *(DWORD*)Buffer = STM_FLASH_BLOCK_SIZE;
    break;
  
  case CTRL_ERASE_SECTOR:
    
    break;
  
  default:
    
    res = RES_PARERR;
    break;
  }
  
  return res;
}

/*-----------------------------------------------------------------------*/
/* User defined function to give a current time to fatfs module          */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{
  return 0;
}
							




