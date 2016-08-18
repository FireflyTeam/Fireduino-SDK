 /**
 * @file Adafruit_ST7735.cpp
 * @brief Definitions Adafruit_ST7735 class for Fireduino
 * @author jiang<jdz@t-chip.com.cn> 
 * @version V1.0
 * @date 2016.02
 * 
 * @par Copyright:
 * Copyright (c) 2016 T-CHIP INTELLIGENCE TECHNOLOGY CO.,LTD. \n\n
 *
 * For more information, please visit website <http://www.t-firefly.com/>, \n\n
 * or email to <service@t-firefly.com>.
 */ 
#include "Adafruit_ST7735.h"
#include "ard_st7735s.h"
inline uint16_t swapcolor(uint16_t x) { 
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}

Adafruit_ST7735::Adafruit_ST7735(void) :
Adafruit_GFX(ST7735_TFTWIDTH, ST7735_TFTHEIGHT) 
{

}

void Adafruit_ST7735::backlight(unsigned char bl) {
	ard_lcd_backlight(bl);
}

void Adafruit_ST7735::writecommand(unsigned int c) {
	ard_writecommand(c);
}

void Adafruit_ST7735::writedata(unsigned int  d) {
	ard_writedata(d);
}

void Adafruit_ST7735::initS(void) {
	ard_lcd_init();
}

void Adafruit_ST7735::setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1,uint8_t y1) {
	ard_ST7735S_SetWindow(x0,y0,x1,y1);
}

void Adafruit_ST7735::pushColor(uint16_t color) {
  ard_VopSendData (color);
}

void Adafruit_ST7735::drawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height))
	  return;

  setAddrWindow(x,y,x+1,y+1);
  ard_VopSendData (color);
}


void Adafruit_ST7735::drawFastVLine(int16_t x, int16_t y, int16_t h,uint16_t color) {
  if((x >= _width) || (y >= _height))
	  return;
  if((y+h-1) >= _height)
	  h = _height-y;

  setAddrWindow(x, y, x, y+h-1);

  while (h--) {
	  ard_VopSendData (color);
  }
}


void Adafruit_ST7735::drawFastHLine(int16_t x, int16_t y, int16_t w,uint16_t color) {

  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  setAddrWindow(x, y, x+w-1, y);

  while (w--) {
	  ard_VopSendData (color);
  }
}



void Adafruit_ST7735::fillScreen(uint16_t color) {
  fillRect(0, 0,  _width, _height, color);
}

void Adafruit_ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {

  if((x >= _width) || (y >= _height))
	  return;

  if((x + w - 1) >= _width)
	  w = _width  - x;

  if((y + h - 1) >= _height)
	  h = _height - y;

  setAddrWindow(x, y, x+w-1, y+h-1);

  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
    	ard_VopSendData (color);
    }
  }
}


#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x08
#define MADCTL_MH  0x04

void Adafruit_ST7735::setRotation(uint8_t m) {

  rotation = m & 3; // can't be higher than 3
  switch (rotation) {
   case 0:
	 ard_ST7735S_Rotation(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
    _width  = ST7735_TFTWIDTH;
    _height = ST7735_TFTHEIGHT;
    break;
   case 1:
	   ard_ST7735S_Rotation(MADCTL_MY | MADCTL_MV | MADCTL_RGB);
    _width  = ST7735_TFTHEIGHT;
    _height = ST7735_TFTWIDTH;
    break;
   case 2:
	   ard_ST7735S_Rotation(MADCTL_RGB);
    _width  = ST7735_TFTWIDTH;
    _height = ST7735_TFTHEIGHT;
    break;
   case 3:
	   ard_ST7735S_Rotation(MADCTL_MX | MADCTL_MV | MADCTL_RGB);
    _width  = ST7735_TFTHEIGHT;
    _height = ST7735_TFTWIDTH;
    break;
  }
}

void Adafruit_ST7735::invertDisplay(boolean i) {
  writecommand(i ? ST7735_INVON : ST7735_INVOFF);
}
