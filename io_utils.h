#ifndef io_utils_h
#define io_utils_h

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
    Serial.print("pinmode: ");
    Serial.println(getPinMode(pin),HEX);
    Serial.println("pinstate: " + (String)digitalRead(pin));  
}

#endif