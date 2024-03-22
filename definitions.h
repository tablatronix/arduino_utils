
// ESP32
//  SPI    MOSI    MISO    SCLK    CS
// VSPI    GPIO 23 GPIO 19 GPIO 18 GPIO 5
// HSPI    GPIO 13 GPIO 12 GPIO 14 GPIO 15
// setSpiPin(14, 12, 13, 15);


// BASICS2
// #define SPIBUS 2
// #define SPIIOCLK 12
// #define SPIIOMISO 13
// #define SPIIOMOSI 11
// #define SPIIOCS0  10

#define ESP32BASICUSB

#ifdef ESP32BASICUSB
typedef enum {
    INDLEDPIN = 19,
    INDLEDISRGBW = 1,
    IOE1 = 32,
    IOE2 = 33,
    IOE3 = 36, // ADC
    IOE4 = 39, // ADC
    ADCEX = 27,
    BUTTON1 = 0,
    SPIBUS = 2,
} PIN_DEFS;
#elif defined(ESP32BASICS3)

// Controller SPI0 is reserved for caching external memories like Flash
// Controller SPI1 is reserved for external memories like PSRAM
// Controller SPI2 can be used as general purpose SPI (GPSPI) - SPI2_HOST (also called FSPI or HSPI)
// Controller SPI3 can be used as general purpose SPI (GPSPI) - SPI3_HOST (also called VSPI)

typedef enum {
    INDLEDPIN = 19,
    INDLEDISRGBW = 1,
    IOE1 = 32,
    IOE2 = 33,
    IOE3 = 36, // ADC
    IOE4 = 39, // ADC
    ADCEX = 27,
    BUTTON1 = 0,
    SPIBUS = 2,
    SPIIOCLK = 13,
    SPIIOMISO = 12,
    SPIIOMOSI = 11,
    SPIIOCS0  = 10,
} PIN_DEFS;
#else
typedef enum {
    INDLEDPIN = 19,
    INDLEDISRGBW = 1,
    IOE1 = 1,
    IOE2 = 2,
    IOE3 = 3,
    IOE4 = 6,
    ADCEX = 14,
    BUTTON1 = 0,
    SPIBUS = 2,
    JTAG1 = 39, // TCK
    JTAG2 = 40, // TDO
    JTAG3 = 41, // TDI
    JTAG4 = 42, // TMS
} PIN_DEFS;
#endif

