#include <Arduino.h>
#include <NewPing.h>
#include <MPU6050_tockn.h>

// Define motor control pins
const int motorA_pwm = 5; // ENA pin on L298N
const int motorA1 = 7;    // IN1 pin on L298N
const int motorA2 = 6;    // IN2 pin on L298N
const int motorB_pwm = 3; // ENB pin on L298N
const int motorB1 = 4;    // IN3 pin on L298N
const int motorB2 = 2;    // IN4 pin on L298N

int triggerDistance = 20;
int leftmotorspeed = 200;
int rightmotorspeed = 200;

// Define ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 8;

// Timer in microseconds
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool isTimerRunning = false;

/*MPU6050 Gyro & Accelerometer
MPU6050 mpu6050(Wire);
float elapsedTime = 0.01;*/

void setup() {
  // Motor control pins as OUTPUT
  pinMode(motorA_pwm, OUTPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB_pwm, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  /*MPU6050 Gyro & Accelerometer
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);*/

  // Initialize Serial Monitor
  Serial.begin(9600);
}

void loop() {
  timedTurn(true, 5000000);
}

void moveForward() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  analogWrite(motorA_pwm, rightmotorspeed);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
  analogWrite(motorB_pwm, leftmotorspeed);
}

//right is 1, left is 0
//Continuous rotation
void contTurn(bool right) {

    if (right==true) {
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      analogWrite(motorA_pwm, 0);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
      analogWrite(motorB_pwm, rightmotorspeed);
    }

    else {
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      analogWrite(motorA_pwm, leftmotorspeed);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
      analogWrite(motorB_pwm, 0);
    }
}

//timer is the amount of time you want the program to execute something
void time(int timer) {
  startTime = micros();
  while ( (micros() - startTime) < timer ) {}
}

//right is 1, left is 0
//timed rotation
void timedTurn(bool right, int timer) {
  
  startTime = micros();
  while ( (micros() - startTime) < timer ) {

    if (right==true) {
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      analogWrite(motorA_pwm, 0);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
      analogWrite(motorB_pwm, rightmotorspeed);
    }

    else {
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      analogWrite(motorA_pwm, leftmotorspeed);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
      analogWrite(motorB_pwm, 0);
    }
    
    analogWrite(motorA_pwm, 0);
    analogWrite(motorB_pwm, 0);
}

void stopMotors() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  analogWrite(motorA_pwm, 0);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
  analogWrite(motorB_pwm, 0);
}

int checkDistance() {
  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the time taken for the ultrasonic pulse to return
  long duration = pulseIn(echoPin, HIGH, 10000);  // Adding a timeout

  // Print raw duration to Serial Monitor
  Serial.print("Raw Duration: ");
  Serial.println(duration);
}

/*bool checkDirection(int angleOverCompensated) {
  turn(right)
}*/