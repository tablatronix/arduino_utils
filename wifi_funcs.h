#ifndef wifi_funcs_h
#define wifi_funcs_h

#include <creds.h>

const char* hostname   = "esp8266REFLOW";

void init_WiFi(int timeout){
    WiFi.mode(WIFI_STA);
    #ifdef ESP8266
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    #elif defined(ESP32)
    // btStop();
    WiFi.setSleep(false);
    #endif

    WiFi.hostname(hostname);
    unsigned long start = millis();
    WiFi.begin(SSID,PASS);
    Serial.println("[WIFI] Connecting to wifi... [" + (String)timeout + " ms]\n");
    while(WiFi.status() != WL_CONNECTED && (millis()-start < timeout)){
      Serial.print(".");
      delay(100);
    }

    Serial.println("");

    if(WiFi.status() == WL_CONNECTED){
      Serial.println("[WIFI] CONNECTED");
      Serial.print("[WIFI] IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("[WIFI] HOST: ");
      Serial.println(WiFi.hostname());
    }
    else{
      Serial.println("[ERROR] WIFI CONNECT FAILED");
    }
    delay(500);
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
    #ifdef USENEOIND
      indSetColor(255,0,0);
    #endif
    Serial.println("[WIFI] WiFi Disconnected");
    WiFi.reconnect();
  } else {
    Serial.println("[WIFI] RSSI: "+(String)getRSSIasQuality());
    #ifdef USENEOIND
      indSetColor(0,255,0);
    #endif
  }
}

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
        // switch (reason)`
        // {
        // }
      }
    #endif
}

#endif