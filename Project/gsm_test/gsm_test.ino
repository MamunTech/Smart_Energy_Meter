#include <SoftwareSerial.h>

SoftwareSerial SIM900(7, 8);//rx, tx

void setup()
{
SIM900.begin(19200);
Serial.begin(19200);
}

void loop()
{
if (SIM900.available())
  Serial.write(SIM900.read());
if (Serial.available())
  SIM900.write(Serial.read()); 
}
