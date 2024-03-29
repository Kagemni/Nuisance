#include <Arduino.h>
#include <NewPing.h>
#include <MPU6050_tockn.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Define state
#define WAIT_FOR_PHONE 0
#define RUN_AWAY 1
#define DOMINATED 2
#define buzzer 27
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int state = WAIT_FOR_PHONE;

// Define motor control pins
const int motorA_pwm = 5; // ENA pin on L298N
const int motorA1 = 7;    // IN1 pin on L298N
const int motorA2 = 6;    // IN2 pin on L298N
const int motorB_pwm = 3; // ENB pin on L298N
const int motorB1 = 4;    // IN3 pin on L298N
const int motorB2 = 2;    // IN4 pin on L298N

//OLED Display
int minutes = 2;
long timeStampStart;
int totalMinutes = 2;
int seconds = 0;
bool countDown = true;
bool phonePickedUp = false;
bool flag = true;
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//PIR pin
int pirPin = 26;

//force pin
const int forceSensorPin = A0;

//mic pin
const int micPin = 28;
int piezo = 27;

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
int timeRotate = 40000; //3.5 seconds?
int stuckTimer = 5000000; //5 seconds?

/*MPU6050 Gyro & Accelerometer
MPU6050 mpu6050(Wire);
float lastAccelX = 0.0;
float movementDistThreshold = 0.5;*/

void time(int timer);
int moveForward();
bool humanMovement();
bool humanMovementOnlyPIR();
void stopMotors();
bool checkObstacle();
unsigned long int  timedTurn(bool right);
int obstTurn (bool right);
void getRickRolledLol();
bool phoneDetected();
bool screamTrigger();
void blastOff();
void pickUpThePhone();
void areWeThereYet();
void updateDisplay(int minutes,int seconds);
void updateDisplayZero(int minutes,int seconds);
void wompWompNoises();
void tone(int pin,float freq);
void moveForwardForward();
void countDownf();



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
  pinMode(buzzer, OUTPUT);

  //OLED Display Setup
  // Initialize OLED display with I2C address 0x3C, SDA on A4, and SCL on A5
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display

  oled.setTextSize(2);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 10);        // position to display
  oled.println("TIME LEFT"); // text to display
  oled.display();               // show on OLED

  //Noise sensor and PIR sensor
  pinMode(micPin, INPUT);
  pinMode(pirPin, INPUT);
  bool flag = true;

  //Piezo buzzer setup
  pinMode(piezo, OUTPUT);

  // Initialize Serial Monitor
  state = WAIT_FOR_PHONE;
  Serial.begin(9600);
}

int lastTurn = 0;

void loop() {
  screamTrigger();
  if (state == WAIT_FOR_PHONE){

    if (phoneDetected()) state = RUN_AWAY;
  } else if(state == RUN_AWAY){
    
    moveForwardForward();
    wompWompNoises();
    // if(lastTurn == 0) lastTurn = timedTurn(false);
    // else if(millis() >= lastTurn+timeRotate){
    //   stopMotors();
    //   delay(1000);
    //   lastTurn = 0;
    // }
    countDownf();
    
    if (screamTrigger()) state = DOMINATED;
  } else if(state == DOMINATED){
    stopMotors();
  }
  Serial.println(state);
}

void moveForwardForward(){
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA_pwm, leftmotorspeed);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB_pwm, rightmotorspeed);
}

void countDownf(){
  seconds = seconds - 1;

  if (minutes <= 0 && seconds <=0) {
      updateDisplayZero(minutes,seconds);
      oled.clearDisplay();
      return;
    }

  if (seconds<= 0) {
      seconds = 59;
      minutes = minutes - 1;

      if (minutes == 0 && seconds == 1){
        seconds = 0;
      }
}

  if (seconds <10){
    updateDisplayZero(minutes,seconds);
  } else{
  updateDisplay(minutes, seconds);
  }
  delay (1000);
}

