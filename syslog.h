#ifndef syslog_h
#define syslog_h

#include <WiFiUdp.h>
#include <Syslog.h> // https://github.com/arcao/Syslog

#include <creds.h>

// Syslog server connection info
#define SYSLOG_SERVER "syslog-server"
#define SYSLOG_PORT 514

// This device info
#define DEVICE_HOSTNAME "my-device"
#define APP_NAME "my-app"

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udpClient;

// Create a new syslog instance with LOG_KERN facility
Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, DEVICE_HOSTNAME, APP_NAME, LOG_KERN);
int iteration = 1;

void init_syslog(){
  Serial.println("[SYSLOG] init");
  // Syslog new syslog(udpClient);

  syslog.server(SYSLOG_SERVER, SYSLOG_PORT);
  syslog.deviceHostname(DEVICE_HOSTNAME);
  syslog.appName(APP_NAME);
  syslog.defaultPriority(LOG_KERN);  
}

void sendLogTest(){
  syslog.log(LOG_INFO, "Begin Send");

  // Log message can be formated like with printf function.
  syslog.logf(LOG_ERR,  "This is error message no. %d", iteration);
  syslog.logf(LOG_INFO, "This is info message no. %d", iteration);

  // You can force set facility in pri parameter for this log message. More 
  // facilities in syslog.h or in Linux syslog documentation.
  syslog.logf(LOG_DAEMON | LOG_INFO, "This is daemon info message no. %d", 
    iteration);

  // You can set default facility and severity level and use it for all log
  // messages (beware defaultPriority is stored permanently!)
  syslog.defaultPriority(LOG_FTP | LOG_INFO);
  syslog.logf("This is ftp info message no. %d", iteration);

  // Send log messages up to LOG_WARNING severity
  syslog.logMask(LOG_UPTO(LOG_WARNING));
  syslog.log(LOG_ERR, "This is logged.");
  syslog.log(LOG_WARNING, "This is logged.");
  syslog.log(LOG_NOTICE, "This is not logged.");
  syslog.log(LOG_INFO, "This is not logged.");

  // F() macro is supported too
  syslog.log(LOG_INFO, F("End loop"));
  iteration++;
}

#endif