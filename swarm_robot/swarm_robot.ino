#include <AccelStepper.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// LEFT Stepmotor
#define M1_PIN1 5
#define M1_PIN2 7
#define M1_PIN3 6
#define M1_PIN4 8
// RIGHT Stepmotor
#define M2_PIN1 10
#define M2_PIN2 A2
#define M2_PIN3 11
#define M2_PIN4 A3
// Infrared Sensor
#define IR_PIN A1
// Light Sensor
#define LIGHT_PIN A0
// BLUE LED
#define BLUE_PIN 2
// RED LED
#define RED_PIN 3
// Buzzer
#define BUZZER_PIN 13
// Microphone
#define MIC_PIN 4
// Display
#define LCD_A À4
#define LCD_B A5
#define LCD_ADDRESS 0x3C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int xx = 30;
const int yy = -5;

const unsigned char PROGMEM runner [] = {0x00, 0x00, 0xE0, 0x00, 0x01, 0x90, 0x00, 0x01, 0x10, 0x00, 0x21, 0x10, 0x00, 0xFF, 0xE0, 0x01, 0x81, 0x80, 0x02, 0xF8, 0xC0, 0x07, 0x98, 0xC0, 0x02, 0x11, 0xE0, 0x00, 0x31, 0xB0, 0x00, 0x63, 0x9E, 0x00, 0x46, 0xFE, 0x00, 0x44, 0x20, 0x00, 0xE6, 0x00, 0x01, 0xB1, 0x80, 0x3F, 0x3C, 0xC0, 0x60, 0x66, 0xC0, 0x7C, 0xC6, 0x80, 0x0B, 0x84, 0x80, 0x00, 0x05, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x0C, 0x00};
const unsigned char PROGMEM happy [] = {0x00, 0x7E, 0x00, 0x03, 0x81, 0xC0, 0x04, 0x00, 0x60, 0x18, 0x00, 0x18, 0x10, 0x00, 0x08, 0x23, 0x81, 0xC4, 0x44, 0x42, 0x22, 0x44, 0x42, 0x22, 0x40, 0x00, 0x03, 0x80, 0x00, 0x01, 0x80, 0x00, 0x01, 0x80, 0x00, 0x01, 0x9F, 0xFF, 0xF9, 0x90, 0x00, 0x09, 0x90, 0x00, 0x09, 0xC8, 0x00, 0x12, 0x4C, 0x00, 0x32, 0x64, 0x00, 0x26, 0x22, 0x00, 0xC4, 0x11, 0x81, 0x88, 0x18, 0x7E, 0x10, 0x06, 0x00, 0x20, 0x03, 0x81, 0xC0, 0x00, 0xFE, 0x00};
const unsigned char PROGMEM sad [] = {0x00, 0x1C, 0x00, 0x01, 0xF7, 0x80, 0x07, 0x00, 0xE0, 0x0C, 0x00, 0x30, 0x18, 0x00, 0x18, 0x30, 0x00, 0x0C, 0x20, 0x00, 0x04, 0x60, 0x00, 0x06, 0x40, 0x00, 0x02, 0x41, 0x81, 0x82, 0x41, 0xC3, 0x82, 0x81, 0x81, 0x83, 0xC0, 0x00, 0x01, 0xC0, 0x00, 0x03, 0x40, 0x3C, 0x02, 0x40, 0xEF, 0x02, 0x61, 0x81, 0x86, 0x20, 0x00, 0x04, 0x30, 0x00, 0x0C, 0x18, 0x00, 0x18, 0x0C, 0x00, 0x30, 0x07, 0x00, 0xE0, 0x01, 0xEF, 0x80, 0x00, 0x38, 0x00};

// Notes needed for star wars songs
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;


enum emotion {
  HAPPY,
  SAD,
  RUNNING
};

AccelStepper stepper1(AccelStepper::FULL4WIRE, M1_PIN1, M1_PIN2, M1_PIN3, M1_PIN4);
AccelStepper stepper2(AccelStepper::FULL4WIRE, M2_PIN1, M2_PIN2, M2_PIN3, M2_PIN4);

const int turnCount = 0;


