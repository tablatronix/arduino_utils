#ifndef old_i2c_sh1106_h
#define old_i2c_sh1106_h

/*oled_i2c*/

#include <log.h>
#include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #include <Adafruit_SH110X.h>

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Wire.begin(SDA,SCL);  
// Wire.setClock(400000L);

// const char WIFI_SYMBOL[] = { 93,94,123,168, 176, 247, 253, 263, 277,278,279,280,281, '\0'};
#define GLYPH_BELL       93 // bell
#define GLYPH_BT         94 // bt
#define GLYPH_CLOCK     123 // clock
#define GLYPH_FLAME     168 // flame
#define GLYPH_CHART     176 // chart
#define GLYPH_SIGNAL    253 // bell
#define GLYPH_SPARK      96 // bell
#define GLYPH_GEAR      129 // bell
#define GLYPH_ALERT   0x118 // bell
#define GLYPH_SPKOFF  0x117 // bell
#define GLYPH_SPKHI   0x115 // bell
#define GLYPH_SPKLO   0x116 // bell
#define GLYPH_HASHF     177 // bell
#define GLYPH_HASHM     178 // bell
#define GLYPH_HASHC     179 // bell
#define GLYPH_HDOTS     155 // bell
#define GLYPH_HBAR      221 // bell
#define GLYPH_CROSS     218 // bell
#define GLYPH_CROSSB    234 // bell
#define GLYPH_EKG       238 // bell
#define GLYPH_WIFI    0x119 // bell
#define GLYPH_WRENCH  0x120 // bell
#define GLYPH_TARGET  0x107 // bell
#define GLYPH_LISTINV 0x101 // bell
#define GLYPH_STAR    0x102 // bell
#define GLYPH_SUN     0x103 // bell
#define GLYPH_STPWTCH  269  // bell
#define GLYPH_TERM    0x109 // bell
#define GLYPH_DEL     0x121 // bell
#define GLYPH_GOOD      120 // bell
#define GLYPH_BAD       121 // bell
#define GLYPH_HEART     183 // bell
#define GLYPH_HOUSE     184 // bell
#define GLYPH_SQUARE    217 // bell
#define GLYPH_SPACE     287 // bell
#define GLYPH_NO         87 // busted
#define GLYPH_COFFEE     2615 // busted

// const char* testing = u8"\u0263\u0BA3\u0B82";

// lcd.setFont(u8g2_font_open_iconic_embedded_1x_t);
// lcd.setFont(u8g2_font_open_iconic_all_1x_t);
// lcd.drawStr(0,y,WIFI_SYMBOL);
// lcd.drawUTF8(0,y,testing);
// lcd.drawGlyph(0,y, 93);    // bell
// lcd.drawGlyph(10,y, 94);    // bluetooth
// lcd.drawGlyph(20,y, 123);   // clock
// lcd.drawGlyph(30,y, 168);   // flame
// lcd.drawGlyph(40,y, 176);   // chart
// lcd.drawGlyph(50,y, 253);   // signal
// lcd.drawGlyph(60,y, 96);    // terminus
// lcd.drawGlyph(70,y, 129);   // gear
// lcd.drawGlyph(80,y, 0x118); // alert
// lcd.drawGlyph(90,y, 0x117); // speaker off
// lcd.drawGlyph(100,y, 0x115); // speaker high
// lcd.drawGlyph(110,y, 0x116); // speaker med

// lcd.drawGlyph(0,y, 177);   // hash fine
// lcd.drawGlyph(10,y, 178);   // hash med
// lcd.drawGlyph(20,y, 179);   // hash low
// lcd.drawGlyph(30,y, 155);   // 3 dots
// lcd.drawGlyph(40,y, 221);   // mid bar
// lcd.drawGlyph(50,y, 218);   // cross
// lcd.drawGlyph(60,y, 234);   // cross
// lcd.drawGlyph(60,y, 238);   // heartbeat

// lcd.drawGlyph(40,y,0x119); // wifi
// lcd.drawGlyph(50,y,0x120); // wrench not working
// lcd.drawGlyph(60,y,0x107); // target
// lcd.drawGlyph(70,y,0x101); // list inverted
// lcd.drawGlyph(80,y,0x102); // star
// lcd.drawGlyph(90,y,0x103); // sunshine
// lcd.drawGlyph(100,y,269);  // stopwatch
// lcd.drawGlyph(110,y,0x109); // terminal
// lcd.drawGlyph(120,y,0x121); // delete
// 
// 
// lcd.drawGlyph(70,y, 120); // good
// lcd.drawGlyph(80,y, 121); // bad
// lcd.drawGlyph(90,y, 183); // heart
// lcd.drawGlyph(100,y, 184); // house
// lcd.drawGlyph(110,y, 217); // square
// // lcd.drawGlyph(110,y, 287); // space

