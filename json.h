#ifndef json_h
#define json_h

#include <ArduinoJson.h>


void json_test(){
  char json[] =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // deserializeJson(doc, STREAM);
  
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  serializeJsonPretty(json, Serial);

  const char* sensor = doc["sensor"];
  long time = doc["time"];
  double latitude = doc["data"][0];
  double longitude = doc["data"][1];

  // Print values.
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude, 6);

  // The filter: it contains "true" for each value we want to keep
  // StaticJsonDocument<200> filter;
  // filter["list"][0]["dt"] = true;
  // filter["list"][0]["main"]["temp"] = true;

  // // Deserialize the document
  // StaticJsonDocument<400> doc;
  // deserializeJson(doc, input_json, DeserializationOption::Filter(filter));

}

#endif