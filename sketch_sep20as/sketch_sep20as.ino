#include <SoftwareSerial.h>
SoftwareSerial SIM900A(7,8);
void setup()
{
  SIM900A.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println ("SIM900A Ready");
  delay(100);
  Serial.println ("Type s to send message or r to receive message");
  pinMode(10,OUTPUT);
 
}
void loop()
{
  if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      SendMessage();
      break;
    case 'r':
      RecieveMessage();
      break;
      case 'a':
      s();
      break;
      case 'b':
      ss();
      break;
  }
 if (SIM900A.available()>0)
   Serial.write(SIM900A.read());
  
   
   
}
 void SendMessage()
{
  Serial.println ("Sending Message");
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+01687602005\"\r"); //Mobile phone number to send message
  delay(1000);
  Serial.println ("Set SMS Content");
  SIM900A.println("Good morning, how are you doing?");// Messsage content
  delay(100);
  Serial.println ("Finish");
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println ("Message has been sent ->SMS Selesai dikirim");
}
 void RecieveMessage()
{
  Serial.println ("SIM900A Membaca SMS");
  delay (1000);
  SIM900A.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
  Serial.write ("Unread Message done");
 }
 //01979488825
 void s()
 {
  
 SIM900A.println("ATD 01687602005;"); // AT Command to receive a live SMS
  delay(1000);
  }

  void ss()
 {
  
 SIM900A.println("AT + CLCC=1\r"); // AT Command to receive a live SMS
  delay(1000);
  }
