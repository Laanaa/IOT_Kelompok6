#include "DHT.h"

/*#define DHTPIN D2     */
#define LedMerah 16 //D0 pin led merah
#define LedHijau 5 //D1 pin led hijau
#define LedBiru 4 //D2 pin led biru
#define GPIO4 D7    
#define DHTTYPE DHT11   // DHT 11

DHT dht(GPIO4, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  pinMode(LedMerah, OUTPUT);
  pinMode(LedHijau, OUTPUT);
  pinMode(LedBiru, OUTPUT);

  dht.begin();
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);

  
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.println(t);

  if (t < 28){
     digitalWrite(LedMerah, LOW);
     digitalWrite(LedHijau, LOW);
     digitalWrite(LedBiru, HIGH);
     Serial.println("Suhu dingin");
     delay(1000); 
  } else if(t >= 28 && t <= 29){
     digitalWrite(LedMerah, LOW);
     digitalWrite(LedHijau, HIGH);
     digitalWrite(LedBiru, LOW);
     Serial.println("Suhu Normal");
     delay(1000);
  } else {
    digitalWrite(LedMerah, HIGH);
     digitalWrite(LedHijau, LOW);
     digitalWrite(LedBiru, LOW);
     Serial.println("Suhu Panas");
     delay(1000);
  }

 }
