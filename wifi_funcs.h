#ifndef wifi_funcs_h
#define wifi_funcs_h

#include <creds.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #define WIFI_getChipId() ESP.getChipId()
#elif defined(ESP32)
    #include <rom/rtc.h>
    #include <WiFi.h>
    #include <esp_wifi.h>
    #include <ESPmDNS.h>
    #define WIFI_getChipId() (uint32_t)ESP.getEfuseMac()
#endif

// const char* hostname   = "esp8266REFLOW";

bool debug_wifi = true;

bool rebootAfterDowntime = true;
long downtimeRestart = 1*60000; // millis
long downms        = 0;

uint8_t _lastrssiperc = 0; // store rssi

/** IP to String? */
String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

String WiFi_SSID(bool persistent) {
    persistent = true;
    #ifdef ESP8266
    struct station_config conf;
    if(persistent) wifi_station_get_config_default(&conf);
    else wifi_station_get_config(&conf);

    char tmp[33]; //ssid can be up to 32chars, => plus null term
    memcpy(tmp, conf.ssid, sizeof(conf.ssid));
    tmp[32] = 0; //nullterm in case of 32 char ssid
    return String(reinterpret_cast<char*>(tmp));
    
    #elif defined(ESP32)
    if(persistent){
      wifi_config_t conf;
      esp_wifi_get_config(WIFI_IF_STA, &conf);
      return String(reinterpret_cast<const char*>(conf.sta.ssid));
    }
    else {
      if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
          return String();
      }
      wifi_ap_record_t info;
      if(!esp_wifi_sta_get_ap_info(&info)) {
          return String(reinterpret_cast<char*>(info.ssid));
      }
      return String();
    }
    #endif
}

String WiFi_psk(bool persistent)  {
    persistent = true;
    #ifdef ESP8266
    struct station_config conf;

    if(persistent) wifi_station_get_config_default(&conf);
    else wifi_station_get_config(&conf);

    char tmp[65]; //psk is 64 bytes hex => plus null term
    memcpy(tmp, conf.password, sizeof(conf.password));
    tmp[64] = 0; //null term in case of 64 byte psk
    return String(reinterpret_cast<char*>(tmp));
    
    #elif defined(ESP32)
    // only if wifi is init
    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
      return String();
    }
    wifi_config_t conf;
    esp_wifi_get_config(WIFI_IF_STA, &conf);
    return String(reinterpret_cast<char*>(conf.sta.password));
    #endif
}

bool wifiIsAutoConnect(){
  return WiFi_SSID(true) != "";  
}

bool wifiIsConnected(){
  return WiFi.status() == WL_CONNECTED;
}

void setWiFiFastConnect(){
  //fast scan
  // set channel
  // set bssid
  // WiFi.begin(SSID,PASS,WiFi.channel(),WiFi.BSSID(),true);
}

String getDeviecID(){
  String _wifissidprefix = "ESP";
  String hostString = String(WIFI_getChipId(),HEX);
  hostString.toUpperCase();
  // char hostString[16] = {0};
  // sprintf(hostString, "%06X", ESP.getChipId());  
  return _wifissidprefix + "_" + hostString;
}

void setWiFiHostname(const char* hostname){
  // @todo add string templ
  #ifdef ESP32
  WiFi.setHostname(hostname);
  #else
  MDNS.begin(hostname);
  WiFi.hostname(hostname);
  #endif
}

String getHostname(){
  #ifdef ESP32
  return WiFi.getHostname(); // getHostName ( @todo return string or c.str?)
  #else
  return WiFi.hostname(); // getHostName
  #endif
}

