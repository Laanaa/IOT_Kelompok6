#define LedMerah 16 //D0 pin led merah
#define LedHijau 5 //D1 pin led hijau
#define LedBiru 4 //D2 pin led biru

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LedMerah, OUTPUT);
  pinMode(LedHijau, OUTPUT);
  pinMode(LedBiru, OUTPUT); // atur pin digital sebagai output.
}
void RunningLED() {
 digitalWrite(LedMerah, HIGH);
 digitalWrite(LedHijau, LOW);
 digitalWrite(LedBiru, LOW);
 Serial.println("LED Merah nyala");
 delay(4000);

 digitalWrite(LedMerah, LOW);
 digitalWrite(LedHijau, HIGH);
 digitalWrite(LedBiru, LOW);
 Serial.println("LED Hijau nyala");
 delay(2000);

 digitalWrite(LedMerah, LOW);
 digitalWrite(LedHijau, LOW);
 digitalWrite(LedBiru, HIGH);
 Serial.println("LED Biru nyala");
 delay(4000);
 Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
   RunningLED();
}
