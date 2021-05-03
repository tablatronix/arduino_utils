#ifndef ntc_h
#define ntc_h

#include "thermistor.h"
/** 
 * Author: Daniel Berenguer
 * Creation date: 11/05/2014
 * 
 * Basic library that reads a NTC thermistor from an analog pin. Calculations relie on
 * Steinhart–Hart equation: * 
 *
 * https://en.wikipedia.org/wiki/Thermistor#Steinhart.E2.80.93Hart_equation * 
 *
 * See the example for more information. * 
 * 
 * Example of use with panStamp:
 * https://github.com/panStamp/panstamp/wiki/Temperature-measurement-with-cheap-NTC's
*/

// Analog pin used to read the NTC
#define NTC_PIN               32
// #define VERBOSE_SENSOR_ENABLED 1

// 100000 / (320000 +RT) * 3.3 
int V = 3.3;
int rt = 330000/V - 320000;

// Thermistor object
THERMISTOR thermistor(NTC_PIN,        // Analog pin
                      100000,          // Nominal resistance at 25 ºC
                      3950,           // thermistor's beta coefficient
                      10000);         // Value of the series resistor

// Global temperature reading
float ntc_temp;


/**
 * setup
 *
 * Arduino setup function
 */
void init_ntc()
{

}

float get_ntc(){
  return ntc_temp;
}

void calc_ntc(int value){
  ntc_temp = thermistor.read(value);
}

/**
 * loop
 *
 * Arduino main loop
 */
void read_ntc()
{
  ntc_temp = thermistor.read();   // Read temperature
  // Serial.print("Temp in 1/10 ºC : ");
  // Serial.println(ntc_temp);
  // Serial.print("Temp in ºF : ");
  // Serial.println((((ntc_temp/10)*1.8)+32));
}

void printNTC(){
  // read_ntc();
  float tempf = (((ntc_temp/10)*1.8)+32);
  Logger.println("[NTC]: " + (String)(get_ntc()/10) + "ºC " + tempf + "ºF");	
}

#endif