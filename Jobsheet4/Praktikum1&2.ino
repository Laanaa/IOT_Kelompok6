// praktikum 1 dan 2
#define LED_BUILTIN 5

void setup() {
  // put your setup code here, to run once:
   pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);           
  digitalWrite(LED_BUILTIN, LOW); 
  delay(1000);      
}
