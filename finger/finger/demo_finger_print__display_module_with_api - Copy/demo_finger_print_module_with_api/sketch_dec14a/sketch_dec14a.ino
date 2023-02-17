#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#include <Adafruit_Fingerprint.h>
SoftwareSerial mySerial(6, 7);
#include "RTClib.h"
RTC_DS1307 rtc;

#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xAA, 0x48 };

IPAddress serverName(169, 254, 132, 222);
IPAddress ip(169, 254, 132, 250);
IPAddress myDns(192, 168, 0, 1);
EthernetClient client;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

const int buzzer = 5;


int sda_off = 1;

void setup()   {
  Serial.begin(9600);




  if (sda_off > 0)
  {
    rtc.begin();
    pinMode(buzzer, OUTPUT);
    pinMode(A3, OUTPUT);
    digitalWrite(A3, HIGH);

    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);


    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
      Serial.println(F("SSD1306 allocation failed"));
      for (;;); // Don't proceed, loop forever
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    finger.begin(57600);
    if (finger.verifyPassword()) {
      display.setCursor(0, 0);
      display.println("Found FP Module");
      // finger.emptyDatabase();
      //rtc.adjust(DateTime(2019, 12, 21, 12, 43, 0));
      display.setCursor(0, 10);
      display.println("Clear Database");
      finger.getTemplateCount();
      int a = int(finger.templateCount);
      display.setCursor(0, 20);
      display.println("Total Finger : " + String(a));


    } else {
      display.setCursor(0, 30);
      display.println("FPM not Found");



      while (1) {
        delay(1);
      }
    }




    display.display();
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(3500);
  } else
  {
    finger.begin(57600);
    if (finger.verifyPassword()) {

      finger.emptyDatabase();


      finger.getTemplateCount();
      int a = int(finger.templateCount);



    } else {




      while (1) {
        delay(1);
      }
    }
  }

}





void loop() {

  sendGET();
  delay(500);
  /*Serial.println(analogRead(inp));
     delay(1000);*/

}



void oled( String c)
{

  if (sda_off > 0)
  { display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.println(c);
    DateTime time = rtc.now();
    display.setCursor(0, 0);
    char buf2[] = "DD-MMM-YY hh:mm:ss";
    display.println(time.toString(buf2));
    display.setCursor(0, 20);
    char buf3[] = "DD-MMM-YY DDD";
    display.println(time.toString(buf3));
    display.display();

  }
  Serial.println(c);
  Serial.println("Sda  " + String(sda_off));
}

uint8_t getFingerprintEnroll() {
  if (sda_off == 0)
  {

    int p = -1;
    pinMode(buzzer, OUTPUT);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
        case FINGERPRINT_OK:
          Serial.println("Image taken");
          break;
        case FINGERPRINT_NOFINGER:
          Serial.println(".");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Imaging error");
          break;
        default:
          Serial.println("Unknown error");
          break;
      }
    }

    // OK success!

    p = finger.image2Tz(1);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
      default:
        Serial.println("Unknown error");
        return p;
    }

    Serial.println("Remove finger");
    pinMode(buzzer, OUTPUT);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
      p = finger.getImage();
    }
    Serial.print("ID "); Serial.println(id);
    p = -1;
    Serial.println("Place same finger again");
    pinMode(buzzer, OUTPUT);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);

    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
        case FINGERPRINT_OK:
          Serial.println("Image taken");
          break;
        case FINGERPRINT_NOFINGER:
          Serial.print(".");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Imaging error");
          break;
        default:
          Serial.println("Unknown error");
          break;
      }
    }

    // OK success!

    p = finger.image2Tz(2);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
      default:
        Serial.println("Unknown error");
        return p;
    }

    // OK converted!
    Serial.print("Creating model for #");  Serial.println(id);

    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
      Serial.println("Prints matched!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {
      Serial.println("Fingerprints did not match");
      return p;
    } else {
      Serial.println("Unknown error");
      return p;
    }

    Serial.print("ID "); Serial.println(id);
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK) {
      Serial.println("Stored!");
      pinMode(buzzer, OUTPUT);
      digitalWrite(buzzer, HIGH);
      delay(300);
      digitalWrite(buzzer, LOW);
      delay(300);
      pinMode(buzzer, OUTPUT);
      digitalWrite(buzzer, HIGH);
      delay(300);
      digitalWrite(buzzer, LOW);
      delay(300);
      pinMode(buzzer, OUTPUT);
      digitalWrite(buzzer, HIGH);
      delay(300);
      digitalWrite(buzzer, LOW);
      String b;
      b = id;
      sendGET1(b);
      delay(2000);
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      return p;
    } else if (p == FINGERPRINT_BADLOCATION) {
      Serial.println("Could not store in that location");
      return p;
    } else if (p == FINGERPRINT_FLASHERR) {
      Serial.println("Error writing to flash");
      return p;
    } else {
      Serial.println("Unknown error");
      return p;
    }

  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}



uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  String b;
  b = finger.fingerID;
  sendGET2(b);



  return finger.fingerID;
}



int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  String b;
  b = finger.fingerID;
  sendGET2(b);
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);

  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(b);
  display.display();

  delay(1000);

  return finger.fingerID;
}




void sendGET() //client function to send/receive GET request data.
{
  Ethernet.begin(mac, ip, myDns);
  Serial.println(Ethernet.localIP());

  if (client.connect(serverName, 80)) {  //starts client connection, checks for connection
    // oled("connected");
    String pageLink =  String("GET /smartEdu/public/reg");
    client.println(pageLink); //download text
    client.println("Host: www.oil-soft.com");
    client.println("Connection: close");  //close 1.1 persistent connection
    client.println(); //end of get request
  }
  else {
    oled("connection failed");
  }

  while (client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
    Serial.print(c); //prints byte to serial monitor


    finger.getTemplateCount();
    int a = int(finger.templateCount);


    if (c == '1') {

      if (sda_off == 0) {
        finger.getTemplateCount();
        Serial.print("Sensor contains ");
        Serial.print(finger.templateCount);
        Serial.println(" templates");

        id = a + 1;
        while (!  getFingerprintEnroll() );
      }


    }
    else if (c != '1')
    {
      oled("Wait for Finger");
      getFingerprintIDez();
    }


  }
  client.stop(); //stop client
}

void sendGET1(String a) //client function to send/receive GET request data.
{
  Ethernet.begin(mac, ip, myDns);
  Serial.println(Ethernet.localIP());

  if (client.connect(serverName, 80)) {  //starts client connection, checks for connection
    oled("connected");
    String pageLink =  String("GET /smartEdu/public/regu/" + a);
    client.println(pageLink); //download text
    client.println("Host: www.oil-soft.com");
    client.println("Connection: close");  //close 1.1 persistent connection
    client.println(); //end of get request
  }
  else {
    oled("connection failed");
  }

  while (client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
    Serial.print(c); //prints byte to serial monitor

    oled(String(c));

  }
  client.stop(); //stop client
}


void sendGET2(String a) //client function to send/receive GET request data.
{
  Ethernet.begin(mac, ip, myDns);
  Serial.println(Ethernet.localIP());

  if (client.connect(serverName, 80)) {  //starts client connection, checks for connection
    //oled("connected");
    String pageLink =  String("GET /smartEdu/public/atd/" + a);
    client.println(pageLink); //download text
    client.println("Host: www.oil-soft.com");
    client.println("Connection: close");  //close 1.1 persistent connection
    client.println(); //end of get request
  }
  else {
    oled("connection failed");
  }

  while (client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
    Serial.print(c); //prints byte to serial monitor

    //oled(String(c));

  }
  client.stop(); //stop client
}
