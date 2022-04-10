#define trigger_pin 12 //6
#define Echo_pin 14 //5
#define LED 2

long duration;
int distance;


void setup() {
pinMode(trigger_pin, OUTPUT); 
pinMode(LED, OUTPUT); 
pinMode(Echo_pin, INPUT); 
Serial.begin(9600); 
}

void loop() {
  
digitalWrite(trigger_pin, LOW); 
delayMicroseconds(2);


digitalWrite(trigger_pin, HIGH);  
delayMicroseconds(10);            
digitalWrite(trigger_pin, LOW);  


duration = pulseIn(Echo_pin, HIGH); // save time duration value in "duration variable
distance= duration*0.034/2; //Convert pulse duration into distance


if ( distance < 10)
digitalWrite(LED, HIGH);
else 
digitalWrite(LED, LOW);

Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
delay(1000);
}
