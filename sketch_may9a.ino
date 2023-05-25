#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Wire.h>
#include <virtuabotixRTC.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(11, DHT11);
virtuabotixRTC myRTC(6, 7, 8);

byte degree_symbol[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

const int buttonPin = 2;
const int buttonPin2 = 3;
int displayMode = 0;

int buttonState = HIGH;
int lastButtonState = HIGH;
int buttonState2 = HIGH;
int lastButtonState2 = HIGH;
unsigned long lastDebounceTime = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, degree_symbol);
  lcd.setCursor(0, 0);
  lcd.print(" Smart Desk Sys");

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  dht.begin();
  

  //myRTC.setDS1302Time(10, 15, 21, 7, 18, 9, 2022);
}

void loop() {
  int reading = digitalRead(buttonPin);
  int reading2 = digitalRead(buttonPin2);

    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        displayMode = (displayMode + 1) % 3;  // Toggle display mode between temperature, humidity, and time
      }
    }

    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      if (buttonState2 == LOW) {
        displayMode = (displayMode +2) % 3;  // Toggle display mode in the opposite direction
      }
    }

  if (displayMode == 0) {
    float temperature = dht.readTemperature();
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.write(1);
    lcd.print("C");
    lcd.print("    ");

  } else if (displayMode == 1) {
    float humidity = dht.readHumidity();
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
  } else if (displayMode == 2) {
    myRTC.updateTime();
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    if (myRTC.hours < 10) {
      lcd.print("0");
    }
    lcd.print(myRTC.hours);
    lcd.print(":");
    if (myRTC.minutes < 10) {
      lcd.print("0");
    }
    lcd.print(myRTC.minutes);
    lcd.print(":");
    if (myRTC.seconds < 10) {
      lcd.print("0");
    }
    lcd.print(myRTC.seconds);
    lcd.print("   ");
  } 
  lastButtonState = reading;
}
