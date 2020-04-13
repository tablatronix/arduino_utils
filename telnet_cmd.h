#ifndef telnet_cmd_h
#define telnet_cmd_h

#include <ESP8266WiFi.h>
#include <serialcmd.h>
#include <motor.h>

bool DEBUG_telnet_cmd = true;

int telnetport = 23;

// ansi stuff, could always use printf instead of concat
String ansiPRE  = "\033"; // escape code
String ansiHOME = "\033[H"; // cursor home
String ansiESC  = "\033[2J"; // esc
String ansiCLC  = "\033[?25l"; // invisible cursor
String ansiCLS  = "\033c\e[3J";

String ansiEND  = "\033[0m";   // closing tag for styles
String ansiBOLD = "\033[1m";

String ansiEL   = "\033[K";     // erase line

String ansiRED  = "\033[41m"; // red background
String ansiGRN  = "\033[42m"; // green background
String ansiBLU  = "\033[44m"; // blue background

String ansiREDF = "\033[31m"; // red foreground
String ansiGRNF = "\033[34m"; // green foreground
String ansiBLUF = "\033[32m"; // blue foreground
String BELL     = "\a";

// declare telnet server (do NOT put in setup())
WiFiServer TelnetServer(telnetport);
WiFiClient Telnet;

void println(String msg){
    Serial.println(msg);
    Telnet.println(msg);
}

void init_telnet(){
  TelnetServer.begin(); 
  TelnetServer.setNoDelay(true);
  Serial.println("[TELNET] Starting telnet server on port " + (String)telnetport);   	
}

void telnet_status(){
  // control panel sample
  // Telnet.print(ansiCLS); // clear screen buffer
  // Telnet.print(ansiCLC); // hide cursor  
  // Telnet.println("\n\n"); // 2 empty lines for loop

  // Telnet.print(ansiHOME);
  
  Telnet.println("uptime: "+ansiRED + ansiBOLD + " " + (String)millis() + " " + ansiEND + ansiEND+" ms");
  Telnet.print(ansiGRNF + ansiBOLD);
  // do RSSI display
  int32_t rssi = WiFi.RSSI();
  if(rssi < -96) Telnet.println("No Signal");
  else{
    int count = (int)map(rssi,-1,-96,60,1);
    for(int i=0;i<80;i++){
        if(i<count) Telnet.print("|");
        // else Telnet.print(" ");
    }
    Telnet.print(ansiEND+ansiEND);
    Telnet.print(" ");
    Telnet.print((String)rssi + " dB");
  }
  Telnet.print(ansiEL);

  delay(0); // to fast might crash terminals	
}

// new client connected
void handleClientConnect(){
	telnet_status();
}

void handleTelnet(){
  // only allows 1 client, use serverClients[MAX_SRV_CLIENTS] and loop them for more
  if (TelnetServer.hasClient()){
    // client is connected
    if (!Telnet || !Telnet.connected()){
      if(Telnet){
        Telnet.stop();          // client disconnected
        if(DEBUG_telnet_cmd)Serial.println("[TELNET] Client has disconnected");
      } else {
        if(DEBUG_telnet_cmd)Serial.println("[TELNET] Client disappeared");
      }
      Telnet = TelnetServer.available(); // ready for new client
    } else {
      TelnetServer.available().stop();  // have client, block new conections
    }
    if(DEBUG_telnet_cmd)Serial.println("[TELNET] Client Connected");
    Telnet.flush();  // clear input buffer, else you get strange characters         
    handleClientConnect();
  }

  if (Telnet && Telnet.connected() && Telnet.available()){
    // client input processing
    while(Telnet.available()){
        // Serial.println(Serial.peek());
        readSerial(Telnet);
         // inputString = Serial.readStringUntil('\n');
         // parseSerial();
         delay(0);
      }
      // if(echo)
      // Serial.write(Telnet.read()); // pass through
      // do other stuff with client input here
  }
}

// void doTelnet(){
//   if (TelnetServer.hasClient()){
//   	// client is connected
//     if (!Telnet || !Telnet.connected()){
//       if(Telnet) Telnet.stop();          // client disconnected
//       Telnet = TelnetServer.available(); // ready for new client
//     } else {
//       TelnetServer.available().stop();  // have client, block new conections
//     }
//   }

//   // BUG Telnet.available() crashes wifi on 2.0.4 ????

//   if (Telnet && Telnet.connected() && Telnet.available()){
//     // client input processing
//     while(Telnet.available())
//       Serial.write(Telnet.read()); // pass through
//       // do other stuff with client input here
//   }
// }

void process_telnet() {
  handleTelnet();
}

#endif