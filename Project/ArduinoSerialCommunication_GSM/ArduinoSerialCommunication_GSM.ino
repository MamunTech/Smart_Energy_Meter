#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);//RX, TX

String message = "";
bool messageReady = false;

void setup() {
  mySerial.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);    // baud rate 
  delay(200);
}

void loop() {
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
    /*
    if(error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      messageReady = false;
      return;
    }*/
    if(doc["type"] == "request") {
      doc["type"] = "response";
      // Get data from analog sensors
      doc["distance"] = analogRead(A0);
      doc["gas"] = analogRead(A1);
      serializeJson(doc,Serial);
    }
    messageReady = false;
  }
}


void housekeeper()
  {
    mySerial.println("AT+CMGF=1");    //Because we want to send the SMS in text mode
  delay(1000);
  mySerial.println("AT + CMGS = \"+8801933396162\"\r");//send sms message, be careful need to add a country code before the cellphone number
  delay(1000);
  mySerial.println("Smart meter");//the content of the message
  delay(100);
  mySerial.println((char)26);//the ASCII code of the ctrl+z is 26 
  delay(100);
  mySerial.println(); 
    }
