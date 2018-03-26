#include "Arduino.h"
#include "messages.h"

int count = 0;

volatile unsigned char sensors = 0;

// Masks that can be used to check for sensors
enum class Masks : unsigned char {
	yellowGoal = 0x01;
	blackGoal  = 0x02;
	yellowDrop = 0x04;
	blackDrop  = 0x08;
}

constexpr int yellowGoalPin = 2;
constexpr int blackGoalPin  = 3;
constexpr int yellowDropPin = 4;
constexpr int blackDropPin  = 5;

// Individual signals                              // | Decimal | Binary | Hex |
const char* const YellowGoalSignal  = "YG";        // |    1    | 0b0001 | 0x1 |
const char* const BlackGoalSignal   = "BG";        // |    2    | 0b0010 | 0x2 |
const char* const YellowDropSignal  = "YD";        // |    4    | 0b0100 | 0x4 |
const char* const BlackDropSignal   = "BD";        // |    8    | 0b1000 | 0x8 |

// Double/Half Signals                             
const char* const DoubleDropSignal    = "DD";      // | Decimal | Binary | Hex ||
const char* const DoubleGoalSignal    = "DG";      // |    1    | 0b0001 | 0x1 |
const char* const DoubleYellowSignal  = "DY";      // |    1    | 0b0001 | 0x1 |
const char* const DoubleBlackSignal   = "DB";      // |    1    | 0b0001 | 0x1 |
const char* const HalfYellowSignal    = "HY";      // |    1    | 0b0001 | 0x1 |
const char* const HalfBlackSignal     = "HB";      // |    1    | 0b0001 | 0x1 |
                                 
// Triple Signals
const char* const YellowGoalTripleSignal = "YT";   // |    1    | 0b0001 | 0x1 |
const char* const BlackGoalTripleSignal  = "BT";   // |    1    | 0b0001 | 0x1 |
const char* const YellowDropTripleSignal = "TY";   // |    1    | 0b0001 | 0x1 |
const char* const BlackDropTripleSignal  = "TB";   // |    1    | 0b0001 | 0x1 |

// Quadruple Signal
const char* const AllSensorsSignal = "AS"; // All Sensors

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

