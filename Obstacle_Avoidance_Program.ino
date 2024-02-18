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

//PIR pin
int pirPin = 2;
int pirState = LOW;

int triggerDistance = 20;
int rightmotorspeed = 200;
int leftmotorspeed = 200;
int highRightMotorspeed = 400;
int highLeftMotorspeed = 400;
int maxObstDistance = 30;

// Define ultrasonic sensor pins DONE!!!!
const int trigPin1 = 12;
const int echoPin1 = 11;
const int trigPin2 = 10;
const int echoPin2 = 9;

// Timer in microseconds
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool isTimerRunning = false;
int timeRotate = 4000000; //3.5 seconds?
int stuckTimer = 5000000; //5 seconds?

//MPU6050 Gyro & Accelerometer
MPU6050 mpu6050(Wire);
float lastAccelX = 0.0;
float movementDistThreshold = 0.5;

void time(int timer);
void moveForward();
bool humanMovement();
void obstTurn (bool right);
void stopMotors();
bool checkObstacle();
void timedTurn(bool right);
void obstTurn (bool right);

void setup() {
  // Motor control pins as OUTPUT
  pinMode(motorA_pwm, OUTPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB_pwm, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  // Ultrasonic sensor pins
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  /*MPU6050 Gyro & Accelerometer
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);*/

  // Initialize Serial Monitor
  Serial.begin(9600);
}

void loop() {
  timedTurn(true);
  delay(2000);
  bool obstacle_detected = checkObstacle();
  delay(1000);
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

//returns true if human movement detected, and false if not
bool humanMovement() {
  //Read PIR and accelerometer's values
  int pirValue = digitalRead(pirPin);
  mpu6050.update();
  float accelX = mpu6050.getAccX();

  //Checks if the PIR sensor is triggered and if so, was it more than robot movement?
  if (pirValue==HIGH && abs(accelX - lastAccelX) < movementDistThreshold) {
    lastAccelX = accelX;
    /*
    Enter screaming and buzzer stuff
    */
    return true;
  }
  return false;
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
  long distance1, distance2, duration1, duration2;

  // Trigger ultrasonic sensor
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);

  // Read the time taken for the ultrasonic pulse to return
  duration1 = pulseIn(echoPin1, HIGH, 10000);
  duration2 = pulseIn(echoPin2, HIGH, 10000);
  //Calculate distance of obstacle if there.
  distance1 = (duration1 * 0.343) / 2;
  distance2 = (duration2 * 0.343) / 2;
  delay(1000);

  if ( distance1 >= maxObstDistance or distance2 >= maxObstDistance) {
    Serial.print("Obstacle Detected at a distance of ");
    Serial.println("Dist1 = ");
    Serial.println(distance1);
    Serial.print(" cm!!!!!!!!");
    Serial.println("Dist2 = ");
    Serial.println(distance2);
    Serial.print(" cm!!!!!!!!");
    return true;
  }
  Serial.println("None can stop me. Not even walls.");
  return false;
}

//right is 1, left is 0
//timed rotation
void timedTurn(bool right) {
  unsigned long int startTime = micros();
  Serial.println("Starting turn.");
  Serial.println(startTime);
  
  unsigned long int micro = micros();
  while ( micro <= (micro+timeRotate) ) {
    if (right==true) {
      Serial.println(micro);
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      analogWrite(motorA_pwm, 0);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
      analogWrite(motorB_pwm, rightmotorspeed);
      Serial.println("right turn.");
      micro = micros();
      Serial.println(micro);
    }
    else {
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      analogWrite(motorA_pwm, leftmotorspeed);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
      analogWrite(motorB_pwm, 0);
      micro = micros();
      Serial.println("left turn.");
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