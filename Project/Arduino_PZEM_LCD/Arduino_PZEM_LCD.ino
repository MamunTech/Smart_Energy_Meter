//For GSM Start
#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial mySerial(2,3); //RX and TX
//For GSM End


//For PZEM004T Start
#include <PZEM004Tv30.h>
/* Use software serial for the PZEM
 * Pin 10 Rx (Connects to the Tx pin on the PZEM)
 * Pin 11 Tx (Connects to the Rx pin on the PZEM)
*/
PZEM004Tv30 pzem(10, 11);//rx, tx
float Current=0.0;
float Power=0.0;
float PF=0.0;
//For PZEM004T End


//for LCD start 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//for LCD end


int Relay=13; //Relay control pin

void setup()
{ 
  
  pinMode(Relay,OUTPUT);
  digitalWrite(Relay,LOW); //Relay make on
  
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
  lcd.setCursor(15,0);
  lcd.print("A");
  
  lcd.setCursor(0,1);
  lcd.print("Power  : ");
  lcd.setCursor(15,1);
  lcd.print("W");
  
  lcd.setCursor(0,2);
  lcd.print("Pf     : ");
   //For LCD test END 

  mySerial.begin(9600);               // the GPRS baud rate 
  Serial.begin(9600);
     
}

void loop()
{
   //For PXEM004 start
    float current = pzem.current();
    Current=current;
    if( !isnan(current) ){
        Serial.print("Current: "); Serial.print(current); Serial.println("A");
    } else {
        Serial.println("Error reading current");
    }

    float power = pzem.power();
    Power=power;
    if( !isnan(power) ){
        Serial.print("Power: "); Serial.print(power); Serial.println("W");
    } else {
        Serial.println("Error reading power");
    }
    
    float pf = pzem.pf();
    PF=pf;
    if( !isnan(pf) ){
        Serial.print("PF: "); Serial.println(pf);
    } else {
        Serial.println("Error reading power factor");
    }

    Serial.println();
    
    if(Power>1000){   //if power is more than greater than 1000W or 1KW
      digitalWrite(Relay,HIGH);
      }
    
    delay(2000);
    //for PXME END
    
    
    LcdDisplay();//show on LCD Display
  
}


void LcdDisplay(){
  lcd.setCursor(9,0);
  lcd.print(Current);
  lcd.setCursor(9,1);
  lcd.print(Power);
  lcd.setCursor(9,2);
  lcd.print(PF);
  }
