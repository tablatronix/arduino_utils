#ifndef log_h
#define log_h

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
    void        DEBUG_WM(Generic text);

    template <typename Generic>
    void        DEBUG_WM(debuglevel_t level,Generic text);
    template <typename Generic, typename Genericb>
    void        DEBUG_WM(Generic text,Genericb textb);
    template <typename Generic, typename Genericb>
    void        DEBUG_WM(debuglevel_t level, Generic text,Genericb textb);


// DEBUG
// @todo fix DEBUG_WM(0,0);
template <typename Generic>
void DEBUG_WM(Generic text) {
  DEBUG_WM(DEBUG_NOTIFY,text,"");
}

template <typename Generic>
void DEBUG_WM(debuglevel_t level,Generic text) {
  if(_debugLevel >= level) DEBUG_WM(level,text,"");
}

template <typename Generic, typename Genericb>
void DEBUG_WM(Generic text,Genericb textb) {
  DEBUG_WM(DEBUG_NOTIFY,text,textb);
}

template <typename Generic, typename Genericb>
void DEBUG_WM(debuglevel_t level,Generic text,Genericb textb) {
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
  _debugPort.print("*WM: ");
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
    DEBUG_WM(F("getCoreVersion():         "),ESP.getCoreVersion());
    DEBUG_WM(F("system_get_sdk_version(): "),system_get_sdk_version());
    DEBUG_WM(F("system_get_boot_version():"),system_get_boot_version());
    DEBUG_WM(F("getFreeHeap():            "),(String)ESP.getFreeHeap());
  #elif defined(ESP32)
    size_t freeHeap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    DEBUG_WM("Free heap:       ", freeHeap);
    DEBUG_WM("ESP-IDF version: ", esp_get_idf_version());
  #endif
}

#endif
