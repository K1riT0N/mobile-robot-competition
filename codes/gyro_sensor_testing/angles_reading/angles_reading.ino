#include <Wire.h>
#include <MPU6050_tockn.h>

MPU6050 mpu(Wire);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets(true); // auto-calibration
}

void loop() {
  mpu.update();
  Serial.print("AngleX : "); Serial.print(mpu.getAngleX());
  Serial.print("\tAngleY : "); Serial.print(mpu.getAngleY());
  Serial.print("\tAngleZ : "); Serial.println(mpu.getAngleZ());
  delay(100);
}
