#define buzzer 8

// bool canIScreamYet = true;

void setup()
{  
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);
}

void getRickRolledLol(){
  // insert rickroll stuff here
}

void happyBirthdayGrandma() {
  // Happy birthday here
  int length = 28; // the number of notes
  char notes[] = "GGAGcB GGAGdc GGxecBA yyecdc";
  int beats[] = {2, 2, 8, 8, 8, 16, 1, 2, 2, 8, 8, 8, 16, 1, 2, 2, 8, 8, 8, 8, 16, 1, 2, 2, 8, 8, 8, 16};
  int tempo = 200; // time delay between notes

  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // delay between notes
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    // time delay between notes
    delay(tempo);
  }
}
void playNote(char note, int duration) {
char names[] = {'C', 'D', 'E', 'F', 'G',   'A', 'B',           

                 'c', 'd', 'e', 'f', 'g', 'a', 'b',

                  'x', 'y' };

int tones[] = { 1915, 1700, 1519, 1432, 1275,   1136, 1014,

                 956,  834,  765,  593,  468,  346,  224,

                  655 , 715 };

int SPEE = 5;

// play the tone corresponding   to the note name

for (int i = 0; i < 17; i++) {

   if (names[i] ==   note) {
    int newduration = duration/SPEE;
     playTone(tones[i], newduration);
    }
}
}

void wompWompNoises(){
  for (int freq = 1000; freq <= 9000; freq += 100){
    tone(buzzer,freq);
    delay(100);
  }
  noTone(buzzer);
}
 
void loop(){

  // Turn off both buzzers
  noTone(buzzer);

}
