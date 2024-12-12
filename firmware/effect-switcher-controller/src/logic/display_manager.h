#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <ST7789_AVR.h>
#include <Adafruit_GFX.h>


class DisplayManager {
  private:
    ST7789_AVR m_st7789;
    uint16_t m_width;
    uint16_t m_height;
    uint8_t m_newLine;
    uint8_t m_newTab;
    uint8_t m_fontSize = 2;

  public:
    DisplayManager(uint16_t t_width, uint16_t t_height, uint8_t t_cs, uint8_t t_dc, uint8_t t_rst) :
      m_st7789(t_dc, t_rst, t_cs),
      m_width(t_width),
      m_height(t_height) {
        m_st7789.setTextSize(m_fontSize);
        m_newTab = calcTextWidth("A");
        m_newLine = calcTextHeight("A");
      }

    void setup();

    void reset();

    void render();

    uint16_t calcTextWidth(const char* t_text);

    uint16_t calcTextHeight(const char* t_text);

    uint16_t getWidth();

    uint16_t getHeight();

    uint8_t getNewLine();

    uint8_t getNewTab();

    uint8_t getMaxVisibleLines();

    void printItem(const char* t_text, uint16_t t_x, uint16_t t_y);

    void printFullScreenItem(const char* t_text, uint16_t t_usableHeight, uint16_t t_usableWidth);

    void printHighlightedItem(const char* t_text, uint16_t t_x, uint16_t t_y);

    void drawInvertedLine(uint8_t t_y);
};


