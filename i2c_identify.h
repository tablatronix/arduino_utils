I2CDiscoverDevice

enum {
  DEVICE_UNKNOWN = 0,
  DEVICE_SSD1306,
  DEVICE_SH1106,
  DEVICE_VL53L0X,
  DEVICE_BMP180,
  DEVICE_BMP280,
  DEVICE_BME280,
  DEVICE_MPU6000,
  DEVICE_MPU9250,
  DEVICE_MCP9808,
  DEVICE_LSM6DS3,
  DEVICE_ADXL345,
  DEVICE_ADS1115,
  DEVICE_MAX44009,
  DEVICE_MAG3110,
  DEVICE_CCS811,
  DEVICE_HTS221,
  DEVICE_LPS25H,
  DEVICE_LSM9DS1,
  DEVICE_LM8330,
  DEVICE_DS3231,
  DEVICE_LIS3DH,
  DEVICE_LIS3DSH,
  DEVICE_INA219,
  DEVICE_SHT3X,
  DEVICE_HDC1080,
  DEVICE_MPU6886,
  DEVICE_BME680
};

int I2CReadRegister(BBI2C *pI2C, uint8_t iAddr, uint8_t u8Register, uint8_t *pData, int iLen){

}

// Figure out what device is at that address
// returns the enumerated value
//
int I2CDiscoverDevice(BBI2C *pI2C, uint8_t i)
{
uint8_t j, cTemp[8];
int iDevice = DEVICE_UNKNOWN;

  if (i == 0x3c || i == 0x3d) // Probably an OLED display
  {
    I2CReadRegister(pI2C, i, 0x00, cTemp, 1);
    cTemp[0] &= 0xbf; // mask off power on/off bit
    if (cTemp[0] == 0x8) // SH1106
       iDevice = DEVICE_SH1106;
    else if (cTemp[0] == 3 || cTemp[0] == 6)
       iDevice = DEVICE_SSD1306;
    return iDevice;
  }
  if (i >= 0x40 && i <= 0x4f) // check for TI INA219 power measurement sensor
  {
    I2CReadRegister(pI2C, i, 0x00, cTemp, 2);
    if (cTemp[0] == 0x39 && cTemp[1] == 0x9f)
       return DEVICE_INA219;
  }
//  else if (i == 0x5b) // MLX90615?
//  {
//    I2CReadRegister(pI2C, i, 0x10, cTemp, 3);
//    for (j=0; j<3; j++) Serial.println(cTemp[j], HEX);
//  }
  // try to identify it from the known devices using register contents
  {
    // Check for TI HDC1080
    I2CReadRegister(pI2C, i, 0xff, cTemp, 2);
    if (cTemp[0] == 0x10 && cTemp[1] == 0x50)
       return DEVICE_HDC1080;

    // Check for BME680
    if (i == 0x76 || i == 0x77)
    {
       I2CReadRegister(pI2C, i, 0xd0, cTemp, 1); // chip ID
       if (cTemp[0] == 0x61) // BME680
          return DEVICE_BME680;
    }
    // Check for VL53L0X
    I2CReadRegister(pI2C, i, 0xc0, cTemp, 3);
    if (cTemp[0] == 0xee && cTemp[1] == 0xaa && cTemp[2] == 0x10)
       return DEVICE_VL53L0X;

    // Check for CCS811
    I2CReadRegister(pI2C, i, 0x20, cTemp, 1);
    if (cTemp[0] == 0x81) // Device ID
       return DEVICE_CCS811;

    // Check for LIS3DSH accelerometer from STMicro
    I2CReadRegister(pI2C, i, 0x0f, cTemp, 1);
    if (cTemp[0] == 0x3f) // WHO_AM_I
       return DEVICE_LIS3DSH;

    // Check for LIS3DH accelerometer from STMicro
    I2CReadRegister(pI2C, i, 0x0f, cTemp, 1);
    if (cTemp[0] == 0x33) // WHO_AM_I
       return DEVICE_LIS3DH;

    // Check for LSM9DS1 magnetometer/gyro/accel sensor from STMicro
    I2CReadRegister(pI2C, i, 0x0f, cTemp, 1);
    if (cTemp[0] == 0x68) // WHO_AM_I
       return DEVICE_LSM9DS1;

    // Check for LPS25H pressure sensor from STMicro
    I2CReadRegister(pI2C, i, 0x0f, cTemp, 1);
    if (cTemp[0] == 0xbd) // WHO_AM_I
       return DEVICE_LPS25H;
    
    // Check for HTS221 temp/humidity sensor from STMicro
    I2CReadRegister(pI2C, i, 0x0f, cTemp, 1);
    if (cTemp[0] == 0xbc) // WHO_AM_I
       return DEVICE_HTS221;
    
    // Check for MAG3110
    I2CReadRegister(pI2C, i, 0x07, cTemp, 1);
    if (cTemp[0] == 0xc4) // WHO_AM_I
       return DEVICE_MAG3110;

    // Check for LM8330 keyboard controller
    I2CReadRegister(pI2C, i, 0x80, cTemp, 2);
    if (cTemp[0] == 0x0 && cTemp[1] == 0x84) // manufacturer code + software revision
       return DEVICE_LM8330;

    // Check for MAX44009
    if (i == 0x4a || i == 0x4b)
    {
      for (j=0; j<8; j++)
        I2CReadRegister(pI2C, i, j, &cTemp[j], 1); // check for power-up reset state of registers
      if ((cTemp[2] == 3 || cTemp[2] == 2) && cTemp[6] == 0 && cTemp[7] == 0xff)
         return DEVICE_MAX44009;
    }
       
    // Check for ADS1115
    I2CReadRegister(pI2C, i, 0x02, cTemp, 2); // Lo_thresh defaults to 0x8000
    I2CReadRegister(pI2C, i, 0x03, &cTemp[2], 2); // Hi_thresh defaults to 0x7fff
    if (cTemp[0] == 0x80 && cTemp[1] == 0x00 && cTemp[2] == 0x7f && cTemp[3] == 0xff)
       return DEVICE_ADS1115;

    // Check for MCP9808
    I2CReadRegister(pI2C, i, 0x06, cTemp, 2); // manufacturer ID && get device ID/revision
    I2CReadRegister(pI2C, i, 0x07, &cTemp[2], 2); // need to read them individually
    if (cTemp[0] == 0 && cTemp[1] == 0x54 && cTemp[2] == 0x04 && cTemp[3] == 0x00)
       return DEVICE_MCP9808;
       
    // Check for BMP280/BME280
    I2CReadRegister(pI2C, i, 0xd0, cTemp, 1);
    if (cTemp[0] == 0x55) // BMP180
       return DEVICE_BMP180;
    else if (cTemp[0] == 0x58)
       return DEVICE_BMP280;
    else if (cTemp[0] == 0x60) // BME280
       return DEVICE_BME280;

    // Check for LSM6DS3
    I2CReadRegister(pI2C, i, 0x0f, cTemp, 1); // WHO_AM_I
    if (cTemp[0] == 0x69)
       return DEVICE_LSM6DS3;
       
    // Check for ADXL345
    I2CReadRegister(pI2C, i, 0x00, cTemp, 1); // DEVID
    if (cTemp[0] == 0xe5)
       return DEVICE_ADXL345;
       
    // Check for MPU-60x0i, MPU-688x, and MPU-9250
    I2CReadRegister(pI2C, i, 0x75, cTemp, 1);
    if (cTemp[0] == (i & 0xfe)) // Current I2C address (low bit set to 0)
       return DEVICE_MPU6000;
    else if (cTemp[0] == 0x71)
       return DEVICE_MPU9250;
    else if (cTemp[0] == 0x19)
       return DEVICE_MPU6886;

    // Check for DS3231 RTC
    I2CReadRegister(pI2C, i, 0x0e, cTemp, 1); // read the control register
    if (i == 0x68 && cTemp[0] == 0x1c) // fixed I2C address and power on reset value  
       return DEVICE_DS3231;
  }
  return iDevice;
} /* I2CDiscoverDevice() */