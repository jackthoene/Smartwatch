#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include "WiFiCam.hpp"
#include "screen.h"

// Button pin definitions
#define BUTTON_PIN_1 32
#define BUTTON_PIN_2 33

// Counter for button presses
volatile int counter;
volatile bool button1Pressed = false;
volatile bool button2Pressed = false;
volatile unsigned long lastButton1PressTime = 0; // For debouncing
volatile unsigned long lastButton2PressTime = 0; // For debouncing
const unsigned long debounceDelay = 200;         // 200 milliseconds debounce delay

// Function prototypes
void IRAM_ATTR buttonInterrupt1();
void IRAM_ATTR buttonInterrupt2();

extern void screenloop();
extern void screensetup();
extern void imusetup();
extern void imuloop();
extern void websetup();
extern void webloop();
extern void camsetup();
extern void camloop();

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);
  counter = 0;

  screensetup();
  websetup();
  Serial.println("Web Finished");

  // Configure button pins
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);

  // Attach interrupts to the buttons
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), buttonInterrupt1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_2), buttonInterrupt2, FALLING);
}

void loop()
{
  // Check if button 1 was pressed
  if (button1Pressed)
  {
    button1Pressed = false; // Reset flag
    counter = 1;
  }

  // Check if button 2 was pressed
  if (button2Pressed)
  {
    button2Pressed = false; // Reset flag
    counter = 0;
  }

  // Main loop - runs the display or camera/web loops based on the counter
  while (counter == 0)
  {
    screenloop();
  }

  if (counter == 1)
  {
    Serial.println("Setup Started");
    imusetup();
    Serial.println("IMU Finished");
    camsetup();
    Serial.println("Cam Finished");

    Serial.println("Setup Finished");
    counter == 2;
  }
}

// Interrupt service routine for Button 1
void IRAM_ATTR buttonInterrupt1()
{
  unsigned long currentTime = millis();
  if (currentTime - lastButton1PressTime > debounceDelay)
  {
    button1Pressed = true;
    lastButton1PressTime = currentTime;
  }
  Serial.println("Button 1 pressed");
  counter = 1;
}

// Interrupt service routine for Button 2
void IRAM_ATTR buttonInterrupt2()
{
  unsigned long currentTime = millis();
  if (currentTime - lastButton2PressTime > debounceDelay)
  {
    button2Pressed = true;
    lastButton2PressTime = currentTime;
    Serial.println("Button 2 pressed");
  }
  counter = 0;
}
