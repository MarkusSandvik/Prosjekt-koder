const int speedPin = 32;
const int accelerationPin = 33;
const int startCountPin = 34;

const int readings = 50;

const int accelerationOffset = 0.5;

int speedArrayCounter = 0;
int accelerationArrayCounter = 0;

bool speedArrayFull = false;
bool accelerationArrayFull = false;

float speedArray[readings];
float accelerationArray[readings];

unsigned long previousSpeedMeasurementMillis = 0;
const long speedMeasurementInterval = 2000;

unsigned long previousAccelerationMeasurementMillis = 0;
const long accelerationMeasurementInterval = 500;

unsigned long previousCalculationMillis = 0;
const long calculationInterval = 10000;

unsigned long previousReportMillis = 0;
const long reportInterval = 10000;


void readAndStoreSpeed(){
    unsigned long currentMillis = millis();

    if (currentMillis - previousSpeedMeasurementMillis >= accelerationMeasurementInterval){
        previousSpeedMeasurementMillis = currentMillis;

        if (speedArrayCounter >= readings){
            speedArrayCounter = 0;
            speedArrayFull = true;
        }

        float speedReading = analogRead(speedPin);

        float speed = map(speedReading, 0, 4095, 0, 40);

        speedArray[speedArrayCounter] = speed;

        speedArrayCounter += 1;
    } 
}

void readAndStoreAcceleration(){
    unsigned long currentMillis = millis();

    if (currentMillis - previousAccelerationMeasurementMillis >= speedMeasurementInterval){
        previousSpeedMeasurementMillis = currentMillis;

        if (accelerationArrayCounter >= readings){
            accelerationArrayCounter = 0;
            accelerationArrayFull = true;
        }

        float accelerationReading = analogRead(accelerationPin);
        float acceleration = map(accelerationReading, 0, 4095, 0, 50); // Ten times wanted limit since map function returns int
        acceleration = acceleration / 10; // converts from Int value to a float

        if (acceleration >= accelerationOffset){
            accelerationArray[accelerationArrayCounter] = acceleration;
            accelerationArrayCounter += 1;
        }
    }
}

float getAverageSpeed(){
    float speedSum = 0;
    int speedReadings;

    if (speedArrayFull == false){
        speedReadings = speedArrayCounter;
    }
    else{
        speedReadings = readings;
    }

    for (int i = 0; i < speedReadings; i++){
        speedSum += speedArray[i];
    }

    return (speedSum / speedReadings);
}

float getAverageAcceleration(){
    float accelerationSum = 0;
    int accelerationReadings;

    if (accelerationArrayFull == true){
        accelerationReadings = accelerationArrayCounter;
    }
    else{
        accelerationReadings = readings;
    }

    for (int i = 0; i < accelerationReadings; i++){
        accelerationSum += accelerationArray[i];
    }

    return (accelerationSum / readings);
}


void printReport(){
    float averageSpeed = getAverageSpeed();
    float averageAcceleration = getAverageAcceleration();
    Serial.print("Average Speed");
    Serial.print("       ");
    Serial.println("Average acceleration");

    Serial.print(averageSpeed);
    Serial.print("   ");
    Serial.println(averageAcceleration);

    Serial.println("Speed array");
    for(int i=0; i < readings; i++){
        Serial.print(speedArray[i]);
    }
    Serial.println("");
    Serial.println("");

    Serial.println("Acceleration array");
    for(int i=0; i < readings; i++){
        Serial.print(accelerationArray[i]);
    }
    Serial.println("");
    Serial.println("");
}


void setup(){
    Serial.begin(9600);
    pinMode(speedPin, INPUT);
    pinMode(accelerationPin, INPUT);
    pinMode(startCountPin, INPUT_PULLUP);
}

void loop(){
    unsigned long currentMillis = millis();

    readAndStoreSpeed();
    readAndStoreAcceleration();
    

    if (currentMillis - previousReportMillis >= reportInterval){
        previousReportMillis = currentMillis;
        printReport();
    }
}