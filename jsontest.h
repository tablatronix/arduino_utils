#include <ArduinoJson.h>

StaticJsonDocument<1024> payload;
StaticJsonDocument<1024> rootdoc;
DynamicJsonDocument pubjson(1024);
JsonArray jsondata = pubjson.to<JsonArray>();

void setup(){
	Serial.begin(115200);	
}

void add(){
    payload["topic"] = "thetopic";
    payload["clientid"] = "theclientid";
    payload["type"] = "thetype";
    payload["value"] = "thevalue";
    jsondata.add(payload);
}

void prepare(){
  rootdoc.add(pubjson);

  char message[1024];
  serializeJson(rootdoc, message);
  Serial.println((String)message);
  // client.publish(topic,message); // do stuff
  
  // serial output, arrlen is 3 last element is always null
	// [{"topic":"thetopic","clientid":"theclientid","type":"thetype","value":"thevalue"},
	// {"topic":"thetopic","clientid":"theclientid","type":"thetype","value":"thevalue"},null]

  // clean 
  rootdoc.clear();
  jsondata = pubjson.to<JsonArray>();
}

void test(){
	StaticJsonDocument<256> doc;

	JsonObject doc_0 = doc.createNestedObject();
	doc_0["topic"] = "device";
	doc_0["clientid"] = "ESP_ENV";
	doc_0["type"] = "uptime_s";
	doc_0["value"] = 25;

	JsonObject doc_1 = doc.createNestedObject();
	doc_1["topic"] = "device";
	doc_1["clientid"] = "ESP_ENV";
	doc_1["type"] = "rssi";
	doc_1["value"] = 92;

	serializeJson(doc, Serial);
}


void loop(){
	add();
	add();
	prepare();
	// test();
	delay(5000);
}