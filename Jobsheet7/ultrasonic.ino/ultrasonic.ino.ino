#define triggerPin D5
#define echoPin D6
long duration;
int jarak;

void setup() {
 Serial.begin (9600);
 pinMode(triggerPin, OUTPUT);
 pinMode(echoPin, INPUT);
// pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
 digitalWrite(triggerPin, LOW);
 delayMicroseconds(2);
 digitalWrite(triggerPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(triggerPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 jarak = duration * 0.034 / 2;
 Serial.print(jarak);
 Serial.println(" cm");
 delay(1000);
}
