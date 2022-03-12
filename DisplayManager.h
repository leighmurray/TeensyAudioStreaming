#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

// FastLED_SPITFT_GFX example for single NeoPixel Shield.
// By Marc MERLIN <marc_soft@merlins.org>
// Contains code (c) Adafruit, license BSD

// Please see the other demos and FastLED_NeoMatrix_SmartMatrix_LEDMatrix_GFX_Demos/neomatrix_config.h
// on how to avoid having all the config pasted in all your scripts.

#include <Adafruit_SSD1331.h>
#include <FastLED_SPITFT_GFX.h>
#include <FastLED.h>
#include <SPI.h>

#define TFT_DC 40
#define TFT_CS 38
#define TFT_MOSI 26
#define TFT_SCK 27
#define TFT_RST 41
#define TFT_MISO 39 // not used

// Color definitions
// This could also be defined as matrix->color(255,0,0) but those defines
// are meant to work for adafruit_gfx backends that are lacking color()
#define LED_BLACK    0

#define LED_RED_VERYLOW   (3 <<  11)
#define LED_RED_LOW     (7 <<  11)
#define LED_RED_MEDIUM    (15 << 11)
#define LED_RED_HIGH    (31 << 11)

#define LED_GREEN_VERYLOW (1 <<  5)   
#define LED_GREEN_LOW     (15 << 5)  
#define LED_GREEN_MEDIUM  (31 << 5)  
#define LED_GREEN_HIGH    (63 << 5)  

#define LED_BLUE_VERYLOW  3
#define LED_BLUE_LOW    7
#define LED_BLUE_MEDIUM   15
#define LED_BLUE_HIGH     31

#define LED_ORANGE_VERYLOW  (LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW    (LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM (LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH   (LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW  (LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW    (LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM (LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH   (LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW  (LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW    (LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM   (LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH   (LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW (LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW   (LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM  (LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH    (LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)


class DisplayManager{

public:

  void Setup(){
    this->display->begin();
    this->matrix->begin();
    clearScreen();
  }

  void clearScreen(){
    this->matrix->clear();
    this->matrix->setCursor(0,0);
  }

  void print(String text){
    this->matrix->setTextColor(LED_BLUE_HIGH);
    this->matrix->setTextSize(1);
    this->matrix->print(text);
    this->matrix->show();
  }

  void ShowDataReceived(){
    if (showingDataReceived == false){
      showingDataReceived = true;
      this->matrix->fillRect(20, 43, 8, 8, LED_RED_HIGH);
      this->matrix->show();
    }
    noDataReceivedCounter = 0;
  }

  void ShowNoDataReceived(){
    if (showingDataReceived == true && noDataReceivedCounter > 10000) {
      showingDataReceived = false;
      this->matrix->fillRect(20, 43, 8, 8, LED_BLACK);
      this->matrix->show();
    }
    noDataReceivedCounter++;
  }

  void ShowDataSent(){
    if (showingDataSent == false) {
      showingDataSent = true;
      this->matrix->fillRect(20, 33, 8, 8, LED_GREEN_HIGH);
      this->matrix->show();
    }
    noDataSentCounter = 0;
  }

  void ShowNoDataSent(){
    // only show no data sent if there's been over 10000 cycles without data, stops screen flicker
    if (showingDataSent == true && noDataSentCounter > 10000) {
      showingDataSent = false;
      this->matrix->fillRect(20, 33, 8, 8, LED_BLACK);
      this->matrix->show();
    }
    noDataSentCounter++;
  }

private:
  Adafruit_SSD1331 *display = new Adafruit_SSD1331(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);
  static const int mw = 96;
  static const int mh = 64;
  int rotation = 0;
  CRGB matrixleds[mw*mh];;
  FastLED_SPITFT_GFX *matrix = new FastLED_SPITFT_GFX(matrixleds, mw, mh, 96, 64, display, rotation);
  
  bool showingDataSent = false;
  bool showingDataReceived = false;
  int noDataSentCounter = 0;
  int noDataReceivedCounter = 0;
};


#endif
