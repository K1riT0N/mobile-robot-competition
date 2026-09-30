#include <Servo.h>

#define MOTOR_B_EN 5
#define MOTOR_B_1 7
#define MOTOR_B_2 6

#define MOTOR_A_EN 10
#define MOTOR_A_1 9
#define MOTOR_A_2 8

Servo myservo1;
Servo myservo2;

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_A_EN, OUTPUT);

  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);
  pinMode(MOTOR_B_EN, OUTPUT);

  Serial.begin(9600);
  myservo1.attach(2);
  myservo2.attach(3);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  // int val;

  // while (Serial.available() > 0) {
  //   val = Serial.parseInt();
  //   if (val != 0) {
  //     Serial.println(val);
  //     Serial.println(180-val);
  //     myservo1.write(val);
  //     myservo2.write(180-val);
  //   }
  //   delay(5);
  // }

  digitalWrite(MOTOR_A_1, HIGH);
  digitalWrite(MOTOR_A_2, LOW);
  digitalWrite(MOTOR_A_EN, 255);

  digitalWrite(MOTOR_B_1, HIGH);
  digitalWrite(MOTOR_B_2, LOW);
  digitalWrite(MOTOR_B_EN, 255);

  delay(500);

  digitalWrite(MOTOR_A_1, LOW);
  digitalWrite(MOTOR_A_2, LOW);

  digitalWrite(MOTOR_B_1, LOW);
  digitalWrite(MOTOR_B_2, LOW);

  delay(500);

  digitalWrite(MOTOR_A_1, LOW);
  digitalWrite(MOTOR_A_2, HIGH);
  digitalWrite(MOTOR_A_EN, 255);

  digitalWrite(MOTOR_B_1, HIGH);
  digitalWrite(MOTOR_B_2, LOW);
  digitalWrite(MOTOR_B_EN, 255);

  delay(300);

  digitalWrite(MOTOR_A_1, LOW);
  digitalWrite(MOTOR_A_2, LOW);

  digitalWrite(MOTOR_B_1, LOW);
  digitalWrite(MOTOR_B_2, LOW);

  delay(500);

}
