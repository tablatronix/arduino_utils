#include <ntc_multi.h>

void setup(){
	
}

void loop(){
  printNTC();
    calc_ntc(get_PCF8591(0)*16);
  calc_ntc_B(get_PCF8591(1)*16);
  calc_ntc_C(get_PCF8591(2)*16);
  calc_ntc_D(get_PCF8591(3)*16);
  Serial.println(String(get_ntc()/10));
  Serial.println(String(get_ntc_B()/10));
  Serial.println(String(get_ntc_C()/10));
  Serial.println(String(get_ntc_D()/10));	
}