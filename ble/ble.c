////////////////////////////////////////////////////////////////////////////////
/// @file     BLE.C
/// @author   YQ Liu
/// @version  v2.0.0
/// @date     2019-02-18
/// @brief    THIS FILE PROVIDES ALL THE BLE EXAMPLE.
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
#define _BLE_C_

// Includes  -------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "system_mm32.h"

#include "wl_hw.h"
#include "wl_ble_services.h"
#include "wl_ble_lowpower.h"

#include "mg_api.h"
#include "ble.h"
#include "bsp_led.h"
#include "hal_exti.h"
////////////////////////////////////////////////////////////////////////////////
void EXTI0_1_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line1)) {
        EXTI_ClearITPendingBit(EXTI_Line1);
        wl_ble_irq_handler();
    }
}

void EXTI2_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line2)) {
        EXTI_ClearITPendingBit(EXTI_Line2);
        wl_ble_irq_handler();
    }
}

void EXTI9_5_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line9)) {
        EXTI_ClearITPendingBit(EXTI_Line9);
        
        wl_ble_irq_handler();
    }
}

u8* p;
////////////////////////////////////////////////////////////////////////////////
void getSand()
{
    for (u8 i = 0; i < 64; i++) {
        srand(SysTick->VAL);  
        *p++ = rand();
    }
}

////////////////////////////////////////////////////////////////////////////////
void ByteToHexStr(const char* source, char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;
    
    for (i = 0; i < sourceLen; i++) {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;
        
        highByte += 0x30;
        
        if (highByte > 0x39)
            dest[i * 2] = highByte + 0x07;
        else
            dest[i * 2] = highByte;
        
        lowByte += 0x30;
        if (lowByte > 0x39)
            dest[i * 2 + 1] = lowByte + 0x07;
        else
            dest[i * 2 + 1] = lowByte;
    }
    return ;
}


////////////////////////////////////////////////////////////////////////////////
void wl_ble_mode()
{
    ble_running_flag = false;
    char chipUID[3];
    char uidStr[6];
    
#if defined(__MM32_MB021)
    u8 device_name[13] = "MB-021_";
#endif
#if defined(__MM32_MB020)
    u8 device_name[13] = "MB-020_";
#endif
    
    //getSand();
    
    memset(&chipUID, 0x00, sizeof(chipUID));
	memcpy(&chipUID, UID, 3);
    ByteToHexStr(chipUID, uidStr, 3);
    memcpy(device_name + 7, uidStr, 6);
    
    
    cur_notifyhandle = 19;
    memcpy(ble_device_name, device_name, sizeof(device_name));
    
    wl_spi_init();
    wl_irq_it_init();
    
    SetBleIntRunningMode();
    
    radio_initBle(TXPWR_0DBM, &ble_mac_addr); 
    
    
    SysTick_Count = 0;
    while(SysTick_Count < 5);
    
    ble_run_interrupt_start(160*2);
    ble_running_flag = true;
}

u32 tick_count = 0;
void wl_ble_tick_task()
{
    static u32 tx_led_count = 0;
    
    if (ble_running_flag) {
        ble_nMsRoutine();
        tick_count++;
    }
    if (ble_tx_led) {
        ble_tx_led = false;
        ledStatus[0] = 1;
        tx_led_count = SysTick_Count + 100;
    }
    if (tx_led_count < SysTick_Count) {
        ledStatus[0] = 0;
    }
}

u32 irq_count = 0;
void wl_ble_task()
{
    
    //    if (wl_irq_status())
    //    if (ble_irq_flag) {
    //        if (wl_irq_status())
    //            wl_ble_irq_handler();
    //    } else {
    //        irq_count = 0;
    //    }
}

void wl_ble_irq_handler()
{
    //    if (irq_count == 0)
    //        wl_ble_mode();
    
    if (ble_running_flag) {
        irq_count++;
        ble_run(0);        
    }
}

uint32_t GetSysTickCount(void) 	// porting api
{
    return SysTick_Count;
}

static unsigned char gConnectedFlag=0;
char GetConnectedStatus(void)
{
    return gConnectedFlag;
}

void ConnectStausUpdate(unsigned char IsConnectedFlag) //porting api
{	
    if(!IsConnectedFlag)
        CanNotifyFlag = IsConnectedFlag; //disconnected, so can NOT notify data
    
    //[IsConnectedFlag] indicates the connection status
    gConnectedFlag = IsConnectedFlag;
}

// -----------------------------------------------------------------------------
// ---- USER CALLBACK ----------------------------------------------------------
// -----------------------------------------------------------------------------
// As central device
void UsrProcCallback_Central(u8 fin, u8* dat_rcv, u8 dat_len)
{
    
}

// As peri
void UsrProcCallback()
{
	
}

void gatt_client_send_callback(void)
{
    
}

void att_cli_receive_callback(u16 att_hd, u8* attValue/*app data pointer*/, u8 valueLen_w/*app data size*/)
{
    
}