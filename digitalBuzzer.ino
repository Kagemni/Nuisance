#define buzzer 8

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(buzzer, HIGH);
    delayMicroseconds(tone);
    digitalWrite(buzzer, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B',
                  'c', 'd', 'e', 'f', 'g', 'a', 'b',
                  'x', 'y' };

  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014,
                   956, 834, 765, 593, 468, 346, 224,
                   655 , 715 };

  int SPEE = 5;

  // play the tone corresponding to the note name
  for (int i = 0; i < 17; i++) {
    if (names[i] == note) {
      int newduration = duration / SPEE;
      playTone(tones[i], newduration);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);

}

void getRickRolledLol() {
  int a3f = 208;     // 208 Hz
  int b3f = 233;     // 233 Hz
  int b3 = 247;      // 247 Hz
  int c4 = 261;      // 261 Hz MIDDLE C
  int c4s = 277;     // 277 Hz
  int e4f = 311;     // 311 Hz    
  int f4 = 349;      // 349 Hz 
  int a4f = 415;     // 415 Hz  
  int b4f = 466;     // 466 Hz 
  int b4 = 493;      //  493 Hz 
  int c5 = 523;      // 523 Hz 
  int c5s = 554;     // 554  Hz
  int e5f = 622;     // 622 Hz  
  int f5 = 698;      // 698 Hz 
  int f5s = 740;     // 740 Hz
  int a5f = 831;     // 831 Hz 

  int rest = -1;

  volatile int beatlength = 100; // determines tempo
  float beatseparationconstant = 0.3;

  int threshold;

  int a; // part index
  int b; // song index
  int c; // lyric index

  boolean flag;

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

void wompWompNoises() {
  for (int freq = 1000; freq <= 9000; freq += 100) {
    tone(buzzer, freq);
    delay(100);
  }
  noTone(buzzer);
}

void loop() {
  // Turn off both buzzers
  noTone(buzzer);
}
