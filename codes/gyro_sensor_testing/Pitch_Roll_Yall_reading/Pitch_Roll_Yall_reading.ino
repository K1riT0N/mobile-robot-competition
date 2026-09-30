#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();
  Serial.println("MPU6050_light test");
}

void loop() {
  mpu.update();
  Serial.print("Pitch: "); Serial.print(mpu.getAngleX());
  Serial.print(" | Roll: "); Serial.print(mpu.getAngleY());
  Serial.print(" | Yaw: "); Serial.println(mpu.getAngleZ());
  delay(500);
}
