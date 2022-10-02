#include "ACS712.h"
#include <SoftwareSerial.h>

ACS712  ACS(A0, 5.0, 1023, 66);
ACS712  ACS1(A1, 5.0, 1023, 66);


SoftwareSerial sim800l(3, 2); // RX,TX for Arduino and for the module it's TXD RXD, they should be inverted

void setup()
{

  sim800l.begin(9600);   //Module baude rate, this is on max, it depends on the version
  Serial.begin(9600);   
  delay(1000);
  ACS.autoMidPoint();
  ACS1.autoMidPoint();
}


void loop()
{
  int mA1 = ACS.mA_AC();
  int mA2 = ACS1.mA_AC();
  float result= mA2-mA1;
  if (result>155) {          
   Serial.println("THEFT Detected!");   //Shows this message on the serial monitor
    delay(200);                         
    
    SendSMS();                          //And this function is called

 }
  if (sim800l.available()){            //Displays on the serial monitor if there's a communication from the module
  Serial.write(sim800l.read()); 
  }
delay(3000);
}
void SendSMS()
{
  sim800l.print("AT+CMGF=1\r");                   //Set the module to SMS mode
  delay(100);
  sim800l.print("AT+CMGS=\"+8801997754177\"\r");  //Your phone number don't forget to include your country code, example +212123456789"
  delay(500);
  sim800l.print("SOMEONE IS STEALING POWER FROM THE LINE!");       //This is the text to send to the phone number, don't make it too long or you have to modify the SoftwareSerial buffer
  delay(60000);
  sim800l.print((char)26);// (required according to the datasheet)
  delay(500);
  sim800l.println();
  Serial.println("Text Sent.");
  delay(500);
}
