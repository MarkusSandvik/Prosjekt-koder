/* This example reads the raw values from the accelerometer,
magnetometer, and gyro on the Zumo 32U4, and prints those raw
values to the serial monitor.

The accelerometer readings can be converted to units of g using
the sensitivity specified in the LSM303 or LSM6DS33 datasheet.
The default full-scale (FS) setting is +/- 2 g, so the conversion
factor is 0.061 mg/LSB (least-significant bit).  A raw reading of
16384 would correspond to 1 g.

The gyro readings can be converted to degrees per second (dps)
using the sensitivity specified in the L3GD20H or LSM6DS33
datasheet.  The default sensitivity is 8.75 mdps/LSB (least-
significant bit).  A raw reading of 10285 would correspond to
90 dps.

The magnetometer readings are more difficult to interpret and
will usually require calibration. */

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4IMU imu;
Zumo32U4OLED display;

char report[120];

void setup()
{
  Wire.begin();

  if (!imu.init())
  {
    // Failed to detect the compass.
    ledRed(1);
    while(1)
    {
      Serial.println(F("Failed to initialize IMU sensors."));
      delay(100);
    }
  }

  imu.enableDefault();
}

void loop()
{
  imu.read();

  float gReadingX = imu.a.x * 9.81 / -16480; // Converts raw reading to G by deviding, and then to m/s^2 by mulitplying with g=9.81

  display.clear();
  display.print(gReadingX);
  /*
  double gReadingY = imu.a.y * 9.81 / 16514;
  double gReadingZ = imu.a.z * 9.81 / 16994;

  Serial.print(gReadingX);
  Serial.print("   ");
  Serial.print(gReadingY);
  Serial.print("   ");
  Serial.println(gReadingZ);
  */
  delay(100);
}
