//#define BLYNK_PRINT Serial
//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <BlynkSimpleEsp32.h>
#include <LoRa.h>
#include <SPI.h>
//#include <SimpleTimer.h>
//define the pins used by the LoRa transceiver module
#define SCK 18
#define MISO 19
#define MOSI 23
#define SS 5
#define RST 14
#define DIO0 2
#define BAND 866E6
//SimpleTimer timer;
//char auth[] = "eunmDL3FHf0kOY5gA9Qs60gyvSDlTgEd";
//char ssid[] = "RSD";
//char pass[] = "airrsd8896"; 

int rssi;
String loRaMessage;
String temperature;
String humidity;
String sensorValue;
String readingID;
String temperature1;
String humidity1;
String sensorValue1;


void startLoRA(){
  int counter;
//  SPI LoRa pins;
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
    Serial.println("Starting LoRa failed!"); 
  }
  Serial.println("LoRa Initialization OK!");
}
void getLoRaData() {
  Serial.print("Lora packet received: ");
  // Read packet
  while (LoRa.available()) {
    String LoRaData = LoRa.readString();
    // LoRaData format: readingID/temperature&soilMoisture#batterylevel
    // String example: 1/27.43&654#95.34
    Serial.print(LoRaData); 
    
    // Get readingID, temperature and soil moisture
    int pos1 = LoRaData.indexOf('/');
    int pos2 = LoRaData.indexOf('&');
    int pos3 = LoRaData.indexOf('#');
    readingID = LoRaData.substring(0, pos1);
    if (readingID =="1"){
    temperature = LoRaData.substring(pos1 +1, pos2);
    humidity = LoRaData.substring(pos2+1, pos3);
   sensorValue = LoRaData.substring(pos3+1, LoRaData.length());}
   if(readingID =="2"){
    temperature1 = LoRaData.substring(pos1 +1, pos2);
    humidity1 = LoRaData.substring(pos2+1, pos3);
   sensorValue1 = LoRaData.substring(pos3+1, LoRaData.length());}
   
//   Blynk.virtualWrite(V1, temperature); 
//  Blynk.virtualWrite(V2, humidity); 
//  Blynk.virtualWrite(V3, sensorValue);
//   Blynk.virtualWrite(V4, temperature1); 
//  Blynk.virtualWrite(V5, humidity1); 
//  Blynk.virtualWrite(V6, sensorValue1);    
//  }
  // Get RSSI
  rssi = LoRa.packetRssi();
  Serial.print(" with RSSI ");    
  Serial.println(rssi);
}
}

void setup() 
{
  Serial.begin(115200);
//  Blynk.begin(auth, ssid, pass);
  
  startLoRA();
}
void loop() 
{  
//  Blynk.run();
//  timer.run();  
  int packetSize = LoRa.parsePacket();    // try to parse packet
  if (packetSize) 
  {
    getLoRaData();
}
}
