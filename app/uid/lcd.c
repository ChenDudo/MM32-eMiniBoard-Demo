////////////////////////////////////////////////////////////////////////////////
#define _LCD_C_
////////////////////////////////////////////////////////////////////////////////


#if defined(__MDM2803)


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "types.h"

#include "common.h"
#include "hal_gpio.h"
#include "bsp_lcd.h" 
#include "lcd.h" 

#include "adc.h" 
#include "main.h" 

////////////////////////////////////////////////////////////////////////////////
//void dispRTC(u8 sta)
//{
//	dateTimeDef tp;
//	
//	char str[32];
//	if (!sta){
//		drawTab(  5, 70, 60,  15, 1, 1);
//		drawTab( 64, 70, 180, 15, 1, 1);
//
//		strcpy(str, "RTC");
//		putStr(27, 74, 0, 1, str);
//	}
//	else {
//		RTC_GetTime(&tp);
//		sprintf((char*)str, "%0.4d/%0.2d/%0.2d %0.2d:%0.2d:%0.2d",
//						tp.year,
//						tp.month,
//						tp.day,
//						tp.hours,
//						tp.minute,
//						tp.second);
//		putStr(75, 74, 0, 1, str);
//	}
//}


////////////////////////////////////////////////////////////////////////////////
u16 getColor(u8 c)
{
	u16 colorTab[] = { 0x0000, 0x000F, 0x03E0, 0x03EF, 0x7800, 0x780F, 0x7BE0, 0xC618, 
					   0x7BEF, 0x001F, 0x07E0, 0x07FF, 0xF800, 0xF81F, 0xFFE0, 0xFFFF};
	return colorTab[c];
}

////////////////////////////////////////////////////////////////////////////////
void randRefresh()
{
	//if (!*flag) return;
	//*flag = false;
	//u16 x,y,w,h,c;
	//if (drawBlockCnt < 200)
	//	drawBlockCnt++;
	//if (drawBlockCnt < 80) {
	//	x = rand();		x %= (dw - 2);
	//	y = rand();		y %= (dh - 2);
	//	w = rand();		w %= DMAX;
	//	h = rand();		h %= DMAX;
	//	c = rand(); c &= 0x0f;
	//	if ((x + w) > (dw - 2)) x = dw - w - 2;
	//	if ((y + h) > (dh - 2)) y = dh - h - 2;
	//	drawRec (x + dx + 1, y + dy + 1, w, h, getColor(c));
	//}
	//else if (drawBlockCnt < 90) {
	//	c = rand(); c &= 0x0f;
	//	drawRec (dx + 1, dy + 1, dw - 2, dh - 2, getColor(c));
	//}
    
    u16 x,y,w,h,c;
    drawSquare(dx,  dy,  dw, dh, SPACE, NUL);
	if (drawBlockCnt++ % 2) {
		x = rand();		x %= (dw - 2);
		y = rand();		y %= (dh - 2);
		w = rand();		w %= DMAX;
		h = rand();		h %= DMAX;
		c = rand(); c &= 0x0f;
		if ((x + w) > (dw - 2)) x = dw - w - 2;
		if ((y + h) > (dh - 2)) y = dh - h - 2;
		drawRec (x + dx + 1, y + dy + 1, w, h, getColor(c));
	}
	else {
		c = rand(); c &= 0x0f;
		drawRec (dx + 1, dy + 1, dw - 2, dh - 2, getColor(c));
	}

}

////////////////////////////////////////////////////////////////////////////////
void clearLeftScreen()
{
	drawRec (dx , dy , dw, dh + 95, Black);
}

////////////////////////////////////////////////////////////////////////////////
void clearButtomScreen()
{
	drawRec (dx , dy + dh + 95 , dw, 240, Black);
}

