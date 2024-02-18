#define buzzerOne 14
#define buzzerTwo 15

bool canIScreamYet = false;

void setup()
{  
  // put your setup code here, to run once:
  pinMode(buzzerOne, OUTPUT);
  pinMode(buzzerTwo, OUTPUT);
}
 
void loop(){

  if (canIScreamYet = true) {
  int frequencyOne = random(1000, 9001);
  tone(buzzerOne, frequencyOne);
  delay(1000);  // BuzzerOne plays for 1 second
  
  int frequencyTwo = random(1000, 9001);
  tone(buzzerTwo, frequencyTwo);
  delay(1000);  // 1 second
  } else {
  // Turn off both buzzers
  noTone(buzzerOne);
  noTone(buzzerTwo);

  }
}
