#include "Arduino.h"
#include "messages.h"

#define ANALOG_INPUTS     1

int ins[6];   // Cannot exceed 6 analog inputs 
bool triggered = false;
static constexpr int THRESHOLD = 22;
int count = 0;

void setup()
{
  // initialize serial baud. Must be 9600 to conform
  // with python
  Serial.begin(230400);
}

void loop()
{

  for (int i = 0; i < ANALOG_INPUTS; ++i) {
    ins[i] = analogRead(i);
  }

  for (int j = 0; j < ANALOG_INPUTS; ++j) {
    if (ins[j] < THRESHOLD && !triggered)
    {
      triggered = true;
      Serial.print("GOOOOOOAAAAAALLLLLLL\n");
      Serial.print("Threshold: ");
      Serial.print(ins[j]);
      Serial.print("\n");
      Serial.print("Score: ");
      Serial.print(++count);
      Serial.print("\n");
    }
    else if (ins[j] >= THRESHOLD && triggered)
    {
      triggered = false;
      Serial.print("OUT\n");
    }
  }

}
