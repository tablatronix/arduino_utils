  // Escape any special HTML (unsafe) characters in a string. e.g. anti-XSS.
  // Args:
  //   unescaped: A string containing text to make HTML safe.
  // Returns:
  //   A string that is HTML safe.
  String htmlEscape(const String unescaped) {
    String result = "";
    uint16_t ulen = unescaped.length();
    result.reserve(ulen);  // The result will be at least the size of input.
    for (size_t i = 0; i < ulen; i++) {
      char c = unescaped[i];
      switch (c) {
        // ';!-"<>=&#{}() are all unsafe.
        case '\'':
          result += F("&apos;");
          break;
        case ';':
          result += F("&semi;");
          break;
        case '!':
          result += F("&excl;");
          break;
        case '-':
          result += F("&dash;");
          break;
        case '\"':
          result += F("&quot;");
          break;
        case '<':
          result += F("&lt;");
          break;
        case '>':
          result += F("&gt;");
          break;
        case '=':
          result += F("&#equals;");
          break;
        case '&':
          result += F("&amp;");
          break;
        case '#':
          result += F("&num;");
          break;
        case '{':
          result += F("&lcub;");
          break;
        case '}':
          result += F("&rcub;");
          break;
        case '(':
          result += F("&lpar;");
          break;
        case ')':
          result += F("&rpar;");
          break;
        default:
          result += c;
      }
    }
    return result;
  }

String uint64ToString(uint64_t input, uint8_t base = 10);

// Convert a uint64_t (unsigned long long) to a string.
// Arduino String/toInt/Serial.print() can't handle printing 64 bit values.
//
// Args:
//   input: The value to print
//   base:  The output base.
// Returns:
//   A string representation of the integer.
// Note: Based on Arduino's Print::printNumber()
String uint64ToString(uint64_t input, uint8_t base) {
  String result = "";
  // prevent issues if called with base <= 1
  if (base < 2) base = 10;
  // Check we have a base that we can actually print.
  // i.e. [0-9A-Z] == 36
  if (base > 36) base = 10;

  // Reserve some string space to reduce fragmentation.
  // 16 bytes should store a uint64 in hex text which is the likely worst case.
  // 64 bytes would be the worst case (base 2).
  result.reserve(16);

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c += '0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}

#ifdef ARDUINO
// Print a uint64_t/unsigned long long to the Serial port
// Serial.print() can't handle printing long longs. (uint64_t)
//
// Args:
//   input: The value to print
//   base: The output base.
void serialPrintUint64(uint64_t input, uint8_t base) {
  Serial.print(uint64ToString(input, base));
}
#endif

#define D_CHR_TIME_SEP ":"
#define D_STR_DAY "Day"
#define D_STR_DAYS D_STR_DAY "s"
#define D_STR_HOUR "Hour"
#define D_STR_HOURS D_STR_HOUR "s"
#define D_STR_MINUTE "Minute"
#define D_STR_MINUTES D_STR_MINUTE "s"
#define D_STR_SECOND "Second"
#define D_STR_SECONDS D_STR_SECOND "s"
#define D_STR_NOW " - "

const PROGMEM char* kTimeSep    = D_CHR_TIME_SEP;
const PROGMEM char* kDayStr     = D_STR_DAY;
const PROGMEM char* kDaysStr    = D_STR_DAYS;
const PROGMEM char* kHourStr    = D_STR_HOUR;
const PROGMEM char* kHoursStr   = D_STR_HOURS;
const PROGMEM char* kMinuteStr  = D_STR_MINUTE;
const PROGMEM char* kMinutesStr = D_STR_MINUTES;
const PROGMEM char* kSecondStr  = D_STR_SECOND;
const PROGMEM char* kSecondsStr = D_STR_SECONDS;
const PROGMEM char* kNowStr     = D_STR_NOW;

String getTimerMS(uint32_t const msecs){
    uint32_t totalseconds = msecs / 1000;
    if (totalseconds == 0) return kNowStr;

    // Note: uint32_t can only hold up to 45 days, so uint8_t is safe.
    uint8_t days = totalseconds / (60 * 60 * 24);
    uint8_t hours = (totalseconds / (60 * 60)) % 24;
    uint8_t minutes = (totalseconds / 60) % 60;
    uint8_t seconds = totalseconds % 60; 
    uint8_t mils    = msecs % 100;
    String result = "";

    if(minutes < 10) result += "0";
    result += uint64ToString(minutes) + kTimeSep;
    if(seconds < 10) result += "0";
    result += uint64ToString(seconds) + kTimeSep;
    if(mils < 10) result += "0";
    result += (String)mils;
    return result;
}

String getTimer(uint32_t const msecs){
    uint32_t totalseconds = msecs / 1000;
    if (totalseconds == 0) return kNowStr;

    // Note: uint32_t can only hold up to 45 days, so uint8_t is safe.
    uint8_t days = totalseconds / (60 * 60 * 24);
    uint8_t hours = (totalseconds / (60 * 60)) % 24;
    uint8_t minutes = (totalseconds / 60) % 60;
    uint8_t seconds = totalseconds % 60; 
    uint8_t mils    = msecs % 100;
    String result = "";

    if(minutes < 10) result += "0";
    result += uint64ToString(minutes) + kTimeSep;
    if(seconds < 10) result += "0";
    result += uint64ToString(seconds);
    return result;
}

  String msToString(uint32_t const msecs) {
    uint32_t totalseconds = msecs / 1000;
    if (totalseconds == 0) return kNowStr;

    // Note: uint32_t can only hold up to 45 days, so uint8_t is safe.
    uint8_t days = totalseconds / (60 * 60 * 24);
    uint8_t hours = (totalseconds / (60 * 60)) % 24;
    uint8_t minutes = (totalseconds / 60) % 60;
    uint8_t seconds = totalseconds % 60;

    String result = "";
    if (days)
      result += uint64ToString(days) + ' ' + ((days > 1) ? "kDaysStr" : kDayStr);
    if (hours) {
      if (result.length()) result += ' ';
      result += uint64ToString(hours) + ' ' + ((hours > 1) ? kHoursStr
                                                           : kHourStr);
    }
    if (minutes) {
      if (result.length()) result += ' ';
      result += uint64ToString(minutes) + ' ' + ((minutes > 1) ? kMinutesStr
                                                               : kMinuteStr);
    }
    if (seconds) {
      if (result.length()) result += ' ';
      result += uint64ToString(seconds) + ' ' + ((seconds > 1) ? kSecondsStr
                                                               : kSecondStr);
    }
    return result;
  }

  String minsToString(const uint16_t mins) {
    String result = "";
    result.reserve(5);  // 23:59 is the typical worst case.
    if (mins / 60 < 10) result += '0';  // Zero pad the hours
    result += uint64ToString(mins / 60) + kTimeSep;
    if (mins % 60 < 10) result += '0';  // Zero pad the minutes.
    result += uint64ToString(mins % 60);
    return result;
  }
