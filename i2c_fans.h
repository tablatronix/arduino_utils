#ifndef i2c_fans_h
#define i2c_fans_h


#ifdef DEBUG
bool DEBUG_i2c_fans = true;
#else
bool DEBUG_i2c_fans = true;
#endif

#include <Adafruit_MCP4725.h>
#include <IoAbstraction.h>
#include <IoAbstractionWire.h>

#define MCP4725ERROR // use error checking on modded lib https://github.com/adafruit/Adafruit_MCP4725/pull/14
Adafruit_MCP4725 dac;
Adafruit_MCP4725 dacb;

int percvalue = 41; // (4095/100);

int fanAVCC = 500; // fan dac vcc in mv
int fanALOW = 170; // fan turn on voltage
int fanAHI  = 300; // fan max voltage
int dacscale = 4095/5;

int fanAVCC_out = 12; // 5.2;
int fanBVCC_out = 5; // 5.2;

const int fanCpin = 5;
const int fanDpin = 6;

int fan_1_status = 0;
int fan_2_status = 0;
int fan_3_status = 0;
int fan_4_status = 0;
int fan_5_status = 0;

// should I calculate hfe /beta here? Is that even necessary?
// probably just need range min max and linearity and scale to percentage 0-100
// voltage calculations
// 5.00/4095 = .0122
// .012 * 170 = 21
// 4095 / 5.00 = 819 

 // i2c results for int8_t wire.endtransmission()
 // * Output   0 .. success
 // *          1 .. length to long for buffer
 // *          2 .. address send, NACK received
 // *          3 .. data send, NACK received
 // *          4 .. other twi error (lost bus arbitration, bus error, ..)
 // 
bool getDacSuccess(){
  #ifdef MCP4725ERROR
  // Serial.print("[MCP4725] ");
  // Serial.println((String)dac._lastResult);
  return (dac._lastResult == 0);
  #else
  return true;
  #endif
}

bool getDacbSuccess(){
  #ifdef MCP4725ERROR
  // Serial.print("[MCP4725] ");
  // Serial.println((String)dacb._lastResult);
  return (dacb._lastResult == 0);
  #else
  return true;
  #endif
}

int fanDacVoltage(int vcc){
	return vcc*dacscale;
}

void setFanStatus(int id,int status){
  if(id==1) fan_1_status = status;
  if(id==2) fan_2_status = status;
  if(id==3) fan_3_status = status;
  if(id==4) fan_4_status = status;
  if(id==5) fan_5_status = status;
}

int getFanStatus(int id){
  if(id==1) return fan_1_status;
  if(id==2) return fan_2_status;
  if(id==3) return fan_3_status;
  if(id==4) return fan_4_status;
  if(id==5) return fan_5_status;
}

void fanA(int perc){
  setFanStatus(1,perc);
	int value = constrain(percvalue*perc,0,4095);
  dac.setVoltage(value, false);	
	if(DEBUG_i2c_fans) Serial.println("[FAN] 1 " + (String)perc +"% -" + value);
}

void fanB(int perc){
  setFanStatus(2,perc); 
	int value = constrain(percvalue*perc,0,4095);
  	dacb.setVoltage(value, false);	
	if(DEBUG_i2c_fans) Serial.println("[FAN] 2 " + (String)perc +"% -" + value);
}

void fanAVolts(int value){
	value = constrain(value,0,fanAVCC); // mv
	value = (value*dacscale)/100;
  setFanStatus(1,value);
  dac.setVoltage((value), false);	
	if(DEBUG_i2c_fans) Serial.println("[FAN] 1 " + (String) value);
}

void fanAV(int value){
	value = constrain(value,0,4095);
  dac.setVoltage(value, false);	
  setFanStatus(1,value);
	if(DEBUG_i2c_fans) Serial.println("[FAN] 1 " + (String) value);
}

void fanBV(int value){
  value = constrain(value,0,4095);
  dacb.setVoltage(value, false);
  setFanStatus(2,value);  
  if(DEBUG_i2c_fans) Serial.println("[FAN] 2 " + (String) value);
}

