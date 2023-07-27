/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * @file    user_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
 /* USER CODE END Header */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/*
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future.
 * Kept to ensure backward compatibility with previous CubeMx versions when
 * migrating projects.
 * User code previously added there should be copied in the new user sections before
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"
#include "w25qxx.h"
#include "usart_cli.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define PAGE_SIZE       256
#define SECTOR_SIZE     4096
#define SECTOR_COUNT	2048
#define BLOCK_SIZE		65536
#define BLOCK_COUNT		128
#define FLASH_PAGES_PER_SECTOR	SECTOR_SIZE/PAGE_SIZE

/* Private variables ---------------------------------------------------------*/
FATFS fs;
FIL file;						  	/* 文件对象 */
FRESULT f_res;                    	/* 文件操作结果 */
UINT fnum;            				/* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        	/* 读缓冲区 */
BYTE WriteBuffer[]= "WWB is the high hand\n";
BYTE work[4096];
char USER_Path[4];

/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

void mount_disk(void)
{
   f_res = f_mount(&fs, USER_Path, 0);
	if (f_res != 0) {
		SHELL_DEBUG("Mount Disk Failed!");
	} else {
		SHELL_DEBUG("Mount Disk Success!");
	}
   return;

}

void format_disk(void)
{
  uint8_t res = 0;
  //这里根据版本不同函数输入参数不一样
  f_res = f_mkfs(USER_Path, 1, 4096, work, sizeof(work));
	if (f_res != 0) {
		SHELL_DEBUG("Format File System Failed!");
	} else {
		SHELL_DEBUG("Format File System Success!");
	}
}

 UINT bw;
void create_file(void)
{
  FIL file;
  FIL *pf = &file;
  uint8_t res;
  f_res = f_open(pf, "0:/test.txt", FA_OPEN_ALWAYS | FA_WRITE);
	if (f_res != 0) {
		SHELL_DEBUG("File Open Failed!");
		return;
	}
  f_res = f_write(&file, WriteBuffer, sizeof(WriteBuffer), &bw);
	if (f_res != 0) {
		SHELL_DEBUG("File Write Failed!");
	}
  f_res = f_close(pf);
	if (f_res != 0) {
		SHELL_DEBUG("File Close Failed!");
	}
}

void read_file(void)
{
  FIL file;
  FRESULT res;
  uint8_t rbuf[100] = {0};
  f_res = f_open(&file, "0:/test.txt", FA_READ);
	if (f_res != 0) {
		SHELL_DEBUG("File Open Failed!");
		return;
	}
  f_res = f_read(&file, ReadBuffer, sizeof(WriteBuffer), &bw);
	if (f_res != 0) {
		SHELL_DEBUG("File Read Failed!");
		return;
	} else {
		SHELL_DEBUG("read: %s", ReadBuffer);
	}
  f_res = f_close(&file);
	if (f_res != 0) {
		SHELL_DEBUG("File Close Failed!");
	}
}

