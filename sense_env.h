#ifndef sensors_h
#define sensors_h

#include <Average.h>

Average<float> avg_a(10);

// supported sensors
// SHT31
// SHT21
// HTU21D
// 
// BMP280
// BME280
// CS811
// GP2Y
// TSL2561
// BH1750
// 

// BUGS
// sensors do not reinit is they drop out
// co2 voc, resubmits the same value over and over if device is lost
// use real temp and humidity to compensate other sensors

#include <Wire.h>
#include <log.h>

// I2C
#define USESHT31 // SHT31  Temp/Humidity
// #define USESHT21 // SHT21 / HTU21D Temp/Humidity
// #define USEBMP180 // BMP180 Temp/Pressure/Altitude (replaces BMP085) https://www.adafruit.com/product/1603
/*
Vin: 3 to 5VDC
Logic: 3 to 5V compliant
Pressure sensing range: 300-1100 hPa (9000m to -500m above sea level)
Up to 0.03hPa / 0.25m resolution
-40 to +85°C operational range, +-2°C temperature accuracy
This board/chip uses I2C 7-bit address 0x77.
*/

#define USEBMP280 // BMP280 Temp/Pressure/Altitude (upgrade for BMP085/BMP180/BMP183)
// #define USEBME280 // BME280 Humidity/Pressure/Altitude
// Pressure: 300...1100 hPa

#define USECS811  // CCS811 Temp/CO2/VOC
#define USEGP2Y   // Sharp Particle/Dust sensor GP2Y1010AU0F, GP2Y1014AU0F
#define APDS9960  // Proximity, Light, RGB, and Gesture Sensor

#define BH1750    // Light Sensor

#define TSL2561   // Luminosity/Lux/Light Address = 0x39 //Slave addr also 0x29 or 0x49
/*
TSL2561
Approximates Human eye Response
Precisely Measures Illuminance in Diverse Lighting Conditions
Temperature range: -30 to 80 *C
Dynamic range (Lux): 0.1 to 40,000 Lux
Voltage range: 2.7-3.6V
Interface: I2C

TSL2591 ++
Approximates Human eye Response
Extremely wide dynamic range 1 to 600,000,000 Counts
Lux Range: 188 uLux sensitivity, up to 88,000 Lux input measurements.
Temperature range: -30 to 80 *C
Voltage range: 3.3-5V into onboard regulator
Interface: I2C
*/

// [I2C] Device found - ADDR: 0x23 // BH1750
// [I2C] Device found - ADDR: 0x39 // APDS9960 / TSL2561
// [I2C] Device found - ADDR: 0x5A // 
// [I2C] Device found - ADDR: 0x76 // 

/*
#ifdef ENV_TEMPLATE
#include <Genv_asset.h>

void init_env(){
}

void print_env(){
}

float get_env(uint8_t channel = 0){
  // print_env();
  if(channel == 0) return ;
  if(channel == 1) return ;
  if(channel == 2) return ;

#endif
*/

/*
#ifdef ENV_TEMPLATE
#include <Genv_asset.h>

bool init_env(){
}

void print_env(){
}

float get_env(uint8_t channel = 0){
  // print_env();
  if(channel == 0) return ;
  if(channel == 1) return ;
  if(channel == 2) return ;

#endif
*/

#ifdef BH1750
#include <hp_BH1750.h>  //inlude the library
hp_BH1750 env_BH1750;

// BH1750Address addr = BH1750_TO_VCC;
// BH1750Address BH1750addr = BH1750_TO_GROUND;
// 0x23/0x5A , or 0X5C
// BH1750_TO_GROUND = 0x23,
// BH1750_TO_VCC = 0x5C

bool init_bh1750(){
  Logger.println("[ENV] hp_BH175 init");
  bool status = env_BH1750.begin(BH1750_TO_GROUND);   // will be false no sensor found
                                            // use BH1750_TO_GROUND or BH1750_TO_VCC depending how you wired the address pin of the sensor.
  
  // BH1750.calibrateTiming();  //uncomment this line if you want to speed up your sensor
  env_BH1750.start();
  // BH1750.start(BH1750_QUALITY_HIGH2, mtreg);
  // BH1750.setQuality(mode);
  
  if(!status) Logger.println("[ERROR] bh1750 failed to initialize device! Please check your wiring.");
  else Logger.println("[ENV] bh1750 Device initialized!");  
  return status;
}

void print_bh1750(){
   if (env_BH1750.hasValue() == true) {    // non blocking reading
    float lux = env_BH1750.getLux();
    Logger.println(lux);
    env_BH1750.start();
  } 
}

float get_bh1750(uint8_t channel = 0){
  float lux;
  if(channel == 0){
    if (env_BH1750.hasValue() == true) {    // non blocking reading
     lux = env_BH1750.getLux();
     env_BH1750.start();
     Logger.println(lux);
    }
  }
  else{
    env_BH1750.start();   //starts a measurement
    lux=env_BH1750.getLux();
    Logger.println(lux);
  }
  return lux;
}
#endif


