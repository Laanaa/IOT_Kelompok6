#include <LiquidCrystal_I2C.h>
#define trigger_pin D5 //6
#define Echo_pin D6 //5
#define LED_MERAH D7
#define LED_KUNING D4
#define LED_HIJAU D3
//#define LED 2

LiquidCrystal_I2C lcd (0x27, 16,2);  //pengenalan alamat i2c
long duration;
int distance;

void  setup () {

  pinMode(trigger_pin, OUTPUT); 
  pinMode(LED_MERAH, OUTPUT); 
  pinMode(LED_KUNING, OUTPUT); 
  pinMode(LED_HIJAU, OUTPUT); 
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
  lcd.println(" cm ");
  
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000);

  if(distance == 2 ){
    digitalWrite(LED_MERAH, LOW); 
    digitalWrite(LED_KUNING, LOW);
    digitalWrite(LED_HIJAU, HIGH);
  } else if (distance == 3){
    digitalWrite(LED_MERAH, LOW); 
    digitalWrite(LED_KUNING, HIGH);
    digitalWrite(LED_HIJAU, LOW);
  } else if(distance == 4){
    digitalWrite(LED_MERAH, HIGH); 
    digitalWrite(LED_KUNING, LOW);
    digitalWrite(LED_HIJAU, LOW);
  } else if(distance > 4) {
    for(int i=0;i<2;i++){
      digitalWrite(LED_MERAH, HIGH); 
      digitalWrite(LED_KUNING, HIGH);
      digitalWrite(LED_HIJAU, HIGH); 
      delay(500);
      digitalWrite(LED_MERAH, LOW); 
      digitalWrite(LED_KUNING, LOW);
      digitalWrite(LED_HIJAU, LOW); 
      delay(500);
    }
  } else {
    digitalWrite(LED_MERAH, LOW); 
    digitalWrite(LED_KUNING, LOW);
    digitalWrite(LED_HIJAU, LOW);
  }
  
}