//returns 1 if program executed like normal aka obstacle avoidance
//returns 2 if PIR sensor is triggered
//returns 3 if force sensor loses phone
int moveForward() {
  bool humanMovement = humanMovementOnlyPIR();
  while (countDown == true) {
    while (!checkObstacle() && !humanMovement && phoneDetected()) {
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      analogWrite(motorA_pwm, leftmotorspeed);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
      analogWrite(motorB_pwm, rightmotorspeed);
      delay(2000);
      humanMovement = humanMovementOnlyPIR();
    }
    if (humanMovement) {
      return 2;
    }
    if (!phoneDetected()) {
      return 3;
    }
    if (checkObstacle()) {
      obstTurn(true);
      Serial.println(checkObstacle());
    }
    
    seconds = seconds - 2;

    if (minutes <= 0 && seconds <=0) {
      countDown = false;
      updateDisplayZero(minutes,seconds);
      oled.clearDisplay();
    }

    if (seconds<= 0) {
      seconds = 59;
      minutes = minutes - 1;

      if (minutes == 0 && seconds == 1){
        seconds = 0;
      }
    }

    if (seconds <10){
      updateDisplayZero(minutes,seconds);
    } 
    else{
      updateDisplay(minutes, seconds);
    }
    delay (1000);

    while (!checkObstacle() && !humanMovement && phoneDetected()) {
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      analogWrite(motorA_pwm, leftmotorspeed);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
      analogWrite(motorB_pwm, rightmotorspeed);
      delay(2000);
      humanMovement = humanMovementOnlyPIR();
    }
    if (humanMovement) {
      return 2;
    }
    if (!phoneDetected()) {
      return 3;
    }
    if (checkObstacle()) {
      obstTurn(true);
      Serial.println(checkObstacle());
    }
    return 1;
  }
  return 1;
}

int runaway () {
  getRickRolledLol();
  while (screamTrigger()) {
    int move = moveForward();
    while (move==1 || move==2) {
      move = moveForward();
    }
    if (move==3) {
      stopMotors();
      //wompWompNoises();
      Serial.println("Womp Womp Woooooooomp");
    }
  }

}

bool wondering() {
  int move = moveForward();
  while (move==1) {
    move = moveForward();
  }
  if (move==2) { //PIR
    runaway();
  }
  if (move==3) {
    //wompWompNoises();
    Serial.println("Womp Womp Woooooooomp");
    stopMotors();
  }
}

bool humanMovementOnlyPIR() {
  float pirState = digitalRead(pirPin);
  if (pirState == HIGH) {
    Serial.println("HUMAN GET AWAY FROM ME");
    while (flag == true) {
      getRickRolledLol();
    }
    delay(3000);
    return true;
  }
  Serial.println("lalalalalalala");
  delay(3000);
  /*if (val == LOW && pirState == HIGH) {
    Serial.println("You've stopped, human. Better it remain that way until your time is done.");
    pirState = LOW;
  }*/
  return false;
}

void stopMotors() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  analogWrite(motorA_pwm, 0);
  digitalWrite(motorB1, HIGH);
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
unsigned long int timedTurn(bool right) {
  if (right==false) {
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA_pwm, 0);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB_pwm, rightmotorspeed);
    Serial.println("left turn.");
  }
  else {
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(motorA_pwm, leftmotorspeed);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
    analogWrite(motorB_pwm, 0);
    Serial.println("right turn.");
  }
  return millis();
}

//returns 1 if program executed like normal aka obstacle avoidance
//returns 2 if PIR sensor is triggered
//returns 3 if force sensor loses phone
//right is 1, left is 0
int obstTurn (bool right) {
  bool humanChase = humanMovementOnlyPIR();
  Serial.println(humanChase);
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

  timedTurn(true);
  delay(timeRotate);
  stopMotors();
  delay(1500);
  humanChase = humanMovementOnlyPIR();
  Serial.println(humanChase);
  if (humanChase) {
    return 2;
  }
  else if (!phoneDetected()) {
    return 3;
  }
  else {
    return 1;
  }
}

bool phoneDetected(){
  int runAway;
  int analogReading = analogRead(forceSensorPin);

  Serial.print("Force sensor reading = ");
  // for data, remove when done
  Serial.print(analogReading); 
  


  if (analogReading >= 40) {
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
    Serial.println("quiet");
  }
}

void blastOff(){
  if (countDown == false && phonePickedUp == false) {
    //happyBirthdayGrandma  ();
  }
}
 
void pickUpThePhone(){
  while (phonePickedUp == true){
    minutes = 0;
    seconds = 0;
    //play wompWompAudio();
    Serial.println("Womp Womp Woooooooomp");
  } 
}

void areWeThereYet(){
    exit(0);
}

void updateDisplay(int minutes,int seconds) {
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.setCursor(10, 10);
  oled.print("TIME LEFT");
  oled.setCursor(40,40);
  oled.print(minutes);
  oled.print(":");
  oled.println(seconds);
  oled.display();
}

void updateDisplayZero(int minutes,int seconds) {
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.setCursor(10, 10);
  oled.print("TIME LEFT");
  oled.setCursor(40,40);
  oled.print(minutes);
  oled.print(":0");
  oled.println(seconds);
  oled.display();
}

//Audio Setup//*******************************************
void tone(int pin,float freq){
  for(int i=0;i<freq;i++)
    digitalWrite(pin, HIGH);
    delay(1000.00/freq);
    digitalWrite(pin, LOW);
    delay(1000.00/freq);
}

void wompWompNoises() {
  for (int freq = 1000; freq <= 9000; freq += 100) {
    tone(buzzer, freq);
  }
  digitalWrite(buzzer, LOW);
}