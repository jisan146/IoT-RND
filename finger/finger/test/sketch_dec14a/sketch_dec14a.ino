

#include <Adafruit_Fingerprint.h>
SoftwareSerial mySerial(6, 7);
#include <SPI.h>
#include <Ethernet.h>


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

IPAddress ip(169,254,132,250);
IPAddress myDns(192, 168, 0, 1);


EthernetClient client;


IPAddress server(169,254,132,222);


unsigned long lastConnectionTime = 0;           
const unsigned long postingInterval = 10*1000; 

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

void setup() {
  Serial.begin(9600);
  while (!Serial);  
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");


  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    finger.emptyDatabase();

    Serial.println("Now database is empty :)");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

    // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  Ethernet.begin(mac, ip, myDns);
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  delay(1000);

}
uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
void loop() {

  if (client.available()) {
    char c = client.read();
    finger.getTemplateCount();
    int a=int(finger.templateCount);
    Serial.write(c);


    if(c=='1'){ finger.getTemplateCount();
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");

      id =a+1;
      while (!  getFingerprintEnroll() );}
      else if(c!='1')
      {
        
        
        getFingerprintIDez();
        
        

        
      }
    }

  // if ten seconds have passed since your last connection,
  // then connect again and send data:
 /* if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }*/

    httpRequest();
    delay(1000);

  }

  uint8_t getFingerprintEnroll() {

    int p = -1;
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
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
      p = finger.getImage();
    }
    Serial.print("ID "); Serial.println(id);
    p = -1;
    Serial.println("Place same finger again");
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
      String b;
      b=id;
      httpRequest1(b);
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

  void httpRequest() {
    client.stop();
    if (client.connect(server, 80)) {
      client.println("GET /smartEdu/public/reg");
      client.println("Connection: close");
      client.println();
      lastConnectionTime = millis();
    } else {
      Serial.println("connection failed");
    }
  }

  void httpRequest1(String a) {
    client.stop();
    if (client.connect(server, 80)) {
      client.println("GET /smartEdu/public/regu/"+a);
      client.println("Connection: close");
      client.println();
      lastConnectionTime = millis();
    } else {
      Serial.println("connection failed");
    }
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
    b=finger.fingerID;
    httpRequest2(b);

    

    return finger.fingerID;
  }
  void httpRequest2(String a) {
    client.stop();
    if (client.connect(server, 80)) {
      client.println("GET /smartEdu/public/atd/"+a);
      client.println("Connection: close");
      client.println();
      lastConnectionTime = millis();
    } else {
      Serial.println("connection failed");
    }
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
    b=finger.fingerID;
    httpRequest2(b);
    return finger.fingerID; 
  }
