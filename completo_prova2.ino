#include "RTClib.h"
RTC_DS1307 rtc;
#include "DHT.h"
#include <SPI.h>
#include <SD.h>
const int trigPin = 9;
const int echoPin = 6;
float duration;
float distance;
char livello_serbatoio[] = "Livello serbatoio:";
#define DHTPIN 2     // pin digitale collegato al sensore dht
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
File myFile;
// change this to match your SD shield or module;
const int chipSelect = 10;
int motorPin = 3;



void setup () {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  dht.begin();
  while (!Serial);{
  Serial.print("Initializing SD card...");
  Serial.print("Speed 0 to 255");
  Serial.println("But the advice 50 to 255. Because the minimum voltage required to start the motor is 50.");
 
    }
  
  rtc.begin();
 // rtc.adjust(DateTime(2022, 03, 05, 9, 12, 0));
    // Open serial communications and wait for port to open:  
  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");  
  
  }



  



  
  
  


void loop () {
  DateTime adesso = rtc.now();
  // distanza  
  digitalWrite(trigPin,LOW);
  delayMicroseconds(20);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin,LOW);
    
  // take average of 10 measurements
  duration = 0.0;
  for (int i=0; i<10; i++)
  {
    duration += pulseIn(echoPin, HIGH);
  }
  distance = duration*0.017;  // 10 measurements means another divider!!

  // temperatura e umidità 
   float h = dht.readHumidity();
  //legge l'umidità
  float t = dht.readTemperature();
  // legge la temperatura con i gradi celsius
  // controlla se la lettura è andata a buon fine.
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }



    myFile = SD.open("test9.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    myFile.print(prefixaZero(adesso.day()));
    myFile.print(",");
    myFile.print(prefixaZero(adesso.month()));
    myFile.print(",");    
    myFile.print(prefixaZero(adesso.hour()));
    myFile.print(",");    
    myFile.print(prefixaZero(adesso.minute()));
    myFile.print(",");    
    myFile.print(prefixaZero(adesso.second()));
    myFile.print(",");   
    myFile.print(distance);   
    myFile.print(",");   
    myFile.print(t);  
    myFile.print(",");   
    myFile.println(h);    
    // close the file:
    myFile.close();
  } 
   
  delay(5000);
  
   if (Serial.available())
  {
    int speed = Serial.parseInt();
    if (speed >= 50 && speed <= 255)
    {
      analogWrite(motorPin, speed);
    }
  }
  delay(5000);
}



String prefixaZero(uint8_t numero) {
  if (numero < 10) {
    return "0" + String(numero);
  }
  return String(numero);
}
