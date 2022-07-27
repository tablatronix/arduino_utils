#ifndef sensors_h
#define sensors_h

#include <Average.h>

Average<float> avg_a(20);

// @todo
// add global env_debug
// add local status for each sensor, avoid sending bad values if init failed

// SUPPORTED SENSORS
// SHT31
// SHT21
// HTU21D
// 
// BMP280
// BME280
// CS811
// TSL2561
// BH1750  https://github.com/claws/BH1750
// APDS9960
// GP2Y
// PM Dust sensors https://github.com/avaldebe/PMserial
// MPU6050
// PCF8591
// scd4x C02 sensor
// VEML6070 // UV

// NOT IMPLEMENTED @TODO
// HMC5883L
// MAX9814
// MCP3421
// INA219
// MCP4725
// MCP3421

// BUGS
// sensors do not reinit is they drop out, add heathcheck()
// co2 voc, resubmits the same value over and over if device is lost
// use real temp and humidity to compensate other sensors

#include <Wire.h>
#include <log.h>

// I2C

//MOTION
#define MPU6050   // MPU 6050 GY521 3axis gyro/accel
#define HMC5883L  // NI Honeywell HMC5883L


// TEMP/HUMIDITY/GAS
#define SI7021
#define AHT10

// #define USESHT31 // SHT31  Temp/Humidity
#define USESHT21 // SHT21 / HTU21D Temp/Humidity

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

#define SDC4X     // SDC40 Co2/Temp/Humidity
#define USECS811  // CCS811 Temp/CO2/VOC
// #define USEGP2Y   // Sharp Particle/Dust sensor GP2Y1010AU0F, GP2Y1014AU0F
#define PMSx

// LIGHT
#define APDS9960  // Proximity, Light, RGB, and Gesture Sensor

#define USEBH1750    // Light Sensor
/*
  BH1750 has six different measurement modes. They are divided in two groups;
  continuous and one-time measurements. In continuous mode, sensor continuously
  measures lightness value. In one-time mode the sensor makes only one
  measurement and then goes into Power Down mode.

  Each mode, has three different precisions:

    - Low Resolution Mode - (4 lx precision, 16ms measurement time)
    - High Resolution Mode - (1 lx precision, 120ms measurement time)
    - High Resolution Mode 2 - (0.5 lx precision, 120ms measurement time)

  By default, the library uses Continuous High Resolution Mode, but you can
  set any other mode, by passing it to BH1750.begin() or BH1750.configure()
  functions.

  [!] Remember, if you use One-Time mode, your sensor will go to Power Down
  mode each time, when it completes a measurement and you've read it.

  Full mode list:

    BH1750_CONTINUOUS_LOW_RES_MODE
    BH1750_CONTINUOUS_HIGH_RES_MODE (default)
    BH1750_CONTINUOUS_HIGH_RES_MODE_2

    BH1750_ONE_TIME_LOW_RES_MODE
    BH1750_ONE_TIME_HIGH_RES_MODE
    BH1750_ONE_TIME_HIGH_RES_MODE_2
*/

#define VEML6070  // UV Sensor


// #define TSL2561   // Luminosity/Lux/Light Address = 0x39 //Slave addr also 0x29 or 0x49
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

// SOUND
#define MAX9814 // MAX9814 Auto GC amplifier
#define MAX4466 // MAX4466 Adj GC amplifier


// Energy
#define INA219  // INA219 current sense

// IO
#define MCP4725 // 12bit DAC with EEPROM
#define MCP3421 // 18bit delta-sigma ADC
// #define PCF8591 // PCF8591 io expander


// 0x23 0x46 BH1750
// 0x38 0x70
// 0x39 0x72 APDS9960 / TSL2561
// 0x3C 0x78
// 0x44 0x88
// 0x48 0x90
// 0x5A 0xB4 CS811
// 0x62 0xC4 SCD4X
// 0x68 0xD0
// 0x76 0xEC

/*
#ifdef ENV_TEMPLATE
#include <Genv_asset.h>

void init_env(){
  bool ret = false;
  ret = env.begin();
  if(!ret){
    Logger.println("[ERROR] _env init FAILED");
  }
  else Logger.println("[ENV] _env is ACTIVE");
  // return ret;  
}

void print_env(){
}

float get_env(uint8_t channel = 0){
  // print_env();
  if(channel == 0) return _env.readvalue();
  // if(channel == 1) return ;
  // if(channel == 2) return ;
}
#endif
*/

