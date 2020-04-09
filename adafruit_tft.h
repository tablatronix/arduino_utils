#ifndef adafruit_tft_h
#define adafruit_tft_h

#define LTBLUE    0xB6DF
#define LTTEAL    0xBF5F
#define LTGREEN   0xBFF7
#define LTCYAN    0xC7FF
#define LTRED     0xFD34
#define LTMAGENTA 0xFD5F
#define LTYELLOW  0xFFF8
#define LTORANGE  0xFE73
#define LTPINK    0xFDDF
#define LTPURPLE  0xCCFF
#define LTGREY    0xE71C
#define LTGRAY    0xE71C

#define BLUE      0x001F
#define TEAL      0x0438
#define GREEN     0x07E0
#define CYAN      0x07FF
#define RED       0xF800
#define MAGENTA   0xF81F
#define MAROON    0x7800
#define YELLOW    0xFFE0
#define ORANGE    0xFC00
#define PINK      0xF81F
#define PURPLE    0x8010
#define GREY      0xC618
#define GRAY      0xC618
#define WHITE     0xFFFF
#define BLACK     0x0000

#define DKBLUE    0x000D
#define DKTEAL    0x020C
#define DKGREEN   0x03E0
#define DKCYAN    0x03EF
#define DKRED     0x6000
#define DKMAGENTA 0x8008
#define DKYELLOW  0x8400
#define DKORANGE  0x8200
#define DKPINK    0x9009
#define DKPURPLE  0x4010
#define DKGREY    0x4A49
#define DKGRAY    0x4A49
// #define LTBLACK   0x2945
#define LTBLACK   0x18C3

#include "Adafruit_GFX.h" // Library Manager
#include <SPI.h>

// #define USE_ADAFRUIT_ST7735
// #define USE_ADAFRUIT_ST7789H
// 
#ifdef USE_ADAFRUIT_ST7735
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#elif defined(USE_ADAFRUIT_ST7789)
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7735
#else
#include "Adafruit_ILI9341.h" // Library Manager
#endif

// TFT SPI pins
// #define TFT_DC 0
// #define TFT_CS 3
// #define TFT_RESET 1

// @TODO use CS and get tft and max31855 tc with hspi pins
#define TFT_DC     0 // D3
#define TFT_CS    -1 // Tied to ground
#define TFT_RST   -1 // tied to RST - ST7789 CS low, D2 not working
// Initialise the TFT screen
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RESET);
// Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
// Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

#ifdef USE_ADAFRUIT_ST7735
using RM_tft = Adafruit_ST7735;
#elif defined(USE_ADAFRUIT_ST7789)
using RM_tft = Adafruit_ST7789;
#else 
using RM_tft = Adafruit_ILI9341;
#endif

RM_tft tft = RM_tft(TFT_CS, TFT_DC, TFT_RST);

#define TFT_WIDTH   ILI9341_TFTWIDTH
#define TFT_HEIGHT  ILI9341_TFTHEIGHT

// #include <BarGraph.h>
// #define SCALECOLOR 0xFFFF
// BarGraph bar0;

// NODEMCU hspi
// HW scl      D5 14
// HW mosi/sda D7 13
// HW miso     D6 12
// HW cs       D8 15
// 
// ESPLED      D4 02
// boardLED    D0 16

uint16_t rotation = 3;

#ifdef USE_ADAFRUIT_ST7735
uint16_t textsize_1 = 1;
uint16_t textsize_2 = 1;
uint16_t textsize_3 = 1;
uint16_t textsize_4 = 2;
uint16_t textsize_5 = 3;
#else
uint16_t textsize_1 = 1;
uint16_t textsize_2 = 2;
uint16_t textsize_3 = 3;
uint16_t textsize_4 = 4;
uint16_t textsize_5 = 5;
#endif


void println_header(const char *string, uint16_t color)
{
  int thickness = 24;
  int vpad = 2;
  int fontsize = 4;
  // tft.fillScreen(color);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.fillRect(0, 0, 320, thickness, color);
  // tft.setTextDatum(TC_DATUM);
  // tft.drawString(string, 160, 2, fontsize); // Font 4 for fast drawing with background
  // tft.drawString(string,tft.width()/2,vpad,fontsize);
  tft.setCursor(10+tft.width()/3,vpad);
  tft.println(string);
}

void println_footer(const char *string, uint16_t color){
  int thickness = 24;
  int vpad = 3;
  int fontsize = 4;

  tft.setTextSize(2);
    tft.fillRect(0, tft.height()-thickness , tft.width(), thickness, color);
    // tft.setTextDatum(TC_DATUM);
    tft.setTextColor(WHITE);
    tft.setCursor(10+tft.width()/3,tft.height()-(thickness-vpad));
    tft.println(string);
    // tft.drawString(string,tft.width()/2,tft.height()-(thickness-vpad),fontsize);
}

void initTFT(){

  #ifdef DEBUG
    Serial.println("TFT Begin...");
  #endif

  #ifdef USE_ADAFRUIT_ST7735
    tft.initR(INITR_144GREENTAB); 
  #elif defined(USE_ADAFRUIT_ST7789)
    tft.init(240, 240, SPI_MODE3);   // Init ST7789 240x240
  #else
    // default ili9341
    tft.begin();
  #endif
    tft.setRotation(rotation);
    // tft.setFont(&FreeMono9pt7b);
  tft.fillScreen(ILI9341_BLACK);
  println_header("HEADER",PURPLE);
  // tft.drawFastHLine(0, 24, tft.width(), TFT_DARKGREY);  
  println_footer("FOOTER",MAROON);
  // tft.drawFastHLine(0, tft.height()-24, tft.width(), TFT_DARKGREY);  

  #ifdef DEBUG
    Serial.println("Booted Spash Screen");
  #endif
}

void tft_init(){
  initTFT();
}

#endif