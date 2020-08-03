/*oled_i2c*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// #define SH1106_128_64
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1
// Adafruit_SSD1306 lcd(OLED_RESET);
Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET,800000,800000);

int fpsmicros = 0;

// #if (SSD1306_LCDHEIGHT != 64)
// #error("Height incorrect, please fix Adafruit_SSD1306.h!");
// #endif

void invertText(){
  lcd.setTextColor(BLACK, WHITE); // 'inverted' text  
}

void whiteText(){
  lcd.setTextColor(WHITE); // 'inverted' text  
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
  if(flush)lcd.display();
}

void init_oled(bool preamble){
  Serial.println("\nInitializing SSD1306 OLED");
  Serial.println("SDA: "+(String)SDA);   
  Serial.println("SCL: "+(String)SCL);

  Wire.begin(SCL,SDA);  // begin(sda, scl) SWAP!
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }

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
  lcd.println("Booting....");
  lcd.display();
  delay(1000);
  lcd.clearDisplay();
  lcd.display();
}

void displayFPS(){
    lcd.print((String)(1000000/((micros()-fpsmicros)/2)));
    // println(" FPS");
    fpsmicros = micros();
}