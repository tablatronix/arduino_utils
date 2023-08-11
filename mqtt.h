#ifndef mqtt_h
#define mqtt_h

#undef nullptr

#define USEJSON
#ifdef USEJSON
#include <ArduinoJson.h> // bblanchon/ArduinoJson

// #define _JSONSIZE 4096

#ifndef _JSONSIZE
#define _JSONSIZE 1024
#endif

StaticJsonDocument<_JSONSIZE> payload;
StaticJsonDocument<_JSONSIZE> rootdoc;
// payload["sensor"] = "gps";
// payload["time"] = 1351824120;

  // const size_t CAPACITY = JSON_ARRAY_SIZE(1);
  // StaticJsonDocument<CAPACITY> docH;
  // JsonArray arrayH = docH.to<JsonArray>();
  DynamicJsonDocument pubjson(_JSONSIZE);
  JsonArray jsondata = pubjson.to<JsonArray>();
#endif

#include <PubSubClient.h> // knolleary/pubsubclient
#define MQTT_MAX_PACKET_SIZE 2048

WiFiClient espClient;

PubSubClient client(espClient);

bool debug_mqtt      = true;
bool debug_mqtt_json = false;
const char* clientID = "";

bool mqttconnected = false;

long lastReconnectAttempt = 0;
uint32_t mqttretry = 5000;

void MQTTGetErrorMessage(){
// Possible values for client.state()
// #define MQTT_CONNECTION_TIMEOUT     -4
// #define MQTT_CONNECTION_LOST        -3
// #define MQTT_CONNECT_FAILED         -2
// #define MQTT_DISCONNECTED           -1
// #define MQTT_CONNECTED               0
// #define MQTT_CONNECT_BAD_PROTOCOL    1
// #define MQTT_CONNECT_BAD_CLIENT_ID   2
// #define MQTT_CONNECT_UNAVAILABLE     3
// #define MQTT_CONNECT_BAD_CREDENTIALS 4
// #define MQTT_CONNECT_UNAUTHORIZED    5
}

void MQTTreconnect() {
  if (!client.connected()) {
  // while (!client.connected()) {
    if(debug_mqtt) Logger.println("[MQTT] Connecting...");
    // Attempt to connect
    if (client.connect(clientID)) {
      mqttconnected = true;
      Logger.println("[MQTT] Connected");
      // Once connected, publish an announcement...
      client.publish("TESTOUT", "hello world");
      // ... and resubscribe
      client.subscribe("ESP_env_c/CMD");
    } else {
      Logger.print("[ERROR] [MQTT] failed, rc="); // @todo we get here but no actual reconnnect
      Logger.println(client.state());
    }
  }
}

void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  Logger.print("[MQTT] IN Message arrived [");
  Logger.print(topic);
  Logger.print("] ");
  Logger.print("payload: ");
  for (int i = 0; i < length; i++) {
    Logger.print((char)payload[i]);
  }
  Logger.println("");
  // RESTART
  if ((char)payload[0] == '1') {
    Logger.println("[MQTT] payload: 1 TRIGGERED");
    ESP.restart();
  }
  // WIFI off
  if ((char)payload[0] == '2') {
    Logger.println("[MQTT] payload: 2 TRIGGERED");
    WiFi.disconnect();
  }  
}

boolean mqttWiFiReconnect() {
  WiFi.reconnect();
  return WiFi.status() == 'WL_CONNECTED';
}

void mqtt_checkconn(){
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > mqttretry) {
      Logger.println("[MQTT] try client re-connect");      
      lastReconnectAttempt = now;
      MQTTreconnect();
    }
  } else {
    // Client connected
    client.loop();
  }
}

bool process_MQTT_nb(){
  if (!client.connected()) {
    mqttconnected = false;
    mqtt_checkconn();
  }
  client.loop(); // will wait loop reconnect to mqtt
  return client.connected();
}

