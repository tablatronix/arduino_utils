#ifndef sensors_h
#define sensors_h

#include <Wire.h>

#define USESHT31
// #define USESHT21
#define USEBMP280

#ifdef USELM75
#include <LM75A.h>
// Create I2C LM75A instance
LM75A lm75(true,  //A0 LM75A pin state
                   true,  //A1 LM75A pin state
                   true); //A2 LM75A pin state
#endif

#ifdef USESHT21
#include <HTU21D.h>
HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);
#endif

#ifdef USESHT31
#include <Adafruit_SHT31.h>
Adafruit_SHT31 sht31 = Adafruit_SHT31();
#endif

#ifdef USEBMP280
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // I2C
#endif

#ifdef USEBMP280
void init_bmp280(){
   if (!bmp.begin(BMP280_ADDRESS_ALT)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */ 
}

void print_bmp280(){
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Serial.println(" m");

    Serial.println();  
}
#endif


#ifdef USESHT31
bool enableHeater = false;
uint8_t loopCnt = 0;

void init_sht31(){
  bool init = sht31.begin(0x44);
  if(init){
      Serial.println(F("SHT31 sensor is active")); 
  }
  else
  {
      Serial.println(F("SHT31 sensor is faild or not connected")); //(F()) saves string to flash & keeps dynamic memory free
  }

  Serial.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())  Serial.println("ENABLED");
  else  Serial.println("DISABLED");
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
    Serial.print("Temp *C = "); Serial.print(t); Serial.print("\t\t");
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(h);
  } else { 
    Serial.println("Failed to read humidity");
  }
}

void sht31_process(){
  // Toggle heater enabled state every 30 seconds
  // An ~3.0 degC temperature increase can be noted when heater is enabled
  if (++loopCnt == 30) {
    enableHeater = !enableHeater;
    sht31.heater(enableHeater);
    Serial.print("Heater Enabled State: ");
    if (sht31.isHeaterEnabled())
      Serial.println("ENABLED");
    else
      Serial.println("DISABLED");

    loopCnt = 0;
  }
}
#endif

#ifdef USESHT21
void init_sht21(){
  bool init = myHTU21D.begin();
  if(init){
      Serial.println(F("HTU21D, SHT21 sensor is active")); 
  }
  else
  {
      Serial.println(F("HTU21D, SHT21 sensor is faild or not connected")); //(F()) saves string to flash & keeps dynamic memory free
  }
  // while (myHTU21D.begin(SCL,SDA) != true)
  // {
  //   Serial.println(F("HTU21D, SHT21 sensor is failed or not connected")); //(F()) saves string to flash & keeps dynamic memory free
  //   delay(5000);
  // }
  // Serial.println(F("HTU21D, SHT21 sensor is active")); 
}

String getSHT21(){
  return (String)myHTU21D.readCompensatedHumidity() 
}

void print_sht21(){
  #ifdef USESHT21
    /* DEMO - 1 */
  Serial.println(F("DEMO 1: 12-Bit Resolution"));
  Serial.print(F("Humidity............: ")); Serial.print(myHTU21D.readHumidity());            Serial.println(F(" +-2%"));
  Serial.print(F("Compensated Humidity: ")); Serial.print(myHTU21D.readCompensatedHumidity()); Serial.println(F(" +-2%"));

  Serial.println(F("DEMO 1: 14-Bit Resolution")); 
  Serial.print(F("Temperature.........: ")); Serial.print(myHTU21D.readTemperature()); Serial.println(F(" +-0.3C"));

 
  /* DEMO - 2 */
  Serial.println(F("DEMO 2: 11-Bit Resolution"));
  myHTU21D.setResolution(HTU21D_RES_RH11_TEMP11);
  Serial.print(F("Humidity............: ")); Serial.print(myHTU21D.readHumidity());            Serial.println(F(" +-2%"));
  Serial.print(F("Compensated Humidity: ")); Serial.print(myHTU21D.readCompensatedHumidity()); Serial.println(F(" +-2%"));

  Serial.println(F("DEMO 2: 11-Bit Resolution"));
  Serial.print(F("Temperature.........: ")); Serial.print(myHTU21D.readTemperature()); Serial.println(F(" +-0.3C"));


  /* DEMO - 3 */
  Serial.println(F("DEMO 3: Battery Status"));
  if   (myHTU21D.batteryStatus() == true) Serial.println(F("Battery.............: OK.  Level > 2.25v"));
  else                                    Serial.println(F("Battery.............: LOW. Level < 2.25v"));


  /* DEMO - 4 */
  Serial.println(F("DEMO 4:"));
  Serial.print(F("Firmware version....: ")); Serial.println(myHTU21D.readFirmwareVersion());


  /* DEMO - 5 */
  Serial.println(F("DEMO 5:"));
  Serial.print(F("Sensor's ID.........: ")); Serial.println(myHTU21D.readDeviceID());


  /* back to lib. default resolution */
  myHTU21D.softReset();
  myHTU21D.setResolution(HTU21D_RES_RH12_TEMP14);
  #endif
}
#endif

#ifdef USELM75
void init_LM75(){
}

void print_LM75(){
     Serial.println((String)lm75.getTemperatureInFahrenheit() + " F"); 
}
#endif


#endif