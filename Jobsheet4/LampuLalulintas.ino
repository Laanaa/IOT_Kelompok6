#define red1 D0
#define yellow1 D1
#define green1 D2
#define red2 D3
#define yellow2 D4
#define green2 D5
#define red3 D6
#define yellow3 D7
#define green3 D8

void setup() {
  pinMode(red1, OUTPUT);
  pinMode(yellow1, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(yellow2, OUTPUT);
  pinMode(green2, OUTPUT);
  pinMode(red3, OUTPUT);
  pinMode(yellow3, OUTPUT);
  pinMode(green3, OUTPUT);
}

void loop() {
  // Transition move to Traffic 1 to 3
  digitalWrite(green1, LOW);
  digitalWrite(red2, HIGH);
  digitalWrite(yellow1, HIGH);
  delay(1000);

  //Traffic 3 Run
  digitalWrite(yellow1, LOW);
  digitalWrite(red3, LOW);
  digitalWrite(red1, HIGH);
  digitalWrite(red2, HIGH);
  digitalWrite(green3, HIGH);
  delay(5000);

  //Transition move to Traffic 3 to 2
  digitalWrite(green3, LOW);
  digitalWrite(yellow3, HIGH);
  delay(1000);

  //Trafic 2 Run
  digitalWrite(yellow3, LOW);
  digitalWrite(red2, LOW);
  digitalWrite(red3, HIGH);
  digitalWrite(green2, HIGH);
  delay(5000);

  //Transition move to Traffic 2 to 1
  digitalWrite(green2, LOW);
  digitalWrite(yellow2, HIGH);
  delay(1000);

  //Trafic 1 Run
  digitalWrite(yellow2, LOW);
  digitalWrite(red1, LOW);
  digitalWrite(red2, HIGH);
  digitalWrite(green1, HIGH);
  delay(5000);
}