// if(glyphanimstate==0){
//   lcd.drawGlyph(120,y, 155); // 
//   glyphanimstate = 1;
// }
// else{
//   lcd.drawGlyph(120,y, 287); //
//   glyphanimstate = 0;
// }


// https://github.com/olikraus/u8g2/wiki/fntlistall#24-pixel-height
// U8g2 Font names
// <prefix> '_' <name> '_' <purpose> <char set>
// <purpose> Description
// t Transparent font, Do not use a background color.
// h All glyphs have common height.
// m All glyphs have common height and width (monospace).
// 8 All glyphs fit into a 8x8 pixel box.
// <char set>  Description
// f The font includes up to 256 glyphs.
// r Only glyphs on the range of the ASCII codes 32 to 127 are included in the font.
// u Only glyphs on the range of the ASCII codes 32 to 95 (uppercase chars) are included in the font.
// n Only numbers and extra glyphs for writing date and time strings are included in the font.
// ... Other custom character list.

// #define WHITE SH110X_WHITE
// #define BLACK SH110X_BLACK

// #define SH1106_128_64
// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels

// #define OLED_RESET -1
// Adafruit_SSD1306 lcd(OLED_RESET);
// Adafruit_SH110X lcd = Adafruit_SH110X(SCREEN_HEIGHT, SCREEN_WIDTH, &Wire,-1);
// Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET,800000,800000);

