
// this code will be uploaded on esp8266-12E
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>


char ssid[] = "IoTCodeLab";//ssid 
char wifiPassword[] = "@IoTCodeLab";//password

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "3e72ac20-2fda-11ea-8221-599f77add412";
char password[] = "eae77232abd1253b73d71fe288af32330089f050";
char clientID[] = "9c05e730-2fda-11ea-84bb-8f71124cfdfb";



void setup()
{
  
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  
}

void loop()
{
  
Cayenne.loop();

/*
  /// ADC0 - CHANNEL 0  ,this is a virtual channel
  Cayenne.virtualWrite(0, 20.05);
  // Cayenne.virtualWrite(1, 0  );
  
  
  /// ADC0 - CHANNEL 2, this is a virtual channel  
  Cayenne.virtualWrite(2, distance);
  delay(500);
  
/// ADC0 - CHANNEL 3,  this is a virtual channel  
  Cayenne.virtualWrite(3, gas);
  delay(500);

*/


  handleIndex();

  delay(3000);
}

void handleIndex()
{
  // Send a JSON-formatted request with key "type" and value "request"
  // then parse the JSON-formatted response with keys "gas" and "distance"
  DynamicJsonDocument doc(1024);
  double current = 0, unit = 0;
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
  current = doc["current"];
  unit = doc["unit"];
  
  /// ADC0 - CHANNEL 2  
  Cayenne.virtualWrite(2, current);
  delay(100);
  
/// ADC0 - CHANNEL 3  
  Cayenne.virtualWrite(3, unit);
  delay(100);
  
  Serial.println(current);
  Serial.println(unit);


}




CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");

   int i = getValue.asInt();
   digitalWrite(2, i);
 
}
