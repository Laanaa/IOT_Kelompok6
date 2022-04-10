#define PIN_LDR A0
#define RUANG_TAMU D1  
#define RUANG_TERAS D2
#define RUANG_KELUARGA D3  

void setup() {
  Serial.begin(9600);
  pinMode(RUANG_TAMU, OUTPUT);
  pinMode(RUANG_TERAS, OUTPUT);
  pinMode(RUANG_KELUARGA, OUTPUT);
}

void loop() {
 int a = analogRead(A0);

 Serial.println(a);
if (a > 50) {
//  Ketika intensitas cahaya menurun, maka semua menyala
digitalWrite(RUANG_TAMU, HIGH);
digitalWrite(RUANG_TERAS, HIGH);
digitalWrite(RUANG_KELUARGA, HIGH);
}
else {
//  Ketika intensitas cahaya naik/ada cahaya, maka semua mati
digitalWrite(RUANG_TAMU, LOW);
digitalWrite(RUANG_TERAS, LOW);
digitalWrite(RUANG_KELUARGA, LOW);
}
 
 delay(2000);
}
