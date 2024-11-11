#include "display.h"

void Display::resetCursor() {
  m_ssd1306.setCursor(0, 0);
}

void Display::newLine() {
  uint16_t currentY = m_ssd1306.getCursorY();
  m_ssd1306.setCursor(0, currentY + m_newLine);
}

uint16_t Display::calcTextWidth(const char* t_text) {
  int16_t x1, y1;
  uint16_t textWidth, height;
  m_ssd1306.getTextBounds(t_text, 0, 0, &x1, &y1, &textWidth, &height);

  return textWidth;
}

uint16_t Display::calcTextHeight(const char* t_text) {
  int16_t x1, y1;
  uint16_t textWidth, height;
  m_ssd1306.getTextBounds(t_text, 0, 0, &x1, &y1, &textWidth, &height);

  return height;
}

void Display::renderScrollIndicator(bool t_up) {
  if (t_up) {
     // Top right corner under the header
    m_ssd1306.setCursor(m_width - 8, m_newLine);
    m_ssd1306.write(c_scrollUpArrow);
  } else {
    // Bottom right corner
    m_ssd1306.setCursor(m_width - 8, m_height - m_newLine);
    m_ssd1306.write(c_scrollDownArrow);
  }
}

void Display::setup() {
  // Initialize SSD1306 with I2C address 0x3C
  m_ssd1306.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  clear();
  render();
}

void Display::clear() {
  m_ssd1306.clearDisplay();
}

void Display::render() {
  m_ssd1306.display();
}

uint8_t Display::getHeight() {
  return m_height;
}

uint8_t Display::getLineHeight() {
  return m_newLine;
}

uint8_t Display::getMaxVisibleLines(bool t_includeHeader) {
  if (t_includeHeader) {
    return (m_height - m_newLine - m_headerOffset) / m_newLine;
  }
  else {
    return m_height / m_newLine;
  }
}

void Display::renderHeader(const char* t_text) {
  clear();
  resetCursor();
  // Reset to default font
  m_ssd1306.setFont();

  // White background for a full new line
  m_ssd1306.fillRect(0, 0, m_width, m_newLine, WHITE);
  // Invert colors for the header
  m_ssd1306.setTextColor(SSD1306_BLACK);
  // Center text horizontally and vertically within the header
  m_ssd1306.setCursor((m_width - calcTextWidth(t_text)) / 2, m_headerOffset);
  m_ssd1306.print(t_text);

  // Reset text color after the header
  m_ssd1306.setTextColor(SSD1306_WHITE);
}

void Display::renderBankAndPreset(char t_bank, uint8_t t_preset) {
  m_ssd1306.setFont(&SourceCodePro_Bold32pt7b);

  // Calculate the text width for 3 characters, each char is 6 pixels
  uint16_t textWidth = calcTextWidth("A-1");
  // Center the text horizontally and vertically in the space left beneath the header
  m_ssd1306.setCursor((m_width - textWidth) / 2, m_height - m_newLine / 2);

  // Print the bank, separator and preset
  m_ssd1306.print(t_bank);
  m_ssd1306.print("-");
  m_ssd1306.print(t_preset);

  render();
}

void Display::renderListMenu(const char* t_items[], uint8_t t_itemCount, uint8_t t_startIndex, uint8_t t_selectedIndex) {
  resetCursor();

  // Calculate how many items fit on the screen beneath the header
  uint8_t maxVisibleItems = (m_height - m_newLine) / m_newLine;

  // Adjust startIndex to ensure selectedIndex is in view
  if (t_selectedIndex >= t_startIndex + maxVisibleItems) {
    t_startIndex = t_selectedIndex - maxVisibleItems + 1;
  } else if (t_selectedIndex < t_startIndex) {
    t_startIndex = t_selectedIndex;
  }

  // Render each item
  for (uint8_t i = 0; i < maxVisibleItems && (t_startIndex + i) < t_itemCount; i++) {
    const char* item = t_items[t_startIndex + i];

    // Render the cursor in front of the selected item
    if (t_startIndex + i == t_selectedIndex) {
      m_ssd1306.setCursor(0, i * m_newLine + m_headerOffset + m_newLine);
      m_ssd1306.write(c_menuCursor);
    }

    m_ssd1306.setCursor(c_newTab, i * m_newLine + m_headerOffset + m_newLine);
    m_ssd1306.print(item);
  }

  // Render scroll indicators if needed
  if (t_startIndex > 0) {
    // Up indicator
    renderScrollIndicator(true);
  }
  if (t_startIndex + maxVisibleItems < t_itemCount) {
    // Down indicator
    renderScrollIndicator(false);
  }

  render();
}

