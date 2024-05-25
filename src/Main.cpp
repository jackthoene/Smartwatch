#include <TFT_eSPI.h>
#include <TimeLib.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include "WiFiCam.hpp"

// Define color constants
#define TFT_GRAY tft.color565(128, 128, 128)

// TFT display instance
TFT_eSPI tft = TFT_eSPI();

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

  // Initialize TFT display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Set the initial time
  setTime(1, 43, 0, 1, 1, 2024);

  // Configure button pins
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);

  // Attach interrupts to the buttons
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), buttonInterrupt1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_2), buttonInterrupt2, FALLING);

  // Initialize other modules
  imusetup();
  camsetup();
  websetup();
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
    // Display gradient background
    for (int i = 0; i < 128; i++)
    {
      tft.setTextColor(TFT_GRAY);
      uint16_t color = tft.color565(i, 0, 128 - i);
      tft.drawFastHLine(0, i, tft.width(), color);
    }

    // Display current time
    char buf[10];
    sprintf(buf, "%02d:%02d:%02d", hour(), minute(), second());
    tft.fillRect(0, 60, 240, 40, TFT_BLACK);
    tft.setTextColor(TFT_GRAY);
    tft.drawString(buf, tft.width() / 2 + 2, 80 + 2);
    tft.setTextColor(TFT_WHITE);
    tft.setFreeFont(&FreeSansBold24pt7b);
    tft.drawString(buf, tft.width() / 2, 80);

    delay(1000); // Delay added to prioritize display
    yield();     // Allow background tasks to run
  }

  // Run camera loop when counter is 1
  if (counter == 1)
  {
    imuloop(); // Prioritize screen over camera loop
    yield();   // Allow background tasks to run
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
