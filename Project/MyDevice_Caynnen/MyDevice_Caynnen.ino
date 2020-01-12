#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <ESP8266WiFi.h>

// WiFi network info.
char ssid[] = "IoTCodeLab";//ssid 
char wifiPassword[] = "@IoTCodeLab";//password

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "3e72ac20-2fda-11ea-8221-599f77add412";
char password[] = "eae77232abd1253b73d71fe288af32330089f050";
char clientID[] = "9c05e730-2fda-11ea-84bb-8f71124cfdfb";

unsigned long lastMillis = 0;

void setup() {
   pinMode(2, OUTPUT);
  Serial.begin(115200);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
  Cayenne.loop();

  /// ADC0 - CHANNEL 0  
  Cayenne.virtualWrite(0, 20.05);
  // Cayenne.virtualWrite(1, 0  );
  
  /// ADC0 - CHANNEL 2  
  Cayenne.virtualWrite(2, analogRead(A0));
  delay(500);
  
/// ADC0 - CHANNEL 3  
  Cayenne.virtualWrite(3, analogRead(A0));
  delay(500);
  
}


//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.

///  WRITE state LED  - Escribe estado LED  

CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");

   int i = getValue.asInt();
   digitalWrite(2, i);
 
}
