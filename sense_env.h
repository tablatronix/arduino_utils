#ifndef sensors_h
#define sensors_h

#include <Wire.h>

#ifdef USELM75
#include <LM75A.h>
#include <Wire.h>
// Create I2C LM75A instance
LM75A lm75(true,  //A0 LM75A pin state
                   true,  //A1 LM75A pin state
                   true); //A2 LM75A pin state
#endif

#ifdef USESHT21
#include <HTU21D.h>
HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);
#endif


void init_sht(){
  #ifdef USESHT21
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
  #endif
}

void get_sht(){
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