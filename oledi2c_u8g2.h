#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1 // no reset
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SSD1306 display2(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void oled1(bool init){
  Wire.begin(D1,D2);  Wire.setClock(400000L);  // set i2c speed 400khz
  if(init)display.begin(SSD1306_SWITCHCAPVCC, 0x3c,false);  // initialize with the I2C addr 0x3D (for the 128x64)
}

void oled2(bool init){
  Wire.begin(D5,D6);  Wire.setClock(400000L);  // set i2c speed 400khz
  if(init)display2.begin(SSD1306_SWITCHCAPVCC, 0x3c,false);  // initialize with the I2C addr 0x3D (for the 128x64)
}

uint16_t curX = 0;
uint16_t curY = 0;

void flusholed1(){
	display.display();
	oled2(false);
	// display.clearDisplay();
	curX = display.getCursorX();
	curY = display.getCursorY();
	Serial.println(curX);
	Serial.println(curY);
	// display.clearDisplay();
	// display.setCursor(0,curY); //reset line
}

void flusholed2(){
	display2.display();
	oled1(false);
	// display2.clearDisplay();
}

void clearDisplays(){
	oled2(false);
	display.setCursor(0,0);
	display.clearDisplay();
	display.display();
	oled1(false);
	display2.setCursor(0,0);
	display2.clearDisplay();
	display2.display();
}

void clearLine(){
	display.setCursor(curX,curY);	
}

void testScroll(){
	for(int i = 0;i<200;i++){
		display.println(i*345345);
		flusholed1();
		display2.println(i*435234523);
		flusholed2();
		yield();
	}
	delay(0);
}

void init_oled(){
  oled1(true);
  oled2(true);

  display.setTextScroll(true);
  display2.setTextScroll(true);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display2.clearDisplay();
  display2.setTextSize(1);
  display2.setTextColor(WHITE);
  display2.setCursor(0,0);
  

  oled1(false);  
  display.println("Booting oled 1....");
  flusholed1();
  display2.println("Booting oled 2....");
  flusholed2();

  oled1(false);
  display.println("0123456789");
  flusholed1();
  display2.println("101112131415161718");
  flusholed2();
  
  // testScroll(); 

  delay(2000);
  clearDisplays();
  delay(2000);

}