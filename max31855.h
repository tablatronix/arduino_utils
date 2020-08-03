#ifndef max31855_h
#define max31855_h

// #include "MAX31855.h" // by Rob Tillaart Library Manager (NO HSPI!!)
#include "Adafruit_MAX31855.h" // Seperate calls for spiread for each function, no raw bit cache!
#include <ktypelinear.h>
// #include <quickstats.h> // @todo test quickstats, add child class for container
// #include <Statistics.h> // https://github.com/provideyourown/statistics
#include <ntc.h>
#include <Average.h>
#include <number_format.h>

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

bool updateLock = false;

// Averaging
// options
bool useInternal              = false; // use internal temp as TC
int avgSamples                = 10;   // 1 to disable averaging
float tempOffset              = 0; // allow temp skew
int tempSampleRate            = 1000; // how often to sample temperature when idle

// averaging vars when not using stats averaging
unsigned long nextTempRead    = 0; // NI averaging timing
unsigned long nextTempAvgRead = 0; // NI averaging timing
// clean up averaging, when using stats lib some below are not used
int avgReadCount              = 0; // running avg counter
float maxT; // highest temp in window (tempSampleRate)
float minT; // lowest temp in window (tempSampleRate)

#define USESTATS
// options
bool useAveraging = true; // use stats lib averaging
int max31855_numsamples = 10; // use avgSamples?
// Statistics stats(max31855_numsamples); // init stats for avg (samples)
// Statistics tcdev(16); // init stats for avg (samples)

bool useModeAveraging = true; // use mode for averages to reduce spurious noise, noise is usually negative value temps
// @todo change library, only supports ints atm
 
Average<float> stats(max31855_numsamples);
Average<float> tcdev(max31855_numsamples*2);

// inline Average::addData(T entry) {push(T entry);}

// Runtime reflow variables
float currentTemp    = 0; // current real temp w offset applied
float currentTempB   = 0; // alt thermocouple
float currentTempAvg = 0; // current avg temp (avgSamples) mean
float internalTemp   = 0;  // cold junction
float currentTempCorr = 0; // linearized k type
float lastTemp       = -1; // used in pid.h

int TCinterval = 200; // temp reading interval

// TC WARN
// options
bool tcWarn          = false; // stop and show error if a tc issue is detected
uint16_t numfailwarn   = 10; // how many failures in window
uint16_t numfailwindow = 10000; // ms to compare

uint8_t lastTCStatus   = 0; // last error status
uint16_t TCnumErrors     = 0;  // counter for failures

// max31855 error states
#define STATUS_OK               0x00
#define STATUS_OPEN_CIRCUIT     0x01
#define STATUS_SHORT_TO_GND     0x02
#define STATUS_SHORT_TO_VCC     0x04
#define STATUS_NOREAD           0x80

// Adafruit_MAX31855 tc(MAXCLK, MAXCS, MAXDO);
// Initialise the MAX31855 IC for thermocouple tempterature reading
// MAX31855 tc(MAXCLK, MAXCS, MAXDO);

void averages(){
    int minat = 0;
    int maxat = 0;  
    Serial.print("Mean:   "); Serial.println(stats.mean());
    Serial.print("Mode:   "); Serial.println(stats.mode());
    Serial.print("Max:    "); Serial.println(stats.maximum(&maxat));
    Serial.print(" at:    "); Serial.println(maxat);
    Serial.print("Min:    "); Serial.println(stats.minimum(&minat));
    Serial.print(" at:    "); Serial.println(minat);
    Serial.print("StdDev: "); Serial.println(stats.stddev()); 
    // Serial.print("predict: "); Serial.println(stats.predict(x)); // linear regression ?
    // Serial.print("leastSquares: "); Serial.println(stats.leastSquares(m,b,r)); 
}

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
    // use internal cj as real temp, useful for testing without tc or monitoring pcb etc
  	internalTemp = tc.readInternal();
  	currentTemp = internalTemp + tempOffset;
  }
  else {
    // sanity check
    // @todo replace with sanitycheck
    // also check for dev not changing over long periods, could be bad tc, or runaway is heat is being applied
    double readC = tc.readCelsius();
    if(isnan(readC) || readC < 0 || readC > 700){
      // Serial.println("[ERROR] TC OUT OF RANGE, skipping " + (String)readC);
      TCnumErrors++;
      return;
    }

    // offset
    currentTemp = readC + tempOffset;

    // linearized NI
    // correctKType();
    }

    // average
    if(useAveraging){
      stats.push(currentTemp);
      if(useModeAveraging){
        currentTempAvg = stats.mode();
        tcdev.push(stats.mode());
      }
      else {
        currentTempAvg = stats.mean();
        tcdev.push(currentTempAvg); // might lag, better to use currentTemp but will include more noise
      }  
      // Serial.print("[TEMP] tc ");
      // Serial.println(currentTemp);
      // Serial.print("[TEMP] tc avg ");
      // Serial.println(currentTempAvg);
    }
    else currentTempAvg = currentTemp;
}

