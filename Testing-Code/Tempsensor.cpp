int temp = 0;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(temp, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(temp);
  int novolt = 0.00489 * sensorValue;
  int fin = novolt/0.01;
  // print out the value you read:
  Serial.println(fin);
  delay(1000);  // delay in between reads for stability
  
}
