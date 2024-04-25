#include "Deploii_ESP32.h"

Deploii oi; // lager en "instans"/objekt av biblioteket

const char ssid[] = "NTNU-IOT";               // Sett inn nettverksnavn her
const char passord[] = "";                    // Sett inn nettverkspassord her
const char kortID[] = "etfuka6OMLkmZgRRhClw"; // Sett inn Kort-ID her

unsigned long previousMillis = 0;
const long interval = 5000;

void setup()
{
    Serial.begin(9600); // Aktiverer seriell overvåker i Arduino IDE

    oi.oppkobling(ssid, passord, kortID); // kobler opp til Deploii-nettsiden

    oi.intervall(2000, sendData);     // Biblioteksfunksjon som kaller sendData funksjonen hvert andre sekund
    oi.intervall(2000, sendKortData); // Kaller sendKortData funksjonen hvert andre sekund
}

void sendData()
{ // Sender data fra kort til nettsiden
    float data = 42.00;
    oi.send("data", data); // Biblioteksfunksjon som sender data
}

void mottaData(String modulID, String data)
{ // Får data fra nettsiden (halvveis biblioteksfunksjon)
    // printer aller innkommende kommandoer
    Serial.print("modulID: ");
    Serial.println(modulID);
    Serial.println(data);
}

void sendKortData()
{ // Sender data fra kort til andre kort
    float floatArr[2] = {45, random(1, 20)};
    // oi.sendKort("kortID", floatArr, 4);               //sender til void mottaKortFArray
    oi.sendKort("qMz6XnE0940m2CFXrb5R", "drivingData", floatArr, 2); // sender til void mottaKortFArrayMedIDs
}

void mottaKortFArrayMedID(String kortID, String dataID, float data[], int len)
{ // Mottar float array med dataID
    Serial.print("kortID: ");
    Serial.println(kortID);
    Serial.print("dataID: ");
    Serial.println(dataID);

    for (int i = 0; i < len; i++)
    {
        Serial.println(data[i]);
    }
    Serial.println();
}

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval){
        sendKortData();
        previousMillis = currentMillis;
    }

    oi.loop(); // Holder oppkoblingen til serveren gående
    /* OBS! OBS! Det er veldig viktig at det ikke brukes delay noen som helst sted i koden.
    Delay vil bryte opp koblingen mellom mikrokontrolleren og nettsiden.
    Bruk oi.intervall() funksjonen for å lage repeternde hendelser istedenfor. */
}