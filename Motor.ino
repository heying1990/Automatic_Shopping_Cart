int transistorPin = 5; // connect to transistor base 

void setup() {                          
  pinMode(transistorPin, OUTPUT);
}

void loop() {                           // read the potentiometer, convert it to between 0 - 255 for the value accepted by the digital pin.
  analogWrite(transistorPin, 255);
  delay(2000);
  analogWrite(transistorPin, 127);
  delay(2000);
  analogWrite(transistorPin, 63);
  delay(2000);
  analogWrite(transistorPin, 0);
  delay(2000);
}