// ADC Sensors
// ADC I2C

// ESP32 ADC

// MCP3421
// 

/*
ANALOG PRESSURE SENDER
--30 psi
Input: 0-30 psi
Output: 0.5V~4.5V linear voltage output. 0 psi outputs 0.5V, 15psi outputs 2.5V, 30 psi outputs 4.5V.
Accuracy: within 2% of reading (full scale).
Thread: 1/8"-27 NPT.
Wiring Connector: Water sealed quick disconnect. Mating connector is included.
Wiring: Red for +5V; Black for ground; Blue for signal output.
*/


//ONEWIRE

#ifdef DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>
// The DallasTemperature library can do all this work for you!
// http://milesburton.com/Dallas_Temperature_Control_Library
#define _DS18B20Pin 14
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);


// url=https://github.com/milesburton/Arduino-Temperature-Control-Library
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature _DS18B20(&oneWire);
// DallasTemperature _DS18B20[oneWireCount];

uint8_t _DS18B20_num_sensors = 3;
DeviceAddress _DS18B20Addr[_DS18B20_num_sensors];

void init_DS18B20(){
  bool ret = false;

  // oneWire.setPin(_DS18B20Pin);
  _DS18B20.setOneWire(_DS18B20Pin);
  ret = _DS18B20.begin();

  if(!ret){
    Logger.println("[ERROR] _DS18B20 init FAILED");
  }
  else Logger.println("[ENV] _DS18B20 is ACTIVE");
  // return ret;

  //oneWire.reset_search();

  for(size_t i=0; i<_DS18B20_num_sensors; i++){
    _DS18B20.getAddress(_DS18B20Addr[i], i);    // get address
    _DS18B20.setResolution(_DS18B20Addr[i], 9); // set precision
    // _DS18B20.getResolution(_DS18B20Addr[i], DEC);
  }

}

void print_DS18B20(){
  float tempC = _DS18B20.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }
}

float get_DS18B20(uint8_t channel = 0){
  _DS18B20.requestTemperatures();
  // for (int i = 0; i < oneWireCount; i++) {
  //   sensor[i].requestTemperatures();
  // }
  // print_env();
  if(channel == 0) return _DS18B20.getTempCByIndex(0);
  if(channel == 1) return _DS18B20.getTempCByIndex(1);
  if(channel == 2) return _DS18B20.getTempCByIndex(2);
  // if(channel == 1) return ;
  // if(channel == 2) return ;
}
#endif


#ifdef VEML6070

// PRODUCT SUMMARY
// PART NUMBER
// OPERATING VOLTAGE RANGE
// (V)2.7 to 5.5 
// I2C BUS VOLTAGE RANGE
// (V) 1.7 to 5.5 
// PEAK SENSITIVITY
// (nm) 355 
// RANGE OF SPECTRAL BANDWIDTH λ0.5
// (nm) ± 20 
// OUTPUT CODE 16 bit, I2C
// VEML6070 

// Slave Address and Function Description
// The VEML6070 has one slave address used for write functions (command) and two slave addresses used for read functions
// (UV data LSB and MSB).
// The 7-bit address for write functions is 38h = 0111000x resulting in a 70h = 01110000 8-bit address. The 7-bit addresses
// for read functions are 38h = 0111000x for the UV Data LSB and 39h = 0111001x for the UV data MSB. This results in a
// 71h = 01110001 and 73h = 01110011 8-bit address, respectively. The 7-bit address 39h should not be used for a write function.

#include "Adafruit_VEML6070.h"

Adafruit_VEML6070 env_veml6070 = Adafruit_VEML6070();

void init_veml6070(){
  bool ret = true; // NI
  env_veml6070.begin(VEML6070_1_T);  // pass in the integration time constant
  if(!ret){
    Logger.println("[ERROR] _veml6070 init FAILED");
  }
  else Logger.println("[ENV] _veml6070 is ~ACTIVE");
  // return ret;  
}

void print_veml6070(){
}

float get_veml6070(uint8_t channel = 0){
  // print_env();
  if(channel == 0) return env_veml6070.readUV();
  return 0;
}
#endif


