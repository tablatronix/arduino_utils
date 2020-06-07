#ifndef MY_OTA_H
#define MY_OTA_H

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
#include <wifi_funcs.h>

const char* otahost		= "esp8266OTA"; // esp8266-[ChipID]
bool otastarted = false;
int lastprogress = 0; // micros timestamp
int progressthrottle = 0;// progress serial throttle, should make ota faster

void handleOTA(){
  // MDNS.update(); // handled by ota, not required
  ArduinoOTA.handle();
}

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as `nm
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  // ArduinoOTA.setRebootOnSuccess(bool reboot);

void startOTA(){
  ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
          type = "sketch";
        } else { // U_FS
          type = "filesystem";
        }    
       otastarted = true;
      Serial.println("\n[OTA] onStart " + type);
  });

  ArduinoOTA.onEnd([]() {
        otastarted = false;
        Serial.println("\n[OTA] onEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    if (progressthrottle>0 && (micros () - lastprogress > progressthrottle || progress <= 1 || progress >=99)){
      Serial.printf("[OTA] Progress: %u%%\n", (progress / (total / 100)));
    }
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("[OTA] Error[%u]: ", error);
    // @todo set ota fail flag, keep otastarted flag on for a bit so retries succeed
    otastarted = false;
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

   /* setup the OTA server */
   ArduinoOTA.begin();
   Serial.println("[OTA] Started");
   Serial.println("[OTA] Hostname: " + (String)ArduinoOTA.getHostname());
}

void init_ota(){
  // ArduinoOTA.setHostname(otahost);  
	startOTA();
}

// @todo add singleton, allow to be used in loop return status to main code
void OTAReboot(bool always = false,int timeout = 10000){
  // if code is crashing, add delay for ota for recovery emergencies
  if(getResetReason() == "Exception" || always){ // @todo add reset reason masks
    // ArduinoOTA.begin();
    init_ota();
    int timeout = 10000; // how long will esptool wait ?
    int start = millis();
    Serial.print("[OTA] Waiting for OTA (");
    Serial.println((String)timeout + "s) ..... ");
    while((millis()-start < timeout)){
      // ArduinoOTA.handle();
      handleOTA();
      delay(100);
      // Serial.println(".");
    }
  }
}

#endif