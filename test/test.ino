///////////// ZUMO ///////////
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4OLED display;
Zumo32U4Motors motors;

unsigned long previousMillis = 0;
const long interval = 500;

int x = 0;

void setup()
{
	Wire.begin(4);				  	// join i2c bus with address #4
	Wire.onReceive(receiveEvent); 	// register event
	Wire.onRequest(requestEvent);
	Serial.begin(115200);			// start serial for output
}

void loop()
{
	delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
	while(1 < Wire.available()) // loop through all but the last
  	{
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  	}

	int x = Wire.read();
	int leftSpeed = 200;
	int rightSpeed = 200;

	if (x == 0){
		leftSpeed = rightSpeed/2;
	}
	else if (x == 1){
		leftSpeed += 30;
		rightSpeed += 30;
	}
	else if (x == 2){
		rightSpeed = leftSpeed/2;
	}
	else if (x == 3){
		leftSpeed -= 10;
	}
	else if (x == 4){
		leftSpeed -= 30;
		rightSpeed -= 30;
	}
	else if (x == 5){
		rightSpeed -= 10;
	}

	int beltSpeedLeft = constrain(leftSpeed, 0, 400);
	int beltSpeedRight = constrain(rightSpeed, 0, 400);
	motors.setSpeeds(beltSpeedLeft,beltSpeedRight);
}

void requestEvent(int howMany){
	Wire.write("x is ");        // sends five bytes
	Wire.write(x);              // sends one byte  
}

void buffer(){
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval)
	{
		int x = Wire.read(); // receive byte as an integer
		display.clear();
		display.print(x);
		previousMillis = currentMillis;
		Serial.println(x); // print the integer
		if(x == 50){
			Wire.beginTransmission(1); // transmit to device #4
  			Wire.write("x is ");        // sends five bytes
  			Wire.write(50);              // sends one byte  
  			Wire.endTransmission();
		}
	}
}