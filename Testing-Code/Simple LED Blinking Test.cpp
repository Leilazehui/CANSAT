int ledpin1 = 9;
int ledpin2 = 6;
int ledpin3 = 4;

void setup() {
  pinMode(ledpin1, OUTPUT);
  pinMode(ledpin2, OUTPUT);
  pinMode(ledpin3, OUTPUT);
  

}

void loop() {
  digitalWrite(ledpin1, HIGH);
  delay(1000);
  digitalWrite(ledpin1, LOW);
  delay(1000);
  digitalWrite(ledpin2, HIGH);
  delay(1000);
  digitalWrite(ledpin2, LOW);
  delay(1000);
  digitalWrite(ledpin3, HIGH);
  delay(1000);
  digitalWrite(ledpin3, LOW);
  delay(1000);

}
