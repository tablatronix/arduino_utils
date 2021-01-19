#ifndef mqtt_h
#define mqtt_h

#include <PubSubClient.h>

WiFiClient espClient;

PubSubClient client(espClient);

String clientID = "";

void MQTTreconnect() {

  // Loop until we're reconnected
  // this is blocking
  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");

    // Create a random client ID

    String clientId = "ESP8266Client-";

    clientId += String(random(0xffff), HEX);

    // Attempt to connect

    if (client.connect(clientId.c_str())) {

      Serial.println("connected");

      // Once connected, publish an announcement...

      client.publish("TESTOUT", "hello world");

      // ... and resubscribe

      client.subscribe("TESTIN");

    } else {

      Serial.print("failed, rc=");

      Serial.print(client.state());

      Serial.println(" try again in 5 seconds");

      // Wait 5 seconds before retrying

      // delay(5000);

    }

    delay(100);

  }

}



void MQTTcallback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");

  Serial.print(topic);

  Serial.print("] ");

  for (int i = 0; i < length; i++) {

    Serial.print((char)payload[i]);

  }

  Serial.println();



  // Switch on the LED if an 1 was received as first character

  if ((char)payload[0] == '1') {

    // digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level

    // but actually the LED is on; this is because

    // it is active low on the ESP-01)

  } 

  // else {

  //   // digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH

  // }



}

long lastReconnectAttempt = 0;
uint16_t mqttretry = 5000;

boolean mqttReconnect() {
  WiFi.reconnect();
  return WiFi.status() == 'WL_CONNECTED';
}

void mqtt_checkconn(){
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > mqttretry) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqttReconnect()) {
        lastReconnectAttempt = 0;
        client.connect(clientID.c_str());
      }
    }
  } else {
    // Client connected
    client.loop();
  }  
}

void process_MQTT_nb(){
  if (!client.connected()) {
    mqtt_checkconn();
  }
  client.loop(); // will wait loop reconnect to mqtt
}

void process_MQTT(){
  MQTTreconnect();
  client.loop(); // will wait loop reconnect to mqtt
}

void init_MQTT(String clientid){
  clientID = clientid;
  client.setServer(mqtt_server_ip, 1883);
  client.setCallback(MQTTcallback);
  process_MQTT();
}

void init_MQTT(){
  client.setServer(mqtt_server_ip, 1883);
  client.setCallback(MQTTcallback);
  process_MQTT(); 
}


void MQTT_pub(String topic, String msg){
    Serial.print("[MQTT] Publish message: ");
    Serial.print("topic: ");
    Serial.print(topic);
    Serial.print(" mesg: ");
    Serial.println(msg);
    client.publish(topic.c_str(), msg.c_str());
}

void MQTT_pub(String topic, String sensor, String value){
    Serial.print("[MQTT] Publish message: ");
    Serial.print("topic: ");
    Serial.print(topic+"/"+clientID+"/"+sensor);
    Serial.print("\t\tvalue: ");
    Serial.println(value);
    if(value == "") {
      Serial.println("[ERROR] MQTT value is empty");
      return;
    }
    client.publish((topic+"/"+clientID+"/"+sensor).c_str(), value.c_str());
}

#endif