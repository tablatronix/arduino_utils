#ifndef max31855_h
#define max31855_h

// #include "MAX31855.h" // by Rob Tillaart Library Manager (NO HSPI!!)
#include "Adafruit_MAX31855.h" // Seperate calls for spiread for each function, no raw bit cache!

// NODEMCU hspi
// HW scl      D5 14
// HW mosi/sda D7 13
// HW miso     D6 12
// HW cs       D8 15

// #define MAXDO   5 // HWMISO
#define MAXCS   15 // 2
// #define MAXCLK  4 // HWSLK 
Adafruit_MAX31855 tc(MAXCS);

#ifdef TC2
Adafruit_MAX31855 tcB(1);
#endif

int tempOffset = 0;
bool useInternal = false;
unsigned long nextTempRead = 0;
unsigned long nextTempAvgRead = 0;
int avgReadCount = 0; // running avg
int avgSamples = 4;   // 1 to disable averaging
int tempSampleRate = 1000; // how often to sample temperature when idle

// Runtime reflow variables
float currentTemp = 0;
float currentTempB = 0; // alt thermocouple
float currentTempAvg = 0;
float internalTemp = 0;

float lastTemp = -1;
bool tcWarn = false; // stop and show error if a tc issue is detected
float maxT;
float minT;

#define STATUS_OK               0x00
#define STATUS_OPEN_CIRCUIT     0x01
#define STATUS_SHORT_TO_GND     0x02
#define STATUS_SHORT_TO_VCC     0x04
#define STATUS_NOREAD           0x80

// Adafruit_MAX31855 tc(MAXCLK, MAXCS, MAXDO);
// Initialise the MAX31855 IC for thermocouple tempterature reading
// MAX31855 tc(MAXCLK, MAXCS, MAXDO);

// @todo better average library, lerp etc
void ReadCurrentTempAvg()
{
  int status = tc.readError();  
  float internal = tc.readInternal();
  if(useInternal) currentTempAvg += internal + tempOffset;
  else currentTempAvg += tc.readCelsius() + tempOffset;
  avgReadCount++;

  #ifdef DEBUG
  // Serial.print(" avgtot: ");
  // Serial.print( currentTempAvg );
  // Serial.print(" avg count: ");
  // Serial.println( avgReadCount );
  // Serial.print(" avg: ");
  // Serial.println( currentTempAvg/avgReadCount );  
  #endif
}

// Read the temp probe
void ReadCurrentTemp()
{
  int status = tc.readError();
  #ifdef DEBUG
  // Serial.print("tc status: ");
  // Serial.println( status );
  #endif
  if(useInternal){
  	internalTemp = tc.readInternal();
  	currentTemp = internalTemp + tempOffset;
  }	
  else currentTemp = tc.readCelsius() + tempOffset;
}

String getTcStatus(){
  // tc.read();
  uint8_t tcStatus = tc.readError();
  if(tcStatus == STATUS_OK) return "OK";
  else if(tcStatus == STATUS_OPEN_CIRCUIT) return "Open";
  else if(tcStatus == STATUS_SHORT_TO_GND) return "GND Short";
  else if(tcStatus == STATUS_SHORT_TO_VCC) return "VCC Short";
  else if(tcStatus == STATUS_NOREAD) return "Read Failed";
  else return "ERROR";
}

void resetDev(){
  maxT = minT = currentTemp;  
}

void updateTemps(){
	// @todo use timer instead, actually itmust be in sync with main poll, until I have a circular buffer
    if ( nextTempRead < millis() ) // we only read the probe every second
    {
      nextTempRead = millis() + tempSampleRate;
      resetDev();
    }
    
    internalTemp = tc.readInternal(); 
    ReadCurrentTemp();
	  // Serial.println("deviation: " + (String)(maxT-minT));
    if(currentTemp>maxT) maxT = currentTemp;
    if(currentTemp<minT) minT = currentTemp;
    // Serial.println(minT);
    // Serial.println(maxT);
}

float readTCDev(){
  float dev = (maxT-minT);
  updateTemps();
  return dev;
}

float getTCDev(){
  return (maxT-minT);
}

void printTC(){
	updateTemps();
  	int tempf = tc.readFahrenheit();
	Serial.println("[TC] Thermocouple:  " + String( round_f( currentTemp ) )+"ºC " + tempf + "ºF");
  	tempf = (((internalTemp)*1.8)+32);
	Serial.println("[TC] Cold Junction:" + String( round_f( internalTemp ) )+"ºC " + tempf + "ºF");
}

// color code temperature
/**
int hotTemp  = 80; // C burn temperature for HOT indication, 0=disable
int coolTemp = 50; // C burn temperature for HOT indication, 0=disable
int shutDownTemp = 210; // degrees C

if(currentTemp > hotTemp) tft.setTextColor( RED, BLACK );
else if(currentTemp < coolTemp) tft.setTextColor( GREEN, BLACK );
else tft.setTextColor( YELLOW, BLACK );
**/

void initTC(){
  // Start up the MAX31855
  // @todo sanity
  tc.begin();
  tc.readError();
  #ifdef DEBUG
    Serial.println("[TC] MAX31855 Thermocouple Begin...");
    if((uint8_t)tc.readError() != STATUS_OK) Serial.println("[ERROR] Status: "+ getTcStatus());
    printTC();
    // Serial.println("[TC] "+(String)round(tc.readInternal()));
    // Serial.println("[TC] "+(String)round(tc.readCelsius()));
    // Serial.println("[TC] "+(String)round(tc.readFahrenheit()));
  #endif
  updateTemps();
  resetDev();
}

#endif