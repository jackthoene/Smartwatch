#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include "WiFiCam.hpp"
#include "screen.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_task_wdt.h"

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

// Task handles
TaskHandle_t screenTaskHandle;
TaskHandle_t imuTaskHandle;
TaskHandle_t camTaskHandle;
TaskHandle_t webTaskHandle;

// Function prototypes
void IRAM_ATTR buttonInterrupt1();
void IRAM_ATTR buttonInterrupt2();
void screenTask(void *parameter);
void imuTask(void *parameter);
void camTask(void *parameter);
void webTask(void *parameter);

extern void screenloop();
extern void screensetup();
extern void imusetup();
extern void imuloop();
extern void websetup();
extern void webloop();
extern void camsetup();
extern void camloop();

void disableWDT()
{
  // Disable the Task Watchdog Timer for both cores
  esp_task_wdt_init(60, false);                         // Set timeout to 60 seconds, disable panic
  esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(0)); // Disable WDT on core 0
  esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(1)); // Disable WDT on core 1
}

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);
  counter = 0;

  // Disable watchdog timers
  disableWDT();

  screensetup();
  websetup();
  Serial.println("Web Finished");

  // Configure button pins
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);

  // Attach interrupts to the buttons
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), buttonInterrupt1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_2), buttonInterrupt2, FALLING);

  // Create tasks
  xTaskCreate(screenTask, "Screen Task", 2048, NULL, 1, &screenTaskHandle);
  xTaskCreate(imuTask, "IMU Task", 2048, NULL, 1, &imuTaskHandle);
  xTaskCreate(camTask, "Camera Task", 4096, NULL, 1, &camTaskHandle);
  xTaskCreate(webTask, "Web Task", 2048, NULL, 1, &webTaskHandle);
}

void loop()
{
  // Main loop is empty as tasks handle the functionality
}

// Interrupt service routine for Button 1
void IRAM_ATTR buttonInterrupt1()
{
  unsigned long currentTime = millis();
  if (currentTime - lastButton1PressTime > debounceDelay)
  {
    button1Pressed = true;
    lastButton1PressTime = currentTime;
    Serial.println("Button 1 pressed");
    counter = 1;
  }
  // Serial.println("Button 1 pressed");
  // counter = 1;
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
    counter = 0;
  }
  // counter = 0;
}

void screenTask(void *parameter)
{
  for (;;)
  {
    if (counter == 0)
    {
      screenloop();
      vTaskDelay(100 / portTICK_PERIOD_MS); // Delay to allow other tasks to run
    }
    else
    {
      vTaskDelay(10 / portTICK_PERIOD_MS); // Delay to yield task
    }
  }
}

void imuTask(void *parameter)
{
  for (;;)
  {
    if (counter == 1)
    {
      Serial.println("IMU Setup Started");
      imusetup();
      Serial.println("IMU Finished");
      counter = 2;
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); // Delay to yield task
  }
}

void camTask(void *parameter)
{
  for (;;)
  {
    if (counter == 2)
    {
      Serial.println("Camera Setup Started");
      camsetup();
      Serial.println("Camera Finished");
      counter = 3;
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); // Delay to yield task
  }
}

void webTask(void *parameter)
{
  for (;;)
  {
    if (counter == 3)
    {
      // Serial.println("Go to websetup");
      // websetup();
      Serial.println("websetup done, go to webloop");
      webloop();
      vTaskDelay(100 / portTICK_PERIOD_MS); // Delay to allow other tasks to run
    }
    else
    {
      vTaskDelay(10 / portTICK_PERIOD_MS); // Delay to yield task
    }
  }
}
