#include "Arduino.h"
#include "messages.h"
#include <Wiegand.h>

#define ANALOG_INPUTS     4

WIEGAND  wg;
int ins[6];   // Cannot exceed 6 analog inputs 
bool triggered = false;
static constexpr int THRESHOLD = 22;
int count = 0;


constexpr char YellowGoalSignal = 'A';
constexpr char BlackGoalSignal  = 'B';
constexpr char YellowDropSignal = 'C';
constexpr char BlackDropSignal  = 'D'; 

constexpr int YellowGoalPin = 0;
constexpr int BlackGoalPin  = 1;
constexpr int YellowDropPin = 2;
constexpr int BlackDropPin  = 3;

void setup()
{
  // initialize serial baud. Must be 9600 to conform
  // with python
  Serial.begin(230400);
  wg.begin();
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

      switch(ins[j]){

        case YellowGoalPin:
          Serial.print("");
          Serial.print(YellowGoalSignal);
          Serial.print("\n");
          break;

        case BlackGoalPin:
          Serial.print("");
          Serial.print(BlackGoalSignal);
          Serial.print("\n");
          break;

       case YellowDropPin:
          Serial.print("");
          Serial.print(YellowDropSignal);
          Serial.print("\n");
          break;

       case BlackDropPin:
          Serial.print("");
          Serial.print(BlackDropSignal);
          Serial.print("\n");
          break; 

      default:
        break;
        
      }
    }
    
    if((wg.available()))
    {
      triggered = false;
      Serial.print("S");
      Serial.print(wg.getCode(),HEX);
      Serial.print("F");
    }
  }
}

