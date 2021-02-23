/*oled_i2c*/
// DOESNT WORK

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

// @todo add pinswap
void init_oled(bool preamble,bool pinswap = false){
  Serial.println("\nInitializing SSD1106 OLED");
  Serial.println("SDA: "+(String)SDA);
  Serial.println("SCL: "+(String)SCL);
  if(pinswap) Wire.begin(5,4);  // begin(sda, scl) SWAP!
  // Wire.setClock(400000L);
  lcd.setDisplayRotation(U8G2_R2);
  lcd.begin();
  // lcd.setI2CAddress(0x7A);
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
  lcd.clearDisplay();
  lcd.display();
}

void displayFPS(){
    lcd.print((String)(1000000/((micros()-oledfpsmicros)/2)));
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
    lcd.clearDisplay();
    print_oled_line("millis",0);
    print_oled_line((String)millis(),1);
    lcd.display();
    delay(1000);
    print_oled_line("Line One",0);
    print_oled_line("Line Two",1);
    print_oled_line("Line Three",2);
    lcd.display();
    delay(1000);
    lcd.clearDisplay();
    print_oled_line("Line One",0,2);  
    print_oled_line("Line Two",2);  
    lcd.display();
  }
}


  // lcd.setDrawColor(1);
  // uint16_t strw = lcd.drawStr(posx+padx, posy, str); // x,y(NEGATIVE GOING!)
  // lcd.setFontMode(1); 
  // lcd.setDrawColor(2);
  // lcd.drawBox(posx, (posy-lh)+pady, strw+(padx*2),lh); // x,y(POSITIVE GOING!),w,h
  // lcd.setDrawColor(1); // restore default