////////////////////////////////////////////////////////////////////////////////
//void setTIME(u16 w, u16 h, u8 idx, u8 sel)
//{
//	char str[16];
//	u16 x = (320 - w) / 2;
//	u16 y = (240 - h) / 2;
//	
//	if (!sel){
//		strcpy(str, "  Year");	putStr(x + 20 , y + 18, 2, 0, str);
//		strcpy(str, " Mouth");	putStr(x + 20 , y + 38, 2, 0, str);
//		strcpy(str, "   Day");	putStr(x + 20 , y + 58, 2, 0, str);
//		strcpy(str, "  Hour");	putStr(x + 20 , y + 78, 2, 0, str);
//		strcpy(str, "Minute");	putStr(x + 20 , y + 98, 2, 0, str);
//		strcpy(str, "second");	putStr(x + 20 , y + 118, 2, 0, str);
//		drawLine ( x + 15, y + 142,  x + 155, y + 142, DarkGrey);
//	}
//
//	(idx == 0) ? frontBack(1) : frontBack(0); 	sprintf((char*)str, "%0.4d",gtp.year);	putStr(x + 110,  y + 18,  2, 1, str);
//	(idx == 1) ? frontBack(1) : frontBack(0);	sprintf((char*)str, "%0.2d",gtp.month);	putStr(x + 126 , y + 38,  2, 1, str);
//	(idx == 2) ? frontBack(1) : frontBack(0);	sprintf((char*)str, "%0.2d",gtp.day);	putStr(x + 126 , y + 58,  2, 1, str);
//	(idx == 3) ? frontBack(1) : frontBack(0);	sprintf((char*)str, "%0.2d",gtp.hours);	putStr(x + 126 , y + 78,  2, 1, str);
//	(idx == 4) ? frontBack(1) : frontBack(0);	sprintf((char*)str, "%0.2d",gtp.minute);putStr(x + 126,  y + 98,  2, 1, str);
//	(idx == 5) ? frontBack(1) : frontBack(0);	sprintf((char*)str, "%0.2d",gtp.second);putStr(x + 126 , y + 118, 2, 1, str);
//	(idx == 6) ? frontBack(1) : frontBack(0);	strcpy(str, " Save & Exit ");			putStr(x + 32 ,  y + 148, 2, 1, str);
//}

////////////////////////////////////////////////////////////////////////////////
void menuTIME(u8 idx, u8 sta, u8 sel)
{
	Color_Def c;
	char str[12];
	
	if (sta){
		if (!sel) {
			strcpy(str, " Set Time ");
			c.fore = DarkCyan;
			c.back = LightGrey;
			drawDialog(170, 178, &str[0], c);
		}
		c.fore = White;
		c.back = LightGrey;
		setTIME(170, 178, idx, sel);
	}
	else {
		c.fore = LightGrey;
		c.back = DarkCyan;
		clearDialog(170, 178, c);
	}
}

////////////////////////////////////////////////////////////////////////////////
void dispScreen()
{
	text.fore = White;
	text.back = Black;
    
	//dispRTC(0);
}