#ifdef APDS9960

#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds;
uint8_t apds_int_pin = -1;

bool init_apds(){
  bool ret = false;
  ret = apds.begin();
  if(!ret){
    Logger.println("[ERROR] APDS9960 init failed");
  }
  else Logger.println("[ENV] APDS9960 initialized!");
  if(apds_int_pin > 0)  pinMode(apds_int_pin, INPUT_PULLUP);
  return ret;
}

void init_apds_color(){
  //enable color sensign mode
  apds.enableColor(true);  
}

void init_apds_proximity(){
  if(apds_int_pin > 0)  pinMode(apds_int_pin, INPUT_PULLUP);
  //enable proximity mode
  apds.enableProximity(true);
  //set the interrupt threshold to fire when proximity reading goes above 175
  apds.setProximityInterruptThreshold(0, 175);
  //enable the proximity interrupt
  apds.enableProximityInterrupt();
}

void init_apds_gesture(){
  apds.enableProximity(true);
  apds.enableGesture(true);  
}

// NOT interrupt
String get_apds_proximity(){
  if(!digitalRead(apds_int_pin)){
    Logger.println(apds.readProximity());

    //clear the interrupt
    apds.clearInterrupt();
  }
}

String get_apds_gesture(){
    uint8_t gesture = apds.readGesture();
    if(gesture == APDS9960_DOWN) Logger.println("v");
    if(gesture == APDS9960_UP) Logger.println("^");
    if(gesture == APDS9960_LEFT) Logger.println("<");
    if(gesture == APDS9960_RIGHT) Logger.println(">");
    return "";
}

void print_apds_color(){
  uint16_t r, g, b, c;
  
  //wait for color data to be ready
  while(!apds.colorDataReady()){
    delay(5);
  }

  //get the data and print the different channels
  apds.getColorData(&r, &g, &b, &c);
  Logger.print("red: ");
  Logger.print(r);
  
  Logger.print(" green: ");
  Logger.print(g);
  
  Logger.print(" blue: ");
  Logger.print(b);
  
  Logger.print(" clear: ");
  Logger.println(c);
  Logger.println(); 
}

float get_apds_color(uint8_t channel = 0){
  // print_apds_color();
  uint16_t r, g, b, c;
  if(!apds.colorDataReady()) return 0;
  apds.getColorData(&r, &g, &b, &c);
  if(channel == 0) return r;
  if(channel == 1) return g;
  if(channel == 2) return b;
  if(channel == 3) return c;
  return 0;
}

#endif


#ifdef USEGP2Y
#include <GP2YDustSensor.h>
const uint8_t SHARP_LED_PIN = 27;   // Sharp Dust/particle sensor Led Pin
const uint8_t SHARP_VO_PIN  = 33;    // Sharp Dust/particle analog out pin used for reading 
uint16_t gp2y_avgnumsamples = 10;   // running average samples
uint16_t gp2y_numsamples    = 5;    // num samples to return

// GP2Y1010AU0F
// GP2Y1014AU0F
GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1014AU0F, SHARP_LED_PIN, SHARP_VO_PIN, gp2y_avgnumsamples);
#endif

#ifdef USEGP2Y
void init_gp2y(){
  dustSensor.setBaseline(0.1); // set no dust voltage according to your own experiments
  //dustSensor.setCalibrationFactor(1.1); // calibrate against precision instrument
  dustSensor.begin();

   // * Set sensitivity in volts/100ug/m3
   // * Typical sensitivity is 0.5V, set by default
   // * GP2Y1010AU0F sensitivity: min/typ/max: 0.425 / 0.5 / 0.75
   // * GP2Y1014AU0F sensitivity: min/typ/max: 0.35 / 0.5 / 0.65 
  // dustSensor.setSensitivity();
  
  // getBaselineCandidate();
  // setBaseline();
}

float get_gp2y(uint8_t channel = 0){
  // print_bmp280();
  if(channel == 0) return dustSensor.getDustDensity(gp2y_numsamples);
  if(channel == 1) return dustSensor.getRunningAverage();
  if(channel == 2) return analogRead(SHARP_VO_PIN);
}

String print_gp2y(){
  // Logger.print("Dust density: ");
  Logger.print(dustSensor.getDustDensity(gp2y_numsamples));
  // Logger.print(" ug/m3; Running average: ");
  Logger.print("\t");
  Logger.print(dustSensor.getRunningAverage());
  // Logger.println(" ug/m3");
  Logger.print("\t");
  Logger.print(analogRead(SHARP_VO_PIN));
  Logger.println("");
}
#endif