// @todo use actual window, using a periodic reset now
void resetError(){
  TCnumErrors = 0;
}

uint16_t getTCErrorCount(){
  return TCnumErrors;
}

bool getTcHasError(){
  return lastTCStatus != STATUS_OK;
}

String getTcStatus(){
  uint8_t tcStatus = lastTCStatus;
  if(tcStatus == STATUS_OK) return "OK";
  else if(tcStatus == STATUS_OPEN_CIRCUIT) return "Open";
  else if(tcStatus == STATUS_SHORT_TO_GND) return "GND Short";
  else if(tcStatus == STATUS_SHORT_TO_VCC) return "VCC Short";
  else if(tcStatus == STATUS_NOREAD) return "Read Failed";
  else return "ERROR";
}

void updateDevVars(){
  if(currentTempAvg>maxT) maxT = currentTempAvg; // update min max
  if(currentTempAvg<minT) minT = currentTempAvg;
}

void updateAltTemps(){
  internalTemp = tc.readInternal();
  lastTCStatus = tc.readError();  
}


void setTCInterval(int intv){
  TCinterval = intv;
}

// MAIN UPDATER SCHDULER
// @todo consolidate into single cleaner functions
void updateTemps(){
    // if(updateLock) return;
    // digitalWrite(0,LOW); // fixes DC left HIGH and data clocked to max ic causing screen artifacts. 
    delay(TCinterval); // stabilizes temperatures ????
    internalTemp = tc.readInternal();
    ReadCurrentTemp();
    if(!useAveraging)updateDevVars();
    // Serial.println(currentTemp);
}

void resetDev(){
  maxT = minT = currentTempAvg;
  TCnumErrors = 0; // for now do this here
}

float readTCDev(){
  float dev = (maxT-minT);
  updateTemps();
  return dev;
}

// @todo add windowing, dev is currently tied to averageCount and samplerate
// track time stamp calls or period and track dev over time window
// eg. rate of change per second, or extrapolate
// add timer and average, then avg dev over 500ms and double
// y = m * log(x) + b
float getTCDev(){
  float dev;
  if(useAveraging){
    dev = tcdev.stddev(); // @note will return nan, if 0
    if(isnan(dev)){
      dev = 0;
      // Serial.println("[ERROR] stats dev NAN");
    }
  }
  else dev = (maxT-minT);
  return dev;
}

bool TCSanityCheck(){
  bool ret = true;
  if(TCnumErrors > numfailwarn){
    Serial.println("[ERROR] TC has read errors");    
    ret = false;
  }
  if(getTcHasError()){
    Serial.print("[ERROR] TC Error - ");
    Serial.println(getTcStatus());
    ret = false;
  }
  if((int)currentTemp > 300 || (int)currentTemp < 0) {
    Serial.println("[ERROR] TC OUT OF RANGE");
    ret = false;
  }
  return true;
}

float readFahrenheit(){
  return (currentTempAvg*1.8)+32;
}

void printTC(){
	updateTemps();
  	int tempf = readFahrenheit();
	Serial.print("[TC] TC1: " + String(( currentTempAvg ))+"ºC " + tempf + "ºF");
  	tempf = (((internalTemp)*1.8)+32);
	Serial.print(" CJ: " + String( round_f( internalTemp ) )+"ºC " + tempf + "ºF");
  Serial.print(" Dev: " + String( round_f( getTCDev() ) )+"ºC");
  Serial.print(" Raw: ");
  Serial.print(currentTemp);
  // Serial.print(" Dev: ");
  // Serial.print(stats.stdDeviation());
  // Serial.print(" TCLIN: ");
  // Serial.print(currentTempCorr);
  // Serial.print(" NTC: ");
  // Serial.print((String)(float(ntc_temp)/10));

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
  bool res = tc.begin();
  // if(!res) add check now, see commits
  delay(200);
  tc.readError();
  // check for sanity
  Serial.println("[TC] MAX31855 Thermocouple Begin...");
  if(!TCSanityCheck()) Serial.println("[ERROR] Status: "+ getTcStatus());
  
  #ifdef DEBUG
    printTC();
    // Serial.println("[TC] "+(String)round(tc.readInternal()));
    // Serial.println("[TC] "+(String)round(tc.readCelsius()));
    // Serial.println("[TC] "+(String)round(readFahrenheit()));
  #endif
  updateTemps();
  resetDev();
}

#endif