U8G2_SH1106_128X64_NONAME_F_HW_I2C lcd(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int oledfpsmicros = 0;

// #if (SSD1306_LCDHEIGHT != 64)
// #error("Height incorrect, please fix Adafruit_SSD1306.h!");
// #endif

void invertText(){
  lcd.setDrawColor(2);
  // lcd.setTextColor(BLACK, WHITE); // 'inverted' text  
}

void whiteText(){
  lcd.setDrawColor(1);
  // lcd.setTextColor(WHITE); // 'inverted' text  
}

// void init_oled(){
//   init_oled(false);
// }

// void init_oled(){
//   Wire.begin(SCL,SDA);  // begin(sda, scl) SWAP!
//   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
//   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
//     Serial.println(F("SSD1306 allocation failed"));
//   }

//   display.clearDisplay();
//   display.setTextSize(1);             // Normal 1:1 pixepl scale
//   display.setTextColor(WHITE);        // Draw white text
//   display.setCursor(0,0);             // Start at top-left corner
//   display.display();
// }

/**
 * print oled lines
 * @param str  string to print
 * @param no   line 0-3
 * @param size text size 1-2
 */
void print_oled_line(String str,uint16_t no = 1,uint16_t size = 1){
  uint16_t y = 0;
  if(size == 1) y = 9*no;
  if(size == 2) y = 18*no;
  lcd.setCursor(0,y);
  // lcd.setTextSize(size);  
  lcd.println(str);
}

void print_oled(String str,uint8_t size,bool flush){
  lcd.clearBuffer();         // clear the internal memory
  lcd.setFont(u8g2_font_ncenB08_tr);
  // lcd.drawStr(0,10,String(micros()).c_str());  // write something to the internal memory
  lcd.setFont(u8g2_font_inb30_mn);
  lcd.drawStr(0,60,str.c_str()); 
  // lcd.drawStr(0,10,micros().c_str();  // write something to the internal memory
  if(flush)lcd.sendBuffer();          // transfer internal memory to the display
}

void init_oled(bool preamble,bool pinswap = false){
  Logger.println("[OLED] Initializing SSD1106 OLED");
  Logger.println("[OLED] SDA: "+(String)SDA);
  Logger.println("[OLED] SCL: "+(String)SCL);
  if(pinswap){
    Wire.begin(5,4);  // begin(sda, scl) SWAP!
    Logger.println("[OLED] pinswapped");
    Logger.println("[OLED] SDA: "+(String)SDA);
    Logger.println("[OLED] SCL: "+(String)SCL);
  }
  // Wire.setClock(400000L);
  #ifdef ROT
    lcd.setDisplayRotation(U8G2_R2);
  #endif
  lcd.setBusClock(100000L);
  lcd.begin();
  // lcd.setI2CAddress(0x78);
  // if(!lcd.begin()) { // Address 0x3C for 128x32
    // Serial.println(F("SSD1106 begin failed"));
  // }
  lcd.setFont(u8g2_font_ncenB08_tr);
  // u8g2_font_inb30_mn
  lcd.clearBuffer();         // clear the internal memory

  int px = 10;
  lcd.clearBuffer();         // clear the internal memory
  lcd.drawStr(0,px,"Starting..."); 
  // lcd.drawStr(60,px,String(micros()).c_str());  // write something to the internal memory
  lcd.sendBuffer();          // transfer internal memory to the display

  return;
  // Wire.setClock(400000L);  // set i2c speed 400khz
  // Wire.setClock(100000L);  // set i2c speed 400khz

  // lcd.clearDisplay();
  // lcd.setTextSize(1);             // Normal 1:1 pixepl scale  
  // lcd.setCursor(0,0);             // Start at top-left corner
  // lcd.setTextColor(WHITE); // REQUIRED!  
  // lcd.display();
  // delay(1000);
  if(!preamble) return;

  // lcd.setTextSize(1);
  lcd.setCursor(0,0);
  lcd.println("Booting....");
  lcd.display();
  delay(1000);
  lcd.clearBuffer();
  lcd.sendBuffer();
}

void displayFPS(){
  String str = (String)(1000000/((micros()-oledfpsmicros)/2));
  lcd.clearBuffer();         // clear the internal memory
  lcd.drawStr(0,10,String(str+ "fps").c_str()); 
  // lcd.drawStr(60,px,String(micros()).c_str());  // write something to the internal memory
  lcd.sendBuffer();
    // println(" FPS");
  oledfpsmicros = micros();
}

// void printInverted(const char* str){
//   lcd.setFontMode(1);
//   lcd.print(str);
//   lcd.setFontMode(0);
// }


int printInverted(const char* str,uint16_t posx,uint16_t posy){
  int lh = lcd.getAscent()+abs(lcd.getDescent()); // font height ?

  // int posy = px*2; // posy VAR
  // int posx = 0; // posy VAR
// Serial.println(lcd.getStrWidth("OFF"));
// Serial.println(lcd.getStrWidth("ON"));
  int padx = (lcd.getStrWidth("OFF")-lcd.getStrWidth("ON"))/2; // w padding
  // int pady = 2; // h padding
  int pady = 1; // h padding
  // Serial.println(padx);
  lcd.setDrawColor(1);
  uint16_t strw = lcd.drawStr(posx+padx, posy, str); // x,y(NEGATIVE GOING!)

  lcd.setFontMode(1); 
  lcd.setDrawColor(2);
  lcd.drawBox(posx, (posy-lh)+pady, strw+(padx*2),lh); // x,y(POSITIVE GOING!),w,h
  
  lcd.setDrawColor(1); // restore default
  return strw+(padx*2);
}

void printInvertedStr(String str,uint16_t posx,uint16_t posy){
  printInverted(str.c_str(),posx,posy);
}

int printValuePair(const char* str,String strb,int x,int y){
    int xoff = printInverted(str,x,y);
    lcd.setCursor(x+xoff+5, y);
    lcd.print(strb.c_str());
    return xoff; // get new cursor?
}

void oled_test(uint8_t num = 0){
  // print_oled_line(msg, line, size);
  for(uint8_t i=0;i<num;i++){
    lcd.clearBuffer();
    print_oled_line("millis",0);
    print_oled_line((String)millis(),1);
    lcd.sendBuffer();
    delay(1000);
    print_oled_line("Line One",0);
    print_oled_line("Line Two",1);
    print_oled_line("Line Three",2);
    lcd.sendBuffer();
    delay(1000);
    lcd.clearBuffer();
    print_oled_line("Line One",0,2);  
    print_oled_line("Line Two",2);  
    lcd.sendBuffer();
  }
}


  // lcd.setDrawColor(1);
  // uint16_t strw = lcd.drawStr(posx+padx, posy, str); // x,y(NEGATIVE GOING!)
  // lcd.setFontMode(1); 
  // lcd.setDrawColor(2);
  // lcd.drawBox(posx, (posy-lh)+pady, strw+(padx*2),lh); // x,y(POSITIVE GOING!),w,h
  // lcd.setDrawColor(1); // restore default

  #endif