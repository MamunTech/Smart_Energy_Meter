#include <ESP8266WiFi.h>
#include <ArduinoJson.h>


void setup()
{
  
  Serial.begin(9600);
  
}

void loop()
{
  
 
  handleIndex();

  delay(3000);
}

void handleIndex()
{
  // Send a JSON-formatted request with key "type" and value "request"
  // then parse the JSON-formatted response with keys "gas" and "distance"
  DynamicJsonDocument doc(256);////allocate 256 if not working then allocate 1024 or 2048 and more if not working.
                                    //but remamber them more you allocate the more space then the flash memory then  it willl create problem ( ArduinoJson version 6+)
  double gas = 0, distance = 0;
  // Sending the request
  doc["type"] = "request";
  serializeJson(doc,Serial);
  // Reading the response
  boolean messageReady = false;
  String message = "";
  while(messageReady == false) { // blocking but that's ok
    if(Serial.available()) {
      message = Serial.readString();
      //Serial.println(message);
      messageReady = true;
    }
  }
  // Attempt to deserialize the JSON-formatted message
  DeserializationError error = deserializeJson(doc,message);
  if(error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  distance = doc["distance"];
  gas = doc["gas"];
  Serial.println(distance);
  Serial.println(gas);


}
