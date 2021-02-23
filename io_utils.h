#ifndef io_utils_h
#define io_utils_h

#include <log.h>
#include <Wire.h>

bool swap = false;

int getPinMode(uint8_t pin)
{
  if (pin >= NUM_DIGITAL_PINS) return (-1);

  uint8_t bit = digitalPinToBitMask(pin);
  uint32_t port = digitalPinToPort(pin);
  volatile uint32_t *reg = portModeRegister(port);
  if (*reg & bit) return (OUTPUT); // 0x01

  volatile uint32_t *out = portOutputRegister(port);
  return ((*out & bit) ? INPUT_PULLUP : INPUT); // 0x00
}

void debugPin(uint8_t pin){
    Logger.print("[PIN] " + (String)pin);
    Logger.print(" pinmode: ");
    Logger.print(getPinMode(pin),HEX);
    Logger.println(" pinstate: " + (String)digitalRead(pin));  
}

void scani2c(bool pinswap = false){
  swap = pinswap;
  byte error, address;
  int nDevices;
  if(!swap)Wire.begin();
  else Wire.begin(SCL,SDA);  // begin(sda, scl) SWAP!
  Logger.print("[I2C] SDA:"+(String) SDA);
  Logger.print(" SCL:"+(String) SCL);
  Logger.println(swap ? " \nSWAPPED" : "");
  Logger.println("[I2C] Scanning ... ");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    int res = Wire.endTransmission();
 
 // * Output   0 .. success
 // *          1 .. length to long for buffer
 // *          2 .. address send, NACK received
 // *          3 .. data send, NACK received
 // *          4 .. other twi error (lost bus arbitration, bus error, ..)
 
    if (res == 0)
    {
      Logger.print("[I2C] Device found - ADDR: 0x");
      if (address<16)
        Logger.print("0x");
        Logger.print(address,HEX); // 7 bit
        Logger.print(" 0x");
        Logger.print(2*address,HEX); // 8bit
        Logger.println("");
        nDevices++;
    }
    else if(res!=2)
    {
      Logger.println("[ERROR]:" + (String)res);
      Logger.print("Unknown error ADDR: 0x");
      if (address<16)
        Logger.print("0");
        Logger.print(address,HEX);
        Logger.println("");
    }
  }
  if (nDevices == 0)
    Logger.println("[ERROR] No I2C devices found\n");
  else
    Logger.print("[I2C] scan done found ");
    Logger.println(nDevices);
}

void scanPins(){
  for(int i = 0;i<6;i++){
    if(i == 1) continue;
    pinMode(i,INPUT_PULLUP);
    Logger.print((String)digitalRead(i));
    // debugPin(i);
  }
  for(int i = 12;i<17;i++){
    pinMode(i,INPUT_PULLUP);
    // debugPin(i);
    Logger.print((String)digitalRead(i));
  }
  Logger.println("");
}

void pinregister(){
  // maintain a store of pins we are using before passing them to functions
  // better than a simple define, as we can 
  // register a pin for use, to avoid using a pin twice
  // even if this is a preocompiler structure, it would stil be able to warn
  // also can let you set the use as a flag so you know if you are using it as adc or input
  // and on esp32 etc set the pins desired specs and avoid conflicts with multiple adc reads
}

#ifdef ESP32

#define ADC_1 36
#define ADC_2 38 // missing from some boards?
#define ADC_3 39
#define ADC_4 32
#define ADC_5 33
#define ADC_6 34
#define ADC_7 37

// ADC2 restoring procedure
// This is a Workaround to use ADC2 Pins on ESP32 when Wifi or Bluetooth is on.
// (usually only ADC1 Pins are usable for analogRead() when Wifi or Bluetooth is on.)
// -- We save the ADC2 control register before WifiBegin() or BluetoothBegin()
// -- then restore its original value, then set a specific bit of the ADC2 control register   
//to avoid inverted readings: we do the latter two before every analogRead() cycle.
// -- This achieves ADC2 usability even when Wifi/Bluetooth are turned on!

#include "esp32-hal-adc.h" // needed for adc pin reset
#include "soc/sens_reg.h" // needed for manipulating ADC2 control register
uint64_t reg_b; // Used to store ADC2 control register
int value;

void storeADC() {
// Save ADC2 control register value : Do this before begin Wifi/Bluetooth
reg_b = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);
// Wifi.Begin(); // or similar wifi init function or Bluetooth begin()
}

void restoreADC() {
// ADC2 control register restoring
WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, reg_b);
//VERY IMPORTANT: DO THIS TO NOT HAVE INVERTED VALUES!
SET_PERI_REG_MASK(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_DATA_INV);
Logger.println(analogRead(4));
Logger.println(analogRead(33));
}
#endif

#endif