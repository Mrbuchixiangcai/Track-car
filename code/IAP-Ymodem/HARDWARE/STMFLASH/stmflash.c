#include "stmflash.h"
#include "delay.h"
#include "usart.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//STM32内部FLASH读写 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#ifdef _YMODEM
  //读取指定地址的字(32位数据) 
  //faddr:读地址 
  //返回值:对应数据.
  u32 STMFLASH_ReadWord(u32 faddr)
  {
    return *(vu32*)faddr; 
  }
  //获取某个地址所在的flash扇区
  //addr:flash地址
  //返回值:0~11,即addr所在的扇区
  uint16_t STMFLASH_GetFlashSector(u32 addr)
  {
    if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
    else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
    else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
    else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
    else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
    else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
    else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
    else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
    else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
    else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
    else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
    return FLASH_Sector_11;	
  }
  //擦除StartAddr到EndAddr地址上的扇区
  //StartAddr -- 起始地址
  //EndAddr   -- 结束地址
  u32 STMFLASH_EraseSectorToAddr(u32 StartAddr, u32 DatSize)
  {
    u32 endaddr=0;
    FLASH_Status status = FLASH_COMPLETE;

    if((StartAddr<STM32_FLASH_BASE) || (StartAddr%4))  
    {
      printf("擦除的是非法地址\r\n");
      return 2;	//非法地址
    }
    FLASH_Unlock();									//解锁 
    FLASH_DataCacheCmd(DISABLE);    //FLASH擦除期间,必须禁止数据缓存
    endaddr=StartAddr+DatSize;      //起始地址+文件大小=结束地址
    
    if(StartAddr<0X1FFF0000)			  //只有主存储区,才需要执行擦除操作!!
    {
      while(StartAddr<endaddr)		  //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
      {
        if(STMFLASH_ReadWord(StartAddr)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
        {   
          printf("擦除指定地址到指定地址的扇区\r\n");
          //擦除StartAddr所在扇区 -- VCC=2.7~3.6V之间!!
          status=FLASH_EraseSector(STMFLASH_GetFlashSector(StartAddr),VoltageRange_3);
          if(status!=FLASH_COMPLETE)
          {
            printf("擦除出现错误,\r\n");
            FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
            FLASH_Lock(); //上锁   
            return 1;	    //发生错误了
          }
        }
        else StartAddr+=4;
      } 
    }
    
    FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
    FLASH_Lock();//上锁
    return 0;
  }
  //从指定地址开始写入指定长度的数据
  //特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
  //         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
  //         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
  //         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
  //该函数对OTP区域也有效!可以用来写OTP区!
  //OTP区域地址范围:0X1FFF7800~0X1FFF7A0F
  //WriteAddr:起始地址(此地址必须为4的倍数!!)
  //pBuffer:数据指针
  //NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
  u32 STMFLASH_Write(u32 *WriteAddr,u32 *pBuffer,u32 NumToWrite)	
  { 
    u32 endaddr=0;	
    
    if((*WriteAddr<STM32_FLASH_BASE) || (*WriteAddr%4))  
    {
      printf("擦除的是非法地址\r\n");
      return 2;	//非法地址
    }
    FLASH_Unlock();							 //解锁 
    FLASH_DataCacheCmd(DISABLE); //FLASH擦除期间,必须禁止数据缓存   
    endaddr=*WriteAddr+NumToWrite*4;	//写入的结束地址

    while(*WriteAddr <endaddr)//写数据
    {
      if(FLASH_ProgramWord(*WriteAddr,*pBuffer)!=FLASH_COMPLETE)//写入数据
      { 
        printf("写入异常\r\n");
        return 1;	//写入异常
      }
      *WriteAddr+=4;
      pBuffer++;
    }
    
    FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
    FLASH_Lock();//上锁
    return 0;
  }

#else 
  //读取指定地址的字(32位数据) 
  //faddr:读地址 
  //返回值:对应数据.
  u32 STMFLASH_ReadWord(u32 faddr)
  {
    return *(vu32*)faddr; 
  }  
  //获取某个地址所在的flash扇区
  //addr:flash地址
  //返回值:0~11,即addr所在的扇区
  uint16_t STMFLASH_GetFlashSector(u32 addr)
  {
    if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
    else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
    else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
    else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
    else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
    else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
    else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
    else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
    else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
    else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
    else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
    return FLASH_Sector_11;	
  }
  //从指定地址开始写入指定长度的数据
  //特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
  //         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
  //         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
  //         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
  //该函数对OTP区域也有效!可以用来写OTP区!
  //OTP区域地址范围:0X1FFF7800~0X1FFF7A0F
  //WriteAddr:起始地址(此地址必须为4的倍数!!)
  //pBuffer:数据指针
  //NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
  void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
  { 
    FLASH_Status status = FLASH_COMPLETE;
    u32 addrx=0;
    u32 endaddr=0;	
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
    FLASH_Unlock();									//解锁 
    FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
      
    addrx=WriteAddr;				//写入的起始地址
    endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址
    if(addrx<0X1FFF0000)			//只有主存储区,才需要执行擦除操作!!
    {
      while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
      {
        if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
        {   
          status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!
          if(status!=FLASH_COMPLETE)break;	//发生错误了
        }else addrx+=4;
      } 
    }
    if(status==FLASH_COMPLETE)
    {
      while(WriteAddr<endaddr)//写数据
      {
        if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//写入数据
        { 
          break;	//写入异常
        }
        WriteAddr+=4;
        pBuffer++;
      } 
    }
    FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
    FLASH_Lock();//上锁
  }
  //从指定地址开始读出指定长度的数据
  //ReadAddr:起始地址
  //pBuffer:数据指针
  //NumToRead:字(4位)数
  void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
  {
    u32 i;
    for(i=0;i<NumToRead;i++)
    {
      pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
      ReadAddr+=4;//偏移4个字节.	
    }
  }
#endif


