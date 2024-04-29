const int speedPin = 32;
const int accelerationPin = 33;
const int startCountPin = 34;

const int readings = 50;

const int accelerationOffset = 0.5;

int speedArrayCounter = 0;
int accelerationArrayCounter = 0;

float previousAverageSpeed = 0;
float previousAverageAcceleration = 0;

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
const long reportInterval = 20000;

unsigned long previousStopRemoveMillis = 0;
const long stopRemoveInterval = 60000;

bool ignition = true;
bool carStopped = false;
byte startStopCounter = 0;




void readAndStoreSpeed(){
    unsigned long currentMillis = millis();

    if (currentMillis - previousSpeedMeasurementMillis >= accelerationMeasurementInterval){
        previousSpeedMeasurementMillis = currentMillis;

        if (speedArrayCounter >= readings){
            speedArrayCounter = 0;
            speedArrayFull = true;
        } // end if

        float speedReading = analogRead(speedPin);

        float speed = map(speedReading, 0, 4095, 0, 40);                                                                                                            // Not needed when integrated

        // Only record speed when car is driving
        if (speed > 0){
            speedArray[speedArrayCounter] = speed;
        } // end if

        speedArrayCounter += 1;
    } // end if
} // end void

void readAndStoreAcceleration(){
    unsigned long currentMillis = millis();

    if (currentMillis - previousAccelerationMeasurementMillis >= speedMeasurementInterval){
        previousSpeedMeasurementMillis = currentMillis;

        if (accelerationArrayCounter >= readings){
            accelerationArrayCounter = 0;
            accelerationArrayFull = true;
        } // end if

        float accelerationReading = analogRead(accelerationPin);
        float acceleration = map(accelerationReading, 0, 4095, 0, 50); // Ten times wanted limit since map function returns int,                                    // Not needed when integrated
    acceleration = acceleration / 10; // converts from Int value to a float                                                                                         // Not needed when integrated

        // Only ads value if reading is not below offset --------------------------------- Issues while backing, but backing gives same readings ass deceleration
        if (acceleration >= accelerationOffset){
            accelerationArray[accelerationArrayCounter] = acceleration;
            accelerationArrayCounter += 1;
        } // end if
    } // end if
} // end void

float getAverageSpeed(){
    float speedSum = 0;
    int speedReadings;

    // Calculates for all values if array is filled on trip
    if (speedArrayFull == false){
        speedReadings = speedArrayCounter;
    } // end if

    // Calculates only for read values if array is not yet filled on trip
    else{
        speedReadings = readings;
    } // end else

    // Sums up all readings in array
    for (int i = 0; i < speedReadings; i++){
        speedSum += speedArray[i];
    } // end for

    float currentAverageSpeed = (speedSum / speedReadings);

    // Finds average value of current calculation and previously stored calculations.
    float averageSpeed = (currentAverageSpeed + previousAverageSpeed) / 2; 
    previousAverageAcceleration = averageSpeed;

    // Calculates and returns the average value in this interval
    return averageSpeed;
} // end float

float getAverageAcceleration(){
    float accelerationSum = 0;
    int accelerationReadings;

    // Calculates for all values if array is filled on trip
    if (accelerationArrayFull == true){
        accelerationReadings = accelerationArrayCounter;
    } // end if

    // Calculates only for read values if array is not yet filled on trip
    else{
        accelerationReadings = readings;
    } // end else

    // Sums up all readings in array
    for (int i = 0; i < accelerationReadings; i++){
        accelerationSum += accelerationArray[i];
    } // end for
    
    float currentAverageAcceleration = (accelerationSum / readings);

    // Finds average value of current calculation and previously stored calculations
    float averageAcceleration = (currentAverageAcceleration + previousAverageAcceleration) / 2; 
    previousAverageSpeed = averageAcceleration; 

    // Calculates and returns the average value in this interval
    return averageAcceleration;
} // end float

void countStartStop(){
    unsigned long currentMillis = millis();
    float speedReading = analogRead(speedPin);

    if (speedReading == 0){
        carStopped = true;
    } // end if

    if ((carStopped == true) && (speedReading != 0)){
        startStopCounter += 1;
        carStopped = false;
    } // end if

    if (currentMillis - previousStopRemoveMillis >= stopRemoveInterval){
        previousStopRemoveMillis = currentMillis;
        startStopCounter -= 1;
    } // end if
} // end void

void calculateDriverScore(){
    
}

void printReport(){
    float averageSpeed = previousAverageSpeed;
    float averageAcceleration = previousAverageAcceleration;  

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
} // end void


void setup(){
    Serial.begin(9600);
    pinMode(speedPin, INPUT);
    pinMode(accelerationPin, INPUT);
    pinMode(startCountPin, INPUT_PULLUP);
} // end setup

void loop(){
    unsigned long currentMillis = millis();

    readAndStoreSpeed();
    readAndStoreAcceleration();
    
    if (currentMillis - previousReportMillis >= reportInterval){
        previousReportMillis = currentMillis;
        printReport();
    } // end if
} // end loop