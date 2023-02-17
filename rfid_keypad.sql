#include "SPI.h"
#include "MFRC522.h"
#include <Keypad.h>
#define SS_PIN 53
#define RST_PIN 49
#include <LiquidCrystal.h> 
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
byte rowPins[ROWS] = {6,7,8,9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5,4,3,2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(A0,OUTPUT);
  
  analogWrite(33,Contrast);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
 
}
String key_pad="";String strID = "";
void loop() {
  char key = keypad.getKey();
 
  if (key != NO_KEY){
   // Serial.print(key);
   
     lcd.setCursor(0, 1);
    
  // Print a message to the LCD.
  key_pad=key_pad+key;
  if (key=='D') {lcd.begin(16, 2);key_pad="";strID = "";}else  lcd.print(key_pad);
 
  Serial.println(key_pad);
    tone(A0,30);
     
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
  tone(A0,30);
  delay(300);
  noTone(A0);
  lcd.begin(16, 2);
     lcd.setCursor(0, 0);
 
  // Print a message to the LCD.
  lcd.print(strID);
  key_pad="";
  strID = "";
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}