#ifndef max31855_h
#define max31855_h

// #include "MAX31855.h" // by Rob Tillaart Library Manager (NO HSPI!!)
#include "Adafruit_MAX31855.h" // Seperate calls for spiread for each function, no raw bit cache!
#include <ktypelinear.h>
// #include <quickstats.h>
#include <Statistics.h>
#include <ntc.h>

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

int tempOffset                = 0;
bool useInternal              = false;
unsigned long nextTempRead    = 0;
unsigned long nextTempAvgRead = 0;
int avgReadCount              = 0; // running avg
int avgSamples                = 10;   // 1 to disable averaging
int tempSampleRate            = 1000; // how often to sample temperature when idle

Statistics stats(6); // init stats for avg

// Runtime reflow variables
float currentTemp    = 0;
float currentTempB   = 0; // alt thermocouple
float currentTempAvg = 0;
float internalTemp   = 0;  // cold junction
float currentTempCorr = 0; // linearized k type

float lastTemp       = -1;
bool tcWarn          = false; // stop and show error if a tc issue is detected
float maxT;
float minT;

uint8_t lastTCStatus = 0;

// max31855 error states
#define STATUS_OK               0x00
#define STATUS_OPEN_CIRCUIT     0x01
#define STATUS_SHORT_TO_GND     0x02
#define STATUS_SHORT_TO_VCC     0x04
#define STATUS_NOREAD           0x80

// Adafruit_MAX31855 tc(MAXCLK, MAXCS, MAXDO);
// Initialise the MAX31855 IC for thermocouple tempterature reading
// MAX31855 tc(MAXCLK, MAXCS, MAXDO);

// Really not needed for reflow temps, fairly linear in region, makes almost no difference
float correctKType(){
  float tcmv = (currentTemp - internalTemp) * 0.041276; 
  currentTempCorr = correctedCelsius(tcmv,internalTemp);
}

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
  lastTCStatus = tc.readError();
  #ifdef DEBUG
  // Serial.print("tc status: ");
  // Serial.println( status );
  #endif
  if(useInternal){
  	internalTemp = tc.readInternal();
  	currentTemp = internalTemp + tempOffset;
  }	
  else {
    double readC = tc.readCelsius();
    if(isnan(readC) || readC < 0 || readC > 700){
      Serial.println("[ERROR] TC OUT OF RANGE, skipping " + (String)readC);
      return;
    }

    // offset
    currentTemp = readC + tempOffset;

    // average
    stats.addData(currentTemp);
    currentTempAvg = stats.mean();
    
    // linearized
    // correctKType();
  }
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

// MAIN UPDATER SCHDULER
void updateTemps(){
    read_ntc();
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

bool TCSanityCheck(){
  if((int)currentTemp > 300 || (int)currentTemp < 0) {
    Serial.println("[ERROR] TC OUT OF RANGE");
    return false;
  }
  return true;
}

float readFahrenheit(){
  return (currentTemp*1.8)+32;
}

void printTC(){
	updateTemps();
  	int tempf = readFahrenheit();
	Serial.print("[TC] TC1: " + String( round_f( currentTemp ) )+"ºC " + tempf + "ºF");
  	tempf = (((internalTemp)*1.8)+32);
	Serial.print(" CJ: " + String( round_f( internalTemp ) )+"ºC " + tempf + "ºF");
    tempf = (((getTCDev())*1.8)+32);
  Serial.print(" Dev: " + String( round_f( getTCDev() ) )+"ºC " + tempf + "ºF");
  Serial.print(" Mean: ");
  Serial.print(stats.mean());
  // Serial.print(" Std Dev: ");
  // Serial.print(stats.stdDeviation()); 
  // Serial.print(" TCLIN: ");
  // Serial.print(currentTempCorr);
  Serial.print(" NTC: ");
  Serial.print((String)(float(ntc_temp)/10));

  Serial.println("");
  TCSanityCheck();
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
  delay(200);
  tc.readError();
  #ifdef DEBUG
    Serial.println("[TC] MAX31855 Thermocouple Begin...");
    if((uint8_t)tc.readError() != STATUS_OK) Serial.println("[ERROR] Status: "+ getTcStatus());
    printTC();
    // Serial.println("[TC] "+(String)round(tc.readInternal()));
    // Serial.println("[TC] "+(String)round(tc.readCelsius()));
    // Serial.println("[TC] "+(String)round(readFahrenheit()));
  #endif
  updateTemps();
  resetDev();
}

#endif