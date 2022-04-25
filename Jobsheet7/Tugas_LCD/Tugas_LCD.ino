#include <RTClib.h>

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "RTClib.h"

#define RED_LED D5 //Led Red
#define GREEN_LED D6 //Led Red
#define BLUE_LED D7 //Led Blue
#define DHTTYPE DHT11 //DHT 11

DHT dht(D3, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

void setup() {
  lcd.init(); // LCD
  lcd.backlight();
  pinMode(RED_LED, OUTPUT); //LED
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  lcd.print("Loading.....");
  lcd.home();
}

void scrollText(int row, String message, int delayTime, 
int lcdColumns)
 {
   for (int i = 0; i < lcdColumns; i++)
   {
     message = " " + message;
   }
   message = message + " ";
   for (int pos = 0; pos < message.length(); pos++)
   {
     lcd.setCursor(0, row);
     lcd.print(message.substring(pos, pos + lcdColumns));
     delay(delayTime);
   }
 }

void red(){
  for (int x = 0; x < 3; x++){
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    delay(150);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    delay(150);
  }
}

void green(){
  for (int x = 0; x < 3; x++){
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
    delay(150);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    delay(150);
  }
}

void blue(){
  for (int x = 0; x < 3; x++){
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
    delay(150);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    delay(150);
  }
}

void mati(){
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  delay(1000);
}

void printAngka(int digits){
   if(digits < 10){
   lcd.print('0');
   lcd.print(digits);
   }
   else lcd.print(digits);
 }

void loop() {
  // Menampilkan S
  delay(2000);
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(t) | isnan(f))
  {
    scrollText(1, "DHT tidak terdeteksi.", 250, 16);
    mati();
    return;
  }

  lcd.home();
  lcd.print(t);
  lcd.print("\337C|");
  lcd.print(f);
  lcd.print("\337F");
  
// Menampilkan LED
  if (t >= 35)
  {
    red();
    delay(100);
  }

  else if (t <= 35)
  {
    blue();
    delay(100);
  }

  else if (t <= 25 ){
    green();
    delay(100);
  }

  else
  {
    mati();
  }

// Jam
  DateTime now = rtc.now();
  lcd.setCursor(1,1);
  printAngka(now.day());
  lcd.print("-");
  printAngka(now.month());
  lcd.print("-");
  printAngka(now.year());  
}
