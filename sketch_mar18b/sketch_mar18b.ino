///////// ESP32 ///////
#include "Deploii_ESP32.h"
#include <Wire.h>

Deploii oi; //lager en "instans"/objekt av biblioteket

const char ssid[] = "NTNU-IOT";  //Sett inn nettverksnavn her
const char passord[] = "";  //Sett inn nettverkspassord her
const char kortID[] = "etfuka6OMLkmZgRRhClw";  //Sett inn Kort-ID her



void setup()
{
  Serial.begin(115200);
  Wire.begin(); // join i2c bus (address optional for master)

  oi.oppkobling(ssid, passord, kortID); //kobler opp til Deploii-nettsiden
}

void sendData() { //Sender data fra kort til nettsiden
  float data = 32.00;
  float temperatur = random(0,100);  // Simulerer en sensor for å få mer variasjon i grafen i Deploii
  oi.send("data", data); //Biblioteksfunksjon som sender data
  oi.send("temperatur", temperatur);
}

void sendButtonToZumo(byte direction){
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write("Direction is ");        // sends five bytes
  Wire.write(direction);              // sends one byte  
  Wire.endTransmission();
}

void mottaData(String modulID, String data) { //Får data fra nettsiden
  //Reager på kontrollpanelmodulene
  if (modulID == "tastatur") { //reagerer på kontrollpanelmodulen tastaturstyring
    if (data == "Q") { //Skrur LED på om "W" trykkes på nettsiden
      sendButtonToZumo(0);
    }

    if (data == "W") { //Skrur LED av om "S" trykkes på nettsiden
      sendButtonToZumo(1);
    }

    if (data == "E") { //Skrur LED av om "S" trykkes på nettsiden
      sendButtonToZumo(2);
    }

    if (data == "A") { //Skrur LED av om "S" trykkes på nettsiden
      sendButtonToZumo(3);
    }

    if (data == "S") { //Skrur LED av om "S" trykkes på nettsiden
      sendButtonToZumo(4);
    }

    if (data == "D") { //Skrur LED av om "S" trykkes på nettsiden
      sendButtonToZumo(5);
    }

  }
}

byte x = 0;

void loop()
{
  oi.loop(); //Holder oppkoblingen til serveren gående
  /* OBS! OBS! Det er veldig viktig at det ikke brukes delay noen som helst sted i koden.
  Delay vil bryte opp koblingen mellom mikrokontrolleren og nettsiden. 
  Bruk oi.intervall() funksjonen for å lage repeternde hendelser istedenfor.
  */
  //oi.intervall(2000, sendData); //Kaller sendData funksjonen hvert andre sekund
  delay(500);
}



/*
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
      char c = Wire.read(); // receive a byte as character
      Serial.print(c);         // print the character
    }
    int c  = Wire.read(); // receive byte as an integer
	  Serial.println(c); // print the integer
  }
  */


