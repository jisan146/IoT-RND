#include "SPI.h"
#include "MFRC522.h"
#include <LiquidCrystal.h> 
#define SS_PIN 6
#define RST_PIN 7
#define SP_PIN 8
#include <SPI.h>
#include <Ethernet.h>

int Contrast=100;
 const int rs = 14, en = 15, d4 = 16, d5 = 17, d6 = 18, d7 = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 123);
IPAddress myDns(192, 168, 0, 1);

// initialize the library instance:
EthernetClient client;

//char server[] = "www.srbuildersbd.com";  // also change the Host line in httpRequest()
IPAddress server(103,108,140,63);

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10*1000;  // delay between updates, in milliseconds

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup() {
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("...Activating...");
   pinMode(5, OUTPUT);
 analogWrite(2,Contrast);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
     lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Device Activated");
     tone(5,30);
  delay(2000);
  noTone(5);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Device Activated");
     tone(5,30);
  delay(2000);
  noTone(5);
  
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
   
  
}
String idcard;
void loop() {
  
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();

  // เมื่อถึงส่วนนี้ ตัวแปร strID จะเก็บค่า UID ของแท็กไว้แล้ว
  // สามารถนำไปใช้งานได้เลย เช่น นำไปเข้า IF เพื่อให้หลอด
  // LED ติดสว่าง หรือดับได้
  Serial.print("Tap card key: ");
  Serial.println(strID);
  idcard=strID;
 httpRequest();
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  
}
int a=0,b=0;
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 8086)) {
    Serial.println("connecting...");
    // send the HTTP GET request:
    client.println("GET /?p="+idcard);
    client.println("Host: www.srbuildersbd.com");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
 tone(5,30);
  delay(1000);
  noTone(5);
a=a+1;
b=0;
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print(">>>>> Save <<<<<");
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print("Count >>  "+String(a));
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    b=b+1;
    // if you couldn't make a connection:
    Serial.println("connection failed");
     lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("XXXXX Fail XXXXX");
   lcd.setCursor(0, 1);
  // print the number of seconds since reset:
   lcd.print("Fail XX>>  "+String(b));
  a=0;
  }
}
