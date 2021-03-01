// Button abstraction IO wrapper
// @uses IoAbstraction

#ifndef buttons_abs_h
#define buttons_abs_h


#include <IoAbstraction.h>
#include <IoAbstractionWire.h>

const int interruptPin = 3; // if motor.h stalldetect, Real interrupt for io expander USES SERIAL RX!!!
const int encoderSWPin = 0;
const int encoderAPin  = 1;
const int encoderBPin  = 2;

int _maximumEncoderValue = 0; // 128; 0 for non counting mode

int encoderStale  = 0; // prev stale value
int encoderLast   = -1; // prev value

bool encoderHasChange = false; // flag enc change
bool useInt = false; // use interrupts, via interruptPin

bool encoderHasPress     = false;
bool encoderHasHold      = false;

volatile bool PCFInterruptFlag = false;

void encoderClear(){
  encoderHasPress = false;
  encoderHasHold  = false;
  encoderHasChange = false;
}

void ICACHE_RAM_ATTR onEncoderChange(int newValue) {
  // @todo always a false trigger on start
  // if(encoderLast < 0){
  //   Logger.println("[ENC] init fired");
  //   encoderLast = newValue;
  //   encoderClear();
  //   return; // init
  // }

  if(_maximumEncoderValue > 0){
    Logger.print("[ENCODER] change to ");
    Logger.print(newValue);
    Logger.print(" from ");
    Logger.print(encoderLast);
    if(encoderLast == newValue){
      Logger.println("\n");
      return;
    }
    bool dir = (encoderLast > newValue);
    Logger.println(" dir: " + String(dir ? "CC" : "CW"));
    encoderStale = encoderLast;
    encoderLast  = newValue;
    encoderHasChange = true;
  }
  else{
    if(newValue == 0){
      Logger.print("[ENCODER] no change");
      return;
    }
    Logger.print("[ENCODER] change by ");
    Logger.print(newValue);
    bool dir = (newValue == 1);
    Logger.println(" dir: " + String(dir ? "CW" : "CC")); // for plotting use 10,20 etc
    
    encoderStale = encoderLast;
    encoderLast  = newValue;
    encoderHasChange = true;
  }
}

void ICACHE_RAM_ATTR onEncoderSWPressed(uint8_t pin, bool heldDown) {
  Logger.print("[ENCODER] Button ");
  Logger.println(heldDown ? "Held" : "Pressed");
  encoderHasPress = true;
  encoderHasHold = heldDown;
}


// adjust the encoder acceleraton
// HWACCEL_FAST
// HWACCEL_SLOWER
// HWACCEL_NONE
void setEncoderAccel(HWAccelerationMode accel){
  HardwareRotaryEncoder* hwEncoder = reinterpret_cast<HardwareRotaryEncoder*>(switches.getEncoder());
  hwEncoder->setAccelerationMode(accel);
}

void init_encoder(int encoderAPin, int encoderBPin, int encoderSWPin,uint8_t addr=0){

  if(addr){
    // First we set up the switches library, giving it the task manager and tell it where the pins are located
    // We could also of chosen IO through an i2c device that supports interrupts.
    // the second parameter is a flag to use pull up switching, (true is pull up).
    if(useInt)switches.initialiseInterrupt(ioFrom8574(0x20, interruptPin), true);
    else switches.initialiseInterrupt(ioFrom8574(0x20), true);
  }
  else{
    switches.initialise(ioUsingArduino(), true);
  }

  // encoder sw
  #ifndef ENC_SW_ANALOG
  switches.addSwitch(encoderSWPin, onEncoderSWPressed); // encoder button press
  #endif

  // encoder
  setupRotaryEncoderWithInterrupt(encoderAPin, encoderBPin, onEncoderChange);
  if(_maximumEncoderValue > 0) switches.changeEncoderPrecision(_maximumEncoderValue, 0);

  // switches.setEncoder(0, myEncoder);
  // alt encocder setup
  // HardwareRotaryEncoder* firstEncoder = new HardwareRotaryEncoder(encoderAPin, encoderBPin, onEncoderChange);
  // switches.setEncoder(0, firstEncoder);
  // firstEncoder->setAccelerationMode(HWACCEL_SLOWER); // acceleration mode
  
  // interrupt for any
  // taskManager.setInterruptCallback(onInterrupt);
  

  // now we add the switches, we dont want the spinwheel button to repeat, so leave off the last parameter
  // which is the repeat interval (millis / 20 basically) Repeat button does repeat as we can see.
}

void setEncoderMax(int maximumEncoderValue){
  if(maximumEncoderValue > 0) switches.changeEncoderPrecision(maximumEncoderValue, 0);
  _maximumEncoderValue = maximumEncoderValue;
}

void checkAnalogSW(uint8_t pin, uint16_t value,uint32_t hold){
  // adc range / value 1024/500 = 2 states+ etc
  if(analogRead(pin) < value){
    encoderHasPress = true;
    delay(hold);
    if(analogRead(pin) < value) encoderHasHold = true;
  }
}

  // IoAbstractionRef iodev = switches.getIoAbstraction();
  // iodev->pinDirection(7,OUTPUT);
  // bool res = iodev->runLoop();
  // if(res!=1) Logger.println("devsync: " + (String)res);
  // taskManager.runLoop();
  // if(!res){
  //   Logger.println("error");
  //   delay(500);
  // }
#endif