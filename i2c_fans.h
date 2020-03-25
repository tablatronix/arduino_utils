#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;
Adafruit_MCP4725 dacb;

int percvalue = 41; // (4095/100);

int fanAVCC = 500; // fan dac vcc in mv
int fanALOW = 170; // fan turn on voltage
int fanAHI  = 300; // fan max voltage
int dacscale = 4095/5;

int fanAVCC_out = 12; // 5.2;
int fanBVCC_out = 5; // 5.2;
// should I calculate hfe /beta  here or is that even necessary?

// 
// 5.00/4095 = .0122
// .012 * 170 = 21

// 4095 / 5.00 = 819 
// 

int fanDacVoltage(int vcc){
	return vcc*dacscale;
}

void fan_init(){
  dac.begin(0x60);
  dacb.begin(0x61);
  dac.setVoltage(0, true);   
  dacb.setVoltage(0, true);   
}

void fanA(int perc){
	int value = constrain(percvalue*perc,0,4095);
  	dac.setVoltage(value, false);	
	Serial.println("[FAN] 1 " + (String)perc +"% -" + value);
}

void fanB(int perc){
	int value = constrain(percvalue*perc,0,4095);
  	dacb.setVoltage(value, false);	
	Serial.println("[FAN] 2 " + (String)perc +"% -" + value);
}

void fanAVolts(int value){
	value = constrain(value,0,fanAVCC); // mv
	value = (value*dacscale)/100;
  	dac.setVoltage((value), false);	
	Serial.println("[FAN] 1 " + (String) value);
}

void fanAV(int value){
	value = constrain(value,0,4095);
  	dac.setVoltage(value, false);	
	Serial.println("[FAN] 1 " + (String) value);
}

void fanBV(int value){
  value = constrain(value,0,4095);
    dacb.setVoltage(value, false); 
  Serial.println("[FAN] 2 " + (String) value);
}

void fanBVolts(int value){
	value = constrain(value,0,fanAVCC); // mv
	value = (value*dacscale)/100;
  	dacb.setVoltage((value), false);	
	Serial.println("[FAN] 2 " + (String) value);
}

void fanTest(){
  int duration = 1000;

  fanBVolts(500); //5v
  delay(duration);
  fanBVolts(300); //5v
  delay(duration);
  fanBVolts(230); //5v
  delay(duration);
  fanBVolts(0); //5v
  delay(duration);
  fanAVolts(500); // 12v 
  delay(duration);
  fanAVolts(300); // 12v 
  delay(duration);
  fanAVolts(230); // 12v 
  delay(duration);
  fanAVolts(0); // 12v 
}          