#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int minutes = 2;
long timeStampStart;
int totalMinutes = 2;
int seconds = 0;
bool countDown = true;


// Use A4 for SDA and A5 for SCL
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);

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

void loop() {

while (countDown == true) {
seconds = seconds - 1;

  if (minutes <= 0 && seconds <=0) {
      countDown = false;
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

/*
if (countDown = false) {
  happyBirthdayGrandma  ();

}


*/

}
