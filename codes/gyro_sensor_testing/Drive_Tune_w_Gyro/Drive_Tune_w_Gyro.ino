#include <Wire.h>
#include <MPU6050_tockn.h>
#include <Servo.h>

MPU6050 mpu(Wire);

// --- Motor Pins ---
#define MOTOR_B_EN 5
#define MOTOR_B_1 6
#define MOTOR_B_2 7

#define MOTOR_A_EN 10
#define MOTOR_A_1 9
#define MOTOR_A_2 8

// --- Servo Pins ---
Servo myservo1;
Servo myservo2;

// --- Gyro Variables ---
float baseYaw = 0;
float kp = 3.0;   // proportional gain — tune this value for your robot

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets(true);
  Serial.println("MPU6050 calibrated.");

  // Motor setup
  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_A_EN, OUTPUT);

  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);
  pinMode(MOTOR_B_EN, OUTPUT);

  // Servo setup
  myservo1.attach(2);
  myservo2.attach(3);

  // Get initial orientation
  delay(1000);
  mpu.update();
  baseYaw = mpu.getAngleZ();
  Serial.println("Base orientation set!");
}

void loop() {
  mpu.update();
  float currentYaw = mpu.getAngleZ();
  float error = currentYaw - baseYaw;

  // Compute correction (simple proportional)
  float correction = kp * error;

  // Base motor speed
  int baseSpeed = 180;

  // Apply correction
  int motorASpeed = constrain(baseSpeed - correction, 100, 255);
  int motorBSpeed = constrain(baseSpeed + correction, 100, 255);

  // Drive forward
  digitalWrite(MOTOR_A_1, LOW);
  digitalWrite(MOTOR_A_2, HIGH);
  analogWrite(MOTOR_A_EN, motorASpeed);

  digitalWrite(MOTOR_B_1, HIGH);
  digitalWrite(MOTOR_B_2, LOW);
  analogWrite(MOTOR_B_EN, motorBSpeed);

  // Print debug
  Serial.print("Yaw: "); Serial.print(currentYaw);
  Serial.print(" | Error: "); Serial.print(error);
  Serial.print(" | A Speed: "); Serial.print(motorASpeed);
  Serial.print(" | B Speed: "); Serial.println(motorBSpeed);

  delay(50);
}
