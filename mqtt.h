#ifndef mqtt_h
#define mqtt_h

#include <PubSubClient.h>

WiFiClient espClient;

PubSubClient client(espClient);


void MQTTreconnect() {

  // Loop until we're reconnected

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



void process_MQTT(){

  if (!client.connected()) {

    MQTTreconnect();

  }

  client.loop();  

}



void init_MQTT(){

  client.setServer(mqtt_server_ip, 1883);

  client.setCallback(MQTTcallback);

  process_MQTT(); 

}



void MQTT_pub(String topic, String msg){

    Serial.print("Publish message: ");

    Serial.print("topic: ");

    Serial.print(topic);

    Serial.print(" mesg: ");

    Serial.println(msg);

    client.publish(topic.c_str(), msg.c_str());

}


#endif