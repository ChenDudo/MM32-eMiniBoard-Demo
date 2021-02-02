////////////////////////////////////////////////////////////////////////////////
/// @file     MAIN.C
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
#define _MAIN_C_

// Includes  -------------------------------------------------------------------
#include <string.h>
#include "types.h"
#include "system_mm32.h"

#include "mm32.h"
#include "main.h"
#include "common.h"

#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_beep.h"

#include "music.h"
#include "adc.h"
#include "hci.h"
#include "lcd.h"

#if defined(__MM32_MB020) || defined(__MM32_MB021)
#include "ble.h"
#endif

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
void initPara()
{
    sPlayMusic.MusicNum = 1;
    
#if defined(__MM32_MB039)
    sPlayMusic.PlayFlag = false;
#else
    sPlayMusic.PlayFlag = true;
#endif
}

////////////////////////////////////////////////////////////////////////////////
void initPeri()
{
    BSP_KEY_Configure();
    BSP_KeyFuncSet(1, Key1Down, Key1Pressing);
    BSP_KeyFuncSet(2, Key2Down, Key2Pressing);
    BSP_KeyFuncSet(3, Key3Down, Key3Pressing);
    BSP_KeyFuncSet(4, Key4Down, Key4Pressing);
    
    BSP_BEEP_Configure(1000);
    for(u8 i = 0; i < 2; i++){
        OpenLED();
        BEEP_on(1500);
        while(!delay(50));
        CloseLED();
        BEEP_off();
        while(!delay(200));
    }
    
    BSP_ADC_Configure();
    BSP_LED_Configure();
    
#if defined(__MDM2803)
    BSP_LCD_Configure();
    initLcdDemo();
	clearLeftScreen();
    clearButtomScreen();
    drawMM(20, 160, 20);
#endif    

    ready = true;
    
#if defined(__MM32_MB020) || defined(__MM32_MB021)
    wl_ble_mode();
#endif  
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function handles App SysTick Handler.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void AppTaskTick()
{
    if (tickCnt++ >= 500) {
        tickCnt = 0;
        tickFlag = true;
    }
    if(ready){
        if (playCnt++ >= 20) {
            playCnt = 0;
            musicTick();
        }
#if defined(__MDM2803)
        if(lcdCnt++ >= adcValue[0] / 20 + 50){
            lcdCnt = 0;
            randRefresh();
        }
#endif
    }
    
    adcTick();
    ledTick();
    BSP_KeyTick();
    
#if defined(__MM32_MB020) || defined(__MM32_MB021)
    SysTick_Count++;
    wl_ble_tick_task();
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  main function.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    MCUID = SetSystemClock(emSYSTICK_On, AppTaskTick);

    initPeri();
    initPara();
    
    
    while (1) {
        hci_task();
        
#if defined(__MM32_MB020) || defined(__MM32_MB021)
        wl_ble_task();
#endif
    }
}

/// @}

/// @}

/// @}
