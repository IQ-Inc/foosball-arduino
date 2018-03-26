#include "Arduino.h"
#include "messages.h"

int count = 0;


volatile unsigned char sensors = 0;

enum class Masks : unsigned char {
	yellowGoal = 0x01;
	blackGoal  = 0x02;
	yellowDrop = 0x04;
	blackDrop  = 0x08;
}

// Individual signals
const char* const YellowGoalSignal  = "YG"; // 1
const char* const BlackGoalSignal   = "BG"; // 2
const char* const YellowDropSignal  = "YD"; // 4
const char* const BlackDropSignal   = "BD"; // 8

// Double/Half Signals
const char* const DoubleDropSignal    = "DD"; // both drops
const char* const DoubleGoalSignal    = "DG"; // both goals
const char* const DoubleYellowSignal  = "DY"; // yellow drop and yellow goal
const char* const DoubleBlackSignal   = "DB"; // black drop and black goal
const char* const HalfYellowSignal    = "HY"; // yellow drop and black goal
const char* const HalfBlackSignal     = "HB"; // black drop and yellow goal

// Triple Signals
const char* const YellowGoalTripleSignal = "YT"; // yellow goal and both drops
const char* const BlackGoalTripleSignal  = "BT"; // black goal and both drops
const char* const YellowDropTripleSignal = "TY"; // yellow drop and both goals
const char* const BlackDropTripleSignal  = "TB"; // black drop and both goals

// Quadruple Signal
const char* const AllSensorsSignal = "AS"; // All Sensors

constexpr int yellowGoalPin = 2;
constexpr int blackGoalPin  = 3;
constexpr int yellowDropPin = 4;
constexpr int blackDropPin  = 5;

constexpr int EventDelay = 250;// Time in ms between registering consecutive goals
void setup()
{
	Serial.begin(115200);

  pinMode(yellowGoalPin, INPUT_PULLUP);
  pinMode(blackGoalPin, INPUT_PULLUP);
  pinMode(yellowDropPin, INPUT_PULLUP);
  pinMode(blackDropPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(yellowGoalPin), yellowGoalHandler, LOW);
  attachInterrupt(digitalPinToInterrupt(blackGoalPin), blackGoalHandler, LOW);
}

void yellowGoalHandler()
{
  
}

void blackGoalHandler()
{
  
}


// Since we only have two interrupts poll the ball drops 
void checkForBallDrops()
{
  if(!digitalRead(DblackDropPin))
  {
    ins[BlackDropPin] = THRESHOLD + 1;
  }

  if(!digitalRead(DyellowDropPin))
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
           ins[j] = 0;
						break;

					case BlackGoalPin:
						Serial.println(BlackGoalSignal);
            ins[j] = 0;
						break;

					case YellowDropPin:
						Serial.println(YellowDropSignal);
           ins[j] = 0;
						break;

					case BlackDropPin:
						Serial.println(BlackDropSignal);
            ins[j] = 0;
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

