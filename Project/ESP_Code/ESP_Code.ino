
//float unit_cost=5.5;// per unit cost 5.5 tk

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
  
    handleIndex();
    delay(2000);
   
}
void handleIndex()
{
  // Send a JSON-formatted request with key "type" and value "request"
  // then parse the JSON-formatted response with keys "gas" and "distance"
  DynamicJsonDocument doc(1024);//allocate 256 if not working then allocate 1024 or 2048 and more if not working.
                                    //but remamber them more you allocate the more space then the flash memory then  it willl create problem ( ArduinoJson version 6+)
  double cur = 0, unit = 0,tk=0;
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
  cur = doc["cur"];
  unit = doc["unit"];
  tk = doc["tk"];
  
  /// ADC0 - CHANNEL 2  
  Cayenne.virtualWrite(2, cur);
  delay(100);
  
/// ADC0 - CHANNEL 3 
  Cayenne.virtualWrite(3, unit);
  delay(100);
  
//Decrypted
int taka=round(tk);
float bill=taka/10;
float fine=(tk-bill)*1000;

/// ADC0 - CHANNEL 4  
  Cayenne.virtualWrite(4,bill);
  delay(100);

  /// ADC0 - CHANNEL 5  
  Cayenne.virtualWrite(5,fine);
  delay(100);
  
  Serial.println(cur);
  Serial.println(unit);
  Serial.println(tk);

}


CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");

   int i = getValue.asInt();
   digitalWrite(2, i);
 
}
