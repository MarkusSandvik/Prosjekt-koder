#include <WiFi.h>
#include <esp_now.h>

// Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc)
#define BOARD_ID 1

// timegjennomgang esp32 øving 2 oppg 3
const int sensorPin = 32;
uint8_t broadcastAddress[] = {0x44, 0x17, 0x93, 0x5E, 0x42, 0x84}; // sent data fram og tilbake

float outgoingSensorData;
int incomingId;
float incomingTemp;
float incomingHum;
int incomingReadingId;
String success;

unsigned int readingId = 0;

typedef struct struct_message {
    int id;
    float temp;
    float hum;
    int readingId;
} struct_message;

/*
typedef struct struct_message {
    float sensorData;
    char* msg1;
    float num1;
} struct_message;
*/

struct_message outgoingData;
struct_message incomingData;
esp_now_peer_info_t peerInfo;

void onDataSent(const uint8_t* mac_addr, esp_now_send_status_t status){
    Serial.println("Last Packet Send Status: ");
    if(status == 0){
        success = "Delivery Success :)";
    } else{
        success = "Delivery Fail :(";
    }
    Serial.println(success);
}

void onDataRecv(const uint8_t* mac, const uint8_t* callbackData, int len){
    memcpy(&incomingData, callbackData, sizeof(callbackData));
    Serial.print("Bytes recieved: ");
    Serial.println(len);
    incomingId = incomingData.id;
    incomingTemp = incomingData.temp;
    incomingHum = incomingData.hum;
    incomingReadingId = incomingData.readingId;

    Serial.println(incomingId);
    Serial.println(incomingTemp);
    Serial.println(incomingHum);
    Serial.println(incomingReadingId);
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(sensorPin, INPUT);
    WiFi.mode(WIFI_STA);
    if(esp_now_init() != ESP_OK){
        Serial.println("Error init ESP-NOW");
        return;
    }

    esp_now_register_send_cb(onDataSent);
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if(esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    esp_now_register_recv_cb(onDataRecv);
}

void loop() {
    // put your main code here, to run repeatedly:
    /*
    outgoingData.sensorData = 420;
    outgoingData.msg1 = "A piece of cake";
    outgoingData.num1 = 420;
    */

    outgoingData.id = BOARD_ID;
    outgoingData.temp = 69;
    outgoingData.hum = 314;
    outgoingData.readingId = readingId++;

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*) &outgoingData,

    sizeof(outgoingData));
    if(result == ESP_OK){
        Serial.println("Sent successfully");
    }else{
        Serial.println("Error sending data");
    }
    delay(5000);
}