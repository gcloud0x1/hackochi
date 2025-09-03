#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS 14
#define TFT_DC 27
#define TFT_RST 33
#define TFT_MOSI 23
#define TFT_MISO 19
#define TFT_CLK 18

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void bootMessage(const char *msg, uint16_t y, uint16_t color = ILI9341_GREEN, uint16_t delayMs = 800)
{
  tft.setCursor(8, y);
  tft.setTextColor(color);
  tft.print(msg);
  delay(delayMs);
}

void centerText(const char *msg, uint8_t textSize, int16_t y, uint16_t color = ILI9341_GREEN)
{
  tft.setTextSize(textSize);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);
  int16_t x = (tft.width() - w) / 2;
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.print(msg);
}

void setup()
{
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextSize(2);

  bootMessage("Booting HackOchi v0.1...", 5);
  delay(200);
  bootMessage("Initializing memory...", 25);
  bootMessage("HackOchi is waking up!", 45);

  delay(1200);

  tft.fillScreen(ILI9341_BLACK);
  centerText("HackOchi v0.1", 2, 30, ILI9341_GREEN);
  centerText("^_^", 4, 100, ILI9341_GREEN);
  centerText("Hello, Operator!", 2, 200, ILI9341_GREEN);
}

void loop()
{
}