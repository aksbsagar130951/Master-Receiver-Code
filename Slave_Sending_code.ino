#include "DHT.h"
#include <SPI.h>
#include <LoRa.h>
int pot = 15;
#define DHTPIN 4
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
int thresholdValue = 2000;
int rel = 13;

//define the pins used by the LoRa transceiver module
//#define SCK 18
//#define MISO 19
//#define MOSI 23
//#define SS 5
//#define RST 14
//#define DIO0 2

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6




//packet counter
int readingID = 2;

int counter = 0;
String LoRaMessage = "";

float temperature = 0;
float humidity = 0;
int sensorValue = 0;
int sensor=0;

//Initialize LoRa module
void startLoRA(){
  //SPI LoRa pins
//  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
//  LoRa.setPins(SS, RST, DIO0);

  while (!LoRa.begin(BAND) && counter < 10) {
    Serial.print(".");
    counter++;
    delay(500);
  }
  if (counter == 10) {
    // Increment readingID on every new reading
    readingID++;
    Serial.println("Starting LoRa failed!"); 
  }
  Serial.println("LoRa Initialization OK!");
  delay(2000);
}


void getReadings(){
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  int val = map(analogRead(pot),0,4096,0,100);

sensorValue =map(analogRead(pot),0,4096,100,0);

}
void sendReadings() {
  LoRaMessage = String(readingID) + "/" + String(temperature) + "&" + String(humidity) + "#" + String (sensorValue);
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
}
void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  pinMode(pot,INPUT);
  pinMode(rel,OUTPUT);
  startLoRA();
  dht.begin();
}
void loop() {
  getReadings();
  sendReadings();
  sensor = analogRead(pot);
  Serial.print(sensor);
  if(sensor < thresholdValue){
    Serial.println(" - Doesn't need watering");
    digitalWrite(rel, HIGH);
    
  }
  else {
    Serial.println(" - Time to water your farm");
    digitalWrite(rel, LOW);
    
  }
  delay(500);
  delay(500);
}
