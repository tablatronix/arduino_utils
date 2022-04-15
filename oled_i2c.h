/*oled_i2c*/
#ifndef oled_i2c_h
#define oled_i2c_h

#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// #define SH1106_128_64
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET -1
// Adafruit_SSD1306 lcd(OLED_RESET);
Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT);
// Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET,800000,800000);

int oledfpsmicros = 0;

void invertText(){
  lcd.setTextColor(BLACK, WHITE); // 'inverted' text  
}

void whiteText(){
  lcd.setTextColor(WHITE); // 'inverted' text  
}

void i2cpinswap(){
    Wire.begin(SCL,SDA);  // begin(sda, scl) SWAP!  
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

void print_oled(String str,uint8_t size,bool flush){
  lcd.clearDisplay();
  lcd.setTextSize(size);
  lcd.setTextColor(WHITE);
  lcd.setCursor(0,0);
  lcd.println(str);
  if(flush) lcd.display();
}

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
  lcd.setTextSize(size);  
  lcd.println(str);
}

void init_oled(bool preamble = true){
  Serial.println("\nInitializing SSD1306 OLED");
  Serial.println("SDA: "+(String)SDA);   
  Serial.println("SCL: "+(String)SCL);

  // Wire.begin(SCL,SDA);  // begin(sda, scl) SWAP!
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }
  lcd.setRotation(2);
  // Wire.setClock(400000L);  // set i2c speed 400khz
  // Wire.setClock(100000L);  // set i2c speed 400khz

  lcd.clearDisplay();
  lcd.setTextSize(1);             // Normal 1:1 pixepl scale  
  lcd.setCursor(0,0);             // Start at top-left corner
  lcd.setTextColor(WHITE); // REQUIRED!  
  lcd.display();
  delay(1000);
  if(!preamble) return;

  lcd.setTextSize(1);
  lcd.setCursor(0,0);
  lcd.println("Booting...."); // show init
  lcd.display();
  delay(1000);
  lcd.clearDisplay(); // clear display
  lcd.display();
}

void oled_test(uint8_t num = 1){
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

void displayFPS(){
    lcd.clearDisplay();
    lcd.setTextSize(1);             // Normal 1:1 pixepl scale  
    lcd.setCursor(0,0);             // Start at top-left corner
    lcd.print((String)(10000000/((micros()-oledfpsmicros))));
    lcd.display();
    // println(" FPS");
    oledfpsmicros = micros();
}

#endif