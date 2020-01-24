//
int CurrentLimit=2; //Consumer current limit.alarm sms will be send, if user use more than 2A
float unit_cost=5.5; // per unit cost 5.5 tk

//For GSM Start
#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial mySerial(2,3); //RX and TX
/*
  Arduino Pin 2 Rx (Connects to the Tx pin of the GSM)
  Arduino Pin 3 Tx (Connects to the Rx pin of the GSM)
*/ 
String sms="Your Load is High!"; 
//For GSM End

//for LCD start 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//for LCD end


//this code will be uploaded on arduno
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>

/* Use software serial for the PZEM
 * Arduino Pin 10 Rx (Connects to the Tx pin of the PZEM)
 * Arduino Pin 11 Tx (Connects to the Rx pin of the PZEM)
*/
PZEM004Tv30 pzem(10, 11); //RX and TX

float A=0.0;
float V=0.0;
float unit=0.0;

String message = "";
bool messageReady = false;

float ESP_D4_to_A0=0.0; //connect esp D4 pin to arduino A0 pin
int relay_on_off=13;

int i=0;
int j=0;

void SendTextMessage()
{
  mySerial.println("AT+CMGF=1");    //Because we want to send the SMS in text mode
  delay(1000);
  mySerial.println("AT + CMGS = \"+88019XXXXXXXX\"\r");//send sms message, be careful need to add a country code before the cellphone number
  delay(1000);
  mySerial.println(sms);//the content of the message
  delay(100);
  mySerial.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(100);
  mySerial.println();
}


void pzem03(){
 
    float current = pzem.current();
    if( !isnan(current) ){
      A=current;
      
    } 
    else {
        A=0;
    }

    float voltage = pzem.voltage();
    if( !isnan(voltage) ){
      V=voltage;
      
    } 
    else {
        V=0;
    }

    float energy = pzem.energy();
    if( !isnan(energy) ){
      unit=energy;
      
    } 
    else {
        unit=unit;
    }
  }

void LcdDisplay(){
  lcd.setCursor(9,0);
  lcd.print(A);
  lcd.setCursor(9,1);
  lcd.print(V);
  lcd.setCursor(9,2);
  lcd.print(unit);
  lcd.setCursor(9,3);
  float cost=unit * unit_cost;
  lcd.print(cost);
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

void setup() {
  pinMode(relay_on_off,OUTPUT);
  mySerial.begin(9600);               // the GPRS baud rate
  Serial.begin(9600);
  digitalWrite(relay_on_off,HIGH);

   // For  LCD TEST Start
   lcd.begin(20,4); 
  lcd.setCursor(3,0);
  lcd.print("Hello Everyone");
  lcd.setCursor(8,1);
  lcd.print("****");
  lcd.setCursor(0,2);
  lcd.print("This is Energy Meter");
  lcd.setCursor(8,3);
  lcd.print("****");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Current: ");
  lcd.setCursor(17,0);
  lcd.print("A");

  
  lcd.setCursor(0,1);
  lcd.print("Volatage:");
  lcd.setCursor(17,1);
  lcd.print("V");
  
  lcd.setCursor(0,2);
  lcd.print("Unit   : ");
  lcd.setCursor(17,2);
  lcd.print("KWh");
  
  lcd.setCursor(0,3);
  lcd.print("Bill   : ");
  lcd.setCursor(17,3);
  lcd.print("TK");
   //For LCD test END 
  
  for(int j=0;j<5;j++){
    pzem03();
    delay(10);
   LcdDisplay();
  }
  
}

void loop() {
  /* //the "start" variable is declared to understand the time needed to run the full code at once
   * unsigned long start=millis();
   */
    relay();
  while(Serial.available()) {
    message = Serial.readString();
    messageReady = true;
  }
  // Only process message if there's one
  if(messageReady) {
    // The only messages we'll parse will be formatted in JSON
    DynamicJsonDocument doc(256); //allocate 256 if not working then allocate 1024 or 2048 and more if not working.
                                    //but remamber them more you allocate the more space then the flash memory then  it willl create problem ( ArduinoJson version 6+)
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
      doc["cur"] = A;
      doc["unit"] = unit;
      serializeJson(doc,Serial);
       //Serial.println(A);
      //Serial.println(W);
     // pzem03();
    }
    messageReady = false;
  }
 if(i==100){
    pzem03();
    LcdDisplay();//show on LCD Display
    i=0;
    /* //to understand the time needed to run the full code at once
     *  unsigned long time_needed_run_once_full_code=start-millis();
     *  Serial.println(time_needed_run_once_full_code) //the time will be printed in milli second
     */
  }
  
 if(A>CurrentLimit && j==0){
          SendTextMessage();
          j++;
  }        
 if(j>=1){
     j++;
     if(A>CurrentLimit && j==3000){
        SendTextMessage();
        j=1;
       }
     if(A<CurrentLimit && j==3000)
      {
        j=0;
       }
  }
  /*
  if(unit>9000){
     pzem.resetEnergy();//reset pzrm unit,Since pzem v03 can only mesure upto 9999.99 unit
    }
    */
  i++; 
}
