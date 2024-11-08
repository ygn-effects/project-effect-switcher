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
    const uint8_t c_newTab = 8;

    const uint8_t c_menuCursor = 26;
    const uint8_t c_loopOrder = 16;
    const uint8_t c_loopOrderSwap = 42;
    const uint8_t c_scrollDownArrow = 25;
    const uint8_t c_scrollUpArrow = 24;

    /// @brief Resets the cursor position to the top-left of the screen
    void resetCursor();

    /// @brief Moves the cursor to the next line
    void newLine();

    /// @brief Calculates the width of the text
    /// @param t_text The text to calculate width for
    /// @return The width in pixels
    uint16_t calcTextWidth(const char* t_text);

    /// @brief Calculates the height of the text
    /// @param t_text The text to calculate height for
    /// @return The height in pixels
    uint16_t calcTextHeight(const char* t_text);

    /// @brief Render a scroll indicator
    /// @param t_up True for up arrow, false for down arrow
    void renderScrollIndicator(bool t_up);

  public:
    /// @brief Construct a display object
    /// @param t_width Width of the display
    /// @param t_height Height of the display
    /// @param t_newline Line height
    /// @param t_headerOffset Offset for centering the header text vertically
    Display(uint8_t t_width, uint8_t t_height, uint8_t t_newline, uint8_t t_headerOffset) :
      m_ssd1306(t_width, t_height, &Wire, -1),
      m_width(t_width),
      m_height(t_height),
      m_newLine(t_newline),
      m_headerOffset(t_headerOffset) { };

    /// @brief Initialize the display (SSD1306 setup)
    void setup();

    /// @brief Clear the display
    void clear();

    /// @brief Render the display (send buffer to screen)
    void render();

    /// @brief Get the height of the display
    /// @return uint8_t Height of the display
    uint8_t getHeight();

    /// @brief Get the height of a new line
    /// @return uint8_t Height of a new line
    uint8_t getLineHeight();

    /// @brief Render a centered header at the top of the screen
    /// @param t_text The header text to render
    void renderHeader(const char* t_text);

    /// @brief Render two integers, centered in the middle of the screen, separated by '|'
    /// @param t_bank The bank character
    /// @param t_preset The preset number
    void renderBankAndPreset(char t_bank, uint8_t t_preset);

    /// @brief Render a list of items
    /// @param t_items Array of items to display
    /// @param t_itemCount Number of items in the array
    /// @param t_startIndex Index of the first visible item
    /// @param t_selectedIndex Index of the currently selected item
    void renderListMenu(const char* t_items[], uint8_t t_itemCount, uint8_t t_startIndex, uint8_t t_selectedIndex);

    /// @brief Render the loop order list
    /// @param t_loopOrders Array of loop order numbers
    /// @param t_loopStates Array of loop states (active/inactive)
    /// @param t_loopCount Total number of loops
    /// @param t_selectedIndex Currently selected loop index
    /// @param t_swappingMode Flag indicating if in swap selection mode
    void renderLoopOrderList(const uint8_t* t_loopIndexes, const uint8_t* t_loopOrders, const uint8_t* t_loopStates, uint8_t t_loopCount, uint8_t t_selectedIndex, bool t_swappingMode);

    /// @brief Render the MIDI message list
    /// @param t_types Array of MIDI message types
    /// @param t_channels Array of MIDI message channels
    /// @param t_dataByte1 Array of MIDI data bytes 1
    /// @param t_dataByte2 Array of MIDI data bytes 2
    /// @param t_messageCount Total number of MIDI messages
    /// @param t_selectedIndex Index of the currently selected item
    /// @param t_startIndex Index of the first visible item
    void renderMidiMessages(const uint8_t* t_types, const uint8_t* t_channels, const uint8_t* t_dataByte1, const uint8_t* t_dataByte2, uint8_t t_messageCount, uint8_t t_selectedIndex, uint8_t t_startIndex);

    /// @brief Render the
    /// @param t_fields MIDI messages properties
    /// @param t_values MIDI messages properties values
    /// @param t_fieldsCount MIDI messages properties cont (should be 4)
    /// @param t_selectedField Index of the current selected field
    /// @param t_fieldEditMode Field edit mode
    /// @param t_messageEditMode Message edit mode (true for edits and false for adds)
    void renderMidiMessageEdit(const char** t_fields, uint8_t* t_values, uint8_t t_fieldsCount, uint8_t t_selectedField, bool t_fieldEditMode, bool t_messageEditMode);
};

