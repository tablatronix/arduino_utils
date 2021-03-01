#ifndef wifi_funcs_h
#define wifi_funcs_h

#include <creds.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #define WIFI_getChipId() ESP.getChipId()
#elif defined(ESP32)
    #include <WiFi.h>
    #include <esp_wifi.h>
    #include <ESPmDNS.h>
    #define WIFI_getChipId() (uint32_t)ESP.getEfuseMac()
#endif

// const char* hostname   = "esp8266REFLOW";

bool debug_wifi = false;

long downtimeRestart = 300000;
long downtime        = 0;


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

String getDeviecID(){
  String _wifissidprefix = "ESP";
  String hostString = String(WIFI_getChipId(),HEX);
  hostString.toUpperCase();
  // char hostString[16] = {0};
  // sprintf(hostString, "%06X", ESP.getChipId());  
  return _wifissidprefix + "_" + hostString;
}

void setWiFiHostname(const char* hostname){
  #ifdef ESP32
  WiFi.setHostname(hostname);
  #else
  WiFi.hostname(hostname);
  #endif
}

String getHostname(){
  #ifdef ESP32
  return WiFi.getHostname(); // getHostName ( @todo return string of c.str?)
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
      getHostname();
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
    WiFi.mode(WIFI_STA);
    #ifdef ESP8266
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    #elif defined(ESP32)
    // btStop();
    WiFi.setSleep(false);
    #endif

    // WiFi.hostname(hostname);
    unsigned long start = millis();
    if(wifiIsAutoConnect)  WiFi.begin();
    else WiFi.begin(SSID,PASS);
    if(timeout > 0){
      Serial.println("[WIFI] Connecting to wifi... [" + (String)timeout + " ms]\n");
      while((WiFi.status() != WL_CONNECTED) && (millis()-start < timeout)){
        Serial.print(".");
        delay(100);
      }
    }
    else {
      Serial.println("[WIFI] Connecting to wifi, waiting..... ");
      while(WiFi.waitForConnectResult() != WL_CONNECTED){
        // Serial.print(".");
        delay(100);
      }  
    }

    Serial.println("");

    if(wifiIsConnected()){
      WiFi_print_sta();
    }
    else{
      Serial.println("[ERROR] WIFI CONNECT FAILED");
      Serial.println("[WIFI] waited for " + (String)(millis()-start/1000) + " seconds");
    }
    delay(500);
}

void init_wifi(){
  init_WiFi(0);
}

int getRSSIasQuality() {
  int RSSI = WiFi.RSSI();
  int quality = 0;

  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

void checkWifi(){
  if(WiFi.status() != WL_CONNECTED  ){
    if(downtime == 0) downtime = millis();
    if(millis() > downtime + downtimeRestart) ESP.restart();
    #ifdef USENEOIND
      indSetColor(255,0,0);
    #endif
    Serial.println("[WIFI] WiFi is Disconnected");
    WiFi.reconnect();
  } else {
    if(debug_wifi){
      Serial.println("[WIFI] WiFi is CONNECTED");
      Serial.println("[WIFI] RSSI: "+(String)getRSSIasQuality());
    }  
    #ifdef USENEOIND
      indSetColor(0,255,0);
    #endif
  }
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

//   switch ( reason)
//   {
//     case 1 : Serial.println ("POWERON_RESET");break;          /**<1,  Vbat power on reset*/
//     case 3 : Serial.println ("SW_RESET");break;               /**<3,  Software reset digital core*/
//     case 4 : Serial.println ("OWDT_RESET");break;             /**<4,  Legacy watch dog reset digital core*/
//     case 5 : Serial.println ("DEEPSLEEP_RESET");break;        /**<5,  Deep Sleep reset digital core*/
//     case 6 : Serial.println ("SDIO_RESET");break;             /**<6,  Reset by SLC module, reset digital core*/
//     case 7 : Serial.println ("TG0WDT_SYS_RESET");break;       /**<7,  Timer Group0 Watch dog reset digital core*/
//     case 8 : Serial.println ("TG1WDT_SYS_RESET");break;       /**<8,  Timer Group1 Watch dog reset digital core*/
//     case 9 : Serial.println ("RTCWDT_SYS_RESET");break;       /**<9,  RTC Watch dog Reset digital core*/
//     case 10 : Serial.println ("INTRUSION_RESET");break;       /**<10, Instrusion tested to reset CPU*/
//     case 11 : Serial.println ("TGWDT_CPU_RESET");break;       /**<11, Time Group reset CPU*/
//     case 12 : Serial.println ("SW_CPU_RESET");break;          /**<12, Software reset CPU*/
//     case 13 : Serial.println ("RTCWDT_CPU_RESET");break;      /**<13, RTC Watch dog Reset CPU*/
//     case 14 : Serial.println ("EXT_CPU_RESET");break;         /**<14, for APP CPU, reseted by PRO CPU*/
//     case 15 : Serial.println ("RTCWDT_BROWN_OUT_RESET");break;*<15, Reset when the vdd voltage is not stable
//     case 16 : Serial.println ("RTCWDT_RTC_RESET");break;      /**<16, RTC Watch dog reset digital core and rtc module*/
//     default : Serial.println ("NO_MEAN");
//   }
// }

// void verbose_print_reset_reason(RESET_REASON reason)
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

String getResetReason(){
    int reason;
    #ifdef ESP8266
      return ESP.getResetReason();
    #elif defined(ESP32) && defined(_ROM_RTC_H_)
      // requires #include <rom/rtc.h>
      p = FPSTR(HTTP_INFO_lastreset);
      for(int i=0;i<2;i++){
        int reason = rtc_get_reset_reason(i);
        return (String)reason;
        // switch (reason)
        // {
        // }
      }
      #else 
      return "";
    #endif
}


#endif 