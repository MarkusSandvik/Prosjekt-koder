///////// ESP32 ///////

#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop()
{
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  Serial.println(x);
  x++;
  if (x%50 == 0){
    Wire.requestFrom(4,6);
    while(1 < Wire.available())    // slave may send less than requested
    {
      char c = Wire.read();     // receive a byte as character
      Serial.print(c);          // print the character
    }
    int c  = Wire.read(); // receive byte as an integer
	  Serial.println(c); // print the integer
  }
  delay(500);
}


