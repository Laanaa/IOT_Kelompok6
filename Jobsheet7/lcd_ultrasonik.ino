#include <LiquidCrystal_I2C.h>
#define trigger_pin 12 //6
#define Echo_pin 14 //5
//#define LED 2

LiquidCrystal_I2C lcd (0x27, 16,2);  //pengenalan alamat i2c
long duration;
int distance;

void  setup () {

  pinMode(trigger_pin, OUTPUT); 
 //
 
// pinMode(LED, OUTPUT); 
  pinMode(Echo_pin, INPUT); 
  Serial.begin(9600); 
  lcd. init ();
  lcd. backlight ();
  lcd. print ( "Test Ultrasonik" );
  lcd. backlight ();
}
 
void  loop () {
   
  digitalWrite(trigger_pin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);          
  digitalWrite(trigger_pin, LOW); 
  duration = pulseIn(Echo_pin, HIGH); // save waktu durasi parsing data
  distance= duration*0.034/2; //ubahdurasi ke jarak
   
    
  lcd. setCursor (0, 1);
  lcd.print("Jarak: ");
  lcd.print(distance);
  lcd.print(" cm");

  
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(3000);

  
}