void FileTest(void)
{
//	mount_disk();		//文件系统注册
//	format_disk();		//格式化文件系统
//	create_file();		//建立文件并写入"WWB is the high hand\n"
//	read_file();		//读取文件放到ReadBuffer中
	//FATFS fs;           /* Filesystem object */
    //FIL fil;            /* File object */
    FRESULT res;  /* API result code */
    //UINT bw;            /* Bytes written */
	//#define FF_MAX_SS 4096
    //BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
	BYTE mm[50];
	UINT i;
	
	SHELL_DEBUG("文件系统测试开始:");
	/* 格式化文件系统 */
	res = f_mkfs("0:", FM_ANY, 0, work, sizeof work);//"0:"是卷标，来自于 #define SPI_FLASH		0
	if (res)
	{
		SHELL_DEBUG("文件系统格式化失败.");
		return ;
	}
	else
	{
		SHELL_DEBUG("文件系统格式化成功.");
	}
	/* 挂载文件系统 */
	res = f_mount(&fs, "0:", 0);
	if (res)
	{
		SHELL_DEBUG("文件系统挂载失败.");
	}
	else
	{
		SHELL_DEBUG("文件系统挂载成功.");
	}
	/* Create a file as new */
	res = f_open(&file, "0:/test.txt", FA_CREATE_NEW|FA_WRITE|FA_READ);
	if (res)
	{
		SHELL_DEBUG("打开文件失败.");
	}
	else
	{
		SHELL_DEBUG("打开文件成功.");
	}
	/* Write a message */
	res = f_write(&file, "Hello,World!", 12, &bw);
	//uart_printf("res write:%d\r\n",res);
	if (bw == 12)
	{
		SHELL_DEBUG("写文件成功!");
	}
	else
	{
		SHELL_DEBUG("写文件失败!");
	}
	res = f_size(&file);
	SHELL_DEBUG("文件大小:%d Bytes.",res);
	memset(mm,0x0,50);
	
	//f_lseek(&file,0);
	res = f_close(&file);
	if (res) {
		SHELL_DEBUG("文件保存失败.");
	} else {
		SHELL_DEBUG("文件保存成功.");
	}
	res = f_open(&file, "0:/test.txt", FA_READ);
	if (res) {
		SHELL_DEBUG("再次打开文件失败.");
	} else {
		SHELL_DEBUG("再次打开文件成功.");
	}
	res = f_read(&file,mm,12,&i);
	if (res == FR_OK)
	{
		SHELL_DEBUG("读文件成功!");
		SHELL_DEBUG("读到数据长度:%d Bytes.",i);
	}
	else
	{
		SHELL_DEBUG("读文件失败!");
	}
	SHELL_DEBUG("读到如下数据:%s", mm);
	//buff_print((char *)mm,12);
	/* Close the file */
	f_close(&file);
	/*卸载文件系统*/
	f_mount(0, "0:", 0);
	SHELL_DEBUG("文件系统测试完毕.");
}

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,
#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
  /* USER CODE BEGIN INIT */
    Stat = STA_NOINIT;
	if((W25qxx_ReadID() & 0x000000FF) != 0)
	{
		//Stat &= ~STA_NOINIT;
		return RES_OK;
	}	
    return Stat;
  /* USER CODE END INIT */
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_status (
	BYTE pdrv       /* Physical drive number to identify the drive */
)
{
  /* USER CODE BEGIN STATUS */
    Stat &= ~STA_NOINIT;
    return Stat;
  /* USER CODE END STATUS */
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USER_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
  /* USER CODE BEGIN READ */
	DRESULT res = RES_ERROR;
	UINT i;
  
	for(i = 0;i < count;i++)
	{
		//sFLASH_ReadBuffer(buff + i * 4096,sector * 4096 + i * 4096,4096 );
		W25qxx_ReadSector(buff + i * 4096, sector + i, 0, 4096);
	}
	//W25qxx_ReadBytes(buff, sector * 4096, count * 4096);
    return RES_OK;
  /* USER CODE END READ */
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USER_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{
  /* USER CODE BEGIN WRITE */
  /* USER CODE HERE */
	DRESULT res = RES_ERROR;
	
	UINT i;
	
	//SHELL_DEBUG("flash write sector count = %d", count);
  
	for(i = 0;i < count;i++)
	{
		//SHELL_DEBUG("writing sector:%d", sector + i);
		W25qxx_EraseSector(sector + i);
		W25qxx_WriteSector((uint8_t *)(buff + SECTOR_SIZE * i), sector + i, 0, SECTOR_SIZE);
	}
	
	
	res = RES_OK;
    return res;
  /* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USER_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
  /* USER CODE BEGIN IOCTL */
    DRESULT res = RES_OK;
	switch(cmd)
	{
		case CTRL_SYNC :
			break;	
	 
		case CTRL_TRIM:
			break;
		case GET_BLOCK_SIZE:
		*(DWORD*)buff = BLOCK_SIZE; 
		break;
			
		case GET_SECTOR_SIZE:
		*(DWORD*)buff = SECTOR_SIZE;
			break;
		case GET_SECTOR_COUNT:
		*(DWORD*)buff = SECTOR_COUNT;
		break;
		default:
		res = RES_PARERR;
		break;
	  }
    return res;
  /* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */

