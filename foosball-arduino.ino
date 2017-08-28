#include "Arduino.h"
#include "messages.h"
#include "wiegand/Wiegand.h"

#define ANALOG_INPUTS     4

WIEGAND  wg;
int ins[6];   // Cannot exceed 6 analog inputs 
static constexpr int THRESHOLD = 900;
int count = 0;


const char* const YellowGoalSignal = "GG";
const char* const BlackGoalSignal  = "BG";
const char* const YellowDropSignal = "GD";
const char* const BlackDropSignal  = "BD"; 

constexpr int YellowGoalPin = 0; // 1010
constexpr int BlackDropPin  = 1; // 10101
constexpr int YellowDropPin  = 2; //1010
constexpr int BlackGoalPin  = 3; //

constexpr int EventDelay = 250;// Time in ms between registering consecutive goals
void setup()
{
	// initialize serial baud. Must be 9600 to conform
	// with python
	Serial.begin(115200);
	wg.begin();
}

void loop()
{

	for (int i = 0; i < ANALOG_INPUTS; ++i) {
		ins[i] = analogRead(i);
	}

	for (int j = 0; j < ANALOG_INPUTS; ++j) {
                // Check if a goal is above the goal thresholt
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

	if(wg.available())
	{
//		Serial.print("S");
//		Serial.print(wg.getCode(),HEX);
//		Serial.print("F\n");
	}
}
