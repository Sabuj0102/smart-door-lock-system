#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// === Servo setup ===
Servo myservo;

// === LCD setup ===
LiquidCrystal_I2C lcd(0x27, 16, 2);   // I2C address 0x27, 16x2 display

// === Keypad setup ===
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// === Password setup ===
String password = "2021";   // Correct password
String input = "";

// === Optional: buzzer + LED ===
const int buzzerPin = 11;
const int ledPin = 12;

void setup() {
  // Servo
  myservo.attach(10);
  myservo.write(0); // start locked

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" Smart Door Lock ");
  delay(2000);
  lcd.clear();

  // Optional: buzzer + LED
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == '#') {  // Submit entered code
      if (input == password) {
        lcd.clear();
        lcd.print("Access Granted");
        myservo.write(90);  // unlock
        digitalWrite(ledPin, HIGH);
        tone(buzzerPin, 1000, 200);  // short beep

        delay(5000);            // keep door unlocked for 5 sec
        myservo.write(0);       // lock door again
        digitalWrite(ledPin, LOW);
        lcd.clear();
      } else {
        lcd.clear();
        lcd.print("Access Denied");
        myservo.write(0);  // lock
        digitalWrite(ledPin, LOW);
        tone(buzzerPin, 400, 500);   // longer beep
        delay(2000);
        lcd.clear();
      }
      input = "";  // reset input
    } 
    else if (key == '*') {  // Reset input
      input = "";
      lcd.clear();
      lcd.print("Input Cleared");
      delay(1000);
      lcd.clear();
    } 
    else {  // Add digit to input
      input += key;
      lcd.setCursor(0,1);
      lcd.print(input);
    }
  }
}
