#include <LiquidCrystal_I2C.h>

const int numRows  = 2;
const int numCols  = 16;
int backlightState = true;

uint8_t _SCL = 15;
uint8_t _SDA = 13;

// i2c pins nodemcu
// SCL D1
// SDA D2
LiquidCrystal_I2C lcd(0x27,numCols,numRows);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// quality icon 0-6 0x - 100%
/*
 * XXXXX
 * X   X
 *   X
 *   X  X
 *    XXX
 *  XXXXX
 * XXXXXX
 */
byte qualityIco[][8] = {
{
  0b11100,
  0b10100,
  0b01000,
  0b01000,
  0b00000,
  0b10100,
  0b01000,
  0b10100
},
{
  0b11100,
  0b10100,
  0b01000,
  0b01000,
  0b00000,
  0b00000,
  0b00000,
  0b10000
},
{
  0b11100,
  0b10100,
  0b01000,
  0b01000,
  0b00000,
  0b00000,
  0b01000,
  0b11000
},
{
  0b11100,
  0b10100,
  0b01000,
  0b01000,
  0b00000,
  0b00100,
  0b01100,
  0b11100
},
{
  0b11100,
  0b10100,
  0b01000,
  0b01000,
  0b00010,
  0b00110,
  0b01110,
  0b11110
},
{
  0b11100,
  0b10100,
  0b01000,
  0b01001,
  0b00011,
  0b00111,
  0b01111,
  0b11111
}
};



byte block4[4][8] = {
{
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
},
{
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
},
{
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111
},
{
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
}
};

byte block[8][8] = {
{
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
},
{
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
},
{
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
},
{
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
},
{
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111
},
{
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111
},
{
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111
},
{
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
}
};

// degrees F
byte degF[8] = {
  0b01000,
  0b10100,
  0b01000,
  0b00000,
  0b00011,
  0b00100,
  0b00111,
  0b00100
};

// dergees C
byte degC[8] = {
  0b01000,
  0b10100,
  0b01000,
  0b00000,
  0b00011,
  0b00100,
  0b00100,
  0b00011
};

// thermometer
byte thermico[8] = {
  0b00100,
  0b01010,
  0b01010,
  0b01110,
  0b01110,
  0b11111,
  0b11111,
  0b01110
};

// droplet
byte humico[8] = {
  0b00100,
  0b00100,
  0b01010,
  0b01010,
  0b10001,
  0b10001,
  0b10001,
  0b01110
};

// small %
byte perc[8] = {
  0b00000,
  0b00000,
  0b10010,
  0b00100,
  0b01000,
  0b10010,
  0b00000,
  0b00000
};

byte arial[8] = {
  0b11111,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};

byte wifi[8] = {
  0b01110,
  0b10001,
  0b00100,
  0b01010,
  0b00000,
  0b00100,
  0b00000,
  0b00000
};

byte db[8] = {
  0b00000,
  0b00000,
  0b11100,
  0b00000,
  0b00110,
  0b11111,
  0b10101,
  0b11111
};

byte wlink[4][8] = {
{
  0b00000,
  0b00111,
  0b11101,
  0b10111,
  0b10000,
  0b10101,
  0b11010,
  0b00101
},
{
  0b00000,
  0b00111,
  0b11101,
  0b10111,
  0b10000,
  0b10111,
  0b11101,
  0b00111
},
{
  0b11100,
  0b10111,
  0b11101,
  0b00000,
  0b00000,
  0b11101,
  0b10111,
  0b11100
},
{
  0b10100,
  0b01011,
  0b10101,
  0b00001,
  0b00001,
  0b11101,
  0b10111,
  0b11100
} 
};


const uint8_t WIFIQICO    = 0;
const uint8_t ARIALICO    = 1;
const uint8_t WIFIDBICO   = 32; // space
const uint8_t WIFILINKICO = 2; // space
const uint8_t GRAPHICO    = 2;

// dynamic icon
uint8_t getQualityIcon(int rssi){
  if(rssi >=0 || rssi<-96) rssi = 0; // -96 unusable signal, +int invalid rssi
  else rssi = map(rssi,-1,-100,5,1);
  Serial.println("quality: " + (String)rssi);
  lcd.createChar(0,qualityIco[rssi]);
  return 0;
}
// dynamic icon
uint8_t getConnectIcon(int connState){
  return 32;
  // Serial.println("connect: " + (String)connState);
  lcd.createChar(WIFILINKICO,wlink[connState]);
  return WIFILINKICO;
}

void initLcdChars(){
  lcd.createChar(WIFIQICO, qualityIco[0]);
  lcd.createChar(ARIALICO, arial);
  lcd.createChar(WIFILINKICO, wlink[0]);
  lcd.createChar(WIFIDBICO, db);
}

uint8_t getGraphIcon(int in){
	int value = map(in,100,0,GRAPHICO,GRAPHICO+5);
	if(value == GRAPHICO) return 255; // 0
	return value;
}

void initLcdGraphChars(){
  // lcd.createChar(GRAPHICO,   block[1]); // char 255
  lcd.createChar(GRAPHICO+1, block[2]); 
  lcd.createChar(GRAPHICO+2, block[3]);
  lcd.createChar(GRAPHICO+3, block[4]);
  lcd.createChar(GRAPHICO+4, block[5]);
  lcd.createChar(GRAPHICO+5, block[6]);
  // lcd.createChar(range+6, block[3]);
}

void initLCD(){
  // Wire.begin(_SDA,_SCL);
  Wire.begin();
  Wire.setClock(1700000L);  // set i2c speed 400khz
  lcd.init();              // initialize the lcd

  lcd.noBacklight();
  delay(1000);
  lcd.backlight();

  if(backlightState) lcd.backlight();
  else lcd.noBacklight();

  lcd.clear();
  delay(500);
  lcd.print("Booting......");

  // create a new custom character
  initLcdChars();
  initLcdGraphChars();
}