#ifdef PMSx // particulate matter / DUST sensor

#ifndef ESP32
#include <SoftwareSerial.h>
#endif

#include <PMserial.h> // Arduino library for PM sensors with serial interface

bool pms_debug = false;
#if !defined(PMS_RX) && !defined(PMS_TX)
constexpr auto PMS_RX = 14;
constexpr auto PMS_TX = 17;
#endif

#ifndef ESP32
SerialPM pms(PMS5003, PMS_RX, PMS_TX); // PMSx003, RX, TX

// Alternative:
//SoftwareSerial SoftSerial1(PMS_RX, PMS_TX);
//SerialPM pms(PMS5003, SoftSerial1);
#else
SerialPM pms(PMS5003, PMS_RX, PMS_TX); // PMSx003, RX, TX
#endif

bool init_pms(){
  // Logger.println(F("PMS sensor on SWSerial"));
  // Logger.print(F("  RX:"));
  // Logger.println(PMS_RX);
  // Logger.print(F("  TX:"));
  // Logger.println(PMS_TX);  
  bool ret = true;
  pms.init();
  if(!ret){
    Logger.println("[ERROR] pms init FAILED");
  }
  else Logger.println("[ENV] pms is ACTIVE");
  return ret;  
}

void print_pms_status(){
    Logger.print("[ENV] pms status: ");
    switch (pms.status)
    {
    case pms.OK: // should never come here
      Logger.println("OK");    
      break;     // included to compile without warnings
    case pms.ERROR_TIMEOUT:
      Logger.println(F(PMS_ERROR_TIMEOUT));
      break;
    case pms.ERROR_MSG_UNKNOWN:
      Logger.println(F(PMS_ERROR_MSG_UNKNOWN));
      break;
    case pms.ERROR_MSG_HEADER:
      Logger.println(F(PMS_ERROR_MSG_HEADER));
      break;
    case pms.ERROR_MSG_BODY:
      Logger.println(F(PMS_ERROR_MSG_BODY));
      break;
    case pms.ERROR_MSG_START:
      Logger.println(F(PMS_ERROR_MSG_START));
      break;
    case pms.ERROR_MSG_LENGTH:
      Logger.println(F(PMS_ERROR_MSG_LENGTH));
      break;
    case pms.ERROR_MSG_CKSUM:
      Logger.println(F(PMS_ERROR_MSG_CKSUM));
      break;
    case pms.ERROR_PMS_TYPE:
      Logger.println(F(PMS_ERROR_PMS_TYPE));
      break;
    }
  }

void print_pms(){
  // print the results
  Logger.print(F("PM1.0 "));
  Logger.print(pms.pm01);
  Logger.print(F(", "));
  Logger.print(F("PM2.5 "));
  Logger.print(pms.pm25);
  Logger.print(F(", "));
  Logger.print(F("PM10 "));
  Logger.print(pms.pm10);
  Logger.println(F(" [ug/m3]"));

  if(!pms_debug) return;

  print_pms_status();

  if (pms.has_number_concentration())
  {
    Serial.print(F("N0.3 "));
    Serial.print(pms.n0p3);
    Serial.print(F(", "));
    Serial.print(F("N0.5 "));
    Serial.print(pms.n0p5);
    Serial.print(F(", "));
    Serial.print(F("N1.0 "));
    Serial.print(pms.n1p0);
    Serial.print(F(", "));
    Serial.print(F("N2.5 "));
    Serial.print(pms.n2p5);
    Serial.print(F(", "));
    Serial.print(F("N5.0 "));
    Serial.print(pms.n5p0);
    Serial.print(F(", "));
    Serial.print(F("N10 "));
    Serial.print(pms.n10p0);
    Serial.println(F(" [#/100cc]"));
  }

  if (pms.has_temperature_humidity() || pms.has_formaldehyde())
  {
    Serial.print(pms.temp, 1);
    Serial.print(F(" °C"));
    Serial.print(F(", "));
    Serial.print(pms.rhum, 1);
    Serial.print(F(" %rh"));
    Serial.print(F(", "));
    Serial.print(pms.hcho, 2);
    Serial.println(F(" mg/m3 HCHO"));
  }
}

