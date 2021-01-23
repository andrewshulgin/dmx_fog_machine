#include <Arduino.h>
#include <DMXSerial.h>

#define ENABLE_PIN 2
#define RED_PIN 6
#define GREEN_PIN 3
#define BLUE_PIN 5
#define FOG_PIN 4
#define RF_PIN 7

#define LINK_LED_PIN 13

#define RED_CH 509
#define GREEN_CH 510
#define BLUE_CH 511
#define FOG_CH 512

void setup() {
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(LINK_LED_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(FOG_PIN, OUTPUT);
  pinMode(RF_PIN, INPUT);

  digitalWrite(ENABLE_PIN, 0);
  digitalWrite(LINK_LED_PIN, 0);
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
  digitalWrite(FOG_PIN, 0);

  DMXSerial.init(DMXReceiver);

  DMXSerial.write(RED_CH, 0);
  DMXSerial.write(GREEN_CH, 0);
  DMXSerial.write(BLUE_CH, 0);
  DMXSerial.write(FOG_CH, 0);
}

void loop() {
  unsigned long lastPacket = DMXSerial.noDataSince();
  if (lastPacket < 5000) {
    digitalWrite(LINK_LED_PIN, 1);
    analogWrite(RED_PIN,  DMXSerial.read(RED_CH));
    analogWrite(GREEN_PIN, DMXSerial.read(GREEN_CH));
    analogWrite(BLUE_PIN, DMXSerial.read(BLUE_CH));
    digitalWrite(FOG_PIN, DMXSerial.read(FOG_CH) >= 127);
  } else {
    digitalWrite(LINK_LED_PIN, 0);
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
    digitalWrite(FOG_PIN, 0);
  }
}
