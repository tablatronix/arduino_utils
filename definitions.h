

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

