#ifndef buttons_h
#define buttons_h

#include <i2c_fans.h>
#include <motor.h>

#include <IoAbstraction.h>
#include <IoAbstractionWire.h>

const int interruptPin = 3; // interrupt for io expander
const int encoderSWPin = 0;
const int encoderAPin  = 1;
const int encoderBPin  = 2;
const int maximumEncoderValue = 128;
int encoderLast  = 0;
int encoderCount = 0;

volatile bool PCFInterruptFlag = false;

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

void ICACHE_RAM_ATTR onInterrupt(uint8_t bits) {
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

  switches.addSwitch(motorInt, onFeedback); // stall feedback
  switches.addSwitch(encoderSWPin, onEncoderSWPressed); // encoder button press

  encoderLast = 0;
  // setupRotaryEncoderWithInterrupt(encoderAPin, encoderBPin, onEncoderChange);
  // HardwareRotaryEncoder* firstEncoder = new HardwareRotaryEncoder(encoderAPin, encoderBPin, onEncoderChange);
  // switches.setEncoder(0, firstEncoder);
  // switches.getEncoder()->setAccelerationMode(HWACCEL_SLOWER); 
  // switches.changeEncoderPrecision(maximumEncoderValue, 100);
  switches.changeEncoderPrecision(maximumEncoderValue,encoderLast);
  // firstEncoder->setAccelerationMode(HWACCEL_SLOWER); // acceleration mode
  // taskManager.setInterruptCallback(onInterrupt);
}

#endif