 /**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ���� 8M����flash���ԣ�����������Ϣͨ������1�ڵ��Եĳ����ն��д�ӡ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_spi_flash.h"
#include "bsp_lcd.h"
#include "bsp_led.h"


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;


/* ��ȡ�������ĳ��� */
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
     

/* ���ͻ�������ʼ�� */
uint8_t Tx_Buffer[] = " ��л��ѡ��Ұ��stm32������\r\n                http://firestm32.taobao.com";
//uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

extern uint8_t SPI1_Buffer_Rx[100];

extern uint8_t tempValue;
// ����ԭ������
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
int main(void)
{ 	
	
	char * bbs;
	
	uint8_t i;
	/* ���ô���1Ϊ��115200 8-N-1 */
	USARTx_Config();
	
	LED_GPIO_Config();
	
	/* 8M����flash W25Q64��ʼ�� */
	SPI_FLASH_Init();
	NVIC_Configuration();
	
	LCD_Init ();         //LCD ��ʼ��
	

	/* ͨ���޸�GRAM��ɨ�跽��Ȼ��ͨ����Һ��������ɫ�飬�Ϳ���֪��GRAM��ʵ��ɨ�跽���� */
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
//		printf("\r\n ����һ��SPI_SLAVEʵ�� \r\n");
//		printf("\r\n ���ճɹ� ,%x\r\n",SPI_FLASH_ReadByte());
//		Delay(0xeeff0);
//		SPI_FLASH_SendByte(0x55);
//		printf("\r\n �������ݳɹ���0x55 \r\n");
		
		//bbs = (char * )"abck";
	//	printf("\r\n ���ճɹ� ,%x\r\n",tempValue);
		
		
//		SPI_FLASH_SendByte(0x55);
		//ILI9341_DispChar_EN ( 80, 80, bbs, macBACKGROUND, macRED );
//		ILI9341_DispString_EN ( 100, 100, bbs, macBACKGROUND, macRED );
		Delay(0xeeff0);
//		LED3_TOGGLE
	};  
}

/*
 * ��������Buffercmp
 * ����  ���Ƚ������������е������Ƿ����
 * ����  ��-pBuffer1     src������ָ��
 *         -pBuffer2     dst������ָ��
 *         -BufferLength ����������
 * ���  ����
 * ����  ��-PASSED pBuffer1 ����   pBuffer2
 *         -FAILED pBuffer1 ��ͬ�� pBuffer2
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

//	//ֻ���գ������ͣ�����ط����ص㣬�������Ϊ����ͨ���ǲ���ʵ�����ݲɼ��ġ�
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_RxOnly;

//	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;//�ӻ�ģʽ

//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//����λΪ8

//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����������ʱ��ʱ����Ϊ��

//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//�ڵڶ����ؽ��в���

//	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;			//ӲNSS

//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//MSB����

//	SPI_InitStructure.SPI_CRCPolynomial = 7;

//	SPI_Init(SPI2,&SPI_InitStructure);

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//ʹ���жϽ��н��գ��������NVIC�����ȼ��飬1��ʾ1bit��ռ���ȼ�

//	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;

//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

//	NVIC_Init(&NVIC_InitStructure);

//	/**Enable SPI2 RXNE interrupt*/
//	SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_RXNE,ENABLE);

//	//Enable SPI2

//	//SPI_Cmd(SPI2,ENABLE);//�Ȳ�����SPI,���յ��ض������ʱ��������
//}

//void SPI2_IRQHandler(void)
//{
//	//��������
//	//printf("SPI_IRQ %d\n",Rxldx);
//	SPI2_Buffer_Rx[Rxldx++] = SPI_I2S_ReceiveData(SPI2);
//}

//int main(void)
//{
//	int i;

//	SystemInit();/*����ϵͳʱ��Ϊ72M*/

//	LEDKEY_GPIOInit();

//	USART_GPIOInit();

//	SysTick_Configuration();

//	USART_Configuration(USART1,9600);

//	SPI_slave();

//	LED1_ON;

//	while(1)

//	{
//		if(Rxldx == 50)		//�����������������ݽ��д���
//		{
//			SPI_Cmd(SPI2,DISABLE);

//			Rxldx = 0;

//			printf("rcv full \n");

//			for(i=0;i<49;i++)
//				printf("0x%02X\n",SPI2_Buffer_Rx[i]);//���������ȡ������

//		}

//		if(GetKey() ==1)
//		{
//			LED1_ON;

//			SPI_Cmd(SPI2,ENABLE);//�������º�ʹ��SPI2,Ȼ�����ж��н�������

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
