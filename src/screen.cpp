#include "screen.h"

// Global TFT display instance
TFT_eSPI tft = TFT_eSPI();

void screensetup(void)
{
    // Initialize TFT display
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // Set the initial time
    setTime(1, 43, 0, 1, 1, 2024);
}

void screenloop(void)
{
    // Display gradient background
    for (int i = 0; i < tft.height(); i++)
    {
        tft.setTextColor(TFT_GRAY);
        uint16_t color = tft.color565(i, 0, 128 - i);
        tft.drawFastHLine(0, i, tft.width(), color);
    }

    // Display current time
    char buf[10];
    sprintf(buf, "%02d:%02d:%02d", hour(), minute(), second());
    tft.fillRect(0, 60, tft.width(), 40, TFT_BLACK); // Adjusted the width to tft.width()
    tft.setTextColor(TFT_GRAY);
    tft.drawString(buf, tft.width() / 2 + 2, 80 + 2);
    tft.setTextColor(TFT_WHITE);
    tft.setFreeFont(&FreeSansBold24pt7b);
    tft.drawString(buf, tft.width() / 2, 80);

    delay(1000); // Delay added to prioritize display
    yield();     // Allow background tasks to run
}
