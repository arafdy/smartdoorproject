#include <Adafruit_Fingerprint.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


char auth[] = "bfGOU2h-Lw66ImvhfIo373JosuULZD6o";

// konek wifi
char ssid[] = "TheHunter";//masukin ssid 
char pass[] = "Ramon2121";//masukin paass

char buff [20]="";
SoftwareSerial mySerial(D5, D6);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  pinMode(D2,INPUT_PULLUP);
  Blynk.begin(auth, ssid, pass); 
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");

}

void loop()                     // run over and over again
{
  Blynk.run();
  int tmpID = -1;
  getFingerprintID();
  alarm();
  
  
  delay(10);            //don't ned to run this at full speed.
}

void alarm(){
 bool button = digitalRead(D2);
  if(button){
    Serial.println("pintu terbuka");
    Blynk.notify("pintu terbuka paksa");
    delay(10000);
    }
  }

BLYNK_WRITE(V1)
{
  bool pinValue = param.asInt();
  if(pinValue){
    Serial.println("pintu terbuka");
    }else {
      Serial.println("pintu tertutup");
      }
  
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
     // Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
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

    int jari = finger.fingerID;
    sprintf(buff ,"user id: %d",jari);
    
    Serial.println("Found a print match!:");
    Serial.println((String)buff);
    Blynk.notify((String)buff);
    delay(4000);
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    Blynk.notify("seorang tidak dikenal mencoba masuk");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
//  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
//  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

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
  return finger.fingerID;
}
