#include <Arduino.h>
#include <NewPing.h>
#include <MPU6050_tockn.h>

//Define state
#define WAIT_FOR_PHONE 0
#define RUN_AWAY 1
#define DOMINATED 2

int state = WAIT_FOR_PHONE;

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

//force pin
const int forceSensorPin = A0;

//mic pin
const int micPin = 28;
//int speaker = ;

int triggerDistance = 20;
int rightmotorspeed = 150;
int leftmotorspeed = 200;
int highRightMotorspeed = 200;
int highLeftMotorspeed = 200;
int maxObstDistance = 30;

// Define ultrasonic sensor pins DONE!!!!
const int trigPin1 = 12;
const int echoPin1 = 11;
const int trigPin2 = 10;
const int echoPin2 = 9; 

// Timer in microseconds
unsigned long startTime = 0;
bool isTimerRunning = false;
int timeRotate = 400000; //3.5 seconds?
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
bool phoneDetected();
bool screamTrigger();

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

  pinMode(micPin, INPUT);

  pinMode(pirPin, INPUT);
  //pinMode(speaker, OUTPUT);

  /*MPU6050 Gyro & Accelerometer
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);*/

  // Initialize Serial Monitor
  state = WAIT_FOR_PHONE;
  Serial.begin(9600);
}

void loop() {
  screamTrigger();
  if (state == WAIT_FOR_PHONE){

    if (phoneDetected()) state = RUN_AWAY;
  } else if(state == RUN_AWAY){

    if (screamTrigger()) state = DOMINATED;
  } else if(state == DOMINATED){
    stopMotors();
  }
  Serial.println(state);
}

//timer is the amount of time you want the program to execute something
void time(int timer) {
  unsigned long int startTime = micros();
  while ( (micros() - startTime) < timer ) {}
}

void moveForward() {
  //while (!checkObstacle()) {
    Serial.println(checkObstacle());
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA_pwm, leftmotorspeed);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB_pwm, rightmotorspeed);
  //}
  //obstTurn(true);
  //Serial.println(checkObstacle());
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

bool humanMovementOnlyPIR() {
  float val = digitalRead(pirPin);
  if ((val == HIGH) && (pirState == LOW)) {
    Serial.println("HUMAN GET AWAY FROM ME");
    pirState = HIGH;
    return true;
  }
  /*if (val == LOW && pirState == HIGH) {
    Serial.println("You've stopped, human. Better it remain that way until your time is done.");
    pirState = LOW;
  }*/
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
  float duration1, duration2;
  float distance1, distance2;

  unsigned long timeout_cutoff = 10000;

  // Trigger ultrasonic sensor 1
  // digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH, timeout_cutoff); // Adjust timeout if needed


  // Trigger ultrasonic sensor 2 after a delay
  delay(20); // Adjust this delay as needed based on your setup

  // digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH, timeout_cutoff); // Adjust timeout if needed

  /*Print durations
  Serial.print("Duration 1: ");
  Serial.println(duration1);
  Serial.print("Duration 2: ");
  Serial.println(duration2);*/

  // Calculate distances
  if (duration1==0 && duration2!=0) {
    distance1 = 400;
  }
  else if (duration2==0 && duration1!=0) 
  {
    distance2 = 400;
  }
  else if (duration1==0 && duration2==0) {
    distance1 = 400;
    distance2 = 400;
  }
  else {
    distance1 = (duration1 * 0.0343) / 2.0000;
    distance2 = ((double)duration2 * 0.0343) / 2.00000;
  }

  /* Print distances for debugging
  Serial.print("Distance 1: ");
  Serial.println(distance1);
  Serial.print("Distance 2: ");
  Serial.println(distance2);*/

  // Check for obstacles
  if (distance1 <= maxObstDistance || distance2 <= maxObstDistance) {
    Serial.println("Obstacle Detected!");
    return true;
  } else {
    Serial.println("No Obstacle Detected.");
    return false;
  }
}

//right is 1, left is 0
//timed rotation
void timedTurn(bool right) {
  
    if (right==false) {
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      analogWrite(motorA_pwm, 0);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
      analogWrite(motorB_pwm, rightmotorspeed);
      Serial.println("left turn.");
      delay(5000);

    }
    else {
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      analogWrite(motorA_pwm, leftmotorspeed);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
      analogWrite(motorB_pwm, 0);
      Serial.println("right turn.");
      delay(5000);
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
    analogWrite(motorA_pwm, leftmotorspeed);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB_pwm, 0);
    Serial.println("right turn.");
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
  delay(2000);

  timedTurn(true);
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

bool phoneDetected(){
  int runAway;
  int analogReading = analogRead(forceSensorPin);

  // Serial.print("Force sensor reading = ");
  // // for data, remove when done
  // Serial.print(analogReading); 
  


  if (analogReading >= 20) {
    runAway = true;

  } else {
    runAway = false;
  }

  return runAway;
}

bool screamTrigger(){
  if (digitalRead(micPin) == LOW){
    return true;
    Serial.println("loud");
  }else{
    return false;
    Serial.println("quite");
  }
}