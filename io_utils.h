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
    Serial.print("[PIN] " + (String)pin);
    Serial.print(" pinmode: ");
    Serial.print(getPinMode(pin),HEX);
    Serial.println(" pinstate: " + (String)digitalRead(pin));  
}

void scani2c(){
  byte error, address;
  int nDevices;
  Wire.begin();
  Serial.print("[I2C] SDA:"+(String) SDA);
  Serial.print(" SCL:"+(String) SCL);
  Serial.println("");
  Serial.println("[I2C] Scanning ... ");

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
      Serial.print("[I2C] Device found - ADDR: 0x");
      if (address<16)
        Serial.print("0");
        Serial.print(address,HEX);
        Serial.println("");
        nDevices++;
    }
    else if(res!=2)
    {
      Serial.println("[ERROR]:" + (String)res);
      Serial.print("Unknown error ADDR: 0x");
      if (address<16)
        Serial.print("0");
        Serial.print(address,HEX);
        Serial.println("");
    }
  }
  if (nDevices == 0)
    Serial.println("[ERROR] No I2C devices found\n");
  else
    Serial.println("[I2C] scan done\n");
}

void scanPins(){
  for(int i = 0;i<6;i++){
    if(i == 1) continue;
    pinMode(i,INPUT_PULLUP);
    Serial.print((String)digitalRead(i));
    // debugPin(i);
  }
  for(int i = 12;i<17;i++){
    pinMode(i,INPUT_PULLUP);
    // debugPin(i);
    Serial.print((String)digitalRead(i));
  }
  Serial.println("");
}

#endif