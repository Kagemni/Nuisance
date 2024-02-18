#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int minutes = 2;
long timeStampButtonPressed = 0;
int state=0;


// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);


  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display

  oled.setTextSize(2);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 10);        // position to display
  oled.println("TIME REMAINING"); // text to display
  oled.display();               // show on OLED
}

void loop() {
  int buttonState = digitalRead(2); // Change the pin number based on your button connection

  if (buttonState == HIGH && state == 0) {
    timeStampButtonPressed = millis();
    state = 1;
  }

  if (state == 1) {
    unsigned long elapsedTime = millis() - timeStampButtonPressed;
    int remainingSeconds = (minutes * 60 * 1000 - elapsedTime) / 1000;

    if (remainingSeconds >= 0) {
      updateDisplay(remainingSeconds);
    } else {
      state = 0; // Reset the state
      oled.clearDisplay();
      oled.display();
    }
}

void updateDisplay(int remainingSeconds) {
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.setCursor(0, 10);
  oled.print("TIME REMAINING: ");
  oled.print(remainingSeconds / 60); // Display minutes
  oled.print(":");
  oled.println(remainingSeconds % 60); // Display seconds
  oled.display();