void WiFi_print_sta(){
    if(wifiIsConnected()){
      Serial.println("[WIFI] CONNECTED");
      Serial.print("[WIFI] IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("[WIFI] HOST: ");
      Serial.println(getHostname());
      Serial.print("[WIFI] BSSID: ");
      Serial.println(WiFi.BSSIDstr());
      Serial.print("[WIFI] RSSI: ");
      Serial.println(WiFi.RSSI());
      Serial.print("[WIFI] CHANNEL: ");
      Serial.println(WiFi.channel());         
    } else {
      Serial.println("[WIFI] NOT CONNECTED");
    }
}

// enable wifi sta
// disable sleep
// timeout connect
// set hostname ?
void init_WiFi(int timeout){
    // if(wifiIsConnected()){
    //   WiFi_print_sta();
    //   return;
    // }
    unsigned long start = millis();
    Serial.println("[WIFI] mode STA");
    WiFi.mode(WIFI_STA);
    if(debug_wifi) WiFi.printDiag(Serial);
    #ifdef ESP8266
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    #elif defined(ESP32)
    // btStop();
    WiFi.setSleep(false);
    #endif
// 
    // WiFi.hostname(hostname);
    // if(wifiIsAutoConnect)  WiFi.begin();
    // esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B| WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N|WIFI_PROTOCOL_LR);

    WiFi.begin(SSID,PASS);
    if(timeout > 0){
      // use local timer loop
      if(debug_wifi) Serial.println("[WIFI] Connecting to wifi, wait for timeout... [" + (String)timeout + " ms]\n");
      while((WiFi.status() != WL_CONNECTED) && (millis()-start < timeout)){
        if(debug_wifi) Serial.print(".");
        delay(100);
      }
    }
    else {
      // waitForConnecrtResult default 60000
      if(debug_wifi) Serial.println("[WIFI] Connecting to wifi, waitForConnectResult waiting..... ");
      uint8_t status = WiFi.waitForConnectResult();
      // while(status() != WL_CONNECTED){
      //   Serial.print(".");
      //   delay(500);
        Serial.println((String)WiFi.status());
      // }
    }

    if(debug_wifi) Serial.println("");

    if(wifiIsConnected()){
      setWiFiFastConnect();
      Serial.println("[WIFI] connected in " + (String)(millis()-start/1000) + " ms");
      if(debug_wifi) WiFi_print_sta();
    }
    else{
      Serial.println("[ERROR] WIFI CONNECT FAILED");
      Serial.println("[WIFI] waited for " + (String)(millis()-start/1000) + " ms");
      // delay(1000);
      // WiFi.begin(SSID,PASS);
    }
    delay(500);
}

void init_WiFi(){
  init_WiFi(0);
}

void init_WiFi_saved(){
  WiFi.begin();
}

uint8_t getRSSIasQuality(int8_t RSSI) {
  uint8_t quality = 0;

  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  _lastrssiperc = quality;
  return quality;
}

uint8_t getRSSIasQuality() {
  return getRSSIasQuality(WiFi.RSSI());
}

/**
 * [checkWifi description]
 * @param restart [description]
 */

void checkWifi(bool recon = true, bool restart = false){
  Serial.println("[TASK] checkWiFi");
  if(WiFi.status() != WL_CONNECTED  ){
    if(downms == 0) downms = millis();
    if(millis() > downms + downtimeRestart){
     if(restart){
        // reboot
        #ifdef USENEOIND
          indSetColor(np_red);
        #endif
        Serial.println("[ERROR] wifi not found, rebooting after timeout");
        Serial.flush();
        delay(1000);
        ESP.restart();
      }
      else{
        // reconnect
        #ifdef USENEOIND
          indSetColor(np_red);
        #endif
        Serial.println("[WIFI] WiFi is Disconnected for " + (String)(millis()-downms));
        downms = millis();
        if(recon) WiFi.reconnect();
      }
    }
  }
  else {
    _lastrssiperc = getRSSIasQuality();
    if(debug_wifi){
      Serial.println("[WIFI] WiFi is CONNECTED");
      Serial.println("[WIFI] RSSI: "+(String)_lastrssiperc);
    }
    #ifdef USENEOIND
      indSetColor(np_green);
    #endif
  }
}

void checkWifi(){
  checkWifi(true,false);
}

void enableWiFi(){
  WiFi.mode(WIFI_STA);
  init_WiFi(0);
}

void disableWiFi(){
  WiFi.mode(WIFI_OFF);
  #ifdef ESP32
  WiFi.mode( WIFI_MODE_NULL );
  btStop();
  #endif
}

// uint32_t ResetReason(void)
// {
  
//     user_interface.h
//     REASON_DEFAULT_RST      = 0,  // "Power on"                normal startup by power on
//     REASON_WDT_RST          = 1,  // "Hardware Watchdog"       hardware watch dog reset
//     REASON_EXCEPTION_RST    = 2,  // "Exception"               exception reset, GPIO status won’t change
//     REASON_SOFT_WDT_RST     = 3,  // "Software Watchdog"       software watch dog reset, GPIO status won’t change
//     REASON_SOFT_RESTART     = 4,  // "Software/System restart" software restart ,system_restart , GPIO status won’t change
//     REASON_DEEP_SLEEP_AWAKE = 5,  // "Deep-Sleep Wake"         wake up from deep-sleep
//     REASON_EXT_SYS_RST      = 6   // "External System"         external system reset
  
//   return ESP_ResetInfoReason();
// }

    // if (resetInfo.reason == REASON_DEFAULT_RST) { // normal startup by power on
    //   strcpy_P(buff, PSTR("Power on"));
    // } else if (resetInfo.reason == REASON_WDT_RST) { // hardware watch dog reset
    //   strcpy_P(buff, PSTR("Hardware Watchdog"));
    // } else if (resetInfo.reason == REASON_EXCEPTION_RST) { // exception reset, GPIO status won’t change
    //   strcpy_P(buff, PSTR("Exception"));
    // } else if (resetInfo.reason == REASON_SOFT_WDT_RST) { // software watch dog reset, GPIO status won’t change
    //   strcpy_P(buff, PSTR("Software Watchdog"));
    // } else if (resetInfo.reason == REASON_SOFT_RESTART) { // software restart ,system_restart , GPIO status won’t change
    //   strcpy_P(buff, PSTR("Software/System restart"));
    // } else if (resetInfo.reason == REASON_DEEP_SLEEP_AWAKE) { // wake up from deep-sleep
    //   strcpy_P(buff, PSTR("Deep-Sleep Wake"));
    // } else if (resetInfo.reason == REASON_EXT_SYS_RST) { // external system reset
    //   strcpy_P(buff, PSTR("External System"));
    // } else {
    //   strcpy_P(buff, PSTR("Unknown"));

// #ifdef ESP32
// String ESP32GetResetReason(uint32_t cpu_no) {
//   // tools\sdk\include\esp32\rom\rtc.h
//   switch (rtc_get_reset_reason( (RESET_REASON) cpu_no)) {
//     case POWERON_RESET          : return F("Vbat power on reset");                              // 1
//     case SW_RESET               : return F("Software reset digital core");                      // 3
//     case OWDT_RESET             : return F("Legacy watch dog reset digital core");              // 4
//     case DEEPSLEEP_RESET        : return F("Deep Sleep reset digital core");                    // 5
//     case SDIO_RESET             : return F("Reset by SLC module, reset digital core");          // 6
//     case TG0WDT_SYS_RESET       : return F("Timer Group0 Watch dog reset digital core");        // 7
//     case TG1WDT_SYS_RESET       : return F("Timer Group1 Watch dog reset digital core");        // 8
//     case RTCWDT_SYS_RESET       : return F("RTC Watch dog Reset digital core");                 // 9
//     case INTRUSION_RESET        : return F("Instrusion tested to reset CPU");                   // 10
//     case TGWDT_CPU_RESET        : return F("Time Group reset CPU");                             // 11
//     case SW_CPU_RESET           : return F("Software reset CPU");                               // 12
//     case RTCWDT_CPU_RESET       : return F("RTC Watch dog Reset CPU");                          // 13
//     case EXT_CPU_RESET          : return F("or APP CPU, reseted by PRO CPU");                   // 14
//     case RTCWDT_BROWN_OUT_RESET : return F("Reset when the vdd voltage is not stable");         // 15
//     case RTCWDT_RTC_RESET       : return F("RTC Watch dog reset digital core and rtc module");  // 16
//     default                     : return F("NO_MEAN");                                          // 0
//   }
// }
// #endif

// void esp32_resetreason(RESET_REASON reason)
// {
//   switch ( reason)
//   {
//     case 1  : Serial.println ("Vbat power on reset");break;
//     case 3  : Serial.println ("Software reset digital core");break;
//     case 4  : Serial.println ("Legacy watch dog reset digital core");break;
//     case 5  : Serial.println ("Deep Sleep reset digital core");break;
//     case 6  : Serial.println ("Reset by SLC module, reset digital core");break;
//     case 7  : Serial.println ("Timer Group0 Watch dog reset digital core");break;
//     case 8  : Serial.println ("Timer Group1 Watch dog reset digital core");break;
//     case 9  : Serial.println ("RTC Watch dog Reset digital core");break;
//     case 10 : Serial.println ("Instrusion tested to reset CPU");break;
//     case 11 : Serial.println ("Time Group reset CPU");break;
//     case 12 : Serial.println ("Software reset CPU");break;
//     case 13 : Serial.println ("RTC Watch dog Reset CPU");break;
//     case 14 : Serial.println ("for APP CPU, reseted by PRO CPU");break;
//     case 15 : Serial.println ("Reset when the vdd voltage is not stable");break;
//     case 16 : Serial.println ("RTC Watch dog reset digital core and rtc module");break;
//     default : Serial.println ("NO_MEAN");
//   }
// }

// String ESP_getResetReason(void) {
//   return ESP32GetResetReason(0);  // CPU 0
// }

// uint32_t ESP_ResetInfoReason(void) {
//   RESET_REASON reason = rtc_get_reset_reason(0);
//   if (POWERON_RESET == reason) { return REASON_DEFAULT_RST; }
//   if (SW_CPU_RESET == reason) { return REASON_SOFT_RESTART; }
//   if (DEEPSLEEP_RESET == reason)  { return REASON_DEEP_SLEEP_AWAKE; }
//   if (SW_RESET == reason) { return REASON_EXT_SYS_RST; }
// }

    // esp_wifi_get_mac((wifi_interface_t)interface, mac);
    // sprintf(winstance, "%s [%02x:%02x:%02x:%02x:%02x:%02x]", _hostname.c_str(), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

      // esp_wifi_get_mac((wifi_interface_t)WIFI_IF_STA, eth_mac);
      // snprintf(default_hostname, 32, "%s%02X%02X%02X", CONFIG_IDF_TARGET "-", eth_mac[3], eth_mac[4], eth_mac[5]);


// const char * const WIFI_STA_STATUS[] PROGMEM
// {
//   "WL_IDLE_STATUS",     // 0 STATION_IDLE
//   "WL_NO_SSID_AVAIL",   // 1 STATION_NO_AP_FOUND
//   "WL_SCAN_COMPLETED",  // 2
//   "WL_CONNECTED",       // 3 STATION_GOT_IP
//   "WL_CONNECT_FAILED",  // 4 STATION_CONNECT_FAIL, STATION_WRONG_PASSWORD(NI)
//   "WL_CONNECTION_LOST", // 5
//   "WL_DISCONNECTED",    // 6 
//   "WL_STATION_WRONG_PASSWORD" // 7 KLUDGE 
// };

// #ifdef ESP32
// const char * const AUTH_MODE_NAMES[] PROGMEM
// {
//     "OPEN",
//     "WEP",             
//     "WPA_PSK",         
//     "WPA2_PSK",        
//     "WPA_WPA2_PSK",    
//     "WPA2_ENTERPRISE", 
//     "MAX"
// };
// #elif defined(ESP8266)
// const char * const AUTH_MODE_NAMES[] PROGMEM
// {
//     "",
//     "",
//     "WPA_PSK",      // 2 ENC_TYPE_TKIP
//     "",
//     "WPA2_PSK",     // 4 ENC_TYPE_CCMP
//     "WEP",          // 5 ENC_TYPE_WEP
//     "",
//     "OPEN",         //7 ENC_TYPE_NONE
//     "WPA_WPA2_PSK", // 8 ENC_TYPE_AUTO
// };
// #endif

// const char* const WIFI_MODES[] PROGMEM = { "NULL", "STA", "AP", "STA+AP" };
// typedef enum {
//     NO_MEAN                =  0,
//     POWERON_RESET          =  1,    /**<1, Vbat power on reset*/
//     RTC_SW_SYS_RESET       =  3,    /**<3, Software reset digital core*/
//     DEEPSLEEP_RESET        =  5,    /**<5, Deep Sleep reset digital core*/
//     TG0WDT_SYS_RESET       =  7,    /**<7, Timer Group0 Watch dog reset digital core*/
//     TG1WDT_SYS_RESET       =  8,    /**<8, Timer Group1 Watch dog reset digital core*/
//     RTCWDT_SYS_RESET       =  9,    /**<9, RTC Watch dog Reset digital core*/
//     INTRUSION_RESET        = 10,    /**<10, Instrusion tested to reset CPU*/
//     TG0WDT_CPU_RESET       = 11,    /**<11, Time Group0 reset CPU*/
//     RTC_SW_CPU_RESET       = 12,    /**<12, Software reset CPU*/
//     RTCWDT_CPU_RESET       = 13,    /**<13, RTC Watch dog Reset CPU*/
//     RTCWDT_BROWN_OUT_RESET = 15,    /**<15, Reset when the vdd voltage is not stable*/
//     RTCWDT_RTC_RESET       = 16,    /**<16, RTC Watch dog reset digital core and rtc module*/
//     TG1WDT_CPU_RESET       = 17,    /**<17, Time Group1 reset CPU*/
//     SUPER_WDT_RESET        = 18,    /**<18, super watchdog reset digital core and rtc module*/
//     GLITCH_RTC_RESET       = 19,    /**<19, glitch reset digital core and rtc module*/
// } RESET_REASON;

const char * const RESET_REASON_STR[] PROGMEM
{
    "NO_MEAN"                ,
    "POWERON_RESET"          ,    /**<1, Vbat power on reset*/
    "",
    "RTC_SW_SYS_RESET"       ,    /**<3, Software reset digital core*/
    "",
    "DEEPSLEEP_RESET"        ,    /**<5, Deep Sleep reset digital core*/
    "",
    "TG0WDT_SYS_RESET"       ,    /**<7, Timer Group0 Watch dog reset digital core*/
    "TG1WDT_SYS_RESET"       ,    /**<8, Timer Group1 Watch dog reset digital core*/
    "RTCWDT_SYS_RESET"       ,    /**<9, RTC Watch dog Reset digital core*/
    "INTRUSION_RESET"        ,    /**<10, Instrusion tested to reset CPU*/
    "TG0WDT_CPU_RESET"       ,    /**<11, Time Group0 reset CPU*/
    "RTC_SW_CPU_RESET"       ,    /**<12, Software reset CPU*/
    "RTCWDT_CPU_RESET"       ,    /**<13, RTC Watch dog Reset CPU*/
    "",
    "RTCWDT_BROWN_OUT_RESET" ,    /**<15, Reset when the vdd voltage is not stable*/
    "RTCWDT_RTC_RESET"       ,    /**<16, RTC Watch dog reset digital core and rtc module*/
    "TG1WDT_CPU_RESET"       ,    /**<17, Time Group1 reset CPU*/
    "SUPER_WDT_RESET"        ,    /**<18, super watchdog reset digital core and rtc module*/
    "GLITCH_RTC_RESET"            /**<19, glitch reset digital core and rtc module*/
};

// typedef enum {
//     NO_MEAN                = "NO_MEAN",
//     POWERON_RESET          = "Vbat power on reset",
//     RTC_SW_SYS_RESET       = "Software reset digital core",
//     DEEPSLEEP_RESET        = "Deep Sleep reset digital core",
//     TG0WDT_SYS_RESET       = "Timer Group0 Watch dog reset digital core",
//     TG1WDT_SYS_RESET       = "Timer Group1 Watch dog reset digital core",
//     RTCWDT_SYS_RESET       = "RTC Watch dog Reset digital core",
//     INTRUSION_RESET        = "Instrusion tested to reset CPU",
//     TG0WDT_CPU_RESET       = "Time Group0 reset CPU",
//     RTC_SW_CPU_RESET       = "Software reset CPU",
//     RTCWDT_CPU_RESET       = "RTC Watch dog Reset CPU",
//     RTCWDT_BROWN_OUT_RESET = "Reset when the vdd voltage is not stable",
//     RTCWDT_RTC_RESET       = "RTC Watch dog reset digital core and rtc module",
//     TG1WDT_CPU_RESET       = "Time Group1 reset CPU",
//     SUPER_WDT_RESET        = "super watchdog reset digital core and rtc module",
//     GLITCH_RTC_RESET       = "glitch reset digital core and rtc module"
// } RESET_REASON_STR_v;


// @todo
String getResetReason(uint8_t cpu = 0){
    int reason;
    #ifdef ESP8266
      return ESP.getResetReason();
    #elif defined(ESP32) && defined(_ROM_RTC_H_)
      // requires #include <rom/rtc.h>
      return RESET_REASON_STR[rtc_get_reset_reason(cpu)];
      // return "NA";
      #else 
      return "UNSET";
    #endif
}

void processWiFi(){
  #ifdef ESP8266
    MDNS.update();
  #endif
}

#endif 