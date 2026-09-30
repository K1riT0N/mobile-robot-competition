#include <Wire.h>
#include <MPU6050_tockn.h>

MPU6050 mpu(Wire);

// Thresholds — adjust as needed
float leftThreshold = -5.0;   // degrees
float rightThreshold = 5.0;   // degrees

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets(true);  // auto-calibration
  Serial.println("MPU6050 ready...");
}

void loop() {
  mpu.update();
  float baseYaw = 0;

  float angleZ = mpu.getAngleZ() - baseYaw;  // yaw (rotation)
  Serial.print("Yaw: ");
  Serial.print(angleZ);
  Serial.print("° -> ");

  // Interpret direction
  if (angleZ > rightThreshold) {
    Serial.println("Slightly Right");
  } 
  else if (angleZ < leftThreshold) {
    Serial.println("Slightly Left");
  } 
  else {
    Serial.println("Straight");
    baseYaw = mpu.getAngleZ();
  }

  delay(200);
}
