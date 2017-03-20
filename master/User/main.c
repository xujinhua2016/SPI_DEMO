 /**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   华邦 8M串行flash测试，并将测试信息通过串口1在电脑的超级终端中打印出来
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_spi.h"
#include "bsp_lcd.h"
#include "bsp_led.h"


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;


/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define SPIBufferTxSize (countof(SPI_Buffer_Tx) - 1)
#define SPIBufferRxSize (countof(SPI_Buffer_Rx) - 1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
//#define  sFLASH_ID              0xEF3015     //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define  sFLASH_ID              0XEF4017    //W25Q64


extern uint8_t tempValue;

/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = " 感谢您选用野火stm32开发板\r\n                http://firestm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];

uint8_t SPI_Buffer_Tx[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                           0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                           0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                           0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
                           0x1D, 0x1E, 0x1F, 0x20};
///uint8_t SPI_Buffer_Rx[SPIBufferRxSize];
																			
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// 函数原型声明
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{ 	
	/* 配置串口1为：115200 8-N-1 */
	USARTx_Config();
	//printf("\r\n 这是SPI主机master实验 \r\n");
	
	/* 8M串行flash W25Q64初始化 */
	SPI_FLASH_Init();
//	NVIC_Configuration();
	
	LED_GPIO_Config();
	
	LCD_Init ();         //LCD 初始化
	
	ILI9341_GramScan ( 1 );
	
  ILI9341_Clear ( 0, 0, 240, 320, macBACKGROUND);
	
  ILI9341_DispString_EN ( 0, 10, "this is master program", macBACKGROUND, macRED );
	
	ILI9341_DispChar_EN ( 60, 60, 'A', macBACKGROUND, macRED );
	
	SPI_FLASH_SendByte(0xaa);
	
	while(1)
	{
		
		macSPI_FLASH_CS_ENABLE();
		
		SPI_FLASH_SendByte(0xaa);

	//	Delay(0xffff0);
		
		if((SPI_FLASH_ReadByte() == 0xdd) || (SPI_FLASH_ReadByte() == 0xaa))
		{
			LED1_TOGGLE;
		}
		
		macSPI_FLASH_CS_DISABLE();
		
		Delay(0xffff0);
		
		
		//printf("\r\n接收到数据,%x",SPI_FLASH_ReadByte());
		
	}		
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
