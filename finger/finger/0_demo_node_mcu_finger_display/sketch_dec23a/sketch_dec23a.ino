/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Adafruit_Fingerprint.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"
RTC_DS1307 rtc;
SoftwareSerial mySerial(12, 14);

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;

ESP8266WiFiMulti WiFiMulti;
String host = "192.168.0.103";

int year_;
int month_;
int day_;
int h_;
int m_;
int s_;


void setup() {

  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(300);
  digitalWrite(13, LOW);

  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println(__TIME__);
  Serial.println(__DATE__);



  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Tenda_482F00", "12345678");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  rtc.begin();
  finger.begin(57600);
  if (finger.verifyPassword()) {
    display.setCursor(0, 0);
    display.println("Found FP Module");
    // finger.emptyDatabase();
    //rtc.adjust(DateTime(2019, 12, 23, 7, 14, 0));

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


  for (int i = 1; i <= 6; i++)
  {
    http_date_time(i);
  }
  delay(500);


  digitalWrite(13, HIGH);
  delay(300);
  digitalWrite(13, LOW);
  delay(300);
  digitalWrite(13, HIGH);
  delay(300);
  digitalWrite(13, LOW);


}

void loop() {

  http_get("");
  delay(50);
}

void oled( String data)
{

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(data);
  DateTime time = rtc.now();
  display.setCursor(0, 0);
  char buf2[] = "DD-MMM-YY hh:mm:ss";
  display.println(time.toString(buf2));
  display.setCursor(0, 20);
  char buf3[] = "DD-MMM-YY DDD";
  display.println(time.toString(buf3));
  display.display();

}
uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  oled("Insert Finger");
  digitalWrite(13, HIGH);
  delay(300);
  digitalWrite(13, LOW);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        oled("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        oled("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        oled("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        oled("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      oled("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      oled("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      oled("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      oled("Error");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      oled("Error");
      return p;
    default:
      Serial.println("Unknown error");
      oled("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  oled("Remove finger");
  digitalWrite(13, HIGH);
  delay(300);
  digitalWrite(13, LOW);
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  oled("insert finger again");
  digitalWrite(13, HIGH);
  delay(300);
  digitalWrite(13, LOW);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        oled("");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        oled(".....");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        oled("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        oled("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        oled("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      oled("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      oled("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      oled("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      oled("Error");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      oled("Error");
      return p;
    default:
      Serial.println("Unknown error");
      oled("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
    oled("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    oled("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    oled("Fingerprints not Match");
    return p;
  } else {
    Serial.println("Unknown error");
    oled("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    oled("Stored");
    String b;
    b = id;
    http_get1(b);
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    oled("Unknown error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    oled("Unknown error");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    oled("Unknown error");
    return p;
  } else {
    Serial.println("Unknown error");
    oled("Unknown error");
    return p;
  }
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      oled("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      oled("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      oled("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      oled("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      oled("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      oled("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      oled("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      oled("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      oled("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      oled("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      oled("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    oled("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    oled("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    oled("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    oled("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  String b;
  b = finger.fingerID;
  http_get2(b);



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
  http_get2(b);
  return finger.fingerID;
}
void http_get(String data)
{
  int fid;
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://" + host + "/attendance/chk")) { // HTTP
      // start connection and send HTTP header
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          fid = payload.toInt();
          Serial.println(payload);
          //Serial.println("int : "+String(fid));

          //    id =a+1;
          // while (!  getFingerprintEnroll() );


          if (fid == 0) {
            getFingerprintIDez();
            oled("Waiting For Finger");
          } else if (fid > 0) {
            id = fid;
            while (!  getFingerprintEnroll() );
          }
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        oled("[HTTP] Error");
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
      oled("Unable to connect");
    }
  }
}

void http_get1(String data)
{
  int fid;
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://" + host + "/attendance/regu/" + data)) { // HTTP
      // start connection and send HTTP header
      //client.println("GET /smartEdu/public/regu/"+a);
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          fid = payload.toInt();
          Serial.println(payload);
          //Serial.println("int : "+String(fid));

          digitalWrite(13, HIGH);
          delay(300);
          digitalWrite(13, LOW);
          delay(300);
          digitalWrite(13, HIGH);
          delay(300);
          digitalWrite(13, LOW);
          delay(300);
          digitalWrite(13, HIGH);
          delay(300);
          digitalWrite(13, LOW);
          delay(300);


        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        oled("[HTTP] Error");
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
      oled("Unable to connect");
    }
  }
}
void http_get2(String data)
{
  int fid;
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    DateTime time = rtc.now();
    char buf2[] = "DD-MMM-YY";
    char buf3[] = "hh:mm:ss";

    Serial.println("http://" + host + "/attendance/atd/" + data + "/" + String(time.toString(buf2)));

    if (http.begin(client, "http://" + host + "/attendance/atd/" + data + "/" + String(time.toString(buf2)) + "_" + String(time.toString(buf3)) )) { // HTTP
      // start connection and send HTTP header
      //client.println("GET /smartEdu/public/atd/"+a);
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          fid = payload.toInt();
          Serial.println(payload);
          display.clearDisplay();
          display.setTextSize(4);
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0, 0);
          display.println(data);
          display.display();
          delay(2000);
          display.clearDisplay();
          display.setTextSize(3);
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0, 0);
          display.println(payload);
          display.display();
          delay(1000);
          digitalWrite(13, HIGH);
          delay(300);
          digitalWrite(13, LOW);
          //Serial.println("int : "+String(fid));
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        oled("[HTTP] Error");
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
      oled("Unable to connect");
    }
  }
}

void http_date_time(int a)
{

  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://" + host + "/attendance/time/" + String(a))) { // HTTP
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);


          if (a == 1)
          {
            year_ = payload.toInt();
          } else if (a == 2)
          {
            month_ = payload.toInt();
          } else if (a == 3)
          {
            day_ = payload.toInt();
          } else if (a == 4)
          {
            h_ = payload.toInt();
          } else if (a == 5)
          {
            m_ = payload.toInt();
          } else if (a == 6)
          {
            s_ = payload.toInt();
          }
          rtc.adjust(DateTime(year_, month_, day_, h_, m_, s_));

        }
      }
      http.end();
    } else {
    }
  }
}
