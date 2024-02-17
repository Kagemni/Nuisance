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
int maxObstDistance = 30;

// Define ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 8;

// Timer in microseconds
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool isTimerRunning = false;
int timeRotate = 3500000; //3.5 seconds?
int stuckTimer = 5000000; //5 seconds?

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

//timer is the amount of time you want the program to execute something
void time(int timer) {
  startTime = micros();
  while ( (micros() - startTime) < timer ) {}
}

void moveForward() {
  while (!checkObstacle()) {
    Serial.println(checkObstacle());
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA_pwm, rightmotorspeed);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB_pwm, leftmotorspeed);
  }
  obstTurn(true);
  Serial.println(checkObstacle());
}

void stopMotors() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  analogWrite(motorA_pwm, 0);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
  analogWrite(motorB_pwm, 0);
}

//Obstacle detected = true, Obstacle not detected = false.
bool checkObstacle() {
  long distance, duration;

  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the time taken for the ultrasonic pulse to return
  duration = pulseIn(echoPin, HIGH, 10000);
  //Calculate distance of obstacle if there.
  distance = (duration * 0.0343) / 2;
  delay(1000);

  if ( distance > maxObstDistance ) {
    Serial.print("Obstacle Detected at a distance of ")
    Serial.print(distance);
    Serial.println(" cm!!!!!!!!");
    return true;
  }
  Serial.print("None can stop me. Not even walls.");
  return false;
}

//right is 1, left is 0
//timed rotation
void timedTurn(bool right) {
  startTime = micros();
  Serial.println(startTime);
  Serial.println("Starting turn.");
  
  while ( (micros() - startTime) < timeRotate ) {
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
  Serial.println("Ended turn.");
  stopMotors();
  delay(1000);
}

//right is 1, left is 0
void obstTurn (bool right) {
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

  while (checkObstacle()) {}
  stopMotors();
  delay(1500);

  //To make it stop when it's stuck
  /*int s2 = micros();
  bool obst = true;
  while ( (micros() - s2) < stuckTimer && obst==true ) {
    if (!checkObstacle()) {
      stopMotors();
      obst = false;
      delay(1000);
      timedTurn(right, timeRotate);
    }
  }
  Serial.println("I'M STUCK");*/

}
