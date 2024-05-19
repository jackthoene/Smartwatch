#include <TFT_eSPI.h>
#include <TimeLib.h>
#include "Position.h"

#define TFT_GRAY tft.color565(128, 128, 128)
TFT_eSPI tft = TFT_eSPI();

#define BUTTON_PIN_1 32
#define BUTTON_PIN_2 33
volatile int counter = 0;

// Function prototypes
void buttonInterrupt1();
void buttonInterrupt2();

void setup(void)
{
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  setTime(1, 43, 0, 1, 1, 2024);

  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), buttonInterrupt1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_2), buttonInterrupt2, FALLING);
}

void loop()
{
  while (counter == 0)
  {
    for (int i = 0; i < 128; i++)
    {
      tft.setTextColor(TFT_GRAY);
      uint16_t color = tft.color565(i, 0, 128 - i);
      tft.drawFastHLine(0, i, tft.width(), color);
    }

    char buf[10];
    sprintf(buf, "%02d:%02d:%02d", hour(), minute(), second());

    tft.fillRect(0, 60, 240, 40, TFT_BLACK);

    tft.setTextColor(TFT_GRAY);
    tft.drawString(buf, tft.width() / 2 + 2, 80 + 2);

    tft.setTextColor(TFT_WHITE);
    tft.setFreeFont(&FreeSansBold24pt7b);
    tft.drawString(buf, tft.width() / 2, 80);

    delay(1000);
  }

  while (counter == 1)
  {

    imusetup();
    delay(300);
    imuloop();
  }
}

void buttonInterrupt1()
{
  Serial.println("Button 1 pressed");
  counter = 1;
}

void buttonInterrupt2()
{
  Serial.println("Button 2 pressed");
}
