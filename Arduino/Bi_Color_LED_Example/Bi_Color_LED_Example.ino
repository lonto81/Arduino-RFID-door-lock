/*
Bi-Colour LED Control
Arunkumar M
February 19, 2011

Fades a bi-colour LED between its two colours.
*/

boolean switchSequencer = HIGH;
boolean transienceSequencer = HIGH;

int weight[] = {0, 50};
int transientWeight[2];
int weightCounter[2];

unsigned long int cycleStartTime;
unsigned long int transientDelay = 30;

void setup()
{
     pinMode(10, OUTPUT);
pinMode(11, OUTPUT);

transientWeight[0] = weight[0];
transientWeight[1] = weight[1];

weightCounter[0] = transientWeight[0];
weightCounter[1] = transientWeight[1];

cycleStartTime = millis();
}

void loop()
{
if(switchSequencer)
{
     digitalWrite(10, HIGH);
     digitalWrite(11, LOW);

     -- weightCounter[0];
     if(weightCounter[0] <= 0)
     {
     weightCounter[0] = transientWeight[0];
     switchSequencer = LOW;
     }
}
else
{
     digitalWrite(10, LOW);
     digitalWrite(11, HIGH);

     -- weightCounter[1];
     if(weightCounter[1] <= 0)
     {
     weightCounter[1] = transientWeight[1];
     switchSequencer = HIGH;
     }
}

if((millis() - cycleStartTime) >= transientDelay)
{
     cycleStartTime = millis();

     if(transienceSequencer)
     {
     ++ transientWeight[0];
     -- transientWeight[1];

     if((transientWeight[0] - transientWeight[1]) == (weight[1] - weight[0]))
     {
          transienceSequencer = LOW;
     }
     }
     else
     {
     -- transientWeight[0];
     ++ transientWeight[1];

     if((transientWeight[1] - transientWeight[0]) == (weight[1] - weight[0]))
     {
          transienceSequencer = HIGH;
     }
     }
}
}
