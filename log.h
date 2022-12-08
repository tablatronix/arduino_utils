#ifndef log_h
#define log_h

/**
 * Local logging solutions, using steamutils and syslog
 * StreamUtils
 * https://github.com/Chris--A/PrintEx
 * Allows for stream buffering, redirection, mirroring etc.
 *
 * syslog
 * https://github.com/arcao/Syslog.git
 * An Arduino library for logging to Syslog server via `UDP` protocol in 
 * [IETF (RFC 5424)] and [BSD (RFC 3164)] message format
 */

#define USESYSLOG

#ifdef USESYSLOG
#include <Syslog.h>
#include <log_syslog.h>
#endif

/**
 * syslog preparer
 * @todo, streamutils should have buffering capabilities already, replace our buffer
 * @param msg [description]
 */

char logbuffer[256];
int  logbufferidx = 0; // end char
// might need a ring buffer here, so we can keep logging mutiple messages and send delayed
bool syslogactive = true;

void sendToSyslog(String msg){
  #ifdef USESYSLOG
  // if(logData.indexOf("\n") >= 0){
    // Serial.println("newline found at: " + (String)logData.indexOf("\n"));
    // syslog.log(LOG_INFO,msg);
  // }
  // Serial.print("[RAW] ");
  // Serial.println(String(logbuffer).substring(0,logbufferidx));
  
  // Log to buffer, do string matching for LOG LEVEL
  msg = String(logbuffer).substring(0,logbufferidx-2);
  String msgb = msg;
  msgb.toLowerCase(); // lowercase for string match levels
  msgb.trim(); // remove cr lf

  uint16_t level = LOG_INFO;
  if(msgb.indexOf("notice")  != -1)  level = LOG_NOTICE;
  if(msgb.indexOf("error")   != -1)  level = LOG_ERR;
  if(msgb.indexOf("warning") != -1)  level = LOG_WARNING;
  if(msgb.indexOf("fatal")   != -1)  level = LOG_CRIT;
  if(syslogactive) syslog.log(level,msg); // SEND IT
  // if(syslogactive) syslog.log(level,(String)millis()+" "+msg);
  // @todo clean up string, remove whitespace such as CR LF \t
  
  // reset buffer
  logbuffer[0] = (char)0;
  logbufferidx = 0;
  #endif
}


/**
 * setup logger stream wrapper, redirection
 */
class print2syslog : public Stream {
public:
  bool begun = true; // allow muting via availability
  virtual int available() { return (begun); }
  virtual int read() { return (0); }
  virtual int peek() { return (0); }
  virtual void flush() {}
  void begin(unsigned long, uint8_t) {begun = true;}
  void end() {begun=false;}

  virtual size_t write(uint8_t newchar) {
    if(!begun) return 0;
    logbuffer[logbufferidx] = newchar;
    logbufferidx++;
    if(logbufferidx>256 || newchar == 0x0a) sendToSyslog(""); // buffer full or newline, @todo if no newline at max buffer drop it, use flag for delayed write
    return (1);
  }

  virtual size_t write(const uint8_t *buffer, size_t size)
  {
      if(!begun) return 0;
      size_t n = 0;
      while(size--) {
          n += write(*buffer++);
      }
      return n;
  }
};

print2syslog syslogger;
// syslogger.begin();

/**
 * Setup streamutils stream redirection/mirroring
 * @todo abstract into init class
 *
 * This library provides some helper classes and functions for dealing with streams. 
 * For example, with this library, you can:
 * speed of your program by buffering the data it reads from a file
 * reduce the number of packets sent over WiFi by buffering the data you send
 * improve the reliability of a serial connection by adding error correction codes
 * debug your program more easily by logging what it sends to a Web service
 * send large data with the [Wire library](https://www.arduino.cc/en/reference/wire)
 * use a `String` or EEPROM with a stream interface
 * 
 * Logger +
 *        |-> syslogger -> syslog UDP netlog
 *        |-> Serial -> (uart/usbserial)
 */
#include <StreamUtils.h>

Stream &_Logger = Serial;
LoggingStream Logger(syslogger, _Logger);
// Stream &Logger = Serial; // BYPASS lOGGER

// #ifdef USESYSLOG
// LoggingStream Logger(syslogger, _Logger);
// #else
  // Stream &Logger = Serial; // BYPASS lOGGER
// #endif

// logging streams options
// String result = LoggingStream.str(); 
// WriteLoggingStream loggingClient(client, Serial);
// loggingClient.println("GET / HTTP/1.1");
// loggingClient.println("User-Agent: Arduino");


/**
 * Basic Debug logging methods
 * DEBUGGER(debuglevel_t LOG_LEVEL,F(msg));
 * DEBUGGER(debuglevel_t LOG_LEVEL,F(msg),value); // for msg value pairs
 *
 * #DEBUG_LEVEL
 * #DEBUG_PORT
 *
 * PREFIX:[LOGLEVEL] msg
 * MAX will output timestamps and memory info
 */