#ifdef USELM75
#include <LM75A.h>
// Create I2C LM75A instance
LM75A lm75(true,  //A0 LM75A pin state
                   true,  //A1 LM75A pin state
                   true); //A2 LM75A pin state
#endif

// SHT21 / HTU21D Temp/Humidity
#ifdef USESHT21
#include <HTU21D.h>
/*
resolution:
HTU21D_RES_RH12_TEMP14 - RH: 12Bit, Temperature: 14Bit, by default
HTU21D_RES_RH8_TEMP12  - RH: 8Bit,  Temperature: 12Bit
HTU21D_RES_RH10_TEMP13 - RH: 10Bit, Temperature: 13Bit
HTU21D_RES_RH11_TEMP11 - RH: 11Bit, Temperature: 11Bit
*/
HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14); // 0x40
#endif

// SHT31  Temp/Humidity
#ifdef USESHT31
#include <Adafruit_SHT31.h>
Adafruit_SHT31 sht31 = Adafruit_SHT31();
#endif

// BMP280 Temp/Pressure/Altitude
#ifdef USEBMP280
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // I2C
#endif

#ifdef USEBMP280
void init_bmp280(){
   if (!bmp.begin(BMP280_ADDRESS_ALT)) {
    Logger.println(F("[ERROR] Could not find a valid BMP280 sensor, check wiring!"));
  }
  else Logger.println(F("[ENV] BMP280 sensor is active")); 

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */ 
}

void print_bmp280(){
    Logger.print(F("Status"));
    Logger.println((String)bmp.getStatus());
    Logger.print(F("Temperature = "));
    Logger.print(bmp.readTemperature());
    Logger.println(" *C");

    Logger.print(F("Pressure = "));
    Logger.print(bmp.readPressure());
    Logger.println(" Pa");

    Logger.print(F("Approx altitude = "));
    Logger.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Logger.println(" m");

    Logger.println();  
}

// BUGGY returns bad values not null, or noint
float get_bmp280(uint8_t channel = 0){
  // print_bmp280();
  if(channel == 0) return bmp.readTemperature();
  if(channel == 1) return bmp.readPressure();
  if(channel == 2) return bmp.readAltitude(1013.25);
}

#endif  

#ifdef USESHT31
bool enableHeater = false;
uint8_t loopCnt = 0;

void init_sht31(){
  bool init = sht31.begin(0x44);
  if(init){
      Logger.println(F("[ENV] SHT31 sensor is active")); 
  }
  else
  {
      Logger.println(F("[ERROR] SHT31 init failed")); 
  }

  Logger.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())  Logger.println("ENABLED");
  else Logger.println("DISABLED");
}

String getSHT31Temperature(){
  float t = sht31.readTemperature();
  if(isnan(t)) return "";
  return (String) t;
}

String getSHT31Humidity(){
  float h = sht31.readHumidity();
  if(isnan(h)) return "";
  return (String) h;
}

void print_sht31(){
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (! isnan(t)) {  // check if 'is not a number'
    Logger.print("Temp *C = "); Logger.print(t); Logger.print("\t\t");
  } else { 
    Logger.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Logger.print("Hum. % = "); Logger.println(h);
  } else { 
    Logger.println("Failed to read humidity");
  }
}

void sht31_process(){
  // Toggle heater enabled state every 30 seconds
  // An ~3.0 degC temperature increase can be noted when heater is enabled
  if (++loopCnt == 30) {
    enableHeater = !enableHeater;
    sht31.heater(enableHeater);
    Logger.print("Heater Enabled State: ");
    if (sht31.isHeaterEnabled())
      Logger.println("ENABLED");
    else
      Logger.println("DISABLED");

    loopCnt = 0;
  }
}
#endif

#ifdef USESHT21
void init_sht21(){
  bool init = myHTU21D.begin();
  if(init){
      Logger.println(F("[ENV] HTU21D, SHT21 sensor initialized")); 
  }
  else
  {
      Logger.println(F("[ERROR] HTU21D, SHT21 sensor is failed or not connected")); //(F()) saves string to flash & keeps dynamic memory free
  }

  delay(1000);
  init = myHTU21D.begin();
  // while (myHTU21D.begin(SCL,SDA) != true)
  // {
  //   Logger.println(F("HTU21D, SHT21 sensor is failed or not connected")); //(F()) saves string to flash & keeps dynamic memory free
  //   delay(5000);
  // }
  // Logger.println(F("HTU21D, SHT21 sensor is active")); 
}

String getSHT21Humidity(){
  return (String)myHTU21D.readCompensatedHumidity();
}