float get_pms(uint8_t channel = 0){
  // print_env();
  if(channel == 0){
    pms.read();
    return pms.pm01;
  }  
  if(channel == 1)  return pms.pm25;
  if(channel == 2)  return pms.pm10;
  if(channel == 3)  return pms.n0p3;
  if(channel == 4)  return pms.n0p5;
  if(channel == 5)  return pms.n1p0;
  if(channel == 6)  return pms.n2p5;
  if(channel == 7)  return pms.n5p0;
  if(channel == 8)  return pms.n10p0;
  return 0;
}

#endif

#ifdef PCF8591
#include <Adafruit_PCF8591.h>

// TwoWire Wire_B = TwoWire();
// Wire_B.setClock(100000);
// Wire_B.setClockStretchLimit();
// adafruit_i2c bool setSpeed(uint32_t desiredclk);

// Make sure that this is set to the value in volts of VCC
#define ADC_REFERENCE_VOLTAGE 5.0
Adafruit_PCF8591 pcf = Adafruit_PCF8591();

bool init_PCF8591(){
  bool ret = false;
  ret = pcf.begin();
  if(!ret){
    Logger.println("[ERROR] PCF8591 init FAILED");
    return false;
  }
  else Logger.println("[ENV] PCF8591 is ACTIVE");
  pcf.enableDAC(true);
  return ret;  
}

float int_to_volts(uint16_t dac_value, uint8_t bits, float logic_level) {
  return (((float)dac_value / ((1 << bits) - 1)) * logic_level);
}

float get_PCF8591(uint8_t channel = 0, bool volts=false){
  Wire.setClock(100000L);
  // print_env();
  if(volts){
    if(channel == 0) return int_to_volts(pcf.analogRead(0), 8, ADC_REFERENCE_VOLTAGE);
    if(channel == 1) return int_to_volts(pcf.analogRead(1), 8, ADC_REFERENCE_VOLTAGE);
    if(channel == 2) return int_to_volts(pcf.analogRead(2), 8, ADC_REFERENCE_VOLTAGE);
    if(channel == 3) return int_to_volts(pcf.analogRead(3), 8, ADC_REFERENCE_VOLTAGE);
  }
  else {
    if(channel == 0) return pcf.analogRead(0);
    if(channel == 1) return pcf.analogRead(1);
    if(channel == 2) return pcf.analogRead(2);
    if(channel == 3) return pcf.analogRead(3);
  }
  Wire.setClock(400000L);
}

void print_PCF8591(){
  pcf.analogWrite(random(128)+128);
  for(int i=0;i<4;i++){
    Logger.print(get_PCF8591(i));
  }
  Logger.println("");
  for(int i=0;i<4;i++){
    Logger.print(get_PCF8591(i,true));
  }
  Logger.println("");
}

#endif


#ifdef INA219
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

bool init_INA219(){
  bool ret = false;
  ret = ina219.begin();
  if(!ret){
    Logger.println("[ERROR] INA219 init FAILED");
  }
  else Logger.println("[ENV] INA219 is ACTIVE");
  return ret;  
}

void print_INA219(){
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Logger.print("Bus Voltage:   "); Logger.print(busvoltage); Logger.println(" V");
  Logger.print("Shunt Voltage: "); Logger.print(shuntvoltage); Logger.println(" mV");
  Logger.print("Load Voltage:  "); Logger.print(loadvoltage); Logger.println(" V");
  Logger.print("Current:       "); Logger.print(current_mA); Logger.println(" mA");
  Logger.print("Power:         "); Logger.print(power_mW); Logger.println(" mW");
  Logger.println("");  
}

float get_INA219(uint8_t channel = 0){
  print_INA219();
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  if(channel == 0) return shuntvoltage;
  if(channel == 1) return busvoltage;
  if(channel == 2) return ina219.getCurrent_mA();
  if(channel == 3) return ina219.getPower_mW();
  if(channel == 4) return busvoltage + (shuntvoltage / 1000);
  return 0;
}
#endif


#ifdef MPU6050
// 3.3V onboard ldo for 5v
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

bool init_mpu6050(){
  bool ret = false;
  ret = mpu.begin();

  // mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  // mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  // mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  
  // mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // mpu.setGyroRange(MPU6050_RANGE_1000_DEG);
  // mpu.setGyroRange(MPU6050_RANGE_2000_DEG);

  // mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);
  // mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);


  if(!ret){
    Logger.println("[ERROR] MPU6050 init FAILED");
  }
  else Logger.println("[ENV] MPU6050 gyro/accel is ACTIVE");
  return ret;  
}

