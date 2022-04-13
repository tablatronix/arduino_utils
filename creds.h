#ifndef creds_h
#define creds_h

String apikeya     = "741F4992029440228F7B6B1F270728D4";
const char* SSID_B   = "wifib";
const char* PASS_B   = "password1234";

const char* SSID = "wifia";
const char* PASS = "password1234";

const char* STASSID = SSID;
const char* STAPSK  = PASS;

const char* ssid     = SSID;          // your network SSID (name)
const char* password = PASS;

// mqtt
const char* mqtt_server_host = "mqtt.local";
const char* mqtt_server_ip   = "192.168.100.1";
const int mqtt_server_port =  1883;

// syslog
const char* syslog_server_host = mqtt_server_host;
const char* syslog_server_ip   = mqtt_server_ip;
const int   syslog_server_port = 514;

#endif
