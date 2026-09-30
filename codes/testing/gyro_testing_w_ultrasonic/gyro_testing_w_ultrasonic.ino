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

// --- Ultrasonic Pins ---
#define TRIG 11
#define ECHO 12

// --- Servo Pins ---
Servo myservo1;  // เก็บของ
Servo myservo2;  // ปล่อยของ

// --- Gyro Variables ---
float baseYaw = 0;
float kp = 3.0;     // ปรับค่าให้ตรงของจริง
int baseSpeed = 160;

// --- Timing Variables ---
unsigned long prevTime = 0;
const unsigned long interval = 50; // อัปเดตทุก 50 ms (20 Hz)

// --- Mission Variables ---
int task = 0;   // ภารกิจ
float startDist = 0; // ระยะเริ่มต้น
float currentDist = 0;

// ------------------------------------------------------------
// Ultrasonic Function
// ------------------------------------------------------------
float readDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2.0; // หน่วย cm
}

// ------------------------------------------------------------
// Motor Control
// ------------------------------------------------------------
void drive(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  // Motor A (ขวา)
  if (rightSpeed >= 0) {
    digitalWrite(MOTOR_A_1, LOW);
    digitalWrite(MOTOR_A_2, HIGH);
  } else {
    digitalWrite(MOTOR_A_1, HIGH);
    digitalWrite(MOTOR_A_2, LOW);
  }
  analogWrite(MOTOR_A_EN, abs(rightSpeed));

  // Motor B (ซ้าย)
  if (leftSpeed >= 0) {
    digitalWrite(MOTOR_B_1, HIGH);
    digitalWrite(MOTOR_B_2, LOW);
  } else {
    digitalWrite(MOTOR_B_1, LOW);
    digitalWrite(MOTOR_B_2, HIGH);
  }
  analogWrite(MOTOR_B_EN, abs(leftSpeed));
}

void stopMotors() {
  analogWrite(MOTOR_A_EN, 0);
  analogWrite(MOTOR_B_EN, 0);
}

// ------------------------------------------------------------
// Gyro Straight Drive
// ------------------------------------------------------------
void goStraight(float targetYaw) {
  mpu.update();
  float currentYaw = mpu.getAngleZ();
  float error = currentYaw - targetYaw;
  float correction = kp * error;

  int leftSpeed = baseSpeed - correction;
  int rightSpeed = baseSpeed + correction;
  drive(leftSpeed, rightSpeed);

  Serial.print("Yaw: "); Serial.print(currentYaw);
  Serial.print(" | Error: "); Serial.print(error);
  Serial.print(" | Left: "); Serial.print(leftSpeed);
  Serial.print(" | Right: "); Serial.println(rightSpeed);
}

// ------------------------------------------------------------
// Servo Control
// ------------------------------------------------------------
void pickup() {
  myservo1.write(90); delay(800);
  myservo1.write(0);
}

void release() {
  myservo2.write(90); delay(800);
  myservo2.write(0);
}

// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets(true);
  Serial.println("Gyro calibrated.");

  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_A_EN, OUTPUT);
  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);
  pinMode(MOTOR_B_EN, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  myservo1.attach(2);
  myservo2.attach(3);
  myservo1.write(0);
  myservo2.write(0);

  delay(1000);
  mpu.update();
  baseYaw = mpu.getAngleZ();

  // อ่านระยะเริ่มต้นจาก ultrasonic
  startDist = readDistance();
  Serial.print("Start distance: "); Serial.println(startDist);
}

// ------------------------------------------------------------
// Loop — ทำภารกิจตามแผนที่รู้ล่วงหน้า
// ------------------------------------------------------------
void loop() {
  unsigned long now = millis();
  if (now - prevTime < interval) return;
  prevTime = now;

  currentDist = readDistance();

  switch (task) {
    case 0: // TASK 1: เดินตรง 50 cm ไปเก็บของ
      goStraight(baseYaw);
      if (startDist - currentDist >= 50) { // เดินครบ 50 cm
        stopMotors();
        pickup();
        task++;
        delay(500);
        // เตรียมหมุนขวา
        baseYaw = mpu.getAngleZ();
      }
      break;

    case 1: // TASK 2: หมุนขวา 90°
      {
        float targetYaw = baseYaw + 90;
        mpu.update();
        float currentYaw = mpu.getAngleZ();
        float error = targetYaw - currentYaw;

        if (fabs(error) > 3) drive(-120, 120);
        else {
          stopMotors();
          baseYaw = targetYaw;
          startDist = readDistance();
          task++;
          delay(500);
        }
      }
      break;

    case 2: // TASK 3: เดินตรง 30 cm ไปปล่อยของ
      goStraight(baseYaw);
      if (startDist - currentDist >= 30) {
        stopMotors();
        release();
        task++;
      }
      break;

    default:
      stopMotors();
      break;
  }
}