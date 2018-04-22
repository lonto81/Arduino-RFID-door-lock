/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 10;
int led2 = 11;
int led3 = 12;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT); 
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);  
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(125);
  digitalWrite(led2, LOW);
  delay(100);
  digitalWrite(led3, HIGH);
  delay(125);

  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(125); 
  digitalWrite(led2, HIGH);
  delay(100);
  digitalWrite(led3, LOW);
  delay(125);  // wait for a second
}
