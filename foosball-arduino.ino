#include "Arduino.h"
#include "messages.h"

#define ANALOG_INPUTS     4

int ins[6];   // Cannot exceed 6 analog inputs 
static constexpr int THRESHOLD = 900;
int count = 0;

const char* const YellowGoalSignal = "GG";
const char* const BlackGoalSignal  = "BG";
const char* const YellowDropSignal = "GD";
const char* const BlackDropSignal  = "BD"; 

constexpr int YellowGoalPin  = 0; // 1010
constexpr int BlackDropPin   = 1; // 10101
constexpr int YellowDropPin  = 2; //1010
constexpr int BlackGoalPin   = 3; //

constexpr int DyellowGoalPin = 7;
constexpr int DblackGoalPin  = 6;
constexpr int DyellowDropPin = 5;
constexpr int DblackDropPin  = 4;

constexpr int EventDelay = 250;// Time in ms between registering consecutive goals
void setup()
{
	// initialize serial baud. Must be 9600 to conform
	// with python
	Serial.begin(115200);

  pinMode(DyellowGoalPin, INPUT_PULLUP);
  pinMode(DblackGoalPin, INPUT_PULLUP);
  pinMode(DyellowDropPin, INPUT_PULLUP);
  pinMode(DblackDropPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(DyellowGoalPin), yellowGoalHandler, LOW);
  attachInterrupt(digitalPinToInterrupt(DblackGoalPin), blackGoalHandler, LOW);
}

void yellowGoalHandler()
{
  ins[YellowGoalPin] = THRESHOLD + 1;
}

void blackGoalHandler()
{
  ins[BlackGoalPin] = THRESHOLD + 1;
}


// Since we only have two interrupts poll the ball drops 
void checkForBallDrops()
{
  if(!digitalRead(BlackDropPin))
  {
    ins[BlackDropPin] = THRESHOLD + 1;
  }

  if(!digitalRead(YellowDropPin))
  {
    ins[YellowDropPin] = THRESHOLD + 1;
  }
}

void loop()
{

  // Check for analog based events
	for (int i = 0; i < ANALOG_INPUTS; ++i) {
		ins[i] = analogRead(i);
    checkForBallDrops();
	}

	for (int j = 0; j < ANALOG_INPUTS; ++j) {
                // Check if a goal is above the goal threshold
		if (ins[j] > THRESHOLD) {

			// Store timestamp of last goal
			static unsigned long lastGoal = millis();

                        // Ensure that there has been enough time since last event
			if ( (millis() - lastGoal) >= EventDelay) {
                                // Print out coresponding event char
				switch(j){

					case YellowGoalPin:
						Serial.println(YellowGoalSignal);
						break;

					case BlackGoalPin:
						Serial.println(BlackGoalSignal);
						break;

					case YellowDropPin:
						Serial.println(YellowDropSignal);
						break;

					case BlackDropPin:
						Serial.println(BlackDropSignal);
						break; 

					default:
						break;
				}

                                // Reet lastGoal time
                                lastGoal = millis();
			}
		}
	}
}