#include <memory>

#ifdef ESP8266
#include <core_version.h>
    extern "C" {
      #include "user_interface.h"
    }
#endif

    // debugging
    typedef enum {
        DEBUG_ERROR     = 0,
        DEBUG_NOTIFY    = 1, // default stable
        DEBUG_VERBOSE   = 2,
        DEBUG_DEV       = 3, // default dev
        DEBUG_MAX       = 4
    } debuglevel_t;

    boolean _debug  = true;
    
    // build debuglevel support
    // @todo use DEBUG_ESP_x?
    #ifdef DEBUG_LEVEL
    uint8_t _debugLevel = (uint8_t)DEBUG_LEVEL;
    #else 
    uint8_t _debugLevel = DEBUG_DEV; // default debug level
    #endif

    // @todo use DEBUG_ESP_PORT ?
    #ifdef DEBUG_PORT
    Stream& _debugPort = DEBUG_PORT;
    #else
    Stream& _debugPort = Serial; // debug output stream ref
    #endif

    template <typename Generic>
    void        DEBUGGER(Generic text);

    template <typename Generic>
    void        DEBUGGER(debuglevel_t level,Generic text);
    template <typename Generic, typename Genericb>
    void        DEBUGGER(Generic text,Genericb textb);
    template <typename Generic, typename Genericb>
    void        DEBUGGER(debuglevel_t level, Generic text,Genericb textb);


// DEBUG
// @todo fix DEBUGGER(0,0);
template <typename Generic>
void DEBUGGER(Generic text) {
  DEBUGGER(DEBUG_NOTIFY,text,"");
}

template <typename Generic>
void DEBUGGER(debuglevel_t level,Generic text) {
  if(_debugLevel >= level) DEBUGGER(level,text,"");
}

template <typename Generic, typename Genericb>
void DEBUGGER(Generic text,Genericb textb) {
  DEBUGGER(DEBUG_NOTIFY,text,textb);
}

template <typename Generic, typename Genericb>
void DEBUGGER(debuglevel_t level,Generic text,Genericb textb) {
  if(!_debug || _debugLevel < level) return;

  if(_debugLevel >= DEBUG_MAX){
    uint32_t free;
    uint16_t max;
    uint8_t frag;
    #ifdef ESP8266
    ESP.getHeapStats(&free, &max, &frag);
    _debugPort.printf("[MEM] free: %5d | max: %5d | frag: %3d%% \n", free, max, frag);
    #elif defined ESP32
    // total_free_bytes;      ///<  Total free bytes in the heap. Equivalent to multi_free_heap_size().
    // total_allocated_bytes; ///<  Total bytes allocated to data in the heap.
    // largest_free_block;    ///<  Size of largest free block in the heap. This is the largest malloc-able size.
    // minimum_free_bytes;    ///<  Lifetime minimum free heap size. Equivalent to multi_minimum_free_heap_size().
    // allocated_blocks;      ///<  Number of (variable size) blocks allocated in the heap.
    // free_blocks;           ///<  Number of (variable size) free blocks in the heap.
    // total_blocks;          ///<  Total number of (variable size) blocks in the heap.
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
    free = info.total_free_bytes;
    max  = info.largest_free_block;
    frag = 100 - (max * 100) / free;
    _debugPort.printf("[MEM] free: %5d | max: %5d | frag: %3d%% \n", free, max, frag);    
    #endif
  }
  _debugPort.print("*DLOG: ");
  if(_debugLevel == DEBUG_DEV) _debugPort.print("["+(String)level+"] ");
  _debugPort.print(text);
  if(textb){
    _debugPort.print(" ");
    _debugPort.print(textb);
  }
  _debugPort.println();
}


/**
 * [debugPlatformInfo description]
 * @access public
 * @return {[type]} [description]
 */
void debugPlatformInfo(){
  #ifdef ESP8266
    system_print_meminfo();
    DEBUGGER(F("getCoreVersion():         "),ESP.getCoreVersion());
    DEBUGGER(F("system_get_sdk_version(): "),system_get_sdk_version());
    DEBUGGER(F("system_get_boot_version():"),system_get_boot_version());
    DEBUGGER(F("getFreeHeap():            "),(String)ESP.getFreeHeap());
  #elif defined(ESP32)
    size_t freeHeap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    DEBUGGER("Free heap:       ", freeHeap);
    DEBUGGER("ESP-IDF version: ", esp_get_idf_version());
    // log_v("Chip Info: Model: %d, cores: %d, revision: %d", chipInfo.model, chipInfo.cores, chipInfo.revision);
  #endif
}

#endif
