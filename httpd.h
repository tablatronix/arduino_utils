#include "wifi_funcs.h"
#include <WiFiClient.h>

#ifdef ESP8266
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
#elif defined ESP32
#include <WebServer.h>
WebServer server(80);
#endif


void handleRoot() {


  if(server.hasArg(F("ledindex"))){
    Serial.println("has arg ledindex");
    String pixel = server.arg(F("ledindex"));
    // ledIndex = pixel;
    // setHTTPValue(pixel.toInt());
    // strip.setPixelColor(pixel.toInt(),strip.Color(255,0,0));
    Serial.println("Setting pixel:" + (String)pixel);
    // strip.show();
    // delay(2000);
    server.send(200, "text/plain", "hello from esp8266! ledindex SET");    
  }

  if(server.hasArg(F("pwm"))){
    Serial.println("has arg PWM");
    String pwmvalue = server.arg(F("pwm"));
    // ledIndex = pixel;
    // setPWM(pwmvalue.toInt());
    // strip.setPixelColor(pixel.toInt(),strip.Color(255,0,0));
    Serial.println("Setting PWM:" + (String)pwmvalue);
    // strip.show();
    // delay(2000);
    // server.send(200, "text/plain", "hello from esp8266! PWM set:" + (String)getPWM()); 
    // analogWrite(16,pwmvalue.toInt());
    setHTTPValue(pwmvalue);
  }

  if(server.hasArg(F("freq"))){
    Serial.println("has arg FREQ");
    String pwmvalue = server.arg(F("freq"));
    // ledIndex = pixel;
    // setPWM(pwmvalue.toInt());
    // strip.setPixelColor(pixel.toInt(),strip.Color(255,0,0));
    Serial.println("Setting FREQ:" + (String)pwmvalue);
    // strip.show();
    // delay(2000);
    // server.send(200, "text/plain", "hello from esp8266! PWM set:" + (String)getPWM()); 
    // analogWrite(16,pwmvalue.toInt());
    setHTTPValueB(pwmvalue);
  }

  // digitalWrite(LED_BUILTIN, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  // digitalWrite(LED_BUILTIN, 0);  
}

void handleNotFound() {
  // digitalWrite(LED_BUILTIN, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  // digitalWrite(LED_BUILTIN, 0);
}

void httpd_init(){
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

	server.on("/", HTTP_OPTIONS, []() {
	    server.sendHeader("Access-Control-Max-Age", "10000");
	    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
	    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
	    server.send(200, "text/plain", "" );
	  });

	server.on("/", HTTP_GET, []() {
	    String response ;
	    // ... some code to prepare the response data...

	    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
	    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
	    server.send(200, "text/plain", response.c_str() );
	  });

  server.begin();
  Serial.println("HTTP server started");
}

void httpd_process(void) {
  server.handleClient();
  #ifdef ESP8266
  MDNS.update();
  #endif
}
