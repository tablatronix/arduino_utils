#ifndef ntc_multi_h
#define ntc_multi_h

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
#define NTC_PIN                36
#define NTC_BPIN               39
#define NTC_CPIN               33

// #define VERBOSE_SENSOR_ENABLED 1

// 100000 / (320000 +RT) * 3.3 
int V = 3.3;
int rt = 330000/V - 320000;

// Thermistor object
// THERMISTOR ntc(NTC_PIN,        // Analog pin
//                       10000,          // Nominal resistance at 25 ºC
//                       3950,           // thermistor's beta coefficient
//                       10000);         // Value of the series resistor

uint16_t resist     = 100000;
uint16_t ntcnominal = 100000;
uint16_t betaco     = 3950;

// Does not properly compensate for non balanced reisstance

#ifdef local_thermistor_h
bool lowside = true;
THERMISTOR ntc(NTC_PIN,ntcnominal,betaco,resist,lowside);
THERMISTOR ntcB(NTC_BPIN,ntcnominal,betaco,resist,!lowside);
THERMISTOR ntcC(NTC_CPIN,ntcnominal,betaco,resist,lowside);
THERMISTOR ntcD(NTC_CPIN,ntcnominal,betaco,resist,lowside);
#else
THERMISTOR  ntc(NTC_PIN ,ntcnominal,betaco,resist);
THERMISTOR ntcB(NTC_BPIN,ntcnominal,betaco,resist);
THERMISTOR ntcC(NTC_CPIN,ntcnominal,betaco,resist);
THERMISTOR ntcD(NTC_CPIN,ntcnominal,betaco,resist);
#endif

// Global temperature reading
float ntc_temp;
float ntc_temp_b;
float ntc_temp_c;
float ntc_temp_d;

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

float get_ntc_B(){
  return ntc_temp_b;
}

float get_ntc_C(){
  return ntc_temp_c;
}

float get_ntc_D(){
  return ntc_temp_d;
}

/**
 * loop
 *
 * Arduino main loop
 */
void read_ntc()
{
  ntc_temp = ntc.read();   // Read temperature
  ntc_temp_b = ntcB.read();
  ntc_temp_c = ntcC.read();
  ntc_temp_d = ntcD.read();
  // Serial.print("Temp in 1/10 ºC : ");
  // Serial.println(ntc_temp);
  // Serial.print("Temp in ºF : ");
  // Serial.println((((ntc_temp/10)*1.8)+32));
}

void printNTC(){
  // read_ntc();
  float tempf;
  tempf = (((ntc_temp/10)*1.8)+32);
  Serial.println("[NTC]: " + (String)(get_ntc()/10) + "ºC " + tempf + "ºF");
  
  tempf = (((ntc_temp_b/10)*1.8)+32);
  Serial.println("[NTC]: " + (String)(get_ntc_B()/10) + "ºC " + tempf + "ºF");  
  
  tempf = (((ntc_temp_c/10)*1.8)+32);
  Serial.println("[NTC]: " + (String)(get_ntc_C()/10) + "ºC " + tempf + "ºF");  
  
  tempf = (((ntc_temp_d/10)*1.8)+32);
  Serial.println("[NTC]: " + (String)(get_ntc_D()/10) + "ºC " + tempf + "ºF");
}


void calc_ntc(int value){
  ntc_temp = ntc.read(value);
}
void calc_ntc_B(int value){
  ntc_temp_b = ntcB.read(value);
}
void calc_ntc_C(int value){
  ntc_temp_c = ntcC.read(value);
}
void calc_ntc_D(int value){
  ntc_temp_d = ntcD.read(value);
}

#endif