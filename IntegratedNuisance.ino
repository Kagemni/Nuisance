#include <Arduino.h>
#include <NewPing.h>
#include <MPU6050_tockn.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Define state
#define WAIT_FOR_PHONE 0
#define RUN_AWAY 1
#define DOMINATED 
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
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//PIR pin
int pirPin = 26;

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

/*MPU6050 Gyro & Accelerometer
MPU6050 mpu6050(Wire);
float lastAccelX = 0.0;
float movementDistThreshold = 0.5;*/

//Rickroll Setup//*******************************************
/*  RickRollCode

    AUTHOR: Rowan Packard
    rowanpackard@gmail.com

    DISCLAIMER: The song "Never Gonna Give You Up" by Rick Astley
    is not  the creative property of the author. This code simply
    plays a Piezo buzzer  rendition of the song.
*/

#define  a3f    208     // 208 Hz
#define  b3f    233     // 233 Hz
#define  b3     247     // 247 Hz
#define  c4     261     // 261 Hz MIDDLE C
#define  c4s    277     // 277 Hz
#define  e4f    311     // 311 Hz    
#define  f4     349     // 349 Hz 
#define  a4f    415     // 415 Hz  
#define  b4f    466     // 466 Hz 
#define  b4     493     //  493 Hz 
#define  c5     523     // 523 Hz 
#define  c5s    554     // 554  Hz
#define  e5f    622     // 622 Hz  
#define  f5     698     // 698 Hz 
#define  f5s    740     // 740 Hz
#define  a5f    831     // 831 Hz 

#define  rest    -1

int piezo = 7; // Connect your piezo buzzer to this pin or change  it to match your circuit!

volatile int beatlength  = 100; // determines tempo
float beatseparationconstant = 0.3;

int threshold;

int  a; // part index
int b; // song index
int c; // lyric index

boolean  flag;

// Parts 1 and 2 (Intro)

int song1_intro_melody[] =
{c5s,  e5f, e5f, f5, a5f, f5s, f5, e5f, c5s, e5f, rest, a4f, a4f};

int song1_intro_rhythmn[]  =
{6, 10, 6, 6, 1, 1, 1, 1, 6, 10, 4, 2, 10};

// Parts 3 or 5 (Verse 1)

int  song1_verse1_melody[] =
{ rest, c4s, c4s, c4s, c4s, e4f, rest, c4, b3f, a3f,
  rest, b3f, b3f, c4, c4s, a3f, a4f, a4f, e4f,
  rest, b3f, b3f, c4, c4s, b3f,  c4s, e4f, rest, c4, b3f, b3f, a3f,
  rest, b3f, b3f, c4, c4s, a3f, a3f, e4f,  e4f, e4f, f4, e4f,
  c4s, e4f, f4, c4s, e4f, e4f, e4f, f4, e4f, a3f,
  rest,  b3f, c4, c4s, a3f, rest, e4f, f4, e4f
};

int song1_verse1_rhythmn[] =
{  2, 1, 1, 1, 1, 2, 1, 1, 1, 5,
  1, 1, 1, 1, 3, 1, 2, 1, 5,
  1, 1, 1, 1, 1,  1, 1, 2, 1, 1, 1, 1, 3,
  1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4,
  5, 1, 1, 1,  1, 1, 1, 1, 2, 2,
  2, 1, 1, 1, 3, 1, 1, 1, 3
};


//  Parts 4 or 6 (Chorus)

int song1_chorus_melody[] =
{ b4f, b4f, a4f, a4f,
  f5, f5, e5f, b4f, b4f, a4f, a4f, e5f, e5f, c5s, c5, b4f,
  c5s, c5s, c5s, c5s,
  c5s, e5f, c5, b4f, a4f, a4f, a4f, e5f, c5s,
  b4f, b4f, a4f, a4f,
  f5,  f5, e5f, b4f, b4f, a4f, a4f, a5f, c5, c5s, c5, b4f,
  c5s, c5s, c5s, c5s,
  c5s, e5f, c5, b4f, a4f, rest, a4f, e5f, c5s, rest
};

int song1_chorus_rhythmn[]  =
{ 1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
  1, 1, 1, 1,
  3, 3, 3, 1, 2, 2, 2, 4, 8,
  1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3,  1, 2,
  1, 1, 1, 1,
  3, 3, 3, 1, 2, 2, 2, 4, 8, 4
};
//***********************************************************

void time(int timer);
void moveForward();
bool humanMovement();
void obstTurn (bool right);
void stopMotors();
bool checkObstacle();
void timedTurn(bool right);
void obstTurn (bool right);
void rickrollPlay();
bool phoneDetected();
bool screamTrigger();
void blastOff();
void pickUpThePhone();
void areWeThereYet();
void updateDisplay(int minutes,int seconds);
void updateDisplayZero(int minutes,int seconds);

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

  //Piezo buzzer setup
  pinMode(piezo, OUTPUT);
  flag = true;
  a = 4;
  b = 0;
  c = 0;

  // Initialize Serial Monitor
  state = WAIT_FOR_PHONE;
  Serial.begin(9600);
}

void loop() {
  /*screamTrigger();
  if (state == WAIT_FOR_PHONE){

    if (phoneDetected()) state = RUN_AWAY;
  } else if(state == RUN_AWAY){

    if (screamTrigger()) state = DOMINATED;
  } else if(state == DOMINATED){
    stopMotors();
  }*/
  humanMovementOnlyPIR();
  //Serial.println(state);
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
/*bool humanMovement() {
  //Read PIR and accelerometer's values
  int pirValue = digitalRead(pirPin);
  mpu6050.update();
  float accelX = mpu6050.getAccX();
  }

  //Checks if the PIR sensor is triggered and if so, was it more than robot movement?
  if (pirValue==HIGH && abs(accelX - lastAccelX) < movementDistThreshold) {
    lastAccelX = accelX;
    //Enter screaming and buzzer stuff
    return true;
  }
  return false;
}
*/

bool humanMovementOnlyPIR() {
  float pirState = digitalRead(pirPin);
  if (pirState == HIGH) {
    Serial.println("HUMAN GET AWAY FROM ME");
    while (flag == true) {
      rickrollPlay();
    }
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
int obstTurn (bool right) {
  bool humanChase = humanMovementOnlyPIR;
  Serial.println(humanChase);
  if (!humanChase) {
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
  }

  if (humanChase) {
    return 2;
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

void rickrollPlay() {
  int notelength;
  if (a == 1 || a == 2) {
    // intro
    notelength  = beatlength * song1_intro_rhythmn[b];
    if (song1_intro_melody[b] > 0) {
      tone(piezo, song1_intro_melody[b], notelength);
    }
    b++;
    if (b >= sizeof(song1_intro_melody) / sizeof(int)) {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 3  || a == 5) {
    // verse
    notelength = beatlength * 2 * song1_verse1_rhythmn[b];
    if (song1_verse1_melody[b] > 0) {
      Serial.print(lyrics_verse1[c]);
      tone(piezo, song1_verse1_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_verse1_melody) / sizeof(int)) {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 4 || a == 6) {
    // chorus
    notelength = beatlength * song1_chorus_rhythmn[b];
    if  (song1_chorus_melody[b] > 0) {
      Serial.print(lyrics_chorus[c]);
      tone(piezo, song1_chorus_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_chorus_melody) / sizeof(int)) {
      Serial.println("");
      a++;
      b = 0;
      c = 0;
    }
  }
  delay(notelength);
  noTone(piezo);
  delay(notelength * beatseparationconstant);
  if (a == 7) { // loop back around to beginning of song
    a = 1;
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