void print_mpu6050(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Logger.print("Acceleration X: ");
  Logger.print(a.acceleration.x);
  Logger.print(", Y: ");
  Logger.print(a.acceleration.y);
  Logger.print(", Z: ");
  Logger.print(a.acceleration.z);
  Logger.println(" m/s^2");

  Logger.print("Rotation X: ");
  Logger.print(g.gyro.x);
  Logger.print(", Y: ");
  Logger.print(g.gyro.y);
  Logger.print(", Z: ");
  Logger.print(g.gyro.z);
  Logger.println(" rad/s");

  Logger.print("Temperature: ");
  Logger.print(temp.temperature);
  Logger.println(" degC");  
}

float get_mpu6050(uint8_t channel = 0){
  // print_mpu6050();
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);  
  // accel m/s^2
  if(channel == 0) return a.acceleration.x;
  if(channel == 1) return a.acceleration.y;
  if(channel == 2) return a.acceleration.z; 

  // gyro rad/s
  if(channel == 3) return g.gyro.x;
  if(channel == 4) return g.gyro.y;
  if(channel == 5) return g.gyro.z;
  return 0;
}

#endif



#ifdef USEBH1750
#include <BH1750.h>
// 2.4V to 3.6V
BH1750 lightMeter(0x23);

bool init_BH1750(){
  bool ret = false;
  ret = lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  if(!ret){
    Logger.println("[ERROR] BH1750 init FAILED");
  }
  else Logger.println("[ENV] BH1750 is ACTIVE");
  return ret;
}

void print_BH1750(){
}

float get_BH1750(uint8_t channel = 0){
  // print_env();
  if (lightMeter.measurementReady()){
    if(channel == 0) return (float)lightMeter.readLightLevel();
    if(channel == 1) return 0;
    if(channel == 2) return 0;
  }
  return 0;
}
#endif


// #ifdef BH1750_B
// #include <hp_BH1750.h>  //inlude the library
// hp_BH1750 env_BH1750_B;

// // BH1750Address addr = BH1750_TO_VCC;
// // BH1750Address BH1750addr = BH1750_TO_GROUND;
// // 0x23/0x5A , or 0X5C
// // BH1750_TO_GROUND = 0x23,
// // BH1750_TO_VCC = 0x5C

// void print_bh1750(){
//    if (env_BH1750_B.hasValue() == true) {    // non blocking reading
//     float lux = env_BH1750_B.getLux();
//     Logger.println("[ENV] lux");
//     Logger.println(lux);
//     env_BH1750_B.start();
//   } 
// }

// bool init_bh1750(){
//   bool status = env_BH1750_B.begin(BH1750_TO_GROUND);   // will be false no sensor found
//                                             // use BH1750_TO_GROUND or BH1750_TO_VCC depending how you wired the address pin of the sensor.
  
//   // BH1750.calibrateTiming();  //uncomment this line if you want to speed up your sensor
//   env_BH1750_B.start();
//   // BH1750.start(BH1750_QUALITY_HIGH2, mtreg);
//   // BH1750.setQuality(mode);
  
//   if(!status) Logger.println("[ERROR] BH1750 init FAILED");
//   else Logger.println("[ENV] BH1750 Device is ACTIVE");

//   return status;
// }

// float get_bh1750(uint8_t channel = 0){
//   float lux;
//   if(channel == 0){
//     Logger.println("env_BH1750_B get channel 0");
//     if (env_BH1750_B.hasValue() == true) {    // non blocking reading
//      Logger.println("env_BH1750_B has value");
//      lux = env_BH1750_B.getLux();
//      env_BH1750_B.start();
//      Logger.println(lux);
//     }
//   }
//   else{
//     env_BH1750_B.start();   //starts a measurement
//     lux=env_BH1750_B.getLux();
//     Logger.println(lux);
//   }
//   return lux;
// }
// #endif


#ifdef APDS9960
// 2.4V to 3.6V
#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds;
uint8_t apds_int_pin = -1;

// apds_addr = ;