void Display::renderLoopOrderList(const uint8_t* t_loopIndexes, const uint8_t* t_loopOrders, const uint8_t* t_loopStates, uint8_t t_loopCount, uint8_t t_selectedIndex, bool t_swappingMode) {
  // Center the loops list vertically within the whole,
  // it looks weird if the header is accounted due to the back button
  m_ssd1306.setCursor(0, (m_height / 2));

  // Print the loops list
  for (uint8_t i = 0; i < t_loopCount; i++) {
    if (t_loopStates[i]) {
      // Highlight the active loops by reversing the colors
      m_ssd1306.setTextColor(BLACK, WHITE);
      m_ssd1306.print(t_loopIndexes[i]);
      m_ssd1306.setTextColor(WHITE);
    }
    else {
      // Print the non active loops normally
      m_ssd1306.print(t_loopIndexes[i]);
    }

    // Print a -> after each loop but the last one
    if (i < t_loopCount - 1) {
      m_ssd1306.write(c_menuCursor);
    }
  }

  // When the selectedIndex is out of bound it means the back menu is selected.
  // Print the menu cursor in front of it.
  if (t_selectedIndex == t_loopCount) {
    m_ssd1306.setCursor(0, m_height - m_newLine);
    m_ssd1306.write(c_menuCursor);
  }
  else {
    // The width of a character is 6 pixels, so 12 to account for the arrows.
    // Print the cursor on top of the selected loop.
    m_ssd1306.setCursor((t_selectedIndex * 12), ((m_height - m_newLine * 2) / 2));
    {
      if (t_swappingMode) {
        // If swap is active print the * symbol
        m_ssd1306.write(c_loopOrderSwap);
      }
      else {
        // Regular cursor symbol
        m_ssd1306.write(c_scrollDownArrow);
      }
    }
  }

  // Print the back button at the bottom of the screen
  m_ssd1306.setCursor(c_newTab, m_height - m_newLine);
  m_ssd1306.print("Back");

  render();
}

void Display::renderMidiMessages(const uint8_t* t_types, const uint8_t* t_channels, const uint8_t* t_dataByte1, const uint8_t* t_dataByte2, uint8_t t_messageCount, uint8_t t_selectedIndex, uint8_t t_startIndex) {
  resetCursor();

  const uint8_t typeWidth = 46; // 7 chars width + some spacing
  const uint8_t channelWidth = 18; // 2 characters space + some spacing
  const uint8_t dataByteWidth = 24; // 3 characters + some spacing

  // Calculate how many items fit on the screen
  uint8_t maxVisibleItems = (m_height - m_newLine) / m_newLine;

  // Header
  // White background
  m_ssd1306.fillRect(0, 0, m_width, m_newLine, WHITE);
  // Invert colors for the header
  m_ssd1306.setTextColor(SSD1306_BLACK);
  // Center text horizontally within the header and start with a tab to account for the cursor
  m_ssd1306.setCursor(c_newTab, m_headerOffset);
  m_ssd1306.print("Type");
  // Center text horizontally within the header, add the previous columns and some spacing before the text
  m_ssd1306.setCursor(typeWidth + 2, m_headerOffset);
  m_ssd1306.print("Ch");
  // Center text horizontally within the header, add the previous columns and some spacing before the text
  m_ssd1306.setCursor(typeWidth + channelWidth + 2, m_headerOffset);
  m_ssd1306.print("B1");
  // Center text horizontally within the header, add the previous columns and some spacing before the text
    m_ssd1306.setCursor(typeWidth + channelWidth + dataByteWidth + 2, m_headerOffset);
  m_ssd1306.print("B2");
  // Reset text color after the header
  m_ssd1306.setTextColor(SSD1306_WHITE);

  // Adjust startIndex to ensure selectedIndex is in view
  if (t_selectedIndex >= t_startIndex + maxVisibleItems) {
    t_startIndex = t_selectedIndex - maxVisibleItems + 1;
  } else if (t_selectedIndex < t_startIndex) {
    t_startIndex = t_selectedIndex;
  }

  // Print each MIDI message
  for (uint8_t i = 0; i < maxVisibleItems && (t_startIndex + i) < t_messageCount; i++) {
    // Y position is i*m_newline under the header
    uint8_t cursorY = m_headerOffset + m_newLine + (i * m_newLine);
    uint8_t messageIndex = t_startIndex + i;

    // Print the cursor in front of the selected item
    if (messageIndex == t_selectedIndex) {
      m_ssd1306.setCursor(0, cursorY);
      m_ssd1306.write(c_menuCursor);
    }

    // Type
    m_ssd1306.setCursor(c_newTab, cursorY);
    switch (t_types[messageIndex]) {
      case 0xB0:
        m_ssd1306.print("CC");
        break;

      default:
        break;
    }

    // Channel
    m_ssd1306.setCursor(typeWidth + 2, cursorY);
    m_ssd1306.print(t_channels[messageIndex]);

    // Data byte 1
    m_ssd1306.setCursor(typeWidth + channelWidth + 2, cursorY);
    m_ssd1306.print(t_dataByte1[messageIndex]);

    // Data byte 2
    m_ssd1306.setCursor(typeWidth + channelWidth + dataByteWidth + 2, cursorY);
    m_ssd1306.print(t_dataByte2[messageIndex]);
  }

  // Check if there's room for "Add New Message" below the existing messages
  if (t_messageCount < maxVisibleItems) {
    uint8_t cursorY = m_headerOffset + m_newLine + (t_messageCount * m_newLine);

    // Display "Add New Message" with the menu cursor in front if it’s selected
    if (t_selectedIndex == t_messageCount) {
      m_ssd1306.setCursor(0, cursorY);
      m_ssd1306.write(c_menuCursor);
    }
    m_ssd1306.setCursor(c_newTab, cursorY);
    m_ssd1306.print("Add");
  }

  // Check if there's room for "Back" below the existing messages and new message button
  if (t_messageCount + 1 < maxVisibleItems) {
    uint8_t cursorY = m_headerOffset + m_newLine + (t_messageCount * m_newLine + m_newLine);

    // Display "Add New Message" with the menu cursor in front if it’s selected
    if (t_selectedIndex == t_messageCount + 1) {
      m_ssd1306.setCursor(0, cursorY);
      m_ssd1306.write(c_menuCursor);
    }
    m_ssd1306.setCursor(c_newTab, cursorY);
    m_ssd1306.print("Back");
  }

  // Render scroll indicators if needed
  if (t_startIndex > 0) {
    // Up indicator
    renderScrollIndicator(true);
  }
  if (t_startIndex + maxVisibleItems < t_messageCount) {
    // Down indicator
    renderScrollIndicator(false);
  }

  render();
}

