
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
#elif defined(ESP32)
    #include <WiFi.h>
    #include <esp_wifi.h>  
#endif

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* otahost		= "esp8266OTA";
bool otastarted = false;

void handleOTA(){
  // MDNS.update(); // handled internally
  ArduinoOTA.handle();
}

void startOTA(){
  // ArduinoOTA.setHostname(otahost);
  ArduinoOTA.onStart([]() {
       otastarted = true;
        Serial.println("\n[OTA] onStart");
  });

  ArduinoOTA.onEnd([]() {
        otastarted = false;
        Serial.println("\n[OTA] onEnd");
 		// PAUSED = false;       
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("[OTA] Progress: %u%%\n", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("[OTA] Error[%u]: ", error);
    otastarted = false;
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

   /* setup the OTA server */
   ArduinoOTA.begin();  
}

void init_ota(){
	startOTA();
}