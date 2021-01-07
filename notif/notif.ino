#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "bfGOU2h-Lw66ImvhfIo373JosuULZD6o";

// konek wifi
char ssid[] = "TheHunter";//masukin ssid 
char pass[] = "Ramon2121";//masukin paass

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);  
}

void loop() {
  Blynk.run();
  Blynk.notify("yugo anjing!");
  Serial.println("terkirim");
  delay(5000);
}
