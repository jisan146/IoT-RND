#include "SPI.h"
#include "MFRC522.h"
#include <Keypad.h>
#define SS_PIN 53
#define RST_PIN 49
#include <LiquidCrystal.h> 
#include <SPI.h>
#include <Ethernet.h>

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 122);
IPAddress myDns(192, 168, 0, 1);

// initialize the library instance:
EthernetClient client;

char server[] = "www.srbuildersbd.com";  // also change the Host line in httpRequest()
//IPAddress server(192,168,0,101);

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 100;  // delay between updates, in milliseconds

int Contrast=100;
 const int rs = 31, en = 35, d4 = 37, d5 = 39, d6 = 41, d7 = 43;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {40,42,44,46}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {38,36,34,32}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(A0,OUTPUT);
  
  analogWrite(33,Contrast);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
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
   
    
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
 
}
String key_pad="",send_data="", strID = "",test="",s_replay="",p_replay="";
void loop() {


 if (client.available()) {
    char c = client.read();
   // s_replay=String(c);
   
  s_replay=s_replay+String(c);
    
    if (String(c)=="`")
    {
      p_replay=s_replay;
    p_replay.trim();
    s_replay="";
    p_replay.replace("`","");
    Serial.println(p_replay);
    lcd.print(p_replay);
    }}
  
  char key = keypad.getKey();
 
  if (key != NO_KEY){
   // Serial.print(key);
   
     lcd.setCursor(0, 1);
    
  // Print a message to the LCD.
  key_pad=key_pad+key;
  if (key=='D') {lcd.begin(16, 2);key_pad="";strID = "";}
  else
  {
    if (key_pad.length()>16)
  {
    lcd.clear();
    lcd.setCursor(0, 1);
    key_pad="";strID = "";
    lcd.print(">>>>>>error<<<<<<");
    
  }else
    lcd.print(key_pad);
  }
  if (key=='C') {send_data=strID+"|"+key_pad;httpRequest();lcd.begin(16, 2);key_pad="";strID = "";}
  
 
  Serial.println(key_pad);
  
    tone(A0,3000);
     
    delay(300);
    
    noTone(A0);
  }
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

  strID="";key_pad="";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();

  // ??????????????? ?????? strID ????????? UID ??????????????
  // ?????????????????????? ???? ???????? IF ????????????
  // LED ???????? ??????????

  Serial.println(strID);
  tone(A0,3000);
  delay(300);
  noTone(A0);
  lcd.begin(16, 2);
     lcd.setCursor(0, 0);
 
  // Print a message to the LCD.

  send_data=strID;
 httpRequest();
  //lcd.print(strID);
  
  
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void httpRequest() {
  test="";
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 8086)) {
    Serial.println("connecting...");
   test="      SAVE";
    // send the HTTP GET request:
    client.println("GET /?p="+send_data);
    client.println("Host: www.arduino.cc");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    test="      FAIL";
     
  }
}