void setup() {
  Serial.begin(9600);
  Serial.println("Started with setup");
  // Pins setup
  pinMode(MIC_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(IR_PIN, INPUT);
  digitalWrite(RED_PIN, HIGH);

  // Stepper motor setup
  stepper1.setMaxSpeed(200);
  stepper2.setMaxSpeed(200);

  stepper1.setAcceleration(100);
  stepper2.setAcceleration(100);

  // display setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  display.clearDisplay();
}

void loop() {
  // put your main code here, to run repeated:
  if (stepper1.isRunning() || stepper2.isRunning()) {
    stepper1.run();
    stepper2.run();
  } else {
    digitalWrite(BLUE_PIN, HIGH);

    boolean heard = false;
    boolean seen  = false;

    int lightValue = analogRead(LIGHT_PIN);
    boolean micVal = digitalRead(MIC_PIN);
    int irStatus = analogRead(IR_PIN);

    Serial.print("Reading IR Sensor: "); Serial.println(irStatus);

    if ( irStatus >= 500) { // If it detects something it should turn around
      Serial.println("Detected Border moving out of the way");
      if (turncount < 2) {
        stepper1.move(400);
        stepper2.move(-400);
        turncount++;
      } else {
        stepper1.move(400);
        stepper2.move(400);
        turncount = 0;
      }
    } else {
      int closerPitch = map(lightValue, 400, 1000, 120, 1500); // The higher the light sensitivity the higher the pitch
      Serial.print("Reading Microphone: "); Serial.print(micVal);
      if (micVal == LOW) { // If he hears something it means a Robot is close and should start looking around
        //digitalWrite(BLUE_PIN, HIGH);
        Serial.println(" -- Johnny Heard Something");
        heard = true;
      }
      else {
        Serial.println(" -- Nothing Detected");
        digitalWrite(BLUE_PIN, LOW);
        changeDisplay(SAD);
      }
      Serial.print("Reading Light Value: "); Serial.print(lightValue);
      if (lightValue < 400) {
        Serial.println(" -- Don't see shit");
        tone(BUZZER_PIN, closerPitch , 10);
        delay(10);
      } else if (lightValue < 700) {
        seen = true;
        Serial.println(" -- Getting closer");
        tone(BUZZER_PIN, closerPitch, 10);
        delay(10);
      } else {
        Serial.println(" -- Robot found");
        changeDisplay(HAPPY);
        delay(100);
        // Write something on LCD
      }
      tone(BUZZER_PIN, 1400, 100);
      delay(10);
      if (heard) {
        Serial.println("I have heard");
        if (seen) {
          Serial.println("I have seen");// If I have seen him and I hear him it means he's close and he is somewhere in front of me
          stepper1.move(300);
          stepper2.move(300);
          changeDisplay(RUNNING);
        } else {
          Serial.println("but not seen");// If I haven't seen him but heard him it means someone is around.
          stepper1.move(300);
          stepper2.move(-300);
          changeDisplay(RUNNING);
        }
      } else {
        Serial.println("I haven't heard anything");// If I didn't hear anything it probably means nobody is around.
        if (seen) {
          stepper1.move(300);
          stepper2.move(300);
          changeDisplay(RUNNING);

        } else {
          stepper1.move(300);
          stepper2.move(-300);
          changeDisplay(RUNNING);

        }
      }
    }
    digitalWrite(BLUE_PIN, LOW);
  }
}


//Code used from https://create.arduino.cc/projecthub/HiHiHiHiiHiiIiH/star-wars-on-a-buzzer-0814f2

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(BUZZER_PIN, note, duration);
  delay(duration);
  //Stop tone on buzzerPin
  noTone(BUZZER_PIN);

  delay(50);
}

void firstSection()
{
  beep(a, 500);
  beep(a, 500);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);

  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);
}

void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);
  beep(fSH, 250);

  delay(325);

  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);
  beep(cSH, 175);
  beep(cH, 125);
  beep(b, 125);
  beep(cH, 250);

  delay(350);
}

void changeDisplay(emotion emotion) {
  switch (emotion) {
    case HAPPY:
      display.clearDisplay();
      display.drawBitmap(
        (display.width()  - 24) / 2,
        (display.height() - 24) / 2, happy, 24, 24, 1);
      display.display();
      break;
    case SAD:
      display.clearDisplay();
      display.drawBitmap(
        (display.width()  - 24) / 2,
        (display.height() - 24) / 2,
        sad, 24, 24, 1);
      display.display();
      break;
    case RUNNING:
      display.clearDisplay();
      display.drawBitmap(
        (display.width()  - 24 ) / 2,
        (display.height() - 24) / 2, runner, 24, 24, 1);
      display.display();
      break;
    default:
      Serial.println("Something went wrong");
      break;
  }
  delay(1);
}
