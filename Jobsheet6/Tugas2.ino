#include <Arduino.h>
#include <DHT.h>
#define DHTTYPE DHT11
DHT dht(D2, DHTTYPE);
#define PIN_LDR A0
#define SUHU_TINGGI D0  
#define SUHU_RENDAH D1


void setup() {
  Serial.begin(9600);
  pinMode(SUHU_TINGGI, OUTPUT);
  pinMode(SUHU_RENDAH, OUTPUT);
}

void loop() {
 int a = analogRead(A0);
 float h = dht.readHumidity();
 float t = dht.readTemperature();
 Serial.print("Nilai Sensor Cahaya : ");
 Serial.print(a);

 float hic = dht.computeHeatIndex(t, h, false);
 Serial.print(F(", Humidity: "));
 Serial.print(h);
 Serial.print(F("% Temperature: "));
 Serial.println(t);
 
 if (a > 50 && t < 17) {
  for (int i=0; i<3; i++){
//  Hidupkan LED
    digitalWrite(SUHU_RENDAH, HIGH);
    digitalWrite(SUHU_TINGGI, LOW);
    delay(100);
//  Padamkan LED
    digitalWrite(SUHU_RENDAH, LOW);
    digitalWrite(SUHU_TINGGI, LOW);
    delay(100);
    };
}
else {
digitalWrite(SUHU_TINGGI, HIGH);
digitalWrite(SUHU_RENDAH, LOW);
}

 
 delay(2000);
}
