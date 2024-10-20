#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#include "fonts/SourceCodePro_Bold32pt7b.h"

class Display {
  private:
    Adafruit_SSD1306 m_ssd1306;
    uint8_t m_width;
    uint8_t m_height;
    uint8_t m_newLine;
    uint8_t m_headerOffset;

    /// @brief Resets the cursor position to the top-left of the screen
    void resetCursor();

    /// @brief Moves the cursor to the next line
    void newLine();

    /// @brief Calculates the width of the text
    /// @param text The text to calculate width for
    /// @return The width in pixels
    uint16_t calcTextWidth(const char* text);

  public:
    /// @brief Construct a display object
    /// @param width Width of the display
    /// @param height Height of the display
    /// @param newline Line height
    /// @param headeroffset Offset for centering the header text vertically
    Display(uint8_t width, uint8_t height, uint8_t newline, uint8_t headeroffset) :
      m_ssd1306(width, height, &Wire, -1),
      m_width(width),
      m_height(height),
      m_newLine(newline),
      m_headerOffset(headeroffset) { };

    /// @brief Initialize the display (SSD1306 setup)
    void setup();

    /// @brief Clear the display
    void clear();

    /// @brief Render the display (send buffer to screen)
    void render();

    /// @brief Render a centered header at the top of the screen
    /// @param text The header text to render
    void renderHeader(const char* text);

    /// @brief Render two integers, centered in the middle of the screen, separated by '|'
    /// @param bank The bank character
    /// @param preset The preset number
    void renderBankAndPreset(char bank, uint8_t preset);
};