bool process_MQTT(){
  if(!wifiIsConnected()){
    // Logger.println("[MQTT] wifi not connected");
    return false;
  }
  if(!client.connected()){
    mqttconnected = false;
    Logger.print("[MQTT] client not connected => ");
    Logger.println(client.state());
    MQTTreconnect(); // @todo throttle
    return false;
  }
  client.loop(); // will wait loop reconnect to mqtt
  return true;
}

void init_MQTT(){
  if(!wifiIsConnected()) return;
  if(clientID == ""){
    Logger.println("[MQTT] clientID not set");
    // clientID = getHostname();
  }
  client.setServer(mqtt_server_ip, mqtt_server_port);

  // MQTT_SOCKET_TIMEOUT: socket timeout interval in Seconds. Override with setSocketTimeout()
  client.setSocketTimeout(30); // #define MQTT_SOCKET_TIMEOUT 15
  // MQTT_KEEPALIVE : keepAlive interval in Seconds. Override with setKeepAlive()
  client.setKeepAlive(10);     // #define MQTT_KEEPALIVE 15

  client.setCallback(MQTTcallback);
  if (client.connect(clientID)){
    mqttconnected = true;
    Logger.println("[MQTT] connected to " + (String)mqtt_server_ip + " as " + clientID);
  }
  else Logger.println("[MQTT] init failed to connect to " + (String)mqtt_server_ip);
  client.setBufferSize(_JSONSIZE);
  // client.subscribe("CMD");
  // client.subscribe("ESP_env_c/CMD");
  process_MQTT();
  // jsondata = pubjson.to<JsonArray>();
  // jsondata = pubjson.createNestedArray();
}

void init_MQTT(const char* clientid){
  clientID = clientid; // global
  init_MQTT();
}

void init_MQTT(const String& clientid){
  clientID = clientid.c_str(); // global
  init_MQTT();
}

#ifdef USEJSON

void MQTT_pub(String topic, String sensor, String value){
    if(!wifiIsConnected()){
      // Logger.print("[MQTT] OFFLINE: ");
      // return;
    }
    if(!mqttconnected)return;
    if(debug_mqtt){
      Logger.print("[MQTT] Publish: ");
      Logger.print(sensor);
      Logger.print(" ");
      Logger.println(value);
    }
    if(value == "") {
        // Logger.println("[ERROR] MQTT value is empty");
        return;
    }
    // JsonArray data = payload.createNestedArray(topic);
    payload["topic"] = topic;
    payload["clientid"] = clientID;
    payload["type"] = sensor;
    payload["value"] = value.toFloat();
    // payload["unit"] = "";
    // if(topic == "device") 
    jsondata.add(payload);

    // serializeJson(jsondata, Serial);
    // if(debug_mqtt) serializeJson(payload, Serial);
    // serializeJsonPretty(payload, Serial);
    
    // String output;
    // serializeJson(payload,output);
    // client.publish(topic.c_str(),("["+output+"\n]").c_str()); // must be object?
    // close ?
}

void MQTT_pub(String topic, String sensor, String value, bool json){
    if(!mqttconnected)return;
    if(debug_mqtt){
      Logger.print("[MQTT] Publish: ");
      Logger.print(sensor);
      Logger.print(" ");
      Logger.println(value);
    }
    if(value == "") {
      // Logger.println("[ERROR] MQTT value is empty");
      return;
    }
    JsonArray data = payload.createNestedArray(topic);
    payload["topic"] = data; // tag key = tag value
    payload["clientid"] = clientID;
    payload["type"] = sensor; // field key = field value
    payload["value"] = value.toFloat();
    // payload["unit"] = "";
    // if(debug_mqtt_json) serializeJson(payload, Logger);
    // serializeJsonPretty(payload, Serial);
    
    String output;
    serializeJson(payload,output);
    
    client.publish(topic.c_str(),("["+output+"\n]").c_str()); // must be object?
    // close ?
    // mqttClient.beginMessage(topic);
    // serializeJson(doc, mqttClient);
    // mqttClient.endMessage();
    // client.print
    // client.endPublish();
    //     client.beginPublish("greenBottles/lyrics", msgLen, false);

    // const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3);
    // DynamicJsonBuffer jsonBuffer(capacity);

    // JsonObject& root = jsonBuffer.createObject();
    // root["sensor"] = "gps";
    // root["time"] = 1351824120;

    // JsonArray& data = root.createNestedArray("data");
    // data.add(48.75608);
    // data.add(2.302038);

    // root.printTo(Serial);
}

