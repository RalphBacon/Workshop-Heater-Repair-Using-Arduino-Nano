/*
	I (Ralph Bacon) fixed my Honeywell workshop heater by replacing the control board with an Arduino Nano.

	For full details See my video at https://www.youtube/ralphbacon video #133  
	(Direct link to video: https://youtu.be/TJHJeLOK6nk) 
	https://github.com/RalphBacon/Workshop-Heater-Repair-Using-Arduino-Nano
*/

#include "Arduino.h"

// Definitions of the pins
#define LED1 4
#define LED2 5
#define LED3 6
#define LED4 3

// Relays
#define RLA600W 10
#define RLA900W 11

// Push Switches
#define OnOff 7
#define Rotate 2

// Analog
#define tempSetting 19 // A5
#define thermistorPin 18  // A4

// Others
#define tilt 8

// Current on/off state
bool heaterOn = false;
bool prevHeaterState = false;
bool rotateState = false;
bool autoMode = false;

// Seconds between switch on/off (eg 60000 = 1 minute)
#define switchOnDelay 60000 // milliseconds
unsigned long onTime = millis();
unsigned long offTime = millis();

// Difference in temperature before switch off/on, stops it switching on/off
// in quick succession because the temperature is very close to that required
// eg. actual temp = 17.99, require = 18.00
#define hysterisis 0.5

// Forward declarations
float readThermistor();
int getTempSetting();
void switchLEDs(bool switchOn);

// SETUP
void setup() {
	Serial.begin(9600);

	// Heater and Fan
	pinMode(RLA600W, OUTPUT);
	pinMode(RLA900W, OUTPUT);
	digitalWrite(RLA600W, LOW);
	digitalWrite(RLA900W, LOW);

	// Tilt Switch (high is good)
	pinMode(tilt, INPUT_PULLUP);

	// On/Off (high is good, grounded via 10K resistors)
	pinMode(OnOff, INPUT);
	pinMode(Rotate, INPUT);

	// LEDs
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);
	digitalWrite(LED1, LOW);
	digitalWrite(LED2, LOW);
	digitalWrite(LED3, LOW);
	digitalWrite(LED4, LOW);
	testLEDs();

	// Initial reading
	float currTemp = readThermistor();
	int currTempSetting = getTempSetting();
	Serial.print("Current temp:");
	Serial.print(currTemp);
	Serial.print(", Required temp:");
	Serial.println(currTempSetting);

	Serial.write("Setup completed.\n");
}

void loop() {

	// Until the ON/OFF switch is HIGH do nothing initially
	if (digitalRead(OnOff)) {

		// if currently ON, turn everything off
		if (autoMode) {
			switchLEDs(false);
			turnHeatersOff();
			prevHeaterState = false;
			heaterOn = false;
			rotateState = false;
			autoMode = false;
			while (digitalRead(OnOff)) {
				;
			}
		}
		// otherwise turn it on (900W only)
		else {
			switchLEDs(true);
			prevHeaterState = heaterOn;
			heaterOn = true;
			autoMode = true;
		}

		Serial.print("On/off state now:");
		Serial.println(heaterOn ? "ON" : "OFF");

		// Wait until finger released from switch
		while (digitalRead(OnOff)) {
			;
		}
	}

	// Read the current temp/setting if heater is on and turn off if req
	if (heaterOn) {

		// Only allow infrequent calls to this routine to avoid chatter
		if (millis() > onTime + switchOnDelay) {

			float currTemp = readThermistor();
			int currTempSetting = getTempSetting();

			// Turn off if temperature has been reached but keep the
			// rotate value as-is
			if (currTemp >= currTempSetting + hysterisis) {
				Serial.print("Current temp:");
				Serial.print(currTemp);
				Serial.print(" Required temp:");
				Serial.println(currTempSetting);

				Serial.println("Requested temperature reached.");
				turnHeatersOff();
				prevHeaterState = false; //?????
				heaterOn = false;
			}

			// Update last on time
			onTime = millis();

			// Also update off time so it won't switch off immediately
			offTime = millis();
		}
	}

	// Heater not on but is monitoring ambient temp - time to switch off?
	if (!heaterOn && autoMode) {

		// Only allow infrequent calls to this routine to avoid chatter
		if (millis() > offTime + switchOnDelay) {

			float currTemp = readThermistor();
			int currTempSetting = getTempSetting();
			if (currTemp < currTempSetting - hysterisis) {
				Serial.print("Current temp:");
				Serial.print(currTemp);
				Serial.print(" Required temp:");
				Serial.println(currTempSetting);

				Serial.println("Temperature too low.");
				turn900WHeaterOn();
				heaterOn = true;

				// rotate too?
				if (rotateState) {
					Serial.println("Restoring rotating mode.");
					turn600WHeaterOnWithRotate();
				}

				// Update last off time
				offTime = millis();

				// Also update on time so that it can't switch back on immediately
				onTime = millis();
			}
		}
	}

	// if Tilt is ever high stop everything. Must switch on by user to re-start.
	if (digitalRead(tilt) == HIGH && heaterOn) {
		Serial.println("TILT detected.");
		turnHeatersOff();
		prevHeaterState = false;
		heaterOn = false;
		autoMode = false;
	}

	// if heater should be on turn on 900W (incl fan)
	if (heaterOn && !prevHeaterState) {
		turn900WHeaterOn();
		prevHeaterState = true;
	}

	// Rotate plus extra 600W heater, only when heater is ON already
	if (digitalRead(Rotate) && !rotateState && heaterOn) {
		rotateState = true;
		turn600WHeaterOnWithRotate();
		while (digitalRead(Rotate)) {
			;
		}
	}

	// Stop rotating
	if (digitalRead(Rotate) && rotateState && heaterOn) {
		turn600WHeaterOff();
		rotateState = false;
		while (digitalRead(Rotate)) {
			;
		}
	}
}