void Display::renderMidiMessageEdit(const char** t_fields, uint8_t* t_values, uint8_t t_fieldsCount, bool t_hasDataByte2, uint8_t t_selectedField, bool t_fieldEditMode, bool t_messageEditMode) {
  const uint8_t fieldWidth = 60; // Set value for the fields column

  // Render the fields
  for (uint8_t i = 0; i < t_fieldsCount; i++) {
    uint8_t cursorY = m_headerOffset + m_newLine + (i * m_newLine);

    // Print the cursor in front of the selected item
    if (t_selectedField == i) {
      m_ssd1306.setCursor(0, cursorY);
      m_ssd1306.write(c_menuCursor);
    }

    if (i == 3) {
      if (t_hasDataByte2) {
        m_ssd1306.setCursor(c_newTab, cursorY);
        m_ssd1306.print(t_fields[i]);
      }
    }
    else {
      // Print the field
      m_ssd1306.setCursor(c_newTab, cursorY);
      m_ssd1306.print(t_fields[i]);
    }

    // Print the value
    m_ssd1306.setCursor(fieldWidth, cursorY);
    // Inverted color for field value if in field edit mode
    if (t_selectedField == i && t_fieldEditMode) {
      m_ssd1306.setTextColor(BLACK, WHITE);
    }

    // MIDI message type field
    if (i == 0) {
      switch (t_values[i])
      {
        case 0xB0:
          m_ssd1306.print("CC");
          break;

        case 0xC0:
          m_ssd1306.print("PC");
          break;

        default:
          break;
      }
    }
    else if (i == 3) {
      if (t_hasDataByte2) {
        m_ssd1306.print(t_values[i]);
      }
    }
    // Other fields
    else {
      m_ssd1306.print(t_values[i]);
    }

    m_ssd1306.setTextColor(SSD1306_WHITE); // Reset text color
  }

  // Print Cancel or Delete button underneath the existing fields depending on the editMode
  m_ssd1306.setCursor(c_newTab, m_headerOffset + m_newLine + (4 * m_newLine));
  if (t_messageEditMode) {
    m_ssd1306.print("Delete"); // Edit mode
  }
  else {
    m_ssd1306.print("Cancel"); // Add mode
  }

  // Print the cursor in front of the button if it is selected
  if (t_selectedField == 4) {
    m_ssd1306.setCursor(0, m_headerOffset + m_newLine + (4 * m_newLine));
    m_ssd1306.write(c_menuCursor);
  }

  // Print the save button underneath the cancel button
  m_ssd1306.setCursor(c_newTab, m_headerOffset + m_newLine + (5 * m_newLine));
  m_ssd1306.print("Save");

  // Print the cursor in front of the button if it is selected
  if (t_selectedField == 5) {
    m_ssd1306.setCursor(0, m_headerOffset + m_newLine + (5 * m_newLine));
    m_ssd1306.write(c_menuCursor);
  }

  render();
}