void MQTT_pub_send(String topic){
  if(!mqttconnected)return;
  if(debug_mqtt){
    Logger.println("[MQTT] sending json for topic: " + topic);
  }
  // serializeJson(jsondata, Logger);
  // rootdoc.createNestedObject();
  // rootdoc.add(pubjson);
  // String output;
  // serializeJson(pubjson,output);
  // Serial.println(output);
  // serializeJson(rootdoc,output);
  // Serial.println(output);
  // Serial.flush();

  char message[_JSONSIZE];
  serializeJson(pubjson, message);
  if(debug_mqtt_json){  
    Logger.println((String)message);
    Logger.flush();
  }
  client.publish(topic.c_str(),message);
  delay(500);
  // pubjson.clear();
  rootdoc.clear();
  // jsondata.clear();
  // rootdoc.createNestedObject(jsondata);
  jsondata = pubjson.to<JsonArray>();
  // payload.clear();
  // pubjson.garbageCollect();
}

#else
void MQTT_pub(String topic, String msg){
    if(!mqttconnected)return;
    Logger.print("[MQTT] Publish message: ");
    Logger.print("topic: ");
    Logger.print(topic);
    Logger.print(" mesg: ");
    Logger.println(msg);
    client.publish(topic.c_str(), msg.c_str());
}

void MQTT_pub(String topic, String sensor, String value){
    if(!mqttconnected)return;
    Logger.print("[MQTT] Publish message: ");
    Logger.print("topic: ");
    Logger.print(topic+"/"+clientID+"/"+sensor);
    Logger.print("\t\tvalue: ");
    Logger.println(value);
    if(value == "") {
      Logger.println("[ERROR] MQTT value is empty");
      return;
    }
    client.publish((topic+"/"+clientID+"/"+sensor).c_str(), value.c_str());
}
#endif


#ifdef ESP8266
    #define ESP_getChipId() ESP.getChipId() 
#elif defined(ESP32)    
    #define ESP_getChipId() (uint32_t)ESP.getEfuseMac()
#endif

void MQTT_pub_device(bool verbose){
  if(!mqttconnected)return;
  if(debug_mqtt) Logger.println("[MQTT] Publish Device");
  MQTT_pub("device","uptime_s",(String)(millis()/1000));
  MQTT_pub("device","rssi",(String)getRSSIasQuality());
  MQTT_pub("device","heap",(String)ESP.getFreeHeap());
  if(verbose){
    // MQTT_pub("device","ip",(String)ESP.getFreeHeap());
    // MQTT_pub("device","hostname",(String)ESP.getFreeHeap());
    MQTT_pub("device","ChipType",(String)ESP.getChipModel());
    MQTT_pub("device","chipID",(String)ESP_getChipId());
    MQTT_pub("device","ESPver",(String)ESP.getSdkVersion());
    MQTT_pub("device","chipCores",(String)ESP.getChipCores());
    MQTT_pub("device","chipRev",(String)ESP.getChipRevision());
  }
  // MQTT_pub("device","hall",(String)hallRead());
  #ifdef ESP32
  // MQTT_pub("device","temp",(String)temperatureRead());
  #endif
  MQTT_pub_send("device");
}

void MQTT_pub_device(){
  MQTT_pub_device(false);
}

#endif