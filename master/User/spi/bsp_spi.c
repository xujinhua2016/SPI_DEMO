 /**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   spi flash 底层应用函数bsp 
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_spi.h"
#include "bsp_led.h"

#define Dummy_Byte                0xFF

uint8_t SPI1_Buffer_Rx[100];

uint8_t tempValue;

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
  
  /* Enable SPI1 and GPIO clocks */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO 
       and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
  /*!< SPI_FLASH_SPI Periph clock enable */
	macSPI_APBxClock_FUN ( macSPI_CLK, ENABLE );
 
  /*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
	macSPI_CS_APBxClock_FUN ( macSPI_CS_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = macSPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(macSPI_CS_PORT, &GPIO_InitStructure);
	
  /*!< Configure SPI_FLASH_SPI pins: SCK */
	macSPI_SCK_APBxClock_FUN ( macSPI_SCK_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = macSPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(macSPI_SCK_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MISO */
	macSPI_MISO_APBxClock_FUN ( macSPI_MISO_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = macSPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(macSPI_MISO_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MOSI */
	macSPI_MOSI_APBxClock_FUN ( macSPI_MOSI_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = macSPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(macSPI_MOSI_PORT, &GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */
  macSPI_FLASH_CS_DISABLE();

  /* SPI1 configuration */
  // W25X16: data input on the DIO pin is sampled on the rising edge of the CLK. 
  // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;     //SPI1是72M,选择128分频，为560k
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(macSPIx , &SPI_InitStructure);
	
//	SPI_I2S_ITConfig(macSPIx, SPI_I2S_IT_RXNE, ENABLE);			//使能接受中断

  /* Enable SPI1  */
  SPI_Cmd(macSPIx , ENABLE);
	
}

//88*****************************************************88
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//外设中断
	NVIC_InitStructure.NVIC_IRQChannel = macSPIx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
u8 SPI_FLASH_ReadByte(void)
{
	uint8_t i;
	
	
 // return (SPI_FLASH_SendByte(Dummy_Byte));
	/* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(macSPIx , SPI_I2S_FLAG_RXNE) == RESET);
//	{
//		i++;
//		if(i > 200) return 0;
//	}

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(macSPIx );
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_FLASH_SendByte(u8 byte)
{
	//发送数据时，将片选引脚拉低
//	macSPI_FLASH_CS_ENABLE();
	
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(macSPIx , SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(macSPIx , byte);

//  /* Wait to receive a byte */
//  while (SPI_I2S_GetFlagStatus(macSPIx , SPI_I2S_FLAG_RXNE) == RESET);
//	
//	macSPI_FLASH_CS_DISABLE();

//  /* Return the byte read from the SPI bus */
//  return SPI_I2S_ReceiveData(macSPIx );
//	

	
	return 0;
}


//中断服务程序
void SPI1_IRQHandler(void)
{
//	uint8_t i = 0;
//	
//	if(i == 99)
//	{
//		i =  0;
//	}
//	else
//	{
//		SPI1_Buffer_Rx[i++] = SPI_I2S_ReceiveData(macSPIx);
//	}
	
	//if(SPI_I2S_GetFlagStatus(macSPIx , SPI_I2S_FLAG_RXNE) != RESET)
	if(SPI_I2S_GetITStatus(macSPIx , SPI_I2S_FLAG_RXNE))
	{
		LED1_TOGGLE;
		tempValue = SPI_I2S_ReceiveData(macSPIx );
		
		SPI_FLASH_SendByte(0xaa);
		
		//SPI_I2S_ClearFlag(macSPIx , SPI_I2S_FLAG_RXNE|SPI_I2S_FLAG_TXE);
		
		//Delay(0xeeff0);
	}
	else
		;
	
//	 SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_IT_TXE|SPI_I2S_IT_RXNE);
	
//		LED1_TOGGLE;
//		tempValue = SPI_I2S_ReceiveData(macSPIx );
//		
//		SPI_I2S_ClearFlag(macSPIx , SPI_I2S_FLAG_RXNE|SPI_I2S_FLAG_TXE);
//	
//		SPI_FLASH_SendByte(0xaa);
	
}



/*********************************************END OF FILE**********************/