void drawMM(u16 x, u16 y, u16 w)
{
    u16 color = SkyBlue;
    u8 xsub = 1, ysub = 1;
    
    drawLine(x+0*w, y+1*w, x+1*w, y+0*w, color); //M0 - M1
    drawLine(x+1*w, y+0*w, x+2*w-xsub, y+0*w, color); //M1 - M2
    drawLine(x+2*w-xsub, y+0*w, x+2*w-xsub, y+1*w+ysub, color); //M2 - M3
    drawLine(x+1*w, y+2*w, x+2*w, y+1*w, color); //M3 - M4
    drawLine(x+1*w, y+2*w, x+2*w, y+3*w, color); //M4 - M5
    drawLine(x+2*w, y+3*w, x+3*w-xsub, y+2*w+ysub, color); //M5 - M6
    drawLine(x+3*w-xsub, y+2*w+ysub, x+4*w-xsub, y+2*w+ysub, color); //M6 - M7
    drawLine(x+4*w-xsub, y+2*w+ysub, x+4*w-xsub, y+3*w+ysub, color); //M7 - M8
    drawLine(x+3*w, y+4*w, x+4*w-xsub, y+3*w+ysub, color); //M8 - M9
    drawLine(x+3*w, y+4*w, x+1*w, y+4*w, color); //M9 - M10
    drawLine(x+1*w, y+4*w, x+1*w, y+3*w, color); //M10 - M11
    drawLine(x+1*w, y+3*w, x+0*w, y+3*w, color); //M11 - M12
    drawLine(x+0*w, y+3*w, x+0*w, y+1*w, color); //M12 - M0
    
    color = Yellow;
    drawLine(x+2*w+xsub, y+1*w-ysub, x+3*w, y+0*w, color); //M0 - M1
    drawLine(x+3*w, y+0*w, x+5*w, y+0*w, color); //M1 - M2
    drawLine(x+5*w, y+0*w, x+5*w, y+1*w, color); //M2 - M3
    drawLine(x+5*w, y+1*w, x+6*w, y+1*w, color); //M3 - M4
    drawLine(x+6*w, y+1*w, x+6*w, y+3*w, color); //M4 - M5
    drawLine(x+5*w, y+4*w, x+6*w, y+3*w, color); //M5 - M6
    drawLine(x+4*w+xsub, y+4*w, x+5*w, y+4*w, color); //M6 - M7
    drawLine(x+4*w+xsub, y+4*w, x+4*w+xsub, y+3*w-ysub, color); //M7 - M8
    drawLine(x+4*w+xsub, y+3*w-ysub, x+5*w, y+2*w, color); //M8 - M9
    drawLine(x+4*w, y+1*w, x+5*w, y+2*w, color); //M9 - M10
    drawLine(x+3*w+xsub, y+2*w-ysub, x+4*w, y+1*w, color); //M10 - M11
    drawLine(x+2*w+xsub, y+2*w-ysub, x+3*w+xsub, y+2*w-ysub, color); //M11 - M12
    drawLine(x+2*w+xsub, y+2*w-ysub, x+2*w+xsub, y+1*w-ysub, color); //M12 - M0
    
    putStr(x+6*w, y+0*w+2, 2, 1, "MindMotion");
    putStr(x+6*w+3, y+1*w, 0, 1, "Soc Solutions");
    
    text.fore = White;
    putStr(220, 210, 2, 1, "MM32-EVBoard");
    drawLine(218, 225, 315, 225, White); //M12 - M0
    putStr(243, 230, 0, 1, "MB-039 v0.01");
    
}

////////////////////////////////////////////////////////////////////////////////
void initLcdDemo()
{
	text.fore = SkyBlue;
	text.back = Black;
	color.c1 = Blue;
    
	drawSquare(dx,  dy,  dw, dh, SPACE, NUL);
	color.c1 = LightGrey;
	color.c2 = DarkCyan;
	color.c3 = White;
	color.c4 = Yellow;
    
	drawCircle(60,  195, 30, Yellow);
	drawCircle(190, 195, 30, Yellow);
	
	drawSquare(10,  160, 40, 25, SPACE, NUL);
	drawSquare(10,  210, 40, 25, FRAME, NUL);

	drawSquare(105, 160, 40, 25, FRAME, NUL);
	drawSquare(105, 210, 40, 25, SPACE, NUL);
	
	drawSquare(205,  160, 40, 25, SPACE, NUL);
	drawSquare(205,  210, 40, 25, FRAME, NUL);
    
	drawLine (10, 160, 50, 185, Red);
	drawLine (205, 185, 245, 160, Red);
}

////////////////////////////////////////////////////////////////////////////////
void refreshLCD(u8 sele,  u16* flag)
{
	if (!*flag) return; 
	*flag = false;
	text.back = DarkCyan;
	
	switch (sele) {
        //case REFRESH_ADC:	dispADC(1);		break;
        default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
void BSP_LCD_Configure()
{
    initGPIO_LCD();
	initFSMC();

	LCDC_Init_Reg();
	lcdFillColor(Black);
	lcdBlcH();
}

#endif
