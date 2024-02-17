
#define FORCE_SENSOR_PIN A0 
bool runAway = false;
void setup() {
  // can edit this value to anoter,
  Serial.begin(9600);
}

void loop() {
  int analogReading = analogRead(FORCE_SENSOR_PIN);

  Serial.print("Force sensor reading = ");
  // for data, remove when done
  Serial.print(analogReading); 
  


  if (analogReading >= 20) {
    runAway = true;

  } else {
    runAway = false
  }

  if (runAway) {
    // code to run away and motor logic
    

  }

}