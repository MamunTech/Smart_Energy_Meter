#include <SoftwareSerial.h>
#include <String.h>
 
SoftwareSerial mySerial(2,3); //RX and TX


void setup()
{
  mySerial.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);    // the GPRS baud rate 
  delay(500);
}
 
void loop()
{
  //after start up the program, you can using terminal to connect the serial of gprs shield,
  //if you input 't' in the terminal, the program will execute SendTextMessage(), it will show how to send a sms message,
  
 
  if (Serial.available())
    switch(Serial.read())
   {
     case 't':
       SendTextMessage();
       break;
     case 'd':
       DialVoiceCall();
       break;
    
   } 
  if (mySerial.available())
    Serial.write(mySerial.read());
}
 
///SendTextMessage()
///this function is to send a sms message
void SendTextMessage()
{
  mySerial.println("AT+CMGF=1");    //Because we want to send the SMS in text mode
  delay(1000);
  mySerial.println("AT + CMGS = \"+88019XXXXXXXX\"\r");//send sms message, be careful need to add a country code before the cellphone number
  delay(1000);
  mySerial.println("Bismillah hir rahmanir rahim");//the content of the message
  delay(100);
  mySerial.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(100);
  mySerial.println();
}
 ///DialVoiceCall
///this function is to dial a voice call
void DialVoiceCall()
{
  mySerial.println("ATD + +88019XXXXXXXX;");//dial the number with country code ex:  mySerial.println("ATD + +880192XXXXXXX;")
  delay(100);
  mySerial.println();
}