void turn900WHeaterOn() {
	digitalWrite(RLA900W, HIGH);
	Serial.println("Turning 900W and fan on.");
}

void turnHeatersOff() {
	digitalWrite(RLA600W, LOW);
	digitalWrite(RLA900W, LOW);
	Serial.println("Turning off everything.");
}

void turn600WHeaterOnWithRotate() {
	digitalWrite(RLA600W, HIGH);
	Serial.println("Rotating plus 600W heater.");
}

void turn600WHeaterOff() {
	digitalWrite(RLA600W, LOW);
	Serial.println("Stop rotating and 600W heater off.");
}

int getTempSetting() {
	int tempVal = analogRead(tempSetting);
	int degreesCent = 0;

// Read the pot for a setting
	if (tempVal < 250) degreesCent = 10;
	else if (tempVal < 350) degreesCent = 18;
	else if (tempVal < 435) degreesCent = 21;
	else if (tempVal < 500) degreesCent = 23;
	else if (tempVal < 600) degreesCent = 26;
	else
		degreesCent = 30;

	//return round(degreesCent);
	return degreesCent;
}

float readThermistor() {

	// see https://en.wikipedia.org/wiki/Steinhart-Hart_equation
	float ThermValue = 0;
	float ThermFixed = 10000;
	float LogInterim, ValInterim, Traw, Tcent; // Tfahr;
	float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

	// Take an average thermistor reading
	for (int cnt = 1; cnt < 10; cnt++) {
		int tempRead = analogRead(thermistorPin);
		//Serial.println(tempRead);
		ThermValue += (float) tempRead;
	}
	ThermValue = ThermValue / 10;

	// The calculations required to convert to C or F
	ValInterim = ThermFixed * (1023.0 / (float) ThermValue - 1.0);
	LogInterim = log(ValInterim);
	Traw = (1.0 / (c1 + c2 * LogInterim + c3 * LogInterim * LogInterim * LogInterim));
	Tcent = Traw - 273.15;

	// Fahrenheit option
	//Tfahr = (Tcent * 9.0) / 5.0 + 32.0;

	// Centigade to two decimal places
	return Tcent;
}

// Used to show that the heater is ON, even if not currently heating the air
void switchLEDs(bool switchOn) {
	digitalWrite(LED1, switchOn ? HIGH : LOW);
	digitalWrite(LED2, switchOn ? HIGH : LOW);
	digitalWrite(LED3, switchOn ? HIGH : LOW);
	digitalWrite(LED4, switchOn ? HIGH : LOW);
}

// Test routines
//
//
//
//

// Eye candy as part of setup routine
void testLEDs() {
	Serial.println("Testing LEDs");
	for (int cnt = 0; cnt < 4; cnt++) {
		digitalWrite(LED1, !digitalRead(LED1));
		delay(100);
		digitalWrite(LED2, !digitalRead(LED2));
		delay(100);
		digitalWrite(LED3, !digitalRead(LED3));
		delay(100);
		digitalWrite(LED4, !digitalRead(LED4));
		delay(100);
	}
}
