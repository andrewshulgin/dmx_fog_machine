#include <Arduino.h>
#include <DMXSerial.h>
#include <RCSwitch.h>

#define TX_ENABLE_PIN 7

#define FOG_PIN 4
#define RED_PIN 6
#define GREEN_PIN 3
#define BLUE_PIN 5

#define DMX_SILENCE_THRESHOLD 1000
#define DMX_LINK_PIN 13

#define DMX_FOG_CH 512
#define DMX_RED_CH 509
#define DMX_GREEN_CH 510
#define DMX_BLUE_CH 511

#define RC_INT 0
#define RC_COMMAND_INTERVAL 200
#define RC_COMMAND_BASE 0x5d5500

#define RC_FOG 0b00110000
#define RC_RED 0b00000011
#define RC_GREEN 0b11000000
#define RC_BLUE 0b00001100

RCSwitch rcSwitch = RCSwitch();
unsigned long lastRcCommandTime;

void blackout() {
  digitalWrite(FOG_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

void setup() {
  pinMode(TX_ENABLE_PIN, OUTPUT);
  rcSwitch.enableReceive(RC_INT);

  pinMode(FOG_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(DMX_LINK_PIN, OUTPUT);

  digitalWrite(TX_ENABLE_PIN, LOW);
  digitalWrite(DMX_LINK_PIN, LOW);
  
  blackout();

  DMXSerial.init(DMXReceiver);

  DMXSerial.write(DMX_FOG_CH, 0);
  DMXSerial.write(DMX_RED_CH, 0);
  DMXSerial.write(DMX_GREEN_CH, 0);
  DMXSerial.write(DMX_BLUE_CH, 0);
}

void loop() {
  int dmxActive = DMXSerial.noDataSince() < DMX_SILENCE_THRESHOLD;
  if (dmxActive) {
    digitalWrite(DMX_LINK_PIN, HIGH);
    digitalWrite(FOG_PIN, DMXSerial.read(DMX_FOG_CH) >= 127);
    analogWrite(RED_PIN,  DMXSerial.read(DMX_RED_CH));
    analogWrite(GREEN_PIN, DMXSerial.read(DMX_GREEN_CH));
    analogWrite(BLUE_PIN, DMXSerial.read(DMX_BLUE_CH));
  } else if (rcSwitch.getReceivedValue() >= RC_COMMAND_BASE && rcSwitch.getReceivedValue() <= RC_COMMAND_BASE + 0xff) {
    lastRcCommandTime = millis();
    int command = rcSwitch.getReceivedValue() - RC_COMMAND_BASE;
    digitalWrite(FOG_PIN, command & RC_FOG);
    digitalWrite(RED_PIN, command & RC_RED);
    digitalWrite(GREEN_PIN, command & RC_GREEN);
    digitalWrite(BLUE_PIN, command & RC_BLUE);
    rcSwitch.resetAvailable();
  } else if (millis() - lastRcCommandTime > RC_COMMAND_INTERVAL) {
    blackout();
  }
}
