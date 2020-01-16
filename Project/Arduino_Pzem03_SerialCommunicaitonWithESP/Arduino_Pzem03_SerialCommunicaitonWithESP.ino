
//this code will be uploaded on arduno
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>

/* Use software serial for the PZEM
 * Pin 10 Rx (Connects to the Tx pin on the PZEM)
 * Pin 11 Tx (Connects to the Rx pin on the PZEM)
*/
PZEM004Tv30 pzem(10, 11);

float A=0.0;
float unit=0.0;

String message = "";
bool messageReady = false;


float ESP_D4_to_A0=0.0; //connect esp D4 pin to arduino A0 pin
int relay_on_off=13;

void setup() {
  pinMode(relay_on_off,OUTPUT);
  Serial.begin(9600);
  digitalWrite(relay_on_off,HIGH);
}


void pzem03(){
 
    float current = pzem.current();
    if( !isnan(current) ){
      A=current;
      //Serial.println(A);
      
       // Serial.print("Current: "); Serial.print(current); Serial.println("A");
    } else {
        A=0;
    }

    float energy = pzem.energy();
    if( !isnan(energy) ){
      unit=energy;
      //Serial.println(w);
  
        //Serial.print("Power: "); Serial.print(power); Serial.println("W");
    } else {
        unit=unit;
    }

    //Serial.println();
    
    //delay(2000);
  }

  void relay(){
    
    ESP_D4_to_A0=analogRead(A0);
    
    if(ESP_D4_to_A0<100){
      digitalWrite(relay_on_off,HIGH);
      }
      else{
         digitalWrite(relay_on_off,LOW);
        }
    
    }
  
void loop() {

    relay();
  
    pzem03();
  // Monitor serial communication
  while(Serial.available()) {
    message = Serial.readString();
    messageReady = true;
  }
  // Only process message if there's one
  if(messageReady) {
    // The only messages we'll parse will be formatted in JSON
    DynamicJsonDocument doc(1024); // ArduinoJson version 6+
    // Attempt to deserialize the message
    DeserializationError error = deserializeJson(doc,message);
    //to decode any error fot arduino and esp8266 serial commnicaiton
    if(error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      messageReady = false;
      return;
    }
    if(doc["type"] == "request") {
      doc["type"] = "response";
      // Get data from analog sensors
      doc["current"] = A;
      doc["unit"] = unit;
      serializeJson(doc,Serial);
       //Serial.println(A);
      //Serial.println(W);
     // pzem03();
    }
    messageReady = false;
  }
}
