////////////////////////////////////////////////////////////////////////////////
/// @file     WL_HW.C
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE WIRELESS EXAMPLE.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIA2.4GHZ FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2019 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define _WL_HW_C_

#include "HAL_rcc.h"
#include "HAL_spi.h"
#include "HAL_gpio.h"
#include "HAL_exti.h"
#include "HAL_nvic.h"

#include "wl_hw.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize wireless spi(SPI2).
///         SCK     -   PB15
///         MOSI    -   PB13
///         MISO    -   PB12
///         CSN     -   PB14
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void wl_spi_init()
{
    SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

#if defined(__MM32_MB021)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		
    GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
		
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;   		
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
    GPIO_Init(GPIOD, &GPIO_InitStructure);	
		
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
			
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;  		
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING; 		
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
#endif

#if defined(__MM32_MB020)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    GPIO_SetBits(GPIOB, GPIO_Pin_15);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_4);//MISO
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_4);//MOSI
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_3);//SCK   
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_1);//CSN
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;   		//spi2_cs  pb15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOB, GPIO_Pin_12); //csn=H
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;   		//spi2_sck  pb14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;   		//spi2_mosi  pb13
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;  		//spi2_miso  pb12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
    
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataWidth = 8;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    // mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  // mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//6
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(BLE_SPI, &SPI_InitStructure);
    SPI_CSInternalSelected(BLE_SPI, ENABLE);
    SPI_Cmd(BLE_SPI, ENABLE); //Enables the specified SPI peripheral
    
    SPI_BiDirectionalLineConfig(BLE_SPI, SPI_Direction_Tx);
    SPI_BiDirectionalLineConfig(BLE_SPI, SPI_Direction_Rx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Send spi(SPI2) data(8 bits).
/// @param  data.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void wl_spi_tx(u8 data)
{
    SPI_SendData(BLE_SPI, data);
    while(!SPI_GetFlagStatus(BLE_SPI, SPI_FLAG_TXEPT));
    SPI_ReceiveData(BLE_SPI);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Receive spi(SPI2) data(8 bits).
/// @param  None.
/// @retval data(u8).
////////////////////////////////////////////////////////////////////////////////
u8 wl_spi_rx()
{
    SPI_SendData(BLE_SPI, 0xFF);
    while(!SPI_GetFlagStatus(BLE_SPI, SPI_FLAG_RXAVL));
    return SPI_ReceiveData(BLE_SPI);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the wireless module irq port(PB1).
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void wl_irq_init()
{
#if defined(__MM32_MB021)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
#endif
#if defined(__MM32_MB020)
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_EXTI, ENABLE);
#endif    
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin  = IRQ_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(IRQ_PORT, &GPIO_InitStructure);
    
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize the wireless module IRQ port(PB1), interrupt mode.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void wl_irq_it_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    exNVIC_Init_TypeDef NVIC_InitStructure;

#if defined(__MM32_MB021)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
#endif
    
#if defined(__MM32_MB020)
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_EXTI, ENABLE);
#endif
    
    GPIO_InitStructure.GPIO_Pin  = IRQ_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(IRQ_PORT, &GPIO_InitStructure);
    
    EXTI_InitStructure.EXTI_Line = IRQ_IT_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    EXTI_LineConfig(IRQ_IT_SRC_PORT, IRQ_IT_SRC_PIN);
    
    NVIC_InitStructure.NVIC_IRQChannel = IRQ_IT_CH;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    exNVIC_Init(&NVIC_InitStructure);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Activate the wireless module.
/// @param  data.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void wl_activate(u8 data)
{
    SPI_CS_Enable_();
    
    wl_spi_tx(0x50);
    wl_spi_tx(data);
    
    SPI_CS_Disable_();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Write the register of the wireless module.
/// @param  addr: Register address,
///         data: Write data.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void wl_write_reg(u8 addr, u8 data)
{
    SPI_CS_Enable_();
    
    wl_spi_tx(addr | 0x20);
    wl_spi_tx(data);
    
    SPI_CS_Disable_();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Read the register of the wireless module.
/// @param  addr: Register address,
/// @retval data: Read data.
////////////////////////////////////////////////////////////////////////////////
u8 wl_read_reg(u8 addr)
{
    u8 data;
    
    SPI_CS_Enable_();
    
    wl_spi_tx(addr);
    data = wl_spi_rx();
    
    SPI_CS_Disable_();
    
    return data;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Write a set of data to the registers of the wireless module.
/// @param  addr: Register address,
///         buf: Write data buffer pointer,
///         len: Buffer length.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void wl_write_buf(u8 addr,u8* buf, u8 len)
{
    SPI_CS_Enable_();
    
    wl_spi_tx(addr | 0x20);
    
    for(u8 i = 0; i < len; i++)
        wl_spi_tx(buf[i]);
    
    SPI_CS_Disable_();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Read a set of data to the registers of the wireless module.
/// @param  addr: Register address,
///         buf: Read data buffer pointer,
///         len: Buffer length.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void wl_read_buf(u8 addr, u8* buf, u8 len)
{
    SPI_CS_Enable_();
    wl_spi_tx(addr);
    
    for(u8 i = 0; i < len; i++)
        buf[i] = wl_spi_rx();
    
    SPI_CS_Disable_();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get wireless irq port(PB1) Status.
/// @param  None.
/// @retval status(bool).
////////////////////////////////////////////////////////////////////////////////
bool wl_irq_status()
{
    return !(GPIO_ReadInputDataBit(IRQ_PORT, IRQ_PIN));
}

// =============================================================================
// === HARDCODE ================================================================
// =============================================================================
////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI Write and Read.
///         Used by the Bluetooth protocol stack.
/// @param  data(8 bits),
///         dir: emWL_SPI_RX or emWL_SPI_TX.
/// @retval data(receive data, 8 bits).
////////////////////////////////////////////////////////////////////////////////
u8 SPI_WriteRead(u8 data, EM_WL_SPI_DIR dir) //porting api
{
	BLE_SPI->TDR = data;
	while(!(BLE_SPI->SR & SPI_FLAG_TXEPT));
	while(!(BLE_SPI->SR & SPI_FLAG_RXAVL));
    return BLE_SPI->RDR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI CS enable.
///         Used by the Bluetooth protocol stack.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_CS_Enable_() //porting api
{
#if defined(__MM32_MB020)
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
#endif
#if defined(__MM32_MB021)
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SPI CS disable.
///         Used by the Bluetooth protocol stack.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SPI_CS_Disable_() //porting api
{
#if defined(__MM32_MB020)
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
#endif
#if defined(__MM32_MB021)
    GPIO_SetBits(GPIOD, GPIO_Pin_2);
#endif
}

u8 SPI_WriteBuf(u8 reg, u8 const *pBuf, u8 len)
{
    u8 result = 0;
    u8 i;
    
    SPI_CS_Enable_();
    
    SPI_WriteRead(reg, emWL_SPI_TX);
    
    for (i=0;i<len;i++)
    {
        SPI_WriteRead(*pBuf++, emWL_SPI_TX);
    }
    
    SPI_CS_Disable_();
    
    return result;
}

u8 SPI_ReadBuf(u8 reg, u8 *pBuf, u8 len)
{
    u8 result = 0;
    u8 i;
    
    SPI_CS_Enable_();
    
    result = SPI_WriteRead(reg, emWL_SPI_TX);
    
    for (i=0;i<len;i++)
    {
        *(pBuf+i) = SPI_WriteRead(0xff, emWL_SPI_RX);
    }
    
    SPI_CS_Disable_();
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get wireless irq status.
///         Used by the Bluetooth protocol stack.
/// @param  None.
/// @retval status.
////////////////////////////////////////////////////////////////////////////////
char IsIrqEnabled(void) //porting api
{
    return (!(GPIO_ReadInputData(IRQ_PORT) & IRQ_PIN));
}