bool init_apds(){
  bool ret = false;
  ret = apds.begin();
  if(!ret){
    Logger.println("[ERROR] APDS9960 init FAILED");
  }
  else Logger.println("[ENV] APDS9960 is ACTIVE");
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
  return "";
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
    Logger.println(F("[ERROR] BMP280 init FAILED"));
  }
  else Logger.println(F("[ENV] BMP280 is ACTIVE")); 

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
  return 0;
}

#endif  

#ifdef USESHT31
bool enableHeater = false;
uint8_t loopCnt = 0;

void init_sht31(){
  bool init = sht31.begin(0x44);
  if(init){
      Logger.println(F("[ENV] SHT31 is ACTIVE")); 
  }
  else
  {
      Logger.println(F("[ERROR] SHT31 init FAILED")); 
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
    Logger.print("[ENV] SHT31 Heater State: ");
    if (sht31.isHeaterEnabled())
      Logger.println("ENABLED");
    else
      Logger.println("DISABLED");

    loopCnt = 0;
  }
}
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

void init_sht21(){
  bool init = myHTU21D.begin();
  if(init){
      Logger.println(F("[ENV] HTU21D, SHT21 is ACTIVE")); 
  }
  else
  {
      Logger.println(F("[ERROR] HTU21D, SHT21 init FAILED")); //(F()) saves string to flash & keeps dynamic memory free
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
// 0x5A or 0x5B
#ifdef USECS811
#include "Adafruit_CCS811.h"
Adafruit_CCS811 ccs;

void init_cs811(){
  int starttime = millis();
  if(!ccs.begin()){
    Logger.println("[ERROR] CS811 init FAILED");
  }
  else {
     Logger.println("[ENV] CS811 is ACTIVE");
    //calibrate temperature sensor
      while(!ccs.available() && millis() < starttime+5000);
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



#ifdef SDC4X
#include <SensirionI2CScd4x.h>
SensirionI2CScd4x scd4x;
// CO2 0x62
void init_scd4x(){
  bool ret = false;
  Wire.begin();  
  scd4x.begin(Wire);

  // stop potentially previously started measurement
  ret != scd4x.stopPeriodicMeasurement();
  // if (ret) {
  //     Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
  //     errorToString(error, errorMessage, 256);
  //     Serial.println(errorMessage);
  // } else ret = true;

  ret != scd4x.startPeriodicMeasurement();

  if(!ret){
    Logger.println("[ERROR] SCD4X init FAILED");
  }
  else Logger.println("[ENV] SCD4X is ACTIVE");
  // return ret;  
}

void print_scd4x(){
}

float get_scd4x(uint8_t channel = 0){
  uint16_t co2;
  float temperature;
  float humidity;
  uint16_t error;
  
  error = scd4x.readMeasurement(co2, temperature, humidity);

  // print_env();
  if(channel == 0) return co2;
  if(channel == 1) return temperature;
  if(channel == 2) return humidity;
  return 0;
}
#endif


#ifdef USEBMP180
#include <Adafruit_BMP085.h.h>
Adafruit_BMP085 bmp180;
void init_bmp180(){
  bool ret = false;
  ret = bmp180.begin();
  if(!ret){
    Logger.println("[ERROR] BMP180 init FAILED");
  }
  else Logger.println("[ENV] BMP180 is ACTIVE");
  // return ret;  
}

void print_bmp180(){
    Logger.print("Temperature = ");
    Logger.print(bmp.readTemperature());
    Logger.println(" *C");
    
    Logger.print("Pressure = ");
    Logger.print(bmp.readPressure());
    Logger.println(" Pa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Logger.print("Altitude = ");
    Logger.print(bmp.readAltitude());
    Logger.println(" meters");

    Logger.print("Pressure at sealevel (calculated) = ");
    Logger.print(bmp.readSealevelPressure());
    Logger.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Logger.print("Real altitude = ");
    Logger.print(bmp.readAltitude(101500));
    Logger.println(" meters");
    
    Logger.println();  
}

float get_bmp180(uint8_t channel = 0){
  // print_env();
  if(channel == 0) return bmp180.readTemperature();
  if(channel == 1) return bmp180.readPressure();
  if(channel == 2) return bmp180.readAltitude();
  if(channel == 3) return bmp180.readSealevelPressure();
  if(channel == 4) return bmp180.readAltitude(101500);
  return 0;
}
#endif


#endif