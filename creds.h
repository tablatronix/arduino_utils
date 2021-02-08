#ifndef creds_h
#define creds_h

String apikeya     = "";
const char* SSID_B   = "";
const char* PASS_B   = "";

const char* SSID = "";
const char* PASS = "";

const char* STASSID = SSID;
const char* STAPSK  = PASS;

const char* ssid     = SSID;          // your network SSID (name)
const char* password = PASS;

// mqtt
const char* mqtt_server_host = "";
const char* mqtt_server_ip   = "";
const int mqtt_server_port =  1883;

// syslog
const char* syslog_server_host = mqtt_server_host;
const char* syslog_server_ip   = mqtt_server_ip;
const int   syslog_server_port = 514;

#endif