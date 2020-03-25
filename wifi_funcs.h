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
    Serial.println("Connecting to wifi... [" + (String)timeout + " ms]");
    while(WiFi.status() != WL_CONNECTED && (millis()-start < timeout)){
      Serial.print(".");
      delay(100);
    }

    if(WiFi.status() == WL_CONNECTED){
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("HOST: ");
      Serial.println(WiFi.hostname());
    }
    else{
      Serial.println("NOT CONNECTED");
    }
    delay(500);
}