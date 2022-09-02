#include <Average.h>           // https:github.com/MajenkoLibraries/Average


#include <log.h>
#include <Syslog.h>            //https://github.com/arcao/Syslog.git
#include <log_syslog.h>
#include <StreamUtils.h>       // https://github.com/bblanchon/ArduinoStreamUtils.git


#include <Genv_asset.h>

#include <Wire.h>

#include <OneWire.h>			// https://github.com/
#include <DallasTemperature.h>	// https://github.com/
#include <SoftwareSerial.h>		// https://github.com/
#include <PMserial.h>			// https://github.com/avaldebe/PMserial
#include <Adafruit_VEML6070.h>	// https://github.com/adafruit/Adafruit_VEML6070
#include <Adafruit_PCF8591.h>	// https://github.com/adafruit/Adafruit_PCF8591
#include <Adafruit_INA219.h>	// https://github.com/adafruit/Adafruit_INA219
#include <Adafruit_MPU6050.h>	// https://github.com/adafruit/Adafruit_MPU6050
#include <Adafruit_Sensor.h>	// https://github.com/adafruit/Adafruit_Sensor
#include <Adafruit_APDS9960.h>	// https://github.com/adafruit/Adafruit_APDS9960
#include <Adafruit_SHT31.h>		// https://github.com/adafruit/Adafruit_SHT31
#include <Adafruit_BMP280.h>	// https://github.com/adafruit/Adafruit_BMP280
#include <Adafruit_CCS811.h>	// https://github.com/adafruit/Adafruit_CCS811
#include <Adafruit_BMP085.h.h>	// https://github.com/adafruit/Adafruit_BMP085
#include <BH1750.h>				// https://github.com/claws/BH1750
#include <GP2YDustSensor.h>		// https://github.com/
#include <LM75A.h>				// https://github.com/QuentinCG/Arduino-LM75A-Temperature-Sensor-Library
#include <HTU21D.h> 			// https://github.com/adafruit/Adafruit_HTU21DF_Library
#include <SensirionI2CScd4x.h>	// https://github.com/

// #include <hp_BH1750.h>  //inlude the library
// 
// 
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
lib_deps =
	avaldebe/PMSerial@^1.2.0
	https://github.com/adafruit/Adafruit_INA219
	https://github.com/adafruit/Adafruit_MPU6050
	https://github.com/avaldebe/PMserial
	https://github.com/adafruit/Adafruit_VEML6070
	https://github.com/adafruit/Adafruit_PCF8591
	https://github.com/adafruit/Adafruit_INA219
	https://github.com/adafruit/Adafruit_Sensor
	https://github.com/adafruit/Adafruit_APDS9960
	https://github.com/adafruit/Adafruit_SHT31
	https://github.com/adafruit/Adafruit_BMP280
	https://github.com/adafruit/Adafruit_CCS811
	; https://github.com/adafruit/Adafruit_HTU21DF_Library
	; https://github.com/RobTillaart/SHT2x
	https://github.com/enjoyneering/HTU21D
	https://github.com/claws/BH1750
lib_extra_dirs = /Users/alverson/projects/microcontrollers/dev/libraries
