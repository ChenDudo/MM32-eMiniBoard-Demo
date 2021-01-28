////////////////////////////////////////////////////////////////////////////////
#ifndef __LCD_H_
#define __LCD_H_
////////////////////////////////////////////////////////////////////////////////

#define  DMAX 	80

#define offsetH		50

////////////////////////////////////////////////////////////////////////////////
#ifdef _LCD_C_
#define GLOBAL

u16 dx = 5;
u16 dy = 10;
u16 dw = 242;
u16 dh = 135;

#else
#define GLOBAL extern
#endif

GLOBAL u16 dx;
GLOBAL u16 dy;
GLOBAL u16 dw;
GLOBAL u16 dh;
GLOBAL u16 drawBlockCnt;
GLOBAL u16 lcdCnt;
#undef GLOBAL
////////////////////////////////////////////////////////////////////////////////
void dispADC(u8 sta);
void dispTEMP(u8 sta);
void dispFLASH(u8 sta);
void dispRTC(u8 sta);
void dispI2C(u8 sta);
void dispCAN(u8 sta);
void dispUART1(u8 sta);
void dispUART2(u8 sta);
void dispLED(u8 sta);
u16 getColor(u8 c);
void randRefresh();
void clearLeftScreen();
void clearButtomScreen();

void setCAN(u16 w, u16 h, u8 idx, u8 sel);
void setUART(u16 w, u16 h, u8 idx, u8 sel);
void frontBack(u8 sel);
void setTIME(u16 w, u16 h, u8 idx, u8 sel);
void menuCAN(u8 idx, u8 sta, u8 sel);
void menuUART(u8 idx, u8 sta, u8 sel);
void menuTIME(u8 idx, u8 sta, u8 sel);
void dispScreen();

void drawMM(u16 x, u16 y, u16 w);

void initLcdDemo();
void refreshLCD(u8 sele,  u16* flag);
void BSP_LCD_Configure();


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
