#ifndef buttons_h
#define buttons_h

#include <i2c_fans.h>
#include <motor.h>

#include <IoAbstraction.h>
#include <IoAbstractionWire.h>

const int interruptPin = 3; // if stalldetect, Real interrupt for io expander USES SERIAL RX!!!
const int encoderSWPin = 0;
const int encoderAPin  = 1;
const int encoderBPin  = 2;
const int maximumEncoderValue = 9; // 128; 0 for non counting mode
int encoderStale  = 0;
int encoderLast   = 0;
bool encoderHasChange = false;

volatile bool PCFInterruptFlag = false;

void ICACHE_RAM_ATTR onEncoderChange(int newValue) {
  if(maximumEncoderValue > 0){
    Serial.print("[ENCODER] change to ");
  Serial.print(newValue);
    Serial.print(" from ");
    Serial.print(encoderLast);
    if(encoderLast == newValue){
      Serial.println("\n");
      return;
    }
    bool dir = (encoderLast > newValue);
    Serial.println(" dir: " + String(dir ? "CC" : "CW"));
    encoderStale = encoderLast;
    encoderLast  = newValue;
    encoderHasChange = true;
  }
  else{
    if(newValue == 0){
      Serial.print("[ENCODER] no change");
      return;
    }
    Serial.print("[ENCODER] change by ");
    Serial.print(newValue);
    bool dir = (newValue == 1);
    Serial.println(" dir: " + String(dir ? "CW" : "CC")); // for plotting use 10,20 etc
    
    encoderStale = encoderLast;
    encoderLast  = newValue;
    encoderHasChange = true;
  }
}

void ICACHE_RAM_ATTR onEncoderSWPressed(uint8_t pin, bool heldDown) {
  Serial.print("[ENCODER] Button ");
  Serial.println(heldDown ? "Held" : "Pressed");
}

void ICACHE_RAM_ATTR onFeedbackB(uint8_t pin, bool heldDown) {
  Serial.print("[ENCODER] MOTOR FEEDBACK ");
  Serial.println(heldDown ? "Held" : "Pressed");
}

void ICACHE_RAM_ATTR onInterruptB(uint8_t bits) {
    Serial.print("  Interrupt was ");
    Serial.println(bits);  
  	PCFInterruptFlag = true;
}

// adjust the encoder acceleraton
// HWACCEL_FAST
// HWACCEL_SLOWER
// HWACCEL_NONE
void setEncoderAccel(HWAccelerationMode accel){
  HardwareRotaryEncoder* hwEncoder = reinterpret_cast<HardwareRotaryEncoder*>(switches.getEncoder());
  hwEncoder->setAccelerationMode(accel);
}

void init_buttons(){
  // First we set up the switches library, giving it the task manager and tell it where the pins are located
  // We could also of chosen IO through an i2c device that supports interrupts.
  // the second parameter is a flag to use pull up switching, (true is pull up).
  if(stalldetect)switches.initialiseInterrupt(ioFrom8574(0x20, interruptPin), true);
  else switches.initialiseInterrupt(ioFrom8574(0x20), true);

  // motor driver DRV8333
  ioDevicePinMode(switches.getIoAbstraction(), motorpinA, OUTPUT);
  ioDevicePinMode(switches.getIoAbstraction(), motorpinB, OUTPUT);
  // ioDevicePinMode(switches.getIoAbstraction(), 7,  INPUT); // interrupt for motor stall
  switches.addSwitch(motorInt, onFeedback); // stall feedback

  // transistor sink
  ioDevicePinMode(switches.getIoAbstraction(), fanCpin,   OUTPUT); // ssr fan stage 1
  ioDevicePinMode(switches.getIoAbstraction(), fanDpin,   OUTPUT); // ssr fan stage 2

  // encoder
  ioDevicePinMode(switches.getIoAbstraction(), encoderAPin, INPUT);
  ioDevicePinMode(switches.getIoAbstraction(), encoderBPin, INPUT);
  switches.addSwitch(encoderSWPin, onEncoderSWPressed); // encoder button press
  // init

  setupRotaryEncoderWithInterrupt(encoderAPin, encoderBPin, onEncoderChange);
  if(maximumEncoderValue > 0) switches.changeEncoderPrecision(maximumEncoderValue, 0); // REQUIRED!!!!!

  // alt encocder setup
  // HardwareRotaryEncoder* firstEncoder = new HardwareRotaryEncoder(encoderAPin, encoderBPin, onEncoderChange);
  // switches.setEncoder(0, firstEncoder);
  // firstEncoder->setAccelerationMode(HWACCEL_SLOWER); // acceleration mode
  
  // interrupt for any
  // taskManager.setInterruptCallback(onInterrupt);
}

  // IoAbstractionRef iodev = switches.getIoAbstraction();
  // iodev->pinDirection(7,OUTPUT);
  // bool res = iodev->runLoop();
  // if(res!=1) Serial.println("devsync: " + (String)res);
  // taskManager.runLoop();
  // if(!res){
  //   Serial.println("error");
  //   delay(500);
  // }
#endif