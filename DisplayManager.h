#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

/***************************************************
  This is our library for the Adafruit HX8357D Breakout
  ----> http://www.adafruit.com/products/2050

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "SPI.h"
#include "ILI9341_t3n.h"

// These are 'flexible' lines that can be changed
#define TFT_DC 41
#define TFT_CS 38
#define TFT_MOSI 26
#define TFT_SCK 27
#define TFT_RST 40
#define TFT_MISO 39

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

class DisplayManager{

public:

  void Setup(){
    tft.begin();
    tft.setTextSize(3);
    tft.setRotation(1);
    clearScreen();
  }

  void clearScreen(){
    tft.fillScreen(ILI9341_BLACK);
  }

  void println(char *text) {
    tft.println(text);
  }

private:
  ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);
  float p = 3.1415926;

};


#endif
