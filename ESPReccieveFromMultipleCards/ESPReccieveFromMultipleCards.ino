#include "Deploii_ESP32.h"

Deploii oi; // lager en "instans"/objekt av biblioteket

const char ssid[] = "ToHotToSpot";               // Sett inn nettverksnavn her
const char passord[] = "12345678";                    // Sett inn nettverkspassord her
const char kortID[] = "qMz6XnE0940m2CFXrb5R"; // Sett inn Kort-ID her

const int maxCardAmount = 10;  // Needs to give the arrays a fixed size, set to 10 for prototype

String knownCardsArray[maxCardAmount]; 
float efficiencyFactorsArray[maxCardAmount]; 
float drivingEfficiencyArray[maxCardAmount]; 
bool newDataGainedArray[maxCardAmount];

bool newDataGained = false;
float averageEfficiencyFactor;                                                          // Make variabel global or return value from from function / Cant return multiple values in C
float averageDrivingEfficiency;                                                         // Make variabel global or return value from from function / Cant return multiple values in C

unsigned long previousMillis = 0;
const long interval = 5000;

unsigned long previousSendMillis = 0;


void setup()
{
  Serial.begin(9600); // Aktiverer seriell overvåker i Arduino IDE

  oi.oppkobling(ssid, passord, kortID); // kobler opp til Deploii-nettsiden

  oi.intervall(2000, sendData);     // Biblioteksfunksjon som kaller sendData funksjonen hvert andre sekund
  oi.intervall(2000, sendKortData); // Kaller sendKortData funksjonen hvert andre sekund
}

void sendData()
{ // Sender data fra kort til nettsiden
  unsigned long currentMillis = millis();

  if (currentMillis - previousSendMillis > interval){
    previousSendMillis = currentMillis;
    oi.send("efffactor", averageEfficiencyFactor); // Biblioteksfunksjon som sender data
    oi.send("driveeff", averageDrivingEfficiency);
  }
}


void mottaData(String modulID, String data)
{ // Får data fra nettsiden (halvveis biblioteksfunksjon)
  // printer aller innkommende kommandoer
  Serial.print("modulID: ");
  Serial.println(modulID);
  Serial.println(data);

  // Reager på tastetrykk fra tastaturmodulen på Deploii
  if (modulID == "tastatur")
  { // reagerer på kontrollpanelmodulen tastatur
    /*
    if (data == "W") { //Skrur LED på om "W" trykkes på nettsiden
      digitalWrite(LED, HIGH);
    }

    if (data == "S") { //Skrur LED av om "S" trykkes på nettsiden
      digitalWrite(LED, LOW);
    }
    */
  }
}

void sendKortData()
{ // Sender data fra kort til andre kort
  float floatArr[4] = {45, random(1, 20), random(50, 250), 2.0};
  // oi.sendKort("kortID", floatArr, 4);           // sender til void mottaKortFArray
  oi.sendKort("kortID", "dataID", floatArr, 4); // sender til void mottaKortFArrayMedIDs
}

void mottaKortFArrayMedID(String kortID, String dataID, float data[], int len)
{ // Mottar float array med dataID

  Serial.print("kortID: ");
  Serial.println(kortID);

  Serial.print("dataID: ");
  Serial.println(dataID);

  newDataGained = true;

  for (int i = 0; i < maxCardAmount; i++) 
  {
    if (knownCardsArray[i] == "") // Adds as new card and data if space is empty
    {
      knownCardsArray[i] = kortID;

      efficiencyFactorsArray[i] = data[0]; // Stores data in same address as card 
      drivingEfficiencyArray[i] = data[1];
      break;
    }

    else if (kortID == knownCardsArray[i]) // Replaces data if card is allready known
    {
      efficiencyFactorsArray[i] = data[0];
      drivingEfficiencyArray[i] = data[1];
      newDataGainedArray[i] = true;
      break;
    }
  }
  for (int i = 0; i < len; i++)
  {
    Serial.println(data[i]);
  }
  Serial.println();
}

void processData()
{
  byte newDataPoints = 0;
  float efficiencyFactorsSum = 0;
  float drivingEfficiencySum = 0;

  for (int i = 0; i < maxCardAmount; i++)
  {
    if (newDataGainedArray[i] == true) // Prevents using inactive cars for calculations
    {
      newDataPoints += 1;
      efficiencyFactorsSum += efficiencyFactorsArray[i];
      drivingEfficiencySum += drivingEfficiencyArray[i];
    }
  }

  averageEfficiencyFactor = efficiencyFactorsSum / newDataPoints;
  averageDrivingEfficiency = drivingEfficiencySum / newDataPoints;

  resetDataForCalculations();
}

void resetDataForCalculations(){
  for (int i = 0; i < maxCardAmount; i++){
    newDataGainedArray[i] = false;
  }
  newDataGained = false;
}

void loop()
{
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    for (int i = 0; i < maxCardAmount; i++){
      Serial.print(knownCardsArray[i]);
      Serial.print("   ");
      Serial.print(efficiencyFactorsArray[i]);
      Serial.print("   ");
      Serial.println(drivingEfficiencyArray[i]);
    }
    processData();
  }
  oi.loop(); // Holder oppkoblingen til serveren gående
  /* OBS! OBS! Det er veldig viktig at det ikke brukes delay noen som helst sted i koden.
  Delay vil bryte opp koblingen mellom mikrokontrolleren og nettsiden.
  Bruk oi.intervall() funksjonen for å lage repeternde hendelser istedenfor. */

}