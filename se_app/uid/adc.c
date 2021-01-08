////////////////////////////////////////////////////////////////////////////////
/// @file     UID.C
/// @author   D CHEN
/// @version  v2.0.0
/// @date     2019-03-13
/// @brief    THIS FILE PROVIDES ALL THE UID EXAMPLE.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT 2018-2019 MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define _ADC_C_

// Includes  -------------------------------------------------------------------
#include "types.h"
#include "adc.h"
#include "common.h"

#include "hal_adc.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_dma.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup EXAMPLE_UID
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UID_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
void adcTick()
{
#if defined(__MM32_MB032) 
    if(++adcCnt > 5){
        adcCnt = 0;
        ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
        if(ADC_GetFlagStatus(ADC1, ADC_IT_EOC)){
            adcValue[0] = ADC1->CH7DR;
            adcValue[1] = ADC1->CH5DR;
            adcValue[2] = ADC1->CH2DR;
        }
        for(u8 i = 0; i < 3; i++){
            rv[i] = (u16)((float)(rv[i] * 8 + adcValue[i] * 2) / 10);
        }
        adcFlag = true;
    }
#endif
#if defined(__MM32_MB020) || defined(__MM32_MB021)
    if(++adcCnt > 5){
        adcCnt = 0;
        ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
        while(ADC_GetFlagStatus(ADC1, ADC_IT_EOC) == 0);
        if(ADC_GetFlagStatus(ADC1, ADC_IT_EOC)){
            adcValue[0] = ADC_GetConversionValue(ADC1) & 0xFFFF;
        }
        for(u8 i = 0; i < 3; i++){
            rv[i] = (u16)((float)(rv[i] * 8 + adcValue[0] * 2) / 10);
        }
        adcFlag = true;
    }
#endif
#if defined(__MM32_MB022) || defined(__MM32_MB023)|| defined(__MM32_MB024)|| defined(__MM32_MB025)
    if(++adcCnt > 5){
        adcCnt = 0;
        ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
        if(ADC_GetFlagStatus(ADC1, ADC_IT_EOC)){
            adcValue[0] = ADC1->CH1DR;
            adcValue[1] = ADC1->CH4DR;
            adcValue[2] = ADC1->CH5DR;
        }
        for(u8 i = 0; i < 3; i++){
            rv[i] = (u16)((float)(rv[i] * 8 + adcValue[i] * 2) / 10);
        }
        adcFlag = true;
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
void BSP_ADC_Configure()
{
#if defined(__MZ311)
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_PRESCARE  = ADC_PCLK2_PRESCARE_16;
    ADC_InitStructure.ADC_Mode      = ADC_Mode_Continue;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = 0;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC1->CFGR &= ~ADC_CFGR_SAMCTL;
    ADC1->CFGR |= ADC_CFGR_SAMCTL_14_5;
    
    ADC_Cmd(ADC1, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                           //Output speed
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                               //GPIO mode
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    ADC_ANY_Cmd(ADC1, DISABLE);
    ADC_ANY_NUM_Config(ADC1, 2);
    ADC_ANY_CH_Config(ADC1, 0, ADC_Channel_2);
    ADC_ANY_CH_Config(ADC1, 1, ADC_Channel_5);
    ADC_ANY_CH_Config(ADC1, 2, ADC_Channel_7);
    ADC_ANY_Cmd(ADC1, ENABLE);
#endif
#if defined(__MM32_MB020) || defined(__MM32_MB021)
    ADC_InitTypeDef  ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    COMMON_EnableIpClock(emCLOCK_GPIOA);
    COMMON_EnableIpClock(emCLOCK_ADC1);
    
    GPIO_InitStructure.GPIO_Pin  =  ADCPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Continue;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC1_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, CHENALL, 0, ADC_Samctl_239_5);
    ADC_RegularChannelConfig(ADC1, ADCCHx, 0, ADC_Samctl_239_5);
    ADC_Cmd(ADC1, ENABLE);
#endif
#if defined(__MM32_MB022) || defined(__MM32_MB023)|| defined(__MM32_MB024)|| defined(__MM32_MB025)
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    COMMON_EnableIpClock(emCLOCK_ADC1);
    COMMON_EnableIpClock(emCLOCK_DMA1);
    COMMON_EnableIpClock(emCLOCK_GPIOA);
   
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    ADC_InitStructure.ADC_Mode       = ADC_CR_CONTINUE;
    ADC_InitStructure.ADC_PRESCARE   = ADC_PCLK2_PRESCARE_16;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC1_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign  = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_Init(ADC1, &ADC_InitStructure);
    
#if defined(__MZ309) || defined(__MZ310) || defined(__MZ311)
    ADCSAM_TypeDef samtemp = ADC_Samctl_2_5;
#else
    ADCSAM_TypeDef samtemp = ADC_Samctl_1_5;
#endif
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1,  1, samtemp);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4,  2, samtemp);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5,  3, samtemp);
    
    ADC_Cmd(ADC1, ENABLE);
#endif
}


/// @}

/// @}

/// @}