void fanBVolts(int value){
	value = constrain(value,0,fanAVCC); // mv
	value = (value*dacscale)/100;
  dacb.setVoltage((value), false);
  setFanStatus(2,value);
	if(DEBUG_i2c_fans) Serial.println("[FAN] 2 " + (String) value);
}

void fanCEnable(bool enable){
  setFanStatus(3,(int)enable);  
  if(enable){
    if(DEBUG_i2c_fans) Serial.println("[FAN] fan 3 Enabled");
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), fanCpin, true);
    // ioDeviceDigitalWriteS(switches.getIoAbstraction(), 6, true);
  }
  else {
    if(DEBUG_i2c_fans) Serial.println("[FAN] fan 3 Disabled");
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), fanCpin, false);
    // ioDeviceDigitalWriteS(switches.getIoAbstraction(), 6, false);
  }
}

void fanDEnable(bool enable){
  setFanStatus(4,(int)enable);   
  if(enable){
    if(DEBUG_i2c_fans) Serial.println("[FAN] fan 4 Enabled");
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), fanDpin, true);
  }
  else {
    if(DEBUG_i2c_fans) Serial.println("[FAN] fan 4 Disabled");
    ioDeviceDigitalWriteS(switches.getIoAbstraction(), fanDpin, false);
  }
}

/**
 * [SSRFan description]
 * @param {[type]}  bool enable [description]
 * @param {Boolean} bool power  false 50% true 100%
 */
void SSRFan(bool enable,bool power = true){
  fanCEnable(enable);
  fanDEnable(enable && power);
}

void fansOn(){
  int duration = 1000;  
  SSRFan(true,false);
  delay(duration);
  SSRFan(true);
  delay(duration);
  fanBVolts(500); //5v
  delay(duration);
  fanAVolts(500); // 12v 
  delay(duration);
}

void fansOff(){
  int duration = 1000;  
  SSRFan(true,false);
  delay(duration);
  SSRFan(false);
  delay(duration);
  fanBVolts(0); //5v
  delay(duration);
  fanAVolts(0); // 12v 
  delay(duration);
}

void fanTest(){
  int duration = 1000;

  if(DEBUG_i2c_fans) Serial.println("[FAN] Testing FAN 2");
  // fan 2
  fanBVolts(500); //5v
  delay(duration);
  fanBVolts(300); //5v
  delay(duration);
  fanBVolts(230); //5v
  delay(duration);
  fanBVolts(0); //5v

  delay(duration);

  if(DEBUG_i2c_fans) Serial.println("[FAN] Testing FAN 1");
  // fan 1
  fanAVolts(500); // 12v 
  delay(duration);
  fanAVolts(300); // 12v 
  delay(duration);
  fanAVolts(230); // 12v 
  delay(duration);
  fanAVolts(0); // 12v 

  delay(duration);

  if(DEBUG_i2c_fans) Serial.println("[FAN] Testing FAN 3 50%");
  // fan 3
  // fanCEnable(true);
  // delay(duration*2);
  // fanCEnable(false);
  SSRFan(true,false);
  delay(duration*3);
  SSRFan(false);
  delay(duration*2);
  if(DEBUG_i2c_fans) Serial.println("[FAN] Testing FAN 3 100%");
  // fan 4
  // fanDEnable(true);
  // delay(duration*2);
  // fanDEnable(false);
  SSRFan(true,true);
  delay(duration*3);
  SSRFan(false);
}

void fan_init(){
  dac.begin(0x60);
  dacb.begin(0x61);

  dac.setVoltage(0, true);  // fan 1
  dacb.setVoltage(0, true); // fan 2
  if(!getDacSuccess())  Serial.println("[ERROR] MCP4725 A Failed");
  if(!getDacbSuccess()) Serial.println("[ERROR] MCP4725 B Failed");

  // fanCEnable(false);  // fan 3 (NOT LOADED YET)
  // fanDEnable(false);  // fan 4 (NOT LOADED YET)
}

#endif