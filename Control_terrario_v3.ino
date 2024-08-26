
#include <TimeLib.h>
#include <Time.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include "max6675.h"
#include <DHT.h>


Adafruit_ST7789 lcd = Adafruit_ST7789(28, 43, 20, 21, 42);

int thermoDO = 51;
int thermoCS = 50;
int thermoCLK = 52;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

int buzzPin = 3;



#define SSR_PIN  9

#define DHTPIN 4
#define DHTTYPE DHT21
DHT dht(DHTPIN, DHTTYPE);

//Variables

#define LEDPWM 6
int rel_hum;
float amb_temp;
float real_temp;
float set_temp;

void setup() {
  Serial.begin(250000);
  setTime(20, 1, 0, 19, 04, 2023);   
  dht.begin();
  pinMode(LEDPWM, OUTPUT);
  pinMode(SSR_PIN, OUTPUT);
  digitalWrite(SSR_PIN, HIGH);    // When HIGH, the SSR is Off 
  rel_hum = dht.readHumidity();
  amb_temp = dht.readTemperature();
  real_temp = thermocouple.readCelsius();  
  lcd.init(240, 320, SPI_MODE0);
  lcd.setRotation(3);
  lcd.setTextSize(4);
  lcd.fillScreen(ST77XX_BLACK);
  lcd.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  delay(500);  
}

void loop() {   
  rel_hum = dht.readHumidity();
  amb_temp = dht.readTemperature();
  real_temp = thermocouple.readCelsius();
  
  

Serial.println(amb_temp);
  if (int(amb_temp) < 28) {
     Serial.println("primer if");
    switch (int(amb_temp)) {
      case 23:
        set_temp = 33;
        Serial.println("33 grados");
        break;
      case 24:
      Serial.println("32 grados");
        set_temp = 32;
        break;
      case 25:
        set_temp = 31;
        break;
      case 26:
        set_temp = 30;
        break;
      case 27:
        set_temp = 29;
        break;
      default:
      Serial.println("ninguna <28");
    }
  } else if (int(amb_temp) >= 28) {    
    set_temp = 0;
  } else if (int(amb_temp) < 23) {
    switch (int(amb_temp)) {
      case 22:
        set_temp = 34;
        break;
      case 21:
        set_temp = 35;
        break;
      case 20:
        set_temp = 36;
        break;
      case 19:
        set_temp = 37;
        break;
      case 18:
        set_temp = 38;
        break;
     default:
        Serial.println("ninguna <23"); 
    }
  }

 Serial.println("Antes del buzz");
  if (real_temp >= set_temp+4) {

    tone(buzzPin, 2000, 400);

  } else {

    noTone(buzzPin);

  }


  if (real_temp < set_temp) {

    digitalWrite(SSR_PIN, LOW);

  } else {

    digitalWrite(SSR_PIN, HIGH);

  }
  analogWrite(LEDPWM, 200);
  if (int(hour()) >= 9 && int(hour()) <= 19) {
    Serial.println("entre 9-19 " + LEDPWM);
    analogWrite(LEDPWM, 200);
  } else {
    Serial.println(LEDPWM + hour() + "entre 9-19 ");
    analogWrite(LEDPWM, 0);
  }
 
  if (int(hour()) == 8) {
    switch (int(minute())) {
      case 1:
        analogWrite(LEDPWM, 50);
        break;
      case 16:
        analogWrite(LEDPWM, 100);
        break;
      case 31:
        analogWrite(LEDPWM, 150);
        break;
      case 46:
        analogWrite(LEDPWM, 200);
        break;
    }
  } else if (int(hour()) == 20) {
    switch (int(minute())) {
      case 1:
        analogWrite(LEDPWM, 200);
        break;
      case 16:
        analogWrite(LEDPWM, 150);
        break;
      case 31:
        analogWrite(LEDPWM, 100);
        break;
      case 46:
        analogWrite(LEDPWM, 50);
        break;
    }
  }

  lcd.setCursor(0, 0);
  lcd.print("Set : ");
  lcd.println(set_temp,0);

  lcd.print("M ");
  lcd.print((char)247);
  lcd.print("C: ");
  lcd.println(real_temp, 1);

  lcd.print("A ");
  lcd.print((char)247);
  lcd.print("C: ");
  lcd.println(amb_temp, 1);

  lcd.print("Hr  : ");
  lcd.print(rel_hum, 1);
  lcd.println(" %");

  lcd.println();
  lcd.print(hour());
  lcd.print(":");
  lcd.print(minute());
  


}
