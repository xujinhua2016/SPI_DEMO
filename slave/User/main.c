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
#include "bsp_spi_flash.h"
#include "bsp_lcd.h"
#include "bsp_led.h"


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;


/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
//#define  sFLASH_ID              0xEF3015     //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
#define  sFLASH_ID              0XEF4017    //W25Q64
     

/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = " 感谢您选用野火stm32开发板\r\n                http://firestm32.taobao.com";
//uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

extern uint8_t SPI1_Buffer_Rx[100];

extern uint8_t tempValue;
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
	
	char * bbs;
	
	uint8_t i;
	/* 配置串口1为：115200 8-N-1 */
	USARTx_Config();
	
	LED_GPIO_Config();
	
	/* 8M串行flash W25Q64初始化 */
	SPI_FLASH_Init();
	NVIC_Configuration();
	
	LCD_Init ();         //LCD 初始化
	

	/* 通过修改GRAM的扫描方向，然后通过看液晶清屏的色块，就可以知道GRAM的实际扫描方向了 */
  ILI9341_GramScan ( 1 );
	
  ILI9341_Clear ( 0, 0, 240, 320, macBACKGROUND);
	
  ILI9341_DispString_EN ( 0, 10, "this is slave program", macBACKGROUND, macRED );
	
	ILI9341_DispChar_EN ( 60, 60, 'b', macBACKGROUND, macRED );
	
	
	while(1)
	{
		
		//SPI_FLASH_SendByte(0xbb);
		
		//for(i=0;i<20;i++)
//		SPI_FLASH_SendByte(0x55);
//		printf("%x",SPI_FLASH_ReadByte());
//		printf("\r\n 这是一个SPI_SLAVE实验 \r\n");
//		printf("\r\n 接收成功 ,%x\r\n",SPI_FLASH_ReadByte());
//		Delay(0xeeff0);
//		SPI_FLASH_SendByte(0x55);
//		printf("\r\n 发送数据成功，0x55 \r\n");
		
		//bbs = (char * )"abck";
	//	printf("\r\n 接收成功 ,%x\r\n",tempValue);
		
		
//		SPI_FLASH_SendByte(0x55);
		//ILI9341_DispChar_EN ( 80, 80, bbs, macBACKGROUND, macRED );
//		ILI9341_DispString_EN ( 100, 100, bbs, macBACKGROUND, macRED );
		Delay(0xeeff0);
//		LED3_TOGGLE
	};  
}

/*
 * 函数名：Buffercmp
 * 描述  ：比较两个缓冲区中的数据是否相等
 * 输入  ：-pBuffer1     src缓冲区指针
 *         -pBuffer2     dst缓冲区指针
 *         -BufferLength 缓冲区长度
 * 输出  ：无
 * 返回  ：-PASSED pBuffer1 等于   pBuffer2
 *         -FAILED pBuffer1 不同于 pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

//void SPI_slave(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	SPI_InitTypeDef SPI_InitStructure;

//	NVIC_InitTypeDef NVIC_InitStructure;

//	//Enable SPI2 clock and GPIO clock for SPI2 and SPI

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

//	//IO Initial
//	//Configure SPI2 pins:SCK,MISO and MOSI

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;

//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

//	GPIO_Init(GPIOB,&GPIO_InitStructure);

//	//1st phase:SPI2 slave

//	//SPI1 Config

//	//只接收，不发送，这个地方是重点，如果设置为单工通信是不能实现数据采集的。
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_RxOnly;

//	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;//从机模式

//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//数据位为8

//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//不发送数据时，时钟线为高

//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//在第二个沿进行采样

//	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;			//硬NSS

//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//MSB优先

//	SPI_InitStructure.SPI_CRCPolynomial = 7;

//	SPI_Init(SPI2,&SPI_InitStructure);

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//使用中断进行接收，因此设置NVIC的优先级组，1表示1bit抢占优先级

//	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;

//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

//	NVIC_Init(&NVIC_InitStructure);

//	/**Enable SPI2 RXNE interrupt*/
//	SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_RXNE,ENABLE);

//	//Enable SPI2

//	//SPI_Cmd(SPI2,ENABLE);//先不启动SPI,在收到特定命令的时候再启动
//}

//void SPI2_IRQHandler(void)
//{
//	//接收数据
//	//printf("SPI_IRQ %d\n",Rxldx);
//	SPI2_Buffer_Rx[Rxldx++] = SPI_I2S_ReceiveData(SPI2);
//}

//int main(void)
//{
//	int i;

//	SystemInit();/*配置系统时钟为72M*/

//	LEDKEY_GPIOInit();

//	USART_GPIOInit();

//	SysTick_Configuration();

//	USART_Configuration(USART1,9600);

//	SPI_slave();

//	LED1_ON;

//	while(1)

//	{
//		if(Rxldx == 50)		//接收数据满，对数据进行处理
//		{
//			SPI_Cmd(SPI2,DISABLE);

//			Rxldx = 0;

//			printf("rcv full \n");

//			for(i=0;i<49;i++)
//				printf("0x%02X\n",SPI2_Buffer_Rx[i]);//串口输出获取的数据

//		}

//		if(GetKey() ==1)
//		{
//			LED1_ON;

//			SPI_Cmd(SPI2,ENABLE);//按键按下后，使能SPI2,然后在中断中接收数据

//			Rxldx = 0;

//			printf("key1\n");
//		}

//		if (GetKey() == 2)
//		{
//			/* code */
//			LED1_OFF;
//		}
//	}
//}
