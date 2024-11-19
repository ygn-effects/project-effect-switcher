#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>


class DisplayManager {
  private:
    Adafruit_SSD1306 m_ssd1306;
    uint8_t m_width;
    uint8_t m_height;
    uint8_t m_newLine;
    uint8_t m_newTab;

  public:
    DisplayManager(uint8_t t_width, uint8_t t_height) :
      m_ssd1306(t_width, t_height, &Wire, -1),
      m_width(t_width),
      m_height(t_height) {
        m_newTab = calcTextWidth("A");
        m_newLine = calcTextHeight("A");
      }

    void setup();

    void reset();

    void render();

    uint16_t calcTextWidth(const char* t_text);

    uint16_t calcTextHeight(const char* t_text);

    uint8_t getWidth();

    uint8_t getNewLine();

    uint8_t getNewTab();

    uint8_t getMaxVisibleLines();

    void printItem(const char* t_text, uint8_t t_x, uint8_t t_y);

    void drawInvertedLine(uint8_t t_y);
};