void print_sht21(){
    /* DEMO - 1 */
  Logger.println(F("DEMO 1: 12-Bit Resolution"));
  Logger.print(F("Humidity............: ")); Logger.print(myHTU21D.readHumidity());            Logger.println(F(" +-2%"));
  Logger.print(F("Compensated Humidity: ")); Logger.print(myHTU21D.readCompensatedHumidity()); Logger.println(F(" +-2%"));

  Logger.println(F("DEMO 1: 14-Bit Resolution")); 
  Logger.print(F("Temperature.........: ")); Logger.print(myHTU21D.readTemperature()); Logger.println(F(" +-0.3C"));

 
  /* DEMO - 2 */
  Logger.println(F("DEMO 2: 11-Bit Resolution"));
  myHTU21D.setResolution(HTU21D_RES_RH11_TEMP11);
  Logger.print(F("Humidity............: ")); Logger.print(myHTU21D.readHumidity());            Logger.println(F(" +-2%"));
  Logger.print(F("Compensated Humidity: ")); Logger.print(myHTU21D.readCompensatedHumidity()); Logger.println(F(" +-2%"));

  Logger.println(F("DEMO 2: 11-Bit Resolution"));
  Logger.print(F("Temperature.........: ")); Logger.print(myHTU21D.readTemperature()); Logger.println(F(" +-0.3C"));


  /* DEMO - 3 */
  Logger.println(F("DEMO 3: Battery Status"));
  if   (myHTU21D.batteryStatus() == true) Logger.println(F("Battery.............: OK.  Level > 2.25v"));
  else                                    Logger.println(F("Battery.............: LOW. Level < 2.25v"));


  /* DEMO - 4 */
  Logger.println(F("DEMO 4:"));
  Logger.print(F("Firmware version....: ")); Logger.println(myHTU21D.readFirmwareVersion());


  /* DEMO - 5 */
  Logger.println(F("DEMO 5:"));
  Logger.print(F("Sensor's ID.........: ")); Logger.println(myHTU21D.readDeviceID());


  /* back to lib. default resolution */
  myHTU21D.softReset();
  myHTU21D.setResolution(HTU21D_RES_RH12_TEMP14);
}
#endif

#ifdef USELM75
void init_LM75(){
}

void print_LM75(){
     Logger.println((String)lm75.getTemperatureInFahrenheit() + " F"); 
}
#endif

// *************************************************************
// CCS811 Temp/CO2/VOC
#ifdef USECS811
#include "Adafruit_CCS811.h"
Adafruit_CCS811 ccs;

void init_cs811(){
  Logger.println("[ENV] cs811 init");
  if(!ccs.begin()){
    Logger.println("[ERROR] CS811 Begin Failed");
  }
  else {
    //calibrate temperature sensor
      while(!ccs.available() && millis() < 30000);
      float temp = ccs.calculateTemperature();
      Logger.println("[ENV] CS811 set offset " + String(temp-25.0));
      ccs.setTempOffset(temp - 25.0);
  }
  // ccs.setDriveMode(uint8_t mode);
  ccs.readData();
}

void print_cs811(){
  if(ccs.available()){
    float temp = ccs.calculateTemperature();
    if(!ccs.readData()){
      Logger.print("CO2: ");
      Logger.print(ccs.geteCO2());
      Logger.print("ppm, TVOC: ");
      Logger.print(ccs.getTVOC());
      Logger.print("ppb   Temp:");
      Logger.println(temp);
    }
  }
  else{
    Logger.println("[ERROR] cs811 not available"); //
  }  
}

// first call usually fails?
float get_cs811(uint8_t channel = 0){
  // print_cs811();
  // if(!ccs.available()) return 0;
  // if(ccs.readData()) return 0;
  // @todo detect failures, or else func return old values
  // if(!ccs.available()) Logger.println("[ERROR] cs811 not available"); // always false?
  if(ccs.checkError()) Logger.println("[ERROR] Assert cs811 check error"); // always false?
  uint8_t err = ccs.readData();
  float ret;
  if(err != 0) Logger.println("[ERROR] cs811 readData " + (String)err);
  if(channel == 0) ret =  ccs.calculateTemperature();
  if(channel == 1) ret =  ccs.geteCO2();
  if(channel == 2) ret =  ccs.getTVOC();
  if(channel == 3) ret =  ccs.getCurrentSelected();
  return ret;
}
#endif


/**
 * [filterSensor description]
 * @param  {[type]} float n             [description]
 * @param  {[type]} a     low limit
 * @param  {[type]} b     high limit
 * @param  {String} c     replace
 * @return {[type]}       [description]
 */
String filterSensor( float n, float a,float b,String c = ""){
  if(n <= a ) return c;
  if(n >= b ) return c;
  return (String)n;
}


// #define USEINTVCC
#ifdef USEINTVCC
#ifdef ESP32
  extern "C" int rom_phy_get_vdd33();
#endif

float getVoltage(){
  #ifdef ESP32
    float voltage = ((float)rom_phy_get_vdd33()) / 1000;
    return voltage;
  #endif  
}
#endif

#endif