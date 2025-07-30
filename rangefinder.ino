#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NewPing.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TRIG 9
#define ECHO 10
#define BUTTON_PIN 7

NewPing sonar(TRIG, ECHO, 400); // max distance is 400cm

float dist_3[3] = {0.0, 0.0, 0.0};
float dist, dist_filtered;
float k;
byte i = 0;
bool freeze = false;
bool lastButtonState = HIGH;
bool freezeButtonState = HIGH;   //for a stable state of the button after debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long lastMeasure = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED not found"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Starting..");
  display.display();
  delay(1000);
}

void loop() {
  bool reading = digitalRead(BUTTON_PIN); //// BUTTON: anti-bounce and freeze switching

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != freezeButtonState) {
      freezeButtonState = reading;

      if (freezeButtonState == LOW) {  // button is pressed
        freeze = !freeze;
      }
    }
  }
  lastButtonState = reading;

  // distance measurements
  if (!freeze && millis() - lastMeasure > 300) {
    dist_3[i] = sonar.ping_cm();
    i = (i + 1) % 3;
    dist = middle_of_3(dist_3[0], dist_3[1], dist_3[2]);

    float delta = abs(dist - dist_filtered);
    k = (delta > 1.0) ? 0.7 : 0.1;
    dist_filtered = dist * k + dist_filtered * (1 - k);

    lastMeasure = millis();
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);

  if (dist_filtered < 2.0 && !freeze) {
    display.print("<2cm");
  } else {
    display.print(dist_filtered, 2);
    display.print("cm");
  }

  float inches = dist_filtered / 2.54;
  int feet = inches / 12;
  int remaining_inches = (int)(inches + 0.5) % 12;

  display.setTextSize(2);
  display.setCursor(0, 24);
  display.print(feet);
  display.print("ft ");
  display.print(remaining_inches);
  display.print("in");

  if (freeze) {
    display.setTextSize(2);
    display.setCursor(0, 54);
    display.print("HOLD");
  }

  display.display();
}

float middle_of_3(float a, float b, float c) { // median filter
  if ((a <= b) && (a <= c)) return (b <= c) ? b : c;
  else if ((b <= a) && (b <= c)) return (a <= c) ? a : c;
  else return (a <= b) ? a : b;
}
