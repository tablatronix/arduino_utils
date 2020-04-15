#ifndef buttons_h
#define buttons_h

#include <i2c_fans.h>
#include <motor.h>

#include <IoAbstraction.h>
#include <IoAbstractionWire.h>

const int interruptPin = 3; // Real interrupt for io expander
const int encoderSWPin = 0;
const int encoderAPin  = 1;
const int encoderBPin  = 2;
const int maximumEncoderValue = 128;
int encoderLast  = 0;

// int encoderNew   = 0;
// int encoderDir   = 0;
// int encoderCount = 0;
// bool encoderHasChange = false;;

volatile bool PCFInterruptFlag = false;

//
// Each time the encoder value changes, this function runs, as we registered it as a callback
//
void ICACHE_RAM_ATTR onEncoderChange(int newValue) {
  Serial.print("[ENCODER] change to ");
  Serial.print(newValue);
  Serial.print(" from ");
  Serial.print(encoderLast);
  if(encoderLast == newValue) return;
  bool dir = (encoderLast > newValue);
  Serial.println(" dir: " + String(dir ? "CC" : "CW")); // for plotting
  encoderLast = newValue;
}

//
// When the spinwheel is clicked, this function will be run as we registered it as a callback
//
void onEncoderSWPressed(uint8_t pin, bool heldDown) {
  Serial.print("[ENCODER] Button ");
  Serial.println(heldDown ? "Held" : "Pressed");
}

void setEncoderAccel(HWAccelerationMode accel){
  HardwareRotaryEncoder* hwEncoder = reinterpret_cast<HardwareRotaryEncoder*>(switches.getEncoder());
  hwEncoder->setAccelerationMode(accel);
}

void ICACHE_RAM_ATTR onInterruptB(uint8_t bits) {
    Serial.print("[INT] Interrupt triggered");
    Serial.print("  Interrupt was ");
    Serial.println(bits);  
  	PCFInterruptFlag = true;
}

void init_buttons(){
  // First we set up the switches library, giving it the task manager and tell it where the pins are located
  // We could also of chosen IO through an i2c device that supports interrupts.
  // the second parameter is a flag to use pull up switching, (true is pull up).
  switches.initialiseInterrupt(ioFrom8574(0x20, interruptPin), true);

  // ioDevicePinMode(switches.getIoAbstraction(), ledPin,    OUTPUT);
  ioDevicePinMode(switches.getIoAbstraction(), motorpinA, OUTPUT);
  ioDevicePinMode(switches.getIoAbstraction(), motorpinB, OUTPUT);
  ioDevicePinMode(switches.getIoAbstraction(), motorInt,  INPUT);
  ioDevicePinMode(switches.getIoAbstraction(), fanCpin,   OUTPUT);
  ioDevicePinMode(switches.getIoAbstraction(), fanDpin,   OUTPUT);

  ioDevicePinMode(switches.getIoAbstraction(), encoderAPin, INPUT);
  ioDevicePinMode(switches.getIoAbstraction(), encoderBPin, INPUT);

  switches.addSwitch(motorInt, onFeedback); // stall feedback
  switches.addSwitch(encoderSWPin, onEncoderSWPressed); // encoder button press

  encoderLast = 0;
  setupRotaryEncoderWithInterrupt(encoderAPin, encoderBPin, onEncoderChange);
  switches.changeEncoderPrecision(maximumEncoderValue, 0); // REQUIRED!!!!!
 
  // firstEncoder->setAccelerationMode(HWACCEL_SLOWER); // acceleration mode

  // alt encocder setup
  // HardwareRotaryEncoder* firstEncoder = new HardwareRotaryEncoder(encoderAPin, encoderBPin, onEncoderChange);
  // switches.setEncoder(0, firstEncoder);
  
  // interrupt for any
  // taskManager.setInterruptCallback(onInterrupt);
}

#endif