 /**
 * @file Adafruit_GFX.h
 * @brief Definitions Adafruit_GFX class for Fireduino
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
#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif
#include "TF.h"
#define __SD_H__
#if defined(__SD_H__)  // Arduino SD library
 #include "PImage.h"
#else
 #warning "The SD library was not found. loadImage() and image() won't be supported."
#endif

#define swap(a, b) { int16_t t = a; a = b; b = t; }


typedef uint16_t color;

class Adafruit_GFX : public Print {
 public:

  Adafruit_GFX(int16_t w, int16_t h); // Constructor

  // This MUST be defined by the subclass
  virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

  

  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void 
	drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, 
		uint16_t color),
  	drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
	drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
	drawRect(int16_t x, int16_t y, int16_t w, int16_t h, 
		uint16_t color),
	fillRect(int16_t x, int16_t y, int16_t w, int16_t h, 
		uint16_t color),
	fillScreen(uint16_t color),
	invertDisplay(boolean i);

// These exist only with Adafruit_GFX (no subclass overrides)
  void
	drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
	drawCircleHelper(int16_t x0, int16_t y0,
			int16_t r, uint8_t cornername, uint16_t color),
	fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
	fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
		      uint8_t cornername, int16_t delta, uint16_t color),

	drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		    int16_t x2, int16_t y2, uint16_t color),
	fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		    int16_t x2, int16_t y2, uint16_t color),
	drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		     int16_t radius, uint16_t color),
	fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
		     int16_t radius, uint16_t color),

	drawBitmap(int16_t x, int16_t y, 
		  const uint8_t *bitmap, int16_t w, int16_t h,
		  uint16_t color),
	drawChar(int16_t x, int16_t y, unsigned char c,
		uint16_t color, uint16_t bg, uint8_t size),
	setCursor(int16_t x, int16_t y),
	setTextColor(uint16_t c),
	setTextColor(uint16_t c, uint16_t bg),
	setTextSize(uint8_t s),
	setTextWrap(boolean w),
	setRotation(uint8_t r);

//#if ARDUINO >= 100
  virtual size_t write(uint8_t);
//#else
//  virtual void   write(uint8_t);
//#endif

  int16_t 
	height(void),
	width(void);


  uint8_t getRotation(void);

  
  /*
   * Processing-like graphics primitives
   */
  
  /// transforms a color in 16-bit form given the RGB components.
  /// The default implementation makes a 5-bit red, a 6-bit
  /// green and a 5-bit blue (MSB to LSB). Devices that use
  /// different scheme should override this. 
  virtual uint16_t newColor(uint8_t red, uint8_t green, uint8_t blue);
  
  
  void
	// http://processing.org/reference/background_.html
	background(uint8_t red, uint8_t green, uint8_t blue),
	background(color c),

	// http://processing.org/reference/fill_.html
	fill(uint8_t red, uint8_t green, uint8_t blue),
	fill(color c),

	// http://processing.org/reference/noFill_.html
	noFill(),

	// http://processing.org/reference/stroke_.html
	stroke(uint8_t red, uint8_t green, uint8_t blue),
	stroke(color c),

	// http://processing.org/reference/noStroke_.html
	noStroke(),
  
	text(const char * text, int16_t x, int16_t y),
	textWrap(const char * text, int16_t x, int16_t y),

	textSize(uint8_t size),
  
	// similar to ellipse() in Processing, but with
	// a single radius.
	// http://processing.org/reference/ellipse_.html
	circle(int16_t x, int16_t y, int16_t r),
	point(int16_t x, int16_t y),
	line(int16_t x1, int16_t y1, int16_t x2, int16_t y2),
	quad(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, int16_t x4, int16_t y4),
rect(int16_t x, int16_t y, int16_t width, int16_t height),
	rect(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius),
	triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3);
  
  /* TODO
  void rectMode(RectMode mode);
  
  void pushStyle();
  void popStyle();
  */

#if defined(__SD_H__)  // Arduino SD library
  PImage loadImage(char * fileName) { return PImage::loadImage(fileName); }
  
  void image(PImage & img, uint16_t x, uint16_t y);
#endif
  
 protected:
  int16_t
	WIDTH, HEIGHT;   // this is the 'raw' display w/h - never changes
  int16_t
	_width, _height, // dependent on rotation
	cursor_x, cursor_y;
  uint16_t
	textcolor, textbgcolor;
  uint8_t
	textsize,
	rotation;
  boolean
	wrap; // If set, 'wrap' text at right edge of display
  
  /*
   * Processing-style graphics state
   */
  
  color strokeColor;
  bool useStroke;
  color fillColor;
  bool useFill;
};



#endif // _ADAFRUIT_GFX_H
